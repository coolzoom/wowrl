// Warning : If you need to use this file, include frost_utils_types.h
namespace Frost
{
    /// Base type : boolean
    /** Frost's base types are made to allow simpler
    *   manipulation of numbers, booleans and strings.
    *   They simplify conversions (mostly to string),
    *   allow easy access to math/string functions and
    *   implements new functions that are not present
    *   in the standard libraries.
    *   \note This class doesn't need to be a template,
    *         as there is only one boolean type in C++.
    *         But it is delays the compilation of the
    *         class.
    */
    template<class T>
    class s_bool_t
    {
    public :

        s_bool_t();

        s_bool_t(const T& bValue);

        template<class N>
        explicit s_bool_t(const s_str_t<N>& sValue)
        {
            bValue_ = ((sValue == BOOL_TRUE) ||
                       (sValue == BOOL_ONE)  ||
                       (sValue == BOOL_YES));
        }

        /// Returns a const reference to the boolean.
        /** \return A const reference to the boolean
        */
        inline const T& Get() const { return bValue_; }

        /// Returns a reference to the boolean.
        /** \return A reference to the boolean
        */
        inline T& GetR() { return bValue_; }

        bool operator ! () const;

        typedef const bool& (s_bool_t::*MemberFn)() const;
        operator MemberFn() const;

        s_bool_t operator == (const s_bool_t& bValue) const;

        s_bool_t operator != (const s_bool_t& bValue) const;

        s_bool_t& operator =  (const s_bool_t& bValue);

        static const string_element* BOOL_TRUE;
        static const string_element* BOOL_ONE;
        static const string_element* BOOL_YES;
        static T                     bDummy;

        /// Returns a random boolean.
        /** \return Either 'true' or 'false' (50% each).
        */
        static s_bool_t Random();

    private :

        T bValue_;
    };

    typedef s_bool_t<bool> s_bool;

    /** \cond NOT_REMOVE_FROM_DOC
    */
    template<> class TypeTraits<bool>
    {
    public :
        typedef bool        Type;
        typedef bool        BaseType;
        typedef s_bool      FrostType;
        typedef bool&       RefType;
        typedef const bool& CRefType;
        typedef bool*       PointerType;

        static inline RefType  GetValue(RefType  m) { return m; }
        static inline CRefType GetValue(CRefType m) { return m; }
    };

    template<> class TypeTraits<s_bool>
    {
    public :
        typedef s_bool        Type;
        typedef bool          BaseType;
        typedef s_bool        FrostType;
        typedef s_bool&       RefType;
        typedef const s_bool& CRefType;
        typedef s_bool*       PointerType;

        static inline bool&       GetValue(RefType m)  { return m.GetR(); }
        static inline const bool& GetValue(CRefType m) { return m.Get(); }
    };
    /** \endcond
    */
}
