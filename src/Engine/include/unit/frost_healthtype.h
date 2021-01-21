/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*            HealthType header           */
/*                                        */
/*                                        */


#ifndef FROST_HEALTHTYPE_H
#define FROST_HEALTHTYPE_H

#include "frost_prereqs.h"

#include <frost_utils_timers.h>

namespace Frost
{
    enum HealthRegenType
    {
        HEALTH_REGEN_NONE,
        HEALTH_REGEN_TICK,
        HEALTH_REGEN_CONTINUOUS
    };

    /// Holds generic informations about a specific health type.
    struct HealthType
    {
        s_str           sName;
        HealthRegenType mRegenType;
        s_float         fRegenTickDuration;
        s_float         fRegenRate;

        s_ctnr<s_str>   lDefinedScriptList;
    };

    /// Handles a Unit's health.
    /** This class manages a Unit's health regeneration.
    */
    class HealthTypeInstance
    {
    public :

        HealthTypeInstance(s_ptr<Unit> pUnit, const s_str& sHealthName);

        /// Returns this HealthType's name.
        /** \return This HealthType's name
        */
        const s_str& GetHealthTypeName() const;

        /// Calls a script.
        /** \param sScriptName The name of the script
        *   \param pEvent      Stores scripts arguments
        */
        void         On(const s_str& sScriptName, s_ptr<Event> pEvent = nullptr);

        /// Updates this HealthType's attributes.
        /** \param fDelta The time elapsed since the last call
        *   \note Automatically called by Unit.
        */
        void         Update(const s_float& fDelta);

    private :

        s_ptr<const HealthType> pParent_;
        s_ptr<Unit>             pUnit_;
        s_refptr<PeriodicTimer> pTimer_;

    };
}

#endif
