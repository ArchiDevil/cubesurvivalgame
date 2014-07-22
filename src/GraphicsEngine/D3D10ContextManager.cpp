#include "D3D10ContextManager.h"

#include "../Utilities/logger.h"
#include "../Utilities/ut.h"

#include <sstream>

#include <D3Dcompiler.h>

ShiftEngine::D3D10ContextManager::D3D10ContextManager(HWND hwnd)
	: m_HWND(hwnd), CurrentRasterizerState(RS_Normal), ZBufferState(true), CurrentBlendingState(BS_None), Culling(true)
{
	//MOVE TO ICONTEXTMANAGER CONSTRUCTOR
	defaultVertexSemantic.addSemantic(ET_FLOAT, 3, ES_Position);
	defaultVertexSemantic.addSemantic(ET_FLOAT, 3, ES_Normal);
	defaultVertexSemantic.addSemantic(ET_FLOAT, 2, ES_Texcoord);

	extendedVertexSemantic.addSemantic(ET_FLOAT, 3, ES_Position);
	extendedVertexSemantic.addSemantic(ET_FLOAT, 3, ES_Normal);
	extendedVertexSemantic.addSemantic(ET_FLOAT, 3, ES_Texcoord);
	extendedVertexSemantic.addSemantic(ET_FLOAT, 1, ES_Color);

	colorVertexSemantic.addSemantic(ET_FLOAT, 3, ES_Position);
	colorVertexSemantic.addSemantic(ET_FLOAT, 3, ES_Normal);
	colorVertexSemantic.addSemantic(ET_FLOAT, 1, ES_Color);

	plainSpriteVertexSemantic.addSemantic(ET_FLOAT, 2, ES_Position);
	plainSpriteVertexSemantic.addSemantic(ET_FLOAT, 2, ES_Texcoord);
	//plainSpriteVertexSemantic.addVertexSemantic(ET_FLOAT, 1, ES_Color);
}

ShiftEngine::D3D10ContextManager::~D3D10ContextManager() 
{
}

bool ShiftEngine::D3D10ContextManager::Initialize( GraphicEngineSettings _Settings, ShiftEngine::PathSettings _Paths )
{
	Settings = _Settings;
	Paths = _Paths;

	if(Paths.FontsPath.empty() || Paths.MaterialsPath.empty() || Paths.MeshPath.empty() || Paths.ShaderPath.empty() || Paths.TexturePath.empty())
		MainLog.Error("Some settings paths are not filled");

	DXGI_SWAP_CHAIN_DESC desc;
	ZeroMemory(&desc, sizeof(DXGI_SWAP_CHAIN_DESC));
	desc.BufferDesc.Format					= DXGI_FORMAT_R8G8B8A8_UNORM;			//формат буферов
	desc.BufferDesc.Height					= Settings.screenHeight;			//высота
	desc.BufferDesc.Width					= Settings.screenWidth;			//ширина
	desc.BufferDesc.Scaling					= DXGI_MODE_SCALING_UNSPECIFIED;		//масштабирование?
	desc.BufferDesc.ScanlineOrdering		= DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;	//развертка строк?
	desc.BufferDesc.RefreshRate.Denominator	= 1;									//знаменатель?
	desc.BufferDesc.RefreshRate.Numerator	= Settings.screenRate;			//частота обновления экрана

	desc.SampleDesc.Count	= Settings.multisampleQuality;	//мультисемплинг 
	desc.SampleDesc.Quality	= 0;									//качество его

	desc.BufferCount	= 1;								//кол-во задних буферов - один, за глаза
	desc.BufferUsage	= DXGI_USAGE_RENDER_TARGET_OUTPUT;	//использование буфера
	desc.Flags			= 0;								//флаги
	desc.OutputWindow	= m_HWND;
	desc.SwapEffect		= DXGI_SWAP_EFFECT_DISCARD;
	desc.Windowed		= Settings.windowed;

	unsigned int Flags = 0;
#if defined (DEBUG) || (_DEBUG)
	Flags = D3D10_CREATE_DEVICE_DEBUG;
#endif

	if(FAILED(D3D10CreateDeviceAndSwapChain(
		NULL,						//номер девайса для рендера - 1й, потому что одна видеокарта используется
		D3D10_DRIVER_TYPE_HARDWARE,	//тип девайса
		NULL,						//включить софт растеризатор
		Flags,						//для дебага создаем дебаговое устройство, для релиза нормальное
		D3D10_SDK_VERSION,			//версия сдк - всегда такое
		&desc,						//указатель на описание
		&Context.SwapChain,			//указатель на своп чейн
		&Context.Device				//указатель на устройство
		)))
		MainLog.FatalError("Unable to create DirectX 10 device");

	MainLog.Message("Device created");

	ID3D10Texture2D * tempTex = nullptr;

	Context.SwapChain->GetBuffer(0, __uuidof(ID3D10Texture2D), reinterpret_cast<void**>(&tempTex));
	Context.Device->CreateRenderTargetView(tempTex, 0, &Context.DefaultRT->rt);
	tempTex->Release();

	/////////////////////////////////////
	// Создание депт и стенсил буферов //
	/////////////////////////////////////

	D3D10_TEXTURE2D_DESC depthStencilDesc;										//описание глубинно-стенсильного буфера
	depthStencilDesc.Width				= Settings.screenWidth;					//ширина
	depthStencilDesc.Height				= Settings.screenHeight;				//высота
	depthStencilDesc.MipLevels			= 1;									//мип-уровни
	depthStencilDesc.ArraySize			= 1;									//размер текстуры
	depthStencilDesc.Format				= DXGI_FORMAT_D24_UNORM_S8_UINT;		//формат
	depthStencilDesc.SampleDesc.Count	= Settings.multisampleQuality;			//multisampling must match
	depthStencilDesc.SampleDesc.Quality	= 0;									//swap chain values
	depthStencilDesc.Usage				= D3D10_USAGE_DEFAULT;					//использование
	depthStencilDesc.BindFlags			= D3D10_BIND_DEPTH_STENCIL;				//флаги, куда прибиндивать
	depthStencilDesc.CPUAccessFlags		= 0;									//доступ процессора
	depthStencilDesc.MiscFlags			= 0;									//прочие флаги

	Context.Device->CreateTexture2D(&depthStencilDesc, 0, &Context.DepthStencilBuffer);
	Context.Device->CreateDepthStencilView(Context.DepthStencilBuffer, 0, &Context.DepthStencilView);
	Context.Device->OMSetRenderTargets(1, &Context.DefaultRT->rt, Context.DepthStencilView); //установка рендер-таргетов

	D3D10_VIEWPORT vp;
	vp.TopLeftX	= 0;
	vp.TopLeftY	= 0;
	vp.Width	= Settings.screenWidth;
	vp.Height	= Settings.screenHeight;
	vp.MinDepth	= 0.0f;
	vp.MaxDepth	= 1.0f;

	Context.Device->RSSetViewports(1, &vp);

	if(!Context.CreateStates())
		MainLog.FatalError("Unable to create states for renderer");

	ZBufferState = true;
	Context.Device->OMSetDepthStencilState(Context.dsStateZOn, 1);
	Context.Device->RSSetState(Context.rsNormal);
	const float BlendFactor[4] = {1.0f, 1.0f, 1.0f, 1.0f};
	Context.Device->OMSetBlendState(Context.bsNormal, BlendFactor, 0xffffffff);

	ShaderManager = new D3D10ShaderManager(Context.Device);
	ShaderGenerator = new D3D10ShaderGenerator();
	MeshManager = new cMeshManager(Context.Device);
	TextureManager = new D3D10TextureManager();
	TextureManager->Initialize(Context.Device, Paths.TexturePath);
	pMaterialManager = new MaterialManager(TextureManager, ShaderManager);

	pFntMng = new FontManager();

	Context.Device->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	//HACK: LOADING SAMPLERS
	D3D10_SAMPLER_DESC sDesc;
	sDesc.AddressU = D3D10_TEXTURE_ADDRESS_CLAMP;
	sDesc.AddressV = D3D10_TEXTURE_ADDRESS_CLAMP;
	sDesc.AddressW = D3D10_TEXTURE_ADDRESS_CLAMP;
	sDesc.BorderColor[0] = sDesc.BorderColor[1] = sDesc.BorderColor[2] = sDesc.BorderColor[3] = 1.0f;
	sDesc.ComparisonFunc = D3D10_COMPARISON_GREATER_EQUAL;
	if(Settings.anisotropyLevel > 0)
	{
		sDesc.Filter = D3D10_FILTER_ANISOTROPIC;
		sDesc.MaxAnisotropy = Settings.anisotropyLevel;
	}
	else
	{
		sDesc.Filter = D3D10_FILTER_MIN_MAG_MIP_LINEAR;
		sDesc.MaxAnisotropy = 0;
	}
	sDesc.MaxLOD = D3D10_FLOAT32_MAX;
	sDesc.MinLOD = 0.0f;
	sDesc.MipLODBias = 0.0f;
	ID3D10SamplerState * sampler = nullptr;
	HRESULT hr = Context.Device->CreateSamplerState(&sDesc, &sampler);
	if(FAILED(hr))
	{
		MainLog.FatalError("Unable to create samplers");
		return false;
	}
	Context.Device->PSSetSamplers(0, 1, &sampler);

	RegisterVertexSemantic(defaultVertexSemantic);
	RegisterVertexSemantic(extendedVertexSemantic);
	RegisterVertexSemantic(colorVertexSemantic);
	RegisterVertexSemantic(plainSpriteVertexSemantic);

	return true;
}

std::wstring ShiftEngine::D3D10ContextManager::GetVideocardDesc()
{
	DXGI_ADAPTER_DESC adapterDesc;
	IDXGIFactory * factory;
	IDXGIAdapter * adapter;
	// Create a DirectX graphics interface factory.
	CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&factory);
	// Use the factory to create an adapter for the primary graphics interface (video card).
	factory->EnumAdapters(0, &adapter);
	adapter->GetDesc(&adapterDesc);
	std::wstring buffer = adapterDesc.Description;
	factory->Release();
	adapter->Release();
	return buffer;
}

void ShiftEngine::D3D10ContextManager::Shutdown()
{
	pFntMng->Shutdown();
	//KILL EVERYTHING
}

void ShiftEngine::D3D10ContextManager::BeginScene()
{
	Context.Device->ClearRenderTargetView(Context.DefaultRT->rt, D3DXCOLOR((float)208/255, (float)238/255, (float)248/255, 1.0f));
	Context.Device->ClearDepthStencilView(Context.DepthStencilView,	D3D10_CLEAR_DEPTH | D3D10_CLEAR_STENCIL, 1.0f, 0);
}

void ShiftEngine::D3D10ContextManager::EndScene()
{
	if(Settings.screenRate > 0)
		Context.SwapChain->Present(1, 0);
	else
		Context.SwapChain->Present(0, 0);
}

void ShiftEngine::D3D10ContextManager::ResetPipeline()
{
	UINT null = 0;
	ID3D10Buffer* nullB = nullptr;
	Context.Device->IASetVertexBuffers (0, 1, &nullB, &null, &null);
	Context.Device->IASetIndexBuffer(nullB, DXGI_FORMAT_UNKNOWN, NULL);
	Context.Device->IASetInputLayout(nullptr);
	currentVertexDeclaration = nullptr;
	currentProgram = nullptr;
}

ShiftEngine::TexturePtr ShiftEngine::D3D10ContextManager::LoadTexture( const std::wstring & FileName )
{
	TexturePtr out = TextureManager->CreateTexture2D(FileName);
	if(out == TextureManager->GetErrorTexture())
		MainLog.Error("Unable to load " + utils::WStrToStr(FileName) + ", loaded empty texture.");
	return out;
}

ShiftEngine::IProgramPtr ShiftEngine::D3D10ContextManager::LoadShader( const std::wstring & FileName )
{
	return ShaderManager->CreateProgramFromFile(Paths.ShaderPath + FileName);
}

ShiftEngine::MeshDataPtr ShiftEngine::D3D10ContextManager::LoadMesh( const std::wstring & FileName )
{
	MeshDataPtr out = MeshManager->LoadMesh(Paths.MeshPath + FileName);
	if(out == nullptr)
		MainLog.Error("Unable to load: " + utils::WStrToStr(FileName));
	return out;
}

ShiftEngine::MaterialPtr ShiftEngine::D3D10ContextManager::LoadMaterial( const std::wstring & FileName, const std::wstring & mtlName )
{
	auto ptr = pMaterialManager->LoadMaterial(Paths.MaterialsPath + FileName, mtlName);

	if(ptr == nullptr)
	{
		MainLog.Error("Unable to load " + utils::WStrToStr(FileName));
		return pMaterialManager->LoadMaterial(Paths.MaterialsPath + L"error.mtl");
	}

	return ptr;
}

void ShiftEngine::D3D10ContextManager::SetZState( bool enabled )
{
	ZBufferState = enabled;
	if(enabled)
		Context.Device->OMSetDepthStencilState(Context.dsStateZOn, 1);
	else
		Context.Device->OMSetDepthStencilState(Context.dsStateZOff, 1);
}

ID3D10Device * ShiftEngine::D3D10ContextManager::GetDevicePointer()
{
	return Context.Device;
}

ShiftEngine::GraphicEngineSettings ShiftEngine::D3D10ContextManager::GetParameters() const
{
	return Settings;
}

ShiftEngine::PathSettings ShiftEngine::D3D10ContextManager::GetPaths() const
{
	return Paths;
}

int ShiftEngine::D3D10ContextManager::DrawMesh( MeshDataPtr & mesh )
{
	if(mesh && mesh->vertexDeclaration)
	{
		if(mesh->vertexDeclaration.get() != currentVertexDeclaration)
		{
			mesh->vertexDeclaration->Bind();
			currentVertexDeclaration = mesh->vertexDeclaration.get();
		}
		return mesh->Draw(Context.Device);
	}
	else
	{
		return 0;
	}
}

ShiftEngine::D3D10ShaderManager * ShiftEngine::D3D10ContextManager::GetShaderManager()
{
	return ShaderManager;
}

ShiftEngine::D3D10ShaderGenerator * ShiftEngine::D3D10ContextManager::GetShaderGenerator()
{
	return ShaderGenerator;
}

void ShiftEngine::D3D10ContextManager::SetBlendingState( BlendingState bs )
{
	const float BlendFactor[4] = {1.0f, 1.0f, 1.0f, 1.0f};

	switch (bs)
	{
	case ShiftEngine::BS_None:
		Context.Device->OMSetBlendState(Context.bsNormal, BlendFactor, 0xffffffff);
		break;
	case ShiftEngine::BS_AlphaEnabled:
		Context.Device->OMSetBlendState(Context.bsAlpha, BlendFactor, 0xffffffff);
		break;
	case ShiftEngine::BS_Additive:
		Context.Device->OMSetBlendState(Context.bsAdditive, BlendFactor, 0xffffffff);
		break;
	default:
		break;
	}

	CurrentBlendingState = bs;
}

ShiftEngine::BlendingState ShiftEngine::D3D10ContextManager::GetBlendingState() const
{
	return CurrentBlendingState;
}

void ShiftEngine::D3D10ContextManager::SetRasterizerState( ShiftEngine::RasterizerState rs )
{
	CurrentRasterizerState = rs;
	switch (rs)
	{
	case ShiftEngine::RS_Wireframe:
		Context.Device->RSSetState(Context.rsWireframe);
		break;
	case ShiftEngine::RS_Normal:
		Context.Device->RSSetState(Context.rsNormal);
		break;
	case ShiftEngine::RS_NoCulling:
		Context.Device->RSSetState(Context.rsNoCulling);
		break;
	default:
		break;
	}
}

ShiftEngine::RasterizerState ShiftEngine::D3D10ContextManager::GetRasterizerState() const
{
	return CurrentRasterizerState;
}

ShiftEngine::FontManager * ShiftEngine::D3D10ContextManager::GetFontManager()
{
	return pFntMng;
}

void ShiftEngine::D3D10ContextManager::RegisterVertexSemantic( VertexSemantic & semantic )
{
	auto iter = Declarations.find(semantic);
	if(iter == Declarations.end())
	{
		auto vd = CreateVDFromDescription(semantic);
		Declarations[semantic] = std::make_shared<D3D10VertexDeclaration>(vd);
	}
	else
	{
		MainLog.Message("Semantic is already registered");
	}
}

ShiftEngine::D3D10VertexDeclaration ShiftEngine::D3D10ContextManager::CreateVDFromDescription( VertexSemantic & semantic )
{
	ID3D10Device * pDevice = Context.Device;
	ID3D10InputLayout * outIL = nullptr;

	const size_t bufferSize = 2048;
	char shaderCode[bufferSize] = {};
	std::stringstream stream;

	auto repr = semantic.getInnerRepresentation();

	//writing shader
	for (size_t i = 0; i < repr.size() ; i++)
	{
		switch (repr[i].semantic)
		{
		case ShiftEngine::ES_Position:
			repr[i].name = "POSITION";
			break;
		case ShiftEngine::ES_Normal:
			repr[i].name = "NORMAL";
			break;
		case ShiftEngine::ES_Texcoord:
			repr[i].name = "TEXCOORD";
			break;
		case ShiftEngine::ES_Color:
			repr[i].name = "COLOR";
			break;
		case ShiftEngine::ES_Binormal:
			repr[i].name = "BINORMAL";
			break;
		case ShiftEngine::ES_Tangent:
			repr[i].name = "TANGENT";
			break;
		case ShiftEngine::ES_Custom:
			break;	//name should be specified by user
		default:
			throw;
		}

		if(repr[i].count > 4)
		{
			MainLog.Error("Unable to create input layout for vertex declaration. Reason - wrong byteWidth in vd description. Value is" + std::to_string(repr[i].count));
			outIL = nullptr;
		}
		stream << "float" << repr[i].count << " in_" << i << " : " << repr[i].name << ";\n";
	}

	sprintf_s(shaderCode, "struct vs_in {\n %s }; struct vs_out {float4 Pos : SV_Position;}; vs_out f(vs_in input){vs_out o; o.Pos = float4(0.0f, 0.0f, 0.0f, 0.0f); return o;};", stream.str().c_str());
	//end of writing shader

	ID3D10Blob * compiledShader = nullptr;	//sue this signature to validate Input Layout
	ID3D10Blob * errorMessages = nullptr;
	if(FAILED(D3DCompile(shaderCode, bufferSize, NULL, NULL, NULL, "f", "vs_4_0", 0, 0, &compiledShader, &errorMessages)))
	{
		MainLog.Error(std::string((char*)errorMessages->GetBufferPointer()));
		MainLog.FatalError("Internal fatal error");
		outIL = nullptr;
	}

	D3D10_INPUT_ELEMENT_DESC * ilDesc = new D3D10_INPUT_ELEMENT_DESC[repr.size()];
	unsigned int align = 0;

	for (size_t i = 0; i < repr.size() ; i++)
	{
		//TEMP
		switch (repr[i].semantic)
		{
		case ShiftEngine::ES_Position:
			ilDesc[i].SemanticName = "POSITION";
			break;
		case ShiftEngine::ES_Normal:
			ilDesc[i].SemanticName = "NORMAL";
			break;
		case ShiftEngine::ES_Texcoord:
			ilDesc[i].SemanticName = "TEXCOORD";
			break;
		case ShiftEngine::ES_Color:
			ilDesc[i].SemanticName = "COLOR";
			break;
		case ShiftEngine::ES_Binormal:
			ilDesc[i].SemanticName = "BINORMAL";
			break;
		case ShiftEngine::ES_Tangent:
			ilDesc[i].SemanticName = "TANGENT";
			break;
		case ShiftEngine::ES_Custom:
		default:
			delete [] ilDesc;
			ilDesc = nullptr;
			throw;
		}
		//TEMP

		ilDesc[i].AlignedByteOffset = align;
		if(repr[i].count == 1)
			ilDesc[i].Format = DXGI_FORMAT_R32_FLOAT;
		if(repr[i].count == 2)
			ilDesc[i].Format = DXGI_FORMAT_R32G32_FLOAT;
		if(repr[i].count == 3)
			ilDesc[i].Format = DXGI_FORMAT_R32G32B32_FLOAT;
		if(repr[i].count == 4)
			ilDesc[i].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
		ilDesc[i].InputSlot = 0;
		ilDesc[i].InputSlotClass = D3D10_INPUT_PER_VERTEX_DATA;
		ilDesc[i].InstanceDataStepRate = 0;
		ilDesc[i].SemanticIndex = 0;
		align += repr[i].count * 4;
	}

	if(FAILED(pDevice->CreateInputLayout(ilDesc, repr.size(), compiledShader->GetBufferPointer(), compiledShader->GetBufferSize(), &outIL)))
	{
		delete [] ilDesc;
		compiledShader->Release();
		outIL = nullptr;
		MainLog.FatalError("Unable to create input layout");
		return nullptr;
	}

	delete [] ilDesc;
	compiledShader->Release();

	D3D10VertexDeclaration vd(outIL, pDevice);

	return vd;
}

ShiftEngine::D3D10VDPtr ShiftEngine::D3D10ContextManager::GetVertexDeclaration( const VertexSemantic & semantic )
{
	auto iter = Declarations.find(semantic);
	if(iter == Declarations.end())
	{
		return nullptr;
	}
	else
	{
		return iter->second;
	}
}

ShiftEngine::D3D10TextureManager * ShiftEngine::D3D10ContextManager::GetTextureManager()
{
	return TextureManager;
}
