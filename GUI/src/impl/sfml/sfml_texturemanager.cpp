#include "impl/sfml/sfml_texturemanager.h"

#include <SFML/Graphics/Image.hpp>

using namespace std;
using namespace Frost;
using namespace Frost::GUI;

const s_str TextureManager::CLASS_NAME = "TextureManager";

TextureManager::TextureManager()
{
}

TextureManager::~TextureManager()
{
    s_map< s_str, s_ptr<sf::Image> >::iterator iter;
    foreach (iter, lTextureList_)
    {
        iter->second.Delete();
    }
}

s_ptr<sf::Image> TextureManager::LoadTexture( const s_str& sFile )
{
    s_map< s_str, s_ptr<sf::Image> >::const_iterator iter = lTextureList_.Get(sFile);
    if (iter != lTextureList_.End())
    {
        return iter->second;
    }
    else
    {
        #ifdef USING_THREADS
        Lock l(mMutex_);
        #endif
        s_ptr<sf::Image> pImg = lTextureList_[sFile] = new sf::Image();
        if (!pImg->LoadFromFile(sFile.Get()))
        {
            throw Exception(CLASS_NAME, "Unable to load image : "+sFile);
        }
        return pImg;
    }
}
