#include "MediaSource.h"
#include "Log.h"

MediaSource::MediaSource(ThreadPool* pool) : mPool(pool)
{
    mMutex = Mutex::createNew();
    for(int i = 0; i < DEFAULT_FRAME_NUM; ++i) {
        mAVFrameInputQueue.push(&mAVFrames[i]);
    }

    mTask.setTaskCallback(taskCallback, this);
}

MediaSource::~MediaSource()
{
    delete mMutex;
}

AVFrame* MediaSource::getFrame()
{
    MutexLockGuard mutexLockGuard(mMutex);
    if(mAVFrameOutputQueue.empty()) {
        return NULL;
    }

    AVFrame* frame = mAVFrameOutputQueue.front();
    mAVFrameOutputQueue.pop();

    return frame;
}

void MediaSource::putFrame(AVFrame* frame)
{
    MutexLockGuard mutexLockGuard(mMutex);

    mAVFrameInputQueue.push(frame);
    mPool->addTask(mTask);
}

void MediaSource::taskCallback(void* arg)
{
    MediaSource* source = (MediaSource*)arg;
    source->readFrame();
}