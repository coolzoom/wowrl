/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*            DirectPath header           */
/*                                        */
/*                                        */


#ifndef FROST_PATH_DIRECTPATH_H
#define FROST_PATH_DIRECTPATH_H

#include "frost.h"
#include "path/frost_path.h"

namespace Frost
{
    /// A linearly interpolated path
    /** The advantages of this path type is that it doesn't
    *   require heavy calculations (simple LERP).
    *   The only problem is that it looks rather ugly : the
    *   movement is always done in straight lines. If you
    *   need a smoother movement, go for SmoothPath.
    */
    class DirectPath : public Path
    {
    public :

        /// An element of DirectPath
        struct Point
        {
            Point() {}

            Point(const s_float& fX, const s_float& fY, const s_float& fZ) :
                mPosition(fX, fY, fZ)
            {}

            Vector mPosition;
            s_uint uiID;
        };

        /// Calculates movement on a DirectPath.
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

            /// Changes the way to follow the path
            /** \note By default, the path is followed from its beginning to its end.
            *         You can change this behavior at anytime with this function.<br>
            *         Automatically called if you call SetSpeed() with a negative speed.
            */
            void         Reverse();

            /// Updates movement on the Path.
            /** \param fDelta The time elapsed since the last call
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

            void Increment_(s_ctnr<Point>::const_iterator& iterPrev, s_ctnr<Point>::const_iterator& iterNext);

            s_refptr<DirectPath>          pPath_;
            s_ctnr<Point>::const_iterator mPreviousPoint_;
            s_ctnr<Point>::const_iterator mNextPoint_;

            s_bool bFirstPlay_;
        };

        friend class Iterator;

        /// Default constructor.
        DirectPath();

        /// Copy constructor.
        /** \param mPath The path to copy
        */
        DirectPath(const DirectPath& mPath);

        /// Destructor.
        ~DirectPath();

        /// Adds a point at the back of the point list.
        /** \param mPoint The new point to add
        *   \note The linear interpolation requires a minimum of 2
        *         points. Using only 1 point is allowed but no animation
        *         is produced and the path always returns the same point.
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

        s_ctnr<Point> lPointPath_;
    };
}

#endif
