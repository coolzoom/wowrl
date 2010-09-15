/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*              GUI header                */
/*                                        */
/*                                        */


#ifndef FROST_GUI_CHECKBUTTON_H
#define FROST_GUI_CHECKBUTTON_H

#include <frost_utils.h>
#include "frost_button.h"

namespace Frost
{
    namespace GUI
    {
        /// A button with two additional states : checked and unchecked
        /** This widget works exactly like a classic Button, but is has two
        *   additional special textures for the check sign.
        */
        class CheckButton : public Button
        {
        public :

            /// Constructor.
            CheckButton(s_ptr<GUIManager> pManager);

            /// Destructor.
            virtual ~CheckButton();

            /// Prints all relevant information about this widget in a string.
            /** \param sTab The offset to give to all lines
            *   \return All relevant information about this widget
            */
            virtual s_str  Serialize(const s_str& sTab) const;

            /// Copies an UIObject's parameters into this CheckButton (inheritance).
            /** \param pObj The UIObject to copy
            */
            virtual void   CopyFrom(s_ptr<UIObject> pObj);

            /// Checks this button.
            virtual void   Check();

            /// UnChecks this button.
            virtual void   UnCheck();

            /// Disables this CheckButton.
            /** \note A disabled button doesn't receive any input.
            */
            virtual void   Disable();

            /// Enables this CheckButton.
            virtual void   Enable();

            /// Releases this CheckButton.
            /** \note This function only has a visual impact :
            *         the OnClick() handler is not called.
            */
            virtual void   Release();

            /// Check if this CheckButton is checked :)
            /** \return ... well you get my point.
            */
            const s_bool&  IsChecked();

            /// Returns this Button's checked texture.
            /** \return This Button's checked texture
            */
            s_ptr<Texture> GetCheckedTexture();

            /// Returns this Button's disabled checked texture.
            /** \return This Button's disabled checked texture
            */
            s_ptr<Texture> GetDisabledCheckedTexture();

            /// Sets this Button's checked texture.
            /** \param pTexture The new texture
            */
            void           SetCheckedTexture(s_ptr<Texture> pTexture);

            /// Sets this Button's disabled checked texture.
            /** \param pTexture The new texture
            */
            void           SetDisabledCheckedTexture(s_ptr<Texture> pTexture);

            /// Returns this widget's Lua glue.
            virtual void   CreateGlue();

            /// Parses data from an XML::Block.
            /** \param pBlock The CheckButton's XML::Block
            */
            virtual void   ParseBlock(s_ptr<XML::Block> pBlock);

            static const s_str CLASS_NAME;

        protected :

            s_ptr<Texture>    CreateCheckedTexture_();
            s_ptr<Texture>    CreateDisabledCheckedTexture_();

            s_bool bChecked_;

            s_ptr<Texture> pCheckedTexture_;
            s_ptr<Texture> pDisabledCheckedTexture_;

        };

        /** \cond NOT_REMOVE_FROM_DOC
        */

        class LuaCheckButton : public LuaButton
        {
        public :

            LuaCheckButton(lua_State* pLua);

            // Glues
            int _IsChecked(lua_State*);
            int _GetCheckedTexture(lua_State*);
            int _GetDisabledCheckedTexture(lua_State*);
            int _SetChecked(lua_State*);
            int _SetCheckedTexture(lua_State*);
            int _SetDisabledCheckedTexture(lua_State*);

            static const char className[];
            static const char* classList[];
            static Lunar<LuaCheckButton>::RegType methods[];
            static const s_str CLASS_NAME;

        protected :

            s_ptr<CheckButton> pCheckButtonParent_;

        };

        /** \endcond
        */
    }
}

#endif
