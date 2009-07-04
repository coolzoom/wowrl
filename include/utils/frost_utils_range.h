// Warning : If you need to use this file, include frost_utils_types.h
namespace Frost
{
    /// Base type : range
    /** This class is a special case of s_pair.<br>
    *   It should only contain iterators, but you can use it
    *   with other types if you know what you're doing.<br>
    *   <b>Note</b> : can be used in the "foreach" macro.
    */
    template<class T>
    class s_range
    {
    public :

        /// Default constructor.
        s_range()
        {
        }

        /// Constructor.
        /** \param mPair The pair to convert
        */
        s_range(const s_pair<T, T>& mPair) : mPair_(mPair)
        {
        }

        /// Constructor.
        /** \param mPair The pair to convert
        *   \note Allows full compatiblity with STL.
        */
        s_range(const std::pair<T, T>& mPair) : mPair_(mPair)
        {
        }

        const T& Begin() const
        {
            return mPair_.First();
        }

        const T& End() const
        {
            return mPair_.Second();
        }

        const T& begin() const
        {
            return mPair_.First();
        }

        const T& end() const
        {
            return mPair_.Second();
        }

    private :

        s_pair<T, T> mPair_;

    };
}
