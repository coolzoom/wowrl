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

    AnimManager::AnimManager( s_ptr<Model> pParent, map<s_uint, MeshAnimation> lMAList )
    {
        pParent_ = pParent;
        bPaused_ = true;
        fSpeed_ = 1.0f;
        mActualPriority_ = ANIM_PRIORITY_BACKGROUND;

        map<s_uint, MeshAnimation>::iterator iterAnim;
        foreach (iterAnim, lMAList)
        {
            Animation mAnim;
            mAnim.Set(&iterAnim->second, pParent_);
            lAnimList_[mAnim.uiID].lSequence.push_back(mAnim);
        }
    }

    AnimManager::AnimManager( const AnimManager& mMgr, s_ptr<Model> pParent )
    {
        pParent_ = pParent;
        bPaused_ = true;
        fSpeed_ = 1.0f;
        mActualPriority_ = ANIM_PRIORITY_LOW;

        map<s_uint, AnimationSequence>::const_iterator iterSequence;
        foreach (iterSequence, mMgr.lAnimList_)
        {
            vector<Animation>::const_iterator iterAnim;
            foreach (iterAnim, iterSequence->second.lSequence)
            {
                Animation mAnim;
                mAnim.Set(s_ptr<const Animation>(&(*iterAnim)), pParent_);
                lAnimList_[mAnim.uiID].lSequence.push_back(mAnim);
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
            map<s_uint, AnimationSequence>::iterator iterAnim = lAnimList_.find((uint)mID);
            if (iterAnim != lAnimList_.end())
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

    void AnimManager::ChooseAnim( s_uint uiID )
    {
        if (pOldAnim_)
            pOldAnim_->pAnim->setEnabled(false);

        pOldAnim_ = pActualAnim_;

        map<s_uint, AnimationSequence>::iterator iterAnim = lAnimList_.find(uiID);
        if (iterAnim != lAnimList_.end())
        {
            s_ptr<AnimationSequence> pAS = &(iterAnim->second);
            if (pAS->lSequence.size() > 1)
            {
                s_uint uiRnd = s_uint::Random(0u, 10u);
                if (uiRnd > 8)
                    uiRnd.Randomize(1, pAS->lSequence.size()-1);
                else
                    uiRnd = 0u;
                pActualAnim_ = &pAS->lSequence[uiRnd.Get()];
            }
            else
                pActualAnim_ = &pAS->lSequence[0];

            pActualAnim_->pAnim->setEnabled(true);
            pActualAnim_->pAnim->setTimePosition(0.0f);

            if (pOldAnim_ != pActualAnim_)
            {
                if (pOldAnim_ != NULL)
                {
                    pOldAnim_->pAnim->setEnabled(true);
                    pOldAnim_->pAnim->setWeight(1.0f);
                }
                pActualAnim_->pAnim->setWeight(0.0f);
                bTransition_ = true;
                fBlend_ = 0.0f;
            }
            else
            {
                pActualAnim_->pAnim->setWeight(1.0f);
            }
        }
        else
            pActualAnim_ = NULL;

        // TODO : Model : Calculer la bounding box animée
    }

    void AnimManager::SetAnim( AnimID mID, AnimPriority mPriority, s_bool bQueued )
    {
        if (mPriority == ANIM_PRIORITY_BACKGROUND)
        {
            if (mID == ANIM_NONE)
            {
                uiBackgroundAnimID_.SetNaN();
                if (mActualPriority_ == ANIM_PRIORITY_BACKGROUND)
                    pActualAnim_ = NULL;
            }
            else
            {
                uiBackgroundAnimID_ = (uint)mID;
                if (mActualPriority_ == ANIM_PRIORITY_BACKGROUND)
                {
                    if (pActualAnim_)
                    {
                        if (pActualAnim_->uiID != uiBackgroundAnimID_)
                            ChooseAnim(uiBackgroundAnimID_);
                    }
                    else
                        ChooseAnim(uiBackgroundAnimID_);
                }
            }
        }
        else if ( (bQueued) && (mActualPriority_ >= mPriority) )
        {
            if (mID == ANIM_NONE)
                lQueueList_[mPriority].push_back(NULL);
            else
            {
                lQueueList_[mPriority].push_back(GetAnim(mID));
            }
        }
        else
        {
            lQueueList_.clear();

            if (mID == ANIM_NONE)
            {
                if (pActualAnim_ != NULL)
                    pActualAnim_->pAnim->setEnabled(false);
                pActualAnim_ = NULL;
                pOldAnim_ = NULL;
            }
            else
            {
                ChooseAnim((int)mID);

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
            pActualAnim_->pAnim->setTimePosition(0.0f);
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

    void AnimManager::Update( s_float fDelta )
    {
        if (!bPaused_)
        {
            if (!bTransition_)
            {
                if (pActualAnim_ != NULL)
                {
                    if (!bReversed_)
                    {
                        pActualAnim_->pAnim->addTime((fDelta*fSpeed_).Get());

                        if (pActualAnim_->pAnim->hasEnded())
                        {
                            if (pActualAnim_->bLoop)
                            {
                                ChooseAnim(pActualAnim_->uiID);
                            }
                            else
                            {
                                if (!lQueueList_.empty())
                                {
                                    map< AnimPriority, deque< s_ptr<Animation> > >::iterator iterQueue = lQueueList_.end();
                                    iterQueue--;

                                    if (iterQueue->second.front())
                                    {
                                        ChooseAnim(iterQueue->second.front()->uiID);
                                    }
                                    else
                                    {
                                        pActualAnim_ = NULL;
                                    }

                                    mActualPriority_ = iterQueue->first;

                                    iterQueue->second.pop_front();
                                    if (iterQueue->second.empty())
                                        lQueueList_.erase(iterQueue);
                                }
                                else if (uiBackgroundAnimID_.IsValid() && mActualPriority_ != ANIM_PRIORITY_BACKGROUND)
                                {
                                    ChooseAnim(uiBackgroundAnimID_);
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
                        pActualAnim_->pAnim->addTime((-fDelta*fSpeed_).Get());
                        if (pActualAnim_->pAnim->getTimePosition() == 0.0f)
                        {
                            if (pActualAnim_->bLoop)
                            {
                                ChooseAnim(pActualAnim_->uiID);
                                pActualAnim_->pAnim->setTimePosition(pActualAnim_->pAnim->getLength());
                            }
                            else
                            {
                                if (!lQueueList_.empty())
                                {
                                    map< AnimPriority, deque< s_ptr<Animation> > >::iterator iterQueue = lQueueList_.end();
                                    iterQueue--;

                                    if (iterQueue->second.front())
                                    {
                                        ChooseAnim(iterQueue->second.front()->uiID);
                                        pActualAnim_->pAnim->setTimePosition(pActualAnim_->pAnim->getLength());
                                    }
                                    else
                                    {
                                        pActualAnim_ = NULL;
                                    }

                                    mActualPriority_ = iterQueue->first;

                                    iterQueue->second.pop_front();
                                    if (iterQueue->second.empty())
                                        lQueueList_.erase(iterQueue);
                                }
                                else if (uiBackgroundAnimID_.IsValid())
                                {
                                    ChooseAnim(uiBackgroundAnimID_);
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
                        pActualAnim_->pAnim->setWeight(fCoef);
                        if (pOldAnim_ != NULL)
                            pOldAnim_->pAnim->setWeight(1-fCoef);

                        if (!bReversed_)
                        {
                            pActualAnim_->pAnim->addTime((fDelta*fSpeed_).Get());
                            /*if (pOldAnim_ != NULL)
                                pOldAnim_->pAnim->addTime((fDelta*fSpeed_).Get());*/
                        }
                        else
                        {
                            pActualAnim_->pAnim->addTime((-fDelta*fSpeed_).Get());
                            /*if (pOldAnim_ != NULL)
                                pOldAnim_->pAnim->addTime((-fDelta*fSpeed_).Get());*/
                        }

                        fBlend_ += fDelta;
                    }
                    else
                    {
                        bTransition_ = false;
                        pActualAnim_->pAnim->setWeight(1.0f);
                        if (pOldAnim_ != NULL)
                        {
                            pOldAnim_->pAnim->setEnabled(false);
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
        pAnim = pModel->GetEntity()->getAnimationState(sName.Get());
        pAnim->setLoop(false);
    }

    void Animation::Set( s_ptr<const Animation> pAnimation, s_ptr<Model> pModel )
    {
        sName = pAnimation->sName;
        uiID = pAnimation->uiID;
        bLoop = pAnimation->bLoop;
        pAnim = pModel->GetEntity()->getAnimationState(sName.Get());
        pAnim->setLoop(false);
    }
}
