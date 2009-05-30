/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*            Character header            */
/*                                        */
/*                                        */


#ifndef FROST_CHARACTER_H
#define FROST_CHARACTER_H

#include "frost.h"
#include "unit/frost_unit.h"

#include <OgreUserDefinedObject.h>

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

    enum CharGender
    {
        GENDER_NONE,
        GENDER_MALE,
        GENDER_FEMALE
    };

    /// Holds informations about the model used by a character.
    struct CharacterModelInfo
    {
        s_str               sModel;
        std::vector<s_str>  lBodyTextureList;
        std::vector<s_str>  lHairTextureList;
        std::vector<s_uint> lDefaultPartList;

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
    class CharacterOgreInterface : public Ogre::UserDefinedObject
    {
    public :

        /// Constructor.
        CharacterOgreInterface(s_ptr<Character> pModel);

        /// Inherited from UserDefinedObject.
        virtual const Ogre::String& getTypeName() const;

        /// Returns the associated Character.
        /** \return The associated Character
        */
        s_ptr<Character> GetCharacter() const;

    private :

        s_ptr<Character> pCharacter_;

    };

    /// A player controlled Unit
    /** Can wear items and be physically customized (skin
    *   color, hair style, ...).<br>
    *   It has a unique name, a gender, a class, and belongs
    *   to a race.
    */
    class Character : public Unit
    {
    public :

        /// Constructor.
        /** \param uiID    This Character's unique ID
        *   \param sName   This Character's name
        *   \param mRace   This Character's race
        *   \param mGender This Character's gender
        *   \note You shouldn't have to call this. Use the
        *         UnitManager instead.
        */
        Character(const s_uint& uiID, const s_str& sName, const Race& mRace, CharGender mGender);

        /// Destructor.
        ~Character();

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
        /** \param pLua The State on which to push the Character
        */
        virtual void     PushOnLua(s_ptr<Lua::State> pLua) const;

        /// Creates the associated Lua glue.
        virtual void     CreateGlue();

        static const s_str CLASS_NAME;

    private :

        CharacterOgreInterface mInterface_;

        CharGender mGender_;
        Race       mRace_;
    };

    /** \cond NOT_REMOVE_FROM_DOC
    */

    class LuaCharacter : public LuaUnit
    {
    public :

        LuaCharacter(lua_State* pLua);

        int _GetRace(lua_State*);

        static const char className[];
        static Lunar<LuaCharacter>::RegType methods[];
        static const s_str CLASS_NAME;

    private :

        s_ptr<Character> pParentCharacter_;
    };

    /** \endcond
    */
}

#endif
