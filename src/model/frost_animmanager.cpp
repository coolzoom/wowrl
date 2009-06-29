/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*           AnimManager source           */
/*                                        */
/*  ## : Contains the member functions    */
/*  of the AnimManager class.             */
/*       The AnimManager takes care of a  */
/*  Model animations. That is : update,   */
/*  play, pause, select, ...              */
/*                                        */

#include "model/frost_animmanager.h"

#include "model/frost_model.h"

#include <OgreAnimationState.h>
#include <OgreEntity.h>

#define ANIMATION_BLEND_DURATION 0.2f

using namespace std;

namespace Frost
{
    const s_str AnimManager::CLASS_NAME = "AnimManager";
    const s_str Animation::CLASS_NAME = "Animation";
    const s_str MeshAnimation::CLASS_NAME = "MeshAnimation";
    const s_str AnimationSequence::CLASS_NAME = "AnimationSequence";

    AnimManager::AnimManager( s_ptr<Model> pParent, const map<s_uint, MeshAnimation>& lMAList )
    {
        pParent_ = pParent;
        bPaused_ = true;
        fSpeed_ = 1.0f;
        mActualPriority_ = ANIM_PRIORITY_BACKGROUND;

        s_map<s_uint, MeshAnimation>::const_iterator iterAnim;
        foreach (iterAnim, lMAList)
        {
            Animation mAnim;
            mAnim.Set(&iterAnim->second, pParent_);
            lAnimList_[mAnim.uiID].lSequence.PushBack(mAnim);
        }
    }

    AnimManager::AnimManager( const AnimManager& mMgr, s_ptr<Model> pParent )
    {
        pParent_ = pParent;
        bPaused_ = true;
        fSpeed_ = 1.0f;
        mActualPriority_ = ANIM_PRIORITY_LOW;

        s_map<s_uint, AnimationSequence>::const_iterator iterSequence;
        foreach (iterSequence, mMgr.lAnimList_)
        {
            s_ctnr<Animation>::const_iterator iterAnim;
            foreach (iterAnim, iterSequence->second.lSequence)
            {
                Animation mAnim;
                mAnim.Set(s_ptr<const Animation>(&(*iterAnim)), pParent_);
                lAnimList_[mAnim.uiID].lSequence.PushBack(mAnim);
            }
        }
    }

    AnimManager::~AnimManager()
    {
    }

    s_ptr<Animation> AnimManager::GetAnim( AnimID mID )
    {
        if (mID == ANIM_NONE)
            return pActualAnim_;
        else
        {
            s_ptr<Animation> pAnim;
            map<s_uint, AnimationSequence>::iterator iterAnim;
            iterAnim = lAnimList_.FindIter(static_cast<uint>(mID));
            if (iterAnim != lAnimList_.End())
                pAnim = &iterAnim->second.lSequence[0];
            return pAnim;
        }
    }

    s_uint AnimManager::GetAnimID() const
    {
        if (pActualAnim_ != NULL)
            return pActualAnim_->uiID;
        else
            return 0;
    }

    void AnimManager::ChooseAnim_( const s_uint& uiID )
    {
        if (pOldAnim_)
            pOldAnim_->pOgreAnim->setEnabled(false);

        pOldAnim_ = pActualAnim_;

        s_map<s_uint, AnimationSequence>::iterator iterAnim = lAnimList_.FindIter(uiID);
        if (iterAnim != lAnimList_.End())
        {
            s_ptr<AnimationSequence> pAS = &(iterAnim->second);
            if (pAS->lSequence.GetSize() > 1)
            {
                s_uint uiRnd = s_uint::Random(0u, 10u);
                if (uiRnd > 8)
                    uiRnd.Randomize(1, pAS->lSequence.GetSize()-1);
                else
                    uiRnd = 0u;
                pActualAnim_ = &pAS->lSequence[uiRnd];
            }
            else if (!pAS->lSequence.IsEmpty())
            {
                pActualAnim_ = &pAS->lSequence[0];
            }
            else
            {
                Error(CLASS_NAME,
                    "No animation in the sequence for anim "+uiID+"."
                );
                return;
            }

            pActualAnim_->pOgreAnim->setEnabled(true);
            pActualAnim_->pOgreAnim->setTimePosition(0.0f);

            if (pOldAnim_ != pActualAnim_)
            {
                if (pOldAnim_ != NULL)
                {
                    pOldAnim_->pOgreAnim->setEnabled(true);
                    pOldAnim_->pOgreAnim->setWeight(1.0f);
                }
                pActualAnim_->pOgreAnim->setWeight(0.0f);
                bTransition_ = true;
                fBlend_ = 0.0f;
            }
            else
            {
                pActualAnim_->pOgreAnim->setWeight(1.0f);
            }
        }
        else
            pActualAnim_ = NULL;

        // TODO : Model : Calculer la bounding box animée
    }

    void AnimManager::SetAnim( AnimID mID, AnimPriority mPriority, const s_float& fSpeed, const s_bool& bQueued )
    {
        if (mPriority == ANIM_PRIORITY_BACKGROUND)
        {
            fBackGroundSpeed_ = fSpeed;
            if (mID == ANIM_NONE)
            {
                uiBackgroundAnimID_.SetNaN();
                if (mActualPriority_ == ANIM_PRIORITY_BACKGROUND)
                    pActualAnim_ = NULL;
            }
            else
            {
                uiBackgroundAnimID_ = static_cast<uint>(mID);
                if (mActualPriority_ == ANIM_PRIORITY_BACKGROUND)
                {
                    fTempSpeed_ = fBackGroundSpeed_;
                    if (pActualAnim_)
                    {
                        if (pActualAnim_->uiID != uiBackgroundAnimID_)
                            ChooseAnim_(uiBackgroundAnimID_);
                    }
                    else
                        ChooseAnim_(uiBackgroundAnimID_);
                }
            }
        }
        else if ( (bQueued) && (mActualPriority_ >= mPriority) )
        {
            if (mID == ANIM_NONE)
                lQueueList_[mPriority].PushBack(AnimationParameters(NULL));
            else
            {
                lQueueList_[mPriority].PushBack(AnimationParameters(GetAnim(mID), fSpeed));
            }
        }
        else
        {
            lQueueList_.Clear();

            if (mID == ANIM_NONE)
            {
                if (pActualAnim_ != NULL)
                    pActualAnim_->pOgreAnim->setEnabled(false);
                pActualAnim_ = NULL;
                pOldAnim_ = NULL;
            }
            else
            {
                ChooseAnim_(static_cast<uint>(mID));
                fTempSpeed_ = fSpeed;
                mActualPriority_ = mPriority;
            }
        }
    }

    void AnimManager::Play()
    {
        if (bPaused_)
            bPaused_ = false;

        else if (pActualAnim_ != NULL)
        {
            pActualAnim_->pOgreAnim->setTimePosition(0.0f);
        }
    }

    void AnimManager::Reverse()
    {
        bReversed_ = !bReversed_;
    }

    void AnimManager::Pause()
    {
        if (!bPaused_)
            bPaused_ = true;
    }

    void AnimManager::SetSpeed( const s_float& fSpeed )
    {
        if (fSpeed < 0.0f)
        {
            bReversed_ = true;
            fSpeed_ = -fSpeed;
        }
        else
        {
            bReversed_ = false;
            fSpeed_ = fSpeed;
        }
    }

    const s_float& AnimManager::GetSpeed() const
    {
        return fSpeed_;
    }

    void AnimManager::Update( const s_float& fDelta )
    {
        if (!bPaused_)
        {
            if (!bTransition_)
            {
                if (pActualAnim_ != NULL)
                {
                    if (!bReversed_)
                    {
                        pActualAnim_->pOgreAnim->addTime((fDelta*fSpeed_*fTempSpeed_).Get());

                        if (pActualAnim_->pOgreAnim->hasEnded())
                        {
                            if (pActualAnim_->bLoop)
                            {
                                ChooseAnim_(pActualAnim_->uiID);
                            }
                            else
                            {
                                if (!lQueueList_.IsEmpty())
                                {
                                    s_map< AnimPriority, s_ctnr<AnimationParameters> >::iterator iterQueue = lQueueList_.End();
                                    iterQueue--;

                                    const AnimationParameters& mAnimParam = iterQueue->second.Front();

                                    if (mAnimParam.pAnim)
                                    {
                                        ChooseAnim_(mAnimParam.pAnim->uiID);
                                        fTempSpeed_ = mAnimParam.fSpeed;
                                    }
                                    else
                                    {
                                        pActualAnim_ = NULL;
                                    }

                                    mActualPriority_ = iterQueue->first;

                                    iterQueue->second.PopFront();
                                    if (iterQueue->second.IsEmpty())
                                        lQueueList_.Erase(iterQueue);
                                }
                                else if (uiBackgroundAnimID_.IsValid() && mActualPriority_ != ANIM_PRIORITY_BACKGROUND)
                                {
                                    ChooseAnim_(uiBackgroundAnimID_);
                                    fTempSpeed_ = fBackGroundSpeed_;
                                    mActualPriority_ = ANIM_PRIORITY_BACKGROUND;
                                }
                                else
                                {
                                    Pause();
                                }
                            }
                        }
                    }
                    else
                    {
                        pActualAnim_->pOgreAnim->addTime((-fDelta*fSpeed_*fTempSpeed_).Get());
                        if (pActualAnim_->pOgreAnim->getTimePosition() == 0.0f)
                        {
                            if (pActualAnim_->bLoop)
                            {
                                ChooseAnim_(pActualAnim_->uiID);
                                pActualAnim_->pOgreAnim->setTimePosition(pActualAnim_->pOgreAnim->getLength());
                            }
                            else
                            {
                                if (!lQueueList_.IsEmpty())
                                {
                                    s_map< AnimPriority, s_ctnr<AnimationParameters> >::iterator iterQueue = lQueueList_.End();
                                    iterQueue--;

                                    const AnimationParameters& mAnimParam = iterQueue->second.Front();

                                    if (mAnimParam.pAnim)
                                    {
                                        ChooseAnim_(mAnimParam.pAnim->uiID);
                                        fTempSpeed_ = mAnimParam.fSpeed;
                                        pActualAnim_->pOgreAnim->setTimePosition(pActualAnim_->pOgreAnim->getLength());
                                    }
                                    else
                                    {
                                        pActualAnim_ = NULL;
                                    }

                                    mActualPriority_ = iterQueue->first;

                                    iterQueue->second.PopFront();
                                    if (iterQueue->second.IsEmpty())
                                        lQueueList_.Erase(iterQueue);
                                }
                                else if (uiBackgroundAnimID_.IsValid())
                                {
                                    ChooseAnim_(uiBackgroundAnimID_);
                                    fTempSpeed_ = fBackGroundSpeed_;
                                    mActualPriority_ = ANIM_PRIORITY_BACKGROUND;
                                }
                                else
                                    Pause();
                            }
                        }
                    }
                }
            }
            else
            {
                if (pActualAnim_ != NULL)
                {
                    if (fBlend_ <= ANIMATION_BLEND_DURATION)
                    {
                        float fCoef = fBlend_.Get()/ANIMATION_BLEND_DURATION;
                        pActualAnim_->pOgreAnim->setWeight(fCoef);
                        if (pOldAnim_ != NULL)
                            pOldAnim_->pOgreAnim->setWeight(1-fCoef);

                        if (!bReversed_)
                        {
                            pActualAnim_->pOgreAnim->addTime((fDelta*fSpeed_*fTempSpeed_).Get());
                        }
                        else
                        {
                            pActualAnim_->pOgreAnim->addTime((-fDelta*fSpeed_*fTempSpeed_).Get());
                        }

                        fBlend_ += fDelta;
                    }
                    else
                    {
                        bTransition_ = false;
                        pActualAnim_->pOgreAnim->setWeight(1.0f);
                        if (pOldAnim_ != NULL)
                        {
                            pOldAnim_->pOgreAnim->setEnabled(false);
                            pOldAnim_ = NULL;
                        }
                    }
                }
            }
        }
    }

    const s_bool& AnimManager::IsPaused() const
    {
        return bPaused_;
    }

    void Animation::Set( s_ptr<const MeshAnimation> pMeshAnim, s_ptr<Model> pModel )
    {
        sName = pMeshAnim->sName;
        uiID = pMeshAnim->uiID;
        bLoop = pMeshAnim->bLoop;
        pOgreAnim = pModel->GetEntity()->getAnimationState(sName.Get());
        pOgreAnim->setLoop(false);
    }

    void Animation::Set( s_ptr<const Animation> pAnimation, s_ptr<Model> pModel )
    {
        sName = pAnimation->sName;
        uiID = pAnimation->uiID;
        bLoop = pAnimation->bLoop;
        pOgreAnim = pModel->GetEntity()->getAnimationState(sName.Get());
        pOgreAnim->setLoop(false);
    }
}
