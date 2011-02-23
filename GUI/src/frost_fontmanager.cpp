/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*           Font manager source          */
/*                                        */
/*                                        */


#include "frost_fontmanager.h"
#include "frost_font.h"

#include <frost_utils_file.h>

using namespace std;
using namespace Frost;
using namespace Frost::GUI;

const s_str FontManager::CLASS_NAME = "GUI::FontManager";

FontManager::FontManager()
{
}

FontManager::~FontManager()
{
    ClearFonts();
}

s_ptr<Font> FontManager::GetFont( const s_str& sFontFile, const s_uint& uiSize )
{
    s_str sID = sFontFile + "|" + uiSize;
    s_map< s_str, s_ptr<Font> >::const_iterator iter = lFontList_.Get(sID);
    if (iter != lFontList_.End())
    {
        return iter->second;
    }
    else
    {
        #ifdef USING_THREADS
        Lock l(mMutex_);
        #endif

        if (File::Exists(sFontFile))
        {
            s_ptr<Font> pFont = lFontList_[sID] = new Font(sFontFile, uiSize);

            return pFont;
        }
        else
        {
            Error(CLASS_NAME, "Unknown font file : \""+sFontFile+"\"");
            return nullptr;
        }
    }
}

const s_str& FontManager::GetDefaultFont() const
{
    return sDefaultFont_;
}

void FontManager::ClearFonts()
{
    s_map< s_str, s_ptr<Font> >::iterator iter;
    foreach (iter, lFontList_)
    {
        iter->second.Delete();
    }

    lFontList_.Clear();
}

void FontManager::ReadConfig()
{

}
