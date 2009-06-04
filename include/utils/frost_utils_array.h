/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*                                        */
/*                                        */

#ifndef FROST_UTILS_ARRAY_H
#define FROST_UTILS_ARRAY_H

#include "frost_utils.h"
#include "frost_utils_uint.h"
#include "frost_utils_int.h"

namespace Frost
{
    class s_str;
    template<class T, uint N> class s_array;

    /// Base type : container
    /** This class is basically a wrapper around std::deque.<br>
    *   It works exactly the same, with one exception : iterators
    *   are never invalidated by any of this class's member functions*.<br>
    *   These "home maid" iterators might be a little slower (about 2 times)
    *   than those provided by the standard. If you need performances, use
    *   standard containers directly.<br>
    *   One more important thing to note is the overloading of the ','
    *   operator, which allows you to write :<br><br>
    *   s_array<s_int, 2> myArray((s_int(0), s_int(1)));<br><br>
    *   ... with any class that has overloaded its ',' operator.<br><br>
    *   <b>Don't forget the double parenthesis</b> : if you
    *   don't use them, myArray will initialize all its fields
    *   with the latest value.<br><br>
    *   * : of course, if you have an iterator pointing to the 5th element, and
    *   you resize the container to 4, then the iterator will behave like end()
    *   (and will most likely crash when you'll try to dereference it).
    */
    template<class T>
    class s_ctnr
    {
    public :

        class iterator;
        class const_iterator;

        friend class s_ctnr::iterator;
        friend class s_ctnr::const_iterator;

        /// Default constructor.
        s_ctnr()
        {
        }

        /// Constructor.
        /** \param mElem The first element of a (.., .., ..) sequence
        *   \note You can use this constructor, but it's main use is to build
        *         s_ctnr by enclosing a series of values by parenthesis (if these
        *         values have implemented the ',' operator).
        */
        s_ctnr(T mElem)
        {
            lArray_.push_back(mElem);
        }

        template<uint N>
        /// Constructor.
        /** \param lElemArray The array to use to initialize this s_ctnr
        *   \note Takes every elements contained by the s_array and puts them
        *         inside this s_ctnr.
        */
        s_ctnr(const s_array<T,N>& lElemArray)
        {
            for (uint i = 0; i < N; i++)
                lArray_.push_back(lElemArray[i]);
        }

        /// Returns the last element of this container.
        /** \return The last element of this container
        *   \note You should check this container is not empty
        *         before calling this function.
        */
        T& Back()
        {
            return lArray_.back();
        }

        /// Returns the last element of this container.
        /** \return The last element of this container
        *   \note You should check this container is not empty
        *         before calling this function.
        */
        const T& Back() const
        {
            return lArray_.back();
        }

        /// Adds a value at the end of the container.
        /** \param mElem The value to add
        */
        void PushBack(const T& mElem)
        {
            lArray_.push_back(mElem);
        }

        /// Removes the last value of the container.
        void PopBack()
        {
            lArray_.pop_back();
        }

        /// Returns the first element of this container.
        /** \return The first element of this container
        *   \note You should check this container is not empty
        *         before calling this function.
        */
        T& Front()
        {
            return lArray_.front();
        }

        /// Returns the first element of this container.
        /** \return The first element of this container
        *   \note You should check this container is not empty
        *         before calling this function.
        */
        const T& Front() const
        {
            return lArray_.front();
        }

        /// Adds a value at the beginning of the container.
        /** \param mElem The value to add
        */
        void PushFront(const T& mElem)
        {
            lArray_.push_front(mElem);
        }

        /// Removes the first value of the container.
        void PopFront()
        {
            lArray_.pop_front();
        }

        /// Removes all values from this container.
        void Clear()
        {
            lArray_.clear();
        }

        /// Inserts or erases elements from the end of this container.
        /** \param uiSize The new size of the container
        *   \note If uiSize is superior to the container's actual size,
        *         mElem is appended as many time as neccessary to fit the
        *         new size. Else, elements are erased.
        */
        void Resize(const s_uint& uiSize, const T& mElem = T())
        {
            lArray_.resize(uiSize.Get(), mElem);
        }

        /// Searches the container for the provided element.
        /** \param mElem   The element to search for
        *   \param uiStart The position from which to start searching
        *   \return 'true' if the element has been found
        */
        s_bool Find(const T& mElem, const s_uint& uiStart = 0u) const
        {
            typename std::deque<T>::const_iterator iterStart = lArray_.begin() + uiStart.Get();
            return (std::find(iterStart, lArray_.end(), mElem) != lArray_.end());
        }

        /// Searches the container for the provided element.
        /** \param mElem   The element to search for
        *   \param uiStart The position from which to start searching
        *   \return The position of the element in the container
        *   \note If the element is not present in the container, this function
        *         returns s_uint::NaN.
        */
        s_uint FindPos(const T& mElem, const s_uint& uiStart = 0u) const
        {
            typename std::deque<T>::const_iterator iterStart = lArray_.begin() + uiStart.Get();
            typename std::deque<T>::const_iterator iterPos = std::find(iterStart, lArray_.end(), mElem);
            if (iterPos == lArray_.end())
                return s_uint::NaN;
            else
                return s_uint((uint)(iterPos - lArray_.begin()));
        }

        /// Searches the container for the provided element.
        /** \param mElem   The element to search for
        *   \param uiStart The position from which to start searching
        *   \return An iterator pointing at this element
        *   \note If the element is not present in the container, this function
        *         returns End().
        */
        iterator FindIter(const T& mElem, const s_uint& uiStart = 0u)
        {
            typename std::deque<T>::iterator iterStart = lArray_.begin() + uiStart.Get();
            return iterator(this, std::find(iterStart, lArray_.end(), mElem) - lArray_.begin());
        }

        /// Searches the container for the provided element.
        /** \param mElem   The element to search for
        *   \param uiStart The position from which to start searching
        *   \return An iterator pointing at this element
        *   \note If the element is not present in the container, this function
        *         returns End().
        */
        const_iterator FindIter(const T& mElem, const s_uint& uiStart = 0u) const
        {
            typename std::deque<T>::const_iterator iterStart = lArray_.begin() + uiStart.Get();
            return const_iterator(this, std::find(iterStart, lArray_.end(), mElem) - lArray_.begin());
        }

        /// Erases an element from the container.
        /** \param uiPos The position of the element to erase
        */
        void Erase(const s_uint& uiPos)
        {
            if (uiPos.IsValid() && uiPos < lArray_.size())
            {
                typename std::deque<T>::iterator iter = lArray_.begin() + uiPos.Get();
                iter = lArray_.erase(iter);
            }
        }

        /// Erases an element from the container.
        /** \param mIter The iterator pointing at the element you want to remove
        *   \return The iterator pointing at the next element in the container
        *   \note If the provided iterator was pointing to the last element, then
        *         End() is returned.
        */
        iterator Erase(const iterator& mIter)
        {
            if (mIter.IsValid(this))
            {
                typename std::deque<T>::iterator iter = lArray_.begin() + (mIter - Begin()).Get();
                iter = lArray_.erase(iter);
                if (iter != lArray_.end())
                    return iterator(this, iter - lArray_.begin());
                else
                    return End();
            }
            else
                return End();
        }

        /// Returns the number of elements in this container.
        /** \return The number of elements in this container
        */
        s_uint GetSize() const
        {
            return lArray_.size();
        }

        /// Checks if this container is empty.
        /** \return 'true' if this container is empty
        */
        s_bool IsEmpty() const
        {
            return lArray_.empty();
        }

        s_ctnr& operator, (const T& mElem)
        {
            lArray_.push_back(mElem);
            return (*this);
        }

        s_ctnr<s_ctnr> operator, (const s_ctnr& lValue)
        {
            s_ctnr<s_ctnr> mContainer;
            mContainer.PushBack(*this);
            mContainer.PushBack(lValue);
            return mContainer;
        }

        T& operator[] (const s_uint& uiIndex)
        {
            return lArray_[uiIndex.Get()];
        }

        const T& operator[] (const s_uint& uiIndex) const
        {
            return lArray_[uiIndex.Get()];
        }

        iterator begin()
        {
            return iterator(this, 0);
        }

        const_iterator begin() const
        {
            return const_iterator(this, 0);
        }

        iterator end()
        {
            return iterator(this, -1);
        }

        const_iterator end() const
        {
            return const_iterator(this, -1);
        }

        iterator Begin()
        {
            return iterator(this, 0);
        }

        const_iterator Begin() const
        {
            return const_iterator(this, 0);
        }

        iterator End()
        {
            return iterator(this, -1);
        }

        const_iterator End() const
        {
            return const_iterator(this, -1);
        }

        /// "stl like" s_ctnr iterator
        /** Safely iterates through an s_ctnr.
        */
        class iterator
        {
        public :

            friend class const_iterator;

            iterator()
            {
                pParent_ = NULL;
            }

            iterator(s_ctnr<T>* pParent, const int& iPos)
            {
                pParent_ = pParent;
                iPos_ = iPos;
                if (pParent)
                {
                    if ((uint)iPos_ >= pParent_->lArray_.size())
                        iPos_ = -1;
                }
            }

            explicit iterator(const const_iterator& iter)
            {
                pParent_ = iter.pParent_;
                iPos_ = iter.iPos_;
            }

            s_bool IsValid(s_ctnr<T>* pParent = NULL) const
            {
                if (pParent_)
                {
                    if (pParent != NULL)
                        return ((pParent_ == pParent) && (iPos_ != -1));
                    else
                        return (iPos_ != -1);
                }
                else
                    return false;
            }

            T& operator * () const
            {
                return pParent_->lArray_[iPos_];
            }

            T* operator -> () const
            {
                return &pParent_->lArray_[iPos_];
            }

            iterator operator + ( const s_int& iOffset ) const
            {
                int iNewPos = iPos_;
                if (pParent_ && iOffset.IsValid())
                {
                    if (iOffset > 0)
                    {
                        iNewPos += iOffset.Get();
                        if ((uint)iNewPos >= pParent_->lArray_.size())
                            iNewPos = -1;
                    }
                    if (iOffset < 0)
                    {
                        if (-iOffset < iNewPos)
                            iNewPos += iOffset.Get();
                        else
                            iNewPos = 0;
                    }
                }
                return iterator(pParent_, iNewPos);
            }

            iterator iterator::operator - ( const s_int& iOffset ) const
            {
                int iNewPos = iPos_;
                if (pParent_ && iOffset.IsValid())
                {
                    if (iOffset < 0)
                    {
                        iNewPos -= iOffset.Get();
                        if ((uint)iNewPos >= pParent_->lArray_.size())
                            iNewPos = -1;
                    }
                    if (iOffset > 0)
                    {
                        if (iOffset < iNewPos)
                            iNewPos -= iOffset.Get();
                        else
                            iNewPos = 0;
                    }
                }
                return iterator(pParent_, iNewPos);
            }

            s_int iterator::operator - ( const iterator& iter ) const
            {
                if (iter.pParent_ == pParent_)
                {
                    if (iPos_ == -1)
                    {
                        if (iter.iPos_ == -1)
                            return 0;
                        else
                            return pParent_->lArray_.size() - iter.iPos_;
                    }
                    else
                    {
                        if (iter.iPos_ == -1)
                            return iPos_ - pParent_->lArray_.size();
                        else
                            return iPos_ - iter.iPos_;
                    }
                }
                else
                    return s_int::NaN;
            }

            iterator& iterator::operator ++ ()
            {
                if (pParent_ && (iPos_ != -1))
                {
                    ++iPos_;
                    if ((uint)iPos_ >= pParent_->lArray_.size())
                        iPos_ = -1;
                }

                return *this;
            }

            iterator iterator::operator ++ (int)
            {
                if (pParent_ && (iPos_ != -1))
                {
                    ++iPos_;
                    if ((uint)iPos_ >= pParent_->lArray_.size())
                        iPos_ = -1;
                }

                return *this;
            }

            iterator& iterator::operator -- ()
            {
                if (pParent_)
                {
                    if (iPos_ != -1)
                    {
                        if (iPos_ != 0)
                            --iPos_;
                    }
                    else
                    {
                        if (pParent_->lArray_.empty())
                            iPos_ = -1;
                        else
                            iPos_ = pParent_->lArray_.size() - 1;
                    }
                }

                return *this;
            }

            iterator iterator::operator -- (int)
            {
                if (pParent_)
                {
                    if (iPos_ != -1)
                    {
                        if (iPos_ != 0)
                            --iPos_;
                    }
                    else
                    {
                        if (pParent_->lArray_.empty())
                            iPos_ = -1;
                        else
                            iPos_ = pParent_->lArray_.size() - 1;
                    }
                }

                return *this;
            }

            s_bool iterator::operator != (iterator iter)
            {
                if (iPos_ == -1)
                {
                    if (iter.iPos_ == -1)
                        return (pParent_ != iter.pParent_);
                    else
                        return true;
                }
                else
                {
                    if (iter.iPos_ == -1)
                        return true;
                    else
                        return (pParent_ != iter.pParent_) || (iPos_ != iter.iPos_);
                }
            }

        private :

            s_ctnr<T>* pParent_;
            int        iPos_;

        };

        /// "stl like" s_ctnr iterator
        /** Safely iterates through an s_ctnr.<br>
        *   Ensures it is not modified.
        */
        class const_iterator
        {
        public :

            friend class iterator;

            const_iterator()
            {
                pParent_ = NULL;
            }

            const_iterator(const s_ctnr<T>* pParent, const int& iPos)
            {
                pParent_ = pParent;
                iPos_ = iPos;
                if (pParent)
                {
                    if ((uint)iPos_ >= pParent_->lArray_.size())
                        iPos_ = -1;
                }
            }

            const_iterator(const iterator& iter)
            {
                pParent_ = iter.pParent_;
                iPos_ = iter.iPos_;
            }

            s_bool IsValid(const s_ctnr<T>* pParent = NULL) const
            {
                if (pParent_)
                {
                    if (pParent != NULL)
                        return ((pParent_ == pParent) && (iPos_ != -1));
                    else
                        return (iPos_ != -1);
                }
                else
                    return false;
            }

            const T& operator * () const
            {
                return pParent_->lArray_[iPos_];
            }

            const T* operator -> () const
            {
                return &pParent_->lArray_[iPos_];
            }

            const_iterator operator + ( const s_int& iOffset ) const
            {
                int iNewPos = iPos_;
                if (pParent_ && iOffset.IsValid())
                {
                    if (iOffset > 0)
                    {
                        iNewPos += iOffset.Get();
                        if ((uint)iNewPos >= pParent_->lArray_.size())
                            iNewPos = -1;
                    }
                    if (iOffset < 0)
                    {
                        if (-iOffset < iNewPos)
                            iNewPos += iOffset.Get();
                        else
                            iNewPos = 0;
                    }
                }
                return const_iterator(pParent_, iNewPos);
            }

            const_iterator const_iterator::operator - ( const s_int& iOffset ) const
            {
                int iNewPos = iPos_;
                if (pParent_ && iOffset.IsValid())
                {
                    if (iOffset < 0)
                    {
                        iNewPos -= iOffset.Get();
                        if ((uint)iNewPos >= pParent_->lArray_.size())
                            iNewPos = -1;
                    }
                    if (iOffset > 0)
                    {
                        if (iOffset < iNewPos)
                            iNewPos -= iOffset.Get();
                        else
                            iNewPos = 0;
                    }
                }
                return const_iterator(pParent_, iNewPos);
            }

            s_int const_iterator::operator - ( const const_iterator& iter ) const
            {
                if (iter.pParent_ == pParent_)
                {
                    if (iPos_ == -1)
                    {
                        if (iter.iPos_ == -1)
                            return 0;
                        else
                            return pParent_->lArray_.size() - iter.iPos_;
                    }
                    else
                    {
                        if (iter.iPos_ == -1)
                            return iPos_ - pParent_->lArray_.size();
                        else
                            return iPos_ - iter.iPos_;
                    }
                }
                else
                    return s_int::NaN;
            }

            const_iterator& const_iterator::operator ++ ()
            {
                if (pParent_ && (iPos_ != -1))
                {
                    ++iPos_;
                    if ((uint)iPos_ >= pParent_->lArray_.size())
                        iPos_ = -1;
                }

                return *this;
            }

            const_iterator const_iterator::operator ++ (int)
            {
                if (pParent_ && (iPos_ != -1))
                {
                    ++iPos_;
                    if ((uint)iPos_ >= pParent_->lArray_.size())
                        iPos_ = -1;
                }

                return *this;
            }

            const_iterator& const_iterator::operator -- ()
            {
                if (pParent_)
                {
                    if (iPos_ != -1)
                    {
                        if (iPos_ != 0)
                            --iPos_;
                    }
                    else
                    {
                        if (pParent_->lArray_.empty())
                            iPos_ = -1;
                        else
                            iPos_ = pParent_->lArray_.size() - 1;
                    }
                }

                return *this;
            }

            const_iterator const_iterator::operator -- (int)
            {
                if (pParent_)
                {
                    if (iPos_ != -1)
                    {
                        if (iPos_ != 0)
                            --iPos_;
                    }
                    else
                    {
                        if (pParent_->lArray_.empty())
                            iPos_ = -1;
                        else
                            iPos_ = pParent_->lArray_.size() - 1;
                    }
                }

                return *this;
            }

            s_bool const_iterator::operator != (const_iterator iter)
            {
                if (iPos_ == -1)
                {
                    if (iter.iPos_ == -1)
                        return (pParent_ != iter.pParent_);
                    else
                        return true;
                }
                else
                {
                    if (iter.iPos_ == -1)
                        return true;
                    else
                        return (pParent_ != iter.pParent_) || (iPos_ != iter.iPos_);
                }
            }

        private :

            const s_ctnr<T>* pParent_;
            int              iPos_;

        };

        static const s_str CLASS_NAME;

    private :

        std::deque<T> lArray_;
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

        T& operator [] (const uint& uiIndex)
        {
            if (uiIndex < N)
                return lArray_[uiIndex];
            else
                return mDummy;
        }

        const T& operator [] (const uint& uiIndex) const
        {
            if (uiIndex < N)
                return lArray_[uiIndex];
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

        const T& operator [] (const s_uint& uiIndex) const
        {
            if (uiIndex < N)
                return lArray_[uiIndex.Get()];
            else
                return mDummy;
        }

        s_bool operator == (const s_array& mValue)
        {
            for (uint i = 0; i < N; i++)
            {
                if (lArray_[i] != mValue[i])
                    return false;
            }

            return true;
        }

        s_bool operator != (const s_array& mValue)
        {
            for (uint i = 0; i < N; i++)
            {
                if (lArray_[i] != mValue[i])
                    return true;
            }

            return false;
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
                sTemp << mRight[i];
            else
                sTemp << mRight[i] << ", ";
        }
        sTemp <<")";

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
                sTemp << mRight.Get(i);
            else
                sTemp << mRight.Get(i) << ", ";
        }
        sTemp << ")";

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
                sTemp << mRight.Get(i);
            else
                sTemp << mRight.Get(i) << ", ";
        }
        sTemp << ")";

        return sLeft << sTemp;
    }
}



#endif
