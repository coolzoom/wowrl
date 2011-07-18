/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*           SceneObject header           */
/*                                        */
/*                                        */


#ifndef FROST_SCENEOBJECT_H
#define FROST_SCENEOBJECT_H

#include "frost.h"
#include "frost_editor.h"

#include <OgreUserDefinedObject.h>

namespace Frost
{
    /// Base class for all objects in the scene
    class SceneObject
    {
    public :

        /// Default constructor.
        /** \param sName         The name of this object
        *   \param pSceneManager The scene manager in which to create the object
        *   \note If no scene manager is provided, the default one is used.
        */
        SceneObject(const s_uint& uiID, const s_str& sName);

        /// Destructor.
        virtual ~SceneObject();

        /// Returns the name of this object.
        /** \return The name of this object
        */
        const s_str&  GetName() const;

        /// Makes this object invisible.
        virtual void  Hide();

        /// Makes this object visible.
        virtual void  Show();

        /// Checks if this object is visible.
        /** \return 'true' if this object is visible
        */
        const s_bool& IsShown() const;

        /// Selects this object.
        /** \param bSelected 'true' to select the object
        */
        virtual void  Select(const s_bool& bSelected);

        /// Checks if this object is selected or not.
        /** \return 'true' if this object is selected
        */
        const s_bool& IsSelected() const;

        /// Checks if this object can be selected or not.
        /** \return 'true' if this object can be selected
        */
        const s_bool& IsSelectable() const;

        /// Creates an EditorAction capable of both creating and deleting this object.
        /** \return An EditorAction capable of both creating and deleting this object
        */
        virtual s_refptr<EditorAction> CreateDeleteAction() = 0;

        /// Returns this object's unique ID.
        /** \return This object's unique ID
        */
        const s_uint&  GetID() const;

        /// Returns this object's type.
        /** \return This object's type
        *   \note Simply returns CLASS_NAME.
        */
        virtual const s_str&  GetType() const = 0;

        static const s_str CLASS_NAME;

    protected :

        s_str  sName_;
        s_uint uiSceneID_;

        s_bool bIsShown_;
        s_bool bSelected_;
        s_bool bIsSelectable_;

    };

    /// Abstract base class for mouse picking.
    /** The use of this class is to react to mouse picking in the
    *   3D world. It must be derivated to implement the On() function,
    *   which defines the click behavior of the associated object.
    */
    class OgreInterface : public Ogre::UserDefinedObject
    {
    public :

        /// Constructor.
        OgreInterface();

        /// Destructor.
        virtual ~OgreInterface();

        /// Sets the Ogre::Entity this interface is binded to.
        /** \param pEntity The Ogre::Entity to bind
        */
        void BindEntity(s_ptr<Ogre::Entity> pEntity);

        /// Sets the associated SceneObject.
        /** \param pMovableObject The associated SceneObject
        *   \note This class is still fully functionnal even if
        *         you don't bind a SceneObject to it.
        */
        void                 SetSceneObject(s_ptr<SceneObject> pSceneObject);

        /// Returns the associated MovableObject.
        /** \return The associated MovableObject
        */
        s_ptr<SceneObject>   GetSceneObject() const;

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
        virtual s_bool       IsMouseEnabled() const;

        /// Callback to react to events.
        /** \param sEvent The name of the event
        */
        virtual void         On(const s_str& sEvent) = 0;

    private :

        s_ptr<SceneObject> pSceneObject_;
        s_int              iPriority_;
        s_bool             bMouseEnabled_;
    };

    class VisibilityAction : public EditorAction
    {
    public :

        VisibilityAction(s_ptr<SceneObject> pObject, const s_bool& bVisible);

        virtual void Do();
        virtual void Undo();

    private :

        s_uint uiObjectID_;
        s_bool bVisible_;
    };
}

#endif
