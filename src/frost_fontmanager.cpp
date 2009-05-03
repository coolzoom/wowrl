/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*           Font manager source          */
/*                                        */
/*  ## : Contains the member functions    */
/*  of the FontManager class.             */
/*       A FontManager is obviously a     */
/*  class that manages font creation and  */
/*  storage.                              */
/*                                        */


#include "frost_fontmanager.h"

#include <OgreFontManager.h>

using namespace std;

namespace Frost
{
    const s_str FontManager::CLASS_NAME = "FontManager";

    FontManager::FontManager()
    {
    }

    FontManager::~FontManager()
    {
    }

    s_ptr<Ogre::Font> FontManager::GetFont(const s_str& sFontFile, const s_uint& uiSize)
    {
        s_str sID = sFontFile + "|" + uiSize;
        if (MAPFIND(sID, lFontList_))
        {
            return lFontList_[sID];
        }
        else
        {
            if (File::Exists(sFontFile))
            {
                Ogre::FontPtr pFnt = Ogre::FontManager::getSingleton().create(sID.Get(), "Frost");
                pFnt->setType(Ogre::FT_TRUETYPE);
                pFnt->setSource(sFontFile.Get());
                pFnt->setTrueTypeSize(uiSize.Get());
                pFnt->setTrueTypeResolution(96);
                pFnt->addCodePointRange(Ogre::Font::CodePointRange(33, 255));
                pFnt->load();

                lFontList_[sID] = pFnt.getPointer();

                return s_ptr<Ogre::Font>(pFnt.getPointer());
            }
            else
            {
                Error(CLASS_NAME, "Unknown font file : \""+sFontFile+"\"");
                return NULL;
            }
        }
    }

    const s_str& FontManager::GetDefaultFont() const
    {
        return sDefaultFont_;
    }

    s_bool FontManager::ReadConfig()
    {
        sDefaultFont_ = LuaManager::GetSingleton()->GetDefaultLua()->GetGlobalString("sDefaultFont");
        if (sDefaultFont_ == "")
        {
            Error(CLASS_NAME, "No default font specified.");
            return false;
        }
        return true;
    }
}
