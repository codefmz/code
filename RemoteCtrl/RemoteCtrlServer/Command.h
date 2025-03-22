#ifndef _COMMAND_H_
#define _COMMAND_H_

#include <unordered_map>
#include <list>
class Packet;
using std::unordered_map;
class CCommand
{
    typedef int(CCommand::* CMD_FUNC)(std::list<Packet>& lstPacket, Packet& inPacket); //定义函数指针
private:
    unordered_map<int, CMD_FUNC> umCmdFun;
    unsigned threadId;
private:
    CCommand();
    ~CCommand();
public:
    static CCommand* instance();
    int ExecuteCommand(int nCmd, std::list<Packet>& lstPacket, Packet& inPacket);
    static void RunCommand(void* arg, int sCmd, std::list<Packet>& lstPacket, Packet& inPacket);
    static void ProcPacket(Packet* packet, std::list<Packet>& lstPacket);

protected:
    int ConnectTest(std::list<Packet>& lstPacket, Packet& inPacket);  //查看磁盘分区信息
    int GetDriveInfo(std::list<Packet>& lstPacket, Packet& inPacket);  //查看磁盘分区信息
    int GetDirInfo(std::list<Packet>& lstPacket, Packet& inPacket); //获取目录的文件信息
    int RunFile(std::list<Packet>& lstPacket, Packet& inPacket); //运行程序
    int MouseEvent(std::list<Packet>& lstPacket, Packet& inPacket);//鼠标移动事件
    int DeleteFile(std::list<Packet>& lstPacket, Packet& inPacket);
private:
    bool DirNameFilter(char* name);
};

#endif // !_COMMAND_H_