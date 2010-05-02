/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*                                        */

#include "frost_engine_glues.h"
#include "camera/frost_cameramanager.h"
#include "camera/frost_camera.h"
#include "scene/frost_lightmanager.h"
#include "scene/frost_zonemanager.h"
#include "scene/frost_zone.h"
#include "scene/frost_doodad.h"
#include "scene/frost_scenemanager.h"
#include "material/frost_decal.h"
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


    int LuaEngine::_AddDoodad( lua_State* pLua )
    {
        Lua::Function mFunc("Engine:AddDoodad", pLua);
        mFunc.Add(0, "name", Lua::TYPE_STRING);
        mFunc.Add(1, "model", Lua::TYPE_STRING);

        if (mFunc.Check())
        {
            s_ptr<Zone> pZone = ZoneManager::GetSingleton()->GetCurrentZone();
            s_ptr<Doodad> pDoodad = pZone->AddDoodad(
                mFunc.Get(0)->GetString(), mFunc.Get(1)->GetString()
            );

            SceneManager::GetSingleton()->StartMoving(pDoodad, Vector::CONSTRAINT_NONE, true);
        }

        return mFunc.Return();
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


    int LuaEngine::_EnableMouseDecal( lua_State* pLua )
    {
        Lua::Function mFunc("Engine:EnableMouseDecal", pLua);
        mFunc.Add(0, "enable", Lua::TYPE_BOOLEAN);
        mFunc.Add(1, "texture", Lua::TYPE_STRING, true);
        mFunc.Add(2, "size", Lua::TYPE_NUMBER, true);
        mFunc.Add(3, "color (red)", Lua::TYPE_NUMBER, true);
        mFunc.Add(4, "color (green)", Lua::TYPE_NUMBER, true);
        mFunc.Add(5, "color (blue)", Lua::TYPE_NUMBER, true);
        mFunc.Add(6, "color (alpha)", Lua::TYPE_NUMBER, true);

        if (mFunc.Check())
        {
            if (mFunc.Get(0)->GetBool())
            {
                if (!mFunc.Get(1)->IsProvided())
                {
                    Error(mFunc.GetName(),
                        "You must provide a texture file to enable the mouse decal."
                    );
                    return mFunc.Return();
                }

                s_refptr<Decal> pDecal = s_refptr<Decal>(new Decal(mFunc.Get(1)->GetString()));

                if (mFunc.IsProvided(2))
                {
                    pDecal->SetScale(mFunc.Get(2)->GetNumber());
                }

                if (mFunc.IsProvided(5))
                {
                    pDecal->SetSelfIllumination(Color(
                        mFunc.Get(3)->GetNumber(),
                        mFunc.Get(4)->GetNumber(),
                        mFunc.Get(5)->GetNumber()
                    ));
                }

                if (mFunc.IsProvided(6))
                {
                    pDecal->SetDiffuse(Color(
                        mFunc.Get(6)->GetNumber(), 1, 1, 1
                    ));
                }

                ZoneManager::GetSingleton()->EnableMouseDecal(pDecal);
            }
            else
            {
                ZoneManager::GetSingleton()->DisableMouseDecal();
            }
        }

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


    int LuaEngine::_NotifyDoodadPositioned( lua_State* pLua )
    {
        Lua::Function mFunc("Engine:NotifyDoodadPositioned", pLua);

        SceneManager::GetSingleton()->StopAllTransformations();

        return mFunc.Return();
    }

    int LuaEngine::_SaveZone( lua_State* pLua )
    {
        Lua::Function mFunc("Engine:SaveZone", pLua);
        mFunc.Add(0, "file", Lua::TYPE_STRING);

        if (mFunc.Check())
        {
            ZoneManager::GetSingleton()->SaveZone(mFunc.Get(0)->GetString());
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

    int LuaEngine::_GetMouseDecalColor( lua_State* pLua )
    {
        Lua::Function mFunc("Engine:GetMouseDecalColor", pLua, 4);

        s_wptr<Decal> pDecal = ZoneManager::GetSingleton()->GetMouseDecal();
        if (s_refptr<Decal> pLocked = pDecal.Lock())
        {
            Color mColor = pLocked->GetSelfIllumination();
            mFunc.Push(mColor.GetR());
            mFunc.Push(mColor.GetG());
            mFunc.Push(mColor.GetB());
            mFunc.Push(pLocked->GetDiffuse().GetA());
        }

        return mFunc.Return();
    }

    int LuaEngine::_GetMouseDecalPosition( lua_State* pLua )
    {
        Lua::Function mFunc("Engine:GetMouseDecalPosition", pLua, 4);

        s_wptr<Decal> pDecal = ZoneManager::GetSingleton()->GetMouseDecal();
        if (s_refptr<Decal> pLocked = pDecal.Lock())
        {
            Vector mPos = pLocked->GetPosition(false);
            mFunc.Push(mPos.X());
            mFunc.Push(mPos.Y() - 5.0f);
            mFunc.Push(mPos.Z());
        }

        return mFunc.Return();
    }

    int LuaEngine::_SetMouseDecalColor( lua_State* pLua )
    {
        Lua::Function mFunc("Engine:SetMouseDecalColor", pLua);
        mFunc.Add(0, "red", Lua::TYPE_NUMBER);
        mFunc.Add(1, "green", Lua::TYPE_NUMBER);
        mFunc.Add(2, "blue", Lua::TYPE_NUMBER);
        mFunc.Add(3, "alpha", Lua::TYPE_NUMBER, true);

        s_wptr<Decal> pDecal = ZoneManager::GetSingleton()->GetMouseDecal();
        if (s_refptr<Decal> pLocked = pDecal.Lock())
        {
            if (mFunc.Check())
            {
                pLocked->SetSelfIllumination(Color(
                    mFunc.Get(0)->GetNumber(),
                    mFunc.Get(1)->GetNumber(),
                    mFunc.Get(2)->GetNumber()
                ));

                if (mFunc.IsProvided(3))
                {
                    pLocked->SetDiffuse(Color(
                        mFunc.Get(3)->GetNumber(), 1, 1, 1
                    ));
                }
            }
        }

        return mFunc.Return();
    }

    int LuaEngine::_SetMouseDecalTexture( lua_State* pLua )
    {
        Lua::Function mFunc("Engine:SetMouseDecalTexture", pLua);
        mFunc.Add(0, "file", Lua::TYPE_STRING);

        s_wptr<Decal> pDecal = ZoneManager::GetSingleton()->GetMouseDecal();
        if (s_refptr<Decal> pLocked = pDecal.Lock())
        {
            if (mFunc.Check())
            {
                pLocked->SetTextureFile(mFunc.Get(0)->GetString());
            }
        }

        return mFunc.Return();
    }

    int LuaEngine::_GetBackgroundColor( lua_State* pLua )
    {
        Lua::Function mFunc("Engine:GetBackgroundColor", pLua, 4);

        Color mColor = CameraManager::GetSingleton()->GetBackgroundColor();

        mFunc.Push(mColor.GetR());
        mFunc.Push(mColor.GetG());
        mFunc.Push(mColor.GetB());
        mFunc.Push(mColor.GetA());

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

    int LuaEngine::_UnloadZone( lua_State* pLua )
    {
        Lua::Function mFunc("Engine:UnloadZone", pLua);

        ZoneManager::GetSingleton()->UnloadZone();

        return mFunc.Return();
    }

    int LuaEngine::_GetFolderList( lua_State* pLua )
    {
        Lua::Function mFunc("Engine:GetFolderList", pLua);
        mFunc.Add(0, "folder", Lua::TYPE_STRING);

        if (mFunc.Check())
        {
            Directory mDir(mFunc.Get(0)->GetString());

            s_wptr<Directory> pSubDir;
            foreach_dir (pSubDir, mDir)
            {
                mFunc.Push(pSubDir->GetName());
            }
        }

        return mFunc.Return();
    }

    int LuaEngine::_GetFileList( lua_State* pLua )
    {
        Lua::Function mFunc("Engine:GetFileList", pLua);
        mFunc.Add(0, "folder", Lua::TYPE_STRING);

        if (mFunc.Check())
        {
            Directory mDir(mFunc.Get(0)->GetString());
            s_ctnr<s_str> lFileList = mDir.GetFileList();

            s_ctnr<s_str>::iterator iterFile;
            foreach (iterFile, lFileList)
            {
                mFunc.Push(*iterFile);
            }
        }

        return mFunc.Return();
    }

    int LuaEngine::_FileExists( lua_State* pLua )
    {
        Lua::Function mFunc("Engine:FileExists", pLua, 1);
        mFunc.Add(0, "file", Lua::TYPE_STRING);

        if (mFunc.Check())
        {
            mFunc.Push(File::Exists(mFunc.Get(0)->GetString()));
        }

        return mFunc.Return();
    }

    int LuaEngine::_CutFilePath( lua_State* pLua )
    {
        Lua::Function mFunc("Engine:CutFilePath", pLua, 1);
        mFunc.Add(0, "path", Lua::TYPE_STRING);

        if (mFunc.Check())
        {
            s_str sPath = mFunc.Get(0)->GetString();
            s_ctnr<s_str> lWords = sPath.Cut("/");

            s_ctnr<s_str>::iterator iter;
            foreach (iter, lWords)
            {
                s_ctnr<s_str> lSubWords = iter->Cut("\\");
                if (lSubWords.GetSize() > 1)
                {
                    iter = lWords.Erase(iter);
                    iter = lWords.Insert(iter, lSubWords);
                }
            }

            s_str sFile = lWords.Back();
            lWords.PopBack();

            s_str sFolder;
            foreach (iter, lWords)
            {
                if (sFolder.IsEmpty())
                    sFolder += *iter;
                else
                    sFolder += "/" + *iter;
            }

            s_ptr<Lua::State> pState = mFunc.GetState();
            pState->NewTable();
            pState->SetFieldString("file", sFile);
            pState->SetFieldString("folder", sFolder);
            pState->NewTable();
            pState->SetField("folders");
            pState->GetField("folders");

            s_int i = 1;
            foreach (iter, lWords)
            {
                pState->SetFieldString(i, *iter);
                ++i;
            }

            pState->Pop();

            mFunc.NotifyPushed();
        }

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

        method(Engine, AddDoodad),
        method(Engine, AllowWorldClicks),
        method(Engine, BlockWorldClicks),
        method(Engine, EnableMouseDecal),
        method(Engine, FireEvent),
        method(Engine, ForceWorldClicksAllowed),
        method(Engine, GetBackgroundColor),
        method(Engine, GetConstant),
        method(Engine, GetMouseDecalColor),
        method(Engine, GetMouseDecalPosition),
        method(Engine, LoadZone),
        method(Engine, LoadZoneFile),
        method(Engine, NotifyDoodadPositioned),
        method(Engine, ToggleWireframeView),
        method(Engine, ToggleShading),
        method(Engine, SaveZone),
        method(Engine, SetBackgroundColor),
        method(Engine, SetConstant),
        method(Engine, SetMouseDecalColor),
        method(Engine, SetMouseDecalTexture),
        method(Engine, UnloadZone),

        method(Engine, GetFolderList),
        method(Engine, GetFileList),
        method(Engine, FileExists),
        method(Engine, CutFilePath),

        {0,0}
    };
}
