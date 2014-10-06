#pragma once

#include <GraphicsEngine/ShiftEngine.h>
#include <MathLib/math.h>

#define UNDO_MAX 10

struct Block													//описывает структуру кубика в модели
{
	Block()	: exist(false){}
	Vector3F color;												//цвет задается трехмерными координатам
	bool exist;													//флаг - существует ли здесь кубик
};

struct wVertex													//служебное для движка
{
	wVertex(const Vector3F & pos, const Vector3F & normal, const Vector2F & tex, const Vector3F & col, float _AOFactor) 
		: Position(pos), Normal(normal), Texcoord(tex), Color(col), AOFactor(_AOFactor) {};
	Vector3F Position;
	Vector3F Normal;
	Vector2F Texcoord;
	Vector3F Color;
	float	AOFactor;
};

class cWorkspace												//отвечает за работу с моделью
{
public:
	cWorkspace(int _x, int _y, int _z);							//конструктор принимает размеры модели по трем осям
	~cWorkspace();

	void Initialize();

	void ResizeWithoutSaved(int x_size, int y_size, int z_size);//выполняет изменение размера, но не сохраняет объект внутри
	void Update();												//рисует модель
	Block & GetElem(int x, int y, int z);						//возвращает ссылку на блок, стоящий в координатах XYZ (внутри рабочей области)
	Block GetElem(int x, int y, int z) const;
	void SetNotTesselated();									//служебная, необходима для того, чтобы модель была перестроена
	void Undo();												//выполняет отмену последнего действия (всего 10 отмен)
	void SetColor(Vector3F & color);							//устанавливает цвет кисти, которой ставятся кубики

	void ShowBoundingBox();										//показывает BBox рабочей области
	void HideBoundingBox();										//скрывает (позже объясню)
	bool IsBBoxShowed() const;									//возвращает флаг

	Vector3F GetHalfSize() const;								//возвращает полуразмер рабочей области

	void Save(const std::wstring & filename);					//сохраняет модель в файл
	void Load(const std::wstring & filename);					//загружает

	int GetCurAction() const;

	void SaveToUndo();											//не помню что это)

	void Resize(int Xup, int Yup, int Zup, int Xdown, int Ydown, int Zdown); //
	int GetMaxSize() const;

	void VanishColor(bool flag);

private:
	void Tesselate();											//выполняет тесселяцию объекта (преобразование из кубиков в сетку)
	int GetIndex(int x, int y, int z) const;					//возвращает индекс, переводя координаты трехмерного массива в одномерные
	int GetIndexNew(int x, int y, int z, int xSize, int ySize, int zSize);
	inline float GetAOFactor(float x1, float x2, 
							float y1, float y2, 
							float z1, float z2);
	void CreatePlane();											//создает плоскость под моделью
	void CreateBBox();											//создает BBox

	bool tesselated;											//true if already tesselated
	int x_size, y_size, z_size;									//размер рабочей области
	Block * Elements;											//массив элементов (одномерный)

	ShiftEngine::MeshNode * bbox;
	ShiftEngine::MeshNode * plane;
	ShiftEngine::MeshNode * mesh;

	ShiftEngine::Material ColorMaterial;
	ShiftEngine::Material GeometryMaterial;
	ShiftEngine::TexturePtr GridTexture;

	//массив "отмен" для Ctlr+Z операции	
	Block * ElementsUndo[UNDO_MAX];
	void createUndo(int sizeElementsUndo);

	//флаг - показывается ли BBox
	bool bboxShow;

	int curAction;
};

