#ifndef _MEDIASESSION_H_
#define _MEDIASESSION_H_
#include <string>
#include <list>

#include "RtpInstance.h"
#include "RtpSink.h"

#define MEDIA_MAX_TRACK_NUM 2
class Track
{
public:
    RtpSink* mRtpSink;
    int mTrackId;
    bool mIsAlive;
    std::list<RtpInstance*> mRtpInstances;
};

typedef enum {
    TrackIdNone = -1,
    TrackId0    = 0,
    TrackId1    = 1,
} TrackId;

class MediaSession
{
public:
    static MediaSession* createNew(std::string sessionName);

    MediaSession(const std::string& sessionName);
    ~MediaSession();

    std::string name() const {
        return mSessionName;
    }

    std::string generateSDPDescription();
    bool addRtpSink(TrackId trackId, RtpSink* rtpSink);
    bool addRtpInstance(TrackId trackId, RtpInstance* rtpInstance);
    bool removeRtpInstance(RtpInstance* rtpInstance);
    bool startMulticast();
    bool isStartMulticast();
    std::string getMulticastDestAddr() const {
        return mMulticastAddr;
    }

    uint16_t getMulticastDestRtpPort(TrackId trackId);

private:
    Track* getTrack(TrackId trackId);
    static void sendPacketCallback(void* arg1, void* arg2, RtpPacket* rtpPacket);
    void sendPacket(Track* tarck, RtpPacket* rtpPacket);

private:
    std::string mSessionName;
    std::string mSdp;
    Track mTracks[MEDIA_MAX_TRACK_NUM];
    std::string mMulticastAddr;
    RtpInstance* mMulticastRtpInstances[MEDIA_MAX_TRACK_NUM];
    RtcpInstance* mMulticastRtcpInstances[MEDIA_MAX_TRACK_NUM];
    bool mIsStartMulticast;
};

#endif //_MEDIASESSION_H_