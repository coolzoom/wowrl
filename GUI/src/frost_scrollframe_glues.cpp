/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*                                        */

#include "frost_scrollframe.h"

#include "frost_frame.h"

using namespace std;
using namespace Frost;
using namespace Frost::GUI;

const s_str LuaScrollFrame::CLASS_NAME = "GUI::LuaScrollFrame";

LuaScrollFrame::LuaScrollFrame(lua_State* pLua) : LuaFrame(pLua)
{
    pScrollFrameParent_ = s_ptr<ScrollFrame>::DynamicCast(pParent_);
    if (pParent_ && !pScrollFrameParent_)
    {
        throw Exception(CLASS_NAME, "Dynamic cast failed !");
    }
}

int LuaScrollFrame::_GetHorizontalScroll( lua_State* pLua )
{
    if (!CheckParent_())
        return 0;

    Lua::Function mFunc("ScrollFrame:GetHorizontalScroll", pLua, 1);

    mFunc.Push(pScrollFrameParent_->GetHorizontalScroll());

    return mFunc.Return();
}

int LuaScrollFrame::_GetHorizontalScrollRange( lua_State* pLua )
{
    if (!CheckParent_())
        return 0;

    Lua::Function mFunc("ScrollFrame:GetHorizontalScrollRange", pLua, 1);

    mFunc.Push(pScrollFrameParent_->GetHorizontalScrollRange());

    return mFunc.Return();
}

int LuaScrollFrame::_GetScrollChild( lua_State* pLua )
{
    if (!CheckParent_())
        return 0;

    Lua::Function mFunc("ScrollFrame:GetScrollChild", pLua, 1);

    if (pScrollFrameParent_->GetScrollChild())
    {
        pScrollFrameParent_->GetScrollChild()->PushOnLua(mFunc.GetState());
        mFunc.NotifyPushed();
    }
    else
        mFunc.PushNil();

    return mFunc.Return();
}

int LuaScrollFrame::_GetVerticalScroll( lua_State* pLua )
{
    if (!CheckParent_())
        return 0;

    Lua::Function mFunc("ScrollFrame:GetVerticalScroll", pLua, 1);

    mFunc.Push(pScrollFrameParent_->GetVerticalScroll());

    return mFunc.Return();
}

int LuaScrollFrame::_GetVerticalScrollRange( lua_State* pLua )
{
    if (!CheckParent_())
        return 0;

    Lua::Function mFunc("ScrollFrame:GetVerticalScrollRange", pLua, 1);

    mFunc.Push(pScrollFrameParent_->GetVerticalScrollRange());

    return mFunc.Return();
}

int LuaScrollFrame::_SetHorizontalScroll( lua_State* pLua )
{
    if (!CheckParent_())
        return 0;

    Lua::Function mFunc("ScrollFrame:SetHorizontalScroll", pLua);
    mFunc.Add(0, "horizontal scroll", Lua::TYPE_NUMBER);
    if (mFunc.Check())
    {
        pScrollFrameParent_->SetHorizontalScroll(s_int(mFunc.Get(0)->GetNumber()));
    }

    return mFunc.Return();
}

int LuaScrollFrame::_SetScrollChild( lua_State* pLua )
{
    if (!CheckParent_())
        return 0;

    Lua::Function mFunc("ScrollFrame:SetScrollChild", pLua);
    mFunc.Add(0, "child name", Lua::TYPE_STRING, true);
    mFunc.Add(0, "child", Lua::TYPE_USERDATA, true);
    if (mFunc.Check())
    {
        s_ptr<Lua::Argument> pArg = mFunc.Get(0);
        s_ptr<Frame> pChild;

        if (pArg->IsProvided())
        {
            if (pArg->GetType() == Lua::TYPE_STRING)
            {
                s_ptr<UIObject> pObj = pParent_->GetManager()->GetUIObjectByName(pArg->GetString());
                pChild = s_ptr<Frame>::DynamicCast(pObj);

                if (!pChild && pObj)
                {
                    Warning(mFunc.GetName(),
                        "\""+pObj->GetName()+"\" is not a Frame UIObject."
                    );
                }
            }
            else
            {
                s_ptr<LuaUIObject> pObj = pArg->Get<LuaUIObject>();
                s_ptr<LuaFrame>    pFrame = s_ptr<LuaFrame>::DynamicCast(pObj);

                if (pFrame)
                    pChild = s_ptr<Frame>::DynamicCast(pFrame->GetParent());
                else if (pObj)
                {
                    Warning(mFunc.GetName(),
                        "\""+pObj->GetParent()->GetName()+"\" is not a Frame UIObject."
                    );
                }
            }
        }

        pScrollFrameParent_->SetScrollChild(pChild);
    }

    return mFunc.Return();
}

int LuaScrollFrame::_SetVerticalScroll( lua_State* pLua )
{
    if (!CheckParent_())
        return 0;

    Lua::Function mFunc("ScrollFrame:SetVerticalScroll", pLua);
    mFunc.Add(0, "vertical scroll", Lua::TYPE_NUMBER);
    if (mFunc.Check())
    {
        pScrollFrameParent_->SetVerticalScroll(s_int(mFunc.Get(0)->GetNumber()));
    }

    return mFunc.Return();
}
