// Warning : If you need to use this file, include frost_utils_types.h
namespace Frost
{
    /// Base type : container (sorted associative)
    /** This class is basically a wrapper around std::multimap.
    *   It features a few more finding functions and some renamed ones.
    */
    template<class Key, class Data>
    class s_multimap : public std::multimap<Key, Data>
    {
    public :

        typedef typename std::multimap<Key, Data>::iterator       iterator;
        typedef typename std::multimap<Key, Data>::const_iterator const_iterator;
        typedef          s_range<iterator>                        range;
        typedef          s_range<const_iterator>                  const_range;
        typedef          s_pair<Key, Data>                        pair;

        /// Default constructor.
        s_multimap()
        {
        }

        /// Erases an element from the multimap.
        /** \param mKey The key of the element to erase
        */
        void Erase(const Key& mKey)
        {
            std::multimap<Key, Data>::erase(mKey);
        }

        /// Erases an element from the multimap.
        /** \param mIter An iterator pointing at the element to erase
        */
        void Erase(const iterator& mIter)
        {
            std::multimap<Key, Data>::erase(mIter);
        }

        /// Erases a range of elements from the multimap.
        /** \param mBegin The start of the range
        *   \param mEnd   The ending of the range
        */
        void Erase(const iterator& mBegin, const iterator& mEnd)
        {
            std::multimap<Key, Data>::erase(mBegin, mEnd);
        }

        /// Erases a range of elements from the multimap.
        /** \param mRange The range to erase
        */
        void Erase(const range& mRange)
        {
            std::multimap<Key, Data>::erase(mRange.Begin(), mRange.End());
        }

        /// Searches the multimap for the provided key.
        /** \param mKey The key to search for
        *   \return 'true' if the key has been found
        */
        s_bool Find(const Key& mKey) const
        {
            return std::multimap<Key, Data>::find(mKey) != End();
        }

        /// Searches the multimap for the provided key.
        /** \param mKey The key to search for
        *   \return The position of the key in the multimap
        *   \note If the key is not present in the multimap, this function
        *         returns s_uint::NaN.
        */
        s_uint FindPos(const Key& mKey) const
        {
            typename std::multimap<Key, Data>::const_iterator mIter;
            mIter = std::multimap<Key, Data>::find(mKey);
            if (mIter == End())
                return s_uint::NaN;
            else
                return mIter - Begin();
        }

        /// Searches the multimap for the provided key.
        /** \param mKey The key to search for
        *   \return An iterator pointing to the associated pair
        *   \note If the key is not present in the multimap, this function
        *         returns End().
        */
        iterator FindIter(const Key& mKey)
        {
            return std::multimap<Key, Data>::find(mKey);
        }

        /// Searches the multimap for the provided key.
        /** \param mKey The key to search for
        *   \return An iterator pointing to the associated pair
        *   \note If the key is not present in the multimap, this function
        *         returns End().
        */
        const_iterator FindIter(const Key& mKey) const
        {
            return std::multimap<Key, Data>::find(mKey);
        }

        /// Returns one of the surrounding pairs.
        /** \param mKey The key to search for
        *   \return An iterator pointing to one of the surrounding pairs
        *   \note You are searching for mKey, but it's proably not
        *         in this multimap. But there are two keys in the multimap that
        *         surround mKey : mLower and mUpper.<br>
        *         This function will always return mUpper, unless mKey
        *         equals mLower : it will then return mLower.
        */
        iterator LowerBound(const Key& mKey)
        {
            return std::multimap<Key, Data>::lower_bound(mKey);
        }

        /// Returns one of the surrounding pairs.
        /** \param mKey The key to search for
        *   \return An iterator pointing to one of the surrounding pairs
        *   \note You are searching for mKey, but it's proably not
        *         in this multimap. But there are two keys in the multimap that
        *         surround mKey : mLower and mUpper.<br>
        *         This function will always return mUpper, unless mKey
        *         equals mLower : it will then return mLower.
        */
        const_iterator LowerBound(const Key& mKey) const
        {
            return std::multimap<Key, Data>::lower_bound(mKey);
        }

        /// Returns one of the surrounding pairs.
        /** \param mKey The key to search for
        *   \return An iterator pointing to one of the surrounding pairs
        *   \note You are searching for mKey, but it's proably not
        *         in this multimap. But there are two keys in the multimap that
        *         surround mKey : mLower and mUpper.<br>
        *         This function will always return mUpper, unless mKey
        *         equals mUpper : it will then return mUpper+1.
        */
        iterator UpperBound(const Key& mKey)
        {
            return std::multimap<Key, Data>::upper_bound(mKey);
        }

        /// Returns one of the surrounding pairs.
        /** \param mKey The key to search for
        *   \return An iterator pointing to one of the surrounding pairs
        *   \note You are searching for mKey, but it's proably not
        *         in this multimap. But there are two keys in the multimap that
        *         surround mKey : mLower and mUpper.<br>
        *         This function will always return mUpper, unless mKey
        *         equals mUpper : it will then return mUpper+1.
        */
        const_iterator UpperBound(const Key& mKey) const
        {
            return std::multimap<Key, Data>::upper_bound(mKey);
        }

        /// Returns the number of elements whith the provided key.
        /** \param mKey The key to search for
        *   \return The number of elements whith the provided key
        */
        s_uint Count(const Key& mKey) const
        {
            return std::multimap<Key, Data>::count(mKey);
        }

        /// Returns the range of all elements with the provided key.
        /** \param mKey The key to search for
        *   \return The range of all elements with the provided key
        */
        range EqualRange(const Key& mKey)
        {
            return std::multimap<Key, Data>::equal_range(mKey);
        }

        /// Returns the range of all elements with the provided key.
        /** \param mKey The key to search for
        *   \return The range of all elements with the provided key
        */
        const_range EqualRange(const Key& mKey) const
        {
            return std::multimap<Key, Data>::equal_range(mKey);
        }

        /// Inserts a pair in the multimap.
        /** \param mPair The pair to insert
        *   \return An iterator pointing to the inserted pair
        *   \note The multimap automatically sorts itself after
        *         this operation.
        */
        iterator Insert(const pair& mPair)
        {
            return std::multimap<Key, Data>::insert(mPair);
        }

        /// Inserts a pair in the multimap (with hint).
        /** \param mIter A hint to where it should be inserted
        *   \param mPair The pair to insert
        *   \return An iterator pointing to the inserted pair
        *   \note The multimap automatically sorts itself after
        *         this operation.
        */
        iterator Insert(const iterator& mIter, const pair& mPair)
        {
            return std::multimap<Key, Data>::insert(mIter, mPair);
        }

        /// Removes all elements from this multimap.
        void Clear()
        {
            std::multimap<Key, Data>::clear();
        }

        /// Returns the number of elements in this multimap.
        /** \return The number of elements in this multimap
        */
        s_uint GetSize() const
        {
            return std::multimap<Key, Data>::size();
        }

        /// Checks if this multimap is empty.
        /** \return 'true' if this multimap is empty
        */
        s_bool IsEmpty() const
        {
            return std::multimap<Key, Data>::empty();
        }

        s_ctnr<s_multimap> operator, (const s_multimap& lValue)
        {
            s_ctnr<s_multimap> mContainer;
            mContainer.PushBack(*this);
            mContainer.PushBack(lValue);
            return mContainer;
        }

        iterator Begin()
        {
            return std::multimap<Key, Data>::begin();
        }

        const_iterator Begin() const
        {
            return std::multimap<Key, Data>::begin();
        }

        iterator End()
        {
            return std::multimap<Key, Data>::end();
        }

        const_iterator End() const
        {
            return std::multimap<Key, Data>::end();
        }

    };

    template<class Key, class Data, class N>
    s_str_t<N> operator + (const s_str_t<N>& sLeft, const s_multimap<Key, Data>& mRight)
    {
        s_str_t<N> sTemp = STRING("( ");
        typename std::multimap<Key, Data>::const_iterator iter;
        for (iter = mRight.Begin(); iter != mRight.End(); ++iter)
        {
            if (iter - mRight.Begin() == mRight.GetSize()-1)
                sTemp << "[" << iter->first << "] = " << iter->second;
            else
                sTemp << "[" << iter->first << "] = " << iter->second << STRING(", ");
        }
        sTemp << STRING(" )");

        return sLeft + sTemp;
    }

    template<class Key, class Data, class N>
    s_str_t<N> operator + (const N* sLeft, const s_multimap<Key, Data>& mRight)
    {
        return s_str_t<N>(sLeft) + mRight;
    }

    #ifdef USE_UNICODE
        template<class Key, class Data>
        s_str_t<char> operator + (const char* sLeft, const s_multimap<Key, Data>& mRight)
        {
            return s_str_t<char>(sLeft) + mRight;
        }
    #endif
}
