/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*          MovableObject header          */
/*                                        */
/*                                        */


#ifndef FROST_MOVABLEOBJECT_H
#define FROST_MOVABLEOBJECT_H

#include "frost.h"

#include "path/frost_path.h"

#include <OgreUserDefinedObject.h>
#include <OgreQuaternion.h>

namespace Frost
{
    class OgreInterface : public Ogre::UserDefinedObject
    {
    public :

        /// Constructor.
        OgreInterface();

        /// Destructor.
        virtual ~OgreInterface();

        /// Sets the associated MovableObject.
        /** \param pCharacter The associated MovableObject
        */
        void                 SetMovableObject(s_ptr<MovableObject> pMovableObject);

        /// Returns the associated MovableObject.
        /** \return The associated MovableObject
        */
        s_ptr<MovableObject> GetMovableObject() const;

        /// Sets the priority of this object.
        /** \param iPriority The priority
        *   \note No matter if there is an object in front of
        *         this one : if its priority is higher, it will
        *         be picked in place of the other one.<br>
        *         Default is 0.
        */
        void                 SetPriority(const s_int& iPriority);

        /// Returns the priority of this object.
        /** \return The priority of this object
        */
        const s_int&         GetPriority() const;

        /// Allows this object to react to mouse events.
        /** \param bEnable 'true' to enable mouse events
        */
        void                 EnableMouse(const s_bool& bEnable);

        /// Checks if this object can react to mouse events.
        /** \return 'true' if this object can react to mouse events
        */
        const s_bool&        IsMouseEnabled() const;

        /// Checks if this object can be selected or not.
        /** \return 'true' if this object can be selected
        */
        virtual s_bool       IsSelectable() const;

        /// Callback to react to events.
        /** \param sEvent The name of the event
        */
        virtual void         On(const s_str& sEvent) = 0;

    private :

        s_ptr<MovableObject> pMovableObject_;
        s_int                iPriority_;
        s_bool               bMouseEnabled_;
    };

    /// A class storing position, orientation and scale
    /** It's a base for every object in the 3D world.
    */
    class MovableObject : public EventReceiver
    {
    public :

        /// Default constructor.
        MovableObject();

        /// Constructor.
        MovableObject(const Vector& mPosition);

        /// Copy constructor.
        MovableObject(const MovableObject& mObject);

        /// Destructor.
        virtual ~MovableObject();

        /// Sets the OgreInterface object to use as a callback.
        /** \param pOgreInterface The callback object
        *   \note  This movable object <b>must</b> have an entity for
        *          the callback to work properly.
        */
        void                  SetOgreInterface(s_ptr<OgreInterface> pOgreInterface);

        /// Attaches this object to another.
        /** \param pObject       The new parent
        *   \param bInheritRot   'true' if you want your object to
        *                        rotate along with the other
        *   \param bInheritScale 'true' if you want your object to
        *                        be scaled along with the other
        *   \note If you move pObject, this object will follow it,
        *         but pObject wont move if you move this object.
        */
        virtual void          AttachTo(s_ptr<MovableObject> pObject, const s_bool& bInheritRot = true, const s_bool& bInheritScale = true);

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

        /// Orientates the object to look at another object.
        /** \param pObject The object to look at
        *   \note Disables orbiting, enables tracking.
        */
        virtual void          LookAt(s_ptr<MovableObject> pObject);

        /// Tells this object that it is being looked at by another one.
        /** \param pObject    The object that looks at this one
        *   \param bLookingAt 'true' is the object is looking at this one
        */
        virtual void          NotifyLookedAt(s_ptr<MovableObject> pObject, const s_bool& bLookingAt);

        /// Makes the object rotate around a point in the space.
        /** \param mOrbitCenter         The point around which the object will orbit
        *   \note Disables tracking, enables orbiting.<br>
        *
        */
        virtual void          OrbitAround(const Vector& mOrbitCenter);

        /// Orientates the object to a certain direction.
        /** \param mDirection The direction to look to
        *   \note Disables orbiting and tracking.
        */
        virtual void          SetDirection(const Vector& mDirection);

        /// Orientates the object to a certain direction.
        /** \param fX The first coordinate of the quaternion
        *   \param fY The second coordinate of the quaternion
        *   \param fZ The third coordinate of the quaternion
        *   \param fW The fourth coordinate of the quaternion
        *   \note Disables orbiting and tracking.
        */
        virtual void          SetOrientation(const s_float& fX, const s_float& fY, const s_float& fZ, const s_float& fW);

        /// Sets this object's scale.
        /** \param mScale The new scale
        */
        virtual void          SetScale(const Vector& mScale);

        /// Sets this object's reference direction.
        /** \param mInitialDirection The initial direction
        *   \note This is where the object points at when it is created.<br>
        *         Default is -z.
        */
        void                  SetInitialDirection(const Vector& mInitialDirection);

        /// Sets this object's position.
        /** \param mPosition The new position
        *   \note Disables orbiting and path following.
        */
        virtual void          SetPosition(const Vector& mPosition);

        /// Sets the path to be followed by this object.
        /** \param pPath The path to follow
        *   \note Disables orbiting, enables path following.
        */
        virtual s_wptr<Path::Iterator> SetPath(s_wptr<Path> pPath);

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

        /// Transforms a point in the local space into a point in the world space.
        /** \param mPoint The point to transform
        *   \return The transformed point
        */
        Vector                Transform(const Vector& mPoint) const;

        /// Returns the position of this object.
        /** \return The position of this object
        */
        Vector                GetPosition(const s_bool& bRelative = true) const;

        /// Returns the scale of this object.
        /** \return The scale of this object
        */
        Vector                GetScale(const s_bool& bRelative = true) const;

        /// Returns the direction of this object.
        /** \return The direction of this object
        */
        Vector                GetDirection(const s_bool& bRelative = true) const;

        /// Returns this object's reference direction.
        /** \return This object's reference direction
        *   \note This is where the object points at when it is created.
        */
        const Vector&         GetInitialDirection() const;

        /// Returns the orientation of this object.
        /** \return The orientation of this object
        */
        Ogre::Quaternion      GetOrientation(const s_bool& bRelative = true) const;

        /// Returns the point this object is tracking on.
        /** \return The point this object is tracking on
        */
        const Vector&         GetTrackedPoint() const;

        /// Returns the point this object is orbiting around.
        /** \return The point this object is orbiting around
        */
        const Vector&         GetOrbitCenter() const;

        /// Returns the node this object is orbiting around.
        /** \return The node this object is orbiting around
        */
        s_ptr<Ogre::SceneNode> GetOrbitNode();

        /// Returns the Path used by this object.
        /** \return The Path used by this object
        */
        s_wptr<Path>          GetPath();

        /// Returns the Path::Iterator used by this object.
        /** \return The Path::Iterator used by this object
        */
        s_wptr<Path::Iterator> GetPathIterator();

        /// Returns the controlled node.
        /** \return The controlled node
        */
        s_ptr<Ogre::SceneNode> GetOgreNode();

        /// Returns this MovableObject's entity (if any).
        /** \return This MovableObject's entity (if any)
        */
        s_ptr<Ogre::Entity>   GetOgreEntity();

        /// Returns this MovableObject's unique ID.
        /** \return This MovableObject's unique ID
        */
        const s_uint& GetID() const;

        /// Update this object's parameters.
        /** \param fDelta The time elapsed since the last call
        *   \note Should be called by the derived class.
        */
        virtual void  Update(const s_float& fDelta);

        /// Called whenever an Event occurs.
        /** \param mEvent The Event which has occured
        */
        virtual void  OnEvent(const Event& mEvent);

        /// Creates the associated Lua glue.
        /** \param pLua The Lua::State on which to create the glue
        */
        virtual void  CreateGlue(s_ptr<Lua::State> pLua);

        /// Pushes this MovableObject on the provided Lua::State.
        /** \param pLua The Lua::State on which to push the glue
        */
        virtual void  PushOnLua(s_ptr<Lua::State> pLua) const;

        static const s_str CLASS_NAME;

    protected :

        void  CreateOrbitNode_();
        void  RemoveOrbitNode_();

        s_uint uiID_;

        s_ptr<MovableObject> pParent_;
        s_bool               bInherits_;
        s_ptr<MovableObject> pLookAtObject_;

        Vector mInitialDirection_;

        s_ctnr< s_ptr<MovableObject> > lLookingAtList_;

        s_ptr<Ogre::Entity>     pEntity_;
        s_ptr<Ogre::SceneNode>  pNode_;
        s_ptr<Ogre::SceneNode>  pTargetNode_;

        Vector mOrbitCenter_;
        Vector mTrackedPoint_;
        s_bool bOrbits_;
        s_bool bOrbitCenterRelative_;
        s_bool bTracks_;
        s_bool bTrackedPointRelative_;

        s_refptr<Path::Iterator> pPathIterator_;

        s_ctnr< s_ptr<LuaMovableObject> > lGlueList_;
    };

    /** \cond NOT_REMOVE_FROM_DOC
    */

    /// MovableObject Lua glue
    class LuaMovableObject
    {
    public :

        /// Constructor.
        LuaMovableObject(lua_State* pLua);

        int _Translate(lua_State*);
        int _TranslateOrbitNode(lua_State*);
        int _OrbitAround(lua_State*);
        int _GetDirection(lua_State*);
        int _SetDirection(lua_State*);
        int _Yaw(lua_State*);
        int _Pitch(lua_State*);
        int _Roll(lua_State*);

        s_ptr<MovableObject> GetObject();

        int GetDataTable(lua_State *L);
        static const char className[];
        static const char* classList[];
        static Lunar<LuaMovableObject>::RegType methods[];
        static const s_str CLASS_NAME;

    protected :

        s_ptr<MovableObject> pParent_;

        lua_State* pLua_;
        int        iRef_;
    };

    /** \endcond
    */
}

#endif
