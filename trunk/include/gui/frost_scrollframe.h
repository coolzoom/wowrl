/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*              GUI header                */
/*                                        */
/*                                        */


#ifndef FROST_GUI_SCROLLFRAME_H
#define FROST_GUI_SCROLLFRAME_H

#include "frost.h"
#include "gui/frost_frame.h"

namespace Frost
{
    namespace GUI
    {
        /// A frame with scrollable content
        /** This widget has a special child Frame, the scroll child.
        *   The scroll child is rendered on a RenderTarget, which is
        *   then rendered on the screen.
        */
        class ScrollFrame : public Frame
        {
        public :

            /// Constructor.
            ScrollFrame();

            /// Destructor.
            virtual ~ScrollFrame();

            /// Updates this widget's logic.
            virtual void   Update();

            /// Copies an UIObject's parameters into this ScrollFrame (inheritance).
            /** \param pObj The UIObject to copy
            */
            virtual void   CopyFrom(s_ptr<UIObject> pObj);

            /// Returns 'true' if this ScrollFrame can use a script.
            /** \param sScriptName The name of the script
            *   \note This method can be overriden if needed.
            */
            virtual s_bool CanUseScript(const s_str& sScriptName) const;

            /// Calls a script.
            /** \param sScriptName The name of the script
            *   \param pEvent      Stores scripts arguments
            */
            virtual void   On(const s_str& sScriptName, s_ptr<Event> pEvent = nullptr);

            /// Sets this ScrollFrame's scroll child.
            /** \param pFrame The scroll child
            *   \note Creates the RenderTarget and the associated Sprite.
            */
            void           SetScrollChild(s_ptr<Frame> pFrame);

            /// Returns this ScrollFrame's scroll child.
            /** \return This ScrollFrame's scroll child
            */
            s_ptr<Frame>   GetScrollChild();

            /// Sets the horizontal offset of the scroll child.
            /** \param iHorizontalScroll The horizontal offset
            */
            void           SetHorizontalScroll(const s_int& iHorizontalScroll);

            /// Returns the horizontal offset of the scroll child.
            /** \return The horizontal offset of the scroll child
            */
            const s_int&   GetHorizontalScroll() const;

            /// Returns the maximum horizontal offset of the scroll child.
            /** \return The maximum horizontal offset of the scroll child
            */
            const s_int&   GetHorizontalScrollRange() const;

            /// Sets the vertical offset of the scroll child.
            /** \param iVerticalScroll The vertical offset
            */
            void           SetVerticalScroll(const s_int& iVerticalScroll);

            /// Returns the vertical offset of the scroll child.
            /** \return The vertical offset of the scroll child
            */
            const s_int&   GetVerticalScroll() const;

            /// Returns the maximum vertical offset of the scroll child.
            /** \return The maximum vertical offset of the scroll child
            */
            const s_int&   GetVerticalScrollRange() const;

            /// Checks if the provided coordinates are in the ScrollFrame.
            /** \param iX           The horizontal coordinate
            *   \param iY           The vertical coordinate
            *   \return 'true' if the provided coordinates are in the ScrollFrame
            *   \note The ScrollFrame version of this function also checks if the
            *         mouse is over the scroll texture (which means this function
            *         ignores positive hit rect insets).
            */
            virtual s_bool IsInFrame(const s_int& iX, const s_int& iY) const;

            /// Tells this ScrollFrame it is being overed by the mouse.
            /** \param bMouseInFrame 'true' if the mouse is above this ScrollFrame
            *   \param iX            The horizontal mouse coordinate
            *   \param iY            The vertical mouse coordinate
            */
            virtual void   NotifyMouseInFrame(const s_bool& bMouseInFrame, const s_int& iX, const s_int& iY);

            /// Tells this widget that a manually rendered widget requires redraw.
            virtual void   FireRedraw();

            /// Tells this ScrollFrame that at least one of its children has modified its strata or level.
            /** \param pChild The child that has changed its strata (can also be a child of this child)
            *   \note If pChild is the scroll child, it only rebuilds its internal strata list.
            *   \note If this ScrollFrame has no parent, it calls GUIManager::FireBuildStrataList(). Else it
            *         notifies its parent.
            */
            virtual void   NotifyChildStrataChanged(s_ptr<Frame> pChild);

            /// Returns this widget's Lua glue.
            virtual void   CreateGlue();

            /// Parses data from an XML::Block.
            /** \param pBlock The ScrollFrame's XML::Block
            */
            virtual void   ParseBlock(s_ptr<XML::Block> pBlock);

            static const s_str CLASS_NAME;

        protected :

            virtual void ParseScrollChildBlock_(s_ptr<XML::Block> pBlock);

            virtual void NotifyManuallyRenderedObject_(s_ptr<UIObject> pObject, const s_bool& bManuallyRendered);

            void AddToScrollChildList_(s_ptr<Frame> pChild);
            void RemoveFromScrollChildList_(s_ptr<Frame> pChild);

            void UpdateScrollRange_();
            void UpdateScrollChildInput_();
            void RebuildScrollRenderTarget_();
            void RebuildScrollStrataList_();
            void RenderScrollStrataList_();

            s_int iHorizontalScroll_;
            s_int iHorizontalScrollRange_;
            s_int iVerticalScroll_;
            s_int iVerticalScrollRange_;

            s_ptr<Frame>        pScrollChild_;

            s_bool              bRebuildScrollRenderTarget_;
            s_bool              bRedrawScrollRenderTarget_;
            s_bool              bUpdateScrollRange_;
            s_ptr<RenderTarget> pScrollRenderTarget_;

            s_ptr<Texture>      pScrollTexture_;

            s_bool                             bRebuildScrollStrataList_;
            s_map< s_uint, s_ptr<GUI::Frame> > lScrollChildList_;
            s_map<FrameStrata, Strata>         lScrollStrataList_;

            s_bool       bMouseInScrollTexture_;
            s_ptr<Frame> pOveredScrollChild_;

        };

        /** \cond NOT_REMOVE_FROM_DOC
        */

        class LuaScrollFrame : public LuaFrame
        {
        public :

            LuaScrollFrame(lua_State* pLua);

            // Glues
            int _GetHorizontalScroll(lua_State*);
            int _GetHorizontalScrollRange(lua_State*);
            int _GetScrollChild(lua_State*);
            int _GetVerticalScroll(lua_State*);
            int _GetVerticalScrollRange(lua_State*);
            int _SetHorizontalScroll(lua_State*);
            int _SetScrollChild(lua_State*);
            int _SetVerticalScroll(lua_State*);

            static const char className[];
            static const char* classList[];
            static Lunar<LuaScrollFrame>::RegType methods[];
            static const s_str CLASS_NAME;

        protected :

            s_ptr<ScrollFrame> pScrollFrameParent_;

        };

        /** \endcond
        */
    }
}

#endif
