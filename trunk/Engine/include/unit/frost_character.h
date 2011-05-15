/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*            Character header            */
/*                                        */
/*                                        */


#ifndef FROST_CHARACTER_H
#define FROST_CHARACTER_H

#include "frost_prereqs.h"
#include "unit/frost_movableunit.h"
#include <scene/frost_sceneobject.h>

namespace Frost
{
    enum ModelPartCategory
    {
        MODELPART_BODY = -1,
        MODELPART_HAIRS = 0,
        MODELPART_BEARD01,
        MODELPART_BEARD02,
        MODELPART_FACE_ARTS,
        MODELPART_GLOVES,
        MODELPART_LEGS_LOW,
        MODELPART_UNK01,   // ?
        MODELPART_EARS,
        MODELPART_EAR_RINGS,
        MODELPART_BOOT_UP, // ?
        MODELPART_BELT01,  // ?
        MODELPART_BELT02,  // ?
        MODELPART_TABARD,
        MODELPART_LEGS_UP,
        MODELPART_UNK02,   // ?
        MODELPART_CAPES
    };

    /// Holds informations about the model used by a character.
    struct CharacterModelInfo
    {
        s_str          sModel;
        s_ctnr<s_str>  lBodyTextureList;
        s_ctnr<s_str>  lHairTextureList;
        s_ctnr<s_uint> lDefaultPartList;

        static const s_str CLASS_NAME;
    };

    /// Holds generic informations about a specific character race.
    struct Race
    {
        s_str              sName;
        CharacterModelInfo mMaleModelInfo;
        CharacterModelInfo mFemaleModelInfo;

        static const s_str CLASS_NAME;
    };

    /// Makes the link between Character and Ogre::MovableObject.
    class CharacterOgreInterface : public OgreInterface
    {
    public :

        /// Sets the Character linked to this interface.
        /** \param pCharacter The Character
        */
        void   SetCharacter(s_ptr<Character> pCharacter);

        /// Callback to react to events.
        /** \param sEvent The name of the event
        */
        void   On(const s_str& sEvent);

    private :

        s_ptr<Character> pCharacter_;

    };

    /// A player controlled Unit
    /** Can wear items and be physically customized (skin
    *   color, hair style, ...).<br>
    *   It has a unique name, a gender, a class, and belongs
    *   to a race.
    */
    class Character : public MovableUnit
    {
    public :

        enum Gender
        {
            GENDER_NONE,
            GENDER_MALE,
            GENDER_FEMALE
        };

        /// Constructor.
        /** \param uiID    This Character's unique ID
        *   \param sName   This Character's name
        *   \param mRace   This Character's race
        *   \param mGender This Character's gender
        *   \note You shouldn't have to call this. Use the
        *         UnitManager instead.
        */
        Character(const s_uint& uiID, const s_str& sName, const Race& mRace, Gender mGender);

        /// Destructor.
        ~Character();

        /// Tells this Unit that is is being controlled by the player.
        virtual void     SetPlayerControlled(const s_bool& bPlayerControlled);

        /// Returns this Character's Race.
        /** \return This Character's Race
        */
        const Race&      GetRace();

        /// Returns a sub-model of the body model.
        /** \param uiCategory  The kind of sub-model you want
        *   \param uiVariation The variation you want
        *   \note The body itself is : (0, 0).<br>
        *         Everything else is : (cat, var), with cat starting from
        *         0 and var starting from 1.
        */
        s_ptr<ModelPart> GetModelPart(const s_uint& uiCategory, const s_uint& uiVariation);

        /// Updates this Character's parameters.
        /** \param fDelta The time elapsed since the last call
        *   \note Automatically called by UnitManager.
        */
        virtual void     Update(const s_float& fDelta);

        /// Pushes this Character on the provided Lua::State.
        /** \param pLua The Lua::State on which to push the glue
        */
        virtual void     PushOnLua(s_ptr<Lua::State> pLua) const;

        /// Creates the associated Lua glue.
        /** \param pLua The Lua::State on which to push the glue
        */
        virtual void     CreateGlue(s_ptr<Lua::State> pLua);

        /// Returns this object's type.
        /** \return This object's type
        *   \note Simply returns CLASS_NAME.
        */
        virtual const s_str& GetType() const;

        static const s_str CLASS_NAME;

    private :

        CharacterOgreInterface mOgreInterface_;

        Gender mGender_;
        Race   mRace_;
    };

    /** \cond NOT_REMOVE_FROM_DOC
    */

    class LuaCharacter : public LuaMovableUnit
    {
    public :

        LuaCharacter(lua_State* pLua);

        int _GetRace(lua_State*);

        static const char className[];
        static const char* classList[];
        static Lunar<LuaCharacter>::RegType methods[];
        static const s_str CLASS_NAME;

    private :

        s_ptr<Character> pParentCharacter_;
    };

    /** \endcond
    */
}

#endif
