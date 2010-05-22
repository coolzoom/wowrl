// Warning : If you need to use this file, include frost_utils_types.h
namespace Frost
{
    /// Base type : array
    /** This class is an improved version of C arrays.
    *   It comes with a few features that make it simpler
    *   to use.
    */
    template<class T, default_uint N = 0>
    class s_array
    {
    public :

        typedef T*                      iterator;
        typedef const T*                const_iterator;
        typedef s_range<iterator>       range;
        typedef s_range<const_iterator> const_range;

        s_array()
        {
        }

        s_array(const T& mInitValue)
        {
            for (default_uint i = 0; i < N; ++i)
            {
                lArray_[i] = mInitValue;
            }
        }

        template<class U, class C>
        s_array(const s_ctnr_t<U, C>& mContainer)
        {
            default_uint i;
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

        #ifdef CPP_0X
            /// Initializer list constructor.
            /** \param mList Brace enclosed element list
            *   \note This constructor uses a C++0x feature.<br>
            *         It allows : s_array<s_int, 4> v = {1, 2, 3, 4};<br>
            *         If the initializer_list is too long, only the N first
            *         values are stored into the s_array.<br>
            *         If it is too short, the s_array is filled with T's
            *         default constructor.
            */
            s_array(std::initializer_list<T> mList)
            {
                default_uint i = 0;
                for (const T* p = mList.begin(); (p != mList.end()) && (i < N); ++p, ++i)
                    lArray_[i] = *p;
                for (; i < N; ++i)
                {
                    lArray_[i] = T();
                }
            }
        #endif

        s_array(const s_array& mValue)
        {
            for (default_uint i = 0; i < N; ++i)
                lArray_[i] = mValue.lArray_[i];
        }

        /// Returns the underlying C array.
        /** \return The underlying C array
        */
        T* GetClassicArray()
        {
            return lArray_;
        }

        void Set(const T& mValue)
        {
            for (default_uint i = 0; i < N; ++i)
            {
                lArray_[i] = mValue;
            }
        }

        void Set(const T (&lArray)[N])
        {
            for (default_uint i = 0; i < N; ++i)
            {
                lArray_[i] = lArray[i];
            }
        }

        void Set(const T* lArray)
        {
            for (default_uint i = 0; i < N; ++i)
            {
                lArray_[i] = lArray[i];
            }
        }

        void Set(const s_nullptr& pPtr)
        {
            for (default_uint i = 0; i < N; ++i)
            {
                lArray_[i] = pPtr;
            }
        }

        s_uint_t<default_uint> GetSize() const
        {
            return N;
        }

        const T& Get(const s_uint_t<default_uint>& uiIndex) const
        {
            return lArray_[uiIndex.Get()];
        }

        iterator Begin()
        {
            return lArray_;
        }

        const_iterator Begin() const
        {
            return lArray_;
        }

        iterator End()
        {
            return lArray_ + N;
        }

        const_iterator End() const
        {
            return lArray_ + N;
        }

        iterator begin()
        {
            return lArray_;
        }

        const_iterator begin() const
        {
            return lArray_;
        }

        iterator end()
        {
            return lArray_ + N;
        }

        const_iterator end() const
        {
            return lArray_ + N;
        }

        T& operator [] (const s_uint_t<default_uint>& uiIndex)
        {
            return lArray_[uiIndex.Get()];
        }

        const T& operator [] (const s_uint_t<default_uint>& uiIndex) const
        {
            return lArray_[uiIndex.Get()];
        }

        T& operator [] (const default_uint& uiIndex)
        {
            return lArray_[uiIndex];
        }

        const T& operator [] (const default_uint& uiIndex) const
        {
            return lArray_[uiIndex];
        }

        s_bool operator == (const s_array& mValue)
        {
            for (default_uint i = 0; i < N; ++i)
            {
                if (lArray_[i] != mValue[i])
                    return false;
            }

            return true;
        }

        s_bool operator != (const s_array& mValue)
        {
            for (default_uint i = 0; i < N; ++i)
            {
                if (lArray_[i] != mValue[i])
                    return true;
            }

            return false;
        }

        s_array& operator = (const s_array& mValue)
        {
            for (default_uint i = 0; i < N; ++i)
                lArray_[i] = mValue.lArray_[i];

            return *this;
        }

        template<class U, class C>
        s_array& operator = (const s_ctnr_t<U, C>& mContainer)
        {
            default_uint i;
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

    private :

        T lArray_[N];
    };

    template<class T, default_uint N> class StringConverter< string_element, s_array<T, N> >
    {
    public :

        typedef string_object string;

        static string Convert(const s_array<T, N>& mArray)
        {
            string sTemp = "(";
            for (default_uint i = 0; i < N; ++i)
            {
                if (i != 0)
                    sTemp += ", " + StringConverter<string_element, T>::Convert(mArray[i]);
                else
                    sTemp += StringConverter<string_element, T>::Convert(mArray[i]);
            }
            sTemp += ")";

            return sTemp;
        }
    };
}

