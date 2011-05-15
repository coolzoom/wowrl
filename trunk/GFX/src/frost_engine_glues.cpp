/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*                                        */

#include "frost_engine_glues.h"

#include "frost_inputmanager.h"
#include "frost_engine.h"

#include <frost_utils_directory.h>
#include <frost_utils_file.h>
#include <frost_utils_event.h>
#include <frost_utils_eventmanager.h>

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
            if (sFile.Find("."))
                lWords.PopBack();
            else
                sFile = "";

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

        method(Engine, AllowWorldClicks),
        method(Engine, BlockWorldClicks),
        method(Engine, FireEvent),
        method(Engine, ForceWorldClicksAllowed),
        method(Engine, GetConstant),
        method(Engine, SetConstant),

        method(Engine, GetFolderList),
        method(Engine, GetFileList),
        method(Engine, FileExists),
        method(Engine, CutFilePath),

        {0,0}
    };
}
