/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*            ManualPath header           */
/*                                        */
/*                                        */


#ifndef FROST_PATH_MANUALPATH_H
#define FROST_PATH_MANUALPATH_H

#include "frost.h"
#include "path/frost_path.h"

namespace Frost
{
    /// A smoothed path with custom tangents
    /** Not Yet Implemented...
    */
    class ManualPath : public Path
    {
    public :

        /// An element of ManualPath
        struct Point
        {
            Point() {}

            Point(const s_float& fX, const s_float& fY, const s_float& fZ) :
                mPosition(fX, fY, fZ)
            {}

            Vector mPosition;
        };

        /// Default constructor.
        /** \note You shouldn't have to call this. Use the
        *         PathManager instead.
        */
        ManualPath();

        /// Copy constructor.
        /** \param mPath The path to copy
        *   \note You shouldn't have to call this. Use the
        *         PathManager instead.
        */
        ManualPath(const ManualPath& mPath);

        /// Destructor.
        ~ManualPath();

        static const s_str CLASS_NAME;

    private :


    };
}

#endif
