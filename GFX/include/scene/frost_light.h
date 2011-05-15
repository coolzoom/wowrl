/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*              Light header              */
/*                                        */
/*                                        */


#ifndef FROST_LIGHT_H
#define FROST_LIGHT_H

#include "frost.h"
#include "scene/frost_movableobject.h"

#include <frost_utils_color.h>

namespace Frost
{

    /// A wrapper around Ogre's Light
    class Light : public MovableObject
    {
    public:

        enum Type
        {
            POINT,
            DIRECTIONAL,
            SPOT
        };

        /// Constructor.
        /** \param mType         The type of this light, see Type
        *   \param pSceneManager The scene manager on which to create this light
        *   \note If no scene manager is provided, the default one is used
        */
        Light(Type mType, s_ptr<Ogre::SceneManager> pSceneManager = nullptr );

        /// Destructor.
        ~Light();

        /// Sets this light's diffuse color.
        /** \param mDiffuse The new color (see Color())
        */
        void          SetColor(const Color& mDiffuse);

        /// Sets this light's specular color.
        /** \param mSpecular The new color (see Color())
        */
        void          SetSpecularColor(const Color& mSpecular);

        /// Returns this light's diffuse color.
        /** \return This light's diffuse color
        */
        const Color&  GetColor() const;

        /// Returns this light's specular color.
        /** \return This light's specular color
        */
        const Color&  GetSpecularColor() const;

        /// Sets this light's direction.
        /** \param mDir The new direction
        *   \note Only works for directionnal and spot lights.
        */
        void          SetDirection(const Vector& mDir);

        /// Sets the spot angles.
        /** \param fInAngle  Inner angle (non dimensional angle)
        *   \param fOutAngle Outer angle (non dimensional angle)
        *   \note Only works for spot lights.
        */
        void          SetSpotDimensions(const s_float& fInAngle, const s_float& fOutAngle);

        /// Sets the spot falloff.
        /** \param fFalloff The new falloff
        *   \note High value means slow transition, 1.0f is linear,
        *         low value means sharp transition.<br>
        *         Only works for spot lights.
        */
        void          SetSpotFalloff(const s_float& fFalloff);

        /// Sets the light's attenuation factors.
        /** \param fConstant  The constant factor
        *   \param fLinear    The linear factor
        *   \param fQuadratic The quadratic factor
        *   \note Formula is :<br>
        *         fFinalAttenuation = 1/(fConstant + fLinear*fDistance + fQuadratic*fDistance*fDistance)<br>
        *         ... where fDistance is the distance from the vertex/pixel to the light.
        */
        void          SetAttenuation(const s_float& fConstant, const s_float& fLinear, const s_float& fQuadratic);

        /// Sets the light's range.
        /** \param fRange The new range
        *   \note This light won't be casted on vertice/pixels farther than
        *         the range value.
        */
        void          SetRange(const s_float& fRange);

        /// Returns the associated Ogre::Light.
        /** \return The associated Ogre::Light
        */
        s_ptr<Ogre::Light> GetOgreLight();

        /// Returns this light's ID.
        /** \return This light's ID
        */
        const s_uint& GetID() const;

        /// Update this light's settings.
        /** \param fDelta The time elapsed since the last call
        *   \note Automatically called by LightManager.
        */
        void          Update(const s_float& fDelta);

        static const s_str CLASS_NAME;

    private:

        Type    mType_;
        Color   mDiffuse_;
        Color   mSpecular_;
        s_float fFalloff_;
        s_float fInAngle_;
        s_float fOutAngle_;

        s_array<s_float, 4> lAttenFactors_;
        s_bool              bChangeFactors_;

        s_ptr<Ogre::Light> pOgreLight_;
    };
}

#endif
