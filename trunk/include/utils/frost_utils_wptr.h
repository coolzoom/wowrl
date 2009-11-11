// Warning : If you need to use this file, include frost_utils_types.h
namespace Frost
{
    /// Weak pointer
    /** This is a companion of the reference counter pointer.<br>
    *   Use the Lock() function to get a valid pointer, or a NULL
    *   one if it has been deleted.
    */
    template<class T>
    class s_wptr
    {
    public :

        template<class N> friend class s_wptr;

        /// Default constructor.
        /** \note Initializes the pointer to NULL.
        */
        s_wptr()
        {
            pValue_    = NULL;
            pCounter_  = NULL;
            pWCounter_ = NULL;
        }

        /// Copy constructor.
        s_wptr(const s_wptr& pPtr)
        {
            pValue_ = pPtr.pValue_;
            pCounter_ = pPtr.pCounter_;
            pWCounter_ = pPtr.pWCounter_;

            Increment_();
        }

        /// Destructor.
        /** \note This function will <b>never</b> delete the pointed object (if any).
        */
        ~s_wptr()
        {
            Decrement_();
        }

        template<class N>
        /// s_refptr conversion.
        s_wptr(const s_refptr<N>& pRefPtr)
        {
            pValue_ = pRefPtr.pValue_;
            pCounter_ = pRefPtr.pCounter_;
            pWCounter_ = pRefPtr.pWCounter_;

            Increment_();
        }

        template<class N>
        /// s_wptr conversion.
        s_wptr(const s_wptr<N>& pPtr)
        {
            pValue_ = pPtr.Lock().pValue_;
            pCounter_ = pPtr.pCounter_;
            pWCounter_ = pPtr.pWCounter_;

            Increment_();
        }

        /// Checks if this pointer points to a valid object.
        /** \note The pointer can be invalid if the object has
        *         been deleted somewhere else.
        */
        s_bool IsValid() const
        {
            return (pCounter_ && *pCounter_ != 0u);
        }

        /// Returns the number of s_refptr pointing to the object.
        /** \return The number of s_refptr pointing to the object
        *   \note If this function returns 0, then the object has
        *         been deleted and this pointer is invalid, or it
        *         is simply NULL.
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
        /** \note This function will <b>never</b> delete the pointed object (if any).
        */
        void SetNull()
        {
            Decrement_();

            pValue_    = NULL;
            pCounter_  = NULL;
            pWCounter_ = NULL;
        }

        /// Creates a s_refptr pointing to the object (if any).
        /** \returns A s_refptr pointing to the object (if any)
        *   \note If this pointer is invalid, this function returns an empty
        *         s_refptr.
        */
        s_refptr<T> Lock() const
        {
            if (IsValid())
                return s_refptr<T>(pValue_, pCounter_, pWCounter_);
            else
                return s_refptr<T>();
        }

        /// Dereferences the pointer.
        /** \return The contained pointer
        *   \note Actually returns a s_refptr for code safety. You can't
        *         access the raw pointer directly from weak_ptr.
        *   \note You should test for the pointer's validity before using
        *         this operator, unless you're sure the object is still
        *         alive.
        */
        s_refptr<T> operator -> () const
        {
            if (IsValid())
                return s_refptr<T>(pValue_, pCounter_, pWCounter_);
            else
                return s_refptr<T>();
        }

        /// s_refptr assignation operator.
        /** \param pRefPtr The s_refptr to use as a base
        */
        s_wptr& operator = (const s_refptr<T>& pRefPtr)
        {
            pValue_ = pRefPtr.pValue_;
            pCounter_ = pRefPtr.pCounter_;
            pWCounter_ = pRefPtr.pWCounter_;

            Increment_();

            return *this;
        }

        /// Copy operator.
        /** \param pPtr The value to copy
        */
        s_wptr& operator = (const s_wptr& pPtr)
        {
            pValue_ = pPtr.pValue_;
            pCounter_ = pPtr.pCounter_;
            pWCounter_ = pPtr.pWCounter_;

            Increment_();

            return *this;
        }

        template<class N>
        /// Copy operator.
        /** \param pPtr The value to copy
        */
        s_wptr& operator = (const s_wptr<N>& pPtr)
        {
            pValue_ = pPtr.pValue_;
            pCounter_ = pPtr.pCounter_;
            pWCounter_ = pPtr.pWCounter_;

            Increment_();

            return *this;
        }

        /// Allows : "if (!pPointer)".
        bool operator ! () const
        {
            return (pCounter_ == NULL ? true : *pCounter_ == 0u);
        }

        typedef void (s_wptr::*MemberFn)();
        /// Allows : "if (pPointer)".
        operator MemberFn() const
        {
            if (pCounter_ && *pCounter_ != 0u)
                return &s_wptr::SetNull;
            else
                return NULL;
        }

        s_ctnr< s_wptr<T> > operator , ( s_wptr<T> pValue ) const
        {
            s_ctnr< s_wptr<T> > mContainer;
            mContainer.PushBack(*this);
            mContainer.PushBack(pValue);
            return mContainer;
        }

    private :

        void Increment_()
        {
            if (pWCounter_)
            {
                ++(*pWCounter_);
            }
        }

        void Decrement_()
        {
            if (pWCounter_)
            {
                --(*pWCounter_);
                if (*pWCounter_ == 0u && *pCounter_ == 0u)
                {
                    delete pCounter_;
                    pCounter_ = NULL;
                    delete pWCounter_;
                    pWCounter_ = NULL;
                }
            }
        }

        T*    pValue_;
        uint* pCounter_;
        uint* pWCounter_;
    };

    template<class T>
    s_str operator+ ( const s_str& sLeft, s_wptr<T> pRight )
    {
        return s_str(sLeft) << pRight.Lock();
    }

    template<class T>
    s_str& operator<< ( s_str& sLeft, s_wptr<T> pRight )
    {
        return sLeft << pRight.Lock();
    }

    template<class T>
    s_str operator+ ( const char* sLeft, s_wptr<T> pRight )
    {
        return s_str(sLeft) << pRight.Lock();
    }
}
