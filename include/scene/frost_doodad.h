/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*             Doodad header              */
/*                                        */
/*                                        */


#ifndef FROST_DOODAD_H
#define FROST_DOODAD_H

#include "frost.h"
#include "scene/frost_movableobject.h"

namespace Frost
{
    /// Makes the link between Doodad and Ogre::MovableObject.
    class DoodadOgreInterface : public OgreInterface
    {
    public :

        /// Sets the Doodad linked to this interface.
        /** \param pDoodad The Doodad
        */
        void  SetDoodad(s_ptr<Doodad> pDoodad);

        /// Checks if this object can be selected or not.
        /** \return 'true' if this object can be selected
        */
        s_bool IsSelectable() const;

        /// Checks if this object can react to mouse events.
        /** \return 'true' if this object can react to mouse events
        */
        s_bool IsMouseEnabled() const;

        /// Callback to react to events.
        /** \param sEvent The name of the event
        */
        void   On(const s_str& sEvent);

    private :

        s_ptr<Doodad> pDoodad_;

    };

    /// A decorative object in a Zone.
    class Doodad : public MovableObject
    {
    public :

        /// Constructor.
        /** \param sName      The name of this Doodad
        *   \param sModelName The name of the model to use
        *   \param pParent    The zone this Doodad belongs to
        *   \note You shouldn't have to call this. Use Zone::AddDoodad() instead.
        */
        Doodad(const s_str& sName, const s_str& sModelName, s_ptr<Zone> pParent);

        /// Destructor.
        ~Doodad();

        /// Makes this Doodad visible.
        void          Show();

        /// Makes this Doodad invisible.
        void          Hide();

        /// Checks if this Doodad is visible.
        /** \return 'true' if this Doodad is visible
        */
        const s_bool& IsShown();

        /// Makes this Doodad fully lighted (no shadow).
        /** \param bHighlighted 'true' to make the Doodad highlighted
        */
        void          Highlight(const s_bool& bHighlighted);

        /// Selects this Doodad.
        /** \param bSelected 'true' to select the Doodad
        */
        void          Select(const s_bool& bSelected);

        /// Shows this Doodad's gizmo.
        void          ShowGizmo();

        /// Hides this Doodad's gizmo.
        void          HideGizmo();

        /// Update this object's parameters.
        /** \param fDelta The time elapsed since the last call
        */
        void          Update(const s_float& fDelta);

        /// Returns this Doodad's model.
        /** \return This Doodad's model
        */
        s_wptr<Model> GetModel();

        /// Returns this Doodad's name.
        /** \return This Doodad's name
        */
        const s_str&  GetName() const;

        /// Returns this Doodad's Model's name.
        /** \return This Doodad's Model's name
        */
        const s_str&  GetModelName() const;

        static const s_str CLASS_NAME;

    private :

        s_str       sName_;
        s_str       sModelName_;
        s_ptr<Zone> pParent_;

        s_bool bIsShown_;
        s_bool bHighlighted_;
        s_bool bSelected_;

        s_refptr<Model>     pModel_;
        DoodadOgreInterface mOgreInterface_;

        s_ptr<Gizmo> pGizmo_;
    };
}

#endif
