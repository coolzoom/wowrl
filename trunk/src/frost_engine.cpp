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
#include "path/frost_pathmanager.h"
#include "model/frost_modelmanager.h"
#include "camera/frost_cameramanager.h"
#include "scene/frost_scenemanager.h"
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
#include <OgreMaterialManager.h>
#include <OgreUnifiedHighLevelGpuProgram.h>

using namespace std;

namespace Frost
{
    const s_str Engine::CLASS_NAME = "Engine";

    Engine::Engine() : mLog_("Frost.log", FILE_O)
    {
        uiScreenWidth_ = 1024;
        uiScreenHeight_ = 768;
        iGameState_ = 0;
        bGamePaused_ = false;
        sGameVersion_ = "0.045";
        iMaxComputedPaths_ = 2;
        bEnableSpecular_ = false;
        pFrameFunc_ = NULL;

        bShutDown_ = false;

        // Initialize OGRE
        pRoot_ = new Ogre::Root("", "");

        // Create singletons;
        pUtilsMgr_    = UtilsManager::GetSingleton();
        pLuaMgr_      = LuaManager::GetSingleton();
        pEventMgr_    = EventManager::GetSingleton();
        pTimeMgr_     = TimeManager::GetSingleton();
        pInputMgr_    = InputManager::GetSingleton();
        pLocaleMgr_   = LocaleManager::GetSingleton();
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

        s_str sLine = "# Starting Frost Engine (" + sGameVersion_ + ") #";
        Log(s_str('#', sLine.GetLength()));
        Log(sLine);
        Log(s_str('#', sLine.GetLength())+"\n");

        // Load configuration
        if (!pLua_->DoFile("Config.lua"))
            return false;

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
        if (!LoadShaders_())
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
        pSpriteMgr_->Initialize(pOgreSceneMgr_, s_float(uiScreenWidth_), s_float(uiScreenHeight_));

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

    s_bool Engine::LoadShaders_()
    {
        Ogre::HighLevelGpuProgram* pProgram;
        Ogre::UnifiedHighLevelGpuProgram* pUProgram;

        // --------
        // Skinning : vertex shaders
        // --------

        // HLSL vertex shader
        pProgram = Ogre::HighLevelGpuProgramManager::getSingleton().createProgram(
            "Character_Skinning_HLSL_VS", "Frost", "hlsl", Ogre::GPT_VERTEX_PROGRAM
        ).get();
        pProgram->setSourceFile("Shaders/Character/Character_Skinning_vs.hlsl");
        pProgram->setSkeletalAnimationIncluded(true);
        pProgram->setParameter("entry_point", "main_vs");
        pProgram->setParameter("target", "vs_2_0");
        pProgram->setParameter("column_major_matrices", "false");

        // GLSL vertex shader
        pProgram = Ogre::HighLevelGpuProgramManager::getSingleton().createProgram(
            "Character_Skinning_GLSL_VS", "Frost", "glsl", Ogre::GPT_VERTEX_PROGRAM
        ).get();
        pProgram->setSourceFile("Shaders/Character/Character_Skinning_vs.glsl");
        pProgram->setSkeletalAnimationIncluded(true);

        // Unified vertex shader
        pUProgram = static_cast<Ogre::UnifiedHighLevelGpuProgram*>(
            Ogre::HighLevelGpuProgramManager::getSingleton().createProgram(
            "Character_Skinning_VS", "Frost", "unified", Ogre::GPT_VERTEX_PROGRAM
        ).get());
        pUProgram->addDelegateProgram("Character_Skinning_HLSL_VS");
        pUProgram->addDelegateProgram("Character_Skinning_GLSL_VS");
        pUProgram->load();

        // --------
        // Skinning : pixel shaders
        // --------

        // HLSL pixel shader
        pProgram = Ogre::HighLevelGpuProgramManager::getSingleton().createProgram(
            "Character_Skinning_HLSL_PS", "Frost", "hlsl", Ogre::GPT_FRAGMENT_PROGRAM
        ).get();
        pProgram->setSourceFile("Shaders/Character/Character_Skinning_ps.hlsl");
        pProgram->setParameter("entry_point", "main_ps");
        pProgram->setParameter("target", "ps_2_0");

        // GLSL pixel shader
        pProgram = Ogre::HighLevelGpuProgramManager::getSingleton().createProgram(
            "Character_Skinning_GLSL_PS", "Frost", "glsl", Ogre::GPT_FRAGMENT_PROGRAM
        ).get();
        pProgram->setSourceFile("Shaders/Character/Character_Skinning_ps.glsl");

        // Unified pixel shader
        pUProgram = static_cast<Ogre::UnifiedHighLevelGpuProgram*>(
            Ogre::HighLevelGpuProgramManager::getSingleton().createProgram(
            "Character_Skinning_PS", "Frost", "unified", Ogre::GPT_FRAGMENT_PROGRAM
        ).get());
        pUProgram->addDelegateProgram("Character_Skinning_HLSL_PS");
        pUProgram->addDelegateProgram("Character_Skinning_GLSL_PS");
        pUProgram->load();

        // --------
        // GUI : desaturation pixel shaders
        // --------

        // HLSL pixel shader
        pProgram = Ogre::HighLevelGpuProgramManager::getSingleton().createProgram(
            "GUI_Desaturation_HLSL_PS", "Frost", "hlsl", Ogre::GPT_FRAGMENT_PROGRAM
        ).get();
        pProgram->setSourceFile("Shaders/GUI/GUI_Desaturation_ps.hlsl");
        pProgram->setParameter("entry_point", "main_ps");
        pProgram->setParameter("target", "ps_2_0");

        // GLSL pixel shader
        pProgram = Ogre::HighLevelGpuProgramManager::getSingleton().createProgram(
            "GUI_Desaturation_GLSL_PS", "Frost", "glsl", Ogre::GPT_FRAGMENT_PROGRAM
        ).get();
        pProgram->setSourceFile("Shaders/GUI/GUI_Desaturation_ps.glsl");

        // Unified pixel shader
        pUProgram = static_cast<Ogre::UnifiedHighLevelGpuProgram*>(
            Ogre::HighLevelGpuProgramManager::getSingleton().createProgram(
            "GUI_Desaturation_PS", "Frost", "unified", Ogre::GPT_FRAGMENT_PROGRAM
        ).get());
        pUProgram->addDelegateProgram("GUI_Desaturation_HLSL_PS");
        pUProgram->addDelegateProgram("GUI_Desaturation_GLSL_PS");
        pUProgram->load();

        // --------
        // Terrain : simple vertex shaders
        // --------

        // [1] Texture
        if (bEnableSpecular_)
        {
            // HLSL vertex shader
            pProgram = Ogre::HighLevelGpuProgramManager::getSingleton().createProgram(
                "Terrain_Specular_HLSL_VS", "Frost", "hlsl", Ogre::GPT_VERTEX_PROGRAM
            ).get();
            pProgram->setSourceFile("Shaders/Terrain/Terrain_Specular_vs.hlsl");
            pProgram->setParameter("entry_point", "main_vs");
            pProgram->setParameter("target", "vs_2_0");

            // GLSL vertex shader
            pProgram = Ogre::HighLevelGpuProgramManager::getSingleton().createProgram(
                "Terrain_Specular_GLSL_VS", "Frost", "glsl", Ogre::GPT_VERTEX_PROGRAM
            ).get();
            pProgram->setSourceFile("Shaders/Terrain/Terrain_Specular_vs.glsl");

            // Unified vertex shader
            pUProgram = static_cast<Ogre::UnifiedHighLevelGpuProgram*>(
                Ogre::HighLevelGpuProgramManager::getSingleton().createProgram(
                "Terrain_Specular_VS", "Frost", "unified", Ogre::GPT_VERTEX_PROGRAM
            ).get());
            pUProgram->addDelegateProgram("Terrain_Specular_HLSL_VS");
            pUProgram->addDelegateProgram("Terrain_Specular_GLSL_VS");
            pUProgram->load();
        }

        s_str sParams;
        if (bEnableSpecular_) sParams += ",SPECULAR=1";

        // [2] Textures

        // HLSL vertex shader
        pProgram = Ogre::HighLevelGpuProgramManager::getSingleton().createProgram(
            "Terrain_Splatting_2_HLSL_VS", "Frost", "hlsl", Ogre::GPT_VERTEX_PROGRAM
        ).get();
        pProgram->setSourceFile("Shaders/Terrain/Terrain_Splatting_vs.hlsl");
        pProgram->setParameter("entry_point", "main_vs");
        pProgram->setParameter("target", "vs_2_0");
        pProgram->setParameter("preprocessor_defines", ("LAYER=2"+sParams).Get());

        // GLSL vertex shader
        pProgram = Ogre::HighLevelGpuProgramManager::getSingleton().createProgram(
            "Terrain_Splatting_2_GLSL_VS", "Frost", "glsl", Ogre::GPT_VERTEX_PROGRAM
        ).get();
        pProgram->setSourceFile("Shaders/Terrain/Terrain_Splatting_vs.glsl");
        pProgram->setParameter("preprocessor_defines", ("LAYER=2"+sParams).Get());

        // Unified vertex shader
        pUProgram = static_cast<Ogre::UnifiedHighLevelGpuProgram*>(
            Ogre::HighLevelGpuProgramManager::getSingleton().createProgram(
            "Terrain_Splatting_2_VS", "Frost", "unified", Ogre::GPT_VERTEX_PROGRAM
        ).get());
        pUProgram->addDelegateProgram("Terrain_Splatting_2_HLSL_VS");
        pUProgram->addDelegateProgram("Terrain_Splatting_2_GLSL_VS");
        pUProgram->load();

        // [3] Textures

        // HLSL vertex shader
        pProgram = Ogre::HighLevelGpuProgramManager::getSingleton().createProgram(
            "Terrain_Splatting_3_HLSL_VS", "Frost", "hlsl", Ogre::GPT_VERTEX_PROGRAM
        ).get();
        pProgram->setSourceFile("Shaders/Terrain/Terrain_Splatting_vs.hlsl");
        pProgram->setParameter("entry_point", "main_vs");
        pProgram->setParameter("target", "vs_2_0");
        pProgram->setParameter("preprocessor_defines", ("LAYER=3"+sParams).Get());

        // GLSL vertex shader
        pProgram = Ogre::HighLevelGpuProgramManager::getSingleton().createProgram(
            "Terrain_Splatting_3_GLSL_VS", "Frost", "glsl", Ogre::GPT_VERTEX_PROGRAM
        ).get();
        pProgram->setSourceFile("Shaders/Terrain/Terrain_Splatting_vs.glsl");
        pProgram->setParameter("preprocessor_defines", ("LAYER=3"+sParams).Get());

        // Unified vertex shader
        pUProgram = static_cast<Ogre::UnifiedHighLevelGpuProgram*>(
            Ogre::HighLevelGpuProgramManager::getSingleton().createProgram(
            "Terrain_Splatting_3_VS", "Frost", "unified", Ogre::GPT_VERTEX_PROGRAM
        ).get());
        pUProgram->addDelegateProgram("Terrain_Splatting_3_HLSL_VS");
        pUProgram->addDelegateProgram("Terrain_Splatting_3_GLSL_VS");
        pUProgram->load();

        // [4] Textures

        // HLSL vertex shader
        pProgram = Ogre::HighLevelGpuProgramManager::getSingleton().createProgram(
            "Terrain_Splatting_4_HLSL_VS", "Frost", "hlsl", Ogre::GPT_VERTEX_PROGRAM
        ).get();
        pProgram->setSourceFile("Shaders/Terrain/Terrain_Splatting_vs.hlsl");
        pProgram->setParameter("entry_point", "main_vs");
        pProgram->setParameter("target", "vs_2_0");
        pProgram->setParameter("preprocessor_defines", ("LAYER=4"+sParams).Get());

        // GLSL vertex shader
        pProgram = Ogre::HighLevelGpuProgramManager::getSingleton().createProgram(
            "Terrain_Splatting_4_GLSL_VS", "Frost", "glsl", Ogre::GPT_VERTEX_PROGRAM
        ).get();
        pProgram->setSourceFile("Shaders/Terrain/Terrain_Splatting_vs.glsl");
        pProgram->setParameter("preprocessor_defines", ("LAYER=4"+sParams).Get());

        // Unified vertex shader
        pUProgram = static_cast<Ogre::UnifiedHighLevelGpuProgram*>(
            Ogre::HighLevelGpuProgramManager::getSingleton().createProgram(
            "Terrain_Splatting_4_VS", "Frost", "unified", Ogre::GPT_VERTEX_PROGRAM
        ).get());
        pUProgram->addDelegateProgram("Terrain_Splatting_4_HLSL_VS");
        pUProgram->addDelegateProgram("Terrain_Splatting_4_GLSL_VS");
        pUProgram->load();

        // --------
        // Terrain : splatting pixel shaders
        // --------

        // [1] Texture
        if (bEnableSpecular_)
        {
            // HLSL pixel shader
            pProgram = Ogre::HighLevelGpuProgramManager::getSingleton().createProgram(
                "Terrain_Specular_HLSL_PS", "Frost", "hlsl", Ogre::GPT_FRAGMENT_PROGRAM
            ).get();
            pProgram->setSourceFile("Shaders/Terrain/Terrain_Specular_ps.hlsl");
            pProgram->setParameter("entry_point", "main_ps");
            pProgram->setParameter("target", "ps_2_0");

            // GLSL pixel shader
            pProgram = Ogre::HighLevelGpuProgramManager::getSingleton().createProgram(
                "Terrain_Specular_GLSL_PS", "Frost", "glsl", Ogre::GPT_FRAGMENT_PROGRAM
            ).get();
            pProgram->setSourceFile("Shaders/Terrain/Terrain_Specular_ps.glsl");

            // Unified pixel shader
            pUProgram = static_cast<Ogre::UnifiedHighLevelGpuProgram*>(
                Ogre::HighLevelGpuProgramManager::getSingleton().createProgram(
                "Terrain_Specular_PS", "Frost", "unified", Ogre::GPT_FRAGMENT_PROGRAM
            ).get());
            pUProgram->addDelegateProgram("Terrain_Specular_HLSL_PS");
            pUProgram->addDelegateProgram("Terrain_Specular_GLSL_PS");
            pUProgram->load();
        }

        // [2] Textures

        // HLSL pixel shader
        pProgram = Ogre::HighLevelGpuProgramManager::getSingleton().createProgram(
            "Terrain_Splatting_2_HLSL_PS", "Frost", "hlsl", Ogre::GPT_FRAGMENT_PROGRAM
        ).get();
        pProgram->setSourceFile("Shaders/Terrain/Terrain_Splatting_ps.hlsl");
        pProgram->setParameter("entry_point", "main_ps");
        pProgram->setParameter("target", "ps_2_0");
        pProgram->setParameter("preprocessor_defines", ("LAYER=2"+sParams).Get());

        // GLSL pixel shader
        pProgram = Ogre::HighLevelGpuProgramManager::getSingleton().createProgram(
            "Terrain_Splatting_2_GLSL_PS", "Frost", "glsl", Ogre::GPT_FRAGMENT_PROGRAM
        ).get();
        pProgram->setSourceFile("Shaders/Terrain/Terrain_Splatting_ps.glsl");
        pProgram->setParameter("preprocessor_defines", ("LAYER=2"+sParams).Get());

        // Unified pixel shader
        pUProgram = static_cast<Ogre::UnifiedHighLevelGpuProgram*>(
            Ogre::HighLevelGpuProgramManager::getSingleton().createProgram(
            "Terrain_Splatting_2_PS", "Frost", "unified", Ogre::GPT_FRAGMENT_PROGRAM
        ).get());
        pUProgram->addDelegateProgram("Terrain_Splatting_2_HLSL_PS");
        pUProgram->addDelegateProgram("Terrain_Splatting_2_GLSL_PS");
        pUProgram->load();

        // [3] Textures

        // HLSL pixel shader
        pProgram = Ogre::HighLevelGpuProgramManager::getSingleton().createProgram(
            "Terrain_Splatting_3_HLSL_PS", "Frost", "hlsl", Ogre::GPT_FRAGMENT_PROGRAM
        ).get();
        pProgram->setSourceFile("Shaders/Terrain/Terrain_Splatting_ps.hlsl");
        pProgram->setParameter("entry_point", "main_ps");
        pProgram->setParameter("target", "ps_2_0");
        pProgram->setParameter("preprocessor_defines", ("LAYER=3"+sParams).Get());

        // GLSL pixel shader
        pProgram = Ogre::HighLevelGpuProgramManager::getSingleton().createProgram(
            "Terrain_Splatting_3_GLSL_PS", "Frost", "glsl", Ogre::GPT_FRAGMENT_PROGRAM
        ).get();
        pProgram->setSourceFile("Shaders/Terrain/Terrain_Splatting_ps.glsl");
        pProgram->setParameter("preprocessor_defines", ("LAYER=3"+sParams).Get());

        // Unified pixel shader
        pUProgram = static_cast<Ogre::UnifiedHighLevelGpuProgram*>(
            Ogre::HighLevelGpuProgramManager::getSingleton().createProgram(
            "Terrain_Splatting_3_PS", "Frost", "unified", Ogre::GPT_FRAGMENT_PROGRAM
        ).get());
        pUProgram->addDelegateProgram("Terrain_Splatting_3_HLSL_PS");
        pUProgram->addDelegateProgram("Terrain_Splatting_3_GLSL_PS");
        pUProgram->load();

        // [4] Textures

        // HLSL pixel shader
        pProgram = Ogre::HighLevelGpuProgramManager::getSingleton().createProgram(
            "Terrain_Splatting_4_HLSL_PS", "Frost", "hlsl", Ogre::GPT_FRAGMENT_PROGRAM
        ).get();
        pProgram->setSourceFile("Shaders/Terrain/Terrain_Splatting_ps.hlsl");
        pProgram->setParameter("entry_point", "main_ps");
        pProgram->setParameter("target", "ps_2_0");
        pProgram->setParameter("preprocessor_defines", ("LAYER=4"+sParams).Get());

        // GLSL pixel shader
        pProgram = Ogre::HighLevelGpuProgramManager::getSingleton().createProgram(
            "Terrain_Splatting_4_GLSL_PS", "Frost", "glsl", Ogre::GPT_FRAGMENT_PROGRAM
        ).get();
        pProgram->setSourceFile("Shaders/Terrain/Terrain_Splatting_ps.glsl");
        pProgram->setParameter("preprocessor_defines", ("LAYER=4"+sParams).Get());

        // Unified pixel shader
        pUProgram = static_cast<Ogre::UnifiedHighLevelGpuProgram*>(
            Ogre::HighLevelGpuProgramManager::getSingleton().createProgram(
            "Terrain_Splatting_4_PS", "Frost", "unified", Ogre::GPT_FRAGMENT_PROGRAM
        ).get());
        pUProgram->addDelegateProgram("Terrain_Splatting_4_HLSL_PS");
        pUProgram->addDelegateProgram("Terrain_Splatting_4_GLSL_PS");
        pUProgram->load();

        return true;
    }

    s_bool Engine::ReadGameConfig_()
    {
        sGameVersion_ = pLua_->GetGlobalString("sGameVersion", false, "0");

        s_str sGameplay = pLua_->GetGlobalString("sDefaultGameplay");
        pGameplayMgr_->SetCurrentGameplay(sGameplay);

        return true;
    }

    s_bool Engine::ReadGraphicsConfig_()
    {
        s_str sRenderSystem = pLua_->GetGlobalString("sRenderSystem");
        if (sRenderSystem[0] == 'D')
        {
            #ifdef _DEBUG
                pRoot_->loadPlugin("RenderSystem_Direct3D9_d");
            #else
                pRoot_->loadPlugin("RenderSystem_Direct3D9");
            #endif
            sRenderer_ = "Direct3D9";
        }
        else
        {
            #ifdef _DEBUG
                pRoot_->loadPlugin("RenderSystem_GL_d");
            #else
                pRoot_->loadPlugin("RenderSystem_GL");
            #endif
            sRenderer_ = "OpenGL";
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

        uiScreenWidth_ = s_uint(pLua_->GetGlobalInt("iScreenWidth", false, 1024));
        uiScreenHeight_ = s_uint(pLua_->GetGlobalInt("iScreenHeight", false, 768));
        // NOTE : 32bit is required for A8R8G8B8 format
        s_uint uiScreenDepth = 32;
        s_bool bFullScreen = pLua_->GetGlobalBool("bFullScreen", false, false);
        s_bool bVSync = pLua_->GetGlobalBool("bVSync", false, false);
        s_uint uiFSAA = s_uint(pLua_->GetGlobalInt("iFSAA", false, 0));
        bEnableSpecular_ = s_bool(pLua_->GetGlobalBool("bEnableSpecular", false, false));

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

    s_var Engine::GetConstant( const s_str& sName ) const
    {
        if (sName == "MaxComputedPaths")
        {
            return iMaxComputedPaths_;
        }
        else if (sName == "GameVersion")
        {
            return sGameVersion_;
        }
        else if (sName == "EnableSpecular")
        {
            return bEnableSpecular_;
        }
        else
            return s_var();
    }

    const s_str& Engine::GetRenderer() const
    {
        return sRenderer_;
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

    s_ptr<Lua::State> Engine::GetLua()
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
