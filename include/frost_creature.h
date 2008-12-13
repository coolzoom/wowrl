/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*            Creature header             */
/*                                        */
/*                                        */


#ifndef FROST_CREATURE_H
#define FROST_CREATURE_H

#include "frost.h"
#include "frost_unit.h"

namespace Frost
{
    /// A non playable character
    /** Can be the boss of an instance, a boar, or a bird...<br>
    *   It's not actually an non playable character, since the
    *   player can (temporarily) take control of some of them,
    *   through mind control for example.
    */
    class Creature : public Unit
    {
    public :

        /// Constructor.
        /** \param uiID  This Unit's unique ID
        *   \param sName This Unit's name
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
        void Update(const s_float& fDelta);

        static const s_str CLASS_NAME;

    private :


    };
}

#endif
