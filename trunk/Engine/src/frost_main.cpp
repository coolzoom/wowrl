#include "frost.h"
#include "frost_editor.h"
#include <frost_guimanager.h>
#include <frost_uiobject.h>
#include <frost_frame.h>
#include <frost_fontstring.h>
#include "gui/frost_modelframe.h"
#include <frost_inputmanager.h>
#include "gameplay/frost_gameplaymanager.h"
#include "unit/frost_unitmanager.h"
#include "scene/frost_zonemanager.h"
#include "scene/frost_lightmanager.h"
#include "scene/frost_physicsmanager.h"
#include "frost_engine.h"

#include <OgreException.h>

using namespace std;
using namespace Frost;

s_bool GameFrameFunc()
{
    static s_ptr<InputManager> pInputMgr = InputManager::GetSingleton();

    if (pInputMgr->KeyIsPressed(KEY_ESCAPE))
    {
        Log("Escape pressed, exiting...");
        return false;
    }

    if (pInputMgr->KeyIsPressed(KEY_T))
    {
        Engine::GetSingleton()->TakeScreenshot();
    }

    if (pInputMgr->KeyIsPressed(KEY_Z) && pInputMgr->AltPressed())
    {
        s_ptr<GUI::UIObject> pObj = Engine::GetSingleton()->GetGUIManager()->GetUIObjectByName("UIParent");
        if (pObj->IsVisible())
            pObj->Hide();
        else
            pObj->Show();
    }

    if (pInputMgr->KeyIsPressed(KEY_P))
    {
        Engine::GetSingleton()->GetGUIManager()->PrintUI();
    }

    if (pInputMgr->KeyIsPressed(KEY_G))
    {
        s_ptr<Character>::DynamicCast(UnitManager::GetSingleton()->GetUnitByID(0))->EnablePhysics();
    }

    if (pInputMgr->KeyIsPressed(KEY_F1))
    {
        GameplayManager::GetSingleton()->SetCurrentGameplay("Free");
    }

    if (pInputMgr->KeyIsPressed(KEY_F2))
    {
        GameplayManager::GetSingleton()->SetCurrentGameplay("TopDown");
    }

    if (pInputMgr->KeyIsPressed(KEY_F3))
    {
        GameplayManager::GetSingleton()->SetCurrentGameplay("FirstPerson");
    }

    return true;
}

s_bool EditorFrameFunc()
{
    static s_ptr<InputManager> pInputMgr = InputManager::GetSingleton();

    if (pInputMgr->KeyIsPressed(KEY_T))
    {
        Engine::GetSingleton()->TakeScreenshot();
    }

    if (pInputMgr->KeyIsPressed(KEY_Z) && pInputMgr->AltPressed())
    {
        s_ptr<GUI::UIObject> pObj = Engine::GetSingleton()->GetGUIManager()->GetUIObjectByName("UIParent");
        if (pObj->IsVisible())
            pObj->Hide();
        else
            pObj->Show();
    }

    if (pInputMgr->KeyIsPressed(KEY_P))
    {
       Engine::GetSingleton()->GetGUIManager()->PrintUI();
    }

    if (pInputMgr->KeyIsPressed(KEY_R))
    {
        s_wptr<GUIManager> pGUIMgr = Engine::GetSingleton()->GetGUIManager();
        pGUIMgr->CloseUI();

        pGUIMgr->CreateLua();

        Engine::GetSingleton()->CreateGlue(pGUIMgr->GetLua());
        Editor::GetSingleton()->CreateGlue(pGUIMgr->GetLua());
        pGUIMgr->RegisterFrame<GUI::ModelFrame>("ModelFrame");

        pGUIMgr->ReadFiles();
    }

    return true;
}

int main(int argc, char* argv[])
{
    s_bool bEditor = true;

    // Read commands
    if (argc > 1)
    {
        s_str arg = argv[argc-1];
        if (arg == "-e")
        {
            bEditor = true;
        }
    }

    try
    {
        // Create the engine
        s_ptr<Engine> pFrost = Engine::GetSingleton();

        // Initialize base parameters
        pFrost->Initialize();

        if (bEditor)
        {
            pFrost->SetState(Engine::STATE_EDITOR);
            Log("Entering Editor mode...");

            pFrost->SetFrameFunction(&EditorFrameFunc);

            GameplayManager::GetSingleton()->SetCurrentGameplay("Editor");

            s_wptr<GUIManager> pGUIMgr = Engine::GetSingleton()->GetGUIManager();
            Engine::GetSingleton()->CreateGlue(pGUIMgr->GetLua());
            Editor::GetSingleton()->CreateGlue(pGUIMgr->GetLua());
            pGUIMgr->AddAddOnDirectory("Interface/BaseUI");
            pGUIMgr->AddAddOnDirectory("Interface/Editor");
            pGUIMgr->ReadFiles();

            LightManager::GetSingleton()->SetSunDirection(Vector(1, -1, 0));

            Editor::GetSingleton()->Initialize();
        }
        else
        {
            pFrost->SetState(Engine::STATE_GAME);

            // Set render and frame functions
            pFrost->SetFrameFunction(&GameFrameFunc);

            // Load GUI
            s_wptr<GUIManager> pGUIMgr = Engine::GetSingleton()->GetGUIManager();
            pGUIMgr->AddAddOnDirectory("Interface/BaseUI");
            pGUIMgr->AddAddOnDirectory("Interface/AddOns");

            pGUIMgr->ReadFiles();

            // Populate the world !
            ZoneManager::GetSingleton()->LoadZone("SmallHouse");

            // Create Units
            s_ptr<Character> pChar = UnitManager::GetSingleton()->CreateCharacter("Athrauka", "Orc", Character::GENDER_MALE);
            pChar->Teleport(Vector(-11.3391f, 27.5f, 0.5f));

            pChar->SetClass("MAGE");
            pChar->SetLevel(51);
            pChar->SetStat("SPIRIT", s_int(50));
            pChar->SetStat("INTELLECT", s_int(50));

            /*s_ptr<Character> pChar2 = UnitManager::GetSingleton()->CreateCharacter("Loulou", "Orc", Character::GENDER_MALE);
            pChar2->EnablePhysics();
            pChar2->Teleport(Vector(0, 1, -5));
            pChar2->LookAtUnit(pChar);

            pChar2->SetClass("MAGE");
            pChar2->SetLevel(51);
            pChar2->SetStat("SPIRIT", s_int(50));
            pChar2->SetStat("INTELLECT", s_int(50));*/

            LightManager::GetSingleton()->SetSunDirection(Vector(1, -1, 0));

            // Some light
            /*s_ptr<Light> pLight1 = LightManager::GetSingleton()->CreateLight(Light::POINT);
            pLight1->SetPosition(Vector(0, 5, 0));
            pLight1->SetColor(Color(0.0f, 1.0f, 0.0f));
            pLight1->SetAttenuation(0.0f, 0.125f, 0.0f);
            pLight1->SetRange(100.0f);*/
        }

        // Enter the main loop
        pFrost->Loop();
    }
    catch (Ogre::Exception&  e)
    {
        Log("Fatal exception : "+e.getFullDescription());
    }
    catch (std::exception&   e)
    {
        Log("Fatal exception : "+s_str(e.what()));
    }
    catch (Frost::Exception& e)
    {
        Log("Fatal exception : "+e.GetDescription());
    }
    catch (...)
    {
        Log("Fatal exception.");
    }

    // Close the engine
    Engine::Delete();
    // Close the editor if needed
    Editor::Delete();

    return 0;
}
