// Ogre 2d: a small wrapper for 2d Graphics Programming in Ogre.
/*
   Wrapper for 2d Graphics in the Ogre 3d engine.

   Coded by H. Hernán Moraldo from Moraldo Games
   www.hernan.moraldo.com.ar/pmenglish/field.php

   Thanks for the Cegui team as their rendering code in Ogre gave me
   fundamental insight on the management of hardware buffers in Ogre.

   --------------------

   Copyright (c) 2006 Horacio Hernan Moraldo

   This software is provided 'as-is', without any express or
   implied warranty. In no event will the authors be held liable
   for any damages arising from the use of this software.

   Permission is granted to anyone to use this software for any
   purpose, including commercial applications, and to alter it and
   redistribute it freely, subject to the following restrictions:

   1. The origin of this software must not be misrepresented; you
   must not claim that you wrote the original software. If you use
   this software in a product, an acknowledgment in the product
   documentation would be appreciated but is not required.

   2. Altered source versions must be plainly marked as such, and
   must not be misrepresented as being the original software.

   3. This notice may not be removed or altered from any source
   distribution.

   NOTE : This code has been modified to follow Frost Engine's
   naming rules, and to allow Materials instead of plain textures.
   It has also been edited to simplify sprite rendering.

*/


#ifndef FROST_SPRITEMANAGER_H
#define FROST_SPRITEMANAGER_H

#include "frost.h"
#include "frost_gfx.h"

#include <OgreRenderOperation.h>
#include <OgreHardwareVertexBuffer.h>
#include <OgreRenderQueueListener.h>

namespace Frost
{
    enum AxisType
    {
        AXIS_UP, /// Will put the origin (0, 0) at the bottom left corner.
        AXIS_DOWN /// Will put the origin (0, 0) at the upper left corner.
    };

    /// Sprite manager
    /** This class has been written by Horacio Hernan Moraldo
    *   (see the header for more details).<br>
    *   It is used to draw Sprites on the screen (mainly for
    *   the UI).
    */
    class SpriteManager : public Manager<SpriteManager>, public Ogre::RenderQueueListener
    {
    friend class Manager<SpriteManager>;
    public :

        /// Called by Ogre, for being a render queue listener
        virtual void renderQueueStarted(Ogre::uint8 uiQueueGroupId, const Ogre::String &sInvocation, bool &bSkip);
        /// Called by Ogre, for being a render queue listener
        virtual void renderQueueEnded(Ogre::uint8 uiQueueGroupId, const Ogre::String &sInvocation, bool &bRepeat);

        /// Initializes this manager.
        /** \param pSceneMgr The scene manager to use
        *   \param fWidth    The width of the rendering region
        *   \param fHeight   The height of the rendering region
        *   \note Automatically called by Engine.
        */
        void         Initialize(s_ptr<Ogre::SceneManager> pSceneMgr, const s_float& fWidth, const s_float& fHeigth);

        /// Renders a single quad on the screen.
        /** \param mQuad A reference to a Quad
        *   \note Must be called between Begin() and End().
        */
        void         RenderQuad(const Quad &mQuad);

        /// Tells the manager to render into a specific render target.
        /** \param pRenderTarget The render target to render in
        *   \note Must be called before any rendering.<br>
        *         If no render target is provided, quads are rendered
        *         into the main render target, which is then rendered
        *         on the screen.
        */
        void         Begin(s_ptr<RenderTarget> pRenderTarget = NULL);

        /// Ends rendering into the render target.
        /** \note Must be called after Begin().<br>
        *         The real render is done when this function is called.
        */
        void         End();

        /// Fills the actual render target with the provided color.
        /** \param mColor The color to use
        *   \note Must be called between Begin() and End().
        */
        void         Clear(const Color& mColor);

        /// Calls the render function.
        /** \note Automatically called by Engine.
        */
        s_bool       RenderTargets();

        typedef s_bool (*Function)();
        /// Sets the function to call for rendering.
        /** \param pRenderFunc A pointer to the function.
        */
        void         SetRenderFunction(Function pRenderFunc);

        /// Sets the windows's size.
        /** \param fWidth  The new width
        *   \param fHeight The new height
        *   \note You should only call this if your Ogre window is resized.
        */
        void         SetWindowSize(const s_float& fWidth, const s_float& fHeight);

        /// Sets the Y axis type for rendering.
        /** \param mAxis The new axis to use
        *   \note AXIS_UP will put the origin (0, 0) at the bottom left corner.<br>
        *         AXIS_DOWN will put the origine (0, 0) at the upper left corner.<br>
        *         Coordinates will always be positive if chosen on the screen (you can
        *         still go outside the screen with negative/too large coordinates).
        */
        void         SetYAxisType(const AxisType& mAxis);

        /// Returns the axis type used for rendering.
        /** \return The axis type used for rendering
        *   \note For more info, see SetYAxisType().
        */
        const AxisType& GetYAxisType() const;

        static const s_str CLASS_NAME;

    protected :

        /// Default constructor
        /** \note This is a singleton class. That means that there can
        *         only be a single instance at the same time. That's why
        *         the constructor is not in public scope.<br>
        *         If you need to get a pointer to that unique instance,
        *         call SpriteManager::GetSingleton().<br>
        *         Note that, if GetSingleton() has been called at least once,
        *         you'll have to delete the returned pointer when you close
        *         your program by calling SpriteManager::Delete() (this is
        *         automatically done by Engine).
        */
        SpriteManager();

        /// Destructor
        /** \note If GetSingleton() has been called at least once,
        *         you'll have to delete the returned pointer when
        *         you close your program by calling Delete() (this is
        *         automatically done by Engine).
        */
        ~SpriteManager();

    private :

        /// Renders all the 2D data stored in the hardware buffers.
        /** \param bCallBeginEnd 'true' if you want to call DirectX's
        *                        Begin() before rendering and End() after.
        */
        void RenderBuffers_(s_bool bCallBeginEnd = false);

        /// Creates hardware buffers.
        /** \param size Vertex count for the new hardware buffer.
        */
        void CreateHardwareBuffers_(uint uiSize);

        /// Destroys hardware buffers.
        void DestroyHardwareBuffers_();

        /// Prepares Ogre for 2D rendering.
        /** \param bGeneral 'true' if you want to set generic parameters,
        *                   'false' if you want to call render target's
        *                   specific ones.
        */
        void PrepareForRender_(s_bool bGeneral = true);

        /// Renders the main target on the screen.
        s_bool RenderMainTarget_();

        s_ptr<Ogre::SceneManager> pSceneMgr_;
        s_ptr<Ogre::RenderSystem> pRS_;

        // Ogre specifics
        Ogre::RenderOperation               mRenderOp_;
        Ogre::HardwareVertexBufferSharedPtr mHardwareBuffer_;
        Ogre::HardwareVertexBufferSharedPtr mColorBuffer_;

        // Sprite list
        std::vector<Quad> lQuadList_;

        // Render target if any
        RenderTarget     mRenderTarget_;
        RenderTarget     mMainTarget_;
        s_refptr<Sprite> pMainSprite_;

        s_float f2_ScreenWidth_;
        s_float f2_ScreenHeight_;

        s_float fXOffset_;
        s_float fYOffset_;

        s_bool bFrameStarted_;
        s_bool bRenderTargets_;

        AxisType mAxisType_;

        Function pRenderFunc_;
    };
}

#endif
