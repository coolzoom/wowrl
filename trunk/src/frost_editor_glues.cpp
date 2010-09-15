/* ###################################### */
/* ###     Frost Editor, by Kalith    ### */
/* ###################################### */
/*                                        */

#include "frost_editor.h"

#include "scene/frost_lightmanager.h"
#include "camera/frost_cameramanager.h"
#include "camera/frost_camera.h"
#include "scene/frost_scenemanager.h"
#include "scene/frost_zonemanager.h"
#include "scene/frost_zone.h"

#include <OgreCamera.h>

#define method(widget, function) {#function, &Lua##widget::_##function}

using namespace std;

namespace Frost
{
    const s_str LuaEditor::CLASS_NAME = "GUI::LuaEditor";

    LuaEditor::LuaEditor(lua_State* pLua)
    {
        pEditor_ = Editor::GetSingleton();

        lua_newtable(pLua);
        iRef_ = luaL_ref(pLua, LUA_REGISTRYINDEX);
        pLua_ = pLua;
    }

    int LuaEditor::_AddDoodad( lua_State* pLua )
    {
        Lua::Function mFunc("Editor:AddDoodad", pLua, 1);
        mFunc.Add(0, "name", Lua::TYPE_STRING);
        mFunc.Add(1, "model", Lua::TYPE_STRING);

        if (mFunc.Check())
        {
            try
            {
                pEditor_->AddDoodad(mFunc.Get(0)->GetString(), mFunc.Get(1)->GetString());
                mFunc.PushNil();
            }
            catch (Exception& e)
            {
                mFunc.Push(e.GetDescription());
            }
        }

        return mFunc.Return();
    }

    int LuaEditor::_GetCurrentZoneFile( lua_State* pLua )
    {
        Lua::Function mFunc("Editor:GetCurrentZoneFile", pLua, 1);

        mFunc.Push(pEditor_->GetCurrentZoneFile());

        return mFunc.Return();
    }

    int LuaEditor::_GetModelFile( lua_State* pLua )
    {
        Lua::Function mFunc("Editor:GetModelFile", pLua, 1);
        mFunc.Add(0, "model name", Lua::TYPE_STRING);
        if (mFunc.Check())
        {
            s_ptr<Zone> pZone = ZoneManager::GetSingleton()->GetCurrentZone();
            if (pZone)
            {
                const s_map<s_str,s_str>& lModelList = pZone->GetModelList();
                s_map<s_str,s_str>::const_iterator iter = lModelList.Get(mFunc.Get(0)->GetString());
                if (iter != lModelList.End())
                {
                    mFunc.Push(iter->second);
                }
                else
                {
                    Warning(mFunc.GetName(),
                        "No model with the name \""+mFunc.Get(0)->GetString()+"\"."
                    );
                }
            }
            else
            {
                Warning(mFunc.GetName(),
                    "Can't get a model's file if no zone is loaded."
                );
            }
        }

        return mFunc.Return();
    }

    int LuaEditor::_GetZoneModelList( lua_State* pLua )
    {
        Lua::Function mFunc("Editor:GetZoneModelList", pLua);

        s_ptr<Zone> pZone = ZoneManager::GetSingleton()->GetCurrentZone();
        if (pZone)
        {
            const s_map<s_str,s_str>& lModelList = pZone->GetModelList();
            s_map<s_str,s_str>::const_iterator iter;
            foreach (iter, lModelList)
            {
                mFunc.Push(iter->first);
            }
        }
        else
        {
            Warning(mFunc.GetName(), "Can't create a model list if no zone is loaded.");
        }

        return mFunc.Return();
    }

    int LuaEditor::_IsModelLoaded( lua_State* pLua )
    {
        Lua::Function mFunc("Editor:IsModelLoaded", pLua, 1);
        mFunc.Add(0, "name", Lua::TYPE_STRING);
        if (mFunc.Check())
        {
            s_ptr<Zone> pZone = ZoneManager::GetSingleton()->GetCurrentZone();
            if (pZone)
            {
                const s_map<s_str,s_str>& lModelList = pZone->GetModelList();
                mFunc.Push(lModelList.Find(mFunc.Get(0)->GetString()));
            }
            else
            {
                mFunc.Push(s_bool(false));
            }
        }

        return mFunc.Return();
    }

    int LuaEditor::_IsZoneLoaded( lua_State* pLua )
    {
        Lua::Function mFunc("Editor:IsZoneLoaded", pLua, 1);

        mFunc.Push(ZoneManager::GetSingleton()->GetCurrentZone() != nullptr);

        return mFunc.Return();
    }

    int LuaEditor::_IsZoneSaved( lua_State* pLua )
    {
        Lua::Function mFunc("Editor:IsZoneSaved", pLua, 1);

        mFunc.Push(pEditor_->IsZoneSaved());

        return mFunc.Return();
    }

    int LuaEditor::_LoadZone( lua_State* pLua )
    {
        Lua::Function mFunc("Editor:LoadZone", pLua, 1);
        mFunc.Add(0, "name", Lua::TYPE_STRING);
        if (mFunc.Check())
        {
            try
            {
                pEditor_->LoadZone(mFunc.Get(0)->GetString());
                mFunc.PushNil();
            }
            catch (Exception& e)
            {
                mFunc.Push(e.GetDescription());
            }
        }

        return mFunc.Return();
    }

    int LuaEditor::_LoadZoneFile( lua_State* pLua )
    {
        Lua::Function mFunc("Editor:LoadZoneFile", pLua, 1);
        mFunc.Add(0, "name", Lua::TYPE_STRING);

        if (mFunc.Check())
        {
            try
            {
                pEditor_->LoadZoneFile(mFunc.Get(0)->GetString());
                mFunc.PushNil();
            }
            catch (Exception& e)
            {
                mFunc.Push(e.GetDescription());
            }
        }

        return mFunc.Return();
    }

    int LuaEditor::_NewZone( lua_State* pLua )
    {
        Lua::Function mFunc("Editor:NewZone", pLua, 1);
        mFunc.Add(0, "name", Lua::TYPE_STRING);

        if (mFunc.Check())
        {
            try
            {
                pEditor_->NewZone(mFunc.Get(0)->GetString());
                mFunc.PushNil();
            }
            catch (Exception& e)
            {
                mFunc.Push(e.GetDescription());
            }
        }

        return mFunc.Return();
    }

    int LuaEditor::_NotifyDoodadPositioned( lua_State* pLua )
    {
        Lua::Function mFunc("Editor:NotifyDoodadPositioned", pLua);

        SceneManager::GetSingleton()->StopAllTransformations();

        return mFunc.Return();
    }

    int LuaEditor::_SaveZone( lua_State* pLua )
    {
        Lua::Function mFunc("Editor:SaveZone", pLua, 2);
        mFunc.Add(0, "file", Lua::TYPE_STRING, true);

        if (mFunc.Check())
        {
            if (mFunc.IsProvided(0))
                pEditor_->SetCurrentZoneFile(mFunc.Get(0)->GetString());

            try
            {
                pEditor_->SaveZone();
                mFunc.PushNil();
            }
            catch (Exception& e)
            {
                mFunc.Push(e.GetDescription());
            }
        }

        return mFunc.Return();
    }

    int LuaEditor::_ToggleWireframeView( lua_State* pLua )
    {
        Lua::Function mFunc("Editor:ToggleWireframeView", pLua);

        s_ptr<Camera> pCam = CameraManager::GetSingleton()->GetMainCamera();
        if (pCam->GetOgreCamera()->getPolygonMode() == Ogre::PM_WIREFRAME)
            pCam->GetOgreCamera()->setPolygonMode(Ogre::PM_SOLID);
        else
            pCam->GetOgreCamera()->setPolygonMode(Ogre::PM_WIREFRAME);

        return mFunc.Return();
    }

    int LuaEditor::_ToggleShading( lua_State* pLua )
    {
        Lua::Function mFunc("Editor:ToggleShading", pLua);

        if (LightManager::GetSingleton()->IsAmbientLocked())
            LightManager::GetSingleton()->UnlockAmbient();
        else
            LightManager::GetSingleton()->LockAmbient(Color::WHITE);

        return mFunc.Return();
    }

    int LuaEditor::_GetBackgroundColor( lua_State* pLua )
    {
        Lua::Function mFunc("Editor:GetBackgroundColor", pLua, 4);

        Color mColor = CameraManager::GetSingleton()->GetBackgroundColor();

        mFunc.Push(mColor.GetR());
        mFunc.Push(mColor.GetG());
        mFunc.Push(mColor.GetB());
        mFunc.Push(mColor.GetA());

        return mFunc.Return();
    }

    int LuaEditor::_SetBackgroundColor( lua_State* pLua )
    {
        Lua::Function mFunc("Editor:SetBackgroundColor", pLua);
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
                    mFunc.Get(3)->GetNumber(),
                    mFunc.Get(0)->GetNumber(),
                    mFunc.Get(1)->GetNumber(),
                    mFunc.Get(2)->GetNumber()
                );
            }
            else
            {
                mColor = Color(
                    mFunc.Get(0)->GetNumber(),
                    mFunc.Get(1)->GetNumber(),
                    mFunc.Get(2)->GetNumber()
                );
            }

            CameraManager::GetSingleton()->SetBackgroundColor(mColor);
        }

        return mFunc.Return();
    }

    int LuaEditor::_UnloadZone( lua_State* pLua )
    {
        Lua::Function mFunc("Editor:UnloadZone", pLua);

        pEditor_->CloseZone();

        return mFunc.Return();
    }

    int LuaEditor::GetDataTable( lua_State* pLua )
    {
        lua_getref(pLua, iRef_);
        return 1;
    }

    const char  LuaEditor::className[] = "Editor";
    const char* LuaEditor::classList[] = {"Editor", 0};
    Lunar<LuaEditor>::RegType LuaEditor::methods[] = {
        {"dt", &LuaEditor::GetDataTable},

        method(Editor, AddDoodad),
        method(Editor, GetCurrentZoneFile),
        method(Editor, GetBackgroundColor),
        method(Editor, GetModelFile),
        method(Editor, GetZoneModelList),
        method(Editor, IsModelLoaded),
        method(Editor, IsZoneLoaded),
        method(Editor, IsZoneSaved),
        method(Editor, LoadZone),
        method(Editor, LoadZoneFile),
        method(Editor, NewZone),
        method(Editor, NotifyDoodadPositioned),
        method(Editor, ToggleWireframeView),
        method(Editor, ToggleShading),
        method(Editor, SaveZone),
        method(Editor, SetBackgroundColor),
        method(Editor, UnloadZone),

        {0,0}
    };
}
