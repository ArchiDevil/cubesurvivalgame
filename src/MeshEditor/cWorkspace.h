#pragma once

#include <GraphicsEngine/ShiftEngine.h>
#include <MathLib/math.h>

#define UNDO_MAX 10

struct Block													//��������� ��������� ������ � ������
{
	Block()	: exist(false){}
	Vector3F color;												//���� �������� ����������� �����������
	bool exist;													//���� - ���������� �� ����� �����
};

struct wVertex													//��������� ��� ������
{
	wVertex(const Vector3F & pos, const Vector3F & normal, const Vector2F & tex, const Vector3F & col, float _AOFactor) 
		: Position(pos), Normal(normal), Texcoord(tex), Color(col), AOFactor(_AOFactor) {};
	Vector3F Position;
	Vector3F Normal;
	Vector2F Texcoord;
	Vector3F Color;
	float	AOFactor;
};

class cWorkspace												//�������� �� ������ � �������
{
public:
	cWorkspace(int _x, int _y, int _z);							//����������� ��������� ������� ������ �� ���� ����
	~cWorkspace();

	void Initialize();

	void ResizeWithoutSaved(int x_size, int y_size, int z_size);//��������� ��������� �������, �� �� ��������� ������ ������
	void Update();												//������ ������
	Block & GetElem(int x, int y, int z);						//���������� ������ �� ����, ������� � ����������� XYZ (������ ������� �������)
	Block GetElem(int x, int y, int z) const;
	void SetNotTesselated();									//���������, ���������� ��� ����, ����� ������ ���� �����������
	void Undo();												//��������� ������ ���������� �������� (����� 10 �����)
	void SetColor(Vector3F & color);							//������������� ���� �����, ������� �������� ������

	void ShowBoundingBox();										//���������� BBox ������� �������
	void HideBoundingBox();										//�������� (����� �������)
	bool IsBBoxShowed() const;									//���������� ����

	Vector3F GetHalfSize() const;								//���������� ���������� ������� �������

	void Save(const std::wstring & filename);					//��������� ������ � ����
	void Load(const std::wstring & filename);					//���������

	int GetCurAction() const;

	void SaveToUndo();											//�� ����� ��� ���)

	void Resize(int Xup, int Yup, int Zup, int Xdown, int Ydown, int Zdown); //
	int GetMaxSize() const;

	void VanishColor(bool flag);

private:
	void Tesselate();											//��������� ���������� ������� (�������������� �� ������� � �����)
	int GetIndex(int x, int y, int z) const;					//���������� ������, �������� ���������� ����������� ������� � ����������
	int GetIndexNew(int x, int y, int z, int xSize, int ySize, int zSize);
	inline float GetAOFactor(float x1, float x2, 
							float y1, float y2, 
							float z1, float z2);
	void CreatePlane();											//������� ��������� ��� �������
	void CreateBBox();											//������� BBox

	bool tesselated;											//true if already tesselated
	int x_size, y_size, z_size;									//������ ������� �������
	Block * Elements;											//������ ��������� (����������)

	ShiftEngine::MeshNode * bbox;
	ShiftEngine::MeshNode * plane;
	ShiftEngine::MeshNode * mesh;

	ShiftEngine::Material ColorMaterial;
	ShiftEngine::Material GeometryMaterial;
	ShiftEngine::TexturePtr GridTexture;

	//������ "�����" ��� Ctlr+Z ��������	
	Block * ElementsUndo[UNDO_MAX];
	void createUndo(int sizeElementsUndo);

	//���� - ������������ �� BBox
	bool bboxShow;

	int curAction;
};

