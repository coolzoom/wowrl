/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*                                        */
/*                                        */

#ifndef FROST_UTILS_POINT_H
#define FROST_UTILS_POINT_H

#include "frost_utils_types.h"

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

        Point()
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

        T GetNorm() const
        {
            return T(sqrt(mX_*mX_ + mY_*mY_));
        }

        T GetNormSquared() const
        {
            return mX_*mX_ + mY_*mY_;
        }

        void Normalize()
        {
            T mNorm = T(sqrt(mX_*mX_ + mY_*mY_));
            mX_ = mX_/mNorm;
            mY_ = mY_/mNorm;
        }

        Point GetUnit() const
        {
            T mNorm = T(sqrt(mX_*mX_ + mY_*mY_));
            return Point(mX_/mNorm, mY_/mNorm);
        }

        void Rotate(const s_float& fAngle)
        {
            Point p;

            p.mX_ = mX_*T(cos(fAngle)) - mY_*T(sin(fAngle));
            p.mY_ = mY_*T(sin(fAngle)) + mY_*T(cos(fAngle));

            mX_ = p.mX_;
            mY_ = p.mY_;
        }

        Point GetRotation(const s_float& fAngle) const
        {
            return Point(mX_*T(cos(fAngle)) - mY_*T(sin(fAngle)), mY_*T(sin(fAngle)) + mY_*T(cos(fAngle)));
        }

        Point operator + (const Point& mPoint)  const
        {
            return Point(mX_ + mPoint.mX_, mY_ + mPoint.mY_);
        }
        void operator += (const Point& mPoint)
        {
            mX_ += mPoint.mX_; mY_ += mPoint.mY_;
        }

        Point operator - () const
        {
            return Point(-mX_, -mY_);
        }

        Point operator - (const Point& mPoint) const
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

        Point operator * (const T& mValue) const
        {
            return Point(mX_*mValue, mY_*mValue);
        }

        Point operator / (const T& mValue) const
        {
            return Point(mX_/mValue, mY_/mValue);
        }

        T operator * (const Point& mPoint) const
        {
            return mX_*mPoint.mX_ + mY_*mPoint.mY_;
        }

        const T& X() const
        {
            return mX_;
        }
        const T& Y() const
        {
            return mY_;
        }

        T& X()
        {
            return mX_;
        }
        T& Y()
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
    Point<typename TypeTraits<T>::FrostType> operator * (const T& mValue, const Point<typename TypeTraits<T>::FrostType>& mVec)
    {
        return Point<typename TypeTraits<T>::FrostType>(mVec.X()*mValue, mVec.Y()*mValue);
    }

    template<class T>
    Point<typename TypeTraits<T>::FrostType> operator / (const T& mValue, const Point<typename TypeTraits<T>::FrostType>& mVec)
    {
        return Point<typename TypeTraits<T>::FrostType>(mVec.X()/mValue, mVec.Y()/mValue);
    }

    template<class T>
    const s_str Point<T>::CLASS_NAME = "Point";

    typedef Point<s_float> Vector2D;

    /** \cond NOT_REMOVE_FROM_DOC
    */
    template<class T> class StringConverter< string_element, Point<T> >
    {
    public :

        typedef s_str::string string;

        static string Convert(const Point<T>& mVector)
        {
            return "("+StringConverter<string_element, T>::Convert(mVector.X())+
                  ", "+StringConverter<string_element, T>::Convert(mVector.Y())+")";
        }
    };
    /** \endcond
    */
}

#endif
