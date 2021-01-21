// Warning : If you need to use this file, include frost_utils_types.h
namespace Frost
{
    template<class T>
    struct PtrTraits
    {
        typedef const T& CRef;
        typedef T&       Ref;
    };

    template<>
    struct PtrTraits<void>
    {
        typedef void CRef;
        typedef void Ref;
    };

    /// Smart pointer
    /** This is very basic pointer.<br>
    *   Its only advantage is that it is initialized
    *   to nullptr automatically.<br>
    *   There is <b>no</b> automatic deletion.
    */
    template<class T>
    class s_ptr
    {
    public :

        template<class> friend class s_ptr;

        /// Default constructor.
        /** \note Initializes the pointer to NULL.
        */
        s_ptr()
        {
            pValue_ = nullptr;
        }

        /// Constructor.
        s_ptr(T* pValue)
        {
            pValue_ = pValue;
        }

        /// Conversion from nullptr.
        s_ptr(const s_nullptr& pNull)
        {
            pValue_ = nullptr;
        }

        /// Copy constructor.
        s_ptr(const s_ptr& mPointer)
        {
            pValue_ = mPointer.pValue_;
        }

        /// Conversion constructor.
        template<class N>
        explicit s_ptr(const s_ptr<N>& mPointer)
        {
            pValue_ = mPointer.pValue_;
        }

        /// Returns the contained pointer.
        /** \return The contained pointer
        */
        const T* Get() const
        {
            return pValue_;
        }

        /// Returns the contained pointer.
        /** \return The contained pointer
        */
        T* Get()
        {
            return pValue_;
        }

        /// Checks if this pointer is usable.
        /** \return 'true' is this pointer is usable
        */
        s_bool IsValid() const
        {
            return (pValue_ != 0);
        }

        /// Sets this pointer to NULL.
        void SetNull()
        {
            pValue_ = nullptr;
        }

        /// Deletes the content of the pointer;
        void Delete()
        {
            delete pValue_;
            pValue_ = nullptr;
        }

        /// Returns a reference to the contained value.
        /** \return A reference to the contained value
        */
        typename PtrTraits<T>::CRef operator * () const
        {
            return *pValue_;
        }

        /// Returns a reference to the contained value.
        /** \return A reference to the contained value
        */
        typename PtrTraits<T>::Ref operator * ()
        {
            return *pValue_;
        }

        /// Dereferences the pointer.
        /** \return The contained pointer
        */
        const T* operator -> () const
        {
            return pValue_;
        }

        /// Dereferences the pointer.
        /** \return The contained pointer
        */
        T* operator -> ()
        {
            return pValue_;
        }

        /// nullptr assignation operator.
        /** \param pPtr The value to copy
        */
        s_ptr<T>& operator = (const s_nullptr& pPtr)
        {
            pValue_ = nullptr;
            return *this;
        }

        /// Assign a new pointer to this smart pointer.
        /** \param pValue The new pointer
        */
        template<class N>
        s_ptr<T>& operator = (const s_ptr<N>& pValue)
        {
            pValue_ = pValue.pValue_;
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
        s_bool operator == (const s_ptr<N>& pValue) const
        {
            return (pValue_ == pValue.pValue_);
        }

        /// Checks if this pointer equals another
        /** \param pValue The pointer to test
        */
        template<class N>
        s_bool operator == (N* pValue) const
        {
            return (pValue_ == pValue);
        }

        /// Checks if this pointer is null
        /** \param pValue The null pointer
        *   \return 'true' if this pointer is null
        */
        s_bool operator == (s_nullptr pValue) const
        {
            return (pValue_ == 0);
        }

        /// Checks if this pointer is different from another
        /** \param pValue The pointer to test
        *   \return 'true' if this pointer is different from another
        */
        template<class N>
        s_bool operator != (const s_ptr<N>& pValue) const
        {
            return (pValue_ != pValue.pValue_);
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

        /// Checks if this pointer is not null
        /** \param pValue The null pointer
        *   \return 'true' if this pointer is not null
        */
        s_bool operator != (s_nullptr pValue) const
        {
            return (pValue_ != 0);
        }

        /// Checks if this pointer's value is lower than the other
        /** \param pValue The pointer to test
        *   \return 'true' if this pointer's value is lower than the other
        */
        template<class N>
        s_bool operator < (const s_ptr<N>& pValue) const
        {
            return (pValue_ < pValue.pValue_);
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
        s_bool operator <= (const s_ptr<N>& pValue) const
        {
            return (pValue_ <= pValue.pValue_);
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
        s_bool operator > (const s_ptr<N>& pValue) const
        {
            return (pValue_ > pValue.pValue_);
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
        s_bool operator >= (const s_ptr<N>& pValue) const
        {
            return (pValue_ >= pValue.pValue_);
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
            return (pValue_ == 0);
        }

        typedef void (s_ptr::*MemberFn)();
        /// Allows : "if (pPointer)".
        operator MemberFn() const
        {
            if (pValue_ != 0)
                return &s_ptr::SetNull;
            else
                return 0;
        }

        /// Allows limited implicit inheritance conversion.
        template<class N>
        operator s_ptr<N>()
        {
            return s_ptr<N>(pValue_);
        }

        /// Allows limited implicit inheritance conversion.
        template<class N>
        operator s_ptr<const N>() const
        {
            return s_ptr<const N>(pValue_);
        }

        s_ctnr< s_ptr<T> > operator , (const s_ptr<T>& pValue) const
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
        static s_ptr<T> StaticCast(const s_ptr<N>& pValue)
        {
            return static_cast<T*>(pValue.pValue_);
        }

        /// Casts the provided pointer to this one's type.
        /** \param pValue The pointer to cast
        *   \return The new casted pointer
        */
        template<class N>
        static s_ptr<T> StaticCast(N* pValue)
        {
            return static_cast<T*>(pValue);
        }

        /// Tries to dynamic cast the provided pointer to this one's type.
        /** \param pValue The pointer to cast
        *   \return The new casted pointer
        *   \note Dynamic cast can fail, and in this case, will result in
        *         a NULL pointer.
        */
        template<class N>
        static s_ptr<T> DynamicCast(const s_ptr<N>& pValue)
        {
            return dynamic_cast<T*>(pValue.pValue_);
        }

        /// Tries to dynamic cast the provided pointer to this one's type.
        /** \param pValue The pointer to cast
        *   \return The new casted pointer
        *   \note Dynamic cast can fail, and in this case, will result in
        *         a NULL pointer.
        */
        template<class N>
        static s_ptr<T> DynamicCast(N* pValue)
        {
            return dynamic_cast<T*>(pValue);
        }

    private :

        T* pValue_;
    };

    /** \cond NOT_REMOVE_FROM_DOC
    */
    template<class T> class TypeTraits<T*>
    {
    public :
        typedef T*         Type;
        typedef T*         BaseType;
        typedef s_ptr<T>   FrostType;
        typedef T*&        RefType;
        typedef T* const & CRefType;
        typedef T**        PointerType;

        static inline RefType  GetValue(RefType m)  { return m; }
        static inline CRefType GetValue(CRefType m) { return m; }
    };

    template<> class TypeTraits<void>
    {
    public :
        typedef void  Type;
        typedef void  BaseType;
        typedef void  FrostType;
        typedef void  RefType;
        typedef void  CRefType;
        typedef void* PointerType;
    };

    template<class T> class TypeTraits< s_ptr<T> >
    {
    public :
        typedef s_ptr<T>        Type;
        typedef T*              BaseType;
        typedef s_ptr<T>        FrostType;
        typedef s_ptr<T>&       RefType;
        typedef const s_ptr<T>& CRefType;
        typedef s_ptr<T>*       PointerType;

        static inline typename TypeTraits::BaseType GetValue(RefType m)  { return m.Get(); }
        static inline typename TypeTraits::BaseType GetValue(CRefType m) { return m.Get(); }
    };
    /** \endcond
    */
}

