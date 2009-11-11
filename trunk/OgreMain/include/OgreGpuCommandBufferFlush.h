#ifndef __GPUCOMMANDBUFFERFLUSH_H__
#define __GPUCOMMANDBUFFERFLUSH_H__

#include "OgrePrerequisites.h"
#include "OgreFrameListener.h"
#include "OgreRoot.h"
#include "OgreRenderSystem.h"
#include "OgreHardwareOcclusionQuery.h"

namespace Ogre
{

	/** Helper class which can assist you in making sure the GPU command
		buffer is regularly flushed, so in cases where the CPU is outpacing the
		GPU we do not hit a situation where the CPU suddenly has to stall to
		wait for more space in the buffer.
	*/
	class GpuCommandBufferFlush : public FrameListener
	{
	protected:
		bool mUseOcclusionQuery;
		typedef std::vector<HardwareOcclusionQuery*> HOQList;
		HOQList mHOQList;
		size_t mMaxQueuedFrames;
		size_t mCurrentFrame;
		bool mStartPull;
		bool mStarted;

	public:
		GpuCommandBufferFlush()
            : mUseOcclusionQuery(true)
            , mMaxQueuedFrames(2)
            , mCurrentFrame(0)
            , mStartPull(false)
            , mStarted(false) { }

		~GpuCommandBufferFlush() { stop(); }

		void start(size_t maxQueuedFrames = 2)
        {
            if (!Root::getSingletonPtr() || !Root::getSingletonPtr()->getRenderSystem())
                return;
            stop();

            mMaxQueuedFrames = maxQueuedFrames;
            RenderSystem* rsys = Root::getSingleton().getRenderSystem();
            mUseOcclusionQuery = rsys->getCapabilities()->hasCapability(RSC_HWOCCLUSION);

            if (mUseOcclusionQuery)
            {
                for (size_t i = 0; i < mMaxQueuedFrames; ++i)
                {
                    HardwareOcclusionQuery* hoq = rsys->createHardwareOcclusionQuery();
                    mHOQList.push_back(hoq);
                }
            }

            mCurrentFrame = 0;
            mStartPull = false;

            Root::getSingleton().addFrameListener(this);

            mStarted = true;
        }

		void stop()
        {
            if (!mStarted || !Root::getSingletonPtr() || !Root::getSingletonPtr()->getRenderSystem())
                return;

            RenderSystem* rsys = Root::getSingleton().getRenderSystem();
            for (HOQList::iterator i = mHOQList.begin(); i != mHOQList.end(); ++i)
                rsys->destroyHardwareOcclusionQuery(*i);
            mHOQList.clear();

            Root::getSingleton().removeFrameListener(this);

            mStarted = false;
        }

		bool frameStarted(const FrameEvent& evt)
        {
            if (mUseOcclusionQuery) mHOQList[mCurrentFrame]->beginOcclusionQuery();
            return true;
        }

		bool frameEnded(const FrameEvent& evt)
        {
            if (mUseOcclusionQuery)
                mHOQList[mCurrentFrame]->endOcclusionQuery();

            mCurrentFrame = (mCurrentFrame + 1) % mMaxQueuedFrames;

            // If we've wrapped around, time to start pulling
            if (mCurrentFrame == 0)
                mStartPull = true;

            if (mStartPull)
            {
                unsigned int dummy;
                mHOQList[mCurrentFrame]->pullOcclusionQuery(&dummy);
            }

            return true;
        }

	};


}


#endif
