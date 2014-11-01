#pragma once

#include <memory>
#include <map>
#include <vector>

#include "D3D10Texture.h"
#include "ShaderProgram.h"
#include "D3D10VertexDeclaration.h"

using std::map;
using std::string;
using std::vector;

namespace ShiftEngine
{
	struct bufferDescription
	{
		bufferDescription() 
			: bufferData(nullptr)
            , pixel(false)
            , vertex(false)
            , isDirty(true)
            , cbFromShader(nullptr)
            , StartSlotVertex(UINT_MAX)
            , StartSlotPixel(UINT_MAX)
            , Size(0)
			{}
		string Name;
		bool isDirty;
		uint32_t Size;
		uint32_t StartSlotVertex;
		uint32_t StartSlotPixel;
		ID3D10Buffer * cbFromShader;
		uint8_t * bufferData;
		bool pixel, vertex;
	};

	struct varDesc
	{
		string Name;
		uint32_t Offset;
		uint32_t Size;
		uint32_t BufferIndex;
	};

	struct resDesc
	{
		resDesc()
			: Name("")
            , BindPointVertex(UINT_MAX)
            , BindPointPixel(UINT_MAX)
            , View(nullptr)
            , isDirty(true)
            , pixel(false)
            , vertex(false) 
            {}
		string Name;
		uint32_t BindPointVertex;
		uint32_t BindPointPixel;
		ID3D10ShaderResourceView * View;
		bool isDirty;
		bool pixel, vertex;
	};

	class IProgram
	{
	public:
		virtual ~IProgram() {}

		virtual bool SetMatrixConstantByName(const char * nameInShader, const float * mat) = 0;
		virtual bool SetScalarConstantByName(const char * nameInShader, const float * scalar) = 0;
		virtual bool SetVectorConstantByName(const char * nameInShader, const float * vec) = 0;
		virtual bool SetArrayConstantByName(const char * nameInShader, const void * data) = 0;
		virtual bool SetTextureByName(const char * textureName, TexturePtr & texture) = 0;

		virtual void SetMatrixConstantByIndex(unsigned int index, const float * mat) = 0;
		virtual void SetScalarConstantByIndex(unsigned int index, const float * mat) = 0;
		virtual void SetVectorConstantByIndex(unsigned int index, const float * mat) = 0;
		virtual void SetArrayConstantByIndex(unsigned int index, const void * data) = 0;
		virtual bool SetTextureByIndex(unsigned int index, TexturePtr & texture) = 0;

		virtual bool GetVariableIndex(const char * name, unsigned int * index) = 0;
		virtual bool GetResourceIndex(const char * name, unsigned int * index) = 0;

		virtual void Apply(bool setShader) = 0;
	};

	class D3D10Program : public IProgram
	{
		friend class D3D10ContextManager;
	public:
		D3D10Program(D3D10ShaderPtr & _vertexShader, D3D10ShaderPtr & _pixelShader, ID3D10Device * _pDevice);
		~D3D10Program();

		virtual bool SetMatrixConstantByName(const char * nameInShader, const float * mat) override;
		virtual bool SetScalarConstantByName(const char * nameInShader, const float * scalar) override;
		virtual bool SetVectorConstantByName(const char * nameInShader, const float * vec) override;
		virtual bool SetArrayConstantByName( const char * nameInShader, const void * data ) override;
		virtual bool SetTextureByName(const char * textureName, TexturePtr & texture) override;	

		virtual void SetMatrixConstantByIndex(unsigned int index, const float * mat) override;
		virtual void SetScalarConstantByIndex(unsigned int index, const float * mat) override;
		virtual void SetVectorConstantByIndex(unsigned int index, const float * mat) override;
		virtual void SetArrayConstantByIndex(unsigned int index, const void * data) override;
		virtual bool SetTextureByIndex(unsigned int index, TexturePtr & texture) override;

		virtual bool GetVariableIndex(const char * name, unsigned int * index) override;
		virtual bool GetResourceIndex(const char * name, unsigned int * index) override;

		virtual void Apply(bool setShader) override;

	private:
		void ParseInfo();

		void SetUniformByIndex(unsigned int index, const void * data);

		//mapped buffers
		vector<bufferDescription> constantBuffers;
		vector<varDesc> variables;

		//samplers only
		vector<resDesc> resources;

		//shaders to bound
		D3D10ShaderPtr vertexShader;
		D3D10ShaderPtr pixelShader;

		ID3D10Device * pDevice;
	};

	typedef std::shared_ptr<IProgram> IProgramPtr;
	typedef std::shared_ptr<D3D10Program> D3D10ProgramPtr;

}	//end of namespace ShiftEngine