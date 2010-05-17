/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*          LocaleManager source          */
/*                                        */


#include "frost_localemanager.h"
#include "lua/frost_lua.h"

#include <utils/frost_utils_directory.h>
#include <utils/frost_utils_lua.h>

using namespace std;

namespace Frost
{
    const s_str LocaleManager::CLASS_NAME = "LocaleManager";

    LocaleManager::LocaleManager()
    {
        Log("Creating "+CLASS_NAME+".");
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

    s_str LocaleManager::GetKeyString( const s_uint& uiKey ) const
    {
        s_map< s_uint, s_array<s_str,3> >::const_iterator iter = lKeyMap_.Get(uiKey);
        if (iter != lKeyMap_.End())
            return iter->second[0];
        else
            return "";
    }

    s_str LocaleManager::GetKeyShiftString( const s_uint& uiKey ) const
    {
        s_map< s_uint, s_array<s_str,3> >::const_iterator iter = lKeyMap_.Get(uiKey);
        if (iter != lKeyMap_.End())
            return iter->second[1];
        else
            return "";
    }

    s_str LocaleManager::GetKeyAltString( const s_uint& uiKey ) const
    {
        s_map< s_uint, s_array<s_str,3> >::const_iterator iter = lKeyMap_.Get(uiKey);
        if (iter != lKeyMap_.End())
            return iter->second[2];
        else
            return "";
    }

    s_str LocaleManager::GetKeyComboString( const s_str& sComboKey, const s_uint& uiSecondKey ) const
    {
        s_map< s_str, s_map< s_uint, s_array<s_str,3> > >::const_iterator iter = lComboKeyMap_.Get(sComboKey);
        if (iter != lComboKeyMap_.End())
        {
            const s_map< s_uint, s_array<s_str,3> >& lSubMap = iter->second;
            s_map< s_uint, s_array<s_str,3> >::const_iterator iterSub = lSubMap.Get(uiSecondKey);
            if (iterSub != lSubMap.End())
            {
                return iterSub->second[0];
            }
            else
            {
                return sComboKey + GetKeyString(uiSecondKey);
            }
        }
        else
        {
            return "";
        }
    }

    s_str LocaleManager::GetKeyComboShiftString( const s_str& sComboKey, const s_uint& uiSecondKey ) const
    {
        s_map< s_str, s_map< s_uint, s_array<s_str,3> > >::const_iterator iter = lComboKeyMap_.Get(sComboKey);
        if (iter != lComboKeyMap_.End())
        {
            const s_map< s_uint, s_array<s_str,3> >& lSubMap = iter->second;
            s_map< s_uint, s_array<s_str,3> >::const_iterator iterSub = lSubMap.Get(uiSecondKey);
            if (iterSub != lSubMap.End())
            {
                return iterSub->second[1];
            }
            else
            {
                return sComboKey + GetKeyShiftString(uiSecondKey);
            }
        }
        else
        {
            return "";
        }
    }

    s_str LocaleManager::GetKeyComboAltString( const s_str& sComboKey, const s_uint& uiSecondKey ) const
    {
        s_map< s_str, s_map< s_uint, s_array<s_str,3> > >::const_iterator iter = lComboKeyMap_.Get(sComboKey);
        if (iter != lComboKeyMap_.End())
        {
            const s_map< s_uint, s_array<s_str,3> >& lSubMap = iter->second;
            s_map< s_uint, s_array<s_str,3> >::const_iterator iterSub = lSubMap.Get(uiSecondKey);
            if (iterSub != lSubMap.End())
            {
                return iterSub->second[2];
            }
            else
            {
                return sComboKey + GetKeyAltString(uiSecondKey);
            }
        }
        else
        {
            return "";
        }
    }

    s_bool LocaleManager::IsComboKey( const s_str& sComboKey ) const
    {
        return lComboKeyMap_.Find(sComboKey);
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
        Lua::RegisterKeyCodes(pLua_);

        Directory mDir("Locale/"+sLocale_);
        s_ctnr<s_str> lFileList = mDir.GetFileList(true, "lua");
        s_ctnr<s_str>::iterator iter;
        foreach (iter, lFileList)
        {
            try
            {
                pLua_->DoFile(*iter);
            }
            catch (LuaException& e)
            {
                Warning(CLASS_NAME, e.GetDescription());
            }
        }

        pLua_->GetGlobal("KeyMap");
        if (pLua_->GetType() == Lua::TYPE_TABLE)
        {
            for (pLua_->PushNil(); pLua_->Next(); pLua_->Pop())
            {
                if (pLua_->GetType(-2) != Lua::TYPE_NUMBER)
                {
                    Warning(CLASS_NAME,
                        "Parsing KeyMap : keys must be numbers (got "+
                        pLua_->GetTypeName(pLua_->GetType(-2))+")."
                    );
                    continue;
                }
                if (pLua_->GetType(-1) != Lua::TYPE_TABLE)
                {
                    Warning(CLASS_NAME,
                        "Parsing KeyMap : values must be tables (got "+
                        pLua_->GetTypeName(pLua_->GetType(-1))+")."
                    );
                    continue;
                }

                s_uint uiKey = s_uint(pLua_->GetNumber(-2));
                s_array<s_str,3> lMaps;
                s_uint i;
                for (pLua_->PushNil(); pLua_->Next(); pLua_->Pop())
                {
                    if (pLua_->GetType(-1) != Lua::TYPE_STRING)
                    {
                        Warning(CLASS_NAME,
                            "Parsing KeyMap : values must be tables of string (got table of "+
                            pLua_->GetTypeName(pLua_->GetType(-1))+")."
                        );
                        break;
                    }
                    lMaps[i] = pLua_->GetString(-1);
                    ++i;
                }

                lKeyMap_[uiKey] = lMaps;
            }

            pLua_->Pop();
        }
        else
        {
            pLua_->Pop();
            throw Exception(CLASS_NAME, "No keyboard map defined.");
        }

        pLua_->GetGlobal("KeyCombo");
        if (pLua_->GetType() == Lua::TYPE_TABLE)
        {
            for (pLua_->PushNil(); pLua_->Next(); pLua_->Pop())
            {
                if (pLua_->GetType(-2) != Lua::TYPE_STRING)
                {
                    Warning(CLASS_NAME,
                        "Parsing KeyCombo : keys must be strings (got "+
                        pLua_->GetTypeName(pLua_->GetType(-2))+")."
                    );
                    continue;
                }
                if (pLua_->GetType(-1) != Lua::TYPE_TABLE)
                {
                    Warning(CLASS_NAME,
                        "Parsing KeyCombo : values must be tables (got "+
                        pLua_->GetTypeName(pLua_->GetType(-1))+")."
                    );
                    continue;
                }

                s_str sKey = pLua_->GetString(-2);
                s_map< s_uint, s_array<s_str,3> > lMap;
                for (pLua_->PushNil(); pLua_->Next(); pLua_->Pop())
                {
                    if (pLua_->GetType(-2) != Lua::TYPE_NUMBER)
                    {
                        Warning(CLASS_NAME,
                            "Parsing KeyCombo.map : keys must be numbers (got "+
                            pLua_->GetTypeName(pLua_->GetType(-2))+")."
                        );
                        continue;
                    }
                    if (pLua_->GetType(-1) != Lua::TYPE_TABLE)
                    {
                        Warning(CLASS_NAME,
                            "Parsing KeyCombo.map : values must be tables (got "+
                            pLua_->GetTypeName(pLua_->GetType(-1))+")."
                        );
                        continue;
                    }

                    s_uint uiSecondKey = s_uint(pLua_->GetNumber(-2));
                    s_array<s_str,3> lComboMap;
                    s_uint i;
                    for (pLua_->PushNil(); pLua_->Next(); pLua_->Pop())
                    {
                        if (pLua_->GetType(-1) != Lua::TYPE_STRING)
                        {
                            Warning(CLASS_NAME,
                                "Parsing KeyMap.map : values must be tables of string (got table of "+
                                pLua_->GetTypeName(pLua_->GetType(-1))+")."
                            );
                            break;
                        }
                        lComboMap[i] = pLua_->GetString(-1);
                        ++i;
                    }

                    lMap[uiSecondKey] = lComboMap;
                }

                lComboKeyMap_[sKey] = lMap;
            }
        }
        pLua_->Pop();
    }
}
