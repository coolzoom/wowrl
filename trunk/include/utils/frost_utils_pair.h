// Warning : If you need to use this file, include frost_utils_types.h
namespace Frost
{
    /// Base type : pair
    /** Not much to say about this one : a typedef could have
    *   been enough, but its not possible in the current C++
    *   version (might be with C++0X).
    */
    template<class T, class N>
    class s_pair : public std::pair<T, N>
    {
    public :

        /// Default constructor.
        s_pair() : std::pair<T, N>()
        {
        }

        /// Constructor.
        /** \param mPair The pair to convert
        *   \note Allows full compatiblity with STL.
        */
        s_pair(const std::pair<T, N>& mPair) : std::pair<T, N>(mPair)
        {
        }
    };
}
