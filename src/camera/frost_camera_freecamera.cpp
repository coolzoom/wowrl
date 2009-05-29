/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*            FreeCamera source           */
/*                                        */
/*                                        */

#include "camera/frost_camera_freecamera.h"
#include "frost_inputmanager.h"

using namespace std;

namespace Frost
{
    FreeCamera::FreeCamera( const s_uint& uiID ) :
        Camera(uiID), fSpeed_(20.0f)
    {
    }

    FreeCamera::~FreeCamera()
    {
    }

    void FreeCamera::NotifyMainCamera( const s_bool& bMainCamera )
    {
        if (bMainCamera)
        {
            this->RegisterEvent("KEY_DOWN");
            this->RegisterEvent("MOUSE_MOVED");
        }
        else
        {
            this->UnregisterEvent("KEY_DOWN");
            this->UnregisterEvent("MOUSE_MOVED");
        }
    }

    void FreeCamera::SetSpeed( const s_float& fSpeed )
    {
        fSpeed_ = fSpeed;
    }

    const s_float& FreeCamera::GetSpeed() const
    {
        return fSpeed_;
    }

    void FreeCamera::OnEvent( const Event& mEvent )
    {
        if (mEvent.GetName() == "KEY_DOWN")
        {
            KeyCode mKey = KeyCode(mEvent[0].GetUI().Get());
            if (mKey == KEY_W)
            {
                Translate(
                    Vector::UNIT_Z*(-fSpeed_)*s_float(TimeManager::GetSingleton()->GetDelta()), true
                );
            }
            else if (mKey == KEY_S)
            {
                Translate(
                    Vector::UNIT_Z*fSpeed_*s_float(TimeManager::GetSingleton()->GetDelta()), true
                );
            }
            else if (mKey == KEY_A)
            {
                Translate(
                    Vector::UNIT_X*(-fSpeed_)*s_float(TimeManager::GetSingleton()->GetDelta()), true
                );
            }
            else if (mKey == KEY_D)
            {
                Translate(
                    Vector::UNIT_X*fSpeed_*s_float(TimeManager::GetSingleton()->GetDelta()), true
                );
            }
            else if (mKey == KEY_Q)
            {
                Translate(
                    Vector::UNIT_Y*fSpeed_*s_float(TimeManager::GetSingleton()->GetDelta()), true
                );
            }
            else if (mKey == KEY_E)
            {
                Translate(
                    Vector::UNIT_Y*(-fSpeed_)*s_float(TimeManager::GetSingleton()->GetDelta()), true
                );
            }
        }
        else if (mEvent.GetName() == "MOUSE_MOVED")
        {
            if (InputManager::GetSingleton()->MouseIsDown(MOUSE_LEFT) ||
                InputManager::GetSingleton()->MouseIsDown(MOUSE_RIGHT))
            {
                Yaw(-mEvent[0].GetF()/s_float(Engine::GetSingleton()->GetScreenWidth()));
                Pitch(-mEvent[1].GetF()/s_float(Engine::GetSingleton()->GetScreenHeight()));
            }
        }
    }
}
