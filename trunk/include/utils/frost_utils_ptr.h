// Warning : If you need to use this file, include frost_utils_types.h
namespace Frost
{
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
            pValue_ = mPointer.Get();
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
        s_bool operator == (s_ptr<N> pValue) const
        {
            return (pValue_ == pValue.Get());
        }

        /// Checks if this pointer equals another
        /** \param pValue The pointer to test
        */
        template<class N>
        s_bool operator == (N* pValue) const
        {
            return (pValue_ == pValue);
        }

        /// Checks if this pointer is different from another
        /** \param pValue The pointer to test
        *   \return 'true' if this pointer is different from another
        */
        template<class N>
        s_bool operator != (s_ptr<N> pValue) const
        {
            return (pValue_ != pValue.Get());
        }

        /// Checks if this pointer is different from another
        /** \param pValue The pointer to test
        *   \return 'true' if this pointer is different from another
        */
        template<class N>
        s_bool operator != (N* pValue) const
        {
            return (pValue_ != pValue);
        }

        /// Checks if this pointer's value is lower than the other
        /** \param pValue The pointer to test
        *   \return 'true' if this pointer's value is lower than the other
        */
        template<class N>
        s_bool operator < (s_ptr<N> pValue) const
        {
            return (pValue_ < pValue.Get());
        }

        /// Checks if this pointer's value is lower than the other
        /** \param pValue The pointer to test
        *   \return 'true' if this pointer's value is lower than the other
        */
        template<class N>
        s_bool operator < (N* pValue) const
        {
            return (pValue_ < pValue);
        }

        /// Checks if this pointer's value is lower or equal than the other
        /** \param pValue The pointer to test
        *   \return 'true' if this pointer's value is lower or equal than the other
        */
        template<class N>
        s_bool operator <= (s_ptr<N> pValue) const
        {
            return (pValue_ <= pValue.Get());
        }

        /// Checks if this pointer's value is lower or equal than the other
        /** \param pValue The pointer to test
        *   \return 'true' if this pointer's value is lower or equal than the other
        */
        template<class N>
        s_bool operator <= (N* pValue) const
        {
            return (pValue_ <= pValue);
        }

        /// Checks if this pointer's value is greater than the other
        /** \param pValue The pointer to test
        *   \return 'true' if this pointer's value is greater than the other
        */
        template<class N>
        s_bool operator > (s_ptr<N> pValue) const
        {
            return (pValue_ > pValue.Get());
        }

        /// Checks if this pointer's value is greater than the other
        /** \param pValue The pointer to test
        *   \return 'true' if this pointer's value is greater than the other
        */
        template<class N>
        s_bool operator > (N* pValue) const
        {
            return (pValue_ > pValue);
        }

        /// Checks if this pointer's value is greater or equal than the other
        /** \param pValue The pointer to test
        *   \return 'true' if this pointer's value is greater or equal than the other
        */
        template<class N>
        s_bool operator >= (s_ptr<N> pValue) const
        {
            return (pValue_ >= pValue.Get());
        }

        /// Checks if this pointer's value is greater or equal than the other
        /** \param pValue The pointer to test
        *   \return 'true' if this pointer's value is greater or equal than the other
        */
        template<class N>
        s_bool operator >= (N* pValue) const
        {
            return (pValue_ >= pValue);
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

        /// Casts the provided pointer to this one's type.
        /** \param pValue The pointer to cast
        *   \return The new casted pointer
        */
        template<class N>
        static s_ptr<T> StaticCast(s_ptr<N> pValue)
        {
            return static_cast<T*>(pValue.Get());
        }

        /// Tries to dynamic cast the provided pointer to this one's type.
        /** \param pValue The pointer to cast
        *   \return The new casted pointer
        *   \note Dynamic cast can fail, and in this case, will result in
        *         a NULL pointer.
        */
        template<class N>
        static s_ptr<T> DynamicCast(s_ptr<N> pValue)
        {
            return dynamic_cast<T*>(pValue.Get());
        }

        static const s_str CLASS_NAME;

    private :

        T* pValue_;
    };

    template<class T>
    const s_str s_ptr<T>::CLASS_NAME = "s_ptr";

    /** \cond NOT_REMOVE_FROM_DOC
    */
    template<class T> class TypeTraits<T*>  { public : typedef s_ptr<T> Type; };
    /** \endcond
    */

    template<class T>
    s_str operator+ ( const s_str& sLeft, s_ptr<T> pRight )
    {
        return s_str(sLeft) << static_cast<void*>(pRight.Get());
    }

    template<class T>
    s_str& operator<< ( s_str& sLeft, s_ptr<T> pRight )
    {
        return sLeft << static_cast<void*>(pRight.Get());
    }

    template<class T>
    s_str operator+ ( const char* sLeft, s_ptr<T> pRight )
    {
        return s_str(sLeft) << static_cast<void*>(pRight.Get());
    }
}

