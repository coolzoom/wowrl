/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*            TopCamera source            */
/*                                        */
/*                                        */

#include "camera/frost_camera_topcamera.h"
#include "frost_inputmanager.h"

#include <OgreSceneNode.h>

using namespace std;

namespace Frost
{
    TopCamera::TopCamera( const s_uint& uiID ) :
        Camera(uiID), fSpeed_(20.0f)
    {
        SetPosition(Vector(2, 8, 2));
        OrbitAround(Vector(0, 0, 0), true);
    }

    TopCamera::~TopCamera()
    {
    }

    void TopCamera::NotifyMainCamera( const s_bool& bMainCamera )
    {
        if (bMainCamera)
        {
            this->RegisterEvent("KEY_DOWN");
            this->RegisterEvent("MOUSE_MOVED");
            this->RegisterEvent("MOUSE_WHEEL");
        }
        else
        {
            this->UnregisterEvent("KEY_DOWN");
            this->UnregisterEvent("MOUSE_MOVED");
            this->UnregisterEvent("MOUSE_WHEEL");
        }
    }

    void TopCamera::SetSpeed( const s_float& fSpeed )
    {
        fSpeed_ = fSpeed;
    }

    const s_float& TopCamera::GetSpeed() const
    {
        return fSpeed_;
    }

    void TopCamera::OnEvent( const Event& mEvent )
    {
        if (mEvent.GetName() == "KEY_DOWN")
        {
            KeyCode mKey = KeyCode(mEvent[0].GetUI().Get());
            if (mKey == KEY_W)
            {
                Vector mTranslation = GetDirection();
                mTranslation.Y(0.0f); mTranslation.Normalize();
                mTranslation *= fSpeed_*s_float(TimeManager::GetSingleton()->GetDelta());
                pTargetNode_->translate(Vector::FrostToOgre(mTranslation));
            }
            else if (mKey == KEY_S)
            {
                Vector mTranslation = GetDirection();
                mTranslation.Y(0.0f); mTranslation.Normalize();
                mTranslation *= -fSpeed_*s_float(TimeManager::GetSingleton()->GetDelta());
                pTargetNode_->translate(Vector::FrostToOgre(mTranslation));
            }
            else if (mKey == KEY_A)
            {
                Vector mTranslation = GetDirection();
                mTranslation.Y(0.0f);
                mTranslation.Rotate(0.25, Vector::UNIT_Y);
                mTranslation.Normalize();
                mTranslation *= fSpeed_*s_float(TimeManager::GetSingleton()->GetDelta());
                pTargetNode_->translate(Vector::FrostToOgre(mTranslation));
            }
            else if (mKey == KEY_D)
            {
                Vector mTranslation = GetDirection();
                mTranslation.Y(0.0f);
                mTranslation.Rotate(0.25, Vector::UNIT_Y);
                mTranslation.Normalize();
                mTranslation *= -fSpeed_*s_float(TimeManager::GetSingleton()->GetDelta());
                pTargetNode_->translate(Vector::FrostToOgre(mTranslation));
            }
        }
        else if (mEvent.GetName() == "MOUSE_MOVED")
        {
            if (InputManager::GetSingleton()->MouseIsDown(MOUSE_LEFT) ||
                InputManager::GetSingleton()->MouseIsDown(MOUSE_RIGHT))
            {
                Yaw(-mEvent[0].GetF()/s_float(Engine::GetSingleton()->GetScreenWidth()));
                Pitch(-mEvent[1].GetF()/s_float(Engine::GetSingleton()->GetScreenWidth()));
            }
        }
        else if (mEvent.GetName() == "MOUSE_WHEEL")
        {
            Translate(Vector::UNIT_Z*(-s_float(mEvent[0].GetI())/120.0f), true);
        }
    }
}
