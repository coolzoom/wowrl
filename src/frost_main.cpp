
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

#include <OgreException.h>

#ifndef FROST_LINUX
    #include <windows.h>
#endif

using namespace std;
using namespace Frost;

s_ptr<Engine> pFrost;
s_ptr<Character> pChar;
s_ptr<Character> pChar2;
s_ptr<Model>  pModel;

s_refptr<Sprite> pSprite;
s_refptr<Sprite> pSprite2;
RenderTarget     mRTarget;

s_refptr<SmoothPath> pPath;
s_ptr<Camera>        pCam;

s_ptr<Plane>         pPlane;
s_ptr<Light>         pLight1;
s_ptr<Light>         pLight2;
s_ptr<Light>         pLight3;

s_float fCoefX = M_PI_4;
s_float fCoefY = M_PI_4;
s_float fZoom = 1.0f;
s_float fCamSpeed = 20.0f;

s_bool bCameraMovedAlone;
s_bool bAdjustCamDir;

enum CameraType
{
    CAMERA_FREE,
    CAMERA_UNIT,
    CAMERA_TOP
};

CameraType mCamType = CAMERA_FREE;

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

    if (pInputMgr->KeyIsPressed(KEY_P))
    {
        GUIManager::GetSingleton()->PrintUI();
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

    // Render in the main target
    pSpriteMgr->Begin();

        pSpriteMgr->Clear(Color(0, 0, 0, 0));

        // And the sprite another time
        pSprite->Render(0, 0);

    pSpriteMgr->End();

    return true;
}

// To Do List :

#ifdef FROST_LINUX
int main(int argc, char *argv[])
#else
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
            //pChar2 = UnitManager::GetSingleton()->CreateCharacter("Loulette", "Orc", GENDER_MALE);
            pChar = UnitManager::GetSingleton()->CreateCharacter("Athrauka", "Orc", GENDER_MALE);
            pModel = pChar->GetBodyModel().Get();
            pCam = pFrost->GetFreeCamera();

            // The ground
            s_refptr<Material> pGroundMat = MaterialManager::GetSingleton()->CreateMaterial3D(
                "Textures/Tileset/Aerie Peaks/AeriePeaksTrollTile.png"
            );
            pGroundMat->SetTilling(0.01f, 0.01f);

            pPlane = SceneManager::GetSingleton()->CreatePlane();
            pPlane->SetMaterial(pGroundMat);

            // Some light
            /*pLight1 = LightManager::GetSingleton()->CreateLight(LIGHT_POINT);
            pLight1->SetPosition(Vector(5, 0, 1));
            pLight1->SetColor(Color(255, 0, 0));
            pLight1->SetAttenuation(0.0f, 0.125f, 0.0f);
            pLight1->SetRange(100.0f);

            pLight2 = LightManager::GetSingleton()->CreateLight(LIGHT_POINT);
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
            s_refptr<Material> pMat = MaterialManager::GetSingleton()->CreateMaterial2D(
                "UI-UnitFrame.png"
            );
            pSprite = s_refptr<Sprite>(new Sprite(pMat, 256, 128));

            /*mRTarget = GFX::CreateRenderTarget("RttTex", 256, 128);
            Material* pMat2 = MaterialManager::GetSingleton()->CreateMaterial2DFromRT(
                "RttTex"
            );
            pSprite2 = new Sprite(pMat2, 256, 128);*/

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
