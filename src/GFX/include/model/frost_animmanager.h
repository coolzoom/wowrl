/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*           AnimManager header           */
/*                                        */
/*                                        */


#ifndef FROST_ANIMMANAGER_H
#define FROST_ANIMMANAGER_H

#include "frost.h"

#include <OgreVector3.h>
#include <OgreQuaternion.h>

namespace Frost
{
    // Common animations
    enum AnimID
    {
        ANIM_NONE = -1,
        ANIM_STAND = 0,
        ANIM_DEATH,
        ANIM_SPELL,
        ANIM_STOP,
        ANIM_WALK,
        ANIM_RUN,
        ANIM_DEAD,
        ANIM_RISE,
        ANIM_STAND_WOUND,
        ANIM_COMBAT_WOUND,
        ANIM_COMBAT_CRITICAL,
        ANIM_SHUFFLE_LEFT,
        ANIM_SHUFFLE_RIGHT,
        ANIM_WALKBACKWARDS,
        ANIM_STUN,
        ANIM_HANDS_CLOSED,
        ANIM_ATTACK_UNARMED,
        ANIM_ATTACK_1H,
        ANIM_ATTACK_2H,
        ANIM_ATTACK_2HL,
        ANIM_PARRY_UNARMED,
        ANIM_PARRY_1H,
        ANIM_PARRY_2H,
        ANIM_PARRY_2HL,
        ANIM_SHIELD_BLOCK,
        ANIM_READY_UNARMED,
        ANIM_READY_1H,
        ANIM_READY_2H,
        ANIM_READY_2HL,
        ANIM_READY_BOW,
        ANIM_DODGE,
        ANIM_SPELL_PRECAST,
        ANIM_SPELL_CAST,
        ANIM_SPELL_CAST_AREA,
        ANIM_NPC_WELCOME,
        ANIM_NPC_GOODBYE,
        ANIM_BLOCK,
        ANIM_JUMP_START,
        ANIM_JUMP,
        ANIM_JUMP_END,
        ANIM_FALL,
        ANIM_SWIM_IDLE,
        ANIM_SWIM,
        ANIM_SWIM_LEFT,
        ANIM_SWIM_RIGHT,
        ANIM_SWIM_BACKWARDS,
        ANIM_ATTACK_BOW,
        ANIM_FIRE_BOW,
        ANIM_READY_RIFLE,
        ANIM_ATTACK_RIFLE,
        ANIM_LOOT,
        ANIM_READY_SPELL_DIRECTED,
        ANIM_READY_SPELL_OMNI,
        ANIM_SPELL_CAST_DIRECTED,
        ANIM_SPELL_CAST_OMNI,
        ANIM_BATTLE_ROAR,
        ANIM_READY_ABILITY,
        ANIM_SPECIAL_1H,
        ANIM_SPECIAL_2H,
        ANIM_SHIELD_BASH,
        ANIM_EMOTE_TALK,
        ANIM_EMOTE_EAT,
        ANIM_EMOTE_WORK,
        ANIM_EMOTE_USE_STANDING,
        ANIM_EMOTE_TALK_EXCLAMATION,
        ANIM_EMOTE_TALK_QUESTION,
        ANIM_EMOTE_BOW,
        ANIM_EMOTE_WAVE,
        ANIM_EMOTE_CHEER,
        ANIM_EMOTE_DANCE,
        ANIM_EMOTE_LAUGH,
        ANIM_EMOTE_SLEEP,
        ANIM_EMOTE_SIT_GROUND,
        ANIM_EMOTE_RUDE,
        ANIM_EMOTE_ROAR,
        ANIM_EMOTE_KNEEL,
        ANIM_EMOTE_KISS,
        ANIM_EMOTE_CRY,
        ANIM_EMOTE_CHICKEN,
        ANIM_EMOTE_BEG,
        ANIM_EMOTE_APPLAUD,
        ANIM_EMOTE_SHOUT,
        ANIM_EMOTE_FLEX,
        ANIM_EMOTE_SHY,
        ANIM_EMOTE_POINT,
        ANIM_ATTACK_1H_PIERCE,
        ANIM_ATTACK_2H_LOOSE_PIERCE,
        ANIM_ATTACK_OFF,
        ANIM_ATTACK_OFF_PIERCE,
        ANIM_SHEATH,
        ANIM_HIP_SHEATH,
        ANIM_MOUNT,
        ANIM_RUN_RIGHT,
        ANIM_RUN_LEFT,
        ANIM_MOUNT_SPECIAL,
        ANIM_KICK,
        ANIM_SIT_GROUND_DOWN,
        ANIM_SIT_GROUND,
        ANIM_SIT_GROUND_UP,
        ANIM_SLEEP_DOWN,
        ANIM_SLEEP,
        ANIM_SLEEP_UP,
        ANIM_SIT_CHAIR_LOW,
        ANIM_SIT_CHAIR_MED,
        ANIM_SIT_CHAIR_HIGH,
        ANIM_LOAD_BOW,
        ANIM_LOAD_RIFLE,
        ANIM_ATTACK_THROWN,
        ANIM_READY_THROWN,
        ANIM_HOLD_BOW,
        ANIM_HOLD_RIFLE,
        ANIM_HOLD_THROWN,
        ANIM_LOAD_THROWN,
        ANIM_EMOTE_SALUTE,
        ANIM_KNEEL_START,
        ANIM_KNEEL_LOOP,
        ANIM_KNEEL_END,
        ANIM_ATTACK_UNARMED_OFF,
        ANIM_SPECIAL_UNARMED,
        ANIM_STEALTH_WALK,
        ANIM_STEALTH_STAND,
        ANIM_KNOCKDOWN,
        ANIM_EATING_LOOP,
        ANIM_USE_STANDING_LOOP,
        ANIM_CHANNEL_CAST_DIRECTED,
        ANIM_CHANNEL_CAST_OMNI,
        ANIM_WHIRLWIND,
        ANIM_BIRTH,
        ANIM_USE_STANDING_START,
        ANIM_USE_STANDING_END,
        ANIM_CREATURE_SPECIAL,
        ANIM_DROWN,
        ANIM_DROWNED,
        ANIM_FISHING_CAST,
        ANIM_FISHING_LOOP,
        ANIM_FLY,
        ANIM_EMOTE_WORK_NO_SHEATHE,
        ANIM_EMOTE_STUN_NO_SHEATHE,
        ANIM_EMOTE_USE_STANDING_NO_SHEATHE,
        ANIM_SPELL_SLEEP_DOWN,
        ANIM_SPELL_KNEEL_START,
        ANIM_SPELL_KNEEL_LOOP,
        ANIM_SPELL_KNEEL_END,
        ANIM_SPRINT,
        ANIM_IN_FLIGHT,
        ANIM_SPAWN,
        ANIM_CLOSE,
        ANIM_CLOSED,
        ANIM_OPEN,
        ANIM_OPENED,
        ANIM_DESTROY,
        ANIM_DESTROYED,
        ANIM_REBUILD,
        ANIM_CUSTOM0,
        ANIM_CUSTOM1,
        ANIM_CUSTOM2,
        ANIM_CUSTOM3,
        ANIM_DESPAWN,
        ANIM_HOLD,
        ANIM_DECAY,
        ANIM_BOW_PULL,
        ANIM_BOW_RELEASE,
        ANIM_SHIP_START,
        ANIM_SHIP_MOVING,
        ANIM_SHIP_STOP,
        ANIM_GROUP_ARROW,
        ANIM_ARROW,
        ANIM_CORPSE_ARROW,
        ANIM_GUIDE_ARROW,
        ANIM_SWAY,
        ANIM_DRUID_CAT_POUNCE,
        ANIM_DRUID_CAT_RIP,
        ANIM_DRUID_CAT_RAKE,
        ANIM_DRUID_CAT_RAVAGE,
        ANIM_DRUID_CAT_CLAW,
        ANIM_DRUID_CAT_COWER,
        ANIM_DRUID_BEAR_SWIPE,
        ANIM_DRUID_BEAR_BITE,
        ANIM_DRUID_BEAR_MAUL,
        ANIM_DRUID_BEAR_BASH,
        ANIM_DRAGON_TAIL,
        ANIM_DRAGON_STOMP,
        ANIM_DRAGON_SPIT,
        ANIM_DRAGON_SPIT_HOVER,
        ANIM_DRAGON_SPIT_FLY,
        ANIM_EMOTE_YES,
        ANIM_EMOTE_NO,
        ANIM_JUMP_LAND_RUN,
        ANIM_LOOT_HOLD,
        ANIM_LOOT_UP,
        ANIM_STAND_HIGH,
        ANIM_IMPACT,
        ANIM_LIFT_OFF,
        ANIM_HOVER,
        ANIM_SUCCUBUS_ENTICE,
        ANIM_EMOTE_TRAIN,
        ANIM_EMOTE_DEAD,
        ANIM_EMOTE_DANCE_ONCE,
        ANIM_DEFLECT,
        ANIM_EMOTE_EAT_NO_SHEATHE,
        ANIM_LAND,
        ANIM_SUBMERGE,
        ANIM_SUBMERGED,
        ANIM_CANNIBALIZE,
        ANIM_ARROW_BIRTH,
        ANIM_GROUP_ARROW_BIRTH,
        ANIM_CORPSE_ARROW_BIRTH,
        ANIM_GUIDE_ARROW_BIRTH
    };

    enum AnimPriority
    {
        ANIM_PRIORITY_BACKGROUND = -1,
        ANIM_PRIORITY_LOW = 0,
        ANIM_PRIORITY_MED,
        ANIM_PRIORITY_HIGH
    };

    /// Temporary struct, only used while loading
    struct MeshAnimation
    {
        s_ptr<Ogre::Animation> pAnim;

        s_str  sName;
        s_uint uiID;
        s_bool bLoop;
        s_uint uiStart;
        s_uint uiEnd;

        static const s_str CLASS_NAME;
    };

    /// Stores an animation's settings and data
    struct Animation
    {
        s_ptr<Ogre::AnimationState> pOgreAnim;

        s_str  sName;
        s_uint uiID;
        s_bool bLoop;

        void Set(s_ptr<const MeshAnimation> pMeshAnim, s_ptr<Model> pModel);
        void Set(s_ptr<const Animation> pAnimation, s_ptr<Model> pModel);

        static const s_str CLASS_NAME;
    };

    /// Stores all animations with the same ID
    struct AnimationSequence
    {
        s_uint uiID;

        s_ctnr<Animation> lSequence;

        static const s_str CLASS_NAME;
    };

    /// Stores an animation and its temporary parameters
    struct AnimationParameters
    {
        AnimationParameters() : fSpeed(1.0f) {}
        AnimationParameters(s_ptr<Animation> anim, const s_float& speed = 1.0f) :
            pAnim(anim), fSpeed(speed) {}

        s_ptr<Animation> pAnim;
        s_float          fSpeed;
    };

    /// Animation manager
    /** This class manages a single model's animation.<br>
    *   It is instanciated for each Model in the game.<br>
    *   <b>Note</b> : This is not a singleton.
    */
    class AnimManager
    {
    public :

        /// Default constructor.
        /** \param pParent The Model this manager belongs to
        *   \param lMAList The animation list
        */
        AnimManager(s_ptr<Model> pParent, const s_map<s_uint, MeshAnimation>& lMAList);

        /// Copy constructor.
        /** \param mMgr The source
        *   \param pParent The new parent
        */
        AnimManager(const AnimManager& mMgr, s_ptr<Model> pParent);

        /// Destructor.
        /** \note Automatically called by Model.
        */
        ~AnimManager();

        /// Returns the animation with the provided ID.
        /** \return The animation with the provided ID
        *   \note If no parameter is provided, returns the current animation.
        */
        s_ptr<Animation> GetAnim(AnimID mID = ANIM_NONE);

        /// Returns the current animation's ID.
        /** \return The current animation's ID
        */
        s_uint           GetAnimID() const;

        /// The main function : puts a new animation on the queue.
        /** \param mID       The new animation's ID
        *   \param mPriority The prority of this animation
        *   \param fSpeed    The speed at which this animation should be played
        *   \param bQueued   Tells whether this animation should be put
        *                    in the queue or erase every previous call.
        *   \note If the current animation has the same priority as the new one, it
        *         keeps playing until it ends. Then, the new animation starts.<br>
        *         If the current animation has a lower priority, it immediately stops
        *         and the new animation starts.<br>
        *         If the current animation has a higher priority, the new one is just
        *         stored in the queue, and will play when no other queued animation
        *         has a higher priority.<br>
        *         If you use the ANIM_PRIORITY_BACKGROUND priority, then this
        *         animation will be used whenever there is nothing to play
        *         (for example : "stand", "run", "dead", ... should always uses
        *         this priority).<br><br>
        *         When you set an animation, all other animations in lower
        *         priority queues will be erased (except the background one).<br><br>
        *         The speed argument only applies to this particular animation. Once
        *         it has stopped playing, only the global speed factor (the one you
        *         set using SetSpeed()) applies.
        */
        void             SetAnim(
            AnimID mID,
            AnimPriority mPriority = ANIM_PRIORITY_HIGH,
            const s_float& fSpeed = 1.0f,
            const s_bool& bQueued = false
        );

        /// Plays the current animation.
        /** \note If it's already being played, the timer is reset and
        *         the animation starts over at its beginnging.
        */
        void             Play();

        /// Pauses the current animation.
        void             Pause();

        /// Checks if the current animation is paused.
        /** \return 'true' if the current animation is paused
        */
        const s_bool&    IsPaused() const;

        /// Plays the animation from the end to the beginning.
        void             Reverse();

        /// Sets the animation play speed.
        /** \param fSpeed The new speed (1.0f is normal speed)
        */
        void             SetSpeed(const s_float& fSpeed);

        /// Returns the animation play speed.
        /** \return The animation play speed
        */
        const s_float&   GetSpeed() const;

        /// Updates the current animation.
        /** \note Automatically called by ModelManager.
        */
        void             Update(const s_float& fDelta);

        static const s_str CLASS_NAME;

    private :

        void ChooseAnim_(const s_uint& mID);

        AnimPriority mActualPriority_;
        s_bool       bPaused_;
        s_bool       bTransition_;
        s_bool       bReversed_;
        s_float      fSpeed_;
        s_float      fTempSpeed_;
        s_float      fBackGroundSpeed_;
        s_float      fBlend_;
        s_ptr<Model> pParent_;

        s_ptr<Animation> pActualAnim_;
        s_ptr<Animation> pOldAnim_;
        s_uint           uiBackgroundAnimID_;

        s_map<s_uint, AnimationSequence> lAnimList_;
        s_map< AnimPriority, s_ctnr<AnimationParameters> > lQueueList_;
    };
}

#endif
