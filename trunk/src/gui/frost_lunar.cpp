/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*                                        */

#include "frost_gui.h"
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

#define method(widget, function) {#function, &widget::_##function}

namespace Frost
{
namespace GUI
{
    const char UIObject::className[] = "UIObject";
    Lunar<UIObject>::RegType UIObject::methods[] = {
        {"dt", &UIObject::GetDataTable},

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

    const char Frame::className[] = "Frame";
    Lunar<Frame>::RegType Frame::methods[] = {
        {"dt", &Frame::GetDataTable},
        {0,0}
    };

    const char StatusBar::className[] = "StatusBar";
    Lunar<StatusBar>::RegType StatusBar::methods[] = {
        {"dt", &StatusBar::GetDataTable},
        {0,0}
    };

    const char EditBox::className[] = "EditBox";
    Lunar<EditBox>::RegType EditBox::methods[] = {
        {"dt", &EditBox::GetDataTable},
        {0,0}
    };

    const char ScrollingMessageFrame::className[] = "ScrollingMessageFrame";
    Lunar<ScrollingMessageFrame>::RegType ScrollingMessageFrame::methods[] = {
        {"dt", &ScrollingMessageFrame::GetDataTable},
        {0,0}
    };

    const char Button::className[] = "Button";
    Lunar<Button>::RegType Button::methods[] = {
        {"dt", &Button::GetDataTable},
        {0,0}
    };

    const char Slider::className[] = "Slider";
    Lunar<Slider>::RegType Slider::methods[] = {
        {"dt", &Slider::GetDataTable},
        {0,0}
    };

    const char LayeredRegion::className[] = "LayeredRegion";
    Lunar<LayeredRegion>::RegType LayeredRegion::methods[] = {
        {"dt", &LayeredRegion::GetDataTable},
        {0,0}
    };

    const char Texture::className[] = "Texture";
    Lunar<Texture>::RegType Texture::methods[] = {
        {"dt", &Texture::GetDataTable},
        {0,0}
    };

    const char FontString::className[] = "FontString";
    Lunar<FontString>::RegType FontString::methods[] = {
        {"dt", &FontString::GetDataTable},
        {0,0}
    };
}
}
