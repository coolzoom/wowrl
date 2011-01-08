/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*              Gizmo header              */
/*                                        */
/*                                        */


#ifndef FROST_GIZMO_H
#define FROST_GIZMO_H

#include "frost.h"
#include "scene/frost_movableobject.h"
#include "scene/frost_planeobstacle.h"

namespace Frost
{
    /// Makes the link between Gizmo and Ogre::MovableObject.
    class GizmoOgreInterface : public OgreInterface
    {
    public :

        /// Sets the Gizmo linked to this interface.
        /** \param pGizmo The Gizmo
        *   \param cAxis  The letter of the axis
        */
        void   SetGizmo(s_ptr<Gizmo> pGizmo, const s_char& cAxis);

        /// Checks if this object can react to mouse events.
        /** \return 'true' if this object can react to mouse events
        */
        s_bool IsMouseEnabled() const;

        /// Callback to react to events.
        /** \param sEvent The name of the event
        */
        void   On(const s_str& sEvent);

    private :

        s_ptr<Gizmo> pGizmo_;
        s_char       cAxis_;

    };

    /// An object to manipulate others.
    class Gizmo : public MovableObject
    {
    public :

        /// Constructor.
        Gizmo();

        /// Constructor.
        Gizmo(const Vector& mPos);

        /// Destructor.
        ~Gizmo();

        void Show();

        void Hide();

        void Highlight(const s_char& cAxis);

        void Unlight(const s_char& cAxis);

        const s_bool& IsShown() const;

        void SetControlledObject(s_ptr<MovableObject> pObj);

        s_ptr<MovableObject> GetControlledObject();

        void StartDrag(const s_char& cAxis);

        void StopDrag(const s_char& cAxis);

        /// Update this object's parameters.
        /** \param fDelta The time elapsed since the last call
        */
        void  Update(const s_float& fDelta);

        static const Color X_BASE_COLOR;
        static const Color Y_BASE_COLOR;
        static const Color Z_BASE_COLOR;

        static const Color X_HIGHLIGHT_COLOR;
        static const Color Y_HIGHLIGHT_COLOR;
        static const Color Z_HIGHLIGHT_COLOR;

        static const Color ALPHA_MASK;

        static const s_str CLASS_NAME;

    private :

        void CreateAxis_();

        struct Axis
        {
            s_uint             uiID;
            s_refptr<Model>    pModel;
            GizmoOgreInterface mOgreInterface;
            s_bool             bHighlighted;
            s_bool             bDragged;

            s_array<s_refptr<PlaneObstacle>,3> lDragPlaneList;
            s_wptr<PlaneObstacle>              pDraggedPlane;
            Vector                             mOldDragPos;
            s_float                            fOldDistance;
        };

        s_array<Axis,3> lAxisList_;
        s_bool          bIsShown_;

        s_ptr<MovableObject> pControlledObject_;
    };
}

#endif
