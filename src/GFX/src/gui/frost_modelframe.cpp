/* ##########/############################ */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*                                        */

#include "gui/frost_modelframe.h"

#include <frost_frame.h>
#include <frost_texture.h>
#include <frost_guimanager.h>
#include <frost_rendertarget.h>
#include <impl/ogre/ogre_rendertarget.h>
#include "camera/frost_camera.h"
#include "scene/frost_light.h"
#include "scene/frost_lightmanager.h"
#include "model/frost_model.h"
#include "model/frost_modelmanager.h"
#include "model/frost_animmanager.h"
#include "material/frost_materialmanager.h"
#include "material/frost_material.h"
#include "frost_engine.h"

#include <frost_inputmanager.h>
#include <frost_utils_event.h>
#include <frost_utils_timemanager.h>

#include <OgreRoot.h>
#include <OgreRenderTarget.h>

using namespace std;
using namespace Frost;
using namespace Frost::GUI;

const s_str ModelFrame::CLASS_NAME = "GUI::ModelFrame";

ModelFrame::ModelFrame( s_ptr<GUIManager> pManager ) : Frame(pManager)
{
    lType_.PushBack("ModelFrame");

    mModelDirection_ = -Vector::UNIT_Z;
    fModelScale_ = 1.0f;

    bUpdateRenderTarget_ = true;

    uiModelAnimation_ = s_uint::NaN;

    RegisterForDrag((s_str("LeftButton"), s_str("RightButton")));

    mLightColor_ = Color::WHITE;
    mLightDirection_ = Vector(-1, -1, -1);
}

ModelFrame::~ModelFrame()
{
    pModel_ = nullptr;

    pLight_.Delete();
    pCamera_.Delete();

    if (pSceneManager_)
        Engine::GetSingleton()->GetOgreRoot()->destroySceneManager(pSceneManager_.Get());
}

void ModelFrame::CopyFrom( s_ptr<UIObject> pObj )
{
    Frame::CopyFrom(pObj);

    s_ptr<ModelFrame> pModelFrame = s_ptr<ModelFrame>::DynamicCast(pObj);

    if (pModelFrame)
    {
        this->EnableLighting(pModelFrame->IsLightingEnabled());
        this->SetAmbientColor(pModelFrame->GetAmbientColor());
        this->SetLightDirection(pModelFrame->GetLightDirection());
        this->SetLightColor(pModelFrame->GetLightColor());
        this->SetModelFile(pModelFrame->GetModelFile());
        this->SetModelDirection(pModelFrame->GetModelDirection());
        this->SetModelScale(pModelFrame->GetModelScale());
        this->SetModelPosition(pModelFrame->GetModelPosition());
        this->SetModelAnimation(pModelFrame->GetModelAnimation());
    }
}

void ModelFrame::OnEvent( const Event& mEvent )
{
    Frame::OnEvent(mEvent);

    if (mEvent.GetName() == "MOUSE_WHEEL_SMOOTH" && bMouseInFrame_)
    {
        if (pCamera_)
        {
            s_float fDist = 0.5f*(pCamera_->GetPosition() - pCamera_->GetOrbitCenter()).GetLength();
            pCamera_->Translate(-mEvent[0].Get<s_float>()*fDist*Vector::UNIT_Z, true);
            NotifyRendererNeedRedraw();
            bRedrawRenderTarget_ = true;
        }
    }

    if (mEvent.GetName() == "KEY_PRESSED" && bMouseInFrame_)
    {
        s_uint uiChar = mEvent[0].Get<s_uint>();
        if (uiChar == KEY_HOME)
        {
            SetupCamera_();
            NotifyRendererNeedRedraw();
            bRedrawRenderTarget_ = true;
        }
    }
}

void ModelFrame::On( const s_str& sScriptName, s_ptr<Event> pEvent )
{
    Frame::On(sScriptName, pEvent);

    if (sScriptName == "SizeChanged")
    {
        bUpdateRenderTarget_ = true;
    }
}

void ModelFrame::EnableMouse( const s_bool& bIsMouseEnabled, const s_bool& bAllowWorldInput )
{
    if (!bVirtual_)
    {
        if (bIsMouseEnabled && !bIsMouseEnabled_)
        {
            EventReceiver::RegisterEvent("MOUSE_MOVED_SMOOTH");
            EventReceiver::RegisterEvent("MOUSE_PRESSED");
            EventReceiver::RegisterEvent("MOUSE_DOUBLE_CLICKED");
            EventReceiver::RegisterEvent("MOUSE_RELEASED");
        }
        else if (!bIsMouseEnabled && bIsMouseEnabled_)
        {
            EventReceiver::UnregisterEvent("MOUSE_MOVED_SMOOTH");
            EventReceiver::UnregisterEvent("MOUSE_PRESSED");
            EventReceiver::UnregisterEvent("MOUSE_DOUBLE_CLICKED");
            EventReceiver::UnregisterEvent("MOUSE_RELEASED");
        }
    }

    bAllowWorldInput_ = bAllowWorldInput;
    bIsMouseEnabled_ = bIsMouseEnabled;
}

void ModelFrame::EnableMouseWheel( const s_bool& bIsMouseWheelEnabled )
{
    if (!bVirtual_)
    {
        if (bIsMouseWheelEnabled && !bIsMouseWheelEnabled_)
            EventReceiver::RegisterEvent("MOUSE_WHEEL_SMOOTH");
        else if (!bIsMouseWheelEnabled && bIsMouseWheelEnabled_)
            EventReceiver::UnregisterEvent("MOUSE_WHEEL_SMOOTH");
    }

    bIsMouseWheelEnabled_ = bIsMouseWheelEnabled;
}

void ModelFrame::UpdateModelAnimation( const s_float& fDelta )
{
    if (pModel_ && pModel_->GetAnimMgr())
    {
        pModel_->GetAnimMgr()->Update(fDelta);
        NotifyRendererNeedRedraw();
        bRedrawRenderTarget_ = true;
    }
}

void ModelFrame::RemoveModel()
{
    if (pModel_)
    {
        pModel_ = nullptr;
        sModelFile_ = "";
        NotifyRendererNeedRedraw();
        bRedrawRenderTarget_ = true;
    }
}

const Vector& ModelFrame::GetModelDirection() const
{
    return mModelDirection_;
}

const s_bool& ModelFrame::IsLightingEnabled() const
{
    return bLightingEnabled_;
}

const Color& ModelFrame::GetAmbientColor() const
{
    return mAmbientColor_;
}

Vector ModelFrame::GetLightDirection() const
{
    if (pLight_)
        return pLight_->GetDirection();
    else
        return Vector::ZERO;
}

Color ModelFrame::GetLightColor() const
{
    if (pLight_)
        return pLight_->GetColor();
    else
        return Color::BLACK;
}

const s_str& ModelFrame::GetModelFile() const
{
    return sModelFile_;
}

const s_float& ModelFrame::GetModelScale() const
{
    return fModelScale_;
}

const Vector& ModelFrame::GetModelPosition() const
{
    return mModelPosition_;
}

const s_uint& ModelFrame::GetModelAnimation() const
{
    return uiModelAnimation_;
}

void ModelFrame::SetModelDirection( const Vector& mDirection )
{
    if (mDirection != mModelDirection_)
    {
        mModelDirection_ = mDirection;
        if (pModel_)
        {
            pModel_->SetDirection(mModelDirection_);
            NotifyRendererNeedRedraw();
            bRedrawRenderTarget_ = true;
        }
    }
}

void ModelFrame::EnableLighting( const s_bool& bLightingEnabled )
{
    if (bLightingEnabled_ != bLightingEnabled)
    {
        bLightingEnabled_ = bLightingEnabled;
        if (pModel_)
        {
            if (bLightingEnabled_)
                pSceneManager_->setAmbientLight(Color::FrostToOgre(mAmbientColor_));
            else
                pSceneManager_->setAmbientLight(Ogre::ColourValue::White);

            NotifyRendererNeedRedraw();
            bRedrawRenderTarget_ = true;
        }
    }
}

void ModelFrame::SetAmbientColor( const Color& mAmbientColor )
{
    if (mAmbientColor_ != mAmbientColor)
    {
        mAmbientColor_ = mAmbientColor;
        if (pSceneManager_)
        {
            pSceneManager_->setAmbientLight(Color::FrostToOgre(mAmbientColor_));

            if (pModel_ && bLightingEnabled_)
            {
                NotifyRendererNeedRedraw();
                bRedrawRenderTarget_ = true;
            }
        }
    }
}

void ModelFrame::SetLightDirection( const Vector& mLightDirection )
{
    if (mLightDirection != mLightDirection_)
    {
        mLightDirection_ = mLightDirection;
        if (pLight_)
        {
            pLight_->SetDirection(mLightDirection_);

            if (pModel_ && bLightingEnabled_)
            {
                NotifyRendererNeedRedraw();
                bRedrawRenderTarget_ = true;
            }
        }
    }
}

void ModelFrame::SetLightColor( const Color& mLightColor )
{
    if (mLightColor != mLightColor_)
    {
        mLightColor_ = mLightColor;
        if (pLight_)
        {
            pLight_->SetColor(mLightColor_);

            if (pModel_ && bLightingEnabled_)
            {
                NotifyRendererNeedRedraw();
                bRedrawRenderTarget_ = true;
            }
        }
    }
}

s_bool ModelFrame::SetModelFile( const s_str& sFile )
{
    if (sFile.IsEmpty())
    {
        RemoveModel();
        return true;
    }

    if (sFile != sModelFile_ && !IsVirtual())
    {
        if (!pSceneManager_)
        {
            pSceneManager_ = Engine::GetSingleton()->GetOgreRoot()->createSceneManager(
                Ogre::ST_GENERIC, ("FrostModelFrameSceneMgr_"+uiID_).Get()
            );

            if (bLightingEnabled_)
                pSceneManager_->setAmbientLight(Color::FrostToOgre(mAmbientColor_));
            else
                pSceneManager_->setAmbientLight(Ogre::ColourValue::White);

            pCamera_ = new Camera(pSceneManager_);
            pLight_ = new Light(Light::DIRECTIONAL, pSceneManager_);
            pLight_->SetDirection(mLightDirection_);
            pLight_->SetColor(mLightColor_);
        }

        if (!pRenderTexture_)
        {
            // Create the scroll texture
            pRenderTexture_ = new Texture(pManager_);
            pRenderTexture_->SetSpecial();
            pRenderTexture_->SetParent(this);
            pRenderTexture_->SetDrawLayer("ARTWORK");
            pRenderTexture_->SetName("$parentRenderTexture");

            if (!pManager_->AddUIObject(pRenderTexture_))
            {
                Warning(lType_.Back(),
                    "Trying to create render texture for \""+sName_+"\",\n"
                    "but its name was already taken : \""+pRenderTexture_->GetName()+"\". Skipped."
                );
                pRenderTexture_.Delete();
                return false;
            }

            pRenderTexture_->CreateGlue();
            AddRegion(pRenderTexture_);

            pRenderTexture_->SetAllPoints(this);

            if (pRenderTarget_)
                pRenderTexture_->SetTexture(pRenderTarget_.Get());

            pRenderTexture_->NotifyLoaded();
        }

        s_ptr<ModelManager> pModelMgr = ModelManager::GetSingleton();

        pModelMgr->LinkModelNameToFile("Temporary", sFile, sFile);

        if (pModelMgr->PreloadModel("Temporary", sFile))
        {
            pModel_ = nullptr;

            sModelFile_ = sFile;
            pModel_ = pModelMgr->CreateModel("Temporary", sModelFile_, "Model", pSceneManager_);

            pModel_->SetMaterial(MaterialManager::GetSingleton()->CreateMaterial3D(Color::WHITE));

            pModel_->SetScale(fScale_*Vector::UNIT);
            pModel_->SetDirection(mModelDirection_);
            pModel_->SetPosition(mModelPosition_);

            NotifyRendererNeedRedraw();
            bRedrawRenderTarget_ = true;

            SetupCamera_();
        }

        pModelMgr->ClearLink("Temporary", sModelFile_);
    }

    return true;
}

void ModelFrame::SetModelScale( const s_float& fScale )
{
    if (fScale != fScale_)
    {
        fScale_ = fScale;
        if (pModel_)
        {
            pModel_->SetScale(fScale_*Vector::UNIT);
            NotifyRendererNeedRedraw();
            bRedrawRenderTarget_ = true;
        }
    }
}

void ModelFrame::SetModelPosition( const Vector& mPosition )
{
    if (mPosition != mModelPosition_)
    {
        mModelPosition_ = mPosition;
        if (pModel_)
        {
            pModel_->SetPosition(mModelPosition_);
            NotifyRendererNeedRedraw();
            bRedrawRenderTarget_ = true;
        }
    }
}

void ModelFrame::SetModelAnimation( const s_uint& uiAnimID )
{
    if (pModel_ && pModel_->GetAnimMgr())
    {
        uiModelAnimation_ = uiAnimID;

        if (uiAnimID.IsValid())
        {
            pModel_->GetAnimMgr()->SetAnim((AnimID)uiAnimID.Get());
            pModel_->GetAnimMgr()->Play();
        }
        else
            pModel_->GetAnimMgr()->SetAnim(ANIM_NONE);

        NotifyRendererNeedRedraw();
        bRedrawRenderTarget_ = true;
    }
}

void ModelFrame::SetModelAnimationTime( const s_float& fTime )
{
    if (pModel_ && pModel_->GetAnimMgr())
    {
        pModel_->GetAnimMgr()->Update(fTime);
        NotifyRendererNeedRedraw();
        bRedrawRenderTarget_ = true;
    }
}

void ModelFrame::SetModelTexture( const s_str& sFile, const s_bool& bAlphaReject )
{
    if (pModel_)
    {
        s_refptr<Frost::Material> pMat = MaterialManager::GetSingleton()->CreateMaterial3D(sFile);
        pMat->SetAlphaReject(bAlphaReject);

        pModel_->SetMaterial(pMat);

        NotifyRendererNeedRedraw();
        bRedrawRenderTarget_ = true;

        mModelMat_.Clear();
        mModelMat_.AddMaterialDefinition(MaterialDefinition(sFile, bAlphaReject));
    }
}

void ModelFrame::SetModelTexture( const Color& mColor )
{
    if (pModel_)
    {
        s_refptr<Frost::Material> pMat = MaterialManager::GetSingleton()->CreateMaterial3D(mColor);
        pMat->SetShaders("SimpleColorNoPP");
        pModel_->SetMaterial(pMat);

        NotifyRendererNeedRedraw();
        bRedrawRenderTarget_ = true;

        mModelMat_.Clear();
        mModelMat_.AddMaterialDefinition(MaterialDefinition(mColor));
    }
}

void ModelFrame::SetSubMeshTexture( const s_uint& uiSubMeshID, const s_str& sFile, const s_bool& bAlphaReject )
{
    if (pModel_)
    {
        s_ptr<ModelPart> pModelPart = pModel_->GetModelPart(uiSubMeshID);
        if (pModelPart)
        {
            s_refptr<Frost::Material> pMat = MaterialManager::GetSingleton()->CreateMaterial3D(sFile);
            pMat->SetAlphaReject(bAlphaReject);

            pModelPart->SetMaterial(pMat);

            NotifyRendererNeedRedraw();
            bRedrawRenderTarget_ = true;

            mModelMat_.Clear(s_int(uiSubMeshID));
            mModelMat_.AddMaterialDefinition(MaterialDefinition(sFile, bAlphaReject), s_int(uiSubMeshID));
        }
    }
}

void ModelFrame::SetSubMeshTexture( const s_uint& uiSubMeshID, const Color& mColor )
{
    if (pModel_)
    {
        s_ptr<ModelPart> pModelPart = pModel_->GetModelPart(uiSubMeshID);
        if (pModelPart)
        {
            s_refptr<Frost::Material> pMat = MaterialManager::GetSingleton()->CreateMaterial3D(mColor);
            pMat->SetShaders("SimpleColorNoPP");
            pModelPart->SetMaterial(pMat);

            NotifyRendererNeedRedraw();
            bRedrawRenderTarget_ = true;

            mModelMat_.Clear(s_int(uiSubMeshID));
            mModelMat_.AddMaterialDefinition(MaterialDefinition(mColor), s_int(uiSubMeshID));
        }
    }
}

void ModelFrame::SetSubEntityTexture( const s_uint& uiSubMeshID, const s_uint& uiSubEntityID,
    const s_str& sFile, const s_bool& bAlphaReject)
{
    if (pModel_)
    {
        s_ptr<ModelPart> pModelPart = pModel_->GetModelPart(uiSubMeshID);
        if (pModelPart)
        {
            s_refptr<Frost::Material> pMat = MaterialManager::GetSingleton()->CreateMaterial3D(sFile);
            pMat->SetAlphaReject(bAlphaReject);

            pModelPart->SetMaterial(pMat, uiSubEntityID);

            NotifyRendererNeedRedraw();
            bRedrawRenderTarget_ = true;

            mModelMat_.AddMaterialDefinition(MaterialDefinition(sFile, bAlphaReject), s_int(uiSubMeshID), s_int(uiSubEntityID));
        }
    }
}

void ModelFrame::SetSubEntityTexture( const s_uint& uiSubMeshID, const s_uint& uiSubEntityID, const Color& mColor )
{
    if (pModel_)
    {
        s_ptr<ModelPart> pModelPart = pModel_->GetModelPart(uiSubMeshID);
        if (pModelPart)
        {
            s_refptr<Frost::Material> pMat = MaterialManager::GetSingleton()->CreateMaterial3D(mColor);
            pMat->SetShaders("SimpleColorNoPP");
            pModelPart->SetMaterial(pMat, uiSubEntityID);

            NotifyRendererNeedRedraw();
            bRedrawRenderTarget_ = true;

            mModelMat_.AddMaterialDefinition(MaterialDefinition(mColor), s_int(uiSubMeshID), s_int(uiSubEntityID));
        }
    }
}

void ModelFrame::SetModelMaterial( const ModelMaterial& mModelMat )
{
    mModelMat_ = mModelMat;

    if (pModel_)
        mModelMat_.ApplyOn(pModel_, false);

    NotifyRendererNeedRedraw();
    bRedrawRenderTarget_ = true;
}

void ModelFrame::HideModel()
{
    if (pModel_)
    {
        pModel_->Hide(true);

        NotifyRendererNeedRedraw();
        bRedrawRenderTarget_ = true;
    }
}

void ModelFrame::HideSubMesh( const s_uint& uiSubMeshID )
{
    if (pModel_)
    {
        s_ptr<ModelPart> pModelPart = pModel_->GetModelPart(uiSubMeshID);
        if (pModelPart)
        {
            pModelPart->Hide();

            NotifyRendererNeedRedraw();
            bRedrawRenderTarget_ = true;
        }
    }
}

void ModelFrame::HideSubEntity( const s_uint& uiSubMeshID, const s_uint& uiSubEntityID )
{
    if (pModel_)
    {
        s_ptr<ModelPart> pModelPart = pModel_->GetModelPart(uiSubMeshID);
        if (pModelPart)
        {
            pModelPart->Hide(uiSubEntityID);

            NotifyRendererNeedRedraw();
            bRedrawRenderTarget_ = true;
        }
    }
}

void ModelFrame::ShowModel()
{
    if (pModel_)
    {
        pModel_->Show(true);

        NotifyRendererNeedRedraw();
        bRedrawRenderTarget_ = true;
    }
}

void ModelFrame::ShowSubMesh( const s_uint& uiSubMeshID )
{
    if (pModel_)
    {
        s_ptr<ModelPart> pModelPart = pModel_->GetModelPart(uiSubMeshID);
        if (pModelPart)
        {
            pModel_->Show();
            pModelPart->Show();

            NotifyRendererNeedRedraw();
            bRedrawRenderTarget_ = true;
        }
    }
}

void ModelFrame::ShowSubEntity( const s_uint& uiSubMeshID, const s_uint& uiSubEntityID )
{
    if (pModel_)
    {
        s_ptr<ModelPart> pModelPart = pModel_->GetModelPart(uiSubMeshID);
        if (pModelPart)
        {
            pModel_->Show();
            pModelPart->Show(uiSubEntityID);

            NotifyRendererNeedRedraw();
            bRedrawRenderTarget_ = true;
        }
    }
}

s_wptr<Model> ModelFrame::GetModel()
{
    return pModel_;
}

const ModelMaterial& ModelFrame::GetModelMaterial() const
{
    return mModelMat_;
}

void ModelFrame::Update()
{
    Frame::Update();

    if (bMouseDragged_ && pCamera_)
    {
        s_bool bLeft = lMouseButtonList_.Find("LeftButton");
        s_bool bRight = lMouseButtonList_.Find("RightButton");

        s_float fRelDX = InputManager::GetSingleton()->GetMouseSmoothDX()/s_float(Engine::GetSingleton()->GetScreenWidth());
        s_float fRelDY = InputManager::GetSingleton()->GetMouseSmoothDY()/s_float(Engine::GetSingleton()->GetScreenHeight());

        if (bLeft && bRight)
        {
            if (!fRelDX.IsNull() || !fRelDY.IsNull())
            {
                s_float fDist = (pCamera_->GetPosition() - pCamera_->GetOrbitCenter()).GetLength();
                pCamera_->Translate(-fRelDX*fDist*Vector::UNIT_X + fRelDY*fDist*Vector::UNIT_Y, true);

                NotifyRendererNeedRedraw();
                bRedrawRenderTarget_ = true;
            }
        }
        else if (bLeft)
        {
            if (!fRelDX.IsNull() || !fRelDY.IsNull())
            {
                pCamera_->Yaw(-fRelDX);
                pCamera_->Pitch(-fRelDY);

                NotifyRendererNeedRedraw();
                bRedrawRenderTarget_ = true;
            }
        }
    }

    s_float fDelta = s_float(TimeManager::GetSingleton()->GetDelta());

    if (pLight_)
        pLight_->Update(fDelta);

    if (pCamera_)
        pCamera_->Update(fDelta);

    if (bUpdateRenderTarget_ && pCamera_)
    {
        UpdateRenderTarget_();

        bUpdateRenderTarget_ = false;
        bRedrawRenderTarget_ = true;
    }

    if (bRedrawRenderTarget_ && pRenderTarget_ && IsVisible())
    {
        // Update the render target
        pRenderTarget_->GetImpl()->GetOgreRenderTarget()->update();
        bRedrawRenderTarget_ = false;
    }
}

void ModelFrame::CreateGlue()
{
    if (bVirtual_)
    {
        s_ptr<Lua::State> pLua = pManager_->GetLua();
        pLua->PushNumber(uiID_);
        lGlueList_.PushBack(pLua->PushNew<LuaVirtualGlue>());
        pLua->SetGlobal(sLuaName_);
    }
    else
    {
        s_ptr<Lua::State> pLua = pManager_->GetLua();
        pLua->PushString(sLuaName_);
        lGlueList_.PushBack(pLua->PushNew<LuaModelFrame>());
        pLua->SetGlobal(sLuaName_);
    }
}

void ModelFrame::UpdateRenderTarget_()
{
    if (uiAbsWidth_.IsValid() && uiAbsHeight_.IsValid())
    {
        pCamera_->GetOgreCamera()->setAspectRatio(
            Ogre::Real(uiAbsWidth_.Get()) /
            Ogre::Real(uiAbsHeight_.Get())
        );
    }

    if (!pRenderTarget_)
    {
        if (uiAbsWidth_.IsValid() && uiAbsHeight_.IsValid())
        {
            pRenderTarget_ = s_refptr<RenderTarget>(new RenderTarget(uiAbsWidth_, uiAbsHeight_));
            s_ptr<Ogre::Viewport> pViewport = pRenderTarget_->GetImpl()->GetOgreRenderTarget()->addViewport(
                pCamera_->GetOgreCamera().Get(), 1
            );
            pViewport->setBackgroundColour(Ogre::ColourValue(0, 0, 0, 0));
            pViewport->setDimensions(
                0, 0,
                pRenderTarget_->GetWidth().Get()/(float)pRenderTarget_->GetRealWidth().Get(),
                pRenderTarget_->GetHeight().Get()/(float)pRenderTarget_->GetRealHeight().Get()
            );

            if (pRenderTexture_ && pRenderTarget_)
                pRenderTexture_->SetTexture(pRenderTarget_.Get());
        }
    }
    else
    {
        if (uiAbsWidth_.IsValid() && uiAbsHeight_.IsValid())
        {
            s_bool bTargetRecreated = pRenderTarget_->SetDimensions(uiAbsWidth_, uiAbsHeight_);

            if (bTargetRecreated)
            {
                s_ptr<Ogre::Viewport> pViewport = pRenderTarget_->GetImpl()->GetOgreRenderTarget()->addViewport(
                    pCamera_->GetOgreCamera().Get(), 1
                );
                pViewport->setBackgroundColour(Ogre::ColourValue(0, 0, 0, 0));
                pViewport->setDimensions(
                    0, 0,
                    pRenderTarget_->GetWidth().Get()/(float)pRenderTarget_->GetRealWidth().Get(),
                    pRenderTarget_->GetHeight().Get()/(float)pRenderTarget_->GetRealHeight().Get()
                );

                if (pRenderTexture_ && pRenderTarget_)
                    pRenderTexture_->SetTexture(pRenderTarget_.Get());
            }
        }
    }
}

void ModelFrame::SetupCamera_()
{
    if (!pCamera_ || !pModel_)
        return;

    const AxisAlignedBox& mBox = pModel_->GetBoundingBox();

    s_float fYFoV = 0.125f;

    s_float fDist = 0.5f*(mBox.GetMax().Y() - mBox.GetMin().Y())/tan(fYFoV/2.0f) +
                    0.5f*(mBox.GetMax().Z() - mBox.GetMin().Z());

    pCamera_->SetPosition(Vector(0, (mBox.GetMax().Y() + mBox.GetMin().Y())/2.0f, -fDist));
    pCamera_->OrbitAround(Vector(0, (mBox.GetMax().Y() + mBox.GetMin().Y())/2.0f, 0));
}
