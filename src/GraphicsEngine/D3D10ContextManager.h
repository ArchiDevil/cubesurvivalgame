#pragma once

#include <exception>
#include <vector>

#include "cD3D10.h"
#include "Program.h"
#include "cMesh.h"
#include "MiscTypes.h"
#include "ShaderManager.h"
#include "ShaderGenerator.h"
#include "D3D10TextureManager.h"
#include "cMeshManager.h"
#include "MaterialManager.h"
#include "Sprite.h"
#include "cRenderTarget.h"
#include "TextLib/cText.h"
#include "cMaterial.h"
#include "D3D10VertexDeclaration.h"
#include "FontManager.h"

using MathLib::Vector2F;
using MathLib::vec3;
using MathLib::vec4;

namespace ShiftEngine
{
	enum BlendingState
	{
		BS_None,
		BS_AlphaEnabled,
		BS_Additive,
	};
	
	enum RasterizerState
	{
		RS_Wireframe,
		RS_Normal,
		RS_NoCulling,
	};

	class D3D10ContextManager
	{
		friend class Renderer;
	public:
		D3D10ContextManager(HWND hwnd);
		~D3D10ContextManager();

		virtual bool						Initialize( GraphicEngineSettings _Settings, PathSettings _Paths );

		std::wstring						GetVideocardDesc();

		void								Shutdown();
		void								BeginScene();
		void								EndScene();
		void								ResetPipeline();


		TexturePtr							LoadTexture(const std::wstring & FileName);
		MaterialPtr							LoadMaterial(const std::wstring & FileName, const std::wstring & mtlName);
		IProgramPtr							LoadShader(const std::wstring & FileName);
		MeshDataPtr							LoadMesh(const std::wstring & FileName);

		D3D10ShaderManager *				GetShaderManager();
		D3D10ShaderGenerator *				GetShaderGenerator();

		void								SetZState(bool enabled);
		void								SetBlendingState(BlendingState bs);
		BlendingState						GetBlendingState() const;
		void								SetRasterizerState(RasterizerState rs);
		RasterizerState						GetRasterizerState() const;

		ID3D10Device *						GetDevicePointer();
		D3D10TextureManager *				GetTextureManager();

		GraphicEngineSettings				GetParameters() const;
		PathSettings						GetPaths() const;

		int									DrawMesh(MeshDataPtr & mesh);

		FontManager*						GetFontManager();

		void								RegisterVertexSemantic(VertexSemantic & semantic);
		D3D10VDPtr							GetVertexDeclaration(const VertexSemantic & semantic);

		D3DMATRIX							GetOrthoMatrix() const;

	private:
		ShiftEngine::D3D10VertexDeclaration CreateVDFromDescription( VertexSemantic & semantic );

		HWND								m_HWND;
		cD3D10								Context;
		PathSettings						Paths;
		GraphicEngineSettings				Settings;
		D3D10VDPtr							CurrentVD;
		IProgramPtr							currentProgram;
		FontManager*						pFntMng;
		//default render target
		//current shader
		//something more

		std::map<VertexSemantic, D3D10VDPtr>	Declarations;

		D3D10TextureManager	*				TextureManager;
		cMeshManager *						MeshManager;
		D3D10ShaderManager *				ShaderManager;
		D3D10ShaderGenerator *				ShaderGenerator;
		MaterialManager *					pMaterialManager;

		RasterizerState						CurrentRasterizerState;
		bool								ZBufferState;
		BlendingState						CurrentBlendingState;
		bool								Culling;

		D3D10VertexDeclaration *			currentVertexDeclaration;
	};

}	//end of ShiftEngine namespace
