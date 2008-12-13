/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*               XXX header               */
/*                                        */
/*                                        */


#ifndef FROST_EVENT_H
#define FROST_EVENT_H

#include "frost.h"

namespace Frost
{
    /// Contains an event informations
    class Event
    {
    public :

        /// Constructor.
        /** \param sName         The name of this event
        *   \param bOncePerFrame 'true' if you allow several events of
        *                        this type to be fired during the same
        *                        frame
        */
        Event(const s_str& sName, const s_bool& bOncePerFrame = false);

        /// Adds a parameter to this event.
        /** \param mValue The value
        */
        void Add(const s_var& mValue);

        /// Returns a parameter of this event.
        /** \return A parameter of this event
        */
        s_ptr<const s_var> Get(const s_uint& uiIndex) const;

        /// Returns the number of parameter.
        /** \return The number of parameter
        */
        s_uint GetNumParam() const;

        /// Returns the name of this event.
        /** \return The name of this event
        */
        const s_str& GetName() const;

        /// Checks if this event should only be fired once per frame.
        /** \return 'true' if this should only be fired once per frame
        */
        const s_bool& IsOncePerFrame() const;

        static const s_str CLASS_NAME;

    private :

        s_str              sName_;
        s_bool             bOncePerFrame_;
        std::vector<s_var> lArgList_;
    };
}

#endif
