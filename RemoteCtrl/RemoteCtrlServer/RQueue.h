#pragma once
#include "pch.h"
#include <atomic>
#include <list>
#include "ThreadBase.h"

#define RQ_INVALID_SIZE (uint32_t)(-1)

//线程安全的队列（利用IOCP实现）
template<class T>
class RQueue
{
public:
    typedef enum {
        RQ_NONE,
        RQ_PUSH,
        RQ_POP,
        RQ_SIZE,
        RQ_CLEAR
    } RQ_OPER;

    //Post Parameter 用于投递信息的结构体 key
    typedef struct IocpNode {
        RQ_OPER oper;//操作
        T data;//数据
        HANDLE hEvent;//pop操作需要的
        size_t size;
        IocpNode(int op, T data, HANDLE hEve = NULL) {
            oper = (RQ_OPER)op;
            this->data = data;
            hEvent = hEve;
        }

        IocpNode(int op, HANDLE hEve = NULL) {
            oper = (RQ_OPER)op;
            hEvent = hEve;
        }

        IocpNode() {
            oper = RQ_NONE;
        }
    } Node;
protected:
    std::list<T> dataList;
    HANDLE cmptPort;
    HANDLE hThread;
public:
    RQueue() {
        cmptPort = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, NULL, 1);
        hThread = INVALID_HANDLE_VALUE;
        if (cmptPort != NULL) {
            hThread = (HANDLE)_beginthread(&RQueue<T>::threadEntry, 0, this);
        }
    }

    virtual ~RQueue() {
        PostQueuedCompletionStatus(cmptPort, 0, NULL, NULL);
        WaitForSingleObject(hThread, INFINITE);
        if (cmptPort != NULL) {
            /* 这里搞一个临时变量的作用还需要再考虑一下, 目前看应该是为了防止并发closeHandle(comptPort) */
            CloseHandle(cmptPort);
        }
    }

    bool PushBack(const T& data) {
        Node* pNode = new Node(RQ_PUSH, data);
        if (!PostQueuedCompletionStatus(cmptPort, sizeof(Node*), (ULONG_PTR)pNode, NULL)) {
            delete pNode;
        }

        return true;
    }

    bool PopFront(T& data) {
        HANDLE hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
        if (hEvent == NULL) {
            return false;
        }

        Node node(RQ_POP, data, hEvent);
        if (!PostQueuedCompletionStatus(cmptPort, sizeof(Node*), (ULONG_PTR)&node, NULL)) {
            CloseHandle(hEvent);
            return false;
        }

        if (WaitForSingleObject(hEvent, INFINITE) == WAIT_OBJECT_0) {
            CloseHandle(hEvent);
            data = node.data;
        }

        return true;
    }

    size_t Size() {
        HANDLE hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
        if (hEvent == NULL) {
            return RQ_INVALID_SIZE;
        }

        Node node(RQ_SIZE, hEvent);
        if (!PostQueuedCompletionStatus(cmptPort, sizeof(Node*), (ULONG_PTR)&node, NULL)) {
            CloseHandle(hEvent);
            return RQ_INVALID_SIZE;
        }
        if (WaitForSingleObject(hEvent, INFINITE) == WAIT_OBJECT_0) {
            CloseHandle(hEvent);
            return node.size;
        }
        return RQ_INVALID_SIZE;
    }

    bool Clear() {
        Node* pNode = new Node(RQ_CLEAR, T());
        if (!PostQueuedCompletionStatus(cmptPort, sizeof(Node), (ULONG_PTR)pNode, NULL)) {
            delete pNode;
            return false;
        }

        delete pNode;
        return true;
    }

protected:
    static void threadEntry(void* arg) {
        RQueue<T>* thiz = (RQueue<T>*)arg;
        thiz->threadMain();
        _endthread();
    }

    virtual void DealOper(Node* pNode) {
        switch (pNode->oper) {
            case RQ_PUSH:
                dataList.push_back(pNode->data);
                delete pNode;
                break;
            case RQ_POP:
                if (dataList.size() > 0) {
                    pNode->data = dataList.front();
                    dataList.pop_front();
                }
                SetEvent(pNode->hEvent);
                break;
            case RQ_SIZE:
                pNode->size = dataList.size();
                SetEvent(pNode->hEvent);
                break;
            case RQ_CLEAR:
                dataList.clear();
                delete pNode;
                break;
            default:
                LOG(Log::ERR, "error oper (%d) ", pNode->oper);
                break;
        }
    }

    virtual void threadMain() {
        DWORD length = 0;
        Node* pNode = NULL;
        ULONG_PTR key = 0;
        OVERLAPPED* pOverlapped = NULL;
        while (GetQueuedCompletionStatus(cmptPort, &length, &key, &pOverlapped, INFINITE)) {
            if ((length == 0) || (key == 0)) {
                LOG(Log::ERR, " get complete port error, length (%d).", length);
                break;
            }
            pNode = (Node*)key;
            DealOper(pNode);
        }
    }
};


//template<class T>
//class CSendQueue :public CQueue<T>, public ThreadFuncBase
//{
//public:
//    typedef int (ThreadFuncBase::* MCALLBACK)(T& data);
//
//    CSendQueue(ThreadFuncBase* obj, MCALLBACK callback)
//        :CQueue<T>(), m_base(obj), m_callback(callback)
//    {
//        m_thread.Start();
//        m_thread.UpdateWorker(::ThreadWorker(this, (FUNCTYPE)&CSendQueue<T>::threadTick));
//    }
//    virtual ~CSendQueue()
//    {
//        m_base = NULL;
//        m_callback = NULL;
//    }
//protected:
//    virtual bool PopFront(T& data)
//    {
//        return false;
//    };
//
//    bool PopFront()
//    {
//        typename CSendQueue<T>::IocpParam* Param = new typename CSendQueue<T>::IocpParam(CSendQueue<T>::EQPop, T());
//        if (CQueue<T>::m_lock) {
//            delete Param;
//            return false;
//        }
//        bool ret = PostQueuedCompletionStatus(CQueue<T>::m_hCompeletionPort, sizeof(typename CSendQueue<T>::PPARAM), (ULONG_PTR)&Param, NULL);
//        if (ret == false) {
//            delete Param;
//            return false;
//        }
//        return ret;
//    }
//    int threadTick()
//    {
//        if (CSendQueue<T>::m_lstData.size() > 0)
//        {
//            PopFront();
//        }
//        Sleep(1);
//        return 0;
//    }
//    virtual void DealParam(typename CQueue<T>::PPARAM* pParam) {
//        switch (pParam->nOperator)
//        {
//            case CQueue<T>::EQPush:
//                CQueue<T>::m_lstData.push_back(pParam->Data);
//                delete pParam;
//                //printf("delete %08p\r\n", (void*)pParam);
//                break;
//            case CQueue<T>::EQPop:
//                if (CQueue<T>::m_lstData.size() > 0)
//                {
//                    pParam->Data = CQueue<T>::m_lstData.front();
//                    if ((m_base->*m_callback)(pParam->Data) == 0)
//                    {
//                        CQueue<T>::m_lstData.pop_front();
//                    }
//                }
//                delete pParam;
//                break;
//            case CQueue<T>::EQSize:
//                pParam->nOperator = CQueue<T>::m_lstData.size();
//                if (pParam->hEvent != NULL)
//                {
//                    SetEvent(pParam->hEvent);
//                }
//                break;
//            case CQueue<T>::EQClear:
//                CQueue<T>::m_lstData.clear();
//                delete pParam;
//                //printf("delete %08p\r\n", (void*)pParam);
//                break;
//            default:
//                OutputDebugString(_T("unknown operator!\r\n"));
//                break;
//        }
//    }
//private:
//    ThreadFuncBase* m_base;
//    MCALLBACK m_callback;
//    CThread m_thread;
//};
//
//typedef CSendQueue<std::vector<char>>::MCALLBACK  SENDCALLBACK;
