// Warning : If you need to use this file, include frost_utils_types.h
namespace Frost
{
    /// Base type : container (sequence)
    /** This is a generic 1D container. It is implemented
    *   by an std::deque for s_ctnr, and an std::vector for s_array.
    */
    template< class T, class C >
    class s_ctnr_t
    {
    public :

        typedef T                          ValueType;
        typedef C                          ContainerType;
        typedef typename C::iterator       iterator;
        typedef typename C::const_iterator const_iterator;
        typedef s_range<iterator>          range;
        typedef s_range<const_iterator>    const_range;

        /// Default constructor.
        s_ctnr_t()
        {
        }

        /// Destructor.
        virtual ~s_ctnr_t()
        {
        }

        /// Copy constructor.
        s_ctnr_t(const s_ctnr_t& mCtnr)
        {
            mContainer_ = mCtnr.mContainer_;
        }

        /// Conversion constructor.
        template<class N, class M>
        s_ctnr_t(const s_ctnr_t<N, M>& mCtnr)
        {
            typename s_ctnr_t<N, M>::const_iterator iter;
            for (iter = mCtnr.Begin(); iter != mCtnr.End(); ++iter)
                mContainer_.push_back(*iter);
        }

        /// Constructor.
        /** \param mElem The first element of the sequence
        *   \note You can use this constructor, but it's main use is to build
        *         s_ctnr_t by enclosing a series of values by parenthesis (if these
        *         values have implemented the ',' operator).
        */
        s_ctnr_t(const ValueType& mElem)
        {
            mContainer_.push_back(mElem);
        }

        #ifdef CPP_0X
            /// Initializer list constructor.
            /** \param mList Brace enclosed element list
            *   \note This constructor uses a C++0x feature.<br>
            *         It allows : s_ctnr_t<s_int> v = {1, 2, 3, 4};
            */
            s_ctnr_t(std::initializer_list<ValueType> mList)
            {
                for (const ValueType* p = mList.begin(); p != mList.end(); ++p)
                    mContainer_.push_back(*p);
            }
        #endif

        /// Constructor.
        /** \param lElemArray The array to use to initialize this s_ctnr_t
        *   \note Takes every elements contained by the s_array and puts them
        *         inside this s_ctnr_t.
        */
        template<default_uint N>
        s_ctnr_t(const s_array<ValueType,N>& lElemArray)
        {
            for (default_uint i = 0; i < N; ++i)
                mContainer_.push_back(lElemArray[i]);
        }

        /// Returns the last element of this container.
        /** \return The last element of this container
        *   \note You should check this container is not empty
        *         before calling this function.
        */
        ValueType& Back()
        {
            return mContainer_.back();
        }

        /// Returns the last element of this container.
        /** \return The last element of this container
        *   \note You should check this container is not empty
        *         before calling this function.
        */
        const ValueType& Back() const
        {
            return mContainer_.back();
        }

        /// Adds a value at the end of the container.
        /** \param mElem The value to add
        */
        void PushBack(const ValueType& mElem)
        {
            mContainer_.push_back(mElem);
        }

        /// Adds a set of values at the end of the container.
        /** \param lArray The array of value to add
        *   \note Order is preserved.
        */
        void PushBack(const s_ctnr_t& lArray)
        {
            const_iterator iter;
            for (iter = lArray.begin(); iter != lArray.end(); ++iter)
                mContainer_.push_back(*iter);
        }

        /// Removes the last value of the container.
        void PopBack()
        {
            mContainer_.pop_back();
        }

        /// Removes values from the end of the container.
        /** \param uiNbr The number of value to remove
        */
        void PopBack(const s_uint_t<default_uint>& uiNbr)
        {
            if (uiNbr >= mContainer_.size())
                mContainer_.clear();
            else
            {
                for (default_uint ui = 0; ui < uiNbr.Get(); ++ui)
                    mContainer_.pop_back();
            }
        }

        /// Returns the first element of this container.
        /** \return The first element of this container
        *   \note You should check this container is not empty
        *         before calling this function.
        */
        ValueType& Front()
        {
            return mContainer_.front();
        }

        /// Returns the first element of this container.
        /** \return The first element of this container
        *   \note You should check this container is not empty
        *         before calling this function.
        */
        const ValueType& Front() const
        {
            return mContainer_.front();
        }

        /// Adds a value at the beginning of the container.
        /** \param mElem The value to add
        */
        void PushFront(const ValueType& mElem)
        {
            mContainer_.push_front(mElem);
        }

        /// Adds a set of values at the beginning of the container.
        /** \param lArray The array of value to add
        *   \note Order is preserved.
        */
        void PushFront(const s_ctnr_t& lArray)
        {
            const_iterator iter = lArray.end();
            while (iter != lArray.begin())
            {
                --iter;
                mContainer_.push_front(*iter);
            }
        }

        /// Removes the first value of the container.
        void PopFront()
        {
            mContainer_.pop_front();
        }

        /// Removes values from the beginning of the container.
        /** \param uiNbr The number of value to remove
        */
        void PopFront(const s_uint_t<default_uint>& uiNbr)
        {
            if (uiNbr >= mContainer_.size())
                mContainer_.clear();
            else
            {
                for (default_uint ui = 0; ui < uiNbr.Get(); ++ui)
                    mContainer_.pop_front();
            }
        }

        /// Removes all elements from this container.
        void Clear()
        {
            mContainer_.clear();
        }

        /// Inserts or erases elements from the end of this container.
        /** \param uiSize The new size of the container
        *   \param mElem  The dummy element to append is the new size is
        *                 greater than the current one
        *   \note If uiSize is superior to the container's current size,
        *         mElem is appended as many time as neccessary to fit the
        *         new size. Else, elements are erased.
        */
        void Resize(const s_uint_t<default_uint>& uiSize, const ValueType& mElem = ValueType())
        {
            mContainer_.resize(uiSize.Get(), mElem);
        }

        /// Searches the container for the provided element.
        /** \param mElem   The element to search for
        *   \return 'true' if the element has been found
        */
        s_bool Find(const ValueType& mElem) const
        {
            return (std::find(mContainer_.begin(), mContainer_.end(), mElem) != mContainer_.end());
        }

        /// Searches the container for the provided element.
        /** \param mElem   The element to search for
        *   \param uiStart The position from which to start searching
        *   \return 'true' if the element has been found
        */
        s_bool Find(const ValueType& mElem, const s_uint_t<default_uint>& uiStart) const
        {
            return (std::find(mContainer_.begin() + uiStart.Get(), mContainer_.end(), mElem) != mContainer_.end());
        }

        /// Searches the container for the provided element.
        /** \param mElem   The element to search for
        *   \return The position of the element in the container
        *   \note If the element is not present in the container, this function
        *         returns s_uint::NaN.
        */
        s_uint_t<default_uint> FindPos(const ValueType& mElem) const
        {
            const_iterator iterPos = std::find(mContainer_.begin(), mContainer_.end(), mElem);
            if (iterPos == mContainer_.end())
                return s_uint_t<default_uint>::NaN;
            else
                return iterPos - mContainer_.begin();
        }

        /// Searches the container for the provided element.
        /** \param mElem   The element to search for
        *   \param uiStart The position from which to start searching
        *   \return The position of the element in the container
        *   \note If the element is not present in the container, this function
        *         returns s_uint::NaN.
        */
        s_uint_t<default_uint> FindPos(const ValueType& mElem, const s_uint_t<default_uint>& uiStart) const
        {
            const_iterator iterPos = std::find(mContainer_.begin() + uiStart.Get(), mContainer_.end(), mElem);
            if (iterPos == mContainer_.end())
                return s_uint_t<default_uint>::NaN;
            else
                return (default_uint)(iterPos - mContainer_.begin());
        }

        /// Searches the container for the provided element.
        /** \param mElem   The element to search for
        *   \note If the element is not present in the container, this function
        *         returns End().
        */
        iterator Get(const ValueType& mElem)
        {
            return std::find(mContainer_.begin(), mContainer_.end(), mElem);
        }

        /// Searches the container for the provided element.
        /** \param mElem   The element to search for
        *   \param uiStart The position from which to start searching
        *   \return An iterator pointing to this element
        *   \note If the element is not present in the container, this function
        *         returns End().
        */
        iterator Get(const ValueType& mElem, const s_uint_t<default_uint>& uiStart)
        {
            iterator iterStart = mContainer_.begin() + uiStart.Get();
            return std::find(iterStart, mContainer_.end(), mElem);
        }

        /// Searches the container for the provided element.
        /** \param mElem   The element to search for
        *   \return An iterator pointing to this element
        *   \note If the element is not present in the container, this function
        *         returns End().
        */
        const_iterator Get(const ValueType& mElem) const
        {
            return std::find(mContainer_.begin(), mContainer_.end(), mElem);
        }

        /// Searches the container for the provided element.
        /** \param mElem   The element to search for
        *   \param uiStart The position from which to start searching
        *   \return An iterator pointing to this element
        *   \note If the element is not present in the container, this function
        *         returns End().
        */
        const_iterator Get(const ValueType& mElem, const s_uint_t<default_uint>& uiStart = 0u) const
        {
            const_iterator iterStart = mContainer_.begin() + uiStart.Get();
            return std::find(iterStart, mContainer_.end(), mElem);
        }

        /// Inserts an element before mIter.
        /** \param mIter  The reference iterator
        *   \param mValue The value to insert
        *   \return An iterator pointing to the inserted value
        */
        iterator Insert(const iterator& mIter, const T& mValue)
        {
            return mContainer_.insert(mIter, mValue);
        }

        /// Inserts all values in a range before mIter.
        /** \param mIter  The reference iterator
        *   \param mRange The range containing values to insert
        *   \return An iterator pointing to the last inserted value
        *   \note The last element of the range will not be inserted.
        */
        iterator Insert(const iterator& mIter, const range& mRange)
        {
            iterator mTempIter = mIter;
            --mTempIter;
            mContainer_.insert(mIter, mRange.Begin(), mRange.End());
            return mTempIter + (mRange.End() - mRange.Begin());
        }

        /// Inserts the content of a container before mIter.
        /** \param mIter      The reference iterator
        *   \param mContainer The container to insert
        *   \return An iterator pointing to the last inserted value
        */
        iterator Insert(const iterator& mIter, const s_ctnr_t& mContainer)
        {
            iterator mTempIter = mIter;
            --mTempIter;
            mContainer_.insert(mIter, mContainer.Begin(), mContainer.End());
            return mTempIter + mContainer.mContainer_.size();
        }

        /// Erases an element from the container.
        /** \param uiPos The position of the element to erase
        */
        void Erase(const s_uint_t<default_uint>& uiPos)
        {
            if (uiPos.IsValid() && uiPos < mContainer_.size())
                mContainer_.erase(mContainer_.begin() + uiPos.Get());
        }

        /// Erases an element from the container.
        /** \param mIter The iterator pointing at the element you want to remove
        *   \return The iterator pointing at the next element in the container
        *   \note If the provided iterator was pointing to the last element, then
        *         End() is returned.
        */
        iterator Erase(const iterator& mIter)
        {
            return mContainer_.erase(mIter);
        }

        /// Erases a range of elements from the container.
        /** \param uiStart The position from which to start erasing
        *   \param uiEnd   The position at which to stop erasing (exclusive)
        */
        void EraseRange(const s_uint_t<default_uint>& uiStart, const s_uint_t<default_uint>& uiEnd)
        {
            if (uiStart.IsValid())
            {
                if (!uiEnd.IsValid())
                    mContainer_.erase(mContainer_.begin() + uiStart.Get(), mContainer_.end());
                else
                    mContainer_.erase(mContainer_.begin() + uiStart.Get(), mContainer_.begin() + uiEnd.Get());
            }
        }

        /// Erases a range of elements from the container.
        /** \param mBegin The position from which to start erasing
        *   \param mEnd   The position at which to stop erasing (exclusive)
        *   \return The iterator pointing at the next element in the container
        */
        iterator EraseRange(const iterator& mBegin, const iterator& mEnd)
        {
            return mContainer_.erase(mBegin, mEnd);
        }

        /// Erases a range of elements from the container.
        /** \param mRange The range to erase
        *   \return The iterator pointing at the next element in the container
        */
        iterator EraseRange(const range& mRange)
        {
            return mContainer_.erase(mRange.Begin(), mRange.End());
        }

        /// Returns the number of elements in this container.
        /** \return The number of elements in this container
        */
        s_uint_t<default_uint> GetSize() const
        {
            return mContainer_.size();
        }

        /// Checks if this container is empty.
        /** \return 'true' if this container is empty
        */
        s_bool IsEmpty() const
        {
            return mContainer_.empty();
        }

        /// Inverts the order of the elements.
        void Reverse()
        {
            std::reverse(mContainer_.begin(), mContainer_.end());
        }

        /// Sorts the elements of this container according to the "<" operator.
        void Sort()
        {
            std::sort(mContainer_.begin(), mContainer_.end());
        }

        s_ctnr_t& operator, (const ValueType& mElem)
        {
            mContainer_.push_back(mElem);
            return (*this);
        }

        s_ctnr_t<s_ctnr_t, ContainerType> operator, (const s_ctnr_t& lValue)
        {
            s_ctnr_t<s_ctnr_t, ContainerType> mContainer;
            mContainer.PushBack(*this);
            mContainer.PushBack(lValue);
            return mContainer;
        }

        template<default_uint N>
        s_ctnr_t& operator = (const s_array<ValueType,N>& lElemArray)
        {
            mContainer_.clear();

            for (default_uint i = 0; i < N; ++i)
                mContainer_.push_back(lElemArray[i]);

            return *this;
        }

        ValueType& operator[] (const s_uint_t<default_uint>& uiIndex)
        {
            return mContainer_[uiIndex.Get()];
        }

        const ValueType& operator[] (const s_uint_t<default_uint>& uiIndex) const
        {
            return mContainer_[uiIndex.Get()];
        }

        ValueType& operator[] (const default_uint& uiIndex)
        {
            return mContainer_[uiIndex];
        }

        const ValueType& operator[] (const default_uint& uiIndex) const
        {
            return mContainer_[uiIndex];
        }

        iterator Begin()
        {
            return mContainer_.begin();
        }

        const_iterator Begin() const
        {
            return mContainer_.begin();
        }

        iterator End()
        {
            return mContainer_.end();
        }

        const_iterator End() const
        {
            return mContainer_.end();
        }

        iterator begin()
        {
            return mContainer_.begin();
        }

        const_iterator begin() const
        {
            return mContainer_.begin();
        }

        iterator end()
        {
            return mContainer_.end();
        }

        const_iterator end() const
        {
            return mContainer_.end();
        }

    protected :

        ContainerType mContainer_;
    };

    /// Simple container.
    /** Implemented with std::deque : supports fast operations
    *   at both begin and end of the container.<br>
    *   Usefull for unordered list of elements.
    */
    template<class T>
    class s_ctnr : public s_ctnr_t< T, std::deque<T> >
    {
    public :

        typedef T                            ValueType;
        typedef s_ctnr_t< T, std::deque<T> > ParentContainerType;

        s_ctnr() : ParentContainerType() {}

        s_ctnr(const ValueType& mElem) : ParentContainerType(mElem) {}

        #ifdef CPP_0X
            s_ctnr(std::initializer_list<ValueType> mList) : ParentContainerType(mList) {}
        #endif

        template<default_uint N>
        s_ctnr(const s_array<ValueType,N>& lElemArray) : ParentContainerType(lElemArray) {}

        s_ctnr(const ParentContainerType& mCtnr) : ParentContainerType(mCtnr) {}
    };

    /// Dynamic array.
    /** Implemented with std::vector : supports fast operations
    *   at the end of the container.<br>
    *   It's main advantage over s_ctnr is that you can get a pointer
    *   to the data it contains, if you need to feed it to an external
    *   library without bothering with dynamic allocations and classic
    *   C arrays.<br>
    *   Use the Reserve() function to allocate the needed space : it will
    *   greatly improve performances.
    */
    template<class T>
    class s_array<T,0> : public s_ctnr_t< T, std::vector<T> >
    {
    public :

        typedef T                             ValueType;
        typedef s_ctnr_t< T, std::vector<T> > ParentContainerType;
        using ParentContainerType::mContainer_;

        s_array() : ParentContainerType() {}

        s_array(const ValueType& mElem) : ParentContainerType(mElem) {}

        #ifdef CPP_0X
            s_array(std::initializer_list<ValueType> mList) : ParentContainerType(mList) {}
        #endif

        template<default_uint N>
        s_array(const s_array<ValueType,N>& lElemArray) : ParentContainerType(lElemArray) {}

        s_array(const ParentContainerType& mCtnr) : ParentContainerType(mCtnr) {}

        s_array(const s_ctnr_t< T, std::deque<T> >& mCtnr) : ParentContainerType(mCtnr) {}

        /// Returns the underlying C array.
        /** \return The underlying C array
        */
        ValueType* GetClassicArray()
        {
            return &mContainer_[0];
        }

        /// Reserves some space in memory.
        /** \param uiSize The requested space
        *   \note Can be used to reduce the number of allocations
        *         and improve performances.
        */
        void Reserve(const s_uint_t<default_uint>& uiSize)
        {
            mContainer_.reserve(uiSize.Get());
        }
    };

    /** \cond NOT_REMOVE_FROM_DOC
    */
    template<class T, class C> class StringConverter< string_element, s_ctnr_t<T,C> >
    {
    public :

        typedef string_object string;

        static string Convert(const s_ctnr_t<T,C>& mCtnr)
        {
            string sTemp = "(";
            typename s_ctnr_t<T,C>::const_iterator iter;
            for (iter = mCtnr.Begin(); iter != mCtnr.End(); ++iter)
            {
                if (iter != mCtnr.Begin())
                    sTemp += ", " + StringConverter<string_element, T>::Convert(*iter);
                else
                    sTemp += StringConverter<string_element, T>::Convert(*iter);
            }
            sTemp += ")";

            return sTemp;
        }
    };

    template<class T> class StringConverter< string_element, s_ctnr<T> >
    {
    public :

        typedef string_object string;

        static string Convert(const s_ctnr<T>& mCtnr)
        {
            string sTemp = "(";
            typename s_ctnr<T>::const_iterator iter;
            for (iter = mCtnr.Begin(); iter != mCtnr.End(); ++iter)
            {
                if (iter != mCtnr.Begin())
                    sTemp += ", " + StringConverter<string_element, T>::Convert(*iter);
                else
                    sTemp += StringConverter<string_element, T>::Convert(*iter);
            }
            sTemp += ")";

            return sTemp;
        }
    };

    template<class T> class StringConverter< string_element, s_array<T,0> >
    {
    public :

        typedef string_object string;

        static string Convert(const s_array<T,0>& mCtnr)
        {
            string sTemp = "(";
            typename s_array<T,0>::const_iterator iter;
            for (iter = mCtnr.Begin(); iter != mCtnr.End(); ++iter)
            {
                if (iter != mCtnr.Begin())
                    sTemp += ", " + StringConverter<string_element, T>::Convert(*iter);
                else
                    sTemp += StringConverter<string_element, T>::Convert(*iter);
            }
            sTemp += ")";

            return sTemp;
        }
    };
    /** \endcond
    */
}
