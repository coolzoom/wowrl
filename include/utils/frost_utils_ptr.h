/// Smart pointer
/** This is very basic pointer.<br>
*   Its only advantage is that it is initialized
*   to NULL automatically.<br>
*   There is <b>no</b> automatic deletion.
*/
template<class T>
class s_ptr
{
public :

    /// Default constructor.
    /** \note Initializes the pointer to NULL.
    */
    s_ptr()
    {
        pValue_ = NULL;
    }

    /// Constructor.
    s_ptr(T* pValue)
    {
        pValue_ = pValue;
    }

    /// Constructor.
    template<class N>
    explicit s_ptr(const s_ptr<N>& mPointer)
    {
        pValue_ = (T*)mPointer.Get();
    }

    /// Returns the contained pointer.
    /** \return The contained pointer
    */
    T* Get() const
    {
        return pValue_;
    }

    /// Checks if this pointer is usable.
    /** \return 'true' is this pointer is usable
    */
    s_bool IsValid() const
    {
        return (pValue_ != NULL);
    }

    /// Sets this pointer to NULL.
    void SetNull()
    {
        pValue_ = NULL;
    }

    /// Deletes the content of the pointer;
    void Delete()
    {
        delete pValue_;
        pValue_ = NULL;
    }

    /// Returns a reference to the contained value.
    /** \return A reference to the contained value
    */
    T& operator * () const
    {
        return *pValue_;
    }

    /// Dereferences the pointer.
    /** \return The contained pointer
    */
    T* operator -> () const
    {
        return pValue_;
    }

    /// Assign a new pointer to this smart pointer.
    /** \param pValue The new pointer
    */
    template<class N>
    s_ptr<T>& operator = (s_ptr<N> pValue)
    {
        pValue_ = pValue.Get();
        return *this;
    }

    /// Assign a new pointer to this smart pointer.
    /** \param pValue The new pointer
    */
    s_ptr<T>& operator = (T* pValue)
    {
        pValue_ = pValue;
        return *this;
    }

    /// Checks if this pointer equals another
    /** \param pValue The pointer to test
    */
    template<class N>
    s_bool operator == (s_ptr<N> pValue)
    {
        return (pValue_ == pValue.Get());
    }

    /// Checks if this pointer equals another
    /** \param pValue The pointer to test
    */
    s_bool operator == (T* pValue)
    {
        return (pValue_ == pValue);
    }

    /// Checks if this pointer is different from another
    /** \param pValue The pointer to test
    */
    template<class N>
    s_bool operator != (s_ptr<N> pValue)
    {
        return (pValue_ != pValue.Get());
    }

    /// Checks if this pointer is different from another
    /** \param pValue The pointer to test
    */
    s_bool operator != (T* pValue)
    {
        return (pValue_ != pValue);
    }

    /// Allows : "if (!pPointer)".
    bool operator ! () const
    {
        return (pValue_ == NULL);
    }

    typedef void (s_ptr::*MemberFn)();
    /// Allows : "if (pPointer)".
    operator MemberFn() const
    {
        if (pValue_ != NULL)
            return &s_ptr::SetNull;
        else
            return NULL;
    }

    /// Allows limited implicit inheritance conversion.
    template<class N>
    operator s_ptr<N>()
    {
        return s_ptr<N>(pValue_);
    }

    s_ctnr< s_ptr<T> > operator , ( s_ptr<T> pValue ) const
    {
        s_ctnr< s_ptr<T> > mContainer;
        mContainer.PushBack(*this);
        mContainer.PushBack(pValue);
        return mContainer;
    }

    static const s_str CLASS_NAME;

private :

    T* pValue_;
};

template<class T>
const s_str s_ptr<T>::CLASS_NAME = "s_ptr";


template<class T>
s_str operator+ ( const s_str& sLeft, s_ptr<T> pRight )
{
    return s_str(sLeft) << (void*)pRight.Get();
}

template<class T>
s_str& operator<< ( s_str& sLeft, s_ptr<T> pRight )
{
    return sLeft << (void*)pRight.Get();
}

template<class T>
s_str operator+ ( const char* sLeft, s_ptr<T> pRight )
{
    return s_str(sLeft) << (void*)pRight.Get();
}
