/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*            LuaManager source           */
/*                                        */
/*                                        */

#include "frost_utils_luamanager.h"
#include "frost_utils_lua_state.h"
#include "frost_utils_stdhelper.h"

using namespace std;

namespace Frost
{
    const s_str LuaManager::CLASS_NAME = "LuaManager";

    LuaManager::LuaManager()
    {
    }

    LuaManager::~LuaManager()
    {
        map< lua_State*, s_ptr<Lua::State> >::iterator iter;
        foreach (iter, lLuaStateList_)
        {
            iter->second.Delete();
        }
    }

    s_ptr<Lua::State> LuaManager::CreateLua()
    {
        s_ptr<Lua::State> pLua = new Lua::State();
        lLuaStateList_[pLua->GetState()] = pLua;
        return pLua;
    }

    void LuaManager::CloseLua( s_ptr<Lua::State> pLua )
    {
        if (pLua)
        {
            s_map< lua_State*, s_ptr<Lua::State> >::iterator iter;
            iter = lLuaStateList_.Get(pLua->GetState());
            if (iter != lLuaStateList_.End())
            {
                iter->second.Delete();
                lLuaStateList_.Erase(iter);
            }
        }
    }

    s_ptr<Lua::State> LuaManager::GetState( lua_State* pLua )
    {
        s_map< lua_State*, s_ptr<Lua::State> >::iterator iter;
        iter = lLuaStateList_.Get(pLua);
        if (iter != lLuaStateList_.End())
        {
            return iter->second;
        }
        else
            return nullptr;
    }

    s_ptr<Lua::State> LuaManager::GetDefaultLua() const
    {
        return pDefaultLua_;
    }

    void LuaManager::SetDefaultLua( s_ptr<Lua::State> pLua )
    {
        pDefaultLua_ = pLua;
    }
}

