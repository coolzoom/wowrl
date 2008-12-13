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

namespace Frost
{
    enum GameState
    {
        GAMESTATE_GAME,
        GAMESTATE_LOADING
    };

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

        /// Initializes everything.
        /** \return 'true' if everything went fine
        *   \note Actually, this function initializes all the libraries,
        *         Frost's managers, and crucial elements (main camera, ...).
        */
        s_bool              Initialize();

        /// Enters the main loop.
        /** \note You can only exit the main loop by returning 'false' in
        *         your frame function.<br>
        *         Advanced users : you can bypass this function and create your
        *         own main loop. See implementation to know what you should do.
        */
        void                Loop();

        /// Frees all the allocated resources.
        /** \note This function only deletes what the Engine has created for
        *         you. You still have to "delete" what you "new".
        */
        void                ShutDown();

        /// Changes the game state.
        /** \param mState The new state (see GameState)
        */
        void                SetState(GameState mState);

        /// Returns the actual game state.
        /** \return The actual game state
        */
        GameState           GetState() const;

        /// Sets whether the game should be paused or not.
        /** \param bPause 'true' if you want to pause the game
        */
        void                PauseGame(s_bool bPause);

        /// Checks if the game is paused.
        /** \return 'true' if the game is paused
        */
        const s_bool&       IsGamePaused() const;

        /// Returns a game constant.
        /** \param sConstantName The name of the constant you're searching for
        *   \return An untyped value containing the value you asked
        */
        s_var               GetConstant(s_str sConstantName) const;

        /// Returns the Ogre::Log used to print infos in the log.
        /** \return The Ogre::Log used to print infos in the log.
        *   \note This is not Ogre's log, but Frost's.
        */
        s_ptr<Ogre::Log>   GetLog();

        /// Returns the Ogre::SceneManager used for the main scene.
        /** \return The Ogre::SceneManager used for the main scene
        */
        s_ptr<Ogre::SceneManager> GetOgreSceneManager();

        /// Returns the Lua state used by the engine.
        /** \return The Lua state used by the engine
        */
        lua_State*          GetLua();

        /// Returns the render window's width.
        /** \return The render window's width
        */
        const s_uint&       GetScreenWidth() const;

        /// Returns the render window's height.
        /** \return The render window's height
        */
        const s_uint&       GetScreenHeight() const;

        /// Returns the base camera.
        /** \return The base camera
        */
        s_ptr<Camera>       GetCamera();

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

        static s_str sLuaComString;
        static const s_str CLASS_NAME;

    protected :

        /// Default constructor
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

        /// Destructor
        /** \note If GetSingleton() has been called at least once,
        *         you'll have to delete the returned pointer when
        *         you close your program by calling Delete().
        */
        ~Engine();

    private :

        s_bool ReadConfig_();
        s_bool LoadShaders_();

        s_bool bShutDown_;

        s_uint uiScreenWidth_;
        s_uint uiScreenHeight_;

        // Game state
        s_int  iGameState_;
        s_bool bGamePaused_;

        // Game constants
        s_str sGameVersion_;
        s_int iMaxComputedPaths_;

        // Frame function
        Function pFrameFunc_;

        // Managers
        s_ptr<UtilsManager>    pUtilsMgr_;
        s_ptr<EventManager>    pEventMgr_;
        s_ptr<TimeManager>     pTimeMgr_;
        s_ptr<InputManager>    pInputMgr_;
        s_ptr<FontManager>     pFontMgr_;
        s_ptr<GUIManager>      pGUIMgr_;
        s_ptr<LocaleManager>   pLocaleMgr_;
        s_ptr<SpriteManager>   pSpriteMgr_;
        s_ptr<ModelManager>    pModelMgr_;
        s_ptr<CameraManager>   pCameraMgr_;
        s_ptr<PathManager>     pPathMgr_;
        s_ptr<SceneManager>    pSceneMgr_;
        s_ptr<MaterialManager> pMaterialMgr_;
        s_ptr<LightManager>    pLightMgr_;
        s_ptr<UnitManager>     pUnitMgr_;

        // Base camera
        s_ptr<Camera> pCamera_;

        // Lua
        lua_State* pLua_;

        // OGRE variables
        s_refptr<Ogre::Root>      pRoot_;
        s_ptr<Ogre::SceneManager> pOgreSceneMgr_;
        s_ptr<Ogre::RenderWindow> pRenderWindow_;
        s_ptr<Ogre::Log>          pLog_;
    };
}

#endif
