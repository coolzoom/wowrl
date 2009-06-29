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

        /// Creates a new light.
        /** \param mType The type of this light
        *   \return The new light
        */
        s_ptr<Light> CreateLight(LightType mType);

        /// Deletes a light.
        /** \param pLight The light to delete
        *   \note All light are automatically deleted
        *         when this manager is deleted.
        */
        void         DeleteLight(s_ptr<Light> pLight);

        /// Updates all lights.
        /** \param fDelta The time elapsed since the last call
        *   \note Automatically called by Engine.
        */
        void         UpdateLights(const s_float& fDelta);

        /// Sets the ambient light color.
        /** \param mAmbient The new ambient light color
        */
        void         SetAmbient(const Color& mAmbient);

        /// Returns the ambient light color.
        /** \return The ambient light color
        */
        const Color& GetAmbient() const;

        static const s_str CLASS_NAME;

    protected :

        /// Default constructor
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

        /// Destructor
        /** \note If GetSingleton() has been called at least once,
        *         you'll have to delete the returned pointer when
        *         you close your program by calling Delete() (this is
        *         automatically done by Engine).
        */
        ~LightManager();

        /// Copy constructor
        LightManager(const LightManager& mMgr);

        /// Assignment operator
        LightManager& operator = (const LightManager& mMgr);

    private :

        s_map<s_uint, s_ptr<Light> > lLightList_;

        Color mAmbient_;

    };
}

#endif
