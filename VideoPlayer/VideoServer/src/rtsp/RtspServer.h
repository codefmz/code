#ifndef _RTSPSERVER_H_
#define _RTSPSERVER_H_
#include <map>
#include <vector>
#include <string>

#include "TcpServer.h"
#include "RtspConnection.h"
#include "MediaSession.h"
#include "Scheduler.h"
#include "Event.h"
#include "Mutex.h"

class RtspConnection;

class RtspServer : public TcpServer
{
public:
    static RtspServer* createNew(EventScheduler* scheduler, Ipv4Address& addr);

    RtspServer(EventScheduler* scheduler, const Ipv4Address& addr);
    virtual ~RtspServer();

    bool addMeidaSession(MediaSession* mediaSession);
    MediaSession* loopupMediaSession(std::string name);
    std::string getUrl(MediaSession* session);
    EventScheduler* getEventScheduler() {
        return mScheduler;
    }

protected:
    virtual void handleNewConnection(int connfd);
    static void disconnectionCallback(void* arg, int sockfd);
    void handleDisconnection(int sockfd);
    static void triggerCallback(void*);
    void handleDisconnectionList();

private:
    std::map<std::string, MediaSession*> mMediaSessions;
    std::map<int, RtspConnection*> mConnections;
    std::vector<int> mDisconnectionlist;
    TriggerEvent* mTriggerEvent;
    Mutex* mMutex;
    EventScheduler* mScheduler;
};

#endif //_RTSPSERVER_H_