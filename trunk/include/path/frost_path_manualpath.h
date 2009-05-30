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
    /// An element of ManualPath
    struct ManualPathPoint
    {
        ManualPathPoint() {}

        ManualPathPoint(const s_float& fX, const s_float& fY, const s_float& fZ) :
            mPosition(fX, fY, fZ)
        {}

        Vector mPosition;
    };

    /// A smoothed path with custom tangents [!NYI!]
    /** Not Yet Implemented...
    */
    class ManualPath : public Path
    {
    public :

        /// Default constructor.
        /** \param uiID The unique ID to give to that path
        *   \note You shouldn't have to call this. Use the
        *         PathManager instead.
        */
        ManualPath(const s_uint& uiID);

        /// Copy constructor.
        /** \param uiID  The unique ID to give to that path
        *   \param mPath The path to copy
        *   \note You shouldn't have to call this. Use the
        *         PathManager instead.
        */
        ManualPath(const s_uint& uiID, const ManualPath& mPath);

        /// Destructor
        ~ManualPath();

        /// Updates this path.
        /** \param fDelta The time elapsed since the last call
        */
        virtual void Update(const s_float& fDelta);

        static const s_str CLASS_NAME;

    private :


    };
}

#endif
