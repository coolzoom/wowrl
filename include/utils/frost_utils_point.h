/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*                                        */
/*                                        */

#ifndef FROST_UTILS_POINT_H
#define FROST_UTILS_POINT_H

#include "frost_utils.h"

namespace Frost
{
    /// A simple 2d coordinate storage.
    /** This class is used to store a two dimensions
    *   set of coordinates (x, y). It provides a few
    *   basic operations (+, -, (in)equality).<br>
    *   You have to choose the coordinate value type.
    */
    template<class T>
    class Point
    {
    public :

        Point() : mX_(0), mY_(0)
        {
        }

        /** \param mX The horizontal coordinate
        *   \param mY The vertical coordinate
        */
        Point(const T& mX, const T& mY) : mX_(mX), mY_(mY)
        {
        }

        ~Point()
        {
        }

        /// Sets the point's coordinates.
        /** \param mX The new horizontal coordinate
        *   \param mY The new vertical coordinate
        */
        void Set(const T& mX, const T& mY)
        {
            mX_ = mX; mY_ = mY;
        }

        Point<T> operator + (const Point& mPoint)  const
        {
            return Point(mX_ + mPoint.mX_, mY_ + mPoint.mY_);
        }
        void operator += (const Point& mPoint)
        {
            mX_ += mPoint.mX_; mY_ += mPoint.mY_;
        }

        Point<T> operator - (const Point& mPoint) const
        {
            return Point(mX_ - mPoint.mX_, mY_ - mPoint.mY_);
        }
        void operator -= (const Point& mPoint)
        {
            mX_ -= mPoint.mX_; mY_ -= mPoint.mY_;
        }

        s_bool operator == (const Point& mPoint) const
        {
            return ( (mX_ == mPoint.mX_) && (mY_ == mPoint.mY_) );
        }
        s_bool operator != (const Point& mPoint) const
        {
            return !( (mX_ == mPoint.mX_) && (mY_ == mPoint.mY_) );
        }

        T X() const
        {
            return mX_;
        }
        T Y() const
        {
            return mY_;
        }

        static const s_str CLASS_NAME;

    private :

        /// The horizontal coordinate
        T mX_;
        /// The vertical coordinate
        T mY_;
    };

    template<class T>
    const s_str Point<T>::CLASS_NAME = "Point";
}

#endif
