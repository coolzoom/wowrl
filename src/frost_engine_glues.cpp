/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*                                        */

#include "frost_engine_glues.h"
#include "camera/frost_cameramanager.h"
#include "camera/frost_camera.h"
#include "scene/frost_lightmanager.h"
#include "scene/frost_zonemanager.h"
#include "frost_inputmanager.h"

#include <OgreCamera.h>

#define method(widget, function) {#function, &Lua##widget::_##function}

using namespace std;

namespace Frost
{
    const s_str LuaEngine::CLASS_NAME = "GUI::LuaEngine";

    LuaEngine::LuaEngine(lua_State* pLua)
    {
        pEngine_ = Engine::GetSingleton();

        lua_newtable(pLua);
        iRef_ = luaL_ref(pLua, LUA_REGISTRYINDEX);
        pLua_ = pLua;
    }

    int LuaEngine::_AllowWorldClicks( lua_State* pLua )
    {
        Lua::Function mFunc("Engine:AllowWorldClicks", pLua);

        InputManager::GetSingleton()->AllowClicks("WORLD");

        return mFunc.Return();
    }

    int LuaEngine::_BlockWorldClicks( lua_State* pLua )
    {
        Lua::Function mFunc("Engine:AllowWorldClicks", pLua);

        InputManager::GetSingleton()->BlockClicks("WORLD");

        return mFunc.Return();
    }

    int LuaEngine::_FireEvent( lua_State* pLua )
    {
        Lua::Function mFunc("Engine:FireEvent", pLua);
        mFunc.Add(0, "event name", Lua::TYPE_STRING);

        if (mFunc.Check())
        {
            Event mEvent(mFunc.Get(0)->GetString());

            s_ptr<Lua::State> pLua = mFunc.GetState();
            s_int iTop = s_int(pLua->GetTop());
            if (iTop > 1)
            {
                for (s_int i = 2; i <= iTop; ++i)
                {
                    mEvent.Add(pLua->GetValue(i));
                }
            }

            EventManager::GetSingleton()->FireEvent(mEvent);
        }

        return mFunc.Return();
    }

    int LuaEngine::_ForceWorldClicksAllowed( lua_State* pLua )
    {
        Lua::Function mFunc("Engine:ForceWorldClicksAllowed", pLua);
        mFunc.Add(0, "allowed", Lua::TYPE_BOOLEAN, true);

        if (mFunc.Check())
        {
            if ((mFunc.IsProvided(0) && mFunc.Get(0)->GetBool()) || !mFunc.IsProvided(0))
                InputManager::GetSingleton()->ForceClicksAllowed("WORLD", true);
            else
                InputManager::GetSingleton()->ForceClicksAllowed("WORLD", false);
        }

        return mFunc.Return();
    }

    int LuaEngine::_LoadZone( lua_State* pLua )
    {
        Lua::Function mFunc("Engine:LoadZone", pLua);
        mFunc.Add(0, "name", Lua::TYPE_STRING);

        if (mFunc.Check())
        {
            ZoneManager::GetSingleton()->LoadZone(mFunc.Get(0)->GetString());
        }

        return mFunc.Return();
    }

    int LuaEngine::_LoadZoneFile( lua_State* pLua )
    {
        Lua::Function mFunc("Engine:LoadZoneFile", pLua);
        mFunc.Add(0, "name", Lua::TYPE_STRING);

        if (mFunc.Check())
        {
            ZoneManager::GetSingleton()->LoadZoneFile(mFunc.Get(0)->GetString());
        }

        return mFunc.Return();
    }

    int LuaEngine::_ToggleWireframeView( lua_State* pLua )
    {
        Lua::Function mFunc("Engine:ToggleWireframeView", pLua);

        s_ptr<Camera> pCam = CameraManager::GetSingleton()->GetMainCamera();
        if (pCam->GetOgreCamera()->getPolygonMode() == Ogre::PM_WIREFRAME)
            pCam->GetOgreCamera()->setPolygonMode(Ogre::PM_SOLID);
        else
            pCam->GetOgreCamera()->setPolygonMode(Ogre::PM_WIREFRAME);

        return mFunc.Return();
    }

    int LuaEngine::_ToggleShading( lua_State* pLua )
    {
        Lua::Function mFunc("Engine:ToggleShading", pLua);

        if (LightManager::GetSingleton()->IsAmbientLocked())
            LightManager::GetSingleton()->UnlockAmbient();
        else
            LightManager::GetSingleton()->LockAmbient(Color::WHITE);

        return mFunc.Return();
    }


    int LuaEngine::_GetConstant( lua_State* pLua )
    {
        Lua::Function mFunc("Engine:GetConstant", pLua, 1);
        mFunc.Add(0, "name", Lua::TYPE_STRING);

        if (mFunc.Check())
        {
            mFunc.Push(Engine::GetSingleton()->GetConstant(mFunc.Get(0)->GetString()));
        }

        return mFunc.Return();
    }

    int LuaEngine::_SetConstant( lua_State* pLua )
    {
        Lua::Function mFunc("Engine:SetConstant", pLua);
        mFunc.Add(0, "name", Lua::TYPE_STRING);
        mFunc.Add(1, "value", Lua::TYPE_STRING);
        mFunc.Add(1, "value", Lua::TYPE_NUMBER);
        mFunc.Add(1, "value", Lua::TYPE_BOOLEAN);
        mFunc.Add(1, "value", Lua::TYPE_NIL);

        if (mFunc.Check())
        {
            Engine::GetSingleton()->SetConstant(mFunc.Get(0)->GetString(), mFunc.Get(1)->GetData()->GetValue());
        }

        return mFunc.Return();
    }

    int LuaEngine::_GetBackgroundColor( lua_State* pLua )
    {
        Lua::Function mFunc("Engine:GetBackgroundColor", pLua, 4);

        Color mColor = CameraManager::GetSingleton()->GetBackgroundColor();

        mFunc.Push(s_float(mColor.GetR())/255.0f);
        mFunc.Push(s_float(mColor.GetG())/255.0f);
        mFunc.Push(s_float(mColor.GetB())/255.0f);
        mFunc.Push(s_float(mColor.GetA())/255.0f);

        return mFunc.Return();
    }

    int LuaEngine::_SetBackgroundColor( lua_State* pLua )
    {
        Lua::Function mFunc("Engine:SetBackgroundColor", pLua);
        mFunc.Add(0, "red", Lua::TYPE_NUMBER);
        mFunc.Add(1, "green", Lua::TYPE_NUMBER);
        mFunc.Add(2, "blue", Lua::TYPE_NUMBER);
        mFunc.Add(3, "alpha", Lua::TYPE_NUMBER, true);

        if (mFunc.Check())
        {
            Color mColor;
            if (mFunc.IsProvided(3))
            {
                mColor = Color(
                    s_uchar(255.0f*mFunc.Get(3)->GetNumber()),
                    s_uchar(255.0f*mFunc.Get(0)->GetNumber()),
                    s_uchar(255.0f*mFunc.Get(1)->GetNumber()),
                    s_uchar(255.0f*mFunc.Get(2)->GetNumber())
                );
            }
            else
            {
                mColor = Color(
                    s_uchar(255.0f*mFunc.Get(0)->GetNumber()),
                    s_uchar(255.0f*mFunc.Get(1)->GetNumber()),
                    s_uchar(255.0f*mFunc.Get(2)->GetNumber())
                );
            }

            CameraManager::GetSingleton()->SetBackgroundColor(mColor);
        }

        return mFunc.Return();
    }

    int LuaEngine::_UnloadZone( lua_State* pLua )
    {
        Lua::Function mFunc("Engine:UnloadZone", pLua);

        ZoneManager::GetSingleton()->UnloadZone();

        return mFunc.Return();
    }

    int LuaEngine::GetDataTable( lua_State * pLua )
    {
        lua_getref(pLua, iRef_);
        return 1;
    }

    const char  LuaEngine::className[] = "Engine";
    const char* LuaEngine::classList[] = {"Engine", 0};
    Lunar<LuaEngine>::RegType LuaEngine::methods[] = {
        {"dt", &LuaEngine::GetDataTable},

        method(Engine, AllowWorldClicks),
        method(Engine, BlockWorldClicks),
        method(Engine, FireEvent),
        method(Engine, ForceWorldClicksAllowed),
        method(Engine, GetBackgroundColor),
        method(Engine, LoadZone),
        method(Engine, LoadZoneFile),
        method(Engine, ToggleWireframeView),
        method(Engine, ToggleShading),
        method(Engine, SetBackgroundColor),
        method(Engine, UnloadZone),
        {0,0}
    };
}
