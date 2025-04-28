#ifndef _H264FILE_MEDIA_SOURCE_H_
#define _H264FILE_MEDIA_SOURCE_H_
#include <string>

#include "MediaSource.h"
#include "ThreadPool.h"

class H264FileMediaSource : public MediaSource
{
public:
    static H264FileMediaSource* createNew(ThreadPool* pool, std::string file);

    H264FileMediaSource(ThreadPool* pool, const std::string& file);
    ~H264FileMediaSource();

protected:
    virtual void readFrame();

private:
    int getFrameFromH264File(int fd, uint8_t* frame, int size);

private:
    std::string mFile;
    int mFd;
};

#endif //_H264FILE_MEDIA_SOURCE_H_