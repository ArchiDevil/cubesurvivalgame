#pragma once

#include <string>
#include <cstdint>

#include "../MathLib/math.h"

#define LIM_HEADER_VERSION 2

namespace ShiftEngine
{
	//форматы рендер-таргетов для движка
	enum RTFormats
	{
		Format_RGBA8,
		Format_R32,
	};

	enum class TextureType : uint8_t
	{
		Unknown,
		Texture2D,
		Texture2DArray,
		TextureCubemap,
		Texture3D
	};

	//хидер для файла с моделями
	struct MeshLIMHeader
	{
		MeshLIMHeader(int ver = 0, 
			bool hn = false, 
			bool htc = false, 
			unsigned int vc = 0, 
			unsigned int ic = 0) 
			: VERSION(ver), hasNormals(hn), hasTexCoords(htc), verticesCount(vc), indicesCount(ic) {};
		unsigned int VERSION;
		bool hasNormals;
		bool hasTexCoords;
		unsigned int verticesCount;
		unsigned int indicesCount;
	};

	//прямоугольная область
	struct gRect
	{
		float left;
		float top;
		float right;
		float bottom;
	};

	//параметры графического движка
	struct GraphicEngineSettings
	{
		GraphicEngineSettings()
			: screenWidth(800)
			, screenHeight(600)
			, multisampleQuality(0)
			, windowed(true)
			, screenRate(0)
			, zNear(0.1f)
			, zFar(200.0f)
			, anisotropyLevel(0)
		{}

		int screenWidth;
		int screenHeight;
		int multisampleQuality;
		bool windowed;
		int screenRate;
		float zNear;
		float zFar;
		int anisotropyLevel;
	};

	//структура хранит относительные пути для загрузки ресурсов
	struct PathSettings
	{
		std::wstring MeshPath;
		std::wstring TexturePath;
		std::wstring ShaderPath;
		std::wstring FontsPath;
		std::wstring MaterialsPath;
	};

} // end of ShiftEngine namespace
