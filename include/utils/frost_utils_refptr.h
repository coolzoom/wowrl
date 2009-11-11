// Warning : If you need to use this file, include frost_utils_types.h
namespace Frost
{
    /// Reference counting pointer
    /** This is a pointer that automatically deletes
    *   its content when not used anymore.<br>
    *   Can be used in conjunction with s_wptr.
    *   Uses reference counting.
    */
    template<class T>
    class s_refptr
    {
    public :

        template<class> friend class s_refptr;
        template<class> friend class s_wptr;

        /// Default constructor.
        /** \note Initializes the pointer to NULL.
        */
        s_refptr()
        {
            pValue_    = NULL;
            pCounter_  = NULL;
            pWCounter_ = NULL;
        }

        /// Constructor.
        /** \param pValue The pointer to assign
        *   \note This is the only way to assign a classic
        *         pointer to a s_refptr.
        */
        explicit s_refptr(T* pValue)
        {
            pValue_    = pValue;
            pCounter_  = new uint(0);
            pWCounter_ = new uint(0);

            Increment_();
        }

        /// Copy constructor.
        /** \param mValue the s_refptr to copy
        */
        s_refptr(const s_refptr& mValue)
        {
            pValue_    = mValue.pValue_;
            pCounter_  = mValue.pCounter_;
            pWCounter_ = mValue.pWCounter_;

            Increment_();
        }

        template<class N>
        /// Conversion constructor.
        /** \param mValue the s_refptr to copy
        */
        explicit s_refptr(const s_refptr<N>& mValue)
        {
            pValue_    = mValue.pValue_;
            pCounter_  = mValue.pCounter_;
            pWCounter_ = mValue.pWCounter_;

            Increment_();
        }

        /// Destructor.
        /** \note Can cause deletion of the contained
        *         pointer.
        */
        ~s_refptr()
        {
            Decrement_();
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

        /// Returns the number of s_refptr pointing to the object.
        /** \return The number of s_refptr pointing to the object
        *   \note This function returns 0 if the pointer is NULL.
        */
        s_uint GetCount() const
        {
            if (pCounter_)
                return *pCounter_;
            else
                return 0u;
        }

        /// Returns the number of s_wptr pointing to the object.
        /** \return The number of s_wptr pointing to the object
        *   \note This function returns 0 if the pointer is NULL.
        */
        s_uint GetWeakCount() const
        {
            if (pWCounter_)
                return *pWCounter_;
            else
                return 0u;
        }

        /// Sets this pointer to NULL.
        /** \note Can cause deletion of the contained
        *         pointer.
        */
        void SetNull()
        {
            Decrement_();

            pValue_    = NULL;
            pCounter_  = NULL;
            pWCounter_ = NULL;
        }

        /// Creates a s_wptr pointing at the same object.
        s_wptr<T> CreateWeak() const
        {
            return s_wptr<T>(*this);
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

        /// Copy operator.
        /** \param mValue The value to copy
        *   \note Can cause deletion of the contained
        *         pointer.
        */
        s_refptr& operator = (const s_refptr& mValue)
        {
            if (&mValue != this)
            {
                Decrement_();

                pValue_    = mValue.pValue_;
                pCounter_  = mValue.pCounter_;
                pWCounter_ = mValue.pWCounter_;

                Increment_();
            }

            return *this;
        }

        template<class N>
        /// Copy operator.
        /** \param mValue The value to copy
        *   \note Can cause deletion of the contained
        *         pointer.
        */
        s_refptr& operator = (const s_refptr<N>& mValue)
        {
            if (&mValue != this)
            {
                Decrement_();

                pValue_    = mValue.pValue_;
                pCounter_  = mValue.pCounter_;
                pWCounter_ = mValue.pWCounter_;

                Increment_();
            }

            return *this;
        }

        template<class N>
        /// Checks if this pointer equals another
        /** \param pValue The pointer to test
        */
        s_bool operator == (const s_refptr<N>& mValue)
        {
            return (pValue_ == mValue.pValue_);
        }

        /// Checks if this pointer equals another
        /** \param pValue The pointer to test
        */
        s_bool operator == (T* pValue) const
        {
            return (pValue_ == pValue);
        }

        template<class N>
        /// Checks if this pointer is different from another
        /** \param pValue The pointer to test
        */
        s_bool operator != (const s_refptr<N>& mValue)
        {
            return (pValue_ != mValue.pValue_);
        }

        /// Checks if this pointer is different from another
        /** \param pValue The pointer to test
        */
        s_bool operator != (T* pValue) const
        {
            return (pValue_ != pValue);
        }

        /// Allows : "if (!pPointer)".
        bool operator ! () const
        {
            return (pValue_ == NULL);
        }

        typedef void (s_refptr::*MemberFn)();
        /// Allows : "if (pPointer)".
        operator MemberFn() const
        {
            if (pValue_ != NULL)
                return &s_refptr::SetNull;
            else
                return NULL;
        }

        /// Allows limited implicit inheritance conversion.
        template<class N>
        operator s_refptr<N>() const
        {
            return s_refptr<N>(pValue_, pCounter_, pWCounter_);
        }

        s_ctnr< s_refptr<T> > operator , ( s_refptr<T> pValue ) const
        {
            s_ctnr< s_refptr<T> > mContainer;
            mContainer.PushBack(*this);
            mContainer.PushBack(pValue);
            return mContainer;
        }

        /// Casts the provided pointer to this one's type.
        /** \param pValue The pointer to cast
        *   \return The new casted pointer
        */
        template<class N>
        static s_refptr<T> StaticCast(s_refptr<N> pValue)
        {
            return s_refptr<T>(static_cast<T*>(pValue.pValue_), pValue.pCounter_, pValue.pWCounter_);
        }

        /// Tries to dynamic cast the provided pointer to this one's type.
        /** \param pValue The pointer to cast
        *   \return The new casted pointer
        *   \note Dynamic cast can fail, and in this case, will result in
        *         a NULL pointer.
        */
        template<class N>
        static s_refptr<T> DynamicCast(s_refptr<N> pValue)
        {
            T* pTemp = dynamic_cast<T*>(pValue.pValue_);
            if (pTemp)
                return s_refptr<T>(pTemp, pValue.pCounter_, pValue.pWCounter_);
            else
                return s_refptr<T>();
        }

    protected :

        /// Constructor.
        /** \param pValue   The pointer to assign
        *   \param pCounter The counter to use
        *   \param pWCounter The weak ptr counter to use
        *   \note This function should not be called
        *         by anyone else than itself (and other
        *         template specializations).
        */
        explicit s_refptr(T* pValue, uint* pCounter, uint* pWCounter)
        {
            pValue_    = pValue;
            pCounter_  = pCounter;
            pWCounter_ = pWCounter;

            Increment_();
        }

    private :

        void Increment_()
        {
            if (pCounter_)
                ++(*pCounter_);
        }

        void Decrement_()
        {
            if (pCounter_)
            {
                --(*pCounter_);
                if (*pCounter_ == 0u)
                {
                    delete pValue_;
                    pValue_ = NULL;

                    if (*pWCounter_ == 0u)
                    {
                        delete pCounter_;
                        pCounter_ = NULL;
                        delete pWCounter_;
                        pWCounter_ = NULL;
                    }
                }
            }
        }

        T*    pValue_;
        uint* pCounter_;
        uint* pWCounter_;
    };

    template<class T>
    s_str operator+ ( const s_str& sLeft, s_refptr<T> pRight )
    {
        return s_str(sLeft) << static_cast<void*>(pRight.Get());
    }

    template<class T>
    s_str& operator<< ( s_str& sLeft, s_refptr<T> pRight )
    {
        return sLeft << static_cast<void*>(pRight.Get());
    }

    template<class T>
    s_str operator+ ( const char* sLeft, s_refptr<T> pRight )
    {
        return s_str(sLeft) << static_cast<void*>(pRight.Get());
    }
}
