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
        mFunc.Add(0, "event", Lua::TYPE_STRING);
        if (mFunc.Check())
        {
            pParent_->RegisterEvent(mFunc.Get(0)->GetString());
        }

        return mFunc.Return();
    }

    int LuaGameplay::_SetCamera(lua_State* pLua)
    {
        Lua::Function mFunc("Gameplay:SetCamera", pLua);
        mFunc.Add(0, "camera", Lua::TYPE_USERDATA);
        mFunc.Add(0, "nil", Lua::TYPE_NIL);
        if (mFunc.Check())
        {
            if (mFunc.Get(0)->GetType() == Lua::TYPE_USERDATA)
            {
                s_ptr<Camera> pCam = s_ptr<Camera>::DynamicCast(
                    Lunar<LuaMovableObject>::check(pLua, mFunc.Get(0)->GetIndex().Get())->GetObject()
                );
                pParent_->SetCamera(pCam);
            }
            else
                pParent_->SetCamera(nullptr);
        }

        return mFunc.Return();
    }

    int LuaGameplay::_CreateCamera(lua_State* pLua)
    {
        Lua::Function mFunc("Gameplay:CreateCamera", pLua, 1);
        mFunc.Add(0, "vec", Lua::TYPE_TABLE);
        mFunc.NewParamSet();
        mFunc.Add(0, "x", Lua::TYPE_NUMBER);
        mFunc.Add(1, "y", Lua::TYPE_NUMBER);
        mFunc.Add(2, "z", Lua::TYPE_NUMBER);
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
                    mFunc.Get(0)->GetNumber(),
                    mFunc.Get(1)->GetNumber(),
                    mFunc.Get(2)->GetNumber()
                ));
            }
            else
            {
                pCam = pParent_->CreateCamera();
            }

            pCam->PushOnLua(mFunc.GetState());
            mFunc.NotifyPushed();
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
