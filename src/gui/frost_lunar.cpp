/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*                                        */

#include "gui/frost_uiobject.h"
#include "gui/frost_frame.h"
#include "gui/frost_layeredregion.h"
#include "gui/frost_button.h"
#include "gui/frost_checkbutton.h"
#include "gui/frost_colorselect.h"
#include "gui/frost_cooldown.h"
#include "gui/frost_editbox.h"
#include "gui/frost_messageframe.h"
#include "gui/frost_scrollframe.h"
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
    const char* LuaUIObject::classList[] = {"UIObject",
        "Frame", "Button", "CheckButton", "ColorSelect", "Cooldown", "EditBox", "MessageFrame",
        "ScrollFrame", "ScrollingMessageFrame", "Slider", "StatusBar", "LayeredRegion",
        "Texture", "FontString", 0
    };
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

    const char  LuaFrame::className[] = "Frame";
    const char* LuaFrame::classList[] = {"Frame",
        "Button", "CheckButton", "ColorSelect", "Cooldown", "EditBox", "MessageFrame",
        "ScrollFrame", "ScrollingMessageFrame", "Slider", "StatusBar", 0
    };
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

    const char  LuaCheckButton::className[] = "CheckButton";
    const char* LuaCheckButton::classList[] = {"CheckButton", 0};
    Lunar<LuaCheckButton>::RegType LuaCheckButton::methods[] = {
        {"dt", &LuaCheckButton::GetDataTable},
        {0,0}
    };

    const char  LuaColorSelect::className[] = "ColorSelect";
    const char* LuaColorSelect::classList[] = {"ColorSelect", 0};
    Lunar<LuaColorSelect>::RegType LuaColorSelect::methods[] = {
        {"dt", &LuaColorSelect::GetDataTable},
        {0,0}
    };

    const char  LuaCooldown::className[] = "Cooldown";
    const char* LuaCooldown::classList[] = {"Cooldown", 0};
    Lunar<LuaCooldown>::RegType LuaCooldown::methods[] = {
        {"dt", &LuaCooldown::GetDataTable},
        {0,0}
    };

    const char  LuaStatusBar::className[] = "StatusBar";
    const char* LuaStatusBar::classList[] = {"StatusBar", 0};
    Lunar<LuaStatusBar>::RegType LuaStatusBar::methods[] = {
        {"dt", &LuaStatusBar::GetDataTable},

        // UIObject (inherited)
        method(StatusBar, GetAlpha),
        method(StatusBar, GetName),
        method(StatusBar, GetObjectType),
        method(StatusBar, IsObjectType),
        method(StatusBar, SetAlpha),

        method(StatusBar, ClearAllPoints),
        method(StatusBar, GetBottom),
        method(StatusBar, GetCenter),
        method(StatusBar, GetHeight),
        method(StatusBar, GetLeft),
        method(StatusBar, GetNumPoint),
        method(StatusBar, GetParent),
        method(StatusBar, GetPoint),
        method(StatusBar, GetRight),
        method(StatusBar, GetTop),
        method(StatusBar, GetWidth),
        method(StatusBar, Hide),
        method(StatusBar, IsShown),
        method(StatusBar, IsVisible),
        method(StatusBar, RebuildCache),
        method(StatusBar, SetAllPoints),
        method(StatusBar, SetHeight),
        method(StatusBar, SetParent),
        method(StatusBar, SetPoint),
        method(StatusBar, SetWidth),
        method(StatusBar, Show),

        method(StatusBar, RebuildCache),

        // Frame (inherited)
        method(StatusBar, CreateFontString),
        method(StatusBar, CreateTexture),
        method(StatusBar, CreateTitleRegion),
        method(StatusBar, DisableDrawLayer),
        method(StatusBar, EnableDrawLayer),
        method(StatusBar, EnableKeyboard),
        method(StatusBar, EnableMouse),
        method(StatusBar, EnableMouseWheel),
        method(StatusBar, GetBackdrop),
        method(StatusBar, GetBackdropBorderColor),
        method(StatusBar, GetBackdropColor),
        method(StatusBar, GetChildren),
        method(StatusBar, GetEffectiveAlpha),
        method(StatusBar, GetEffectiveScale),
        method(StatusBar, GetFrameLevel),
        method(StatusBar, GetFrameStrata),
        method(StatusBar, GetFrameType),
        method(StatusBar, GetHitRectInsets),
        method(StatusBar, GetID),
        method(StatusBar, GetMaxResize),
        method(StatusBar, GetMinResize),
        method(StatusBar, GetNumChildren),
        method(StatusBar, GetNumRegions),
        method(StatusBar, GetScale),
        method(StatusBar, GetScript),
        method(StatusBar, GetTitleRegion),
        method(StatusBar, HasScript),
        method(StatusBar, HookScript),
        method(StatusBar, IsClampedToScreen),
        method(StatusBar, IsFrameType),
        method(StatusBar, IsKeyboardEnabled),
        method(StatusBar, IsMouseEnabled),
        method(StatusBar, IsMouseWheelEnabled),
        method(StatusBar, IsMovable),
        method(StatusBar, IsResizable),
        method(StatusBar, IsTopLevel),
        method(StatusBar, IsUserPlaced),
        method(StatusBar, Lower),
        method(StatusBar, Raise),
        method(StatusBar, RegisterAllEvents),
        method(StatusBar, RegisterEvent),
        method(StatusBar, RegisterForDrag),
        method(StatusBar, SetBackdrop),
        method(StatusBar, SetBackdropBorderColor),
        method(StatusBar, SetBackdropColor),
        method(StatusBar, SetClampedToScreen),
        method(StatusBar, SetFrameStrata),
        method(StatusBar, SetHitRectInsets),
        method(StatusBar, SetMaxResize),
        method(StatusBar, SetMinResize),
        method(StatusBar, SetMovable),
        method(StatusBar, SetResizable),
        method(StatusBar, SetScale),
        method(StatusBar, SetScript),
        method(StatusBar, SetTopLevel),
        method(StatusBar, SetUserPlaced),
        method(StatusBar, StartMoving),
        method(StatusBar, StartSizing),
        method(StatusBar, StopMovingOrSizing),
        method(StatusBar, UnregisterAllEvents),
        method(StatusBar, UnregisterEvent),

        // StatusBar
        method(StatusBar, GetMinMaxValues),
        method(StatusBar, GetOrientation),
        method(StatusBar, GetStatusBarColor),
        method(StatusBar, GetStatusBarTexture),
        method(StatusBar, GetValue),
        method(StatusBar, SetMinMaxValues),
        method(StatusBar, SetOrientation),
        method(StatusBar, SetStatusBarColor),
        method(StatusBar, SetStatusBarTexture),
        method(StatusBar, SetValue),

        {0,0}
    };

    const char  LuaEditBox::className[] = "EditBox";
    const char* LuaEditBox::classList[] = {"EditBox", 0};
    Lunar<LuaEditBox>::RegType LuaEditBox::methods[] = {
        {"dt", &LuaEditBox::GetDataTable},
        {0,0}
    };

    const char  LuaMessageFrame::className[] = "MessageFrame";
    const char* LuaMessageFrame::classList[] = {"MessageFrame", 0};
    Lunar<LuaMessageFrame>::RegType LuaMessageFrame::methods[] = {
        {"dt", &LuaMessageFrame::GetDataTable},
        {0,0}
    };

    const char  LuaScrollFrame::className[] = "ScrollFrame";
    const char* LuaScrollFrame::classList[] = {"ScrollFrame", 0};
    Lunar<LuaScrollFrame>::RegType LuaScrollFrame::methods[] = {
        {"dt", &LuaScrollFrame::GetDataTable},
        {0,0}
    };

    const char  LuaScrollingMessageFrame::className[] = "ScrollingMessageFrame";
    const char* LuaScrollingMessageFrame::classList[] = {"ScrollingMessageFrame", 0};
    Lunar<LuaScrollingMessageFrame>::RegType LuaScrollingMessageFrame::methods[] = {
        {"dt", &LuaScrollingMessageFrame::GetDataTable},
        {0,0}
    };

    const char  LuaButton::className[] = "Button";
    const char* LuaButton::classList[] = {"Button", "CheckButton", 0};
    Lunar<LuaButton>::RegType LuaButton::methods[] = {
        {"dt", &LuaButton::GetDataTable},

        // UIObject (inherited)
        method(Button, GetAlpha),
        method(Button, GetName),
        method(Button, GetObjectType),
        method(Button, IsObjectType),
        method(Button, SetAlpha),

        method(Button, ClearAllPoints),
        method(Button, GetBottom),
        method(Button, GetCenter),
        method(Button, GetHeight),
        method(Button, GetLeft),
        method(Button, GetNumPoint),
        method(Button, GetParent),
        method(Button, GetPoint),
        method(Button, GetRight),
        method(Button, GetTop),
        method(Button, GetWidth),
        method(Button, Hide),
        method(Button, IsShown),
        method(Button, IsVisible),
        method(Button, RebuildCache),
        method(Button, SetAllPoints),
        method(Button, SetHeight),
        method(Button, SetParent),
        method(Button, SetPoint),
        method(Button, SetWidth),
        method(Button, Show),

        method(Button, RebuildCache),

        // Frame (inherited)
        method(Button, CreateFontString),
        method(Button, CreateTexture),
        method(Button, CreateTitleRegion),
        method(Button, DisableDrawLayer),
        method(Button, EnableDrawLayer),
        method(Button, EnableKeyboard),
        method(Button, EnableMouse),
        method(Button, EnableMouseWheel),
        method(Button, GetBackdrop),
        method(Button, GetBackdropBorderColor),
        method(Button, GetBackdropColor),
        method(Button, GetChildren),
        method(Button, GetEffectiveAlpha),
        method(Button, GetEffectiveScale),
        method(Button, GetFrameLevel),
        method(Button, GetFrameStrata),
        method(Button, GetFrameType),
        method(Button, GetHitRectInsets),
        method(Button, GetID),
        method(Button, GetMaxResize),
        method(Button, GetMinResize),
        method(Button, GetNumChildren),
        method(Button, GetNumRegions),
        method(Button, GetScale),
        method(Button, GetScript),
        method(Button, GetTitleRegion),
        method(Button, HasScript),
        method(Button, HookScript),
        method(Button, IsClampedToScreen),
        method(Button, IsFrameType),
        method(Button, IsKeyboardEnabled),
        method(Button, IsMouseEnabled),
        method(Button, IsMouseWheelEnabled),
        method(Button, IsMovable),
        method(Button, IsResizable),
        method(Button, IsTopLevel),
        method(Button, IsUserPlaced),
        method(Button, Lower),
        method(Button, Raise),
        method(Button, RegisterAllEvents),
        method(Button, RegisterEvent),
        method(Button, RegisterForDrag),
        method(Button, SetBackdrop),
        method(Button, SetBackdropBorderColor),
        method(Button, SetBackdropColor),
        method(Button, SetClampedToScreen),
        method(Button, SetFrameStrata),
        method(Button, SetHitRectInsets),
        method(Button, SetMaxResize),
        method(Button, SetMinResize),
        method(Button, SetMovable),
        method(Button, SetResizable),
        method(Button, SetScale),
        method(Button, SetScript),
        method(Button, SetTopLevel),
        method(Button, SetUserPlaced),
        method(Button, StartMoving),
        method(Button, StartSizing),
        method(Button, StopMovingOrSizing),
        method(Button, UnregisterAllEvents),
        method(Button, UnregisterEvent),

        // Button
        method(Button, Click),
        method(Button, Disable),
        method(Button, Enable),
        method(Button, GetButtonState),
        method(Button, GetDisabledFontObject),
        method(Button, GetDisabledTextColor),
        method(Button, GetDisabledTexture),
        method(Button, GetFont),
        method(Button, GetFontString),
        method(Button, GetHighlightFontObject),
        method(Button, GetHighlightTextColor),
        method(Button, GetHighlightTexture),
        method(Button, GetNormalFontObject),
        method(Button, GetNormalTexture),
        method(Button, GetPushedTextOffset),
        method(Button, GetPushedTexture),
        method(Button, GetText),
        method(Button, GetTextColor),
        method(Button, GetTextHeight),
        method(Button, GetTextWidth),
        method(Button, IsEnabled),
        method(Button, LockHighlight),
        method(Button, RegisterForClicks),
        method(Button, SetButtonState),
        method(Button, SetDisabledFontObject),
        method(Button, SetDisabledTextColor),
        method(Button, SetDisabledTexture),
        method(Button, SetFont),
        method(Button, SetFontString),
        method(Button, SetHighlightFontObject),
        method(Button, SetHighlightTextColor),
        method(Button, SetHighlightTexture),
        method(Button, SetNormalFontObject),
        method(Button, SetNormalTextColor),
        method(Button, SetNormalTexture),
        method(Button, SetPushedTextOffset),
        method(Button, SetPushedTexture),
        method(Button, SetText),
        method(Button, UnlockHighlight),

        {0,0}
    };

    const char  LuaSlider::className[] = "Slider";
    const char* LuaSlider::classList[] = {"Slider", 0};
    Lunar<LuaSlider>::RegType LuaSlider::methods[] = {
        {"dt", &LuaSlider::GetDataTable},
        {0,0}
    };

    const char  LuaLayeredRegion::className[] = "LayeredRegion";
    const char* LuaLayeredRegion::classList[] = {"LayeredRegion",
        "Texture", "FontString", 0
    };
    Lunar<LuaLayeredRegion>::RegType LuaLayeredRegion::methods[] = {
        {"dt", &LuaLayeredRegion::GetDataTable},
        {0,0}
    };

    const char  LuaTexture::className[] = "Texture";
    const char* LuaTexture::classList[] = {"Texture", 0};
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

    const char  LuaFontString::className[] = "FontString";
    const char* LuaFontString::classList[] = {"FontString", 0};
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
