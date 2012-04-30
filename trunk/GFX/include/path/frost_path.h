/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*              Path header               */
/*                                        */
/*                                        */


#ifndef FROST_PATH_H
#define FROST_PATH_H

#include "frost.h"
#include <frost_utils_vector.h>

namespace Frost
{
    /// Virtual base for all paths.
    class Path
    {
    public :

        /// Handles movements along a Path (virtual class)
        class Iterator
        {
        public :

            /// Contructor.
            Iterator();

            /// Destructor.
            virtual ~Iterator();

            /// Sets whether the Path should be followed forever or just once.
            /** \param bLoop           'true' if you want the Path to be followed forever
            *   \param bLinkEndToStart 'true' if you want to link the last point of the
            *                          Path with the first one
            *   \note Default is 'false'.<br>
            *         This function can be overloaded if needed.
            */
            virtual void   SetLoop(const s_bool& bLoop, const s_bool& bLinkEndToStart = true);

            /// Sets the speed at which the Path should be followed.
            /** \param fSpeed The new speed
            *   \note Default is 20.0f.<br>
            *         You can use negative speeds : the function will then call
            *         Reverse() and keep your speed unsigned.<br>
            *         This function can be overloaded if needed.
            */
            virtual void   SetSpeed(const s_float& fSpeed);

            /// Changes the way to follow the Path
            /** \note By default, the Path is followed from its beginning to its end.
            *         You can change this behavior at anytime with this function.<br>
            *         Automatically called if you call SetSpeed() with a negative speed.<br>
            *         This function can be overloaded if needed.
            */
            virtual void   Reverse();

            /// Prevents the movement from updating.
            /** \note This function can be overloaded if needed.
            */
            virtual void   Pause();

            /// Allows the movement to update.
            /** \note This function can be overloaded if needed.
            */
            virtual void   Play();

            /// Updates movement on the Path (pure virtual).
            /** \param fDelta The time elapsed since the last call
            *   \note This function must be overloaded.
            */
            virtual void   Update(const s_float& fDelta) = 0;

            /// Returns the current point.
            /** \return The current point
            */
            const Vector&  GetPosition() const;

            /// Returns the movement speed.
            /** \return The movement speed
            *   \note This one will always be returned positive.<br>See SetSpeed().
            */
            const s_float& GetSpeed() const;

            /// Checks if movement has ended.
            /** \return 'true' if movement has ended
            */
            const s_bool&  IsEnded() const;

            /// Checks if movement has been paused.
            /** \return 'true' if movement has been paused
            */
            const s_bool&  IsPaused() const;

            /// Sets this iterator's Path.
            /** \param pPath The Path
            *   \note Should only be called once.
            */
            virtual void   SetPath(s_refptr<Path> pPath) = 0;

            /// Returns the associated Path.
            /** \return The associated Path
            */
            virtual s_wptr<Path> GetPath() const = 0;

            static const s_str CLASS_NAME;

        protected :

            s_float        fSpeed_;
            s_bool         bLoop_;
            s_bool         bLinkEndToStart_;
            s_bool         bEnded_;
            s_bool         bReversed_;
            s_bool         bPaused_;
            Vector         mPosition_;
        };

        virtual ~Path() {}

        /// Creates a new iterator to move along this path.
        /** \return The newly created iterator.
        */
        virtual s_refptr<Iterator> CreateIterator() const = 0;

        static const s_str CLASS_NAME;

        /// Creates a new iterator for a Path.
        /** \return The newly created iterator
        */
        static s_refptr<Path::Iterator> CreateIterator(s_wptr<Path> pPath);
    };
}

#endif
