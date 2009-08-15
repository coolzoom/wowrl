/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*         MaterialManager header         */
/*                                        */
/*                                        */


#ifndef FROST_MATERIALMANAGER_H
#define FROST_MATERIALMANAGER_H

#include "frost.h"
#include "utils/frost_utils_manager.h"

namespace Frost
{
    /// Manages creation of Material
    class MaterialManager : public Manager<MaterialManager>
    {
    friend class Manager<MaterialManager>;
    public :

        /// Initializes this manager.
        /** \note Automatically called by Engine.
        */
        void      Initialize();

        /// Creates a Material to wrap an existing Ogre::Material.
        /** \param pOgreMat The Ogre::Material to wrap
        */
        s_refptr<Material> CreateMaterial(s_ptr<Ogre::Material> pOgreMat);

        /// Creates a Material to wrap an existing Ogre::Material.
        /** \param pOgreMat The Ogre::Material to wrap
        *   \note Also performs needed adjustments for 2D rendering
        */
        s_refptr<Material> CreateMaterial2D(s_ptr<Ogre::Material> pOgreMat);

        /// Creates a new Material (for 2D rendering).
        /** \param sName The name of the new Material
        *   \param uiR   The red component
        *   \param uiG   The green component
        *   \param uiB   The blue component
        *   \return The new Material
        *   \note Creates a blank Material filled with a color
        */
        s_refptr<Material> CreateMaterial2D(const s_str& sName, const s_uint& uiR, const s_uint& uiG, const s_uint& uiB);

        /// Creates a new Material (for 2D rendering).
        /** \param sName  The name of the new Material
        *   \param mColor The color of this Material
        *   \return The new Material
        *   \note Creates a blank Material filled with a color
        */
        s_refptr<Material> CreateMaterial2D(const s_str& sName, const Color& mColor);

        /// Creates a new Material (for 2D rendering).
        /** \param sFileName The file name of the texture to use
        *   \return The new Material
        *   \note Automatically loads the texture if needed.
        */
        s_refptr<Material> CreateMaterial2D(const s_str& sFileName);

        /// Creates a new Material (for 2D rendering).
        /** \param sRenderTargetName The file name of the texture to use
        *   \return The new Material
        *   \note The render target must have been created before.
        */
        s_refptr<Material> CreateMaterial2DFromRT(const s_str& sRenderTargetName);

        /// Creates a new Material (for 2D rendering).
        /** \param pRenderTarget The render target to use
        *   \return The new Material
        */
        s_refptr<Material> CreateMaterial2DFromRT(s_ptr<RenderTarget> pRenderTarget);

        /// Returns the default Material (for 2D rendering).
        /** \return The default Material (for 2D rendering)
        */
        s_refptr<Material> GetDefault2D();

        /// Creates a new Material (for 3D rendering).
        /** \param sName The name of the new Material
        *   \param uiR   The red component
        *   \param uiG   The green component
        *   \param uiB   The blue component
        *   \return The new Material
        *   \note Creates a blank Material filled with a color
        */
        s_refptr<Material> CreateMaterial3D(const s_str& sName, const s_uint& uiR, const s_uint& uiG, const s_uint& uiB);

        /// Creates a new Material (for 3D rendering).
        /** \param sFileName The file name of the texture to use
        *   \return The new Material
        *   \note Automatically loads the texture if needed.
        */
        s_refptr<Material> CreateMaterial3D(const s_str& sFileName);

        /// Returns the default Material (for 3D rendering).
        /** \return The default Material (for 3D rendering)
        */
        s_refptr<Material> GetDefault3D();

        static const s_str CLASS_NAME;

    protected :

        /// Default constructor.
        /** \note This is a singleton class. That means that there can
        *         only be a single instance at the same time. That's why
        *         the constructor is not in public scope.<br>
        *         If you need to get a pointer to that unique instance,
        *         call MaterialManager::GetSingleton().<br>
        *         Note that, if GetSingleton() has been called at least once,
        *         you'll have to delete the returned pointer when you close
        *         your program by calling MaterialManager::Delete() (this is
        *         automatically done by Engine).
        */
        MaterialManager();

        /// Destructor.
        /** \note If GetSingleton() has been called at least once,
        *         you'll have to delete the returned pointer when
        *         you close your program by calling Delete() (this is
        *         automatically done by Engine).
        */
        ~MaterialManager();

        /// Copy constructor.
        MaterialManager(const MaterialManager& mMgr);

        /// Assignment operator.
        MaterialManager& operator = (const MaterialManager& mMgr);

    private :

        s_uint uiCounter_;

        s_refptr<Material> pDefault2D_;
        s_refptr<Material> pDefault3D_;
    };
}

#endif
