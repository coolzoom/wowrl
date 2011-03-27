/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*          SceneManager header           */
/*                                        */
/*                                        */


#ifndef FROST_SCENEMANAGER_H
#define FROST_SCENEMANAGER_H

#include "frost.h"

#include <frost_utils_manager.h>
#include <frost_utils_vector.h>

namespace Frost
{
    /// Manages creation of 3D objects
    class SceneManager : public Manager<SceneManager>
    {
    friend class Manager<SceneManager>;
    public :

        /// Initializes this manager.
        /** \note Automatically called by Engine.
        */
        void           Initialize();

        /// Creates a new Plane.
        /** \note The created plane will be "infinite".
        *         Actually, its width and height will be
        *         set to 1500. If you need a larger one,
        *         use the other CreatePlane method.
        *   \return The created Plane
        */
        s_ptr<Plane>   CreatePlane();

        /// Creates a new Plane.
        /** \param fWidth   The width of this plane
        *   \param fHeight  The height of this plane
        *   \param fDensity The number of segment per meter
        *   \return The created Plane
        */
        s_ptr<Plane>   CreatePlane(const s_float& fWidth, const s_float& fHeight, const s_float& fDensity = 0.5f);

        /// Creates a new Node.
        /** \param mPos The position to give to this Node.
        *   \return The created Node
        */
        s_ptr<Node>    CreateNode(const Vector& mPos = Vector::ZERO);

        /// Creates a new Gizmo.
        /** \param mPos The position to give to the Gizmo
        *   \return The created Gizmo
        */
        s_ptr<Gizmo>   CreateGizmo(const Vector& mPos = Vector::ZERO);

        /// Returns the MovableObject associated to the provided id.
        /** \param uiID The ID of the MovableObject you're after
        *   \return The MovableObject associated to the provided id
        */
        s_ptr<MovableObject> GetMovableObjectByID(const s_uint& uiID) const;

        /// Returns the SceneObject associated to the provided id.
        /** \param uiID The ID of the SceneObject you're after
        *   \return The SceneObject associated to the provided id
        */
        s_ptr<SceneObject> GetSceneObjectByID(const s_uint& uiID) const;

        /// Deletes a particular Plane.
        /** \param pPlane The Plane to delete
        *   \note All planes created by this manager are automatically deleted
        *         when the application closes.
        */
        void           DeletePlane(s_ptr<Plane> pPlane);

        /// Deletes a particular Node.
        /** \param pNode The Node to delete
        *   \note All nodes created by this manager are automatically deleted
        *         when the application closes.
        */
        void           DeleteNode(s_ptr<Node> pNode);

        /// Deletes a particular Gizmo.
        /** \param pGizmo The Gizmo to delete
        *   \note All gizmos created by this manager are automatically deleted
        *         when the application closes.
        */
        void           DeleteGizmo(s_ptr<Gizmo> pGizmo);

        /// Starts moving an object.
        /** \param pObject     The object to move
        *   \param mConstraint The axis on which translation must be constrained
        *   \param bOnGround   'true' to force the object to remain on the ground
        */
        void           StartMoving(
            s_ptr<MovableObject> pObject,
            Vector::Constraint   mConstraint = Vector::CONSTRAINT_NONE,
            const s_bool&        bOnGround   = false
        );

        /// Starts sizing an object.
        /** \param pObject     The object to move
        *   \param mConstraint The axis on which scaling must be constrained
        */
        void           StartSizing(
            s_ptr<MovableObject> pObject,
            Vector::Constraint   mConstraint = Vector::CONSTRAINT_NONE
        );

        /// Start rotating an object.
        /** \param pObject     The object to move
        *   \param mConstraint The axis on which rotation must be constrained
        */
        void           StartRotating(
            s_ptr<MovableObject> pObject,
            Vector::Constraint   mConstraint = Vector::CONSTRAINT_NONE
        );

        /// Deselects all objects and selects a new scene object.
        /** \param pObj The object to select
        */
        void           SelectSceneObject(s_ptr<SceneObject> pObj);

        /// Deselects all objects and selects a new scene object.
        /** \param pObj The object to select
        */
        void           DeselectSceneObject(s_ptr<SceneObject> pObj);

        /// Adds a scene object to the current selection.
        /** \param pObj The object to select
        */
        void           AddSceneObjectToSelection(s_ptr<SceneObject> pObj);

        /// Deselects all objects.
        void           ClearSelection();

        /// Makes the translated object follow the ground.
        /** \param bConstraint 'true' to force on ground
        */
        void           ConstraintTranslationOnGround(const s_bool& bConstraint);

        /// Stops all transformations that could be enabled.
        /** \note This stops doodad/light transformations (translation, scale,
        *         rotation) and terrain editing.
        */
        void           StopAllTransformations();

        /// Registers an object to this manager and gives it an unused ID.
        /** \param pObj The object requesting the ID
        *   \return A new unused ID for a new resource
        *   \note Automatically called by MovableObject.
        */
        const s_uint&  RegisterObject(s_ptr<MovableObject> pObj);

        /// Registers a scene object to this manager.
        /** \param pObj The object to register
        *   \note Automatically called by SceneObject.
        */
        void           RegisterSceneObject(s_ptr<SceneObject> pObj);

        /// Unregisters an object from this manager.
        /** \param pObj The object to unregister
        *   \note Automatically called by MovableObject.
        */
        void           UnregisterObject(s_ptr<MovableObject> pObj);

        /// Unregisters a scene object from this manager.
        /** \param pObj The object to unregister
        *   \note Automatically called by SceneObject.
        */
        void           UnregisterSceneObject(s_ptr<SceneObject> pObj);

        /// Deletes an object.
        /** \param pObj The object to delete
        *   \note Only works for objects created by this manager.
        */
        void           DeleteObject(s_ptr<MovableObject> pObj);

        /// Returns all objects of the scene.
        const s_map< s_uint, s_ptr<SceneObject> >& GetSceneObjectList() const;

        /// Returns all selected objects of the scene.
        const s_map< s_uint, s_ptr<SceneObject> >& GetSelectedSceneObjectList() const;

        /// Updates this manager.
        /** \param fDelta The time elapsed since the last call
        *   \note Automatically called by Engine.
        */
        void           Update(const s_float& fDelta);

        static const s_str CLASS_NAME;

    protected :

        /// Default constructor.
        /** \note This is a singleton class. That means that there can
        *         only be a single instance at the same time. That's why
        *         the constructor is not in public scope.<br>
        *         If you need to get a pointer to that unique instance,
        *         call SceneManager::GetSingleton().<br>
        *         Note that, if GetSingleton() has been called at least once,
        *         you'll have to delete the returned pointer when you close
        *         your program by calling SceneManager::Delete() (this is
        *         automatically done by Engine).
        */
        SceneManager();

        /// Destructor.
        /** \note If GetSingleton() has been called at least once,
        *         you'll have to delete the returned pointer when
        *         you close your program by calling Delete() (this is
        *         automatically done by Engine).
        */
        ~SceneManager();

        /// Copy constructor.
        SceneManager(const SceneManager& mMgr);

        /// Assignment operator.
        SceneManager& operator = (const SceneManager& mMgr);

    private :

        enum Transformation
        {
            TRANSFORM_NONE,
            TRANSFORM_TRANSLATION,
            TRANSFORM_SCALING,
            TRANSFORM_ROTATION
        };

        s_uint uiObjectCounter_;

        s_map< s_uint, s_ptr<MovableObject> > lRegisteredObjectList_;
        s_map< s_uint, s_ptr<MovableObject> > lCreatedObjectList_;

        s_map< s_uint, s_ptr<SceneObject> > lSceneObjectList_;
        s_map< s_uint, s_ptr<SceneObject> > lSelectedObjectList_;

        s_ptr<OgreInterface> pMouseOveredObject_;
        s_ptr<OgreInterface> pDraggedObject_;

        Transformation       mCurrentTransformation_;
        Vector::Constraint   mTransformationConstraint_;
        s_ptr<MovableObject> pTransformedObject_;
        s_bool               bConstrainOnGround_;
    };
}

#endif
