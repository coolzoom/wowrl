// Warning : If you need to use this file, include frost_utils_types.h
namespace Frost
{
    /// Base type : container (sorted associative)
    /** This class is basically a wrapper around std::map.
    *   It features a few more finding functions and some renamed ones.
    */
    template<class Key, class Value>
    class s_map
    {
    public :

        typedef typename std::map<Key, Value>::iterator       iterator;
        typedef typename std::map<Key, Value>::const_iterator const_iterator;
        typedef          s_range<iterator>                    range;
        typedef          s_range<const_iterator>              const_range;
        typedef          s_pair<Key, Value>                   pair;

        /// Default constructor.
        s_map()
        {
        }

        #ifdef CPP_0X
            /// Initializer list constructor.
            /** \param mList Brace enclosed pair list
            *   \note This constructor uses a C++0x feature.<br>
            *         It allows : s_map<s_int, s_str> v = {MakePair(1, "bar"), MakePair(4, "foo")};
            */
            s_map(std::initializer_list<pair> mList)
            {
                for (const pair* p = mList.begin(); p != mList.end(); ++p)
                    mMap_.insert(std::make_pair(p->First(), p->Second()));
            }
        #endif

        /// Erases an element from the map.
        /** \param mKey The key of the element to erase
        */
        void Erase(const Key& mKey)
        {
            mMap_.erase(mKey);
        }

        /// Erases an element from the map.
        /** \param mIter An iterator pointing at the element to erase
        */
        void Erase(const iterator& mIter)
        {
            mMap_.erase(mIter);
        }

        /// Erases a range of elements from the map.
        /** \param mBegin The start of the range
        *   \param mEnd   The ending of the range
        */
        void Erase(const iterator& mBegin, const iterator& mEnd)
        {
            mMap_.erase(mBegin, mEnd);
        }

        /// Erases a range of elements from the map.
        /** \param mRange The range to erase
        */
        void Erase(const range& mRange)
        {
            mMap_.erase(mRange.Begin(), mRange.End());
        }

        /// Searches the map for the provided key.
        /** \param mKey The key to search for
        *   \return 'true' if the key has been found
        */
        s_bool Find(const Key& mKey) const
        {
            return (mMap_.find(mKey) != End());
        }

        /// Searches the map for the provided key.
        /** \param mKey The key to search for
        *   \return The position of the key in the map
        *   \note If the key is not present in the map, this function
        *         returns s_uint::NaN.
        */
        s_uint_t<default_uint> FindPos(const Key& mKey) const
        {
            const_iterator mIter;
            mIter = mMap_.find(mKey);
            if (mIter == End())
                return s_uint_t<default_uint>::NaN;
            else
                return mIter - Begin();
        }

        /// Searches the map for the provided key.
        /** \param mKey The key to search for
        *   \return An iterator pointing to the associated pair
        *   \note If the key is not present in the map, this function
        *         returns End().
        */
        iterator Get(const Key& mKey)
        {
            return mMap_.find(mKey);
        }

        /// Searches the map for the provided key.
        /** \param mKey The key to search for
        *   \return An iterator pointing to the associated pair
        *   \note If the key is not present in the map, this function
        *         returns End().
        */
        const_iterator Get(const Key& mKey) const
        {
            return mMap_.find(mKey);
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
            return mMap_.lower_bound(mKey);
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
            return mMap_.lower_bound(mKey);
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
            return mMap_.upper_bound(mKey);
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
            return mMap_.upper_bound(mKey);
        }

        /// Removes all elements from this map.
        void Clear()
        {
            mMap_.clear();
        }

        /// Returns the number of elements in this map.
        /** \return The number of elements in this map
        */
        s_uint_t<default_uint> GetSize() const
        {
            return mMap_.size();
        }

        /// Checks if this map is empty.
        /** \return 'true' if this map is empty
        */
        s_bool IsEmpty() const
        {
            return mMap_.empty();
        }

        Value& operator [] (const Key& mKey)
        {
            return mMap_[mKey];
        }

        const Value& operator [] (const Key& mKey) const
        {
            return mMap_[mKey];
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
            return mMap_.begin();
        }

        const_iterator Begin() const
        {
            return mMap_.begin();
        }

        iterator End()
        {
            return mMap_.end();
        }

        const_iterator End() const
        {
            return mMap_.end();
        }

        iterator begin()
        {
            return mMap_.begin();
        }

        const_iterator begin() const
        {
            return mMap_.begin();
        }

        iterator end()
        {
            return mMap_.end();
        }

        const_iterator end() const
        {
            return mMap_.end();
        }

    private :

        std::map<Key, Value> mMap_;

    };

    template<class Key, class Value, class N>
    s_str_t<N> operator + (const s_str_t<N>& sLeft, const s_map<Key, Value>& mRight)
    {
        s_str_t<N> sTemp = "( ";
        typename s_map<Key, Value>::const_iterator iter;
        for (iter = mRight.Begin(); iter != mRight.End(); ++iter)
        {
            if (iter != mRight.Begin() )
                sTemp << ", [" << iter->first << "] = " << iter->second;
            else
                sTemp << "[" << iter->first << "] = " << iter->second;
        }
        sTemp << " )";

        return sLeft + sTemp;
    }

    template<class Key, class Value, class N>
    s_str_t<N> operator + (const N* sLeft, const s_map<Key, Value>& mRight)
    {
        return s_str_t<N>(sLeft) + mRight;
    }
}
