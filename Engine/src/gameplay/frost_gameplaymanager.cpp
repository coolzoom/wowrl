/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*          GameplayManager source        */
/*                                        */
/*                                        */

#include "gameplay/frost_gameplaymanager.h"
#include "gameplay/frost_gameplay.h"
#include "lua/frost_lua.h"
#include "frost_engine.h"

#include <frost_utils_directory.h>
#include <frost_utils_lua.h>

using namespace std;

namespace Frost
{
    const s_str GameplayManager::CLASS_NAME = "GameplayManager";

    GameplayManager::GameplayManager()
    {
    }

    GameplayManager::~GameplayManager()
    {
        Log("Closing "+CLASS_NAME+"...");
        LuaManager::GetSingleton()->CloseLua(pLua_);
    }

    void GameplayManager::Initialize()
    {
        pLua_ = LuaManager::GetSingleton()->CreateLua();

        Lua::RegisterGlobalFuncs(pLua_);
        Lua::RegisterUnitClass(pLua_);
        Lua::RegisterMovableObjectClass(pLua_);
        Lua::RegisterGameplayClass(pLua_);
        pLua_->DoFile("DB/Vector.lua");
        pLua_->DoFile("KeyCodes.lua");
        Engine::GetSingleton()->CreateGlue(pLua_);
    }

    s_ptr<Gameplay> GameplayManager::GetGameplay( const s_str& sName )
    {
        s_map< s_str, s_ptr<Gameplay> >::iterator iter = lGameplayList_.Get(sName);
        if (iter != lGameplayList_.End())
            return iter->second;
        else
            return nullptr;
    }

    void GameplayManager::SetCurrentGameplay( s_ptr<Gameplay> pGameplay )
    {
        if (pGameplay != pCurrentGameplay_)
        {
            if (pCurrentGameplay_)
            {
                pCurrentGameplay_->On("Disabled");
                pCurrentGameplay_->NotifyCurrentGameplay(false);
            }

            pCurrentGameplay_ = pGameplay;

            if (pCurrentGameplay_)
            {
                pCurrentGameplay_->On("Enabled");
                pCurrentGameplay_->NotifyCurrentGameplay(true);
            }
        }
    }

    void GameplayManager::SetCurrentGameplay( const s_str& sGameplay )
    {
        s_map< s_str, s_ptr<Gameplay> >::iterator iter;
        iter = lGameplayList_.Get(sGameplay);

        if (iter != lGameplayList_.End())
            SetCurrentGameplay(iter->second);
        else
        {
            Warning(CLASS_NAME,
                "Gameplay \""+sGameplay+"\" doesn't exist."
            );
        }
    }

    void GameplayManager::Update( const s_float& fDelta )
    {
        if (pCurrentGameplay_)
            pCurrentGameplay_->On("Update");
    }

    void GameplayManager::ParseData()
    {
        Log<1>("Loading gameplays...");
        Directory mDir("Gameplays");
        s_ctnr<s_str> lXMLFileList = mDir.GetFileList(true, "xml");
        s_ctnr<s_str>::iterator iter;
        foreach (iter, lXMLFileList)
        {
            ParseXMLFile_(*iter);
        }
        Log<1>("Done.");
    }

    s_ptr<Lua::State> GameplayManager::GetLua()
    {
        return pLua_;
    }
}


