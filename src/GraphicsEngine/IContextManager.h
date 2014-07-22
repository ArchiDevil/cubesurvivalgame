#pragma once

typedef void * ShaderPtr;
typedef void * RenderTargetPtr;
typedef void * MaterialPtr;

namespace ShiftEngine
{
	class IContextManager
	{
	public:
		IContextManager();
		~IContextManager();

		virtual bool Initialize() = 0;
		virtual bool BindShader(ShaderPtr shader) = 0;
		virtual bool BindRenderTargers(RenderTargetPtr * rt, int numOfRT = 1) = 0;
		virtual bool BindMaterial(MaterialPtr material) = 0;

		//some more like setting blending and something
	private:

	};

}	//end of ShiftEngine namespace