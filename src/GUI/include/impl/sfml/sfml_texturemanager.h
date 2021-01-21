#ifndef TEXTUREMANAGER_H
#define TEXTUREMANAGER_H

#include <frost_utils.h>
#include <frost_utils_manager.h>
#ifdef USING_THREADS
#include <frost_utils_thread.h>
#endif

namespace sf
{
    class Image;
}

namespace Frost
{
namespace GUI
{
    class TextureManager : public Manager<TextureManager>
    {
    friend class Manager<TextureManager>;
    public :

        s_ptr<sf::Image> LoadTexture(const s_str& sFile);

        static const s_str CLASS_NAME;

    protected:

        TextureManager();
        ~TextureManager();

    private:

        s_map< s_str, s_ptr<sf::Image> > lTextureList_;

        #ifdef USING_THREADS
        Mutex mMutex_;
        #endif

    };
}
}

#endif
