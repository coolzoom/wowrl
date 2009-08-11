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
#include "path/frost_path_directpath.h"
#include "path/frost_path_smoothpath.h"
#include "path/frost_pathmanager.h"
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

#include <OgreException.h>

using namespace std;
using namespace Frost;

s_ptr<Engine> pFrost;
s_ptr<Character> pChar;
s_ptr<Character> pChar2;

s_ptr<Camera> pCam;

s_ptr<Plane>  pPlane;

s_ptr<Light>  pLight1;
s_ptr<Light>  pLight2;
s_ptr<Light>  pLight3;

enum CameraType
{
    CAMERA_FREE,
    CAMERA_UNIT,
    CAMERA_TOP
};

CameraType mCamType = CAMERA_TOP;

s_bool GameFrameFunc()
{
    static s_ptr<InputManager> pInputMgr = InputManager::GetSingleton();
    static s_ptr<TimeManager> pTimeMgr = TimeManager::GetSingleton();

    if (pInputMgr->KeyIsPressed(KEY_ESCAPE))
    {
        Log("Escape pressed, exiting...");
        return false;
    }

    if (pInputMgr->KeyIsPressed(KEY_T))
    {
        pFrost->TakeScreenshot();
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
    static s_ptr<TimeManager>  pTimeMgr = TimeManager::GetSingleton();

    if (pInputMgr->KeyIsPressed(KEY_ESCAPE))
    {
        Log("Escape pressed, exiting...");
        return false;
    }

    if (pInputMgr->KeyIsPressed(KEY_T))
    {
        pFrost->TakeScreenshot();
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

s_bool GameRenderFunc()
{
    static s_ptr<SpriteManager> pSpriteMgr = SpriteManager::GetSingleton();

    // Render in the main target
    pSpriteMgr->Begin();

        pSpriteMgr->Clear(Color::VOID);

        GUIManager::GetSingleton()->RenderUI();

    pSpriteMgr->End();

    return true;
}

s_bool EditorRenderFunc()
{
    static s_ptr<SpriteManager> pSpriteMgr = SpriteManager::GetSingleton();

    // Render in the main target
    pSpriteMgr->Begin();

        pSpriteMgr->Clear(Color::VOID);

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
        pFrost = Engine::GetSingleton();

        // Initialize base parameters
        if (pFrost->Initialize())
        {
            if (bEditor)
            {
                Log("Entering Editor mode...");

                pFrost->SetFrameFunction(&EditorFrameFunc);
                SpriteManager::GetSingleton()->SetRenderFunction(&EditorRenderFunc);

                LightManager::GetSingleton()->SetAmbient(Color(150, 150, 150));

                GameplayManager::GetSingleton()->SetCurrentGameplay("Editor");

                GUIManager::GetSingleton()->AddAddOnFolder("Interface/BaseUI");
                GUIManager::GetSingleton()->AddAddOnFolder("Interface/Editor");
                GUIManager::GetSingleton()->LoadUI();
            }
            else
            {

                // Set render and frame functions
                pFrost->SetFrameFunction(&GameFrameFunc);
                SpriteManager::GetSingleton()->SetRenderFunction(&GameRenderFunc);

                LightManager::GetSingleton()->SetAmbient(Color(150, 150, 150));

                // Load GUI
                GUIManager::GetSingleton()->AddAddOnFolder("Interface/BaseUI");
                GUIManager::GetSingleton()->AddAddOnFolder("Interface/AddOns");
                GUIManager::GetSingleton()->LoadUI();

                // Populate the world !

                // Create the ground
                s_refptr<Material> pGroundMat = MaterialManager::GetSingleton()->CreateMaterial3D(
                    "Textures/Tileset/Aerie Peaks/AeriePeaksTrollTile.png"
                );
                pGroundMat->SetTilling(0.01f, 0.01f);

                pPlane = SceneManager::GetSingleton()->CreatePlane();
                pPlane->SetMaterial(pGroundMat);

                // Register it to the physics manager
                PhysicsManager::GetSingleton()->AddObstacle(pPlane);


                // Create Units
                pChar = UnitManager::GetSingleton()->CreateCharacter("Athrauka", "Orc", GENDER_MALE);
                /*pChar->EnablePhysics();
                pChar->ForceOnGround();*/

                pChar->SetClass("MAGE");
                pChar->SetLevel(51);
                pChar->SetStat("SPIRIT", s_int(50));
                pChar->SetStat("INTELLECT", s_int(50));


                pChar2 = UnitManager::GetSingleton()->CreateCharacter("Loulou", "Orc", GENDER_MALE);
                //pChar2->EnablePhysics();
                pChar2->Teleport(Vector(0, 0, -5));
                //pChar2->ForceOnGround();
                pChar2->LookAtUnit(pChar);

                pChar2->SetClass("MAGE");
                pChar2->SetLevel(51);
                pChar2->SetStat("SPIRIT", s_int(50));
                pChar2->SetStat("INTELLECT", s_int(50));


                // Some light
                pLight1 = LightManager::GetSingleton()->CreateLight(LIGHT_POINT);
                pLight1->SetPosition(Vector(0, 5, 0));
                pLight1->SetColor(Color(255, 255, 255));
                pLight1->SetAttenuation(0.0f, 0.125f, 0.0f);
                pLight1->SetRange(50.0f);
            }


            // Enter the main loop
            pFrost->Loop();
        }
        else
        {
            std::cerr << "An error has occured while loading.\nSee Frost.log." << std::endl;
        }

        // Close the engine
        Engine::Delete();
    }
    catch (Ogre::Exception e)
    {
        std::cerr << e.getFullDescription() << std::endl;
    }
    catch (std::exception e)
    {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}
