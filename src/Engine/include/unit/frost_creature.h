/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*            Creature header             */
/*                                        */
/*                                        */


#ifndef FROST_CREATURE_H
#define FROST_CREATURE_H

#include "frost_prereqs.h"
#include "unit/frost_movableunit.h"
#include <scene/frost_sceneobject.h>

namespace Frost
{
    /// Makes the link between Creature and Ogre::MovableObject.
    class CreatureOgreInterface : public OgreInterface
    {
    public :

        /// Sets the Creature linked to this interface.
        /** \param pCreature The Creature
        */
        void   SetCreature(s_ptr<Creature> pCreature);

        /// Callback to react to events.
        /** \param sEvent The name of the event
        */
        void   On(const s_str& sEvent);

    private :

        s_ptr<Creature> pCreature_;

    };

    /// A non playable character
    /** Can be the boss of an instance, a boar, or a bird...<br>
    *   It's not actually an non playable character, since the
    *   player can (temporarily) take control of some of them,
    *   through mind control for example.
    */
    class Creature : public MovableUnit
    {
    public :

        /// Constructor.
        /** \param uiID  This Creature's unique ID
        *   \param sName This Creature's name
        *   \note You shouldn't have to call this. Use the
        *         UnitManager instead.
        */
        Creature(const s_uint& uiID, const s_str& sName);

        /// Destructor.
        ~Creature();

        /// Sets this Creature's body model.
        /** \param sModelName The new body model
        */
        void             SetBodyModel(const s_str& sModelName);

        /// Updates this Creature's parameters.
        /** \param fDelta The time elapsed since the last call
        *   \note Automatically called by UnitManager.
        */
        virtual void     Update(const s_float& fDelta);

        /// Pushes this Creature on the provided Lua::State.
        /** \param pLua The Lua::State on which to push the glue
        */
        virtual void     PushOnLua(s_ptr<Lua::State> pLua) const;

        /// Creates the associated Lua glue.
        /** \param pLua The Lua::State on which to push the glue
        */
        virtual void     CreateGlue(s_ptr<Lua::State> pLua);

        /// Returns this object's type.
        /** \return This object's type
        *   \note Simply returns CLASS_NAME.
        */
        virtual const s_str& GetType() const;

        static const s_str CLASS_NAME;

    private :

        CreatureOgreInterface mOgreInterface_;

    };

    /** \cond NOT_REMOVE_FROM_DOC
    */

    class LuaCreature : public LuaMovableUnit
    {
    public :

        LuaCreature(lua_State* pLua);



        static const char className[];
        static const char* classList[];
        static Lunar<LuaCreature>::RegType methods[];
        static const s_str CLASS_NAME;

    private :

        s_ptr<Creature> pParentCreature_;
    };

    /** \endcond
    */
}

#endif
