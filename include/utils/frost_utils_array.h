namespace Frost
{
    /// Base type : array
    /** This class is an improved version of C arrays.
    *   It comes with a few features that make it simpler
    *   to use.
    */
    template<class T, uint N>
    class s_array
    {
    public :

        s_array()
        {
        }

        s_array(const T& mInitValue)
        {
            for (uint i = 0; i < N; ++i)
            {
                lArray_[i] = mInitValue;
            }
        }

        s_array(const s_ctnr<T>& mContainer)
        {
            uint i;
            for (i = 0; i < N; ++i)
            {
                if (i >= mContainer.GetSize().Get())
                    break;

                lArray_[i] = mContainer[i];
            }
            for (; i < N; ++i)
            {
                lArray_[i] = T();
            }
        }

        s_array(const s_array& mValue)
        {
            memcpy(lArray_, mValue.lArray_, N*sizeof(T));
        }

        void Set(const T& mValue)
        {
            for (uint i = 0; i < N; ++i)
            {
                lArray_[i] = mValue;
            }
        }

        void Set(const T (&lArray)[N])
        {
            for (uint i = 0; i < N; ++i)
            {
                lArray_[i] = lArray[i];
            }
        }

        void Set(const T* lArray)
        {
            for (uint i = 0; i < N; ++i)
            {
                lArray_[i] = lArray[i];
            }
        }

        s_uint GetSize() const
        {
            return N;
        }

        const T& Get(const s_uint& uiIndex) const
        {
            if (uiIndex < N)
                return lArray_[uiIndex.Get()];
            else
                return mDummy;
        }

        T& operator [] (const uint& uiIndex)
        {
            if (uiIndex < N)
                return lArray_[uiIndex];
            else
                return mDummy;
        }

        const T& operator [] (const uint& uiIndex) const
        {
            if (uiIndex < N)
                return lArray_[uiIndex];
            else
                return mDummy;
        }

        T& operator [] (const s_uint& uiIndex)
        {
            if (uiIndex < N)
                return lArray_[uiIndex.Get()];
            else
                return mDummy;
        }

        const T& operator [] (const s_uint& uiIndex) const
        {
            if (uiIndex < N)
                return lArray_[uiIndex.Get()];
            else
                return mDummy;
        }

        s_bool operator == (const s_array& mValue)
        {
            for (uint i = 0; i < N; ++i)
            {
                if (lArray_[i] != mValue[i])
                    return false;
            }

            return true;
        }

        s_bool operator != (const s_array& mValue)
        {
            for (uint i = 0; i < N; ++i)
            {
                if (lArray_[i] != mValue[i])
                    return true;
            }

            return false;
        }

        s_array& operator = (const s_array& mValue)
        {
            memcpy(lArray_, mValue.lArray_, N*sizeof(T));
            return *this;
        }

        s_array& operator = (const s_ctnr<T>& mContainer)
        {
            uint i;
            for (i = 0; (i < N) && (i < mContainer.GetSize().Get()); ++i)
            {
                lArray_[i] = mContainer[i];
            }
            for (; i < N; ++i)
            {
                lArray_[i] = T();
            }

            return *this;
        }

        static T mDummy;

    private :

        T lArray_[N];
    };

    template<class T, uint N> T s_array<T, N>::mDummy;

    template<class T, uint N, class M>
    s_str_t<M> operator + (const s_str_t<M>& sLeft, const s_array<T, N>& mRight)
    {
        s_str_t<M> sTemp = STRING("(");
        for (uint i = 0; i < N; ++i)
        {
            if (i == N-1)
                sTemp << mRight[i];
            else
                sTemp << mRight[i] << STRING(", ");
        }
        sTemp << STRING(")");

        return sLeft + sTemp;
    }

    template<class T, uint N, class M>
    s_str_t<M> operator + (const M* sLeft, const s_array<T, N>& mRight)
    {
        return s_str_t<M>(sLeft) + mRight;
    }

    #ifdef USE_UNICODE
        template<class T, uint N>
        s_str_t<char> operator + (const char* sLeft, const s_array<T, N>& mRight)
        {
            return s_str_t<char>(sLeft) + mRight;
        }
    #endif
}

