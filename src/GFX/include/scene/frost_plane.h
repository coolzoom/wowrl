/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*              Plane header              */
/*                                        */
/*                                        */


#ifndef FROST_PLANE_H
#define FROST_PLANE_H

#include "frost.h"
#include "scene/frost_movableobject.h"

#include <OgreUserDefinedObject.h>

namespace Frost
{
    /// A plane mesh
    class Plane : public MovableObject
    {
    public :

        /// Constructor.
        /** \note You shouldn't have to call this. Use the
        *         SceneManager instead.
        */
        Plane();

        /// Constructor.
        /** \param fWidth   The width of this Plane
        *   \param fHeight  The height of this Plane
        *   \param fDensity The number of segment per meter
        *   \note You shouldn't have to call this. Use the
        *         SceneManager instead.
        */
        Plane(const s_float& fWidth, const s_float& fHeight, const s_float& fDensity = 0.5f);

        /// Destructor.
        ~Plane();

        /// Makes this Plane visible.
        void Show();

        /// Makes this Plane invisible.
        void Hide();

        /// Sets this Plane's Material.
        /** \param pMat The new Material
        */
        void SetMaterial(s_refptr<Material> pMat);

        /// Returns this Plane's Material.
        /** \return This Plane's Material
        */
        s_wptr<Material> GetMaterial();

        static const s_str CLASS_NAME;

    private :

        s_ptr<Ogre::Entity> pEntity_;
        s_refptr<Material>  pMaterial_;

    };
}

#endif
