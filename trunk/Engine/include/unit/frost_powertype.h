/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*            PowerType header            */
/*                                        */
/*                                        */


#ifndef FROST_POWERTYPE_H
#define FROST_POWERTYPE_H

#include "frost.h"

namespace Frost
{
    enum PowerRegenType
    {
        POWER_REGEN_NONE,
        POWER_REGEN_TICK,
        POWER_REGEN_CONTINUOUS
    };

    /// Holds generic informations about a specific power type.
    struct PowerType
    {
        s_str          sName;
        PowerRegenType mRegenType;
        s_float        fRegenTickDuration;
        s_float        fRegenRate;

        s_ctnr<s_str>  lDefinedScriptList;
    };

    /// Handles a Unit's power.
    /** This class manages a Unit's power regeneration.
    */
    class PowerTypeInstance
    {
    public :

        PowerTypeInstance(s_ptr<Unit> pUnit, const s_str& sPowerName);

        /// Returns this PowerType's name.
        /** \return This PowerType's name
        */
        const s_str& GetPowerTypeName() const;

        /// Calls a script.
        /** \param sScriptName The name of the script
        *   \param pEvent      Stores scripts arguments
        */
        void         On(const s_str& sScriptName, s_ptr<Event> pEvent = nullptr);

        /// Updates this PowerType's attributes.
        /** \param fDelta The time elapsed since the last call
        *   \note Automatically called by Unit.
        */
        void         Update(const s_float& fDelta);

    private :

        s_ptr<const PowerType>  pParent_;
        s_ptr<Unit>             pUnit_;
        s_refptr<PeriodicTimer> pTimer_;

    };
}

#endif
