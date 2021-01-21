/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*         Frost interface header         */
/*                                        */
/*                                        */

/* Warning : This is a precompiled header.
*            Modifying it will trigger a
*            full rebuild !
*/

#ifndef FROST_ENGINE_H
#define FROST_ENGINE_H

#include "frost.h"
#include <frost_utils_manager.h>

namespace Ogre
{
    class Root;
    class SceneManager;
    class RenderSystem;
    class RenderWindow;
}

namespace Frost
{
    class GUIManager;

    /// Game manager
    /** A typical application using Frost :
    *   - create a new Engine by calling Engine::GetSingleton()
    *   - call Initialize() and check the return value
    *   - create a Frame function that will be called on each frame
    *   - register it to Engine with SetFrameFunction()
    *   - intitialize all your resources (models, ...)
    *   - call Loop()
    *   - delete all your resources
    *   - call Engine::Delete()
    */
    class Engine : public Manager<Engine>
    {
    friend class Manager<Engine>;
    public :

        enum State
        {
            STATE_NONE,
            STATE_GAME,
            STATE_EDITOR
        };

        /// Initializes everything.
        /** \return 'true' if everything went fine
        *   \note Actually, this function initializes all the libraries,
        *         Frost's managers, and crucial elements (main camera, ...).
        */
        void                Initialize();

        /// Enters the main loop.
        /** \note You can only exit the main loop by returning 'false' in
        *         your frame function.<br>
        *         Advanced users : you can bypass this function and create your
        *         own main loop. See implementation to know what you should do.
        */
        void                Loop();

        /// Frees all the allocated resources.
        /** \param bForceShutDown 'true' to delete everything now,
        *                         'false' to let it end the current frame
        *   \note This function only deletes what the Engine has created for
        *         you. You still have to "delete" what you "new".
        */
        void                ShutDown(const s_bool& bForceShutDown = false);

        /// Changes the engine's state.
        /** \param mState The new state (see State)
        */
        void                SetState(State mState);

        /// Returns the current engine's state.
        /** \return The current engine's state
        */
        State               GetState() const;

        /// Sets whether the game should be paused or not.
        /** \param bPause 'true' if you want to pause the game
        */
        void                PauseGame(const s_bool& bPause);

        /// Checks if the game is paused.
        /** \return 'true' if the game is paused
        */
        const s_bool&       IsGamePaused() const;

        /// Parses Config.lua.
        void                ReadConfig();

        /// Initializes Ogre renderer and window.
        /** \note Automatically called by Initialize().
        *         Only use if you want to bypass this function.
        */
        void                ReadGraphicsConfig();

        /// Updates the content of Config.lua.
        void                SaveConfig();

        /// Returns a game constant.
        /** \param sConstantName The name of the constant you're searching for
        *   \return An untyped value containing the value you asked
        */
        s_var               GetConstant(const s_str& sConstantName);

        /// Sets the value of a game constant.
        /** \param sConstantName The name of the constant to modify
        *   \param vValue        The value to assign it
        *   \note Fires the GAME_CONSTANT_CHANGED event.
        */
        void                SetConstant(const s_str& sConstantName, const s_var& vValue);

        /// Checks if a game constant has been defined.
        /** \param sConstantName The name of the constant you're searching for
        *   \return 'true' if the constant has been defined
        */
        s_bool              IsConstantDefined(const s_str& sConstantName) const;

        /// Returns a game constant.
        /** \param sConstantName The name of the constant you're searching for
        *   \return A string containing the value you asked
        */
        s_str               GetStringConstant(const s_str& sConstantName);

        /// Returns a game constant.
        /** \param sConstantName The name of the constant you're searching for
        *   \return A float containing the value you asked
        */
        s_float             GetFloatConstant(const s_str& sConstantName);

        /// Returns a game constant.
        /** \param sConstantName The name of the constant you're searching for
        *   \return A uint containing the value you asked
        */
        s_uint              GetUIntConstant(const s_str& sConstantName);

        /// Returns a game constant.
        /** \param sConstantName The name of the constant you're searching for
        *   \return A bool containing the value you asked
        */
        s_bool              GetBoolConstant(const s_str& sConstantName);

        /// Returns the log file used to print debug infos.
        /** \return The log file used to print debug infos
        */
        s_ptr<File>         GetLog();

        /// Returns the Ogre::Root object.
        /** \return The Ogre::Root object
        *   \note Only use this when you don't have a choice.
        */
        s_ptr<Ogre::Root>   GetOgreRoot();

        /// Returns the Ogre::SceneManager used for the main scene.
        /** \return The Ogre::SceneManager used for the main scene
        */
        s_ptr<Ogre::SceneManager> GetOgreSceneManager();

        /// Returns the Lua state used by the engine.
        /** \return The Lua state used by the engine
        */
        s_ptr<Lua::State>   GetLua();

        /// Creates a lua glue on the provided Lua state.
        /** \param pLua The lua state
        */
        void                CreateGlue(s_ptr<Lua::State> pLua);

        /// Returns the render window's width.
        /** \return The render window's width
        */
        s_uint              GetScreenWidth() const;

        /// Returns the render window's height.
        /** \return The render window's height
        */
        s_uint              GetScreenHeight() const;

        /// Returns the main GUI manager.
        /** \return The main GUI manager
        */
        s_wptr<GUIManager>  GetGUIManager();

        /// Returns the name of the currently used renderer.
        /** \return The name of the currently used renderer
        *   \note Can be either "DirectX" or "OpenGL".
        */
        s_str               GetRenderer() const;

        /// Returns the Ogre::RenderSystem used by the engine.
        /** \return The Ogre::RenderSystem used by the engine
        */
        s_ptr<Ogre::RenderSystem> GetRenderSystem();

        /// Returns the Ogre::RenderWindow used by the engine.
        /** \return The Ogre::RenderWindow used by the engine
        */
        s_ptr<Ogre::RenderWindow> GetRenderWindow();

        typedef             s_bool (*Function)();
        /// Sets the new function to call on each frame.
        /** \param pFrameFunc The new function
        *   \note You should always define a frame function :
        *         the engine doesn't know when you want the game
        *         to stop. So, if you don't provide the frame
        *         function, the engine will never stop correctly.<br>
        *         Return 'false' in your frame function to stop the
        *         game.
        */
        void                SetFrameFunction(Function pFrameFunc);

        /// Writes the content of the main render target into a file.
        /** \note Files are automatically named and stored in the
        *         Screenshots folder.
        */
        void                TakeScreenshot();

        /// Renders a quad containing the 3D scene.
        /** \note Must be called between SpriteManager::Begin() and
        *         SpriteManager::End().
        */
        void                RenderScene();

        static const s_str CLASS_NAME;

    protected :

        /// Default constructor.
        /** \note This is a singleton class. That means that there can
        *         only be a single instance at the same time. That's why
        *         the constructor is not in public scope.<br>
        *         If you need to get a pointer to that unique instance,
        *         call Engine::GetSingleton().<br>
        *         Note that, if GetSingleton() has been called at least once,
        *         you'll have to delete the returned pointer when you close
        *         your program by calling Engine::Delete().
        */
        Engine();

        /// Destructor.
        /** \note If GetSingleton() has been called at least once,
        *         you'll have to delete the returned pointer when
        *         you close your program by calling Delete().
        */
        ~Engine();

        /// Copy constructor.
        Engine(const Engine& mMgr);

        /// Assignment operator.
        Engine& operator = (const Engine& mMgr);

    private :

        s_bool bShutDown_;
        s_bool bRun_;

        // Game state
        State  mState_;
        s_bool bGamePaused_;

        // Game constants
        s_map<s_str, s_var> lDefaultGameOptionList_;
        s_map<s_str, s_var> lGameOptionList_;

        // Frame function
        Function pFrameFunc_;

        // Managers
        s_ptr<UtilsManager>    pUtilsMgr_;
        s_ptr<LuaManager>      pLuaMgr_;
        s_ptr<EventManager>    pEventMgr_;
        s_ptr<TimeManager>     pTimeMgr_;
        s_ptr<InputManager>    pInputMgr_;
        s_refptr<GUIManager>   pGUIMgr_;
        s_ptr<LocaleManager>   pLocaleMgr_;
        s_ptr<ModelManager>    pModelMgr_;
        s_ptr<CameraManager>   pCameraMgr_;
        s_ptr<SceneManager>    pSceneMgr_;
        s_ptr<ShaderManager>   pShaderMgr_;
        s_ptr<MaterialManager> pMaterialMgr_;
        s_ptr<LightManager>    pLightMgr_;
        s_ptr<GameplayManager> pGameplayMgr_;
        s_ptr<PhysicsManager>  pPhysicsMgr_;

        // Lua
        s_ptr<Lua::State>          pLua_;

        // OGRE variables
        s_ptr<Ogre::Root>         pRoot_;
        s_ptr<Ogre::RenderSystem> pRenderSystem_;
        s_ptr<Ogre::SceneManager> pOgreSceneMgr_;
        s_ptr<Ogre::RenderWindow> pRenderWindow_;
    };
}

#endif
