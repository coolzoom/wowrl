#include "frost_utils_types.h"

namespace Frost
{

template<class T>
s_bool_t<T>::s_bool_t()
{
    bValue_ = false;
}

template<class T>
s_bool_t<T>::s_bool_t(const T& bValue)
{
    bValue_ = bValue;
}

template<class T>
bool s_bool_t<T>::operator ! () const
{
    return !bValue_;
}

template<class T>
s_bool_t<T>::operator MemberFn() const
{
    if (bValue_)
        return &s_bool_t::Get;
    else
        return NULL;
}

template<class T>
s_bool_t<T> s_bool_t<T>::operator == (const s_bool_t& bValue) const
{
    return (bValue_ == bValue.bValue_);
}

template<class T>
s_bool_t<T> s_bool_t<T>::operator != (const s_bool_t& bValue) const
{
    return (bValue_ != bValue.bValue_);
}

template<class T>
s_bool_t<T>& s_bool_t<T>::operator =  (const s_bool_t& bValue)
{
    bValue_ = bValue.bValue_;
    return *this;
}

/// Returns a random boolean.
/** \return Either 'true' or 'false' (50% each).
*/
template<class T>
s_bool_t<T> s_bool_t<T>::Random()
{
    return (rand()/(RAND_MAX+1.0) < 0.5f);
}

template<class T>
const string_element* s_bool_t<T>::BOOL_TRUE = "true";
template<class T>
const string_element* s_bool_t<T>::BOOL_ONE  = "1";
template<class T>
const string_element* s_bool_t<T>::BOOL_YES  = "yes";

template class s_bool_t<bool>;

}
