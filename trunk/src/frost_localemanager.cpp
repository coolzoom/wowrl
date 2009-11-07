/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*          LocaleManager source          */
/*                                        */


#include "frost_localemanager.h"

using namespace std;

namespace Frost
{
    const s_str LocaleManager::CLASS_NAME = "LocaleManager";

    LocaleManager::LocaleManager()
    {
    }

    LocaleManager::~LocaleManager()
    {
        Log("Closing "+CLASS_NAME+"...");
        LuaManager::GetSingleton()->CloseLua(pLua_);
    }

    void LocaleManager::Initialize()
    {
        pLua_ = LuaManager::GetSingleton()->CreateLua();
    }

    s_str LocaleManager::GetLocalizedString( const s_str& sStr )
    {
        return pLua_->GetGlobalString(sStr, false, "<"+sStr+">");
    }

    void LocaleManager::SetLocale( const s_str& sLocale )
    {
        if (sLocale_ == "")
        {
            sLocale_ = sLocale;
            ReadLocale();
        }
        else
            Warning(CLASS_NAME, "SetLocale can't be called more than once.");
    }

    const s_str& LocaleManager::GetLocale() const
    {
        return sLocale_;
    }

    void LocaleManager::ReadConfig()
    {
        sLocale_ = Engine::GetSingleton()->GetStringConstant("GameLocale");
        if (sLocale_ == "")
        {
            throw Exception(CLASS_NAME, "No locale specified.");
        }

        ReadLocale();
    }

    void LocaleManager::ReadLocale()
    {
        Directory mDir("Locale/"+sLocale_);
        s_ctnr<s_str> lFileList = mDir.GetFileList(true, "lua");
        s_ctnr<s_str>::iterator iter;
        foreach (iter, lFileList)
        {
            pLua_->DoFile(*iter);
        }
    }
}
