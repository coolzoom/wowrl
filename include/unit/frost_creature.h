/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*            Creature header             */
/*                                        */
/*                                        */


#ifndef FROST_CREATURE_H
#define FROST_CREATURE_H

#include "frost.h"
#include "unit/frost_movableunit.h"

#include <OgreUserDefinedObject.h>

namespace Frost
{
    /// Makes the link between Creature and Ogre::MovableObject.
    class CreatureOgreInterface : public Ogre::UserDefinedObject
    {
    public :

        /// Constructor.
        CreatureOgreInterface(s_ptr<Creature> pModel);

        /// Inherited from UserDefinedObject.
        virtual const Ogre::String& getTypeName() const;

        /// Returns the associated Creature.
        /** \return The associated Creature
        */
        s_ptr<Creature> GetCreature() const;

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

        static const s_str CLASS_NAME;

    private :

        CreatureOgreInterface mInterface_;

    };

    /** \cond NOT_REMOVE_FROM_DOC
    */

    class LuaCreature : public LuaMovableUnit
    {
    public :

        LuaCreature(lua_State* pLua);



        static const char className[];
        static Lunar<LuaCreature>::RegType methods[];
        static const s_str CLASS_NAME;

    private :

        s_ptr<Creature> pParentCreature_;
    };

    /** \endcond
    */
}

#endif
