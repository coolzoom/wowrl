/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*               Lua source               */
/*                                        */

#include "lua/frost_lua.h"

#include "frost_inputmanager.h"
#include "frost_localemanager.h"
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

using namespace std;
using namespace Frost;
using namespace Frost::Lua;

/* [#] The following functions are called "glues". They aren't used in C++ but
*  in the Lua environnement. To get a description of what they do, you can refer
*  to :
*                 http://www.wowwiki.com/World_of_Warcraft_API
*
*  They aren't all described there, but the most complexe ones are. Their use is,
*  in many case, obvious.
*/

/** \cond NOT_REMOVE_FROM_DOC
*/

int Frost::l_GetLocale( lua_State* pLua )
{
    Lua::Function mFunc("GetLocale", pLua, 1);

    mFunc.Push(LocaleManager::GetSingleton()->GetLocale());

    return mFunc.Return();
}

int Frost::l_GetLocalizedString( lua_State* pLua )
{
    Lua::Function mFunc("GetLocalizedString", pLua, 1);
    mFunc.Add(0, "string id", Lua::TYPE_STRING);
    if (mFunc.Check())
    {
        s_str sID = mFunc.Get(0)->GetString();
        mFunc.Push(LocaleManager::GetSingleton()->GetLocalizedString(sID));
    }

    return mFunc.Return();
}

int Frost::l_GetTime( lua_State* pLua )
{
    Lua::Function mFunc("GetTime", pLua, 1);

    mFunc.Push(TimeManager::GetSingleton()->GetTime());

    return mFunc.Return();
}

int Frost::l_GetTimeOfTheDay( lua_State* pLua )
{
    Lua::Function mFunc("GetTimeOfTheDay", pLua, 4);

    mFunc.Push(TimeManager::GetSingleton()->GetHour());
    mFunc.Push(TimeManager::GetSingleton()->GetMinutes());
    mFunc.Push(TimeManager::GetSingleton()->GetSeconds());
    mFunc.Push(TimeManager::GetSingleton()->GetMicroseconds());

    return mFunc.Return();
}

int Frost::l_Exit( lua_State* pLua )
{
    Lua::Function mFunc("Exit", pLua);

    Engine::GetSingleton()->ShutDown();

    return mFunc.Return();
}

int Frost::l_CreateFrame( lua_State* pLua )
{
    Lua::Function mFunc("CreateFrame", pLua, 1);
    mFunc.Add(0, "type", Lua::TYPE_STRING);
    mFunc.Add(1, "name", Lua::TYPE_STRING);
    mFunc.Add(2, "parent", Lua::TYPE_USERDATA, true);
    mFunc.Add(2, "parent", Lua::TYPE_NIL, true);
    mFunc.Add(3, "inherits", Lua::TYPE_STRING, true);

    if (mFunc.Check())
    {
        s_str sType = mFunc.Get(0)->GetString();
        s_ptr<GUI::Frame> pNewFrame;
        if (sType == "Frame")
            pNewFrame = new GUI::Frame();
        else if (sType == "Button")
            pNewFrame = new GUI::Button();
        else if (sType == "CheckButton")
            pNewFrame = new GUI::CheckButton();
        else if (sType == "ColorSelect")
            pNewFrame = new GUI::ColorSelect();
        else if (sType == "Cooldown")
            pNewFrame = new GUI::Cooldown();
        else if (sType == "EditBox")
            pNewFrame = new GUI::EditBox();
        else if (sType == "MessageFrame")
            pNewFrame = new GUI::MessageFrame();
        else if (sType == "ScrollFrame")
            pNewFrame = new GUI::ScrollFrame();
        else if (sType == "ScrollingMessageFrame")
            pNewFrame = new GUI::ScrollingMessageFrame();
        else if (sType == "Slider")
            pNewFrame = new GUI::Slider();
        else if (sType == "StatusBar")
            pNewFrame = new GUI::StatusBar();
        else
        {
            Error(mFunc.GetName(),
                "Unknown frame type : \""+sType+"\"."
            );
            return mFunc.Return();
        }

        s_ptr<GUI::Frame> pParent;
        if (mFunc.IsProvided(2) && mFunc.Get(2)->GetType() == Lua::TYPE_USERDATA)
        {
            s_ptr<GUI::LuaUIObject> pObj = mFunc.Get(2)->Get<GUI::LuaUIObject>();
            if (pObj)
            {
                pParent = s_ptr<GUI::Frame>::DynamicCast(pObj->GetParent());
                if (pParent)
                {
                    pNewFrame->SetParent(pParent);
                }
                else
                {
                    Warning(mFunc.GetName(),
                        "The second argument of "+mFunc.GetName()+" must be a Frame "
                        "(got a "+pObj->GetParent()->GetObjectType()+")."
                    );
                }
            }
        }

        pNewFrame->SetName(mFunc.Get(1)->GetString());

        if (GUIManager::GetSingleton()->GetUIObjectByName(pNewFrame->GetName()))
        {
            Error(mFunc.GetName(),
                "An object with the name \""+pNewFrame->GetName()+"\" already exists."
            );
            pNewFrame.Delete();
            return mFunc.Return();
        }

        GUIManager::GetSingleton()->AddUIObject(pNewFrame);
        pNewFrame->CreateGlue();

        if (pParent)
        {
            pParent->AddChild(pNewFrame);
            pNewFrame->SetLevel(pParent->GetFrameLevel() + 1);
        }
        else
            pNewFrame->SetLevel(0);

        if (mFunc.Get(3)->GetType() == Lua::TYPE_STRING)
        {
            s_str sInheritance = mFunc.Get(3)->GetString();
            if (!sInheritance.IsEmpty(true))
            {
                s_ctnr<s_str> lObjects = sInheritance.Cut(",");
                s_ctnr<s_str>::iterator iter;
                foreach (iter, lObjects)
                {
                    iter->Trim(' ');
                    s_ptr<GUI::UIObject> pObj = GUIManager::GetSingleton()->GetUIObjectByName(*iter, true);
                    if (pObj)
                    {
                        if (pNewFrame->IsObjectType(pObj->GetObjectType()))
                        {
                            // Inherit from the other Frame
                            pNewFrame->CopyFrom(pObj);
                        }
                        else
                        {
                            Warning(mFunc.GetName(),
                                "\""+pNewFrame->GetName()+"\" ("+pNewFrame->GetObjectType()+") cannot inherit "
                                "from \""+(*iter)+"\" ("+pObj->GetObjectType()+"). Inheritance skipped."
                            );
                        }
                    }
                    else
                    {
                        Warning(mFunc.GetName(),
                            "Couldn't find inherited object \""+(*iter)+"\". Inheritance skipped."
                        );
                    }
                }
            }
        }

        pNewFrame->On("Load");

        pNewFrame->PushOnLua(mFunc.GetState());
        mFunc.NotifyPushed();
    }

    return mFunc.Return();
}

/** \endcond
*/
