
#include "frost.h"
#include "frost_model.h"
#include "frost_modelmanager.h"
#include "frost_animmanager.h"
#include "frost_fontmanager.h"
#include "gui/frost_guimanager.h"
#include "frost_sprite.h"
#include "frost_spritemanager.h"
#include "frost_inputmanager.h"
#include "camera/frost_cameramanager.h"
#include "camera/frost_camera.h"
#include "camera/frost_camera_topcamera.h"
#include "frost_path_directpath.h"
#include "frost_path_smoothpath.h"
#include "frost_pathmanager.h"
#include "frost_materialmanager.h"
#include "frost_material.h"
#include "frost_scenemanager.h"
#include "frost_plane.h"
#include "frost_lightmanager.h"
#include "frost_light.h"
#include "frost_unitmanager.h"
#include "frost_character.h"
#include "frost_node.h"
#include "frost_text.h"

#include <OgreException.h>

using namespace std;
using namespace Frost;

s_ptr<Engine> pFrost;
s_ptr<Character> pChar;
s_ptr<Character> pChar2;
s_ptr<Model>  pModel;

s_ptr<Sprite> pSprite;
s_ptr<Sprite> pRTSprite;
s_ptr<RenderTarget> pRTarget;
s_ptr<Text>   pText;

s_ptr<Camera> pCam;

s_ptr<Plane>  pPlane;
s_ptr<Light>  pLight1;
s_ptr<Light>  pLight2;
s_ptr<Light>  pLight3;

s_bool bHideGUI = false;

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
        bHideGUI = !bHideGUI;
    }

    if (pInputMgr->KeyIsPressed(KEY_P))
    {
        GUIManager::GetSingleton()->PrintUI();
    }

    if (pInputMgr->KeyIsPressed(KEY_R))
    {
        //pChar->Resurrect();
    }

    if (pInputMgr->KeyIsPressed(KEY_E))
    {
        pModel->GetAnimMgr()->SetAnim(ANIM_SLEEP, ANIM_PRIORITY_HIGH);
    }

    if (pInputMgr->KeyIsPressed(KEY_F1))
    {
        if (mCamType == CAMERA_TOP)
            mCamType = CAMERA_FREE;
        else
            mCamType = CameraType(mCamType+1);

        switch (mCamType)
        {
            case CAMERA_FREE :
                pCam = pFrost->GetFreeCamera();
                break;
            case CAMERA_UNIT :
                pCam = pChar->GetCamera();
                break;
            case CAMERA_TOP :
                pCam = pFrost->GetTopCamera();
                break;
        }

        CameraManager::GetSingleton()->SetMainCamera(pCam);
    }

    return true;
}

s_bool RenderFunc()
{
    static s_ptr<SpriteManager> pSpriteMgr = SpriteManager::GetSingleton();

    pText->SetText("FPS : "+TimeManager::GetSingleton()->GetFPS());

    pSpriteMgr->Begin(pRTarget);

        pSpriteMgr->Clear(Color::VOID);

        //pSprite->Render(0, 0);

    pSpriteMgr->End();

    // Render in the main target
    pSpriteMgr->Begin();

        pSpriteMgr->Clear(Color::VOID);

        if (!bHideGUI)
            GUIManager::GetSingleton()->RenderUI();

        pText->Render(s_float(pFrost->GetScreenWidth())-2-pText->GetTextWidth(), s_float(pFrost->GetScreenHeight())-18);

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

            // Create a Unit
            pChar2 = UnitManager::GetSingleton()->CreateCharacter("Loulette", "Orc", GENDER_MALE);
            pChar = UnitManager::GetSingleton()->CreateCharacter("Athrauka", "Orc", GENDER_MALE);
            pChar->SetClass("MAGE");
            pChar->SetLevel(51);
            pChar->SetStat("SPIRIT", s_int(50));
            pChar->SetStat("INTELLECT", s_int(50));
            pModel = pChar->GetBodyModel().Get();

            switch (mCamType)
            {
                case CAMERA_FREE :
                    pCam = pFrost->GetFreeCamera();
                    break;
                case CAMERA_UNIT :
                    pCam = pChar->GetCamera();
                    break;
                case CAMERA_TOP :
                    pCam = pFrost->GetTopCamera();
                    break;
            }
            CameraManager::GetSingleton()->SetMainCamera(pCam);

            // The ground
            s_refptr<Material> pGroundMat = MaterialManager::GetSingleton()->CreateMaterial3D(
                "Textures/Tileset/Aerie Peaks/AeriePeaksTrollTile.png"
            );
            pGroundMat->SetTilling(0.01f, 0.01f);

            pPlane = SceneManager::GetSingleton()->CreatePlane();
            pPlane->SetMaterial(pGroundMat);

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
            std::cerr << e.getFullDescription().c_str() << std::endl;
        #else
            MessageBox(
                NULL, e.getFullDescription().c_str(),
                "Ogre Error", MB_OK|MB_ICONERROR|MB_TASKMODAL
            );
        #endif
    }

    return 0;
}
