// Warning : If you need to use this file, include frost_utils_types.h
namespace Frost
{
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

        static const s_str CLASS_NAME;

    };

    const s_str     s_nullptr::CLASS_NAME = "s_nullptr";
    const s_nullptr nullptr;
}
