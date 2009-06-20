// Warning : If you need to use this file, include frost_utils_types.h
namespace Frost
{
    /// Base type : container (sorted associative)
    /** This class is basically a wrapper around std::map.
    *   It features a few more finding functions and some renamed ones.
    */
    template<class Key, class Data>
    class s_map : public std::map<Key, Data>
    {
    public :

        typedef typename std::map<Key, Data>::iterator       iterator;
        typedef typename std::map<Key, Data>::const_iterator const_iterator;
        typedef          s_range<iterator>                   range;
        typedef          s_range<const_iterator>             const_range;
        typedef          s_pair<Key, Data>                   pair;

        /// Default constructor.
        s_map()
        {
        }

        /// Erases an element from the map.
        /** \param mKey The key of the element to erase
        */
        void Erase(const Key& mKey)
        {
            std::map<Key, Data>::erase(mKey);
        }

        /// Erases an element from the map.
        /** \param mIter An iterator pointing at the element to erase
        */
        void Erase(const iterator& mIter)
        {
            std::map<Key, Data>::erase(mIter);
        }

        /// Erases a range of elements from the map.
        /** \param mBegin The start of the range
        *   \param mEnd   The ending of the range
        */
        void Erase(const iterator& mBegin, const iterator& mEnd)
        {
            std::map<Key, Data>::erase(mBegin, mEnd);
        }

        /// Erases a range of elements from the map.
        /** \param mRange The range to erase
        */
        void Erase(const range& mRange)
        {
            std::map<Key, Data>::erase(mRange.Begin(), mRange.End());
        }

        /// Searches the map for the provided key.
        /** \param mKey The key to search for
        *   \return 'true' if the key has been found
        */
        s_bool Find(const Key& mKey) const
        {
            return std::map<Key, Data>::find(mKey) != End();
        }

        /// Searches the map for the provided key.
        /** \param mKey The key to search for
        *   \return The position of the key in the map
        *   \note If the key is not present in the map, this function
        *         returns s_uint::NaN.
        */
        s_uint FindPos(const Key& mKey) const
        {
            typename std::map<Key, Data>::const_iterator mIter;
            mIter = std::map<Key, Data>::find(mKey);
            if (mIter == End())
                return s_uint::NaN;
            else
                return mIter - Begin();
        }

        /// Searches the map for the provided key.
        /** \param mKey The key to search for
        *   \return An iterator pointing to the associated pair
        *   \note If the key is not present in the map, this function
        *         returns End().
        */
        iterator FindIter(const Key& mKey)
        {
            return std::map<Key, Data>::find(mKey);
        }

        /// Searches the map for the provided key.
        /** \param mKey The key to search for
        *   \return An iterator pointing to the associated pair
        *   \note If the key is not present in the map, this function
        *         returns End().
        */
        const_iterator FindIter(const Key& mKey) const
        {
            return std::map<Key, Data>::find(mKey);
        }

        /// Returns one of the surrounding pairs.
        /** \param mKey The key to search for
        *   \return An iterator pointing to one of the surrounding pairs
        *   \note You are searching for mKey, but it's proably not
        *         in this map. But there are two keys in the map that
        *         surround mKey : mLower and mUpper.<br>
        *         This function will always return mUpper, unless mKey
        *         equals mLower : it will then return mLower.
        */
        iterator LowerBound(const Key& mKey)
        {
            return std::map<Key, Data>::lower_bound(mKey);
        }

        /// Returns one of the surrounding pairs.
        /** \param mKey The key to search for
        *   \return An iterator pointing to one of the surrounding pairs
        *   \note You are searching for mKey, but it's proably not
        *         in this map. But there are two keys in the map that
        *         surround mKey : mLower and mUpper.<br>
        *         This function will always return mUpper, unless mKey
        *         equals mLower : it will then return mLower.
        */
        const_iterator LowerBound(const Key& mKey) const
        {
            return std::map<Key, Data>::lower_bound(mKey);
        }

        /// Returns one of the surrounding pairs.
        /** \param mKey The key to search for
        *   \return An iterator pointing to one of the surrounding pairs
        *   \note You are searching for mKey, but it's proably not
        *         in this map. But there are two keys in the map that
        *         surround mKey : mLower and mUpper.<br>
        *         This function will always return mUpper, unless mKey
        *         equals mUpper : it will then return mUpper+1.
        */
        iterator UpperBound(const Key& mKey)
        {
            return std::map<Key, Data>::upper_bound(mKey);
        }

        /// Returns one of the surrounding pairs.
        /** \param mKey The key to search for
        *   \return An iterator pointing to one of the surrounding pairs
        *   \note You are searching for mKey, but it's proably not
        *         in this map. But there are two keys in the map that
        *         surround mKey : mLower and mUpper.<br>
        *         This function will always return mUpper, unless mKey
        *         equals mUpper : it will then return mUpper+1.
        */
        const_iterator UpperBound(const Key& mKey) const
        {
            return std::map<Key, Data>::upper_bound(mKey);
        }

        /// Removes all elements from this map.
        void Clear()
        {
            std::map<Key, Data>::clear();
        }

        /// Returns the number of elements in this map.
        /** \return The number of elements in this map
        */
        s_uint GetSize() const
        {
            return std::map<Key, Data>::size();
        }

        /// Checks if this map is empty.
        /** \return 'true' if this map is empty
        */
        s_bool IsEmpty() const
        {
            return std::map<Key, Data>::empty();
        }

        s_ctnr<s_map> operator, (const s_map& lValue)
        {
            s_ctnr<s_map> mContainer;
            mContainer.PushBack(*this);
            mContainer.PushBack(lValue);
            return mContainer;
        }

        iterator Begin()
        {
            return std::map<Key, Data>::begin();
        }

        const_iterator Begin() const
        {
            return std::map<Key, Data>::begin();
        }

        iterator End()
        {
            return std::map<Key, Data>::end();
        }

        const_iterator End() const
        {
            return std::map<Key, Data>::end();
        }

    };

    template<class Key, class Data, class N>
    s_str_t<N> operator + (const s_str_t<N>& sLeft, const s_map<Key, Data>& mRight)
    {
        s_str_t<N> sTemp = STRING("( ");
        typename std::map<Key, Data>::const_iterator iter;
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
    s_str_t<N> operator + (const N* sLeft, const s_map<Key, Data>& mRight)
    {
        return s_str_t<N>(sLeft) + mRight;
    }

    #ifdef USE_UNICODE
        template<class Key, class Data>
        s_str_t<char> operator + (const char* sLeft, const s_map<Key, Data>& mRight)
        {
            return s_str_t<char>(sLeft) + mRight;
        }
    #endif
}
