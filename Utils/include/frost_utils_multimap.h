// Warning : If you need to use this file, include frost_utils_types.h
#include <map>

namespace Frost
{
    /// Base type : container (sorted associative)
    /** This class is basically a wrapper around std::multimap.
    *   It features a few more finding functions and some renamed ones.
    */
    template<class Key, class Value>
    class s_multimap
    {
    public :

        typedef typename std::multimap<Key, Value>::iterator       iterator;
        typedef typename std::multimap<Key, Value>::const_iterator const_iterator;
        typedef          s_range<iterator>                         range;
        typedef          s_range<const_iterator>                   const_range;
        typedef          s_pair<Key, Value>                        pair;

        /// Default constructor.
        s_multimap()
        {
        }

        #ifdef CPP_0X
            /// Initializer list constructor.
            /** \param mList Brace enclosed pair list
            *   \note This constructor uses a C++0x feature.<br>
            *         It allows : s_multimap<s_int, s_str> v = {MakePair(1, "bar"), MakePair(4, "foo")};
            */
            s_multimap(std::initializer_list<pair> mList)
            {
                for (const pair* p = mList.begin(); p != mList.end(); ++p)
                    mMulMap_.insert(std::make_pair(p->First(), p->Second()));
            }
        #endif

        /// Erases an element from the multimap.
        /** \param mKey The key of the element to erase
        */
        void Erase(const Key& mKey)
        {
            mMulMap_.erase(mKey);
        }

        /// Erases an element from the multimap.
        /** \param mIter An iterator pointing at the element to erase
        */
        void Erase(const iterator& mIter)
        {
            mMulMap_.erase(mIter);
        }

        /// Erases a range of elements from the multimap.
        /** \param mBegin The start of the range
        *   \param mEnd   The ending of the range
        */
        void Erase(const iterator& mBegin, const iterator& mEnd)
        {
            mMulMap_.erase(mBegin, mEnd);
        }

        /// Erases a range of elements from the multimap.
        /** \param mRange The range to erase
        */
        void Erase(const range& mRange)
        {
            mMulMap_.erase(mRange.Begin(), mRange.End());
        }

        /// Searches the multimap for the provided key.
        /** \param mKey The key to search for
        *   \return 'true' if the key has been found
        */
        s_bool Find(const Key& mKey) const
        {
            return (mMulMap_.find(mKey) != End());
        }

        /// Searches the multimap for the provided key.
        /** \param mKey The key to search for
        *   \return The position of the key in the multimap
        *   \note If the key is not present in the multimap, this function
        *         returns s_uint::NaN.
        */
        s_uint_t<default_uint> FindPos(const Key& mKey) const
        {
            const_iterator mIter = mMulMap_.find(mKey);
            if (mIter == End())
                return s_uint_t<default_uint>::NaN;
            else
                return mIter - Begin();
        }

        /// Searches the multimap for the provided key.
        /** \param mKey The key to search for
        *   \return An iterator pointing to the associated pair
        *   \note If the key is not present in the multimap, this function
        *         returns End().
        */
        iterator Get(const Key& mKey)
        {
            return mMulMap_.find(mKey);
        }

        /// Searches the multimap for the provided key.
        /** \param mKey The key to search for
        *   \return An iterator pointing to the associated pair
        *   \note If the key is not present in the multimap, this function
        *         returns End().
        */
        const_iterator Get(const Key& mKey) const
        {
            return mMulMap_.find(mKey);
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
            return mMulMap_.lower_bound(mKey);
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
            return mMulMap_.lower_bound(mKey);
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
            return mMulMap_.upper_bound(mKey);
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
            return mMulMap_.upper_bound(mKey);
        }

        /// Returns the number of elements whith the provided key.
        /** \param mKey The key to search for
        *   \return The number of elements whith the provided key
        */
        s_uint_t<default_uint> Count(const Key& mKey) const
        {
            return mMulMap_.count(mKey);
        }

        /// Returns the range of all elements with the provided key.
        /** \param mKey The key to search for
        *   \return The range of all elements with the provided key
        */
        range EqualRange(const Key& mKey)
        {
            return mMulMap_.equal_range(mKey);
        }

        /// Returns the range of all elements with the provided key.
        /** \param mKey The key to search for
        *   \return The range of all elements with the provided key
        */
        const_range EqualRange(const Key& mKey) const
        {
            return mMulMap_.equal_range(mKey);
        }

        /// Inserts a pair in the multimap.
        /** \param mPair The pair to insert
        *   \return An iterator pointing to the inserted pair
        *   \note The multimap automatically sorts itself after
        *         this operation.
        */
        iterator Insert(const pair& mPair)
        {
            return mMulMap_.insert(std::make_pair(mPair.First(), mPair.Second()));
        }

        /// Inserts a pair in the multimap.
        /** \param mKey   The access key
        *   \param mValue The value to store
        *   \return An iterator pointing to the inserted pair
        *   \note The multimap automatically sorts itself after
        *         this operation.
        */
        iterator Insert(const Key& mKey, const Value& mValue)
        {
            return mMulMap_.insert(std::make_pair(mKey, mValue));
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
            return mMulMap_.insert(mIter, make_pair(mPair.First(), mPair.Second()));
        }

        /// Inserts a pair in the multimap (with hint).
        /** \param mIter A hint to where it should be inserted
        *   \param mKey   The access key
        *   \param mValue The value to store
        *   \return An iterator pointing to the inserted pair
        *   \note The multimap automatically sorts itself after
        *         this operation.
        */
        iterator Insert(const iterator& mIter, const Key& mKey, const Value& mValue)
        {
            return mMulMap_.insert(mIter, make_pair(mKey, mValue));
        }

        /// Removes all elements from this multimap.
        void Clear()
        {
            mMulMap_.clear();
        }

        /// Returns the number of elements in this multimap.
        /** \return The number of elements in this multimap
        */
        s_uint_t<default_uint> GetSize() const
        {
            return mMulMap_.size();
        }

        /// Checks if this multimap is empty.
        /** \return 'true' if this multimap is empty
        */
        s_bool IsEmpty() const
        {
            return mMulMap_.empty();
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
            return mMulMap_.begin();
        }

        const_iterator Begin() const
        {
            return mMulMap_.begin();
        }

        iterator End()
        {
            return mMulMap_.end();
        }

        const_iterator End() const
        {
            return mMulMap_.end();
        }

        iterator begin()
        {
            return mMulMap_.begin();
        }

        const_iterator begin() const
        {
            return mMulMap_.begin();
        }

        iterator end()
        {
            return mMulMap_.end();
        }

        const_iterator end() const
        {
            return mMulMap_.end();
        }

    private :

        std::multimap<Key, Value> mMulMap_;

    };

    /** \cond NOT_REMOVE_FROM_DOC
    */
    template<class Key, class Value> class StringConverter< string_element, s_multimap<Key, Value> >
    {
    public :

        typedef s_str::string string;

        static string Convert(const s_multimap<Key, Value>& mMap)
        {
            string sTemp = "(";
            typename s_multimap<Key, Value>::const_iterator iter;
            for (iter = mMap.Begin(); iter != mMap.End(); ++iter)
            {
                if (iter != mMap.Begin())
                    sTemp += ", [" + StringConverter<string_element, Key>::Convert(iter->first)
                          + "] = " + StringConverter<string_element, Value>::Convert(iter->second);
                else
                    sTemp +=   "[" + StringConverter<string_element, Key>::Convert(iter->first)
                          + "] = " + StringConverter<string_element, Value>::Convert(iter->second);
            }
            sTemp += ")";

            return sTemp;
        }
    };
    /** \endcond
    */
}
