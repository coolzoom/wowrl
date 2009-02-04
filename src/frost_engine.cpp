/* ###################################### */
/* ###   WoW Raid Leader, by Kalith   ### */
/* ###################################### */
/*                                        */

#include "frost_engine.h"

#include "frost_timemanager.h"
#include "frost_eventmanager.h"
#include "frost_inputmanager.h"
#include "frost_fontmanager.h"
#include "frost_guimanager.h"
#include "frost_localemanager.h"
#include "frost_spritemanager.h"
#include "frost_pathmanager.h"
#include "frost_modelmanager.h"
#include "frost_cameramanager.h"
#include "frost_scenemanager.h"
#include "frost_materialmanager.h"
#include "frost_lightmanager.h"
#include "frost_unitmanager.h"
#include "frost_camera.h"


#include <OgreRoot.h>
#include <OgreRenderSystem.h>
#include <OgreRenderWindow.h>
#include <OgreWindowEventUtilities.h>
#include <OgreMaterialManager.h>
#include <OgreUnifiedHighLevelGpuProgram.h>

using namespace std;

namespace Frost
{
    s_str Engine::sLuaComString = "";
    const s_str Engine::CLASS_NAME = "Engine";

    Engine::Engine()
    {
        uiScreenWidth_ = 1024;
        uiScreenHeight_ = 768;
        iGameState_ = 0;
        bGamePaused_ = false;
        sGameVersion_ = "0.045";
        iMaxComputedPaths_ = 2;
        pLua_ = NULL;
        pFrameFunc_ = NULL;

        bShutDown_ = false;

        srand((unsigned)time(0));

        // Initialize Lua
        if (!Lua::InitLua(&pLua_))
            return;

        Lua::RegisterGlobalFuncs(pLua_);

        // Initialize OGRE
        pRoot_ = s_refptr<Ogre::Root>(new Ogre::Root("", ""));

        // Create singletons;
        pUtilsMgr_    = UtilsManager::GetSingleton();
        pEventMgr_    = EventManager::GetSingleton();
        pTimeMgr_     = TimeManager::GetSingleton();
        pInputMgr_    = InputManager::GetSingleton();
        pLocaleMgr_   = LocaleManager::GetSingleton();
        pMaterialMgr_ = MaterialManager::GetSingleton();
        pFontMgr_     = FontManager::GetSingleton();
        pSceneMgr_    = SceneManager::GetSingleton();
        pPathMgr_     = PathManager::GetSingleton();

        pLightMgr_    = LightManager::GetSingleton();
        pCameraMgr_   = CameraManager::GetSingleton();
        pModelMgr_    = ModelManager::GetSingleton();
        pSpriteMgr_   = SpriteManager::GetSingleton();
        pGUIMgr_      = GUIManager::GetSingleton();
        pUnitMgr_     = UnitManager::GetSingleton();

        //mTimeMgr_->SetProfiling(true);
    }

    Engine::~Engine()
    {
        ShutDown();
    }

    void PrintInLog( const s_str& sMessage, const s_bool& bTimeStamps )
    {
        if (bTimeStamps)
        {
            s_str sNewMessage;
            if (sMessage[0] == '|' && sMessage[1] == 't')
            {
                sNewMessage = sMessage;
                sNewMessage.Erase(0, 2);
            }
            else
            {
                s_str sStamps = TimeManager::GetSingleton()->GetPlayTime() + " : ";
                sNewMessage = sStamps+sMessage;
                sNewMessage.Replace("\n", s_str("\n") + sStamps);
            }

            Engine::GetSingleton()->GetLog()->logMessage(sNewMessage.Get());
        }
        else
        {
            Engine::GetSingleton()->GetLog()->logMessage(sMessage.Get());
        }
    }

    s_bool Engine::Initialize()
    {
        // Create the log
        pLog_ = Ogre::LogManager::getSingleton().createLog("Frost.log");
        pLog_->setTimeStampEnabled(false);
        pUtilsMgr_->SetLogFunction(&PrintInLog);

        s_str sLine = "# Starting Frost Engine (" + sGameVersion_ + ") #";
        Log(s_str('#', sLine.Length()));
        Log(sLine);
        Log(s_str('#', sLine.Length())+"\n");

        // Load configuration
        if (!Lua::DoFile(pLua_, "Scripts/Config.lua"))
            return false;
        if (!this->ReadConfig_())
            return false;
        if (!pLocaleMgr_->ReadConfig())
            return false;
        if (!pFontMgr_->ReadConfig())
            return false;

        // Create the root path
        Ogre::ResourceGroupManager::getSingleton().addResourceLocation("./", "FileSystem", "Frost");
        Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();

        // Create Ogre's scene manager
        pOgreSceneMgr_ = pRoot_->createSceneManager(Ogre::ST_GENERIC, "FrostSceneMgr");

        // Load shaders
        if (!LoadShaders_())
            return false;

        if (!pUnitMgr_->ParseCharacterModels())
            return false;

        if (!pUnitMgr_->ParseRaces())
            return false;

        // Initialize the input manager
        pInputMgr_->Initialize(pRenderWindow_);

        // Initialize the material manager
        pMaterialMgr_->Initialize();

        // Initialize the scene manager
        pSceneMgr_->Initialize();

        // Initialize the light manager
        pLightMgr_->Initialize();

        // Initialize the sprite manager
        pSpriteMgr_->Initialize(pOgreSceneMgr_, s_float(uiScreenWidth_), s_float(uiScreenHeight_));

        // Initialize the GUI manager
        pGUIMgr_->Initialize();

        // Load the UI
        pGUIMgr_->LoadUI();

        // Create the camera
        pCamera_ = pCameraMgr_->CreateCamera(Vector(3, 4, 3));
        pCamera_->SetDirection(Vector(-1, -1, -1));
        pCameraMgr_->SetMainCamera(pCamera_);

        return true;
    }

    void Engine::Loop()
    {
        if (!pCameraMgr_->CheckSettings())
            return;

        pRoot_->getRenderSystem()->_initRenderTargets();
        pTimeMgr_->Initialize();

        // Start the main loop
        while (true)
        {
            Ogre::WindowEventUtilities::messagePump();

            // Check if the window has been closed
            if (pRenderWindow_->isClosed())
                break;

            // Notify ogre a new frame has started
            if (!pRoot_->_fireFrameStarted())
                break;

            // Call the frame function
            if (pFrameFunc_)
            {
                if (!(*pFrameFunc_)())
                    break;
            }

            // Check there is a camera ready for rendering
            if (!pCameraMgr_->CheckSettings())
                break;

            s_float fDelta = s_float(pTimeMgr_->GetDelta());

            // Update cameras' state and animations
            pCameraMgr_->UpdateCameras(fDelta);

            // Update lights' animations
            pLightMgr_->UpdateLights(fDelta);

            // Update units' state, actions, movement, ...
            pUnitMgr_->UpdateUnits(fDelta);

            // Update the GUI
            pGUIMgr_->Update(fDelta);

            // Update our own render targets
            if (!pSpriteMgr_->RenderTargets())
                break;

            // Render everyting
            pRoot_->_updateAllRenderTargets();

            // Update inputs and timers
            pTimeMgr_->Update();
            pInputMgr_->Update();

            if (!pRoot_->_fireFrameEnded())
                break;

            pEventMgr_->FrameEnded();
        }
    }

    void Engine::ShutDown()
    {
        if (!bShutDown_)
        {
            Log("\nGame ended.");
            Log("Average FPS : "+ pTimeMgr_->GetAverageFPS());
            Log("Best FPS : "+ pTimeMgr_->GetBestFPS());
            Log("Worst FPS : "+ pTimeMgr_->GetWorstFPS());

            // End, delete/free everything
            // ...

            // Print out profiling info
            pTimeMgr_->Print();

            // Delete managers
            UnitManager::Delete();
            GUIManager::Delete();
            SpriteManager::Delete();
            ModelManager::Delete();
            CameraManager::Delete();
            LightManager::Delete();

            PathManager::Delete();
            SceneManager::Delete();
            FontManager::Delete();
            MaterialManager::Delete();
            LocaleManager::Delete();
            InputManager::Delete();
            TimeManager::Delete();
            EventManager::Delete();
            UtilsManager::Delete();

            // Close Lua
            lua_close(pLua_);

            // Close OGRE
            if (pRoot_)
            {
                if (pOgreSceneMgr_)
                {
                    pRoot_->destroySceneManager(pOgreSceneMgr_.Get());
                    pOgreSceneMgr_ = NULL;
                }
            }
            if (pRenderWindow_)
            {
                pRenderWindow_->destroy();
                pRenderWindow_ = NULL;
            }
            if (pRoot_)
            {
                pRoot_.SetNull();
            }

            bShutDown_ = true;
        }
    }

    s_bool Engine::LoadShaders_()
    {
        Ogre::HighLevelGpuProgram* pProgram;
        Ogre::UnifiedHighLevelGpuProgram* pUProgram;

        // --------
        // Skinning : vertex shaders
        // --------

        // HLSL vertex shader
        pProgram = Ogre::HighLevelGpuProgramManager::getSingleton().createProgram(
            "Skinning_HLSL_VS", "Frost", "hlsl", Ogre::GPT_VERTEX_PROGRAM
        ).get();
        pProgram->setSourceFile("Shaders/Skinning_vs.hlsl");
        pProgram->setSkeletalAnimationIncluded(true);
        pProgram->setParameter("entry_point", "main_vs");
        pProgram->setParameter("target", "vs_2_0");
        pProgram->setParameter("column_major_matrices", "false");
        /*if (pProgram->isSupported())
        {
            Ogre::GpuProgramParametersSharedPtr pParams = pProgram->createParameters();
            pParams->setNamedAutoConstant(
                "mViewProj",
                Ogre::GpuProgramParameters::ACT_VIEWPROJ_MATRIX
            );
            pParams->setNamedAutoConstant(
                "mBoneMat",
                Ogre::GpuProgramParameters::ACT_WORLD_MATRIX_ARRAY_3x4
            );
        }*/

        // GLSL vertex shader
        pProgram = Ogre::HighLevelGpuProgramManager::getSingleton().createProgram(
            "Skinning_GLSL_VS", "Frost", "glsl", Ogre::GPT_VERTEX_PROGRAM
        ).get();
        pProgram->setSourceFile("Shaders/Skinning_vs.glsl");
        pProgram->setSkeletalAnimationIncluded(true);

        // Unified vertex shader
        pUProgram = static_cast<Ogre::UnifiedHighLevelGpuProgram*>(
            Ogre::HighLevelGpuProgramManager::getSingleton().createProgram(
            "Skinning_VS", "Frost", "unified", Ogre::GPT_VERTEX_PROGRAM
        ).get());
        pUProgram->addDelegateProgram("Skinning_HLSL_VS");
        pUProgram->addDelegateProgram("Skinning_GLSL_VS");
        pUProgram->load();

        // --------
        // Skinning : pixel shaders
        // --------

        // HLSL pixel shader
        pProgram = Ogre::HighLevelGpuProgramManager::getSingleton().createProgram(
            "Skinning_HLSL_PS", "Frost", "hlsl", Ogre::GPT_FRAGMENT_PROGRAM
        ).get();
        pProgram->setSourceFile("Shaders/Skinning_ps.hlsl");
        pProgram->setParameter("entry_point", "main_ps");
        pProgram->setParameter("target", "ps_2_0");

        // GLSL pixel shader
        pProgram = Ogre::HighLevelGpuProgramManager::getSingleton().createProgram(
            "Skinning_GLSL_PS", "Frost", "glsl", Ogre::GPT_FRAGMENT_PROGRAM
        ).get();
        pProgram->setSourceFile("Shaders/Skinning_ps.glsl");
        if (pProgram->isSupported())
        {
            Ogre::GpuProgramParametersSharedPtr pParams = pProgram->createParameters();
            int i = 0;
            pParams->setNamedConstant(
                "mTexture", &i, 1, 1
            );
        }

        // Unified pixel shader
        pUProgram = static_cast<Ogre::UnifiedHighLevelGpuProgram*>(
            Ogre::HighLevelGpuProgramManager::getSingleton().createProgram(
            "Skinning_PS", "Frost", "unified", Ogre::GPT_FRAGMENT_PROGRAM
        ).get());
        pUProgram->addDelegateProgram("Skinning_HLSL_PS");
        pUProgram->addDelegateProgram("Skinning_GLSL_PS");
        pUProgram->load();

        /*Ogre::MaterialSerializer* pSerializer = new Ogre::MaterialSerializer();
        ifstream* pFile = new ifstream();
        pFile->open("shader.txt", ios::in | ios::binary);
        Ogre::FileStreamDataStream* pStream = new Ogre::FileStreamDataStream(pFile);
        Ogre::DataStreamPtr mPtr = Ogre::DataStreamPtr(pStream);
        delete pSerializer;*/

        return true;
    }

    s_bool Engine::ReadConfig_()
    {
        iMaxComputedPaths_ = Lua::GetGlobalInt("iMaxComputedPaths", false, 2);
        sGameVersion_ = Lua::GetGlobalString("sGameVersion", false, "0");

        s_str sRenderSystem = Lua::GetGlobalString("sRenderSystem");
        if (sRenderSystem[0] == 'D')
        {
            #ifdef _DEBUG
                pRoot_->loadPlugin("RenderSystem_Direct3D9_d");
            #else
                pRoot_->loadPlugin("RenderSystem_Direct3D9");
            #endif
        }
        else
        {
            #ifdef _DEBUG
                pRoot_->loadPlugin("RenderSystem_GL_d");
            #else
                pRoot_->loadPlugin("RenderSystem_GL");
            #endif
        }

        Ogre::RenderSystem* pRS = pRoot_->getRenderSystemByName(sRenderSystem.Get());
        if (!pRS)
        {
            Error(CLASS_NAME, "No render system with the name \""+ sRenderSystem +"\".");
            return false;
        }
        else
            pRoot_->setRenderSystem(pRS);

        // Initialize the Root
        pRoot_->initialise(false);

        uiScreenWidth_ = s_uint(Lua::GetGlobalInt("iScreenWidth", false, 1024));
        uiScreenHeight_ = s_uint(Lua::GetGlobalInt("iScreenHeight", false, 768));
        s_uint uiScreenDepth = 32; // 32 is required for A8R8G8B8 format
        s_bool bFullScreen = Lua::GetGlobalBool("bFullScreen", false, false);
        s_bool bVSync = Lua::GetGlobalBool("bVSync", false, false);
        s_uint uiFSAA = s_uint(Lua::GetGlobalInt("iFSAA", false, 0));

        Ogre::NameValuePairList lOptions;
        lOptions.insert(Ogre::NameValuePairList::value_type("colourDepth", s_str(uiScreenDepth).Get()));
        s_str sVSync(CONV_YES_NO); sVSync << bVSync;
        lOptions.insert(Ogre::NameValuePairList::value_type("vsync", sVSync.Get()));
        lOptions.insert(Ogre::NameValuePairList::value_type("FSAA", s_str(uiFSAA).Get()));

        pRenderWindow_ = pRoot_->createRenderWindow(
            #ifdef _DEBUG
                "Frost Engine - WoWRL (Debug)",
            #else
                "Frost Engine - WoWRL",
            #endif
            uiScreenWidth_.Get(),
            uiScreenHeight_.Get(),
            bFullScreen,
            &lOptions
        );
        if (!pRenderWindow_)
        {
            Error(CLASS_NAME, "Couldn't create render window.");
            return false;
        }

        return true;
    }

    s_var Engine::GetConstant( s_str sName ) const
    {
        s_var vValue;
        if (sName == "MaxComputedPaths")
        {
            vValue.SetI(iMaxComputedPaths_);
        }
        else if (sName == "GameVersion")
        {
            vValue.SetS(sGameVersion_);
        }

        return vValue;
    }

    s_ptr<Ogre::Log> Engine::GetLog()
    {
        return pLog_;
    }

    s_ptr<Camera> Engine::GetCamera()
    {
        return pCamera_;
    }

    s_ptr<Ogre::SceneManager> Engine::GetOgreSceneManager()
    {
        return pOgreSceneMgr_;
    }

    s_ptr<Ogre::RenderWindow> Engine::GetRenderWindow()
    {
        return pRenderWindow_;
    }

    lua_State* Engine::GetLua()
    {
        return pLua_;
    }

    const s_uint& Engine::GetScreenWidth() const
    {
        return uiScreenWidth_;
    }

    const s_uint& Engine::GetScreenHeight() const
    {
        return uiScreenHeight_;
    }

    void Engine::SetFrameFunction( Function pFrameFunc )
    {
        pFrameFunc_ = pFrameFunc;
    }

    void Engine::TakeScreenshot()
    {
        s_str sFileName = "Screenshots/scr";
        sFileName += "_"+pTimeMgr_->GetYear();
        sFileName += "_"+s_str(pTimeMgr_->GetMonth(), 2);
        sFileName += "_"+s_str(pTimeMgr_->GetDay(), 2);
        sFileName += "_"+s_str(pTimeMgr_->GetHour(), 2);
        sFileName += "_"+s_str(pTimeMgr_->GetMinutes(), 2);
        sFileName += "_"+s_str(pTimeMgr_->GetSeconds(), 2);
        sFileName += ".jpg";
        pRenderWindow_->writeContentsToFile(sFileName.Get());
    }


}
