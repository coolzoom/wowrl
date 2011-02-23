/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*            SmoothPath header           */
/*                                        */
/*                                        */


#ifndef FROST_PATH_SMOOTHPATH_H
#define FROST_PATH_SMOOTHPATH_H

#include "frost.h"
#include "path/frost_path.h"

namespace Frost
{
    /// A smoothed path
    /** Uses Catmull-Rom method.<br>
    *   The advantages of using this path type is that
    *   you get very smooth results (Catmull-Rom's
    *   method).
    *   But it requires a bit more calculations than
    *   DirectPath, and it is much less flexible :
    *   once you've called Iterator::Play(), you can't
    *   add any more points to the SmoothPath (actually
    *   you can, but it will have no effect).<br>
    *   Be careful though : if you haven't enabled
    *   looping, the first and the last points will
    *   never be reached : they'll only be used to
    *   calculate the movement direction.
    *
    * \note Requires at least 4 points
    */
    class SmoothPath : public Path
    {
    public :

        /// An element of SmoothPath
        struct Point
        {
            Point() {}

            Point(const s_float& fX, const s_float& fY, const s_float& fZ) :
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
        class Segment
        {
        public :

            /// Default constructor.
            Segment();

            /// Constructor.
            /** \note Generates the arc-length table.
            */
            Segment(s_wptr<SmoothPath> pPath, const s_int& iFirstID);

            /// Calculates the point coordinates from a distance.
            /** \param fDistance The distance from the beginning of this segment.
            *   \note Automatically uses the arc-length table.
            */
            Vector         CalcPoint(const s_float& fDistance);

            /// Returns one of the four points
            /** \param uiIndex The index of the point you're searching for
            *   \return The point you're searching for
            */
            const Point&   GetPoint(const s_uint& uiIndex) const;

            /// Returns the true length of this segment, once smoothed.
            /** \return The true length of this segment, once smoothed
            */
            const s_float& GetLength() const;

            static const s_str CLASS_NAME;

        private :

            s_float                 fLength_;
            s_map<s_float, s_float> lLengthTable_;

            s_array<s_ctnr<Point>::const_iterator,4> lPointList_;
        };

        /// Calculates movement on a SmoothPath.
        class Iterator : public Path::Iterator
        {
        public :

            /// Constructor.
            /** \note You shouldn't create these by yourself.
            *         Use Path::CreateIterator() instead.
            */
            Iterator();

            /// Destructor.
            ~Iterator();

            /// Allows the movement to update.
            void         Play();

            /// Updates movement on the path.
            /** \param fDelta The time elapsed since the last call
            *   \note This function must be overloaded.
            */
            void         Update(const s_float& fDelta);

            /// Sets the Path this Iterator is working on.
            /** \param pPath The Path to use
            *   \note Automatically called by Path::CreateIterator().
            */
            void         SetPath(s_refptr<Path> pPath);

            /// Returns the associated Path.
            /** \return The associated Path
            */
            s_wptr<Path> GetPath() const;

            static const s_str CLASS_NAME;

        private :

            s_refptr<SmoothPath>      pPath_;
            s_ctnr<Segment>           lSegmentList_;
            s_ctnr<Segment>::iterator mSegment_;

            s_bool  bFirstPlay_;
            s_float fDistance_;
        };

        friend class Iterator;

        /// Default constructor.
        /** \note You shouldn't have to call this. Use the
        *         PathManager instead.
        */
        SmoothPath();

        /// Copy constructor.
        /** \param mPath The path to copy
        *   \note You shouldn't have to call this. Use the
        *         PathManager instead.
        */
        SmoothPath(const SmoothPath& mPath);

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
        void                     AddPoint(const Point& mPoint);

        /// Returns a certain point in the path.
        /** \param iIndex The index of the point you're looking for
        *   \return The point you're searching for
        *   \note You can use negative indices.
        */
        const Point&             GetPoint(const s_int& iIndex) const;

        /// Returns a certain point iterator in the path.
        /** \param iIndex The index of the point you're looking for
        *   \return The iterator you're searching for
        *   \note You can use negative indices.
        */
        s_ctnr<Point>::const_iterator GetPointIterator(const s_int& iIndex) const;

        /// Returns the number of points this path contains.
        /** \return The number of points this path contains
        */
        s_uint                   GetPointNumber() const;

        /// Creates a new iterator to move along this path.
        /** \return The newly created iterator.
        */
        s_refptr<Path::Iterator> CreateIterator() const;

        static const s_str CLASS_NAME;

    private :

        s_ctnr<Point>   lPointPath_;

    };
}

#endif
