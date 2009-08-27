/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*                                        */

#include "frost_engine.h"

#include "frost_inputmanager.h"
#include "gui/frost_fontmanager.h"
#include "gui/frost_guimanager.h"
#include "frost_localemanager.h"
#include "gui/frost_spritemanager.h"
#include "gui/frost_guistructs.h"
#include "gui/frost_sprite.h"
#include "path/frost_pathmanager.h"
#include "model/frost_modelmanager.h"
#include "camera/frost_cameramanager.h"
#include "scene/frost_scenemanager.h"
#include "material/frost_shadermanager.h"
#include "material/frost_shader.h"
#include "material/frost_materialmanager.h"
#include "scene/frost_lightmanager.h"
#include "unit/frost_unitmanager.h"
#include "gameplay/frost_gameplaymanager.h"
#include "scene/frost_physicsmanager.h"
#include "scene/frost_zonemanager.h"
#include "camera/frost_camera.h"
#include "lua/frost_lua.h"

#include <OgreRoot.h>
#include <OgreRenderSystem.h>
#include <OgreRenderWindow.h>
#include <OgreWindowEventUtilities.h>
#include <OgreRenderTargetListener.h>
#include <OgrePass.h>

using namespace std;

namespace Frost
{
    class SceneTargetListener : public Ogre::RenderTargetListener
    {
    public :

        void preRenderTargetUpdate( const Ogre::RenderTargetEvent& mEvent )
        {
            SpriteManager::GetSingleton()->DisableAutomaticRendering();
        }

        void postRenderTargetUpdate( const Ogre::RenderTargetEvent& mEvent )
        {
            SpriteManager::GetSingleton()->EnableAutomaticRendering();
        }
    };

    class RenderWindowListener : public Ogre::RenderTargetListener
    {
    public :

        void preRenderTargetUpdate( const Ogre::RenderTargetEvent& mEvent )
        {
            Engine::GetSingleton()->GetOgreSceneManager()->getRootSceneNode()->setVisible(false);
        }

        void postRenderTargetUpdate( const Ogre::RenderTargetEvent& mEvent )
        {
            Engine::GetSingleton()->GetOgreSceneManager()->getRootSceneNode()->setVisible(true);
        }
    };

    const s_str Engine::CLASS_NAME = "Engine";

    Engine::Engine() : mLog_("Frost.log", FILE_O)
    {
        iGameState_ = 0;
        bGamePaused_ = false;
        bShutDown_ = false;

        lGameOptionList_["GameVersion"] = s_str("0.045");

        pFrameFunc_ = NULL;

        // Initialize OGRE
        pRoot_ = new Ogre::Root("", "");

        // Create singletons;
        pUtilsMgr_    = UtilsManager::GetSingleton();
        pLuaMgr_      = LuaManager::GetSingleton();
        pEventMgr_    = EventManager::GetSingleton();
        pTimeMgr_     = TimeManager::GetSingleton();
        pInputMgr_    = InputManager::GetSingleton();
        pLocaleMgr_   = LocaleManager::GetSingleton();
        pShaderMgr_   = ShaderManager::GetSingleton();
        pMaterialMgr_ = MaterialManager::GetSingleton();
        pFontMgr_     = FontManager::GetSingleton();
        pPhysicsMgr_  = PhysicsManager::GetSingleton();
        pSceneMgr_    = SceneManager::GetSingleton();
        pPathMgr_     = PathManager::GetSingleton();

        pLightMgr_    = LightManager::GetSingleton();
        pCameraMgr_   = CameraManager::GetSingleton();
        pModelMgr_    = ModelManager::GetSingleton();
        pSpriteMgr_   = SpriteManager::GetSingleton();
        pGUIMgr_      = GUIManager::GetSingleton();

        pUnitMgr_     = UnitManager::GetSingleton();
        pGameplayMgr_ = GameplayManager::GetSingleton();
        pZoneMgr_     = ZoneManager::GetSingleton();

        pLua_ = pLuaMgr_->CreateLua();
        pLuaMgr_->SetDefaultLua(pLua_);

        Lua::RegisterGlobalFuncs(pLua_);

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
                sNewMessage.EraseFromStart(2);
            }
            else
            {
                s_str sStamps = TimeManager::GetSingleton()->GetPlayTime() + " : ";
                sNewMessage = sStamps+sMessage;
                sNewMessage.Replace("\n", "\n"+sStamps);
            }

            Engine::GetSingleton()->GetLog()->WriteLine(sNewMessage.Get());
            Engine::GetSingleton()->GetLog()->Flush();
            #ifdef _DEBUG
                printf("%s\n", sNewMessage.c_str());
            #endif
        }
        else
        {
            Engine::GetSingleton()->GetLog()->WriteLine(sMessage.Get());
            Engine::GetSingleton()->GetLog()->Flush();
            #ifdef _DEBUG
                printf("%s\n", sMessage.c_str());
            #endif
        }
    }

    s_bool Engine::Initialize()
    {
        pUtilsMgr_->Initialize();

        // Register the log function
        pUtilsMgr_->SetLogFunction(&PrintInLog);

        //pEventMgr_->ToggleDebugOutput();

        // Load configuration
        if (!pLua_->DoFile("Config.lua"))
            return false;

        pLua_->PushGlobal("GameOptions");
        pLua_->PushNil();
        while (lua_next(pLua_->GetState(), -2) != 0)
        {
            lGameOptionList_[pLua_->GetString(-2)] = pLua_->GetValue(-1);
            pLua_->Pop();
        }

        s_str sLine = "# Starting Frost Engine (" + lGameOptionList_["GameVersion"].Get<s_str>() + ") #";
        Log(s_str('#', sLine.GetLength()));
        Log(sLine);
        Log(s_str('#', sLine.GetLength())+"\n");

        if (!this->ReadGraphicsConfig_())
            return false;

        if (!pFontMgr_->ReadConfig())
            return false;

        pLocaleMgr_->Initialize();
        if (!pLocaleMgr_->ReadConfig())
            return false;

        // Create the root path
        Ogre::ResourceGroupManager::getSingleton().addResourceLocation("./", "FileSystem", "Frost");
        Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();

        // Create Ogre's scene manager
        pOgreSceneMgr_ = pRoot_->createSceneManager(Ogre::ST_GENERIC, "FrostSceneMgr");

        // Load shaders
        if (!pShaderMgr_->LoadShaders())
            return false;

        pUnitMgr_->Initialize();

        if (!pUnitMgr_->ParseData())
            return false;

        if (!pGameplayMgr_->ParseData())
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
        pSpriteMgr_->Initialize();

        // Initialize the GUI manager
        pGUIMgr_->Initialize();

        // Initialize the zone manager
        pZoneMgr_->Initialize();

        if (!this->ReadGameConfig_())
            return false;

        return true;
    }

    void Engine::Loop()
    {
        if (!pCameraMgr_->CheckSettings())
            return;

        pRoot_->getRenderSystem()->_initRenderTargets();
        pTimeMgr_->Initialize();

        bRun_ = true;

        // Start the main loop
        while (bRun_)
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

            // Update current gameplay
            pGameplayMgr_->Update(fDelta);

            // Update cameras' state and animations
            pCameraMgr_->UpdateCameras(fDelta);

            // Update displayed zone chunks
            pZoneMgr_->Update(fDelta);

            // Update lights' animations
            pLightMgr_->UpdateLights(fDelta);

            // Update units' state, actions, movement, ...
            pUnitMgr_->UpdateUnits(fDelta);

            // Update physics controled objects
            pPhysicsMgr_->UpdateHandlers(fDelta);

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

        bRun_ = false;
    }

    void Engine::ShutDown()
    {
        if (!bShutDown_)
        {
            if (bRun_)
                bRun_ = false;
            else
            {
                Log("\nEngine shutdown.");
                Log("Average FPS : "+ pTimeMgr_->GetAverageFPS());
                Log("Best FPS : "+ pTimeMgr_->GetBestFPS());
                Log("Worst FPS : "+ pTimeMgr_->GetWorstFPS()+"\n");

                // End, delete/free everything
                // ...
                pSceneSprite_.Delete();
                pSpriteMgr_->DeleteRenderTarget(pSceneRenderTarget_);
                pSceneRenderTarget_ = NULL;

                // Print out profiling info
                pTimeMgr_->Print();

                // Delete managers
                ZoneManager::Delete();
                GameplayManager::Delete();
                UnitManager::Delete();
                GUIManager::Delete();
                SpriteManager::Delete();
                ModelManager::Delete();
                CameraManager::Delete();
                LightManager::Delete();

                PathManager::Delete();
                SceneManager::Delete();
                PhysicsManager::Delete();
                FontManager::Delete();
                MaterialManager::Delete();
                ShaderManager::Delete();
                LocaleManager::Delete();
                InputManager::Delete();
                EventManager::Delete();
                LuaManager::Delete();

                Log("Closing Ogre...");

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

                pRoot_.Delete();

                bShutDown_ = true;

                Log("Done.");

                TimeManager::Delete();
                UtilsManager::Delete();
            }
        }
    }

    s_bool Engine::ReadGameConfig_()
    {
        pGameplayMgr_->SetCurrentGameplay(lGameOptionList_["DefaultGameplay"].Get<s_str>());

        return true;
    }

    s_bool Engine::ReadGraphicsConfig_()
    {
        s_str sRenderSystem = lGameOptionList_["RenderSystem"].Get<s_str>();
        if (sRenderSystem == "OpenGL")
        {
            sRenderSystem = "OpenGL Rendering Subsystem";
            #ifdef _DEBUG
                pRoot_->loadPlugin("RenderSystem_GL_d");
            #else
                pRoot_->loadPlugin("RenderSystem_GL");
            #endif
        }
        else if (sRenderSystem == "DirectX")
        {
            sRenderSystem = "Direct3D9 Rendering Subsystem";
            #ifdef _DEBUG
                pRoot_->loadPlugin("RenderSystem_Direct3D9_d");
            #else
                pRoot_->loadPlugin("RenderSystem_Direct3D9");
            #endif
        }

        Ogre::RenderSystem* pRS = pRoot_->getRenderSystemByName(sRenderSystem.Get());

        if (!pRS)
        {
            Error(CLASS_NAME,
                "Unsupported render system : \""+sRenderSystem+"\"."
            );
            return false;
        }

        pRoot_->setRenderSystem(pRS);
        pRoot_->initialise(false);

        s_uint uiScreenWidth = s_uint(lGameOptionList_["ScreenWidth"].Get<s_float>());
        s_uint uiScreenHeight = s_uint(lGameOptionList_["ScreenHeight"].Get<s_float>());

        s_uint uiWindowWidth = s_uint(lGameOptionList_["WindowWidth"].Get<s_float>());
        s_uint uiWindowHeight = s_uint(lGameOptionList_["WindowHeight"].Get<s_float>());

        s_bool bFullScreen = lGameOptionList_["FullScreen"].Get<s_bool>();
        s_bool bVSync = lGameOptionList_["EnableVSync"].Get<s_bool>();

        s_uint uiFSAA = s_uint(lGameOptionList_["AntiAliasingLevel"].Get<s_float>());

        Ogre::NameValuePairList lOptions;
        // NOTE : 32bit is required for A8R8G8B8 format
        lOptions.insert(Ogre::NameValuePairList::value_type("colourDepth", "32"));
        lOptions.insert(Ogre::NameValuePairList::value_type("vsync", (bVSync ? "yes" : "no")));
        lOptions.insert(Ogre::NameValuePairList::value_type("FSAA", s_str(uiFSAA).Get()));

        pRenderWindow_ = pRoot_->createRenderWindow(
            #ifdef _DEBUG
                "Frost Engine - WoWRL (Debug)",
            #else
                "Frost Engine - WoWRL",
            #endif
            bFullScreen ? uiScreenWidth.Get()  : uiWindowWidth.Get(),
            bFullScreen ? uiScreenHeight.Get() : uiWindowHeight.Get(),
            bFullScreen,
            &lOptions
        );
        if (!pRenderWindow_)
        {
            Error(CLASS_NAME, "Couldn't create render window.");
            return false;
        }
        pRenderWindow_->addListener(
            new RenderWindowListener()
        );

        pSceneRenderTarget_ = pSpriteMgr_->CreateRenderTarget(
            "SceneTarget",
            bFullScreen ? uiScreenWidth  : uiWindowWidth,
            bFullScreen ? uiScreenHeight : uiWindowHeight,
            RenderTarget::PIXEL_ARGB, RenderTarget::USAGE_3D
        );
        pSceneRenderTarget_->AddListener(
            new SceneTargetListener()
        );

        s_refptr<Material> pMat = MaterialManager::GetSingleton()->CreateMaterial2DFromRT(Engine::GetSingleton()->GetSceneRenderTarget());
        pMat->GetDefaultPass()->setSeparateSceneBlending(
            Ogre::SBF_ONE, Ogre::SBF_ZERO,
            Ogre::SBF_ZERO, Ogre::SBF_ZERO
        );

        pSceneSprite_ = new Sprite(pMat);

        return true;
    }

    s_var Engine::GetConstant( const s_str& sConstantName ) const
    {
        return lGameOptionList_.Get(sConstantName)->second;
    }

    s_str Engine::GetStringConstant( const s_str& sConstantName ) const
    {
        return lGameOptionList_.Get(sConstantName)->second.Get<s_str>();
    }

    s_uint Engine::GetUIntConstant( const s_str& sConstantName ) const
    {
        return lGameOptionList_.Get(sConstantName)->second.Get<s_uint>();
    }

    s_bool Engine::GetBoolConstant( const s_str& sConstantName ) const
    {
        return lGameOptionList_.Get(sConstantName)->second.Get<s_bool>();
    }

    s_str Engine::GetRenderer() const
    {
        return lGameOptionList_.Get("RenderSystem")->second.Get<s_str>();
    }

    s_ptr<File> Engine::GetLog()
    {
        return &mLog_;
    }

    s_ptr<Ogre::SceneManager> Engine::GetOgreSceneManager()
    {
        return pOgreSceneMgr_;
    }

    s_ptr<Ogre::RenderWindow> Engine::GetRenderWindow()
    {
        return pRenderWindow_;
    }

    s_ptr<RenderTarget> Engine::GetSceneRenderTarget()
    {
        return pSceneRenderTarget_;
    }

    s_ptr<Lua::State> Engine::GetLua()
    {
        return pLua_;
    }

    s_uint Engine::GetScreenWidth() const
    {
        return s_uint(s_float(pRenderWindow_->getWidth()));
    }

    s_uint Engine::GetScreenHeight() const
    {
        return s_uint(s_float(pRenderWindow_->getHeight()));
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
        //pSceneRenderTarget_->GetOgreRenderTarget()->writeContentsToFile(sFileName.Get());
    }

    void Engine::RenderScene()
    {
        pSceneSprite_->Render(0, 0);
    }
}
