/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*                                        */

#include "frost_engine_glues.h"
#include "camera/frost_cameramanager.h"
#include "camera/frost_camera.h"
#include "scene/frost_lightmanager.h"

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

    int LuaEngine::GetDataTable( lua_State * pLua )
    {
        lua_getref(pLua, iRef_);
        return 1;
    }

    const char  LuaEngine::className[] = "Engine";
    const char* LuaEngine::classList[] = {"Engine", 0};
    Lunar<LuaEngine>::RegType LuaEngine::methods[] = {
        {"dt", &LuaEngine::GetDataTable},

        method(Engine, ToggleWireframeView),
        method(Engine, ToggleShading),
        {0,0}
    };
}
