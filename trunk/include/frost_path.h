/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*               XXX header               */
/*                                        */
/*                                        */


#ifndef FROST_PATH_H
#define FROST_PATH_H

#include "frost.h"

namespace Frost
{
    enum PathType
    {
        /// Linear interpolation
        PATH_DIRECT,
        /// Spline interpolation (Catmull-Rom)
        PATH_SMOOTH,
        /// Manual interpolation (Hermite)
        PATH_MANUAL,
        /// Anything but the above three
        PATH_OTHER
    };

    /// Handles movements along a path (virtual class)
    class Path
    {
    public :

        /// Constructor
        /** \param uiID  The unique ID to give to that path
        *   \param mType The type of this path
        *   \note You shouldn't have to call this. Use the
        *         PathManager instead.
        */
        Path(const s_uint& uiID, PathType mType);

        /// Destructor
        virtual ~Path();

        /// Sets whether this path should be followed forever or just once.
        /** \param bLoop           'true' if you want the path to be followed forever
        *   \param bLinkEndToStart 'true' if you want to link the last point of the
        *                          path with the first one
        *   \note Default is 'false'.<br>
        *         This function can be overloaded if needed.
        */
        virtual void     SetLoop(const s_bool& bLoop, const s_bool& bLinkEndToStart = true);

        /// Sets the speed at which this path should be followed.
        /** \param fSpeed The new speed
        *   \note Default is 20.0f.<br>
        *         You can use negative speeds : the function will then call
        *         Reverse() and keep your speed unsigned.<br>
        *         This function can be overloaded if needed.
        */
        virtual void     SetSpeed(const s_float& fSpeed);

        /// Changes the way to follow the path
        /** \note By default, the path is followed from its beginning to its end.
        *         You can change this behavior at anytime with this function.<br>
        *         Automatically called if you call SetSpeed() with a negative speed.<br>
        *         This function can be overloaded if needed.
        */
        virtual void     Reverse();

        /// Prevents the path from updating.
        /** \note This function can be overloaded if needed.
        */
        virtual void     Pause();

        /// Allows the path to update itself.
        /** \note This function can be overloaded if needed.
        */
        virtual void     Play();

        /// Updates this path (pure virtual).
        /** \param fDelta The time elapsed since the last call
        *   \note This function must be overloaded.
        */
        virtual void     Update(const s_float& fDelta) = 0;

        /// Returns the actual point.
        /** \return The actual point
        */
        const Vector&    GetActualPoint() const;

        /// Returns this path's unique ID.
        /** \return This path's unique ID
        */
        const s_uint&    GetID() const;

        /// Returns the type of this path.
        /** \return The type of this path
        */
        PathType         GetType() const;

        /// Returns this path's speed.
        /** \return This path's speed
        *   \note This one will always be returned positive.<br>See SetSpeed().
        */
        const s_float&   GetSpeed() const;

        /// Checks if this path is ended.
        /** \return 'true' if this path is ended
        */
        const s_bool&    IsEnded() const;

        /// Checks if this path is paused.
        /** \return 'true' if this path is paused
        */
        const s_bool&    IsPaused() const;

        static const s_str CLASS_NAME;

    protected :

        s_uint                 uiID_;
        PathType               mType_;

        s_float                fSpeed_;
        s_bool                 bLoop_;
        s_bool                 bLinkEndToStart_;
        s_bool                 bEnded_;
        s_bool                 bReversed_;
        s_bool                 bPaused_;
        Vector                 mActualPoint_;

    };
}

#endif
