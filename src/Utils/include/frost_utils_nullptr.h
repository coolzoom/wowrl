// Warning : If you need to use this file, include frost_utils_types.h
namespace Frost
{
#ifdef HAS_NULLPTR
    typedef std::nullptr_t s_nullptr;
#else
    /// Null pointer
    /** A safe replacement for the NULL macro.
    */
    class s_nullptr
    {
    public :

        /// Default constructor.
        s_nullptr()
        {
        }

        /// Allows : "if (!pPointer)".
        bool operator ! () const
        {
            return true;
        }

        typedef void (s_nullptr::*MemberFn)();
        /// Allows : "if (pPointer)".
        operator MemberFn() const
        {
            return 0;
        }

        /// Allows usage with raw pointers.
        template<class N>
        operator N*() const
        {
            return 0;
        }
    };

    const s_nullptr nullptr;
#endif
}
