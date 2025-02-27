/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*                                        */

#include "gui/frost_modelframe.h"

#include <frost_frame.h>
#include "model/frost_model.h"
#include "material/frost_material.h"
#include "material/frost_materialmanager.h"

#include <frost_xml_document.h>

using namespace std;
using namespace Frost;
using namespace Frost::GUI;

const s_str LuaModelFrame::CLASS_NAME = "GUI::LuaModelFrame";

LuaModelFrame::LuaModelFrame(lua_State* pLua) : LuaFrame(pLua)
{
    pModelFrameParent_ = s_ptr<ModelFrame>::DynamicCast(pParent_);
    if (pParent_ && !pModelFrameParent_)
    {
        throw Exception(CLASS_NAME, "Dynamic cast failed !");
    }
}

int LuaModelFrame::_AdvanceTime( lua_State* pLua )
{
    if (!CheckParent_())
        return 0;

    Lua::Function mFunc("ModelFrame:AdvanceTime", pLua);
    mFunc.Add(0, "time", Lua::TYPE_NUMBER);
    if (mFunc.Check())
    {
        pModelFrameParent_->UpdateModelAnimation(mFunc.Get(0)->GetNumber());
    }

    return mFunc.Return();
}


int LuaModelFrame::_ClearModel( lua_State* pLua )
{
    if (!CheckParent_())
        return 0;

    Lua::Function mFunc("ModelFrame:ClearModel", pLua);

    pModelFrameParent_->RemoveModel();

    return mFunc.Return();
}

int LuaModelFrame::_GetFacing( lua_State* pLua )
{
    if (!CheckParent_())
        return 0;

    Lua::Function mFunc("ModelFrame:GetFacing", pLua, 3);

    const Vector& mDirection = pModelFrameParent_->GetModelDirection();
    mFunc.Push(mDirection.X());
    mFunc.Push(mDirection.Y());
    mFunc.Push(mDirection.Z());

    return mFunc.Return();
}

int LuaModelFrame::_GetLight( lua_State* pLua )
{
    if (!CheckParent_())
        return 0;

    Lua::Function mFunc("ModelFrame:GetLight", pLua, 1);

    mFunc.Push(pModelFrameParent_->IsLightingEnabled());

    const Color& mColor = pModelFrameParent_->GetAmbientColor();
    mFunc.Push(mColor.GetR());
    mFunc.Push(mColor.GetG());
    mFunc.Push(mColor.GetB());

    Vector mDirection = pModelFrameParent_->GetLightDirection();
    mFunc.Push(mDirection.X());
    mFunc.Push(mDirection.Y());
    mFunc.Push(mDirection.Z());

    Color mLightColor = pModelFrameParent_->GetLightColor();
    mFunc.Push(mLightColor.GetR());
    mFunc.Push(mLightColor.GetG());
    mFunc.Push(mLightColor.GetB());

    return mFunc.Return();
}

int LuaModelFrame::_GetMaterial( lua_State* pLua )
{
    if (!CheckParent_())
        return 0;

    Lua::Function mFunc("ModelFrame:GetMaterial", pLua, 1);

    const ModelMaterial& mModelMat = pModelFrameParent_->GetModelMaterial();
    mFunc.Push(mModelMat.Serialize());

    return mFunc.Return();
}

int LuaModelFrame::_GetModel( lua_State* pLua )
{
    if (!CheckParent_())
        return 0;

    Lua::Function mFunc("ModelFrame:GetModel", pLua, 1);

    mFunc.Push(pModelFrameParent_->GetModelFile());

    return mFunc.Return();
}

int LuaModelFrame::_GetModelScale( lua_State* pLua )
{
    if (!CheckParent_())
        return 0;

    Lua::Function mFunc("ModelFrame:GetModelScale", pLua, 1);

    mFunc.Push(pModelFrameParent_->GetModelScale());

    return mFunc.Return();
}

int LuaModelFrame::_GetPosition( lua_State* pLua )
{
    if (!CheckParent_())
        return 0;

    Lua::Function mFunc("ModelFrame:GetPosition", pLua, 1);

    const Vector& mPosition = pModelFrameParent_->GetModelPosition();
    mFunc.Push(mPosition.X());
    mFunc.Push(mPosition.Y());
    mFunc.Push(mPosition.Z());

    return mFunc.Return();
}

int LuaModelFrame::_GetAvailableSubMeshes( lua_State* pLua )
{
    if (!CheckParent_())
        return 0;

    Lua::Function mFunc("ModelFrame:GetAvailableSubMeshes", pLua);

    s_wptr<Model> pModel = pModelFrameParent_->GetModel();
    if (pModel)
    {
        const s_map< s_uint, s_ptr<ModelPart> >& lModelPartList = pModel->GetModelPartList();
        s_map< s_uint, s_ptr<ModelPart> >::const_iterator iter;
        foreach (iter, lModelPartList)
        {
            mFunc.Push(iter->first);
        }
    }

    return mFunc.Return();
}

int LuaModelFrame::_GetSubEntityNumber( lua_State* pLua )
{
    if (!CheckParent_())
        return 0;

    Lua::Function mFunc("ModelFrame:GetSubEntityNumber", pLua, 1);
    mFunc.Add(0, "sub mesh ID", Lua::TYPE_NUMBER);

    s_wptr<Model> pModel = pModelFrameParent_->GetModel();
    if (pModel)
    {
        s_ptr<ModelPart> pModelPart = pModel->GetModelPart(s_uint(mFunc.Get(0)->GetNumber()));
        if (pModelPart)
            mFunc.Push(pModelPart->GetSubEntityList().GetSize());
    }

    return mFunc.Return();
}

int LuaModelFrame::_HideModel( lua_State* pLua )
{
    if (!CheckParent_())
        return 0;

    Lua::Function mFunc("ModelFrame:HideModel", pLua);

    pModelFrameParent_->HideModel();

    return mFunc.Return();
}

int LuaModelFrame::_HideSubMesh( lua_State* pLua )
{
    if (!CheckParent_())
        return 0;

    Lua::Function mFunc("ModelFrame:HideSubMesh", pLua);
    mFunc.Add(0, "sub mesh ID", Lua::TYPE_NUMBER);
    if (mFunc.Check())
    {
        pModelFrameParent_->HideSubMesh(s_uint(mFunc.Get(0)->GetNumber()));
    }

    return mFunc.Return();
}

int LuaModelFrame::_HideSubEntity( lua_State* pLua )
{
    if (!CheckParent_())
        return 0;

    Lua::Function mFunc("ModelFrame:HideSubEntity", pLua);
    mFunc.Add(0, "sub mesh ID", Lua::TYPE_NUMBER);
    mFunc.Add(1, "sub entity ID", Lua::TYPE_NUMBER);
    if (mFunc.Check())
    {
        pModelFrameParent_->HideSubEntity(s_uint(mFunc.Get(0)->GetNumber()), s_uint(mFunc.Get(1)->GetNumber()));
    }

    return mFunc.Return();
}

int LuaModelFrame::_SetFacing( lua_State* pLua )
{
    if (!CheckParent_())
        return 0;

    Lua::Function mFunc("ModelFrame:SetFacing", pLua);
    mFunc.Add(0, "x", Lua::TYPE_NUMBER);
    mFunc.Add(1, "y", Lua::TYPE_NUMBER);
    mFunc.Add(2, "z", Lua::TYPE_NUMBER);
    if (mFunc.Check())
    {
        Vector mDirection(
            mFunc.Get(0)->GetNumber(),
            mFunc.Get(1)->GetNumber(),
            mFunc.Get(2)->GetNumber()
        );
        pModelFrameParent_->SetModelDirection(mDirection);
    }

    return mFunc.Return();
}

int LuaModelFrame::_SetLight( lua_State* pLua )
{
    if (!CheckParent_())
        return 0;

    Lua::Function mFunc("ModelFrame:SetLight", pLua);
    mFunc.Add(0, "enabled", Lua::TYPE_BOOLEAN);
    mFunc.Add(1, "ambient R", Lua::TYPE_NUMBER, true);
    mFunc.Add(2, "ambient G", Lua::TYPE_NUMBER, true);
    mFunc.Add(3, "ambient B", Lua::TYPE_NUMBER, true);
    mFunc.Add(4, "dir X", Lua::TYPE_NUMBER, true);
    mFunc.Add(5, "dir Y", Lua::TYPE_NUMBER, true);
    mFunc.Add(6, "dir Z", Lua::TYPE_NUMBER, true);
    mFunc.Add(7, "dir R", Lua::TYPE_NUMBER, true);
    mFunc.Add(8, "dir G", Lua::TYPE_NUMBER, true);
    mFunc.Add(9, "dir B", Lua::TYPE_NUMBER, true);
    if (mFunc.Check())
    {
        pModelFrameParent_->EnableLighting(mFunc.Get(0)->GetBool());

        if (mFunc.IsProvided(1) && mFunc.IsProvided(2) && mFunc.IsProvided(3))
        {
            pModelFrameParent_->SetAmbientColor(Color(
                mFunc.Get(1)->GetNumber(),
                mFunc.Get(2)->GetNumber(),
                mFunc.Get(3)->GetNumber()
            ));
        }

        if (mFunc.IsProvided(4) && mFunc.IsProvided(5) && mFunc.IsProvided(6))
        {
            pModelFrameParent_->SetLightDirection(Vector(
                mFunc.Get(4)->GetNumber(),
                mFunc.Get(5)->GetNumber(),
                mFunc.Get(6)->GetNumber()
            ));
        }

        if (mFunc.IsProvided(7) && mFunc.IsProvided(8) && mFunc.IsProvided(9))
        {
            pModelFrameParent_->SetLightColor(Color(
                mFunc.Get(7)->GetNumber(),
                mFunc.Get(8)->GetNumber(),
                mFunc.Get(9)->GetNumber()
            ));
        }
    }

    return mFunc.Return();
}

int LuaModelFrame::_SetMaterial( lua_State* pLua )
{
    if (!CheckParent_())
        return 0;

    Lua::Function mFunc("ModelFrame:SetMaterial", pLua, 0);
    mFunc.Add(0, "material", Lua::TYPE_STRING);
    if (mFunc.Check())
    {
        XML::Document mDoc("DB/ModelMaterial.def");
        mDoc.SetSourceString(mFunc.Get(0)->GetString());
        mDoc.Check();
        pModelFrameParent_->SetModelMaterial(ModelMaterial(mDoc.GetMainBlock()));
    }

    return mFunc.Return();
}

int LuaModelFrame::_SetModel( lua_State* pLua )
{
    if (!CheckParent_())
        return 0;

    Lua::Function mFunc("ModelFrame:SetModel", pLua, 1);
    mFunc.Add(0, "model file", Lua::TYPE_STRING);
    if (mFunc.Check())
    {
        mFunc.Push(pModelFrameParent_->SetModelFile(mFunc.Get(0)->GetString()));
    }

    return mFunc.Return();
}

int LuaModelFrame::_SetModelScale( lua_State* pLua )
{
    if (!CheckParent_())
        return 0;

    Lua::Function mFunc("ModelFrame:SetModelScale", pLua);
    mFunc.Add(0, "scale", Lua::TYPE_NUMBER);
    if (mFunc.Check())
    {
        pModelFrameParent_->SetModelScale(mFunc.Get(0)->GetNumber());
    }

    return mFunc.Return();
}

int LuaModelFrame::_SetPosition( lua_State* pLua )
{
    if (!CheckParent_())
        return 0;

    Lua::Function mFunc("ModelFrame:SetPosition", pLua);
    mFunc.Add(0, "x", Lua::TYPE_NUMBER);
    mFunc.Add(1, "y", Lua::TYPE_NUMBER);
    mFunc.Add(2, "z", Lua::TYPE_NUMBER);
    if (mFunc.Check())
    {
        pModelFrameParent_->SetModelPosition(Vector(
            mFunc.Get(0)->GetNumber(),
            mFunc.Get(1)->GetNumber(),
            mFunc.Get(2)->GetNumber()
        ));
    }

    return mFunc.Return();
}

int LuaModelFrame::_SetSequence( lua_State* pLua )
{
    if (!CheckParent_())
        return 0;

    Lua::Function mFunc("ModelFrame:SetSequence", pLua);
    mFunc.Add(0, "animID", Lua::TYPE_NUMBER);
    if (mFunc.Check())
    {
        pModelFrameParent_->SetModelAnimation(s_uint(mFunc.Get(0)->GetNumber()));
    }

    return mFunc.Return();
}

int LuaModelFrame::_SetSequenceTime( lua_State* pLua )
{
    if (!CheckParent_())
        return 0;

    Lua::Function mFunc("ModelFrame:SetSequenceTime", pLua);
    mFunc.Add(0, "animID", Lua::TYPE_NUMBER);
    mFunc.Add(1, "time", Lua::TYPE_NUMBER);
    if (mFunc.Check())
    {
        pModelFrameParent_->SetModelAnimation(s_uint(mFunc.Get(0)->GetNumber()));
        pModelFrameParent_->SetModelAnimationTime(mFunc.Get(1)->GetNumber());
    }

    return mFunc.Return();
}

int LuaModelFrame::_SetModelTexture( lua_State* pLua )
{
    if (!CheckParent_())
        return 0;

    Lua::Function mFunc("ModelFrame:SetModelTexture", pLua);
    mFunc.Add(0, "file", Lua::TYPE_STRING);
    mFunc.Add(1, "alpha reject", Lua::TYPE_BOOLEAN, true);
    mFunc.NewParamSet();
    mFunc.Add(0, "red", Lua::TYPE_NUMBER);
    mFunc.Add(1, "green", Lua::TYPE_NUMBER);
    mFunc.Add(2, "blue", Lua::TYPE_NUMBER);
    mFunc.Add(3, "alpha", Lua::TYPE_NUMBER, true);
    if (mFunc.Check())
    {
        if (mFunc.GetParamSetRank() == 0)
        {
            if (mFunc.IsProvided(1))
                pModelFrameParent_->SetModelTexture(mFunc.Get(0)->GetString(), mFunc.Get(1)->GetBool());
            else
                pModelFrameParent_->SetModelTexture(mFunc.Get(0)->GetString());
        }
        else
        {
            pModelFrameParent_->SetModelTexture(Color(
                (mFunc.IsProvided(3) ? mFunc.Get(3)->GetNumber() : 1.0f),
                mFunc.Get(0)->GetNumber(),
                mFunc.Get(1)->GetNumber(),
                mFunc.Get(2)->GetNumber()
            ));
        }
    }

    return mFunc.Return();
}

int LuaModelFrame::_SetSubMeshTexture( lua_State* pLua )
{
    if (!CheckParent_())
        return 0;

    Lua::Function mFunc("ModelFrame:SetSubMeshTexture", pLua);
    mFunc.Add(0, "sub mesh ID", Lua::TYPE_NUMBER);
    mFunc.Add(1, "file", Lua::TYPE_STRING);
    mFunc.Add(2, "alpha reject", Lua::TYPE_BOOLEAN, true);
    mFunc.NewParamSet();
    mFunc.Add(0, "sub mesh ID", Lua::TYPE_NUMBER);
    mFunc.Add(1, "red", Lua::TYPE_NUMBER);
    mFunc.Add(2, "green", Lua::TYPE_NUMBER);
    mFunc.Add(3, "blue", Lua::TYPE_NUMBER);
    mFunc.Add(4, "alpha", Lua::TYPE_NUMBER, true);
    if (mFunc.Check())
    {
        if (mFunc.GetParamSetRank() == 0)
        {
            if (mFunc.IsProvided(2))
                pModelFrameParent_->SetSubMeshTexture(
                    s_uint(mFunc.Get(0)->GetNumber()),
                    mFunc.Get(1)->GetString(), mFunc.Get(2)->GetBool()
                );
            else
                pModelFrameParent_->SetSubMeshTexture(
                    s_uint(mFunc.Get(0)->GetNumber()), mFunc.Get(1)->GetString()
                );
        }
        else
        {
            pModelFrameParent_->SetSubMeshTexture(s_uint(mFunc.Get(0)->GetNumber()), Color(
                (mFunc.IsProvided(4) ? mFunc.Get(4)->GetNumber() : 1.0f),
                mFunc.Get(1)->GetNumber(), mFunc.Get(2)->GetNumber(), mFunc.Get(3)->GetNumber()
            ));
        }
    }

    return mFunc.Return();
}

int LuaModelFrame::_SetSubEntityTexture( lua_State* pLua )
{
    if (!CheckParent_())
        return 0;

    Lua::Function mFunc("ModelFrame:SetSubEntityTexture", pLua);
    mFunc.Add(0, "sub mesh ID", Lua::TYPE_NUMBER);
    mFunc.Add(1, "sub entity ID", Lua::TYPE_NUMBER);
    mFunc.Add(2, "file", Lua::TYPE_STRING);
    mFunc.Add(3, "alpha reject", Lua::TYPE_BOOLEAN, true);
    mFunc.NewParamSet();
    mFunc.Add(0, "sub mesh ID", Lua::TYPE_NUMBER);
    mFunc.Add(1, "sub entity ID", Lua::TYPE_NUMBER);
    mFunc.Add(2, "red", Lua::TYPE_NUMBER);
    mFunc.Add(3, "green", Lua::TYPE_NUMBER);
    mFunc.Add(4, "blue", Lua::TYPE_NUMBER);
    mFunc.Add(5, "alpha", Lua::TYPE_NUMBER, true);
    if (mFunc.Check())
    {
        if (mFunc.GetParamSetRank() == 0)
        {
            if (mFunc.IsProvided(3))
                pModelFrameParent_->SetSubEntityTexture(
                    s_uint(mFunc.Get(0)->GetNumber()), s_uint(mFunc.Get(1)->GetNumber()),
                    mFunc.Get(2)->GetString(), mFunc.Get(3)->GetBool()
                );
            else
                pModelFrameParent_->SetSubEntityTexture(
                    s_uint(mFunc.Get(0)->GetNumber()), s_uint(mFunc.Get(1)->GetNumber()),
                    mFunc.Get(2)->GetString()
                );
        }
        else
        {
            pModelFrameParent_->SetSubEntityTexture(
                s_uint(mFunc.Get(0)->GetNumber()), s_uint(mFunc.Get(1)->GetNumber()),
                Color((mFunc.IsProvided(5) ? mFunc.Get(5)->GetNumber() : 1.0f), mFunc.Get(2)->GetNumber(),
                    mFunc.Get(3)->GetNumber(), mFunc.Get(4)->GetNumber()
                )
            );
        }
    }

    return mFunc.Return();
}

int LuaModelFrame::_ShowModel( lua_State* pLua )
{
    if (!CheckParent_())
        return 0;

    Lua::Function mFunc("ModelFrame:ShowModel", pLua);

    pModelFrameParent_->ShowModel();

    return mFunc.Return();
}

int LuaModelFrame::_ShowSubMesh( lua_State* pLua )
{
    if (!CheckParent_())
        return 0;

    Lua::Function mFunc("ModelFrame:ShowSubMesh", pLua);
    mFunc.Add(0, "sub mesh ID", Lua::TYPE_NUMBER);
    if (mFunc.Check())
    {
        pModelFrameParent_->ShowSubMesh(s_uint(mFunc.Get(0)->GetNumber()));
    }

    return mFunc.Return();
}

int LuaModelFrame::_ShowSubEntity( lua_State* pLua )
{
    if (!CheckParent_())
        return 0;

    Lua::Function mFunc("ModelFrame:ShowSubEntity", pLua);
    mFunc.Add(0, "sub mesh ID", Lua::TYPE_NUMBER);
    mFunc.Add(1, "sub entity ID", Lua::TYPE_NUMBER);
    if (mFunc.Check())
    {
        pModelFrameParent_->ShowSubEntity(s_uint(mFunc.Get(0)->GetNumber()), s_uint(mFunc.Get(1)->GetNumber()));
    }

    return mFunc.Return();
}

#define method(widget, function) {#function, &Lua##widget::_##function}

const char  LuaModelFrame::className[] = "ModelFrame";
const char* LuaModelFrame::classList[] = {"ModelFrame", 0};
Lunar<LuaModelFrame>::RegType LuaModelFrame::methods[] = {
    {"dt", &LuaModelFrame::GetDataTable},

    // UIObject (inherited)
    method(ModelFrame, GetAlpha),
    method(ModelFrame, GetName),
    method(ModelFrame, GetObjectType),
    method(ModelFrame, IsObjectType),
    method(ModelFrame, SetAlpha),

    method(ModelFrame, ClearAllPoints),
    method(ModelFrame, GetBase),
    method(ModelFrame, GetBottom),
    method(ModelFrame, GetCenter),
    method(ModelFrame, GetHeight),
    method(ModelFrame, GetLeft),
    method(ModelFrame, GetNumPoint),
    method(ModelFrame, GetParent),
    method(ModelFrame, GetPoint),
    method(ModelFrame, GetRight),
    method(ModelFrame, GetTop),
    method(ModelFrame, GetWidth),
    method(ModelFrame, Hide),
    method(ModelFrame, IsShown),
    method(ModelFrame, IsVisible),
    method(ModelFrame, SetAllPoints),
    method(ModelFrame, SetHeight),
    method(ModelFrame, SetParent),
    method(ModelFrame, SetPoint),
    method(ModelFrame, SetWidth),
    method(ModelFrame, Show),

    // Frame (inherited)
    method(ModelFrame, CreateFontString),
    method(ModelFrame, CreateTexture),
    method(ModelFrame, CreateTitleRegion),
    method(ModelFrame, DisableDrawLayer),
    method(ModelFrame, EnableDrawLayer),
    method(ModelFrame, EnableKeyboard),
    method(ModelFrame, EnableMouse),
    method(ModelFrame, EnableMouseWheel),
    method(ModelFrame, GetBackdrop),
    method(ModelFrame, GetBackdropBorderColor),
    method(ModelFrame, GetBackdropColor),
    method(ModelFrame, GetChildren),
    method(ModelFrame, GetEffectiveAlpha),
    method(ModelFrame, GetEffectiveScale),
    method(ModelFrame, GetFrameLevel),
    method(ModelFrame, GetFrameStrata),
    method(ModelFrame, GetFrameType),
    method(ModelFrame, GetHitRectInsets),
    method(ModelFrame, GetID),
    method(ModelFrame, GetMaxResize),
    method(ModelFrame, GetMinResize),
    method(ModelFrame, SetMaxWidth),
    method(ModelFrame, SetMaxHeight),
    method(ModelFrame, SetMinWidth),
    method(ModelFrame, SetMinHeight),
    method(ModelFrame, GetNumChildren),
    method(ModelFrame, GetNumRegions),
    method(ModelFrame, GetScale),
    method(ModelFrame, GetScript),
    method(ModelFrame, GetTitleRegion),
    method(ModelFrame, HasScript),
    method(ModelFrame, IsClampedToScreen),
    method(ModelFrame, IsFrameType),
    method(ModelFrame, IsKeyboardEnabled),
    method(ModelFrame, IsMouseEnabled),
    method(ModelFrame, IsMouseWheelEnabled),
    method(ModelFrame, IsMovable),
    method(ModelFrame, IsResizable),
    method(ModelFrame, IsTopLevel),
    method(ModelFrame, IsUserPlaced),
    method(ModelFrame, Lower),
    method(ModelFrame, On),
    method(ModelFrame, Raise),
    method(ModelFrame, RegisterAllEvents),
    method(ModelFrame, RegisterEvent),
    method(ModelFrame, RegisterForDrag),
    method(ModelFrame, SetBackdrop),
    method(ModelFrame, SetBackdropBorderColor),
    method(ModelFrame, SetBackdropColor),
    method(ModelFrame, SetClampedToScreen),
    method(ModelFrame, SetFrameStrata),
    method(ModelFrame, SetHitRectInsets),
    method(ModelFrame, SetMaxResize),
    method(ModelFrame, SetMinResize),
    method(ModelFrame, SetMovable),
    method(ModelFrame, SetResizable),
    method(ModelFrame, SetScale),
    method(ModelFrame, SetScript),
    method(ModelFrame, SetTopLevel),
    method(ModelFrame, SetUserPlaced),
    method(ModelFrame, StartMoving),
    method(ModelFrame, StartSizing),
    method(ModelFrame, StopMovingOrSizing),
    method(ModelFrame, UnregisterAllEvents),
    method(ModelFrame, UnregisterEvent),

    // ModelFrame
    method(ModelFrame, AdvanceTime),
    method(ModelFrame, ClearModel),
    method(ModelFrame, GetFacing),
    method(ModelFrame, GetLight),
    method(ModelFrame, GetMaterial),
    method(ModelFrame, GetModel),
    method(ModelFrame, GetModelScale),
    method(ModelFrame, GetPosition),
    method(ModelFrame, GetAvailableSubMeshes),
    method(ModelFrame, GetSubEntityNumber),
    method(ModelFrame, HideModel),
    method(ModelFrame, HideSubMesh),
    method(ModelFrame, HideSubEntity),
    method(ModelFrame, SetFacing),
    method(ModelFrame, SetLight),
    method(ModelFrame, SetMaterial),
    method(ModelFrame, SetModel),
    method(ModelFrame, SetModelScale),
    method(ModelFrame, SetModelTexture),
    method(ModelFrame, SetPosition),
    method(ModelFrame, SetSequence),
    method(ModelFrame, SetSequenceTime),
    method(ModelFrame, SetSubMeshTexture),
    method(ModelFrame, SetSubEntityTexture),
    method(ModelFrame, ShowModel),
    method(ModelFrame, ShowSubMesh),
    method(ModelFrame, ShowSubEntity),

    {0,0}
};
