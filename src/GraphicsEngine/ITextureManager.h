#pragma once

#include <unordered_map>
#include <string>

#include "ITexture.h"

namespace ShiftEngine
{
    struct TextureInfo
    {
        unsigned int width;
        unsigned int height;
    };

    class ITextureManager
    {
    public:
        virtual ITexturePtr CreateTexture2D(const std::wstring & FileName) = 0;
        virtual ITexturePtr CreateCubemap(const std::wstring & posX,
                                      const std::wstring & negX,
                                      const std::wstring & posY,
                                      const std::wstring & negY,
                                      const std::wstring & posZ,
                                      const std::wstring & negZ) = 0;
        virtual ITexturePtr CreateTextureArray(const std::vector<std::wstring> & names) = 0;
        virtual TextureInfo GetTextureInfo(const std::wstring & filename) = 0;
        virtual ITexturePtr GetErrorTexture() = 0;
    };
}
