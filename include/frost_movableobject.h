/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*          MovableObject header          */
/*                                        */
/*                                        */


#ifndef FROST_MOVABLEOBJECT_H
#define FROST_MOVABLEOBJECT_H

#include "frost.h"

namespace Frost
{
    /// A class storing position, orientation and scale
    /** It's a base for every object in the 3D world.
    */
    class MovableObject : public EventReceiver
    {
    public :

        /// Default constructor.
        MovableObject();

        /// Copy constructor.
        MovableObject(const MovableObject& mObject);

        /// Constructor.
        MovableObject(const Vector& mPosition);

        /// Destructor.
        virtual ~MovableObject();

        /// Attaches this object to another.
        /** \param pObject       The new parent
        *   \param bInheritRot   'true' if you want your object to
        *                        rotate along with the other
        *   \param bInheritScale 'true' if you want your object to
        *                        be scaled along with the other
        *   \note If you move pObject, this object will follow it,
        *         but pObject wont move if you move this object.
        */
        virtual void          Attach(s_ptr<MovableObject> pObject, const s_bool& bInheritRot = true, const s_bool& bInheritScale = true);

        /// Removes the link this object had with its parent.
        /** \note Disables inheritance.
        */
        virtual void          Detach();

        /// Orientates the object to look at a certain point.
        /** \param mTrackedPoint          The point to look at
        *   \param bTrackedPointRelative 'true' if the point is relative to the parent's
        *                                 position
        *   \note Disables orbiting, enables tracking.
        */
        virtual void          LookAt(const Vector& mTrackedPoint, const s_bool& bTrackedPointRelative = false);

        /// Makes the object rotate around a point in the space.
        /** \param mOrbitCenter         The point around which the object will orbit
        *   \param bOrbitCenterRelative 'true' if the point is relative to this object's parent
        *   \note Disables tracking, enables orbiting.<br>
        *         If bRelative is 'false', this object will be completely detached from
        *         its parent.
        *
        */
        virtual void          OrbitAround(const Vector& mOrbitCenter, const s_bool& bOrbitCenterRelative = false);

        /// Orientates the object to a certain direction.
        /** \param mDirection The direction to look to
        *   \note Disables orbiting and tracking.
        */
        virtual void          SetDirection(const Vector& mDirection);

        /// Sets this object's position.
        /** \param mPosition The new position
        *   \note Disables orbiting and path following.
        */
        virtual void          SetPosition(const Vector& mPosition);

        /// Sets the path to be followed by this object.
        /** \param pPath The path to follow
        *   \note Disables orbiting, enables path following.
        */
        virtual void          SetPath(s_refptr<Path> pPath);

        /// Removes the path this object follows.
        virtual void          RemovePath();

        /// Translates this object.
        /** \param mTranslation The translation vector
        *   \param bLocalSpace  If set to 'true', translation is done in
        *                       the object's space instead of the world one.
        *   \note Disables orbiting and path following.
        */
        virtual void          Translate(const Vector& mTranslation, const s_bool& bLocalSpace = false);

        /// Rotates this object around its local Z axis (up).
        /** \param fValue A non dimensional angle (1 = 2*PI rad = 360°)
        *   \note If orbiting is on, this functions rotates the object
        *         around the orbit/look at point.
        */
        virtual void          Yaw(const s_float& fValue);

        /// Rotates this object around its local Y axis (side).
        /** \param fValue A non dimensional angle (1 = 2*PI rad = 360°)
        *   \note If orbiting is on, this functions rotates the object
        *         around the orbit/look at point.
        */
        virtual void          Pitch(const s_float& fValue);

        /// Rotates this object around its local X axis (direction).
        /** \param fValue A non dimensional angle (1 = 2*PI rad = 360°)
        */
        virtual void          Roll(const s_float& fValue);

        /// Tells this object to stop orbiting.
        /** \note Disables orbiting.
        */
        virtual void          UnlockOrbiting();

        /// Tells this object to stop tracking.
        /** \note Disables tracking.
        */
        virtual void          UnlockTracking();

        /// Checks whether this object is orbiting around a point.
        /** \return 'true' if this object is orbiting around a point
        */
        virtual const s_bool& IsOrbiting() const;

        /// Checks whether this object is tracking on a point.
        /** \return 'true' if this object is tracking on a point
        */
        virtual const s_bool& IsTracking() const;

        /// Returns the position of this object.
        /** \return The position of this object
        */
        Vector                GetPosition(const s_bool& bRelative = true) const;

        /// Returns the direction of this object.
        /** \return The direction of this object
        */
        Vector                GetDirection(const s_bool& bRelative = true) const;

        /// Returns the point this object is tracking on.
        /** \return The point this object is tracking on
        */
        const Vector&         GetTrackedPoint() const;

        /// Returns the point this object is orbiting around.
        /** \return The point this object is orbiting around
        */
        const Vector&         GetOrbitCenter() const;

        /// Returns the Path used by this object.
        /** \return The Path used by this object
        */
        s_refptr<Path>        GetPath();

        /// Returns the controlled node.
        /** \return The controlled node
        */
        s_ptr<Ogre::SceneNode> GetOgreNode();

        /// Update this object's parameters.
        /** \param fDelta The time elapsed since the last call
        *   \note Should be called by the derived class.
        */
        virtual void  Update(const s_float& fDelta);

        /// Called whenever an Event occurs.
        /** \param mEvent The Event which has occured
        */
        virtual void  OnEvent(const Event& mEvent);

        static const s_str CLASS_NAME;

    protected :

        /// Changes the controlled node
        /** \param pNode The new node to use
        */
        void  CreateOrbitNode_();
        void  RemoveOrbitNode_();

        s_ptr<MovableObject>    pParent_;

        s_ptr<Ogre::SceneNode>  pNode_;
        s_ptr<Ogre::SceneNode>  pTargetNode_;
        Vector                  mOrbitCenter_;
        Vector                  mTrackedPoint_;
        s_bool                  bOrbits_;
        s_bool                  bOrbitCenterRelative_;
        s_bool                  bTracks_;
        s_bool                  bTrackedPointRelative_;
        s_bool                  bInherits_;
        s_refptr<Path>          pPath_;
    };
}

#endif
