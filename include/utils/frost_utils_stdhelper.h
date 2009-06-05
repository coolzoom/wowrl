/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*            Std helper header           */
/*                                        */
/*                                        */


#ifndef FROST_UTILS_STDHELPER_H
#define FROST_UTILS_STDHELPER_H

#include "frost_utils.h"
#include "frost_utils_types.h"

// Returns 'true' if the key is found in the map
#define MAPFIND(key, map) ((map).find(key) != (map).end())

// Returns 'true' if the key is found in the vector/list/deque
#define VECTORFIND(key, vector) (std::find((vector).begin(), (vector).end(), key) != (vector).end())

// Iterates through an stl container
#define foreach(iter, container) for ((iter) = (container).begin(); (iter) != (container).end(); ++(iter))

namespace Frost
{
    /// Linear interpolation in an std::map
    template<class Key, class Elem>
    Elem MapLERP(std::map< Key, Elem >* lMap, Key mValue)
    {
        typedef typename std::map< Key, Elem >::iterator Iterator;

        if (lMap->find(mValue) != lMap->end())
        {
            return (*lMap)[mValue];
        }
        else
        {
            Iterator iterFirst, iterSecond;
            iterSecond = lMap->lower_bound(mValue);
            if (iterSecond == lMap->end())
            {
                --iterSecond;
                return iterSecond->second;
            }
            else if (iterSecond == lMap->begin())
            {
                return iterSecond->second;
            }
            else
            {
                iterFirst = iterSecond;
                --iterFirst;

                Key mCoef = (mValue - iterFirst->first)/(iterSecond->first - iterFirst->first);

                return iterFirst->second +
                       mCoef*(iterSecond->second - iterFirst->second);
            }
        }
    }

    /// Linear interpolation in an std::map (with Frost key)
    template<class Key, class Elem>
    Elem MapLERP_F(std::map< Key, Elem >* lMap, Key mValue)
    {
        typedef typename std::map< Key, Elem >::iterator Iterator;

        if (lMap->find(mValue) != lMap->end())
        {
            return (*lMap)[mValue];
        }
        else
        {
            Iterator iterFirst, iterSecond;
            iterSecond = lMap->lower_bound(mValue);
            if (iterSecond == lMap->end())
            {
                --iterSecond;
                return iterSecond->second;
            }
            else if (iterSecond == lMap->begin())
            {
                return iterSecond->second;
            }
            else
            {
                iterFirst = iterSecond;
                --iterFirst;

                Key mCoef = (mValue - iterFirst->first)/(iterSecond->first - iterFirst->first);

                return iterFirst->second +
                       mCoef.Get()*(iterSecond->second - iterFirst->second);
            }
        }
    }

    /// Linear interpolation in an std::map (with Frost key and Frost value)
    template<class Key, class Elem>
    Elem MapLERP_FF(std::map< Key, Elem >* lMap, Key mValue)
    {
        typedef typename std::map< Key, Elem >::iterator Iterator;

        if (lMap->find(mValue) != lMap->end())
        {
            return (*lMap)[mValue];
        }
        else
        {
            Iterator iterFirst, iterSecond;
            iterSecond = lMap->lower_bound(mValue);
            if (iterSecond == lMap->end())
            {
                --iterSecond;
                return iterSecond->second;
            }
            else if (iterSecond == lMap->begin())
            {
                return iterSecond->second;
            }
            else
            {
                iterFirst = iterSecond;
                --iterFirst;

                Key mCoef = (mValue - iterFirst->first)/(iterSecond->first - iterFirst->first);

                return iterFirst->second +
                       Elem(mCoef)*(iterSecond->second - iterFirst->second);
            }
        }
    }

    /// Linear interpolation in an std::map (with integral key)
    template<class Key, class Elem>
    Elem MapLERP_I(std::map< Key, Elem >* lMap, double mValue)
    {
        typedef typename std::map< Key, Elem >::iterator Iterator;

        if (lMap->find((Key)mValue) != lMap->end())
        {
            return (*lMap)[(Key)mValue];
        }
        else
        {
            Iterator iterFirst, iterSecond;
            iterSecond = lMap->lower_bound((Key)mValue);
            if (iterSecond == lMap->end())
            {
                --iterSecond;
                return iterSecond->second;
            }
            else if (iterSecond == lMap->begin())
            {
                return iterSecond->second;
            }
            else
            {
                iterFirst = iterSecond;
                --iterFirst;

                double dCoef = (mValue - iterFirst->first);
                dCoef /= (iterSecond->first - iterFirst->first);

                return iterFirst->second +
                       dCoef*(iterSecond->second - iterFirst->second);
            }
        }
    }

    /// Linear interpolation in an std::map (with integral Frost key)
    template<class Key, class Elem>
    Elem MapLERP_IF(std::map< Key, Elem >* lMap, s_double mValue)
    {
        typedef typename std::map< Key, Elem >::iterator Iterator;

        if (lMap->find(Key(mValue)) != lMap->end())
        {
            return (*lMap)[Key(mValue)];
        }
        else
        {
            Iterator iterFirst, iterSecond;
            iterSecond = lMap->lower_bound(Key(mValue));
            if (iterSecond == lMap->end())
            {
                --iterSecond;
                return iterSecond->second;
            }
            else if (iterSecond == lMap->begin())
            {
                return iterSecond->second;
            }
            else
            {
                iterFirst = iterSecond;
                --iterFirst;

                s_double dCoef = (mValue - s_double(iterFirst->first));
                dCoef /= s_double(iterSecond->first - iterFirst->first);

                return iterFirst->second +
                       dCoef.Get()*(iterSecond->second - iterFirst->second);
            }
        }
    }

    /// Linear interpolation in an std::map (with integral Frost key and Frost value)
    template<class Key, class Elem>
    Elem MapLERP_IFF(std::map< Key, Elem >* lMap, s_double mValue)
    {
        typedef typename std::map< Key, Elem >::iterator Iterator;

        if (lMap->find(Key(mValue)) != lMap->end())
        {
            return (*lMap)[Key(mValue)];
        }
        else
        {
            Iterator iterFirst, iterSecond;
            iterSecond = lMap->lower_bound(Key(mValue));
            if (iterSecond == lMap->end())
            {
                --iterSecond;
                return iterSecond->second;
            }
            else if (iterSecond == lMap->begin())
            {
                return iterSecond->second;
            }
            else
            {
                iterFirst = iterSecond;
                --iterFirst;

                s_double dCoef = (mValue - s_double(iterFirst->first));
                dCoef /= s_double(iterSecond->first - iterFirst->first);

                return iterFirst->second +
                       Elem(dCoef)*(iterSecond->second - iterFirst->second);
            }
        }
    }
}

#endif
