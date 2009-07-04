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

        operator const std::pair<T,N>& ()
        {
            return mPair_;
        }

        const T& First() const
        {
            return mPair_.first;
        }

        const N& Second() const
        {
            return mPair_.second;
        }

    private :

        std::pair<T, N> mPair_;
    };

    template<class T, class N>
    s_pair<T, N> MakePair(const T& mFirst, const N& mSecond)
    {
        return s_pair<T, N>(mFirst, mSecond);
    }
}
