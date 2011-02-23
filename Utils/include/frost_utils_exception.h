/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*            Exception header            */
/*                                        */
/*                                        */

#ifndef FROST_EXCEPTION_H
#define FROST_EXCEPTION_H

#include "frost_utils_config.h"
#include "frost_utils_types.h"

namespace Frost
{
    /// Frost's exception class.
    class Exception
    {
    public :

        /// Default exception.
        /** \note Reports : "Undefined exception."
        */
        Exception();

        /// Simple message exception.
        /** \param sMessage The message to throw
        *   \note Reports : "<sMessage>"
        */
        Exception(const s_str& sMessage);

        /// Class name + message exception.
        /** \param sClassName The name of the class which throws the exception
        *   \param sMessage   The message to throw
        *   \note Reports : "<sClassName> : <sMessage>"
        */
        Exception(const s_str& sClassName, const s_str& sMessage);

        /// Destructor.
        virtual ~Exception();

        /// Returns the message of the exception.
        /** \return The message of the exception
        */
        const s_str& GetDescription() const;

    protected :

        s_str sMessage_;
    };
}

#endif
