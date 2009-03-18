/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*          ChasingCamera source          */
/*                                        */
/*                                        */

#include "camera/frost_camera_chasingcamera.h"
#include "frost_unit.h"
#include "frost_inputmanager.h"

using namespace std;

namespace Frost
{
    ChasingCamera::ChasingCamera( const s_uint& uiID, s_ptr<Unit> pUnit ) :
        Camera(uiID), pUnit_(pUnit)
    {
        SetPosition(Vector(0, 4, 5));
        OrbitAround(Vector(0, 2, 0), true);
        Attach(s_ptr<MovableObject>(pUnit_->GetNode()), false, false);
    }

    ChasingCamera::~ChasingCamera()
    {
        //Detach();
    }

    void ChasingCamera::NotifyMainCamera( const s_bool& bMainCamera )
    {
        if (bMainCamera)
        {
            this->RegisterEvent("KEY_PRESSED");
            this->RegisterEvent("KEY_RELEASED");
            this->RegisterEvent("MOUSE_MOVED");
            this->RegisterEvent("MOUSE_PRESSED");
            this->RegisterEvent("MOUSE_RELEASED");
            this->RegisterEvent("MOUSE_WHEEL");
        }
        else
        {
            this->UnregisterEvent("KEY_PRESSED");
            this->UnregisterEvent("KEY_RELEASED");
            this->UnregisterEvent("MOUSE_MOVED");
            this->UnregisterEvent("MOUSE_PRESSED");
            this->UnregisterEvent("MOUSE_RELEASED");
            this->UnregisterEvent("MOUSE_WHEEL");
        }
    }

    void ChasingCamera::OnEvent( const Event& mEvent )
    {
        if (mEvent.GetName() == "KEY_PRESSED")
        {
            KeyCode mKey = KeyCode(mEvent[0].GetUI().Get());
            if (mKey == KEY_W)
            {
                pUnit_->SetMoveForward(true);
            }
            else if (mKey == KEY_S)
            {
                pUnit_->SetMoveBackward(true);
            }
            else if (mKey == KEY_A)
            {
                pUnit_->SetMoveLeft(true);
            }
            else if (mKey == KEY_D)
            {
                pUnit_->SetMoveRight(true);
            }
            else if (mKey == KEY_SPACE)
            {
                pUnit_->Jump();
            }
            else if (mKey == KEY_DIVIDE)
            {
                pUnit_->ToggleWalking();
            }
        }
        else if (mEvent.GetName() == "KEY_RELEASED")
        {
            KeyCode mKey = KeyCode(mEvent[0].GetUI().Get());
            if (mKey == KEY_W)
            {
                pUnit_->SetMoveForward(false);
            }
            else if (mKey == KEY_S)
            {
                pUnit_->SetMoveBackward(false);
            }
            else if (mKey == KEY_A)
            {
                pUnit_->SetMoveLeft(false);
            }
            else if (mKey == KEY_D)
            {
                pUnit_->SetMoveRight(false);
            }
        }
        else if (mEvent.GetName() == "MOUSE_MOVED")
        {
            if (InputManager::GetSingleton()->MouseIsDown(MOUSE_LEFT))
            {
                pUnit_->RotateCamera(
                    -mEvent[0].GetF()/s_float(Engine::GetSingleton()->GetScreenWidth()),
                    -mEvent[1].GetF()/s_float(Engine::GetSingleton()->GetScreenWidth())
                );
            }
            else if (InputManager::GetSingleton()->MouseIsDown(MOUSE_RIGHT))
            {
                pUnit_->RotateModel(
                    -mEvent[0].GetF()/s_float(Engine::GetSingleton()->GetScreenWidth()),
                    -mEvent[1].GetF()/s_float(Engine::GetSingleton()->GetScreenWidth())
                );
            }
        }
        else if ( (mEvent.GetName() == "MOUSE_PRESSED") || (mEvent.GetName() == "MOUSE_RELEASED") )
        {
            if (MouseButton(mEvent[0].GetUI().Get()) == MOUSE_RIGHT)
            {
                pUnit_->ToggleTurning();
                if (mEvent.GetName() == "MOUSE_PRESSED")
                    pUnit_->RotateModel(0.0f, 0.0f);
            }
        }
        else if (mEvent.GetName() == "MOUSE_WHEEL")
        {
            pUnit_->ZoomCamera(-s_float(mEvent[0].GetI())/120.0f);
        }
    }
}
