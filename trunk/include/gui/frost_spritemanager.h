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

    /// A wrapper around Ogre::RenderTarget
    class RenderTarget
    {
    public :

        /// Constructor.
        /** \param uiID     The unique ID to give to that RenderTarget
        *   \param uiWidth  The width of the RenderTarget
        *   \param uiHeight The height of the RenderTarget
        *   \note You shouldn't have to call this. Use the
        *         SpriteManager instead.
        */
        RenderTarget(const s_uint& uiID, const s_uint& uiWidth, const s_uint& uiHeight);

        /// Constructor.
        /** \param uiID     The unique ID to give to that RenderTarget
        *   \param sName    The name to give to this RenderTarget
        *   \param uiWidth  The width of the RenderTarget
        *   \param uiHeight The height of the RenderTarget
        *   \note You shouldn't have to call this. Use the
        *         SpriteManager instead.
        */
        RenderTarget(const s_uint& uiID, const s_str& sName, const s_uint& uiWidth, const s_uint& uiHeight);

        /// Destructor.
        ~RenderTarget();

        /// Returns this render target's width.
        /** \return This render target's width
        */
        const s_uint&             GetWidth() const;

        /// Returns this render target's height.
        /** \return This render target's height
        */
        const s_uint&             GetHeight() const;

        /// Sets this render target's dimensions.
        /** \param uiWidth This render target's width
        *   \param uiHeight This render target's height
        *   \return 'true' if the function had to re-create a
        *           new render target
        */
        s_bool                    SetDimensions(const s_uint& uiWidth, const s_uint& uiHeight);

        /// Returns this render target's real width.
        /** \return This render target's real width
        *   \note This is the physical size of the render target
        *         (the nearest power of two from GetWidth()).
        */
        const s_uint&             GetRealWidth() const;

        /// Returns this render target's real height.
        /** \return This render target's real height
        *   \note This is the physical size of the render target
        *         (the nearest power of two from GetHeight()).
        */
        const s_uint&             GetRealHeight() const;

        /// Returns the associated Ogre::RenderTarget.
        /** \return The associated Ogre::RenderTarget
        */
        s_ptr<Ogre::RenderTarget> GetOgreRenderTarget();

        /// Returns this RenderTarget's ID.
        /** \return This RenderTarget's ID
        */
        const s_uint&             GetID();

        /// Returns this RenderTarget's name.
        /** \return This RenderTarget's name
        */
        const s_str&              GetName();

        static const s_str CLASS_NAME;

    private :

        s_uint                    uiID_;
        s_str                     sName_;
        s_ptr<Ogre::RenderTarget> pOgreRenderTarget_;
        ulong                     hResourceHandle_;
        s_uint                    uiRealWidth_;
        s_uint                    uiRealHeight_;
        s_uint                    uiWidth_;
        s_uint                    uiHeight_;

    };

    /// A point in the 2D environment
    /** This structure contains several informations :
    *   - the point's coordinate on the screen
    *   - the point's coordiante on the texture (if any)
    *   - the color
    */
    struct Vertex
    {
        s_float fX, fY;
        s_float fTX, fTY;
        Color   mColor;

        Vertex(const s_float& x, const s_float& y, const s_float& u, const s_float& v, const Color& color)  :
            fX(x), fY(y), fTX(u), fTY(v), mColor(color)
        {
        }

        Vertex() : mColor(255, 255, 255)
        {

        }

        void Set(const s_float& x, const s_float& y)
        {
            fX = x.Get(); fY = y.Get();
        }

        void SetUV(const s_float& u, const s_float& v)
        {
            fTX = u.Get(); fTY = v.Get();
        }
    };

    /// An array of four vertices
    /** This is the base of UI rendering.<br>
    *   It contains the four vertices that define the shape of
    *   the thing you want to draw, and the Material that should
    *   be used to draw that shape.
    */
    struct Quad
    {
        Vertex             lVertexArray[4];
        s_refptr<Material> pMat;

        Quad();
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
        void         Initialize(s_ptr<Ogre::SceneManager> pSceneMgr, const s_float& fWidth, const s_float& fHeight);

        /// Renders a single quad on the screen.
        /** \param mQuad A reference to a Quad
        *   \note Must be called between Begin() and End().
        */
        void         RenderQuad(const Quad& mQuad);

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

        /// Creates a new RenderTarget.
        /** \param sTargetName The name of this render target
        *   \param uiWidth     The width of this render target
        *   \param uiHeight    The height of this render target
        *   \return The new RenderTarget
        *   \note For compatibility issues, the created render target will be
        *         enlarged to have a power of two for both height and width. This won't
        *         affect the final render.<br>
        *         If you call Ogre::RenderTarget::getWidth(), you'll get the power of two
        *         just above the value you've given to that function.<br>
        *         To get the "rendered" width, use Frost::RenderTarget::uiWidth.<br>
        *         Same goes for the height.
        */
        s_ptr<RenderTarget> CreateRenderTarget(const s_str& sTargetName, const s_uint& uiWidth, const s_uint& uiHeight);

        /// Creates a new RenderTarget.
        /** \param uiWidth     The width of this render target
        *   \param uiHeight    The height of this render target
        *   \return The new RenderTarget
        *   \note For compatibility issues, the created render target will be
        *         enlarged to have a power of two for both height and width. This won't
        *         affect the final render.<br>
        *         If you call Ogre::RenderTarget::getWidth(), you'll get the power of two
        *         just above the value you've given to that function.<br>
        *         To get the "rendered" width, use Frost::RenderTarget::uiWidth.<br>
        *         Same goes for the height.
        */
        s_ptr<RenderTarget> CreateRenderTarget(const s_uint& uiWidth, const s_uint& uiHeight);

        /// Deletes a RenderTarget.
        /** \param mTarget The RenderTarget you want to delete
        *   \note This function is more secure than the other one.
        */
        void DeleteRenderTarget(s_ptr<RenderTarget> mTarget);

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

        /// Copy constructor
        SpriteManager(const SpriteManager& mMgr);

        /// Assignment operator
        SpriteManager& operator = (const SpriteManager& mMgr);

    private :

        /// Renders all the 2D data stored in the hardware buffers.
        /** \param bCallBeginEnd 'true' if you want to call DirectX's
        *                        Begin() before rendering and End() after.
        */
        void RenderBuffers_(s_bool bCallBeginEnd = false);

        /// Creates hardware buffers.
        /** \param uiSize Vertex count for the new hardware buffer.
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
        s_ptr<RenderTarget> pRenderTarget_;
        s_ptr<RenderTarget> pMainTarget_;
        std::map< s_uint, s_ptr<RenderTarget> > lRenderTargetList_;
        s_uint              uiTargetCounter_;
        s_refptr<Sprite>    pMainSprite_;

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
