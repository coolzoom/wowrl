
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

s_ptr<Sprite> pSprite;
s_ptr<Sprite> pRTSprite;
s_ptr<RenderTarget> pRTarget;
s_ptr<Text>   pText;

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

s_bool FrameFunc()
{
    static s_ptr<InputManager> pInputMgr = InputManager::GetSingleton();
    static s_ptr<TimeManager> pTimeMgr = TimeManager::GetSingleton();

    if (pInputMgr->KeyIsPressed(KEY_ESCAPE))
        return false;

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

s_bool RenderFunc()
{
    static s_ptr<SpriteManager> pSpriteMgr = SpriteManager::GetSingleton();

    //pText->SetText("FPS : "+TimeManager::GetSingleton()->GetFPS());

    pSpriteMgr->Begin(pRTarget);

        pSpriteMgr->Clear(Color::VOID);

        //pSprite->Render(0, 0);

    pSpriteMgr->End();

    // Render in the main target
    pSpriteMgr->Begin();

        pSpriteMgr->Clear(Color::VOID);

        GUIManager::GetSingleton()->RenderUI();

        //pText->Render(s_float(pFrost->GetScreenWidth())-2-pText->GetTextWidth(), s_float(pFrost->GetScreenHeight())-18);

        pRTSprite->Render(0, 0);

    pSpriteMgr->End();

    return true;
}

// To Do List :
#ifdef FROST_LINUX
int main(int argc, char *argv[])
#else
#include <windows.h>
INT WINAPI WinMain( HINSTANCE hInst, HINSTANCE, LPSTR strCmdLine, INT )
#endif
{
    try
    {
        // Create the engine
        pFrost = Engine::GetSingleton();

        // Initialize base parameters
        if (pFrost->Initialize())
        {
            // Set render and frame functions
            pFrost->SetFrameFunction(&FrameFunc);
            SpriteManager::GetSingleton()->SetRenderFunction(&RenderFunc);

            LightManager::GetSingleton()->SetAmbient(Color(150, 150, 150));

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

            s_refptr<Text> pTxt = s_refptr<Text>(new Text("Fonts/Calibri.ttf", 16));
            pTxt->SetAlignment(Text::ALIGN_RIGHT);
            pText = pTxt.Get();

            /*pLight2 = LightManager::GetSingleton()->CreateLight(LIGHT_POINT);
            pLight2->SetPosition(Vector(0, 5, 1));
            pLight2->SetColor(Color(0, 255, 0));
            pLight2->SetAttenuation(0.0f, 0.125f, 0.0f);
            pLight2->SetRange(100.0f);

            pLight3 = LightManager::GetSingleton()->CreateLight(LIGHT_POINT);
            pLight3->SetPosition(Vector(0, 0, 1));
            pLight3->SetColor(Color(0, 0, 255));
            pLight3->SetAttenuation(0.0f, 0.125f, 0.0f);
            pLight3->SetRange(100.0f);*/

            // UI
            /*s_refptr<Material> pMat = MaterialManager::GetSingleton()->CreateMaterial2D(
                "Textures/UI/UI-CloseButton-Up.png"
            );*/
            /*s_refptr<Material> pMat = MaterialManager::GetSingleton()->CreateMaterial2D(
                "UI-UnitFrame.png"
            );
            s_refptr<Sprite> pSpr = s_refptr<Sprite>(new Sprite(pMat));
            pSprite = pSpr.Get();
            pSprite->SetColor(Color(128, 255, 255, 255));*/

            pRTarget = SpriteManager::GetSingleton()->CreateRenderTarget("RttTex", 1024, 768);
            s_refptr<Material> pMat2 = MaterialManager::GetSingleton()->CreateMaterial2DFromRT(
                pRTarget
            );
            s_refptr<Sprite> pSpr2 = s_refptr<Sprite>(new Sprite(pMat2, 1024, 768));
            pRTSprite = pSpr2.Get();

            // Enter the main loop
            pFrost->Loop();
        }
        else
        {
            #ifdef FROST_LINUX
                std::cerr << "An error has occured while loading.\nSee Frost.log." << std::endl;
            #else
                MessageBox(
                    NULL, "An error has occured while loading.\nSee Frost.log.",
                    "Frost Error", MB_OK|MB_ICONERROR|MB_TASKMODAL
                );
            #endif
        }

        pText.SetNull();
        pSprite.SetNull();

        // Close the engine
        Engine::Delete();
    }
    catch (Ogre::Exception e)
    {
        #ifdef FROST_LINUX
            std::cerr << e.getFullDescription() << std::endl;
        #else
            MessageBox(
                NULL, e.getFullDescription().c_str(),
                "Ogre Error", MB_OK|MB_ICONERROR|MB_TASKMODAL
            );
        #endif
    }
    catch (std::exception e)
    {
        #ifdef FROST_LINUX
            std::cerr << e.what() << std::endl;
        #else
            MessageBox(
                NULL, e.what(),
                "Ogre Error", MB_OK|MB_ICONERROR|MB_TASKMODAL
            );
        #endif
    }

    return 0;
}
