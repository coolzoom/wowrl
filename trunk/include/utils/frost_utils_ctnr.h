// Warning : If you need to use this file, include frost_utils_types.h
namespace Frost
{
    /// Base type : container (sequence)
    /** This class is basically a wrapper around std::deque.
    *   A "cool feature" is the overloading of the ',' operator,
    *   which allows you to write :<br><br>
    *   s_ctnr<s_int> myArray((x, y, ...));<br><br>
    *   ... with any class that has overloaded its ',' operator.<br><br>
    *   <b>Don't forget the double parenthesis</b> : if you don't
    *   use them, myArray will only contain the latest value.
    */
    template<class T>
    class s_ctnr
    {
    public :

        typedef typename std::deque<T>::iterator       iterator;
        typedef typename std::deque<T>::const_iterator const_iterator;

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
            mDeque_.push_back(mElem);
        }

        template<uint N>
        /// Constructor.
        /** \param lElemArray The array to use to initialize this s_ctnr
        *   \note Takes every elements contained by the s_array and puts them
        *         inside this s_ctnr.
        */
        s_ctnr(const s_array<T,N>& lElemArray)
        {
            for (uint i = 0; i < N; ++i)
                mDeque_.push_back(lElemArray[i]);
        }

        /// Returns the last element of this container.
        /** \return The last element of this container
        *   \note You should check this container is not empty
        *         before calling this function.
        */
        T& Back()
        {
            return mDeque_.back();
        }

        /// Returns the last element of this container.
        /** \return The last element of this container
        *   \note You should check this container is not empty
        *         before calling this function.
        */
        const T& Back() const
        {
            return mDeque_.back();
        }

        /// Adds a value at the end of the container.
        /** \param mElem The value to add
        */
        void PushBack(const T& mElem)
        {
            mDeque_.push_back(mElem);
        }

        /// Adds a set of values at the end of the container.
        /** \param lArray The array of value to add
        *   \note Order is preserved.
        */
        void PushBack(const s_ctnr& lArray)
        {
            const_iterator iter;
            for (iter = lArray.begin(); iter != lArray.end(); ++iter)
                mDeque_.push_back(*iter);
        }

        /// Removes the last value of the container.
        void PopBack()
        {
            mDeque_.pop_back();
        }

        /// Removes values from the end of the container.
        /** \param uiNbr The number of value to remove
        */
        void PopBack(const s_uint& uiNbr)
        {
            if (uiNbr >= mDeque_.size())
                mDeque_.clear();
            else
            {
                for (uint ui; ui < uiNbr.Get(); ++ui)
                    mDeque_.pop_back();
            }
        }

        /// Returns the first element of this container.
        /** \return The first element of this container
        *   \note You should check this container is not empty
        *         before calling this function.
        */
        T& Front()
        {
            return mDeque_.front();
        }

        /// Returns the first element of this container.
        /** \return The first element of this container
        *   \note You should check this container is not empty
        *         before calling this function.
        */
        const T& Front() const
        {
            return mDeque_.front();
        }

        /// Adds a value at the beginning of the container.
        /** \param mElem The value to add
        */
        void PushFront(const T& mElem)
        {
            mDeque_.push_front(mElem);
        }

        /// Adds a set of values at the beginning of the container.
        /** \param lArray The array of value to add
        *   \note Order is preserved.
        */
        void PushFront(const s_ctnr& lArray)
        {
            const_iterator iter = lArray.end();
            while (iter != lArray.begin())
            {
                --iter;
                mDeque_.push_front(*iter);
            }
        }

        /// Removes the first value of the container.
        void PopFront()
        {
            mDeque_.pop_front();
        }

        /// Removes values from the beginning of the container.
        /** \param uiNbr The number of value to remove
        */
        void PopFront(const s_uint& uiNbr)
        {
            if (uiNbr >= mDeque_.size())
                mDeque_.clear();
            else
            {
                for (uint ui; ui < uiNbr.Get(); ++ui)
                    mDeque_.pop_front();
            }
        }

        /// Removes all elements from this container.
        void Clear()
        {
            mDeque_.clear();
        }

        /// Inserts or erases elements from the end of this container.
        /** \param uiSize The new size of the container
        *   \param mElem  The dummy element to append is the new size is
        *                 greater than the actual one
        *   \note If uiSize is superior to the container's actual size,
        *         mElem is appended as many time as neccessary to fit the
        *         new size. Else, elements are erased.
        */
        void Resize(const s_uint& uiSize, const T& mElem = T())
        {
            mDeque_.resize(uiSize.Get(), mElem);
        }

        /// Searches the container for the provided element.
        /** \param mElem   The element to search for
        *   \param uiStart The position from which to start searching
        *   \return 'true' if the element has been found
        */
        s_bool Find(const T& mElem, const s_uint& uiStart = 0u) const
        {
            return (std::find(mDeque_.begin() + uiStart.Get(), mDeque_.end(), mElem) != mDeque_.end());
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
            const_iterator iterPos = std::find(mDeque_.begin() + uiStart.Get(), mDeque_.end(), mElem);
            if (iterPos == mDeque_.end())
                return s_uint::NaN;
            else
                return s_uint((uint)(iterPos - mDeque_.begin()));
        }

        /// Searches the container for the provided element.
        /** \param mElem   The element to search for
        *   \param uiStart The position from which to start searching
        *   \return An iterator pointing to this element
        *   \note If the element is not present in the container, this function
        *         returns End().
        */
        iterator Get(const T& mElem, const s_uint& uiStart = 0u)
        {
            iterator iterStart = mDeque_.begin() + uiStart.Get();
            return std::find(iterStart, mDeque_.end(), mElem);
        }

        /// Searches the container for the provided element.
        /** \param mElem   The element to search for
        *   \param uiStart The position from which to start searching
        *   \return An iterator pointing to this element
        *   \note If the element is not present in the container, this function
        *         returns End().
        */
        const_iterator Get(const T& mElem, const s_uint& uiStart = 0u) const
        {
            const_iterator iterStart = mDeque_.begin() + uiStart.Get();
            return std::find(iterStart, mDeque_.end(), mElem);
        }

        /// Erases an element from the container.
        /** \param uiPos The position of the element to erase
        */
        void Erase(const s_uint& uiPos)
        {
            if (uiPos.IsValid() && uiPos < mDeque_.size())
                mDeque_.erase(mDeque_.begin() + uiPos.Get());
        }

        /// Erases an element from the container.
        /** \param mIter The iterator pointing at the element you want to remove
        *   \return The iterator pointing at the next element in the container
        *   \note If the provided iterator was pointing to the last element, then
        *         End() is returned.
        */
        iterator Erase(const iterator& mIter)
        {
            return mDeque_.erase(mIter);
        }

        /// Returns the number of elements in this container.
        /** \return The number of elements in this container
        */
        s_uint GetSize() const
        {
            return mDeque_.size();
        }

        /// Checks if this container is empty.
        /** \return 'true' if this container is empty
        */
        s_bool IsEmpty() const
        {
            return mDeque_.empty();
        }

        /// Inverts the order of the elements.
        void Reverse()
        {
            s_ctnr lTemp;
            iterator iter = mDeque_.end();
            while (iter != mDeque_.begin())
            {
                --iter;
                lTemp.push_back(*iter);
            }
            *this = lTemp;
        }

        s_ctnr& operator, (const T& mElem)
        {
            mDeque_.push_back(mElem);
            return (*this);
        }

        s_ctnr<s_ctnr> operator, (const s_ctnr& lValue)
        {
            s_ctnr<s_ctnr> mContainer;
            mContainer.PushBack(*this);
            mContainer.PushBack(lValue);
            return mContainer;
        }

        template<uint N>
        s_ctnr& operator = (const s_array<T,N>& lElemArray)
        {
            mDeque_.clear();

            for (uint i = 0; i < N; ++i)
                mDeque_.push_back(lElemArray[i]);
        }

        T& operator[] (const s_uint& uiIndex)
        {
            return mDeque_[uiIndex.Get()];
        }

        const T& operator[] (const s_uint& uiIndex) const
        {
            return mDeque_[uiIndex.Get()];
        }

        iterator Begin()
        {
            return mDeque_.begin();
        }

        const_iterator Begin() const
        {
            return mDeque_.begin();
        }

        iterator End()
        {
            return mDeque_.end();
        }

        const_iterator End() const
        {
            return mDeque_.end();
        }

        iterator begin()
        {
            return mDeque_.begin();
        }

        const_iterator begin() const
        {
            return mDeque_.begin();
        }

        iterator end()
        {
            return mDeque_.end();
        }

        const_iterator end() const
        {
            return mDeque_.end();
        }

    private :

        std::deque<T> mDeque_;
    };

    template<class T, class N>
    s_str_t<N> operator + (const s_str_t<N>& sLeft, const s_ctnr<T>& mRight)
    {
        s_str_t<N> sTemp = "(";
        for (s_uint i = 0; i < mRight.GetSize(); ++i)
        {
            if (i == mRight.GetSize()-1)
                sTemp << mRight[i];
            else
                sTemp << mRight[i] << ", ";
        }
        sTemp << ")";

        return sLeft + sTemp;
    }

    template<class T, class N>
    s_str_t<N> operator + (const N* sLeft, const s_ctnr<T>& mRight)
    {
        return s_str_t<N>(sLeft) + mRight;
    }
}
