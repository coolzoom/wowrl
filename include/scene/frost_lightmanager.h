/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*          LightManager header           */
/*                                        */
/*                                        */


#ifndef FROST_LIGHTMANAGER_H
#define FROST_LIGHTMANAGER_H

#include "frost.h"
#include "utils/frost_utils_manager.h"
#include "scene/frost_light.h"

namespace Frost
{
    /// Manages Light creation.
    class LightManager : public Manager<LightManager>
    {
    friend class Manager<LightManager>;
    public :

        /// Initializes this manager.
        void         Initialize();

        /// Sets the sun's light direction.
        /** \param mSunDir The new direction
        */
        void           SetSunDirection(const Vector& mSunDir);

        /// Returns the sun's light direction.
        /** \return The sun's light direction
        */
        const Vector&  GetSunDirection() const;

        /// Sets the sun's light color.
        /** \param mSunColor The new color
        */
        void           SetSunColor(const Color& mSunColor);

        /// Returns the sun's light color.
        /** \return The sun's light color
        */
        const Color&   GetSunColor() const;

        /// Creates a new light.
        /** \param mType The type of this light
        *   \return The new light
        */
        s_ptr<Light>   CreateLight(Light::Type mType);

        /// Deletes a light.
        /** \param pLight The light to delete
        *   \note All light are automatically deleted
        *         when this manager is deleted.
        */
        void           DeleteLight(s_ptr<Light> pLight);

        /// Updates all lights.
        /** \param fDelta The time elapsed since the last call
        *   \note Automatically called by Engine.
        */
        void           UpdateLights(const s_float& fDelta);

        /// Sets the ambient light color.
        /** \param mAmbient The new ambient light color
        */
        void           SetAmbient(const Color& mAmbient);

        /// Returns the ambient light color.
        /** \return The ambient light color
        */
        const Color&   GetAmbient() const;

        /// Checks if the ambient color is locked.
        /** \return 'true' if the ambient color is locked
        */
        const s_bool&  IsAmbientLocked() const;

        /// Forces the ambient color.
        /** \param mLockedAmbient The new ambient color
        *   \note If the ambient color is locked, calling
        *         SetAmbient() will have no effect. You'll
        *         have to call UnlockAmbient() before.
        */
        void           LockAmbient(const Color& mLockedAmbient);

        /// Unlocks the ambient color.
        void           UnlockAmbient();

        static const s_str CLASS_NAME;

    protected :

        /// Default constructor.
        /** \note This is a singleton class. That means that there can
        *         only be a single instance at the same time. That's why
        *         the constructor is not in public scope.<br>
        *         If you need to get a pointer to that unique instance,
        *         call LightManager::GetSingleton().<br>
        *         Note that, if GetSingleton() has been called at least once,
        *         you'll have to delete the returned pointer when you close
        *         your program by calling LightManager::Delete() (this is
        *         automatically done by Engine).
        */
        LightManager();

        /// Destructor.
        /** \note If GetSingleton() has been called at least once,
        *         you'll have to delete the returned pointer when
        *         you close your program by calling Delete() (this is
        *         automatically done by Engine).
        */
        ~LightManager();

        /// Copy constructor.
        LightManager(const LightManager& mMgr);

        /// Assignment operator.
        LightManager& operator = (const LightManager& mMgr);

    private :

        s_map<s_uint, s_ptr<Light> > lLightList_;

        Color  mAmbient_;
        Color  mLockedAmbient_;
        s_bool bAmbientLocked_;

        Vector mSunDir_;
        Color  mSunColor_;
    };
}

#endif
