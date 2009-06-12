/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*                                        */

#include "gameplay/frost_gameplay.h"
#include "gameplay/frost_gameplaymanager.h"
#include "scene/frost_movableobject.h"
#include "camera/frost_camera.h"

#define method(widget, function) {#function, &Lua##widget::_##function}

using namespace std;

namespace Frost
{
    const s_str LuaGameplay::CLASS_NAME = "LuaGameplay";

    LuaGameplay::LuaGameplay( lua_State* pLua )
    {
        s_str sName = lua_tostring(pLua, -1);

        lua_newtable(pLua);
        iRef_ = luaL_ref(pLua, LUA_REGISTRYINDEX);
        pLua_ = pLua;

        pParent_ = GameplayManager::GetSingleton()->GetGameplay(sName);
    }

    int LuaGameplay::GetDataTable( lua_State * pLua )
    {
        lua_getref(pLua, iRef_);
        return 1;
    }

    int LuaGameplay::_RegisterEvent(lua_State* pLua)
    {
        Lua::Function mFunc("Gameplay:RegisterEvent", pLua);
        mFunc.Add(0, "event", Lua::TYPE_STRING, VALUE_STRING);
        if (mFunc.Check())
        {
            pParent_->RegisterEvent(mFunc.Get(0)->GetS());
        }

        return mFunc.Return();
    }

    int LuaGameplay::_SetCamera(lua_State* pLua)
    {
        Lua::Function mFunc("Gameplay:SetCamera", pLua);
        mFunc.Add(0, "camera", Lua::TYPE_USERDATA, VALUE_NONE);
        if (mFunc.Check())
        {
            s_ptr<Camera> pCam = s_ptr<Camera>(Lunar<LuaMovableObject>::check(pLua, mFunc.Get(0)->GetI().Get())->GetObject());
            pParent_->SetCamera(pCam);
        }

        return mFunc.Return();
    }

    int LuaGameplay::_CreateCamera(lua_State* pLua)
    {
        Lua::Function mFunc("Gameplay:CreateCamera", pLua, 1);
        mFunc.Add(0, "vec", Lua::TYPE_TABLE, VALUE_NONE);
        mFunc.NewParamSet();
        mFunc.Add(0, "x", Lua::TYPE_NUMBER, VALUE_FLOAT);
        mFunc.Add(1, "y", Lua::TYPE_NUMBER, VALUE_FLOAT);
        mFunc.Add(2, "z", Lua::TYPE_NUMBER, VALUE_FLOAT);
        mFunc.NewParamSet();
        if (mFunc.Check())
        {
            s_ptr<Camera> pCam;
            if (mFunc.GetParamSetRank() == 0)
            {
                s_ptr<Lua::State> pState = LuaManager::GetSingleton()->GetState(pLua);
                pState->SetTop(1);

                Vector mVec(
                    pState->GetFieldFloat("x"),
                    pState->GetFieldFloat("y"),
                    pState->GetFieldFloat("z")
                );

                pCam = pParent_->CreateCamera(mVec);
            }
            else if (mFunc.GetParamSetRank() == 1)
            {
                pCam = pParent_->CreateCamera(Vector(
                    mFunc.Get(0)->GetF(),
                    mFunc.Get(1)->GetF(),
                    mFunc.Get(2)->GetF()
                ));
            }
            else
            {
                pCam = pParent_->CreateCamera();
            }

            Lunar<LuaMovableObject>::push(pLua, pCam->GetGlue().Get());
            mFunc.Push(Lua::ReturnValue(Lua::RETURN_OBJECT));
        }

        return mFunc.Return();
    }

    const char LuaGameplay::className[] = "Gameplay";
    Lunar<LuaGameplay>::RegType LuaGameplay::methods[] = {
        {"dt", &LuaGameplay::GetDataTable},

        method(Gameplay, RegisterEvent),
        method(Gameplay, SetCamera),
        method(Gameplay, CreateCamera),

        {0,0}
    };
}
