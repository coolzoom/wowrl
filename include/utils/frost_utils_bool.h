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
    */
    template<class T>
    class s_bool_t
    {
    public :

        s_bool_t()
        {
            bValue_ = false;
        }

        s_bool_t(const T& bValue)
        {
            bValue_ = bValue;
        }

        template<class N>
        explicit s_bool_t(const N* sValue)
        {
            if ((s_str_t<N>(sValue) == BOOL_TRUE) ||
                (s_str_t<N>(sValue) == BOOL_ONE)  ||
                (s_str_t<N>(sValue) == BOOL_YES))
                bValue_ = true;
            else
                bValue_ = false;
        }

        explicit s_bool_t(const string_object& sValue)
        {
            if ((sValue == BOOL_TRUE) ||
                (sValue == BOOL_ONE)  ||
                (sValue == BOOL_YES))
                bValue_ = true;
            else
                bValue_ = false;
        }

        template<class N>
        explicit s_bool_t(const s_str_t<N>& sValue)
        {
            if ((sValue == BOOL_TRUE) ||
                (sValue == BOOL_ONE)  ||
                (sValue == BOOL_YES))
                bValue_ = true;
            else
                bValue_ = false;
        }

        /// Returns a const reference to the boolean.
        /** \return A const reference to the boolean
        */
        inline const T& Get() const
        {
            return bValue_;
        }

        /// Returns a reference to the boolean.
        /** \return A reference to the boolean
        */
        inline T& GetR()
        {
            return bValue_;
        }

        bool operator ! () const
        {
            return !bValue_;
        }

        typedef const bool& (s_bool_t::*MemberFn)() const;
        operator MemberFn() const
        {
            if (bValue_)
                return &s_bool_t::Get;
            else
                return NULL;
        }

        s_bool_t operator == (const s_bool_t& bValue) const
        {
            return (bValue_ == bValue.bValue_);
        }

        s_bool_t operator != (const s_bool_t& bValue) const
        {
            return (bValue_ != bValue.bValue_);
        }

        template<class N>
        s_str_t<N> operator + (const N* sValue) const
        {
            return s_str_t<N>(*this) + sValue;
        }

        template<class N>
        s_str_t<N> operator + (const s_str_t<N>& sValue) const
        {
            return s_str_t<N>(*this) + sValue;
        }

        s_bool_t& operator =  (const s_bool_t& bValue)
        {
            bValue_ = bValue.bValue_;
            return *this;
        }

        s_ctnr<s_bool_t> operator , (const s_bool_t& bValue) const
        {
            s_ctnr<s_bool_t> mContainer;
            mContainer.PushBack(*this);
            mContainer.PushBack(bValue);
            return mContainer;
        }

        static const string_object BOOL_TRUE;
        static const string_object BOOL_ONE;
        static const string_object BOOL_YES;
        static T                   bDummy;

        /// Returns a random boolean.
        /** \return Either 'true' or 'false' (50% each).
        */
        static s_bool_t Random()
        {
            return (rand()/(RAND_MAX+1.0) < 0.5f);
        }

    private :

        T bValue_;
    };

    template<class T>
    const string_object s_bool_t<T>::BOOL_TRUE = "true";
    template<class T>
    const string_object s_bool_t<T>::BOOL_ONE  = "1";
    template<class T>
    const string_object s_bool_t<T>::BOOL_YES  = "yes";

    template<class T, class N>
    s_str_t<N> operator+ ( const N* sLeft, const s_bool_t<T>& bRight )
    {
        return s_str_t<N>(sLeft) << bRight;
    }

    typedef s_bool_t<bool> s_bool;
}
