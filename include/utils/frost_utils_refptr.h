/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*                                        */
/*                                        */


#ifndef FROST_UTILS_REFPTR_H
#define FROST_UTILS_REFPTR_H

#include "frost_utils.h"
#include "frost_utils_uint.h"
#include "frost_utils_bool.h"
#include "frost_utils_log.h"

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
            pCounter_ = new s_uint();

            Increment_();
        }

        /// Constructor.
        /** \param pValue   The pointer to assign
        *   \param pCounter The counter to use
        *   \note This function should not be called
        *         by anyone else than itself (and other
        *         template specializations).
        */
        explicit s_refptr(T* pValue, s_uint* pCounter)
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
            if (!pCounter_)
                Error(
                    CLASS_NAME,
                    "Trying to get a reference to a NULL pointer."
                );

            return *pValue_;
        }

        /// Dereferences the pointer.
        /** \return The contained pointer
        */
        T* operator -> () const
        {
            if (!pValue_)
                Error(
                    CLASS_NAME,
                    "Trying to dereference a NULL pointer."
                );

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
        bool operator == (s_refptr<N> pValue)
        {
            return (pValue_ == pValue.Get());
        }

        /// Checks if this pointer equals another
        /** \param pValue The pointer to test
        */
        bool operator == (T* pValue)
        {
            return (pValue_ == pValue);
        }

        /// Checks if this pointer is different from another
        /** \param pValue The pointer to test
        */
        template<class N>
        bool operator != (s_refptr<N> pValue)
        {
            return (pValue_ != pValue.Get());
        }

        /// Checks if this pointer is different from another
        /** \param pValue The pointer to test
        */
        bool operator != (T* pValue)
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
            mContainer.Push(*this);
            mContainer.Push(pValue);
            return mContainer;
        }

        static const s_str CLASS_NAME;

    private :

        void Increment_()
        {
            if (pCounter_)
                (*pCounter_)++;
        }

        void Decrement_()
        {
            if (pCounter_)
            {
                (*pCounter_)--;
                if (pCounter_->IsNull())
                {
                    delete pValue_;
                    delete pCounter_;
                    pValue_ = NULL;
                    pCounter_ = NULL;
                }
            }
        }

        T*      pValue_;
        s_uint* pCounter_;
    };

    template<class T>
    const s_str s_refptr<T>::CLASS_NAME = "s_refptr";

    template<class T>
    s_str operator+ ( const s_str& sLeft, s_refptr<T> pRight )
    {
        return s_str(sLeft) << (void*)pRight.Get();
    }

    template<class T>
    s_str& operator<< ( s_str& sLeft, s_refptr<T> pRight )
    {
        return sLeft << (void*)pRight.Get();
    }

    template<class T>
    s_str operator+ ( const char* sLeft, s_refptr<T> pRight )
    {
        return s_str(sLeft) << (void*)pRight.Get();
    }
}

#endif
