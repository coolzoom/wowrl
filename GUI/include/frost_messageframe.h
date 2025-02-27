/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*              GUI header                */
/*                                        */
/*                                        */


#ifndef FROST_GUI_MESSAGEFRAME_H
#define FROST_GUI_MESSAGEFRAME_H

#include <frost_utils.h>
#include "frost_frame.h"

namespace Frost
{
    namespace GUI
    {
        class MessageFrame : public Frame
        {
        public :

            /// Constructor.
            MessageFrame(s_ptr<GUIManager> pManager);

            /// Destructor.
            virtual ~MessageFrame();

            /// Copies an UIObject's parameters into this MessageFrame (inheritance).
            /** \param pObj The UIObject to copy
            */
            virtual void   CopyFrom(s_ptr<UIObject> pObj);

            /// Returns this widget's Lua glue.
            virtual void CreateGlue();

            /// Parses data from an XML::Block.
            /** \param pBlock The MessageFrame's XML::Block
            */
            virtual void ParseBlock(s_ptr<XML::Block> pBlock);

            static const s_str CLASS_NAME;

        protected :

        };

        /** \cond NOT_REMOVE_FROM_DOC
        */

        class LuaMessageFrame : public LuaFrame
        {
        public :

            LuaMessageFrame(lua_State* pLua);

            // Glues

            static const char className[];
            static const char* classList[];
            static Lunar<LuaMessageFrame>::RegType methods[];
            static const s_str CLASS_NAME;

        protected :

            s_ptr<MessageFrame> pMessageFrameParent_;

        };

        /** \endcond
        */
    }
}

#endif
