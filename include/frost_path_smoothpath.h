/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*            SmoothPath header           */
/*                                        */
/*                                        */


#ifndef FROST_PATH_SMOOTHPATH_H
#define FROST_PATH_SMOOTHPATH_H

#include "frost.h"
#include "frost_path.h"

namespace Frost
{
    /// An element of SmoothPath
    struct SmoothPathPoint
    {
        SmoothPathPoint() {}

        SmoothPathPoint(const s_float& fX, const s_float& fY, const s_float& fZ) :
            mPosition(fX, fY, fZ)
        {}

        Vector mPosition;
        s_uint uiID;
    };

    /// A group of 4 points in a SmoothPath
    /** Automatically generates an arc-length table,
    *   which is used to keep a constant speed while
    *   following the path.
    */
    class SmoothPathSegment
    {
    public :

        /// Default constructor.
        SmoothPathSegment() {}

        /// Constructor.
        /** \note Generates the arc-length table.
        */
        SmoothPathSegment(s_ptr<SmoothPath> pPath, const s_int& iFirstID);

        /// Calculates the point coordinates from a distance.
        /** \param fDistance The distance from the beginning of this
        *                    segment.
        *   \note Automatically uses the arc-length table.
        */
        Vector                 CalcPoint(const s_float& fDistance);

        /// Returns one of the four points
        /** \param uiIndex The index of the point you're searching for
        *   \return The point you're searching for
        */
        const SmoothPathPoint& GetPoint(const s_uint& uiIndex) const;

        /// Returns the true length of this segment, once smoothed.
        /** \return The true length of this segment, once smoothed
        */
        const s_float&         GetLength() const;

    private :

        s_float fLength_;

        std::list<SmoothPathPoint>::iterator lP_[4];
        std::map<s_float, s_float>           lLengthTable_;
    };

    /// A smoothed path
    /** Uses Catmull-Rom method.<br><br>
    *   Pros :
    *      - every point is intersected
    *      - good looking curves<br>
    *
    *   Cons :
    *      - a little bit heavier than DirectPath
    *      - less customizable than ManualPath
    *      - looping must be set before the first update<br>
    *
    *   Minimum number of point : 4
    */
    class SmoothPath : public Path
    {
    public :

        /// Default constructor.
        /** \param uiID The unique ID to give to that path
        *   \note You shouldn't have to call this. Use the
        *         PathManager instead.
        */
        SmoothPath(const s_uint& uiID);

        /// Copy constructor.
        /** \param uiID  The unique ID to give to that path
        *   \param mPath The path to copy
        *   \note You shouldn't have to call this. Use the
        *         PathManager instead.
        */
        SmoothPath(const s_uint& uiID, const SmoothPath& mPath);

        /// Destructor.
        ~SmoothPath();

        /// Adds a point at the back of the point list.
        /** \param mPoint The new point to add
        *   \note If this path has been set to loop and Update() has
        *         already be called at least once, this function will
        *         have no effect.<br>
        *         The Catmull-Rom implementation requires a minimum of
        *         4 points.<br>
        *         If you don't loop your path, the first and the last
        *         points are only control points : they won't ever be
        *         reached.<br>
        */
        void                   AddPoint(const SmoothPathPoint& mPoint);

        /// Returns a certain point in the path.
        /** \param iIndex The index of the point you're looking for
        *   \return The point you're searching for
        *   \note You can use negative indices.
        */
        const SmoothPathPoint& GetPoint(const s_int& iIndex) const;

        /// Returns a certain point iterator in the path.
        /** \param iIndex The index of the point you're looking for
        *   \return The iterator you're searching for
        *   \note You can use negative indices.
        */
        std::list<SmoothPathPoint>::iterator GetPointIterator(const s_int& iIndex);

        /// Returns the number of points this path contains.
        /** \return The number of points this path contains
        */
        s_uint                 GetPointNumber() const;

        /// Sets whether this path should be followed forever or just once.
        /** \param bLoop           'true' if you want the path to be followed forever
        *   \param bLinkEndToStart 'true' if you want to link the last point of the
        *                          path with the first one
        *   \note Default is 'false'.<br>
        *         If Update() has already be called at least once, this function will
        *         have no effect.
        */
        virtual void           SetLoop(const s_bool& bLoop, const s_bool& bLinkEndToStart = true);

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

        s_bool  bStarted_;
        s_float fDistance_;

        std::list<SmoothPathPoint>             lPointPath_;
        std::list<SmoothPathSegment>           lSegmentList_;
        std::list<SmoothPathSegment>::iterator mSegment_;


    };
}

#endif
