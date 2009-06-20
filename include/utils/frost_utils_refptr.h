// Warning : If you need to use this file, include frost_utils_types.h
namespace Frost
{
    /// Reference counting pointer
    /** This is a pointer that automatically deletes
    *   its content when not used anymore.<br>
    *   Uses reference counting.
    */
    template<class T>
    class s_refptr
    {
    public :

        /// Default constructor.
        /** \note Initializes the pointer to NULL.
        */
        s_refptr()
        {
            pValue_ = NULL;
            pCounter_ = NULL;
        }

        /// Constructor.
        /** \param pValue The pointer to assign
        *   \note This is the only way to assign a classic
        *         pointer to a s_refptr.
        */
        explicit s_refptr(T* pValue)
        {
            pValue_ = pValue;
            pCounter_ = new uint(0);

            Increment_();
        }

        /// Constructor.
        /** \param pValue   The pointer to assign
        *   \param pCounter The counter to use
        *   \note This function should not be called
        *         by anyone else than itself (and other
        *         template specializations).
        */
        explicit s_refptr(T* pValue, uint* pCounter)
        {
            pValue_ = pValue;
            pCounter_ = pCounter;

            Increment_();
        }

        /// Copy constructor.
        /** \param mValue the s_refptr to copy
        *   \note Can cause deletion of the contained
        *         pointer.
        */
        s_refptr(const s_refptr& mValue)
        {
            pValue_ = mValue.pValue_;
            pCounter_ = mValue.pCounter_;

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

        /// Sets this pointer to NULL.
        /** \note Can cause deletion of the contained
        *         pointer.
        */
        void SetNull()
        {
            Decrement_();
            pValue_ = NULL;
            pCounter_ = NULL;
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

                pValue_ = mValue.pValue_;
                pCounter_ = mValue.pCounter_;

                Increment_();
            }

            return *this;
        }

        /// Checks if this pointer equals another
        /** \param pValue The pointer to test
        */
        template<class N>
        s_bool operator == (s_refptr<N> pValue)
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
        s_bool operator != (s_refptr<N> pValue)
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
        operator s_refptr<N>()
        {
            return s_refptr<N>(pValue_, pCounter_);
        }

        s_ctnr< s_refptr<T> > operator , ( s_refptr<T> pValue ) const
        {
            s_ctnr< s_refptr<T> > mContainer;
            mContainer.PushBack(*this);
            mContainer.PushBack(pValue);
            return mContainer;
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
                    delete pCounter_;
                    pValue_ = NULL;
                    pCounter_ = NULL;
                }
            }
        }

        T*    pValue_;
        uint* pCounter_;
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
