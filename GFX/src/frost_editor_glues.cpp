/* ###################################### */
/* ###     Frost Editor, by Kalith    ### */
/* ###################################### */
/*                                        */

#include "frost_editor.h"

#include "scene/frost_lightmanager.h"
#include "camera/frost_cameramanager.h"
#include "camera/frost_camera.h"
#include "scene/frost_scenemanager.h"
#include "scene/frost_sceneobject.h"
#include "material/frost_material.h"
#include "model/frost_model.h"

#include <frost_xml_document.h>
#include <frost_utils_eventmanager.h>
#include <frost_utils_event.h>

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

    int LuaEditor::_CanRedo( lua_State* pLua )
    {
        Lua::Function mFunc("Editor:CanRedo", pLua, 1);

        mFunc.Push(pEditor_->CanRedo());

        return mFunc.Return();
    }

    int LuaEditor::_CanUndo( lua_State* pLua )
    {
        Lua::Function mFunc("Editor:CanUndo", pLua, 1);

        mFunc.Push(pEditor_->CanUndo());

        return mFunc.Return();
    }

    int LuaEditor::_DeleteObject( lua_State* pLua )
    {
        Lua::Function mFunc("Editor:DeselectObject", pLua, 1);
        mFunc.Add(0, "object id", Lua::TYPE_NUMBER);
        if (mFunc.Check())
        {
            s_ptr<SceneObject> pObj = SceneManager::GetSingleton()->GetSceneObjectByID(s_uint(mFunc.Get(0)->GetNumber()));
            if (!pObj)
            {
                Warning(mFunc.GetName(), "No object found with ID "+mFunc.Get(0)->GetNumber()+".");
                return mFunc.Return();
            }

            SceneManager::GetSingleton()->DeselectSceneObject(pObj);
            pEditor_->AddEditorAction(pObj->CreateDeleteAction());
        }

        return mFunc.Return();
    }

    int LuaEditor::_DeselectObject( lua_State* pLua )
    {
        Lua::Function mFunc("Editor:DeselectObject", pLua, 1);
        mFunc.Add(0, "object id", Lua::TYPE_NUMBER);
        if (mFunc.Check())
        {
            s_ptr<SceneObject> pObj = SceneManager::GetSingleton()->GetSceneObjectByID(s_uint(mFunc.Get(0)->GetNumber()));
            if (!pObj)
            {
                Warning(mFunc.GetName(), "No object found with ID "+mFunc.Get(0)->GetNumber()+".");
                return mFunc.Return();
            }

            SceneManager::GetSingleton()->DeselectSceneObject(pObj);
        }

        return mFunc.Return();
    }

    int LuaEditor::_GetObjectList( lua_State* pLua )
    {
        Lua::Function mFunc("Editor:GetObjectList", pLua);

        s_ptr<Lua::State> pState = mFunc.GetState();

        const s_map< s_uint, s_ptr<SceneObject> >& lObjectList = SceneManager::GetSingleton()->GetSceneObjectList();
        s_map< s_uint, s_ptr<SceneObject> >::const_iterator iter;
        foreach (iter, lObjectList)
        {
            pState->NewTable();
            pState->SetFieldString("name", iter->second->GetName());
            pState->SetFieldInt("id", s_int(iter->first));
            pState->SetFieldString("type", iter->second->GetType());

            mFunc.NotifyPushed();
        }

        return mFunc.Return();
    }

    int LuaEditor::_GetSelectedObjectCount( lua_State* pLua )
    {
        Lua::Function mFunc("Editor:GetSelectedObjectCount", pLua, 1);

        mFunc.Push(SceneManager::GetSingleton()->GetSelectedSceneObjectList().GetSize());

        return mFunc.Return();
    }

    int LuaEditor::_GetSelectedObjects( lua_State* pLua )
    {
        Lua::Function mFunc("Editor:GetSelectedObjects", pLua);

        const s_map< s_uint, s_ptr<SceneObject> >& lObjects = SceneManager::GetSingleton()->GetSelectedSceneObjectList();
        s_map< s_uint, s_ptr<SceneObject> >::const_iterator iter;
        foreach (iter, lObjects)
        {
            mFunc.Push(iter->second->GetID());
        }

        return mFunc.Return();
    }

    int LuaEditor::_HideObject( lua_State* pLua )
    {
        Lua::Function mFunc("Editor:ShowObject", pLua, 1);
        mFunc.Add(0, "object id", Lua::TYPE_NUMBER);
        if (mFunc.Check())
        {
            s_ptr<SceneObject> pObj = SceneManager::GetSingleton()->GetSceneObjectByID(s_uint(mFunc.Get(0)->GetNumber()));
            if (!pObj)
            {
                Warning(mFunc.GetName(), "No object found with ID "+mFunc.Get(0)->GetNumber()+".");
                return mFunc.Return();
            }
            if (pObj->IsShown())
            {
                pEditor_->AddEditorAction(s_refptr<EditorAction>(new VisibilityAction(
                    pObj, false
                )));
            }
        }

        return mFunc.Return();
    }

    int LuaEditor::_Redo( lua_State* pLua )
    {
        Lua::Function mFunc("Editor:Redo", pLua, 1);

        mFunc.Push(pEditor_->Redo());

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

    int LuaEditor::_SelectObject( lua_State* pLua )
    {
        Lua::Function mFunc("Editor:SelectObject", pLua, 1);
        mFunc.Add(0, "object id", Lua::TYPE_NUMBER);
        if (mFunc.Check())
        {
            s_ptr<SceneObject> pObj = SceneManager::GetSingleton()->GetSceneObjectByID(s_uint(mFunc.Get(0)->GetNumber()));
            if (!pObj)
            {
                Warning(mFunc.GetName(), "No object found with ID "+mFunc.Get(0)->GetNumber()+".");
                return mFunc.Return();
            }
            SceneManager::GetSingleton()->SelectSceneObject(pObj);
        }

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

    int LuaEditor::_SetCurrentTool( lua_State* pLua )
    {
        Lua::Function mFunc("Editor:SetCurrentTool", pLua);
        mFunc.Add(0, "tool", Lua::TYPE_STRING);

        if (mFunc.Check())
        {
            Editor::Tool mTool;
            s_str sTool = mFunc.Get(0)->GetString();

            if (sTool == "MOVE")
                mTool = Editor::TOOL_MOVE;
            else if (sTool == "SCALE")
                mTool = Editor::TOOL_SCALE;
            else if (sTool == "ROTATE")
                mTool = Editor::TOOL_ROTATE;
            else
            {
                Warning(mFunc.GetName(), "Unknown tool : \""+sTool+"\".");
                return mFunc.Return();

            }

            pEditor_->SetCurrentTool(mTool);
        }

        return mFunc.Return();
    }

    int LuaEditor::_ShowObject( lua_State* pLua )
    {
        Lua::Function mFunc("Editor:ShowObject", pLua, 1);
        mFunc.Add(0, "object id", Lua::TYPE_NUMBER);
        if (mFunc.Check())
        {
            s_ptr<SceneObject> pObj = SceneManager::GetSingleton()->GetSceneObjectByID(s_uint(mFunc.Get(0)->GetNumber()));
            if (!pObj)
            {
                Warning(mFunc.GetName(), "No object found with ID "+mFunc.Get(0)->GetNumber()+".");
                return mFunc.Return();
            }
            if (!pObj->IsShown())
            {
                pEditor_->AddEditorAction(s_refptr<EditorAction>(new VisibilityAction(
                    pObj, true
                )));
            }
        }

        return mFunc.Return();
    }

    int LuaEditor::_Undo( lua_State* pLua )
    {
        Lua::Function mFunc("Editor:Undo", pLua, 1);

        mFunc.Push(pEditor_->Undo());

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

        method(Editor, CanRedo),
        method(Editor, CanUndo),
        method(Editor, DeleteObject),
        method(Editor, DeselectObject),
        method(Editor, GetBackgroundColor),
        method(Editor, GetObjectList),
        method(Editor, GetSelectedObjectCount),
        method(Editor, GetSelectedObjects),
        method(Editor, HideObject),
        method(Editor, Redo),
        method(Editor, SelectObject),
        method(Editor, SetBackgroundColor),
        method(Editor, SetCurrentTool),
        method(Editor, ShowObject),
        method(Editor, ToggleShading),
        method(Editor, ToggleWireframeView),
        method(Editor, Undo),

        {0,0}
    };
}
