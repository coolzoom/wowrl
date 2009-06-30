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
    class s_range : public std::pair<T, T>
    {
    public :

        /// Default constructor.
        s_range() : std::pair<T, T>()
        {
        }

        /// Constructor.
        /** \param mPair The pair to convert
        *   \note Allows full compatiblity with STL.
        */
        s_range(const std::pair<T, T>& mPair) : std::pair<T, T>(mPair)
        {
        }

        const T& begin()
        {
            return std::pair<T, T>::first;
        }

        const T& end()
        {
            return std::pair<T, T>::second;
        }

        const T& Begin()
        {
            return std::pair<T, T>::first;
        }

        const T& End()
        {
            return std::pair<T, T>::second;
        }
    };
}
