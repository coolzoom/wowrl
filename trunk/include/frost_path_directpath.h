/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*            DirectPath header           */
/*                                        */
/*                                        */


#ifndef FROST_PATH_DIRECTPATH_H
#define FROST_PATH_DIRECTPATH_H

#include "frost.h"
#include "frost_path.h"

namespace Frost
{
    /// An element of DirectPath
    struct DirectPathPoint
    {
        DirectPathPoint() {}

        DirectPathPoint(const s_float& fX, const s_float& fY, const s_float& fZ) :
            mPosition(fX, fY, fZ)
        {}

        Vector mPosition;
        s_uint uiID;
    };

    /// A linearly interpolated path
    /** Pros :
    *   - very fast calculation
    *   - every point is intersected
    *   - can change every property at anytime (except adding a point
    *     in a looping path)<br>
    *
    *   Cons :
    *   - bad looking
    *   - no customization except speed<br>
    *
    *   Minimum number of point : 1
    */
    class DirectPath : public Path
    {
    public :

        /// Default constructor.
        /** \param uiID The unique ID to give to that path
        *   \note You shouldn't have to call this. Use the
        *         PathManager instead.
        */
        DirectPath(const s_uint& uiID);

        /// Copy constructor.
        /** \param uiID  The unique ID to give to that path
        *   \param mPath The path to copy
        *   \note You shouldn't have to call this. Use the
        *         PathManager instead.
        */
        DirectPath(const s_uint& uiID, const DirectPath& mPath);

        /// Destructor
        ~DirectPath();

        /// Adds a point at the back of the point list.
        /** \param mPoint The new point to add
        *   \note If this path has been set to loop and Update() has
        *         already be called at least once, this function will
        *         have no effect.<br>
        *         The linear interpolation requires a minimum of 2
        *         points. Using only 1 point is allowed but no animation
        *         is produced and the path always returns the same point.
        */
        void                   AddPoint(const DirectPathPoint& mPoint);

        /// Returns a certain point in the path.
        /** \param iIndex The index of the point you're looking for
        *   \return The point you're searching for
        *   \note You can use negative indices.
        */
        const DirectPathPoint& GetPoint(const s_int& iIndex) const;

        /// Returns a certain point iterator in the path.
        /** \param iIndex The index of the point you're looking for
        *   \return The iterator you're searching for
        *   \note You can use negative indices.
        */
        std::list<DirectPathPoint>::iterator GetPointIterator(const s_int& iIndex);

        /// Returns the number of points this path contains.
        /** \return The number of points this path contains
        */
        s_uint                 GetPointNumber() const;

        /// Updates this path.
        /** \param fDelta The time elapsed since the last call
        */
        virtual void           Update(const s_float& fDelta);

        /// Allows the path to update itself.
        /** \note If the path is already been played, this function
        *         restarts it to its beginning.
        */
        virtual void           Play();

        static const s_str CLASS_NAME;

    private :

        void CheckPath_();

        s_bool bStarted_;

        std::list<DirectPathPoint>           lPointPath_;
        std::list<DirectPathPoint>::iterator mPreviousPoint_;
        std::list<DirectPathPoint>::iterator mNextPoint_;
    };
}

#endif
