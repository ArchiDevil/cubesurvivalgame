#include "cWorkspace.h"

#include <cassert>
#include <vector>

#include <Utilities/ut.h>

#define AO_DEBUG

cWorkspace::cWorkspace( int _x, int _y, int _z )
	: x_size(_x), y_size(_y), z_size(_z), curAction(0), bboxShow(false),
	bbox(nullptr), plane(nullptr), mesh(nullptr), tesselated(false)
{
	assert(x_size > 0 || y_size > 0 || z_size > 0);
	Elements = new Block[x_size * y_size * z_size];
	
	for (int i = 0; i < UNDO_MAX; i++)
	{
		ElementsUndo[i] = new Block[x_size * y_size * z_size];
		for (int j = 0; j < x_size * y_size * z_size ; j++)
			ElementsUndo[i][j] = Block();
	}
}

cWorkspace::~cWorkspace()
{
	delete [] Elements;

	for (int i = 0; i < UNDO_MAX ; i++)
		delete [] ElementsUndo[i];
}

void cWorkspace::Initialize()
{
	Tesselate();
	CreateBBox();
	CreatePlane();
	MainLog.Message("Workspace initialized");
}

void cWorkspace::ResizeWithoutSaved( int new_x, int new_y, int new_z )
{
	x_size = new_x;
	y_size = new_y;
	z_size = new_z;

	delete [] Elements;
	Elements = new Block[new_x * new_y * new_z];

	createUndo(new_x * new_y * new_z);

	curAction = 0;
}

void cWorkspace::Resize(int Xup, int Yup, int Zup, int Xdown, int Ydown, int Zdown)
{
	int x_new = x_size + Xup + Xdown;
	int y_new = y_size + Yup + Ydown;
	int z_new = z_size + Zup + Zdown;
	
	Block * ElementsNew = new Block [x_new * y_new * z_new];
	
	for(int i = 0; i < x_size; i++)	
	{
		for(int j = 0; j < y_size; j++)
		{
			for(int k = 0; k < z_size; k++)
			{
				int indexnew = GetIndexNew(i + Xdown, j + Ydown, k + Zdown, x_new, y_new, z_new);
				if(GetElem(i, j, k).exist)
					ElementsNew[indexnew] = GetElem(i, j, k);
			}
		}
	}

	delete [] Elements;
	this->Elements = ElementsNew;

	x_size = x_new;
	y_size = y_new;
	z_size = z_new;

	CreateBBox();
	createUndo(x_size * y_size * z_size);
}

int cWorkspace::GetIndexNew(int x, int y, int z, int xSize, int ySize, int zSize)
{
	return x * zSize * ySize + y * zSize + z;
}

void cWorkspace::createUndo(int sizeElementsUndo)
{
	for (int i = 0; i < UNDO_MAX ; i++)
	{
		delete [] ElementsUndo[i];
		ElementsUndo[i] = new Block[sizeElementsUndo];
	}
}

void cWorkspace::Update()
{
	if(!tesselated)
		Tesselate();

	plane->SetScale(Vector3F(x_size * 2.0f, y_size * 2.0f, 1.0f));
	plane->SetPosition(Vector3F((float)x_size / 2 - (float)x_size, (float)y_size / 2 - (float)y_size, 0.0f));

	//if(bboxShow)
	//	bbox->SetVisibility(true);
	//else
	//	bbox->SetVisibility(false);
}

Block & cWorkspace::GetElem( int x, int y, int z )
{
	if(x < x_size && x >= 0 && y < y_size && y >= 0 && z < z_size && z >= 0)
	{
		tesselated = false;
		return Elements[x * z_size * y_size + y * z_size + z];
	}
	else if(z < 0)
	{
		Block hack;
		hack.exist = true;
		return hack;
	}
	else
	{
		return Block();
	}
}

Block cWorkspace::GetElem( int x, int y, int z ) const
{
	if(x < x_size && x >= 0 && y < y_size && y >= 0 && z < z_size && z >= 0)
	{
		return Elements[x * z_size * y_size + y * z_size + z];
	}
	else if(z < 0)
	{
		Block hack;
		hack.exist = true;
		return hack;
	}
	else
	{
		return Block();
	}
}

#define maxShadowFactor2 23.1364880

float cWorkspace::GetAOFactor( float x1, float x2, float y1, float y2, float z1, float z2 )
{
#ifdef AO_DEBUG
	//checking maximum distance here
	
	const float maxDist = 2.1213f;
	float xc = x1 + abs(x1 - x2) / 2.0f;
	float yc = y1 + abs(y1 - y2) / 2.0f;
	float zc = z1 + abs(z1 - z2) / 2.0f;
	const Vector3F center = Vector3F(xc, yc, zc);
	float shadowFactor = 0.0f;
	// ONLY FOR 2 RADIUS AND 2.1213 MAX DISTANCE
	//float maxShadowFactor = maxShadowFactor2;

	//calculating maximum shadow factor
	float maxShadowFactor = 0.0f;
	for (float i = x1; i <= x2; i++)
		for (float j = y1; j <= y2; j++)
			for (float k = z1; k <= z2; k++)
				maxShadowFactor += pow(MathLib::distance(center, Vector3F(i, j, k)) / maxDist, 2);

	for (float i = x1; i <= x2; i++)
		for (float j = y1; j <= y2; j++)
			for (float k = z1; k <= z2; k++)
				if(GetElem(i, j, k).exist)
					shadowFactor += pow(MathLib::distance(center, Vector3F(i, j, k)) / maxDist, 2);

	//dividing
	return 1.0f - shadowFactor / maxShadowFactor;
#else
	return 1.0f;
#endif
}

void cWorkspace::Tesselate()
{
	ShiftEngine::MeshData * cd = new ShiftEngine::MeshData;
	std::vector<wVertex> vertices;
	std::vector<long> indices;
	int ind_index = 0;

	const Vector3F FRONT	= Vector3F(-1.0f, 0.0f, 0.0f);
	const Vector3F BACK		= Vector3F(1.0f, 0.0f, 0.0f);
	const Vector3F LEFT		= Vector3F(0.0f, -1.0f, 0.0f);
	const Vector3F RIGHT	= Vector3F(0.0f, 1.0f, 0.0f);
	const Vector3F DOWN		= Vector3F(0.0f, 0.0f, -1.0f);
	const Vector3F UP		= Vector3F(0.0f, 0.0f, 1.0f);

	for (float x = 0.0f; x < (float)x_size ; x++)
	{
		for (float y = 0.0f; y < (float)y_size ; y++)
		{
			for (float z = 0.0f; z < (float)z_size ; z++)
			{
				auto block = GetElem((int)floor(x), (int)floor(y), (int)floor(z));
				if(block.exist)
				{
					//check around
					if(!GetElem((int)floor(x - 1), (int)floor(y), (int)floor(z)).exist)
					{
						float AOFactor = GetAOFactor(x - 1, x - 1, y - 2, y + 1, z - 2, z + 1);
						vertices.push_back(wVertex(Vector3F(x, y,		z),		FRONT, Vector2F(0.0f, 0.0f), block.color, AOFactor));

						AOFactor = GetAOFactor(x - 1, x - 1, y - 1, y + 2, z - 2, z + 1);
						vertices.push_back(wVertex(Vector3F(x, y + 1,	z),		FRONT, Vector2F(1.0f, 0.0f), block.color, AOFactor));

						AOFactor = GetAOFactor(x - 1, x - 1, y - 2, y + 1, z - 1, z + 2);
						vertices.push_back(wVertex(Vector3F(x, y,		z + 1),	FRONT, Vector2F(0.0f, 1.0f), block.color, AOFactor));

						AOFactor = GetAOFactor(x - 1, x - 1, y - 1, y + 2, z - 1, z + 2);
						vertices.push_back(wVertex(Vector3F(x, y + 1,	z + 1),	FRONT, Vector2F(1.0f, 1.0f), block.color, AOFactor));

						indices.push_back(ind_index + 0);
						indices.push_back(ind_index + 3);
						indices.push_back(ind_index + 2);
						indices.push_back(ind_index + 0);
						indices.push_back(ind_index + 1);
						indices.push_back(ind_index + 3);

						ind_index += 4;
					}					

					if(!GetElem((int)floor(x + 1), (int)floor(y), (int)floor(z)).exist)
					{
						float AOFactor = GetAOFactor(x + 1, x + 1, y - 2, y + 1, z - 2, z + 1);
						vertices.push_back(wVertex(Vector3F(x + 1, y,		z),		BACK, Vector2F(0.0f, 0.0f), block.color, AOFactor));

						AOFactor = GetAOFactor(x + 1, x + 1, y - 1, y + 2, z - 2, z + 1);
						vertices.push_back(wVertex(Vector3F(x + 1, y + 1,	z),		BACK, Vector2F(1.0f, 0.0f), block.color, AOFactor));

						AOFactor = GetAOFactor(x + 1, x + 1, y - 2, y + 1, z - 1, z + 2);
						vertices.push_back(wVertex(Vector3F(x + 1, y,		z + 1), BACK, Vector2F(0.0f, 1.0f), block.color, AOFactor));

						AOFactor = GetAOFactor(x + 1, x + 1, y - 1, y + 2, z - 1, z + 2);
						vertices.push_back(wVertex(Vector3F(x + 1, y + 1,	z + 1), BACK, Vector2F(1.0f, 1.0f), block.color, AOFactor));

						indices.push_back(ind_index + 0);
						indices.push_back(ind_index + 2);
						indices.push_back(ind_index + 3);
						indices.push_back(ind_index + 0);
						indices.push_back(ind_index + 3);
						indices.push_back(ind_index + 1);

						ind_index += 4;
					}

					if(!GetElem((int)floor(x), (int)floor(y - 1), (int)floor(z)).exist)
					{
						float AOFactor = GetAOFactor(x - 2, x + 1, y - 1, y - 1, z - 2, z + 1);
						vertices.push_back(wVertex(Vector3F(x,		y,	z),		LEFT, Vector2F(0.0f, 0.0f), block.color, AOFactor));

						AOFactor = GetAOFactor(x - 1, x + 2, y - 1, y - 1, z - 2, z + 1);
						vertices.push_back(wVertex(Vector3F(x + 1,	y,	z),		LEFT, Vector2F(1.0f, 0.0f), block.color, AOFactor));

						AOFactor = GetAOFactor(x - 2, x + 1, y - 1, y - 1, z - 1, z + 2);
						vertices.push_back(wVertex(Vector3F(x,		y,	z + 1),	LEFT, Vector2F(0.0f, 1.0f), block.color, AOFactor));

						AOFactor = GetAOFactor(x - 1, x + 2, y - 1, y - 1, z - 1, z + 2);
						vertices.push_back(wVertex(Vector3F(x + 1,	y,	z + 1),	LEFT, Vector2F(1.0f, 1.0f), block.color, AOFactor));

						indices.push_back(ind_index + 0);
						indices.push_back(ind_index + 2);
						indices.push_back(ind_index + 3);
						indices.push_back(ind_index + 0);
						indices.push_back(ind_index + 3);
						indices.push_back(ind_index + 1);

						ind_index += 4;
					}

					if(!GetElem((int)floor(x), (int)floor(y + 1), (int)floor(z)).exist)
					{
						float AOFactor = GetAOFactor(x - 2, x + 1, y + 1, y + 1, z - 2, z + 1);
						vertices.push_back(wVertex(Vector3F(x,		y + 1,	z),		RIGHT, Vector2F(0.0f, 0.0f), block.color, AOFactor));

						AOFactor = GetAOFactor(x - 1, x + 2, y + 1, y + 1, z - 2, z + 1);
						vertices.push_back(wVertex(Vector3F(x + 1,	y + 1,	z),		RIGHT, Vector2F(1.0f, 0.0f), block.color, AOFactor));

						AOFactor = GetAOFactor(x - 2, x + 1, y + 1, y + 1, z - 1, z + 2);
						vertices.push_back(wVertex(Vector3F(x,		y + 1,	z + 1),	RIGHT, Vector2F(0.0f, 1.0f), block.color, AOFactor));

						AOFactor = GetAOFactor(x - 1, x + 2, y + 1, y + 1, z - 1, z + 2);
						vertices.push_back(wVertex(Vector3F(x + 1,	y + 1,	z + 1),	RIGHT, Vector2F(1.0f, 1.0f), block.color, AOFactor));

						indices.push_back(ind_index + 0);
						indices.push_back(ind_index + 3);
						indices.push_back(ind_index + 2);
						indices.push_back(ind_index + 0);
						indices.push_back(ind_index + 1);
						indices.push_back(ind_index + 3);

						ind_index += 4;
					}

					if(!GetElem((int)floor(x), (int)floor(y), (int)floor(z - 1)).exist)
					{
						float AOFactor = GetAOFactor(x - 2, x + 1, y - 2, y + 1, z + 1, z - 1);
						vertices.push_back(wVertex(Vector3F(x,		y,		z),	DOWN, Vector2F(0.0f, 0.0f), block.color, AOFactor));

						AOFactor = GetAOFactor(x - 1, x + 2, y - 2, y + 1, z + 1, z - 1);
						vertices.push_back(wVertex(Vector3F(x + 1,	y,		z),	DOWN, Vector2F(1.0f, 0.0f), block.color, AOFactor));

						AOFactor = GetAOFactor(x - 2, x + 1, y - 1, y + 2, z + 1, z - 1);
						vertices.push_back(wVertex(Vector3F(x,		y + 1,	z),	DOWN, Vector2F(0.0f, 1.0f), block.color, AOFactor));

						AOFactor = GetAOFactor(x - 1, x + 2, y - 1, y + 2, z + 1, z - 1);
						vertices.push_back(wVertex(Vector3F(x + 1,	y + 1,	z),	DOWN, Vector2F(1.0f, 1.0f), block.color, AOFactor));

						indices.push_back(ind_index + 0);
						indices.push_back(ind_index + 3);
						indices.push_back(ind_index + 2);
						indices.push_back(ind_index + 0);
						indices.push_back(ind_index + 1);
						indices.push_back(ind_index + 3);

						ind_index += 4;
					}

					if(!GetElem((int)floor(x), (int)floor(y), (int)floor(z + 1)).exist)
					{
						float AOFactor = GetAOFactor(x - 2, x + 1, y - 2, y + 1, z + 1, z + 1);
						vertices.push_back(wVertex(Vector3F(x,		y,		z + 1),	UP, Vector2F(0.0f, 0.0f), block.color, AOFactor));

						AOFactor = GetAOFactor(x - 1, x + 2, y - 2, y + 1, z + 1, z + 1);
						vertices.push_back(wVertex(Vector3F(x + 1,	y,		z + 1),	UP, Vector2F(1.0f, 0.0f), block.color, AOFactor));

						AOFactor = GetAOFactor(x - 2, x + 1, y - 1, y + 2, z + 1, z + 1);
						vertices.push_back(wVertex(Vector3F(x,		y + 1,	z + 1),	UP, Vector2F(0.0f, 1.0f), block.color, AOFactor));

						AOFactor = GetAOFactor(x - 1, x + 2, y - 1, y + 2, z + 1, z + 1);
						vertices.push_back(wVertex(Vector3F(x + 1,	y + 1,	z + 1),	UP, Vector2F(1.0f, 1.0f), block.color, AOFactor));

						indices.push_back(ind_index + 0);
						indices.push_back(ind_index + 2);
						indices.push_back(ind_index + 3);
						indices.push_back(ind_index + 0);
						indices.push_back(ind_index + 3);
						indices.push_back(ind_index + 1);

						ind_index += 4;
					}
				}
			}
		}
	}

	cd->indicesCount = indices.size();
	cd->verticesCount = vertices.size();
	cd->vertexSize = sizeof(wVertex);

	cd->CreateBuffers(true, vertices.data(), sizeof(wVertex) * vertices.size(), 
						indices.data(), sizeof(long) * indices.size(), 
						ShiftEngine::GetContextManager()->GetDevicePointer());

	ShiftEngine::MeshDataPtr meshData = ShiftEngine::MeshDataPtr(cd);
	
	if(!mesh)
	{
		ShiftEngine::VertexSemantic *sem = new ShiftEngine::VertexSemantic;
		sem->addSemantic(ShiftEngine::ET_FLOAT, 3, ShiftEngine::ES_Position);
		sem->addSemantic(ShiftEngine::ET_FLOAT, 3, ShiftEngine::ES_Normal);
		sem->addSemantic(ShiftEngine::ET_FLOAT, 2, ShiftEngine::ES_Texcoord);
		sem->addSemantic(ShiftEngine::ET_FLOAT, 3, ShiftEngine::ES_Color);
		//i.push_back(ShiftEngine::InputElement("AO", 1));
		ShiftEngine::GetContextManager()->RegisterVertexSemantic(*sem);
		cd->vertexSemantic = sem;

		GridTexture = ShiftEngine::GetContextManager()->LoadTexture(L"gridCell.png");
		this->GeometryMaterial = ShiftEngine::Material(ShiftEngine::GetContextManager()->LoadShader(L"wsShaderGeometry.fx"));
		GeometryMaterial.SetDiffuseTexture(GridTexture);
		this->ColorMaterial = ShiftEngine::Material(ShiftEngine::GetContextManager()->LoadShader(L"wsShaderColor.fx"));

		mesh = ShiftEngine::GetSceneGraph()->AddMeshNode(meshData, MathLib::AABB(Vector3F(), Vector3F(x_size, y_size, z_size)), &GeometryMaterial);
	}
	else
	{
		mesh->SetDataPtr(meshData);
	}

	tesselated = true;
}

void cWorkspace::SetNotTesselated()
{
	tesselated = false;
}

void cWorkspace::Save( const std::wstring & filename )
{
	struct Header
	{
		unsigned int size[3];
	};
	
	Header h;
	h.size[0] = x_size;
	h.size[1] = y_size;
	h.size[2] = z_size;

	auto buff = filename;
	std::ofstream stream(buff);

	if(!stream || stream.fail())
		MainLog.Error("Unable to save " + utils::WStrToStr(buff));

	stream.write(reinterpret_cast<char *>(&h), sizeof(Header));

	for (int i = 0; i < x_size ; i++)
		for (int j = 0; j < y_size ; j++)
			for (int k = 0; k < z_size ; k++)
				stream.write(reinterpret_cast<char *>(&GetElem(i, j, k)), sizeof(Block));
}

void cWorkspace::Load( const std::wstring & filename )
{
	struct Header
	{
		unsigned int size[3];
	};

	Header h;

	auto buff = filename;
	std::ifstream stream(buff);

	if(!stream || stream.fail())
	{
		MainLog.Error("Unable to load " + utils::WStrToStr(buff));
		return;
	}

	stream.read(reinterpret_cast<char *>(&h), sizeof(Header));

	ResizeWithoutSaved(h.size[0], h.size[1], h.size[2]);

	for (int i = 0; i < x_size ; i++)
		for (int j = 0; j < y_size ; j++)
			for (int k = 0; k < z_size ; k++)
				stream.read(reinterpret_cast<char *>(&GetElem(i, j, k)), sizeof(Block));
}

void cWorkspace::SaveToUndo()
{
	int newPos = curAction % UNDO_MAX;

	for (int i = 0; i < x_size; i++)
	{
		for (int j = 0; j < y_size; j++)
		{
			for (int k = 0; k < z_size; k++)
			{
				ElementsUndo[newPos][GetIndex(i, j, k)] = Elements[GetIndex(i, j, k)];
			}
		}
	}
	curAction++;
}

int cWorkspace::GetIndex( int x, int y, int z ) const
{
	return x * z_size * y_size + y * z_size + z;
}

void cWorkspace::Undo()
{
	if(curAction-1 < 0)
		return;

	int newPos = (--curAction) % UNDO_MAX;

	for (int i = 0; i < x_size; i++)
		for (int j = 0; j < y_size; j++)
			for (int k = 0; k < z_size; k++)
				Elements[GetIndex(i, j, k)] = ElementsUndo[newPos][GetIndex(i, j, k)];

	Tesselate();
}

Vector3F cWorkspace::GetHalfSize() const
{
	return Vector3F((float)x_size / 2, (float)y_size / 2, (float)z_size / 2);
}

void cWorkspace::SetColor( Vector3F & color )
{
	for (int i = 0; i < x_size ; i++)
		for (int j = 0; j < y_size ; j++)
			for (int k = 0; k < z_size ; k++)
				GetElem(i, j, k).color = color;
}

void cWorkspace::CreatePlane()
{
	if(plane)
		return;

	ShiftEngine::MeshData * temp = new ShiftEngine::MeshData;

	temp->indicesCount = 6;
	temp->verticesCount = 4;
	temp->vertexSize = sizeof(ShiftEngine::DefaultVertex);
	ShiftEngine::DefaultVertex ver[] = 
	{
		{0.0f, 0.0f, 0.0f,		0.0f, 0.0f, 1.0f,		0.0f, 0.0f},
		{0.0f, 1.0f, 0.0f,		0.0f, 0.0f, 1.0f,		0.0f, 1.0f},
		{1.0f, 1.0f, 0.0f,		0.0f, 0.0f, 1.0f,		1.0f, 1.0f},
		{1.0f, 0.0f, 0.0f,		0.0f, 0.0f, 1.0f,		1.0f, 0.0f},
	};
	long ind[6] = {0, 1, 2, 0, 2, 3};

	temp->CreateBuffers(false, ver, sizeof(ShiftEngine::DefaultVertex) * 4,
		ind, sizeof(long) * 6, ShiftEngine::GetContextManager()->GetDevicePointer());

	ShiftEngine::Material PlaneShader;
	temp->vertexSemantic = &ShiftEngine::defaultVertexSemantic;
	PlaneShader.SetDiffuseTexture(ShiftEngine::GetContextManager()->LoadTexture(L"plane.png")); 
	plane = ShiftEngine::GetSceneGraph()->AddMeshNode(std::shared_ptr<ShiftEngine::MeshData>(temp), MathLib::AABB(Vector3F(), Vector3F(1.0f, 1.0f, 0.0f)), &PlaneShader);
}

void cWorkspace::CreateBBox()
{
	//if(!bbox)
	//{
	//	MeshDataPtr temp = ShiftEngine::utils::createCube(cGraphicsEngine::GetInstance().GetDevicePointer());
	//	std::vector<ShiftEngine::InputElement> i;
	//	i.push_back(ShiftEngine::InputElement("POSITION", 3));
	//	i.push_back(ShiftEngine::InputElement("NORMAL", 3));
	//	i.push_back(ShiftEngine::InputElement("TEXCOORD", 2));
	//	ShiftEngine::cMaterial BboxShader = ShiftEngine::cMaterial(cGraphicsEngine::GetInstance().LoadShader(L"bbox.fx", i));
	//	bbox = ShiftEngine::SceneGraphInstance->AddStaticMeshNode(temp, MathLib::AABB(Vector3F(), Vector3F(1.0f, 1.0f, 1.0f)), BboxShader);

	//	Vector3F newScale = Vector3F((float)x_size + 0.1f, (float)y_size + 0.1f, (float)z_size + 0.1f);
	//	bbox->SetScale(newScale);
	//	Vector3F hp = Vector3F(this->GetHalfSize().x, this->GetHalfSize().y, 0.0f);
	//	bbox->SetPosition(hp);
	//}
	//else
	//{
	//	Vector3F newScale = Vector3F((float)x_size + 0.1f, (float)y_size + 0.1f, (float)z_size + 0.1f);
	//	bbox->SetScale(newScale);
	//	Vector3F hp = Vector3F(this->GetHalfSize().x, this->GetHalfSize().y, 0.0f);
	//	bbox->SetPosition(hp);
	//}
}

void cWorkspace::ShowBoundingBox()
{
	bboxShow = true;
}

void cWorkspace::HideBoundingBox()
{
	bboxShow = false;
}

bool cWorkspace::IsBBoxShowed() const
{
	return bboxShow;
}
int cWorkspace:: GetCurAction() const
{
	return curAction;
}

int cWorkspace:: GetMaxSize() const
{
	return max(x_size, max(y_size, z_size));
}

void cWorkspace::VanishColor( bool flag )
{
	if(flag)
		mesh->SetMaterial(&GeometryMaterial);
	else
		mesh->SetMaterial(&ColorMaterial);
}
