/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*                                        */
/*                                        */

#ifndef FROST_UTILS_ARRAY_H
#define FROST_UTILS_ARRAY_H

#include "frost_utils.h"
#include "frost_utils_uint.h"

namespace Frost
{
    class s_uint;
    class s_str;

    /// Base type : container
    /** This class is designed to be used with s_array.<br>
    *   It's main feature is the overloading of the ','
    *   operator, which allows you to write :<br><br>
    *   s_array<s_int, 2> myArray((s_int(0), s_int(1)));<br><br>
    *   ... with any class that has overloaded its ',' operator.<br><br>
    *   <b>Don't forget the double parenthesis</b> : if you
    *   don't use them, myArray will initialize all its fields
    *   with the latest value.
    */
    template<class T>
    class s_ctnr
    {
    public :

        s_ctnr()
        {
        }

        s_ctnr(T mElem)
        {
            lArray_.push_back(mElem);
        }

        s_ctnr& operator, (const T& mElem)
        {
            lArray_.push_back(mElem);
            return (*this);
        }

        void Push(const T& mElem)
        {
            lArray_.push_back(mElem);
        }

        s_uint GetSize() const
        {
            return lArray_.size();
        }

        const T& operator[] (const s_uint& uiIndex) const
        {
            return lArray_[uiIndex.Get()];
        }

        static const s_str CLASS_NAME;

    private :

        std::vector<T> lArray_;
    };

    /// Base type : array
    /** This class is a limited version of std::vector.
    *   It has a fixed size, like standard C arrays, but
    *   it comes with a few features that make it simpler
    *   to use.
    */
    template<class T, uint N>
    class s_array
    {
    public :

        s_array()
        {
        }

        s_array(const T& mInitValue)
        {
            for (uint i = 0; i < N; i++)
            {
                lArray_[i] = mInitValue;
            }
        }

        s_array(const s_ctnr<T>& mContainer)
        {
            for (uint i = 0; i < N; i++)
            {
                if (i >= mContainer.GetSize().Get())
                    break;

                lArray_[i] = mContainer[i];
            }
        }

        s_array(const s_array& mValue)
        {
            memcpy(lArray_, mValue.lArray_, N*sizeof(T));
        }

        void Set(const T& mValue)
        {
            for (uint i = 0; i < N; i++)
            {
                lArray_[i] = mValue;
            }
        }

        void Set(const T lArray[N])
        {
            for (uint i = 0; i < N; i++)
            {
                lArray_[i] = lArray[i];
            }
        }

        void Set(const T*& lArray)
        {
            for (uint i = 0; i < N; i++)
            {
                lArray_[i] = lArray[i];
            }
        }

        s_uint GetSize() const
        {
            return N;
        }

        const T& Get(const s_uint& uiIndex) const
        {
            if (uiIndex < N)
                return lArray_[uiIndex.Get()];
            else
                return mDummy;
        }

        T& operator [] (const s_uint& uiIndex)
        {
            if (uiIndex < N)
                return lArray_[uiIndex.Get()];
            else
                return mDummy;
        }

        bool operator == (const s_array& mValue)
        {
            for (uint i = 0; i < N; i++)
            {
                if (lArray_[i] != mValue[i])
                    return false;
            }

            return true;
        }

        s_array& operator = (const s_array& mValue)
        {
            memcpy(lArray_, mValue.lArray_, N*sizeof(T));
            return *this;
        }

        static T mDummy;

    private :

        T lArray_[N];
    };

    template<class T, uint N> T s_array<T, N>::mDummy;

    template<class T>
    s_str operator + (const s_str& sLeft, const s_ctnr<T>& mRight)
    {
        s_str sTemp = "(";
        for (s_uint i = 0; i < mRight.GetSize(); i++)
        {
            if (i == mRight.GetSize()-s_uint(1u))
                sTemp += s_str(mRight[i]);
            else
                sTemp += s_str(mRight[i]) + ", ";
        }
        sTemp += ")";

        return sLeft + sTemp;
    }

    template<class T>
    s_str operator + (const char* sLeft, const s_ctnr<T>& mRight)
    {
        return s_str(sLeft) + mRight;
    }

    template<class T, uint N>
    s_str operator + (const s_str& sLeft, const s_array<T, N>& mRight)
    {
        s_str sTemp = "(";
        for (uint i = 0; i < N; i++)
        {
            if (i == N-1)
                sTemp += s_str(mRight.Get(i));
            else
                sTemp += s_str(mRight.Get(i)) + ", ";
        }
        sTemp += ")";

        return sLeft + sTemp;
    }

    template<class T, uint N>
    s_str operator + (const char* sLeft, const s_array<T, N>& mRight)
    {
        return s_str(sLeft) + mRight;
    }

    template<class T, uint N>
    s_str& operator << (s_str& sLeft, const s_array<T, N>& mRight)
    {
        s_str sTemp = "(";
        for (uint i = 0; i < N; i++)
        {
            if (i == N-1)
                sTemp += s_str(mRight.Get(i));
            else
                sTemp += s_str(mRight.Get(i)) + ", ";
        }
        sTemp += ")";

        return sLeft << sTemp;
    }
}



#endif
