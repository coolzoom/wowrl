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
            pValue_    = nullptr;
            pCounter_  = nullptr;
            pWCounter_ = nullptr;
        }

        /// Copy constructor.
        s_wptr(const s_wptr& pPtr)
        {
            pValue_    = pPtr.pValue_;
            pCounter_  = pPtr.pCounter_;
            pWCounter_ = pPtr.pWCounter_;

            Increment_();
        }

        /// Conversion from nullptr.
        s_wptr(const s_nullptr& pNull)
        {
            pValue_    = nullptr;
            pCounter_  = nullptr;
            pWCounter_ = nullptr;
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
            pValue_    = pRefPtr.pValue_;
            pCounter_  = pRefPtr.pCounter_;
            pWCounter_ = pRefPtr.pWCounter_;

            Increment_();
        }

        template<class N>
        /// s_wptr conversion.
        s_wptr(const s_wptr<N>& pPtr)
        {
            pValue_    = pPtr.pValue_;
            pCounter_  = pPtr.pCounter_;
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
        s_uint_t<default_uint> GetCount() const
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
        s_uint_t<default_uint> GetWeakCount() const
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

            pValue_    = nullptr;
            pCounter_  = nullptr;
            pWCounter_ = nullptr;
        }

        /// Creates a s_refptr pointing to the object (if any).
        /** \returns A s_refptr pointing to the object (if any)
        *   \note If this pointer is invalid, this function returns an empty
        *         s_refptr.
        */
        s_refptr<const T> Lock() const
        {
            if (IsValid())
                return s_refptr<const T>(pValue_, pCounter_, pWCounter_);
            else
                return s_refptr<const T>();
        }

        /// Creates a s_refptr pointing to the object (if any).
        /** \returns A s_refptr pointing to the object (if any)
        *   \note If this pointer is invalid, this function returns an empty
        *         s_refptr.
        */
        s_refptr<T> Lock()
        {
            if (IsValid())
                return s_refptr<T>(pValue_, pCounter_, pWCounter_);
            else
                return s_refptr<T>();
        }

        /// Dereferences the pointer.
        /** \return The contained pointer
        *   \note You should test for the pointer's validity before using
        *         this operator, unless you're sure the object is still
        *         alive.
        */
        const T* operator -> () const
        {
            return pValue_;
        }

        /// Dereferences the pointer.
        /** \return The contained pointer
        *   \note You should test for the pointer's validity before using
        *         this operator, unless you're sure the object is still
        *         alive.
        */
        T* operator -> ()
        {
            return pValue_;
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

        /// nullptr assignation operator.
        /** \param pPtr The value to copy
        */
        s_wptr& operator = (const s_nullptr& pPtr)
        {
            pValue_    = nullptr;
            pCounter_  = nullptr;
            pWCounter_ = nullptr;

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

        /// Checks if this pointer equals another
        /** \param pValue The pointer to test
        */
        template<class N>
        s_bool operator == (const s_wptr<N>& pValue) const
        {
            return (pValue_ == pValue.pValue_);
        }

        /// Checks if this pointer equals another
        /** \param pValue The pointer to test
        */
        template<class N>
        s_bool operator == (const s_refptr<N>& pValue) const
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
        s_bool operator != (const s_wptr<N>& pValue) const
        {
            return (pValue_ != pValue.pValue_);
        }

        /// Checks if this pointer is different from another
        /** \param pValue The pointer to test
        *   \return 'true' if this pointer is different from another
        */
        template<class N>
        s_bool operator != (const s_refptr<N>& pValue) const
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
        s_bool operator < (const s_wptr<N>& pValue) const
        {
            return (pValue_ < pValue.pValue_);
        }

        /// Checks if this pointer's value is lower than the other
        /** \param pValue The pointer to test
        *   \return 'true' if this pointer's value is lower than the other
        */
        template<class N>
        s_bool operator < (const s_refptr<N>& pValue) const
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
        s_bool operator <= (const s_wptr<N>& pValue) const
        {
            return (pValue_ <= pValue.pValue_);
        }

        /// Checks if this pointer's value is lower or equal than the other
        /** \param pValue The pointer to test
        *   \return 'true' if this pointer's value is lower or equal than the other
        */
        template<class N>
        s_bool operator <= (const s_refptr<N>& pValue) const
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
        s_bool operator > (const s_wptr<N>& pValue) const
        {
            return (pValue_ > pValue.pValue_);
        }

        /// Checks if this pointer's value is greater than the other
        /** \param pValue The pointer to test
        *   \return 'true' if this pointer's value is greater than the other
        */
        template<class N>
        s_bool operator > (const s_refptr<N>& pValue) const
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
        s_bool operator >= (const s_wptr<N>& pValue) const
        {
            return (pValue_ >= pValue.pValue_);
        }

        /// Checks if this pointer's value is greater or equal than the other
        /** \param pValue The pointer to test
        *   \return 'true' if this pointer's value is greater or equal than the other
        */
        template<class N>
        s_bool operator >= (const s_refptr<N>& pValue) const
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
            return (pCounter_ == 0 ? true : *pCounter_ == 0u);
        }

        typedef void (s_wptr::*MemberFn)();
        /// Allows : "if (pPointer)".
        operator MemberFn() const
        {
            if (pCounter_ && *pCounter_ != 0u)
                return &s_wptr::SetNull;
            else
                return 0;
        }

        s_ctnr< s_wptr<T> > operator , (const s_wptr<T>& pValue) const
        {
            s_ctnr< s_wptr<T> > mContainer;
            mContainer.PushBack(*this);
            mContainer.PushBack(pValue);
            return mContainer;
        }

        /// Casts the provided pointer to this one's type.
        /** \param pValue The pointer to cast
        *   \return The new casted pointer
        */
        template<class N>
        static s_wptr<T> StaticCast(const s_wptr<N>& pValue)
        {
            return s_wptr<T>(static_cast<T*>(pValue.pValue_), pValue.pCounter_, pValue.pWCounter_);
        }

        /// Tries to dynamic cast the provided pointer to this one's type.
        /** \param pValue The pointer to cast
        *   \return The new casted pointer
        *   \note Dynamic cast can fail, and in this case, will result in
        *         a NULL pointer.
        */
        template<class N>
        static s_wptr<T> DynamicCast(const s_wptr<N>& pValue)
        {
            T* pTemp = dynamic_cast<T*>(pValue.pValue_);
            if (pTemp)
                return s_wptr<T>(pTemp, pValue.pCounter_, pValue.pWCounter_);
            else
                return s_wptr<T>();
        }

    private :

        s_wptr(T* pValue, default_uint* pCounter, default_uint* pWCounter) :
            pValue_(pValue), pCounter_(pCounter), pWCounter_(pWCounter)
        {
        }

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
                    pCounter_ = nullptr;
                    delete pWCounter_;
                    pWCounter_ = nullptr;
                }
            }
        }

        T*    pValue_;
        default_uint* pCounter_;
        default_uint* pWCounter_;
    };
}
