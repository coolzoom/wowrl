/// Base type : container
/** This class is basically a wrapper around std::deque.
*   A "cool feature" is the overloading of the ',' operator, which allows
*   you to write :<br><br>
*   s_array<s_int, 2> myArray((s_int(0), s_int(1)));<br><br>
*   ... with any class that has overloaded its ',' operator.<br><br>
*   <b>Don't forget the double parenthesis</b> : if you don't use them,
*   myArray will initialize all its fields with the latest value. If you don't
*   provide enough elements, myArray will fill the missing space with default
*   elements (default constructor is called).
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
        for (uint i = 0; i < N; ++i)
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

    /// Adds a set of values at the end of the container.
    /** \param lArray The array of value to add
    *   \note Order is preserved.
    */
    void PushBack(const s_ctnr& lArray)
    {
        typename std::deque<T>::const_iterator iter;
        for (iter = lArray.lArray_.begin(); iter != lArray.lArray_.end(); ++iter)
            lArray_.push_back(*iter);
    }

    /// Removes the last value of the container.
    void PopBack()
    {
        lArray_.pop_back();
    }

    /// Removes values from the end of the container.
    /** \param uiNbr The number of value to remove
    */
    void PopBack(const s_uint& uiNbr)
    {
        if (uiNbr >= lArray_.size())
            lArray_.clear();
        else
        {
            for (s_uint ui; ui < uiNbr; ++ui)
                lArray_.pop_back();
        }
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

    /// Adds a set of values at the beginning of the container.
    /** \param lArray The array of value to add
    *   \note Order is preserved.
    */
    void PushFront(const s_ctnr& lArray)
    {
        typename s_ctnr::const_iterator iter = lArray.End();
        while (iter != lArray.Begin())
        {
            --iter;
            lArray_.push_front(*iter);
        }
    }

    /// Removes the first value of the container.
    void PopFront()
    {
        lArray_.pop_front();
    }

    /// Removes values from the beginning of the container.
    /** \param uiNbr The number of value to remove
    */
    void PopFront(const s_uint& uiNbr)
    {
        if (uiNbr >= lArray_.size())
            lArray_.clear();
        else
        {
            for (s_uint ui; ui < uiNbr; ++ui)
                lArray_.pop_front();
        }
    }

    /// Removes all values from this container.
    void Clear()
    {
        lArray_.clear();
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
        lArray_.resize(uiSize.Get(), mElem);
    }

    /// Searches the container for the provided element.
    /** \param mElem   The element to search for
    *   \param uiStart The position from which to start searching
    *   \return 'true' if the element has been found
    */
    s_bool Find(const T& mElem, const s_uint& uiStart = 0u) const
    {
        return (std::find(lArray_.begin() + uiStart.Get(), lArray_.end(), mElem) != lArray_.end());
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
        const_iterator iterPos = std::find(lArray_.begin() + uiStart.Get(), lArray_.end(), mElem);
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
        iterator iterStart = lArray_.begin() + uiStart.Get();
        return std::find(iterStart, lArray_.end(), mElem);
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
        const_iterator iterStart = lArray_.begin() + uiStart.Get();
        return std::find(iterStart, lArray_.end(), mElem);
    }

    /// Erases an element from the container.
    /** \param uiPos The position of the element to erase
    */
    void Erase(const s_uint& uiPos)
    {
        if (uiPos.IsValid() && uiPos < lArray_.size())
            lArray_.erase(lArray_.begin() + uiPos.Get());
    }

    /// Erases an element from the container.
    /** \param mIter The iterator pointing at the element you want to remove
    *   \return The iterator pointing at the next element in the container
    *   \note If the provided iterator was pointing to the last element, then
    *         End() is returned.
    */
    iterator Erase(const iterator& mIter)
    {
        return lArray_.erase(mIter);
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

    template<uint N>
    s_ctnr& operator = (const s_array<T,N>& lElemArray)
    {
        lArray_.clear();

        for (uint i = 0; i < N; ++i)
            lArray_.push_back(lElemArray[i]);
    }

    T& operator[] (const s_uint& uiIndex)
    {
        return lArray_[uiIndex.Get()];
    }

    const T& operator[] (const s_uint& uiIndex) const
    {
        return lArray_[uiIndex.Get()];
    }

    T& operator[] (const uint& uiIndex)
    {
        return lArray_[uiIndex];
    }

    const T& operator[] (const uint& uiIndex) const
    {
        return lArray_[uiIndex];
    }

    iterator begin()
    {
        return lArray_.begin();
    }

    const_iterator begin() const
    {
        return lArray_.begin();
    }

    iterator end()
    {
        return lArray_.end();
    }

    const_iterator end() const
    {
        return lArray_.end();
    }

    iterator Begin()
    {
        return lArray_.begin();
    }

    const_iterator Begin() const
    {
        return lArray_.begin();
    }

    iterator End()
    {
        return lArray_.end();
    }

    const_iterator End() const
    {
        return lArray_.end();
    }

private :

    std::deque<T> lArray_;
};
