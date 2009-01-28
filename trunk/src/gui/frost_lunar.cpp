/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*                                        */

#include "gui/frost_gui_uiobject.h"
#include "gui/frost_gui_frame.h"
#include "gui/frost_gui_layeredregion.h"
#include "gui/frost_gui_button.h"
#include "gui/frost_gui_editbox.h"
#include "gui/frost_gui_scrollingmessageframe.h"
#include "gui/frost_gui_slider.h"
#include "gui/frost_gui_statusbar.h"
#include "gui/frost_gui_fontstring.h"
#include "gui/frost_gui_texture.h"

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
        {0,0}
    };

    const char LuaFontString::className[] = "FontString";
    Lunar<LuaFontString>::RegType LuaFontString::methods[] = {
        {"dt", &LuaFontString::GetDataTable},
        {0,0}
    };
}
}
