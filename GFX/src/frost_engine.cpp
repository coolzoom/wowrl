/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*                                        */

#include "frost_engine.h"

#include "frost_engine_glues.h"
#include <frost_inputmanager.h>
#include <frost_handler_ois.h>
#include <frost_fontmanager.h>
#include <frost_guimanager.h>
#include <impl/ogre/ogre_guimanager.h>
#include <frost_localemanager.h>
#include "model/frost_modelmanager.h"
#include "camera/frost_cameramanager.h"
#include "scene/frost_scenemanager.h"
#include "material/frost_shadermanager.h"
#include "material/frost_shader.h"
#include "material/frost_materialmanager.h"
#include "material/frost_rendertarget.h"
#include "scene/frost_lightmanager.h"
#include "gameplay/frost_gameplaymanager.h"
#include "scene/frost_physicsmanager.h"
#include "camera/frost_camera.h"
#include "gui/frost_modelframe.h"
#include <frost_sprite.h>
#include "lua/frost_lua.h"

#include <OgreRoot.h>
#include <OgreRenderSystem.h>
#include <OgreRenderWindow.h>
#include <OgreWindowEventUtilities.h>
#include <OgrePass.h>
#include <OgreGpuCommandBufferFlush.h>

#include <frost_utils_lua.h>
#include <frost_utils_file.h>
#include <frost_utilsmanager.h>
#include <frost_utils_timemanager.h>
#include <frost_utils_eventmanager.h>
#include <frost_utils_event.h>

#undef VOID

using namespace std;

namespace Frost
{
    const s_str Engine::CLASS_NAME = "Engine";

    s_double GetTime()
    {
        static Ogre::Timer t;
        return t.getMilliseconds()/1000.0;
    }

    void PrintInLog( const s_str& sMessage, const s_bool& bTimeStamps, const s_uint& uiOffset )
    {
        s_str sNewMessage;
        if (bTimeStamps)
        {
            if (sMessage[0] == '|' && sMessage[1] == 't')
            {
                sNewMessage = sMessage;
                sNewMessage.Replace("\n", "\n"+s_str(' ', uiOffset));
                sNewMessage.EraseFromStart(2);
            }
            else
            {
                s_str sStamps = TimeManager::GetSingleton()->GetPlayTime() + " : ";
                sNewMessage = sStamps+sMessage;
                sNewMessage.Replace("\n", "\n"+sStamps+s_str(' ', uiOffset));
            }

            Engine::GetSingleton()->GetLog()->WriteLine(sNewMessage);
            Engine::GetSingleton()->GetLog()->Flush();
            #ifdef _DEBUG
                cout << sNewMessage.Get() << endl;
            #endif
        }
        else
        {
            sNewMessage = sMessage;
            sNewMessage.Replace("\n", "\n"+s_str(' ', uiOffset));
            Engine::GetSingleton()->GetLog()->WriteLine(sNewMessage);
            Engine::GetSingleton()->GetLog()->Flush();
            #ifdef _DEBUG
                cout << sNewMessage.Get() << endl;
            #endif
        }
    }

    Engine::Engine()
    {
        mState_ = STATE_NONE;
        bGamePaused_ = false;
        bShutDown_ = false;

        SetConstant("GameVersion", s_str("0045"));

        pFrameFunc_ = nullptr;

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
        pPhysicsMgr_  = PhysicsManager::GetSingleton();
        pSceneMgr_    = SceneManager::GetSingleton();

        pLightMgr_    = LightManager::GetSingleton();
        pCameraMgr_   = CameraManager::GetSingleton();
        pModelMgr_    = ModelManager::GetSingleton();

        pGameplayMgr_ = GameplayManager::GetSingleton();

        //mTimeMgr_->SetProfiling(true);
        pUtilsMgr_->Initialize();

        // Register the log function
        pUtilsMgr_->SetLogFunction(&PrintInLog);
        pTimeMgr_->SetGetTimeFunction(&GetTime);
    }

    Engine::~Engine()
    {
        ShutDown(true);
    }

    void Shutdown()
    {
        Engine::GetSingleton()->ShutDown();
    }

    void RenderQuad( const GUI::Quad& mQuad )
    {
        Engine::GetSingleton()->GetGUIManager()->GetImpl()->RenderQuad(mQuad);
    }

    void Engine::Initialize()
    {
        //pEventMgr_->ToggleDebugOutput();

        // Load configuration
        ReadConfig();

        pUtilsMgr_->SetLogLevel(GetUIntConstant("LogLevel"));

        s_str sLine = "# Starting Frost Engine ("+GetStringConstant("GameVersion")+") #";
        Log(s_str('#', sLine.GetLength()));
        Log(sLine);
        Log(s_str('#', sLine.GetLength())+"\n");

        ReadGraphicsConfig();

        pLocaleMgr_->Initialize();

        s_str sLocale = Engine::GetSingleton()->GetStringConstant("GameLocale");
        if (sLocale == "")
            throw Exception(CLASS_NAME, "No locale specified. Exiting.");

        pLocaleMgr_->SetLocale(sLocale);
        pLocaleMgr_->ReadLocale();

        // Load shaders
        pShaderMgr_->LoadShaders();

        pGameplayMgr_->Initialize();
        pGameplayMgr_->ParseData();

        pPhysicsMgr_->ReadConfig();

        size_t iWindowHnd = 0; pRenderWindow_->getCustomAttribute("WINDOW", &iWindowHnd);
        ostringstream os;
        os << iWindowHnd;
        pInputMgr_->Initialize(OISInputHandler(os.str(), pRenderWindow_->getWidth(), pRenderWindow_->getHeight()));

        if (IsConstantDefined("DoubleClickTime"))
            pInputMgr_->SetDoubleClickTime(s_double(GetFloatConstant("DoubleClickTime")));

        if (IsConstantDefined("MouseBufferLength"))
            pInputMgr_->SetMouseBufferDuration(s_double(GetFloatConstant("MouseBufferLength")));

        if (IsConstantDefined("MouseSensibility"))
            pInputMgr_->SetMouseSensibility(GetFloatConstant("MouseSensibility"));

        pMaterialMgr_->Initialize();
        pSceneMgr_->Initialize();
        pLightMgr_->Initialize();

        s_map<s_str, s_var> lParams;
        lParams["game_version"] = GetStringConstant("GameVersion");
        lParams["scene_manager"] = s_str("FrostSceneMgr");
        lParams["width"] = s_uint((uint)pRenderWindow_->getWidth());
        lParams["height"] = s_uint((uint)pRenderWindow_->getHeight());
        pGUIMgr_ = s_refptr<GUIManager>(new GUIManager(lParams));
        pGUIMgr_->CreateLua();
        pGUIMgr_->EnableCaching(GetBoolConstant("EnableGUICaching"));
        pGUIMgr_->RegisterFrame<GUI::ModelFrame>("ModelFrame");
    }

    void Engine::Loop()
    {
        pGameplayMgr_->SetCurrentGameplay(GetStringConstant("DefaultGameplay"));

        pCameraMgr_->CheckSettings();

        pRenderSystem_->_initRenderTargets();
        pTimeMgr_->Initialize();

        // Dummy hardware occlusion query, to avoid frame stuttering
        Ogre::GpuCommandBufferFlush mBufferFlush;
        s_uint uiDummyHOQFrameQueue = GetUIntConstant("DummyHOQFrameQueue");
        if (uiDummyHOQFrameQueue != 0)
            mBufferFlush.start(uiDummyHOQFrameQueue.Get());

        bRun_ = true;
        s_bool bFirst = true;

        // Start the main loop
        while (bRun_)
        {
            Ogre::WindowEventUtilities::messagePump();

            // Check if the window has been closed
            if (pRenderWindow_->isClosed())
                bRun_ = false;

            // Notify ogre a new frame has started
            if (!pRoot_->_fireFrameStarted())
                bRun_ = false;

            // Call the frame function
            if (pFrameFunc_)
            {
                if (!(*pFrameFunc_)())
                    bRun_ = false;
            }

            // Check there is a camera ready for rendering
            pCameraMgr_->CheckSettings();

            s_float fDelta = s_float(pTimeMgr_->GetDelta());
            if (fDelta > 0.5f) fDelta = 0.5f;

            // Update current gameplay
            pGameplayMgr_->Update(fDelta);

            pSceneMgr_->Update(fDelta);

            // Update cameras' state and animations
            pCameraMgr_->UpdateCameras(fDelta);

            // Update lights' animations
            pLightMgr_->UpdateLights(fDelta);

            // Update physics controled objects
            pPhysicsMgr_->Update(fDelta);

            // Update the GUI
            pGUIMgr_->Update(fDelta);

            // Send shader parameters
            pShaderMgr_->UpdateShaders(fDelta);

            // Update our own render targets
            pGUIMgr_->Begin();

                RenderScene();
                pGUIMgr_->RenderUI();

            pGUIMgr_->End();

            // Render everyting
            if (bFirst)
            {
                Log("\nBegin rendering...");
            }
            pRoot_->_updateAllRenderTargets();
            if (bFirst)
            {
                Log<1>("First frame rendered.\n");
                bFirst = false;
            }

            // Update inputs and timers
            pTimeMgr_->Update();
            pInputMgr_->Update(fDelta);

            if (!pRoot_->_fireFrameEnded())
            {
                Log("Error in Ogre ?");
                bRun_ = false;
            }

            pEventMgr_->FrameEnded();
        }
        Log("\nStopped rendering.\n");

        bRun_ = false;
    }

    void Engine::ShutDown( const s_bool& bForceShutDown )
    {
        if (!bShutDown_)
        {
            if (bRun_ && !bForceShutDown)
                bRun_ = false;
            else
            {
                Log("\nEngine shutdown.\n\nGeneral statistics :");
                Log("    Average FPS : "+ pTimeMgr_->GetAverageFPS());
                Log("    Best FPS : "+ pTimeMgr_->GetBestFPS());
                Log("    Worst FPS : "+ pTimeMgr_->GetWorstFPS()+"\n");

                if (pGUIMgr_)
                    pGUIMgr_->PrintStatistics();

                Log("");

                SaveConfig();

                // End, delete/free everything

                // Print out profiling info
                pTimeMgr_->Print();

                // Delete managers
                GameplayManager::Delete();
                pGUIMgr_.SetNull();
                ModelManager::Delete();
                CameraManager::Delete();
                LightManager::Delete();

                SceneManager::Delete();
                PhysicsManager::Delete();
                MaterialManager::Delete();
                ShaderManager::Delete();
                LocaleManager::Delete();
                InputManager::Delete();
                EventManager::Delete();
                LuaManager::Delete();

                GUI::Close();

                Log("Closing Ogre...");

                // Close OGRE
                if (pRoot_)
                {
                    if (pOgreSceneMgr_)
                    {
                        pRoot_->destroySceneManager(pOgreSceneMgr_.Get());
                        pOgreSceneMgr_ = nullptr;
                    }
                }
                if (pRenderWindow_)
                {
                    pRenderWindow_->destroy();
                    pRenderWindow_ = nullptr;
                }

                pRoot_.Delete();

                bShutDown_ = true;

                Log("Done.");

                TimeManager::Delete();
                UtilsManager::Delete();
            }
        }
    }

    void Engine::ReadGraphicsConfig()
    {
        s_str sRenderSystem = GetStringConstant("RenderSystem");

        if (sRenderSystem == "OpenGL")
        {
            #ifdef WIN32
            if (!File::Exists("RenderSystem_GL_d.dll"))
            #else
            if (!File::Exists("RenderSystem_GL_d.so"))
            #endif
            {
                throw Exception(CLASS_NAME,
                    "Can't load the "+sRenderSystem+" renderer, missing DLL."
                );
            }

            #ifdef _DEBUG
                pRoot_->loadPlugin("RenderSystem_GL_d");
            #else
                pRoot_->loadPlugin("RenderSystem_GL");
            #endif

            pRenderSystem_ = pRoot_->getRenderSystemByName("OpenGL Rendering Subsystem");
            if (!pRenderSystem_)
            {
                throw Exception(CLASS_NAME,
                    "Can't load the "+sRenderSystem+" renderer, maybe a problem with the DLL."
                );
            }
        }
        else if (sRenderSystem == "DirectX")
        {
            #ifdef WIN32
            if (!File::Exists("RenderSystem_Direct3D9_d.dll"))
            #else
            if (!File::Exists("RenderSystem_Direct3D9_d.so"))
            #endif
            {
                throw Exception(CLASS_NAME,
                    "Can't load the "+sRenderSystem+" renderer, missing DLL."
                );
            }

            #ifdef _DEBUG
                pRoot_->loadPlugin("RenderSystem_Direct3D9_d");
            #else
                pRoot_->loadPlugin("RenderSystem_Direct3D9");
            #endif

            pRenderSystem_ = pRoot_->getRenderSystemByName("Direct3D9 Rendering Subsystem");
            if (!pRenderSystem_)
            {
                throw Exception(CLASS_NAME,
                    "Can't load the "+sRenderSystem+" renderer, maybe a problem with the DLL."
                );
            }

            // NOTE : Lua needs this to work properly
            pRenderSystem_->setConfigOption("Floating-point mode", "Consistent");
        }
        else
        {
            throw Exception(CLASS_NAME,
                "Unsupported render system : \""+sRenderSystem+"\"."
            );
        }

        pRoot_->setRenderSystem(pRenderSystem_.Get());
        pRoot_->initialise(false);

        s_uint uiScreenWidth = GetUIntConstant("ScreenWidth");
        s_uint uiScreenHeight = GetUIntConstant("ScreenHeight");

        s_uint uiWindowWidth = GetUIntConstant("WindowWidth");
        s_uint uiWindowHeight = GetUIntConstant("WindowHeight");

        s_bool bFullScreen = GetBoolConstant("FullScreen");
        s_bool bVSync = GetBoolConstant("EnableVSync");

        s_uint uiFSAA = GetUIntConstant("AntiAliasingLevel");

        Ogre::NameValuePairList lOptions;
        // NOTE : 32bit is required for A8R8G8B8 format
        lOptions.insert(Ogre::NameValuePairList::value_type("colourDepth", "32"));
        lOptions.insert(Ogre::NameValuePairList::value_type("vsync", (bVSync ? "yes" : "no")));
        lOptions.insert(Ogre::NameValuePairList::value_type("FSAA", s_str::Convert(uiFSAA).Get()));

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
            throw Exception(CLASS_NAME, "Couldn't create render window.");
        }

        if (sRenderSystem == "OpenGL")
        {
            if (!pRenderSystem_->getCapabilities()->isShaderProfileSupported("glsl"))
            {
                throw Exception(CLASS_NAME,
                    "Frost requires a GLSL compatible card."
                );
            }
        }
        else if (sRenderSystem == "DirectX")
        {
            if (!pRenderSystem_->getCapabilities()->isShaderProfileSupported("hlsl") ||
                !pRenderSystem_->getCapabilities()->isShaderProfileSupported("ps_2_0") ||
                !pRenderSystem_->getCapabilities()->isShaderProfileSupported("vs_2_0"))
            {
                throw Exception(CLASS_NAME,
                    "Frost requires a HLSL (VS and PS 2.0) compatible card."
                );
            }
        }

        // Create the root path
        Ogre::ResourceGroupManager::getSingleton().addResourceLocation("./", "FileSystem", "Frost");
        Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();

        // Create Ogre's scene manager
        if (GetBoolConstant("OctreeSceneManager"))
        {
            #ifdef _DEBUG
                pRoot_->loadPlugin("Plugin_OctreeSceneManager_d");
            #else
                pRoot_->loadPlugin("Plugin_OctreeSceneManager");
            #endif

            pOgreSceneMgr_ = pRoot_->createSceneManager("OctreeSceneManager", "FrostSceneMgr");
        }
        else
            pOgreSceneMgr_ = pRoot_->createSceneManager(Ogre::ST_GENERIC, "FrostSceneMgr");
    }

    void Engine::ReadConfig()
    {
        // Create the Lua state
        pLua_ = pLuaMgr_->CreateLua();
        pLuaMgr_->SetDefaultLua(pLua_);
        Lua::RegisterGlobalFuncs(pLua_);
        CreateGlue(pLua_);

        pLua_->DoFile("DefaultConfig.lua");

        pLua_->PushGlobal("GameOptions");
        pLua_->PushNil();
        while (lua_next(pLua_->GetState(), -2) != 0)
        {
            lDefaultGameOptionList_[pLua_->GetString(-2)] = pLua_->GetValue(-1);
            pLua_->Pop();
        }
        pLua_->Pop();

        try
        {
            pLua_->DoFile("Saves/Config.lua");
        }
        catch (LuaException& e)
        {
            Warning(CLASS_NAME, e.GetDescription());
        }

        pLua_->PushGlobal("GameOptions");
        pLua_->PushNil();
        while (lua_next(pLua_->GetState(), -2) != 0)
        {
            lGameOptionList_[pLua_->GetString(-2)] = pLua_->GetValue(-1);
            pLua_->Pop();
        }
        pLua_->Pop();
    }

    void Engine::SaveConfig()
    {
        File mFile("Saves/Config.lua", File::O);

        mFile.WriteLine("-- Frost Engine configuration overrides :");

        s_map<s_str, s_var>::iterator iterChanged, iterDefault;
        foreach (iterChanged, lGameOptionList_)
        {
            s_bool bWrite = false;
            iterDefault = lDefaultGameOptionList_.Get(iterChanged->first);
            if (iterDefault != lDefaultGameOptionList_.End())
                bWrite = iterDefault->second != iterChanged->second;
            else
                bWrite = true;

            if (bWrite)
            {
                s_str sChanged;
                if (iterChanged->second.IsOfType<s_str>())
                    sChanged = "\""+iterChanged->second.Get<s_str>()+"\"";
                else if (iterChanged->second.IsOfType<s_float>())
                    sChanged = s_str::Convert(iterChanged->second.Get<s_float>());
                else if (iterChanged->second.IsOfType<s_bool>())
                    sChanged = s_str::Convert(iterChanged->second.Get<s_bool>());
                else if (iterChanged->second.IsOfType<void>())
                    sChanged = "nil";
                else
                    continue;

                mFile.WriteLine("GameOptions[\""+iterChanged->first+"\"] = "+sChanged+";");
            }
        }
    }

    s_var Engine::GetConstant( const s_str& sConstantName )
    {
        s_map<s_str, s_var>::iterator iter = lGameOptionList_.Get(sConstantName);
        if (iter == lGameOptionList_.End())
            return s_var();
        return iter->second;
    }

    void Engine::SetConstant( const s_str& sConstantName, const s_var& vValue )
    {
        s_map<s_str, s_var>::iterator iter = lGameOptionList_.Get(sConstantName);
        if (iter != lGameOptionList_.End())
        {
            if (iter->second == vValue)
                return;
        }

        lGameOptionList_[sConstantName] = vValue;

        Event mEvent("GAME_CONSTANT_CHANGED");
        mEvent.Add(sConstantName);
        mEvent.Add(vValue);
        EventManager::GetSingleton()->FireEvent(mEvent);
    }

    s_bool Engine::IsConstantDefined( const s_str& sConstantName ) const
    {
        return (lGameOptionList_.Get(sConstantName) != lGameOptionList_.End());
    }

    s_str Engine::GetStringConstant( const s_str& sConstantName )
    {
        s_map<s_str, s_var>::iterator iter = lGameOptionList_.Get(sConstantName);
        if (iter == lGameOptionList_.End())
            return s_str();
        return iter->second.Get<s_str>();
    }

    s_float Engine::GetFloatConstant( const s_str& sConstantName )
    {
        s_map<s_str, s_var>::iterator iter = lGameOptionList_.Get(sConstantName);
        if (iter == lGameOptionList_.End())
            return s_float();
        return iter->second.Get<s_float>();
    }

    s_uint Engine::GetUIntConstant( const s_str& sConstantName )
    {
        s_map<s_str, s_var>::iterator iter = lGameOptionList_.Get(sConstantName);
        if (iter == lGameOptionList_.End())
            return s_uint();
        return s_uint(iter->second.Get<s_float>());
    }

    s_bool Engine::GetBoolConstant( const s_str& sConstantName )
    {
        s_map<s_str, s_var>::iterator iter = lGameOptionList_.Get(sConstantName);
        if (iter == lGameOptionList_.End())
            return s_bool();
        return iter->second.Get<s_bool>();
    }

    s_str Engine::GetRenderer() const
    {
        return lGameOptionList_.Get("RenderSystem")->second.Get<s_str>();
    }

    s_ptr<File> Engine::GetLog()
    {
        static File mLog("Frost.log", File::O);
        return &mLog;
    }

    s_ptr<Ogre::RenderSystem> Engine::GetRenderSystem()
    {
        return pRenderSystem_;
    }

    s_ptr<Ogre::Root> Engine::GetOgreRoot()
    {
        return pRoot_;
    }

    s_ptr<Ogre::SceneManager> Engine::GetOgreSceneManager()
    {
        return pOgreSceneMgr_;
    }

    s_ptr<Ogre::RenderWindow> Engine::GetRenderWindow()
    {
        return pRenderWindow_;
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

    s_wptr<GUIManager> Engine::GetGUIManager()
    {
        return pGUIMgr_;
    }

    void Engine::SetFrameFunction( Function pFrameFunc )
    {
        pFrameFunc_ = pFrameFunc;
    }

    void Engine::TakeScreenshot()
    {
        s_str sFileName = "Screenshots/scr";
        sFileName += "_"+pTimeMgr_->GetYear();
        sFileName += "_"+s_str::Convert(pTimeMgr_->GetMonth(), 2);
        sFileName += "_"+s_str::Convert(pTimeMgr_->GetDay(), 2);
        sFileName += "_"+s_str::Convert(pTimeMgr_->GetHour(), 2);
        sFileName += "_"+s_str::Convert(pTimeMgr_->GetMinutes(), 2);
        sFileName += "_"+s_str::Convert(pTimeMgr_->GetSeconds(), 2);
        sFileName += ".png";
        pRenderWindow_->writeContentsToFile(sFileName.Get());
    }

    void Engine::RenderScene()
    {
        if (pShaderMgr_->IsPostProcessingEnabled())
            pShaderMgr_->RenderPostProcessedScene();
        else
            pRoot_->getRenderSystem()->clearFrameBuffer(Ogre::FBT_COLOUR, Ogre::ColourValue::ZERO);
    }

    void Engine::CreateGlue( s_ptr<Lua::State> pLua )
    {
        pLua->Register<LuaEngine>();
        pLua->PushNew<LuaEngine>();
        pLua->SetGlobal("Frost");
    }

    void Engine::SetState( State mState )
    {
        mState_ = mState;
    }

    Engine::State Engine::GetState() const
    {
        return mState_;
    }
}
