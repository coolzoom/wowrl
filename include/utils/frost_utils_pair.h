// Warning : If you need to use this file, include frost_utils_types.h
namespace Frost
{
    /// Base type : pair
    /** Not much to say about this one : a typedef could have
    *   been enough, but its not possible in the current C++
    *   version (might be with C++0X).
    */
    template<class T, class N>
    class s_pair
    {
    public :

        /// Default constructor.
        s_pair()
        {
        }

        /// Constructor.
        /** \param mFirst  The first element of the pair
        *   \param mSecond The second element of the pair
        */
        s_pair(const T& mFirst, const N& mSecond)
        {
            mPair_.first = mFirst; mPair_.second = mSecond;
        }

        /// Conversion constructor.
        /** \param mPair The pair to convert
        *   \note Allows full compatiblity with STL.
        */
        s_pair(const std::pair<T, N>& mPair) : mPair_(mPair)
        {
        }

        const T& First() const
        {
            return mPair_.first;
        }

        const N& Second() const
        {
            return mPair_.second;
        }

        T& First()
        {
            return mPair_.first;
        }

        N& Second()
        {
            return mPair_.second;
        }

    private :

        std::pair<T, N> mPair_;
    };

    // Frost's version of std::make_pair.
    // This function automatically converts PODs to Frost's types,
    // so writing :
    //      MakePair(1, 1u)
    // ... creates a s_pair<s_int, s_uint> rather than a s_pair<int, uint>, and :
    //      MakePair(1, "bla")
    // ... creates a s_pair<s_int, s_str> rather than throwing a compiler error,
    // as std::make_pair does if your implementation is standard compliant.
    // See : http://www.open-std.org/jtc1/sc22/wg21/docs/lwg-defects.html#181
    template<class T, class N>
    s_pair<typename TypeTraits<T>::Type, typename TypeTraits<N>::Type> MakePair(const T& mFirst, const N& mSecond)
    {
        return s_pair<typename TypeTraits<T>::Type, typename TypeTraits<N>::Type>(mFirst, mSecond);
    }

    template<class N>
    s_pair<s_str, typename TypeTraits<N>::Type> MakePair(const char* mFirst, const N& mSecond)
    {
        return s_pair<s_str, typename TypeTraits<N>::Type>(mFirst, mSecond);
    }

    template<class T>
    s_pair<typename TypeTraits<T>::Type, s_str> MakePair(const T& mFirst, const char* mSecond)
    {
        return s_pair<typename TypeTraits<T>::Type, s_str>(mFirst, mSecond);
    }
}
