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
        Camera(uiID), fSpeed_(20.0f), mDestination_(Vector::NaN)
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
                Vector mTranslation = GetDirection(false);
                mTranslation.Y(0.0f); mTranslation.Normalize();
                mTranslation *= fSpeed_*s_float(TimeManager::GetSingleton()->GetDelta());
                pTargetNode_->translate(Vector::FrostToOgre(mTranslation));
                mDestination_.SetNaN();
            }
            else if (mKey == KEY_S)
            {
                Vector mTranslation = GetDirection(false);
                mTranslation.Y(0.0f); mTranslation.Normalize();
                mTranslation *= -fSpeed_*s_float(TimeManager::GetSingleton()->GetDelta());
                pTargetNode_->translate(Vector::FrostToOgre(mTranslation));
                mDestination_.SetNaN();
            }
            else if (mKey == KEY_A)
            {
                Vector mTranslation = GetDirection(false);
                mTranslation.Y(0.0f);
                mTranslation.Rotate(0.25, Vector::UNIT_Y);
                mTranslation.Normalize();
                mTranslation *= fSpeed_*s_float(TimeManager::GetSingleton()->GetDelta());
                pTargetNode_->translate(Vector::FrostToOgre(mTranslation));
                mDestination_.SetNaN();
            }
            else if (mKey == KEY_D)
            {
                Vector mTranslation = GetDirection(false);
                mTranslation.Y(0.0f);
                mTranslation.Rotate(0.25, Vector::UNIT_Y);
                mTranslation.Normalize();
                mTranslation *= -fSpeed_*s_float(TimeManager::GetSingleton()->GetDelta());
                pTargetNode_->translate(Vector::FrostToOgre(mTranslation));
                mDestination_.SetNaN();
            }
        }
        else if (mEvent.GetName() == "MOUSE_MOVED")
        {
            if (InputManager::GetSingleton()->MouseIsDown(MOUSE_RIGHT))
            {
                Yaw(-mEvent[0].GetF()/s_float(Engine::GetSingleton()->GetScreenWidth()));
                Pitch(-mEvent[1].GetF()/s_float(Engine::GetSingleton()->GetScreenHeight()));
            }
        }
        else if (mEvent.GetName() == "MOUSE_WHEEL")
        {
            Translate(Vector::UNIT_Z*(-s_float(mEvent[0].GetI())/120.0f), true);
        }
    }

    void TopCamera::MoveTo( const Vector& mDestination )
    {
        mDestination_ = mDestination;
    }

    void TopCamera::Update( const s_float& fDelta )
    {
        if (!mDestination_.IsNaN())
        {
            Vector mTranslation = mDestination_ - Vector::OgreToFrost(pTargetNode_->_getDerivedPosition());
            if (mTranslation.GetNorm().IsNull())
            {
                mDestination_.SetNaN();
            }
            else
            {
                mTranslation *= fSpeed_*s_float(TimeManager::GetSingleton()->GetDelta())*
                    s_float::Clamp(mTranslation.GetNorm()/30.0f, 0.1f, 1.0f);
                pTargetNode_->translate(Vector::FrostToOgre(mTranslation));
            }
        }
    }
}
