#pragma once

#include <thread>
#include <iostream>
#include <string>
#include <memory>
#include <vector>
#include <list>
#include <mutex>
#include <future>
#include <optional>
#include <functional>

/**
* @brief  :测试 thread 创建线程，join detach joinable, 函数指针，lambda，可调用对象
* @author :fmw
* @date   :
**/
void testThread();

/**
* @brief  :测试 线程传递参数
* @author :fmw
* @date   :
**/
void  testParameter();


//void myprint(const int& mvar, char* mybuf);
void myprint(const int mvar, const std::string& mybuf);

class  Obj
{
private:
	int mNum;
public:
	Obj(int num) :mNum(num)
	{
		std::cout << " Obj 构造函数 " << " -- threadId = " << std::this_thread::get_id() << std::endl;;
	};

	Obj(const Obj& obj)
	{
		std::cout << "Obj  拷贝构造函数 " << " -- threadId = " << std::this_thread::get_id() << std::endl;;
	}
	~Obj()
	{
		std::cout << "Obj  析构函数 " << " -- threadId = " << std::this_thread::get_id() << std::endl;;
	}

	void setNum(int num)
	{
		mNum = num;
		std::cout << __FILE__ << "(" << __LINE__ << "): " << __func__ << " HHE " << std::endl;
	}

	void thread_work();

	void operator()(int num);

};


/**
* @brief  :测试使用ref 传递参数可以不用 const 修饰引用
* @author :fmw
* @date   :
**/
void testRef();
void refFun(Obj& obj);


/**
* @brief  :测试智能指针作为参数
* @author :fmw
* @date   :
**/
void testPtr();
void ptrFun(std::unique_ptr<int> upn);

//用成员函数作为线程参数
void testMem();


/**
* @brief  :创建多个线程
* @author :fmw
* @date   :
**/
void createMultiThread();
void myPrint(int num);


/**
* @brief  :测试多线程访问 相同的资源
* @author :fmw
* @date   :
**/
class Message
{
public:
	void pushMes();
	void popMes();
	bool outMsgProc(int& num);
private:
	std::list<int> mesList;
	std::mutex mesMut;
};
void testMesThread();


/**
* @brief  :测试死锁， adopt_lock 参数
* @author :fmw
* @date   :
**/
void testDeadLock();


/**
* @brief  :测试 unique_lock的用法
* @author :fmw
* @date   :
**/
void testUniqueLock();


/**
* @brief  :测试 call_once 的用法
* @author :fmw
* @date   :
**/
extern std::once_flag flag;
void testCallOnce();
void fun_call_once();

/*
*@brief: 双重锁定资源
*/
void testDoubleLock();


/**
* @brief: 测试条件变量
**/
void testCondition();


/**
* @brief  :测试 async、 furture
**/
void testAsync();
void testAsync2();





/**
* @brief  :测试 packaged_task
**/
void testPackageTask();

/**
* @brief  :测试 std:: promise
**/
void testPromise();
void mythread(std::promise<int>& prom);


/**
* @brief  :测试 future 的其他接口， wait_for 规定时间， 返回三个状态， 超时、正常、
**/
void testFuture();
/*
* @brief: 测试 两个子线程，一个线程以 package_task 作为可调用对象，
*另一个线程以 它作为参数，在内部调用get方法，控制线程的顺序
*/
void testFuture2();
/*
* @brief: 测试 shared_future ：可以实现多次get
*/
void testFuture3();




/**
* @brief  :测试只有一行语句的代码，多线程执行也会出错
**/
void testAtom();
/**
* @brief  :测试只有一行语句的代码，多线程执行也会出错
**/
void testAtom2();


/*
* 测试windows临界区
*/
void testWinsec();
#include <Windows.h>
#define __WINDOWSJQ_
#ifdef __WINDOWSJQ_
extern CRITICAL_SECTION my_winsec;
#endif

/**
* @brief  :临界区，自动加锁，自动解锁
**/
class CWinLock
{
public:
	CWinLock(CRITICAL_SECTION* pCritmp) //构造函数
	{
		m_pCritical = pCritmp;
		EnterCriticalSection(m_pCritical);
	}
	~CWinLock()
	{
		LeaveCriticalSection(m_pCritical);
	}
private:
	CRITICAL_SECTION* m_pCritical;
};

/*
* 测试windows临界区,使用上面的自动加锁解锁类
*/
void testWinsec2();


void testReentrantLock();
