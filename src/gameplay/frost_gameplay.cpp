/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*             Gameplay source            */
/*                                        */
/*                                        */

#include "gameplay/frost_gameplay.h"
#include "gameplay/frost_gameplaymanager.h"
#include "camera/frost_cameramanager.h"
#include "camera/frost_camera.h"

using namespace std;

namespace Frost
{
    const s_str Gameplay::CLASS_NAME = "Gameplay";

    Gameplay::Gameplay( const s_str& sName ) : sName_(sName)
    {
    }

    Gameplay::~Gameplay()
    {
        s_ctnr< s_ptr<LuaGameplay> >::iterator iter;
        foreach (iter, lGlueList_)
        {
            iter->Delete();
        }
    }

    void Gameplay::SetCamera( s_ptr<Camera> pCamera )
    {
        pCamera_ = pCamera;
        if (bIsCurrentGameplay_)
            CameraManager::GetSingleton()->SetMainCamera(pCamera_);
    }

    s_ptr<Camera> Gameplay::CreateCamera(const Vector& mPosition)
    {
        s_ptr<Camera> pCam = CameraManager::GetSingleton()->CreateCamera(mPosition);
        lCameraList_.PushBack(pCam);
        pCam->CreateGlue(GameplayManager::GetSingleton()->GetLua());
        return pCam;
    }

    void Gameplay::NotifyCurrentGameplay( const s_bool& bIsCurrentGameplay )
    {
        if (bIsCurrentGameplay)
        {
            bIsCurrentGameplay_ = true;
            CameraManager::GetSingleton()->SetMainCamera(pCamera_);
            s_ctnr<s_str>::iterator iter;
            foreach (iter, lRegisteredEventList_)
                EventReceiver::RegisterEvent(*iter);
        }
        else
        {
            bIsCurrentGameplay_ = false;
            s_ctnr<s_str>::iterator iter;
            foreach (iter, lRegisteredEventList_)
                EventReceiver::UnregisterEvent(*iter);
        }
    }

    void Gameplay::CreateGlue()
    {
        s_ptr<Lua::State> pLua = GameplayManager::GetSingleton()->GetLua();
        pLua->PushString(sName_);
        lGlueList_.PushBack(
            pLua->Push<LuaGameplay>(new LuaGameplay(pLua->GetState()))
        );
        pLua->SetGlobal(sName_);
    }

    void Gameplay::NotifyScriptDefined( const s_str& sScriptName )
    {
        s_str sCutScriptName = sScriptName;
        sCutScriptName.EraseFromStart(2);
        lDefinedScriptList_.PushBack(sCutScriptName);
    }

    void Gameplay::On( const s_str& sScriptName )
    {
        if (lDefinedScriptList_.Find(sScriptName))
        {
            s_ptr<Lua::State> pLua = GameplayManager::GetSingleton()->GetLua();

            pLua->PushGlobal(sName_);
            pLua->SetGlobal("this");

            if (sScriptName == "Update")
            {
                // Set delta time
                pLua->PushNumber(TimeManager::GetSingleton()->GetDelta());
                pLua->SetGlobal("arg1");
            }

            pLua->CallFunction(sName_+":On"+sScriptName);
        }
    }

    void Gameplay::OnEvent( const Event& mEvent )
    {
        s_ptr<Lua::State> pLua = GameplayManager::GetSingleton()->GetLua();

        // Lua handlers do not need direct arguments.
        // Instead, we set the value of some global variables
        // (event : event name, arg1, arg2, ... arg9 : arguments)
        // that the user can use however he wants in his handler.

        // Set event name
        pLua->PushString(mEvent.GetName());
        pLua->SetGlobal("event");

        // Set arguments
        for (s_uint i; i < mEvent.GetNumParam(); ++i)
        {
            s_ptr<const s_var> pArg = mEvent.Get(i);
            pLua->PushVar(*pArg);
            pLua->SetGlobal("arg"+(i+1));
        }

        pLua->CallFunction(sName_+":OnEvent");
    }

    void Gameplay::SetFriendlySelection( const SelectionType& mSelection )
    {
        // NOTE : Not yet implemented (Gameplay::SetFriendlySelection).
    }

    void Gameplay::SetHostileSelection( const SelectionType& mSelection )
    {
        // NOTE : Not yet implemented (Gameplay::SetHostileSelection).
    }

    void Gameplay::RegisterEvent( const s_str& sEventName )
    {
        if (!lRegisteredEventList_.Find(sEventName))
            lRegisteredEventList_.PushBack(sEventName);
    }

    const s_str& Gameplay::GetName() const
    {
        return sName_;
    }
}
