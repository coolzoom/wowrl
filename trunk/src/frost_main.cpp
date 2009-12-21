#include "frost.h"
#include "model/frost_model.h"
#include "model/frost_modelmanager.h"
#include "model/frost_animmanager.h"
#include "gui/frost_fontmanager.h"
#include "gui/frost_guimanager.h"
#include "gui/frost_sprite.h"
#include "gui/frost_text.h"
#include "gui/frost_spritemanager.h"
#include "gui/frost_uiobject.h"
#include "frost_inputmanager.h"
#include "camera/frost_cameramanager.h"
#include "camera/frost_camera.h"
#include "path/frost_directpath.h"
#include "path/frost_smoothpath.h"
#include "material/frost_materialmanager.h"
#include "material/frost_material.h"
#include "scene/frost_scenemanager.h"
#include "scene/frost_plane.h"
#include "scene/frost_node.h"
#include "scene/frost_lightmanager.h"
#include "scene/frost_light.h"
#include "unit/frost_unitmanager.h"
#include "unit/frost_character.h"
#include "gameplay/frost_gameplaymanager.h"
#include "scene/frost_physicsmanager.h"
#include "scene/frost_zonemanager.h"
#include "scene/frost_meshobstacle.h"
#include "scene/frost_planeobstacle.h"
#include "scene/frost_physicshandler.h"
#include "lua/frost_lua.h"

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
        s_ptr<GUI::UIObject> pObj = GUIManager::GetSingleton()->GetUIObjectByName("UIParent");
        if (pObj->IsVisible())
            pObj->Hide();
        else
            pObj->Show();
    }

    if (pInputMgr->KeyIsPressed(KEY_P))
    {
        GUIManager::GetSingleton()->PrintUI();
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
        s_ptr<GUI::UIObject> pObj = GUIManager::GetSingleton()->GetUIObjectByName("UIParent");
        if (pObj->IsVisible())
            pObj->Hide();
        else
            pObj->Show();
    }

    if (pInputMgr->KeyIsPressed(KEY_P))
    {
        GUIManager::GetSingleton()->PrintUI();
    }

    return true;
}

s_bool GameRenderFunc()
{
    static s_ptr<SpriteManager> pSpriteMgr = SpriteManager::GetSingleton();

    // Render in the main target
    pSpriteMgr->Begin();

        Engine::GetSingleton()->RenderScene();

        GUIManager::GetSingleton()->RenderUI();

    pSpriteMgr->End();

    return true;
}

s_bool EditorRenderFunc()
{
    static s_ptr<SpriteManager> pSpriteMgr = SpriteManager::GetSingleton();

    // Render in the main target
    pSpriteMgr->Begin();

        Engine::GetSingleton()->RenderScene();

        GUIManager::GetSingleton()->RenderUI();

    pSpriteMgr->End();

    return true;
}

// To Do List :

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
            SpriteManager::GetSingleton()->SetRenderFunction(&EditorRenderFunc);

            GameplayManager::GetSingleton()->SetCurrentGameplay("Editor");

            GUIManager::GetSingleton()->AddAddOnDirectory("Interface/BaseUI");
            GUIManager::GetSingleton()->AddAddOnDirectory("Interface/Editor");
            GUIManager::GetSingleton()->LoadUI();

            //s_ptr<Zone> pZone = ZoneManager::GetSingleton()->LoadZone("Test");

            LightManager::GetSingleton()->SetSunDirection(Vector(1, -1, 0));
        }
        else
        {
            pFrost->SetState(Engine::STATE_GAME);

            // Set render and frame functions
            pFrost->SetFrameFunction(&GameFrameFunc);
            SpriteManager::GetSingleton()->SetRenderFunction(&GameRenderFunc);

            // Load GUI
            GUIManager::GetSingleton()->AddAddOnDirectory("Interface/BaseUI");
            GUIManager::GetSingleton()->AddAddOnDirectory("Interface/AddOns");
            GUIManager::GetSingleton()->LoadUI();

            // Populate the world !
            ZoneManager::GetSingleton()->LoadZone("Test");

            // Create Units
            s_ptr<Character> pChar = UnitManager::GetSingleton()->CreateCharacter("Athrauka", "Orc", Character::GENDER_MALE);
            pChar->Teleport(Vector(0, 1, 0));

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


            // Some light
            /*s_ptr<Light> pLight1 = LightManager::GetSingleton()->CreateLight(LIGHT_POINT);
            pLight1->SetPosition(Vector(0, 5, 0));
            pLight1->SetColor(Color(255, 255, 255));
            pLight1->SetAttenuation(0.0f, 0.125f, 0.0f);
            pLight1->SetRange(50.0f);*/

            LightManager::GetSingleton()->SetSunDirection(Vector(1, -1, 0));
        }

        // Enter the main loop
        pFrost->Loop();
    }
    catch (Ogre::Exception  e)
    {
        Log("Fatal exception : "+e.getFullDescription());
    }
    catch (std::exception   e)
    {
        Log("Fatal exception : "+s_str(e.what()));
    }
    catch (Frost::Exception e)
    {
        Log("Fatal exception : "+e.GetDescription());
    }

    // Close the engine
    Engine::Delete();

    return 0;
}
