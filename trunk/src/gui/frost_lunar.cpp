/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*                                        */

#include "gui/frost_uiobject.h"
#include "gui/frost_frame.h"
#include "gui/frost_layeredregion.h"
#include "gui/frost_button.h"
#include "gui/frost_editbox.h"
#include "gui/frost_scrollingmessageframe.h"
#include "gui/frost_slider.h"
#include "gui/frost_statusbar.h"
#include "gui/frost_fontstring.h"
#include "gui/frost_texture.h"

/****************/
/* Methods list */
/****************/

/* [#] Here we register every member function of each class. As Lunar doesn't
*  know about inheritance, you're obliged to register every member function of a
*  C++ class, including the ones inherited. Else you won't be able to call them
*  in LUA.
*/

#define method(widget, function) {#function, &Lua##widget::_##function}

namespace Frost
{
namespace GUI
{
    const char LuaUIObject::className[] = "UIObject";
    Lunar<LuaUIObject>::RegType LuaUIObject::methods[] = {
        {"dt", &LuaUIObject::GetDataTable},

        // UIObject
        method(UIObject, GetAlpha),
        method(UIObject, GetName),
        method(UIObject, GetObjectType),
        method(UIObject, IsObjectType),
        method(UIObject, SetAlpha),

        method(UIObject, ClearAllPoints),
        method(UIObject, GetBottom),
        method(UIObject, GetCenter),
        method(UIObject, GetHeight),
        method(UIObject, GetLeft),
        method(UIObject, GetNumPoint),
        method(UIObject, GetParent),
        method(UIObject, GetPoint),
        method(UIObject, GetRight),
        method(UIObject, GetTop),
        method(UIObject, GetWidth),
        method(UIObject, Hide),
        method(UIObject, IsShown),
        method(UIObject, IsVisible),
        method(UIObject, RebuildCache),
        method(UIObject, SetAllPoints),
        method(UIObject, SetHeight),
        method(UIObject, SetParent),
        method(UIObject, SetPoint),
        method(UIObject, SetWidth),
        method(UIObject, Show),

        method(UIObject, RebuildCache),
        {0,0}
    };

    const char LuaFrame::className[] = "Frame";
    Lunar<LuaFrame>::RegType LuaFrame::methods[] = {
        {"dt", &LuaFrame::GetDataTable},

        // UIObject (inherited)
        method(Frame, GetAlpha),
        method(Frame, GetName),
        method(Frame, GetObjectType),
        method(Frame, IsObjectType),
        method(Frame, SetAlpha),

        method(Frame, ClearAllPoints),
        method(Frame, GetBottom),
        method(Frame, GetCenter),
        method(Frame, GetHeight),
        method(Frame, GetLeft),
        method(Frame, GetNumPoint),
        method(Frame, GetParent),
        method(Frame, GetPoint),
        method(Frame, GetRight),
        method(Frame, GetTop),
        method(Frame, GetWidth),
        method(Frame, Hide),
        method(Frame, IsShown),
        method(Frame, IsVisible),
        method(Frame, RebuildCache),
        method(Frame, SetAllPoints),
        method(Frame, SetHeight),
        method(Frame, SetParent),
        method(Frame, SetPoint),
        method(Frame, SetWidth),
        method(Frame, Show),

        method(Frame, RebuildCache),

        // Frame
        method(Frame, CreateFontString),
        method(Frame, CreateTexture),
        method(Frame, CreateTitleRegion),
        method(Frame, DisableDrawLayer),
        method(Frame, EnableDrawLayer),
        method(Frame, EnableKeyboard),
        method(Frame, EnableMouse),
        method(Frame, EnableMouseWheel),
        method(Frame, GetBackdrop),
        method(Frame, GetBackdropBorderColor),
        method(Frame, GetBackdropColor),
        method(Frame, GetChildren),
        method(Frame, GetEffectiveAlpha),
        method(Frame, GetEffectiveScale),
        method(Frame, GetFrameLevel),
        method(Frame, GetFrameStrata),
        method(Frame, GetFrameType),
        method(Frame, GetHitRectInsets),
        method(Frame, GetID),
        method(Frame, GetMaxResize),
        method(Frame, GetMinResize),
        method(Frame, GetNumChildren),
        method(Frame, GetNumRegions),
        method(Frame, GetScale),
        method(Frame, GetScript),
        method(Frame, GetTitleRegion),
        method(Frame, HasScript),
        method(Frame, HookScript),
        method(Frame, IsClampedToScreen),
        method(Frame, IsFrameType),
        method(Frame, IsKeyboardEnabled),
        method(Frame, IsMouseEnabled),
        method(Frame, IsMouseWheelEnabled),
        method(Frame, IsMovable),
        method(Frame, IsResizable),
        method(Frame, IsTopLevel),
        method(Frame, IsUserPlaced),
        method(Frame, Lower),
        method(Frame, Raise),
        method(Frame, RegisterAllEvents),
        method(Frame, RegisterEvent),
        method(Frame, RegisterForDrag),
        method(Frame, SetBackdrop),
        method(Frame, SetBackdropBorderColor),
        method(Frame, SetBackdropColor),
        method(Frame, SetClampedToScreen),
        method(Frame, SetFrameStrata),
        method(Frame, SetHitRectInsets),
        method(Frame, SetMaxResize),
        method(Frame, SetMinResize),
        method(Frame, SetMovable),
        method(Frame, SetResizable),
        method(Frame, SetScale),
        method(Frame, SetScript),
        method(Frame, SetTopLevel),
        method(Frame, SetUserPlaced),
        method(Frame, StartMoving),
        method(Frame, StartSizing),
        method(Frame, StopMovingOrSizing),
        method(Frame, UnregisterAllEvents),
        method(Frame, UnregisterEvent),

        {0,0}
    };

    const char LuaStatusBar::className[] = "StatusBar";
    Lunar<LuaStatusBar>::RegType LuaStatusBar::methods[] = {
        {"dt", &LuaStatusBar::GetDataTable},
        {0,0}
    };

    const char LuaEditBox::className[] = "EditBox";
    Lunar<LuaEditBox>::RegType LuaEditBox::methods[] = {
        {"dt", &LuaEditBox::GetDataTable},
        {0,0}
    };

    const char LuaScrollingMessageFrame::className[] = "ScrollingMessageFrame";
    Lunar<LuaScrollingMessageFrame>::RegType LuaScrollingMessageFrame::methods[] = {
        {"dt", &LuaScrollingMessageFrame::GetDataTable},
        {0,0}
    };

    const char LuaButton::className[] = "Button";
    Lunar<LuaButton>::RegType LuaButton::methods[] = {
        {"dt", &LuaButton::GetDataTable},
        {0,0}
    };

    const char LuaSlider::className[] = "Slider";
    Lunar<LuaSlider>::RegType LuaSlider::methods[] = {
        {"dt", &LuaSlider::GetDataTable},
        {0,0}
    };

    const char LuaLayeredRegion::className[] = "LayeredRegion";
    Lunar<LuaLayeredRegion>::RegType LuaLayeredRegion::methods[] = {
        {"dt", &LuaLayeredRegion::GetDataTable},
        {0,0}
    };

    const char LuaTexture::className[] = "Texture";
    Lunar<LuaTexture>::RegType LuaTexture::methods[] = {
        {"dt", &LuaTexture::GetDataTable},

        // UIObject (inherited)
        method(Texture, GetAlpha),
        method(Texture, GetName),
        method(Texture, GetObjectType),
        method(Texture, IsObjectType),
        method(Texture, SetAlpha),

        method(Texture, ClearAllPoints),
        method(Texture, GetBottom),
        method(Texture, GetCenter),
        method(Texture, GetHeight),
        method(Texture, GetLeft),
        method(Texture, GetNumPoint),
        method(Texture, GetParent),
        method(Texture, GetPoint),
        method(Texture, GetRight),
        method(Texture, GetTop),
        method(Texture, GetWidth),
        method(Texture, Hide),
        method(Texture, IsShown),
        method(Texture, IsVisible),
        method(Texture, RebuildCache),
        method(Texture, SetAllPoints),
        method(Texture, SetHeight),
        method(Texture, SetParent),
        method(Texture, SetPoint),
        method(Texture, SetWidth),
        method(Texture, Show),

        method(Texture, RebuildCache),

        // LayeredRegion (inherited)

        // Texture
        method(Texture, GetBlendMode),
        method(Texture, GetTexCoord),
        method(Texture, GetTexCoordModifiesRect),
        method(Texture, GetTexture),
        method(Texture, GetVertexColor),
        method(Texture, IsDesaturated),
        method(Texture, SetBlendMode),
        method(Texture, SetDesaturated),
        method(Texture, SetGradient),
        method(Texture, SetGradientAlpha),
        method(Texture, SetTexCoord),
        method(Texture, SetTexCoordModifiesRect),
        method(Texture, SetTexture),
        method(Texture, SetVertexColor),

        {0,0}
    };

    const char LuaFontString::className[] = "FontString";
    Lunar<LuaFontString>::RegType LuaFontString::methods[] = {
        {"dt", &LuaFontString::GetDataTable},

        // UIObject (inherited)
        method(FontString, GetAlpha),
        method(FontString, GetName),
        method(FontString, GetObjectType),
        method(FontString, IsObjectType),
        method(FontString, SetAlpha),

        method(FontString, ClearAllPoints),
        method(FontString, GetBottom),
        method(FontString, GetCenter),
        method(FontString, GetHeight),
        method(FontString, GetLeft),
        method(FontString, GetNumPoint),
        method(FontString, GetParent),
        method(FontString, GetPoint),
        method(FontString, GetRight),
        method(FontString, GetTop),
        method(FontString, GetWidth),
        method(FontString, Hide),
        method(FontString, IsShown),
        method(FontString, IsVisible),
        method(FontString, RebuildCache),
        method(FontString, SetAllPoints),
        method(FontString, SetHeight),
        method(FontString, SetParent),
        method(FontString, SetPoint),
        method(FontString, SetWidth),
        method(FontString, Show),

        method(FontString, RebuildCache),

        // LayeredRegion (inherited)

        // FontInstance
        method(FontString, GetFont),
        method(FontString, GetFontObject),
        method(FontString, GetJustifyH),
        method(FontString, GetJustifyV),
        method(FontString, GetShadowColor),
        method(FontString, GetShadowOffset),
        method(FontString, GetSpacing),
        method(FontString, GetTextColor),
        method(FontString, SetFont),
        method(FontString, SetFontObject),
        method(FontString, SetJustifyH),
        method(FontString, SetJustifyV),
        method(FontString, SetShadowColor),
        method(FontString, SetShadowOffset),
        method(FontString, SetSpacing),
        method(FontString, SetTextColor),

        // FontString
        method(FontString, CanNonSpaceWrap),
        method(FontString, GetStringHeight),
        method(FontString, GetStringWidth),
        method(FontString, GetText),
        method(FontString, SetAlphaGradient),
        method(FontString, SetNonSpaceWrap),
        method(FontString, SetText),
        method(FontString, SetTextHeight),

        {0,0}
    };
}
}
