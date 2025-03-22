#include "Thread.h"
#include <functional>
using std::cout;
using std::endl;
using std::thread;
using std::string;
using std::vector;


vector<int> buffer{ 1,2,3,4,5,6,7 };

void fun1()
{
	cout << __FILE__ << "(" << __LINE__ << "): " << __func__ << " fun" << endl;
	cout << __FILE__ << "(" << __LINE__ << "): " << __func__ << " fun2" << endl;
	cout << __FILE__ << "(" << __LINE__ << "): " << __func__ << " fun - end" << endl;
}

struct FunObj
{
	void operator()()
	{
		cout << __FILE__ << "(" << __LINE__ << "): " << __func__ << " FunObj" << endl;
		cout << __FILE__ << "(" << __LINE__ << "): " << __func__ << " FunObj" << endl;
		cout << __FILE__ << "(" << __LINE__ << "): " << __func__ << " FunObj - end" << endl;
	}

	void testFunMem()
	{
		cout << __FILE__ << "(" << __LINE__ << "): " << __func__ << " FunObj" << endl;
		cout << __FILE__ << "(" << __LINE__ << "): " << __func__ << " FunObj" << endl;
		cout << __FILE__ << "(" << __LINE__ << "): " << __func__ << " FunObj" << endl;
		cout << __FILE__ << "(" << __LINE__ << "): " << __func__ << " FunObj" << endl;
		cout << __FILE__ << "(" << __LINE__ << "): " << __func__ << " FunObj" << endl;
		cout << __FILE__ << "(" << __LINE__ << "): " << __func__ << " FunObj" << endl;
	}
};

void testThread()
{
	//1. 函数指针
	//thread  myObj(fun1);

	//2. lambda 表达式
	//thread  myObj([&]() {
	//	cout << " 开始执行子线程1" << endl;
	//	cout << " 开始执行子线程2" << endl;
	//	cout << " 子线程执行结束" << endl;
	//	});

	//3. 可调用对象
	//FunObj funObj;
	//thread myObj(funObj); //是通过拷贝传递函数对象

	//4.成员函数
	FunObj funObj;
	thread myObj(&FunObj::testFunMem, &funObj);
	myObj.detach();
	//Sleep(1000);
	//join会阻塞主线程的执行
	// 必须调用join 或者是 detach,否则主线程执行结束，子线程还没结束，会出现异常
	//myObj.join();
	//使用detach() 子线程失去与主线程的关联，此时子线程会驻留在后台运行，被C++运行时库接管
	//myObj.detach();
	//if (myObj.joinable())
	//{
	//	cout << " joinable = true" << endl;
	//}
	//else
	//{
	//	cout << " joinable = false" << endl;
	//}
}

void testParameter()
{
	//1.传递临时对象作为线程参数
	int mvar = 100;
	int& mvary = mvar;
	char mybuf[] = "this is a test";

	//线程传递 int 引用 的值做了一个赋值 传参数参数 var 地址与 mvar 的地址不同，detach 后的结果是安全的
	//但是对指针的传递是不安全的（myprint 的引用参数必须加 const），主线程已经释放了局部变量，子线程再使用会导致问题
	//传参的时候构建一个新的临时对象可以避免 detach失效
	thread mytobj(myprint, mvar, string(mybuf));
	mytobj.detach();

	//1. 使用引用
	cout << __FILE__ << "(" << __LINE__ << "): " << __func__ << " " << endl;
	cout << __FILE__ << "(" << __LINE__ << "): " << __func__ << " " << endl;
	cout << __FILE__ << "(" << __LINE__ << "): " << __func__ << " " << endl;
	cout << __FILE__ << "(" << __LINE__ << "): " << __func__ << " " << endl;
	cout << __FILE__ << "(" << __LINE__ << "): " << __func__ << " " << endl;
	cout << __FILE__ << "(" << __LINE__ << "): " << __func__ << " " << endl;
	cout << __FILE__ << "(" << __LINE__ << "): " << __func__ << " " << endl;


	cout << " main thread_id = " << std::this_thread::get_id() << endl;

}


//void myprint(const int& var, char* mybuf)
void myprint(const int var, const string& mybuf)
{
	cout << __FILE__ << "(" << __LINE__ << "): " << __func__ << " var  = " << var << endl;
	cout << __FILE__ << "(" << __LINE__ << "): " << __func__ << " mybuf  = " << mybuf << endl;
}


void refFun(Obj& obj)
{
	cout << __FILE__ << "(" << __LINE__ << "): " << __func__ << " &Obj.i" << &obj << endl;
	//cout << __FILE__ << "(" << __LINE__ << "): " << __func__ << " -- threadId = " << std::this_thread::get_id() << endl;
}

void testPtr()
{
	std::unique_ptr<int> myu(new int(100));
	std::thread mythread(ptrFun, std::move(myu));
	mythread.join();
	cout << __FILE__ << "(" << __LINE__ << "): " << __func__ << "e" << endl;
}

void ptrFun(std::unique_ptr<int> upn)
{
	cout << __FILE__ << "(" << __LINE__ << "): " << __func__ << " ptr = " << *upn;
}

void testMem()
{
	Obj o(1);
	//成员函数
	//thread myThread(&Obj::thread_work, Obj(1));
	//函数对象
	thread myThread(std::ref(o), 1);
	myThread.join();
}

void createMultiThread()
{
	vector<thread> thVct;
	for (size_t i = 0; i < 10; ++i)
	{
		thVct.emplace_back(myPrint, i);
	}
	for (auto& i : thVct)
	{
		i.join();
		cout << __FILE__ << "(" << __LINE__ << "): " << __func__ << "" << endl;
	}
	cout << __FILE__ << "(" << __LINE__ << "): " << __func__ << "  " << endl;
}

void myPrint(int num)
{
	cout << num << endl;
}

void testMesThread()
{

	Message mes;
	std::thread generateThread(&Message::pushMes, &mes);
	std::thread consumeThread(&Message::popMes, &mes);

	generateThread.join();
	consumeThread.join();
}

void testRef()
{
	int var = 1;
	Obj obj(var);
	//cout << __FILE__ << "(" << __LINE__ << "): " << __func__ << " -- main threadId = " << std::this_thread::get_id() << endl;
	cout << __FILE__ << "(" << __LINE__ << "): " << __func__ << " -- &obj = " << &obj << endl;
	//thread myThread(refFun, obj); //使用ref 可以不用const修饰，而且是引用传递
	thread myThread(refFun, std::ref(obj));
	myThread.join();
	//myThread.detach();
}

void testDeadLock()
{

	std::mutex mutexA;
	std::mutex mutexB;

	thread threadA([&]() {
		for (size_t i = 0; i < 1000; ++i)
		{
			cout << "threadA i = " << i << endl;
			//mutexA.lock();
			//mutexB.lock();
			//mutexA.unlock();
			//mutexB.unlock();

			std::lock(mutexA, mutexB);
			//搭配 adopt_lock 可以实现不 unlock
			std::lock_guard<std::mutex> lgA(mutexA, std::adopt_lock);
			std::lock_guard<std::mutex> lgB(mutexB, std::adopt_lock);
			/*	mutexA.unlock();
				mutexB.unlock();*/
		}
		});
	thread threadB([&]() {
		for (size_t i = 0; i < 1000; ++i)
		{
			cout << "threadB i = " << i << endl;
			std::lock(mutexA, mutexB);
			mutexA.unlock();
			mutexB.unlock();
		}
		});
	threadA.join();
	threadB.join();

}


void Obj::thread_work()
{
	cout << __FILE__ << "(" << __LINE__ << "): " << __func__ << " thread_work " << endl;
}

void Obj::operator()(int num)
{
	cout << __FILE__ << "(" << __LINE__ << "): " << __func__ << " ee" << endl;
}

void Message::pushMes()
{
	std::lock_guard<std::mutex> lg(mesMut);
	for (size_t i = 0; i < 10000; ++i)
	{
		cout << " push  i = " << i << endl;
		//mesMut.lock();
		//mesMut.unlock();
		mesList.push_back(i);
	}
}

void Message::popMes()
{
	for (size_t i = 0; i < 10000; ++i)
	{
		/*	if (!mesList.empty())
			{
				cout << " mesList.front() = " << mesList.front() << endl;
				mesMut.lock();
				mesList.pop_front();
				mesMut.unlock();
			}
			else
			{
				cout << " mesList is empty()" << endl;
			}*/
		int num = 0;
		if (outMsgProc(num))
		{
			cout << "pop num = " << num << endl;
		}
	}
}

bool Message::outMsgProc(int& num)
{
	std::lock_guard<std::mutex> lg(mesMut);
	if (!mesList.empty())
	{
		num = mesList.front();
		mesList.pop_front();
		return true;
	}
	return false;
}



void testUniqueLock()
{
	std::mutex mutexA;
	std::list<int> mesList;
	thread threadA([&]() {
		for (size_t i = 0; i < 1000; ++i)
		{
			cout << "threadA i = " << i << endl;
			//2. adopt_lock 参数
			//mutexA.lock();
			//std::unique_lock<std::mutex>  ulm(mutexA, std::adopt_lock);
			//1.代替 lock_guard
			std::unique_lock<std::mutex>  ulm(mutexA);
			/*		std::chrono::milliseconds dura(2);
					std::this_thread::sleep_for(dura);*/
			mesList.push_back(i);
		}
		});
	thread threadB([&]() {
		for (size_t i = 0; i < 1000000; ++i)
		{
			//3. try_to_lock
			std::unique_lock<std::mutex>  ulm(mutexA, std::try_to_lock);
			if (ulm.owns_lock())
			{
				if (!mesList.empty())
				{
					cout << "threadB i = " << mesList.front() << endl;
					mesList.pop_front();
				}
			}
			else
			{
				cout << "threadB 没有拿到锁" << endl;
			}

		}
		});
	threadA.join();
	threadB.join();
}

std::once_flag flag;
void testCallOnce()
{
	std::mutex mutexA;
	thread threadA([&]() {
		for (size_t i = 0; i < 10; ++i)
		{
			std::call_once(flag, fun_call_once);
			cout << " threadA i = " << i << endl;

		}
		});
	thread threadB([&]() {
		for (size_t i = 0; i < 10; ++i)
		{
			std::call_once(flag, fun_call_once);
			cout << " threadB i = " << i << endl;
		}
		});
	threadA.join();
	threadB.join();
}

void fun_call_once()
{
	cout << "fun_call_once -- " << endl;
	std::chrono::milliseconds time(5000);
	std::this_thread::sleep_for(time);
}


void testDoubleLock()
{
	std::mutex mutexA;
	std::list<int> mesList;
	bool flag = true;
	thread threadA([&]() {
		for (size_t i = 0; i < 1000; ++i)
		{
			cout << "threadA i = " << i << endl;
			std::unique_lock<std::mutex>  ulm(mutexA);
			mesList.push_back(i);
		}
		flag = false;
		});
	thread threadB([&]() {
		while (flag)
		{
			if (!mesList.empty())
			{
				std::unique_lock<std::mutex>  ulm(mutexA);
				if (!mesList.empty())
				{
					cout << "threadB i = " << mesList.front() << endl;
					mesList.pop_front();
				}
			}
		}
		});
	threadA.join();
	threadB.join();
}


void testCondition()
{
	std::mutex mutexA;
	std::list<int> mesList;
	std::condition_variable my_cond;
	bool flag = true;
	thread threadA([&]() {
		for (size_t i = 0; i < 1000; ++i)
		{
			cout << "threadA i = " << i << endl;
			std::unique_lock<std::mutex>  ulm(mutexA);
			mesList.push_back(i);
			my_cond.notify_one();
		}
		flag = false;
		});
	thread threadB([&]() {
		while (flag)
		{
			std::unique_lock<std::mutex>  ulm(mutexA);
			my_cond.wait(ulm, [&]() {
				if (!mesList.empty())
				{
					return true;
				}
				return false;
				});
			cout << "threadB i = " << mesList.front() << endl;
			mesList.pop_front();
		}
		});

	thread threadC([&]() {
		while (flag)
		{
			std::unique_lock<std::mutex>  ulm(mutexA);
			my_cond.wait(ulm, [&]() {
				if (!mesList.empty())
				{
					return true;
				}
				return false;
				});
			cout << "threadC i = " << mesList.front() << endl;
			mesList.pop_front();
		}
		});
	threadA.join();
	threadB.join();
	threadC.join();
}


void testAsync()
{
	cout << "main thread id = " << std::this_thread::get_id() << endl;
	std::future<int> result = std::async(std::launch::deferred, []() {
		cout << " begin  subThread id =  " << std::this_thread::get_id() << endl;
		std::this_thread::sleep_for(std::chrono::milliseconds(5000));
		cout << " begin  subThread id =  " << std::this_thread::get_id() << endl;
		return 10;
		});
	int num = 0;
	num = result.get();
	cout << " main thread gets num  = " << num << endl;
}

void testAsync2()
{
	cout << "main thread id = " << std::this_thread::get_id() << endl;
	std::future<int> result = std::async(std::launch::async, []() {
		cout << "threadA begin  subThread id =  " << std::this_thread::get_id() << endl;
		std::this_thread::sleep_for(std::chrono::milliseconds(5000));
		cout << "threadA begin  subThread id =  " << std::this_thread::get_id() << endl;
		return 10;
		});

	std::thread threadB([](std::future<int>& sf) {
		cout << "threadB begin  subThread id =  " << std::this_thread::get_id() << endl;
		int num = sf.get();
		cout << " num = " << num << endl;
		cout << "threadB begin  subThread id =  " << std::this_thread::get_id() << endl;
		}, std::ref(result));

	threadB.join();
}


void testPackageTask()
{
	//包装一个对象，实现一个动态绑定的效果
	cout << "main " << " threadid = " << std::this_thread::get_id() << endl;
	std::packaged_task<int(int)> mypt([](int num)->int {
		cout << " begin  subThread id =  " << std::this_thread::get_id() << endl;
		std::this_thread::sleep_for(std::chrono::milliseconds(5000));
		++num;
		cout << " begin  subThread id =  " << std::this_thread::get_id() << endl;
		return num;
		});
	std::thread threadA(std::ref(mypt), 2);
	threadA.join();
	std::future<int> result = mypt.get_future();
	cout << " result = " << result.get() << endl;
}



void testPromise()
{
	cout << "main threadid = " << std::this_thread::get_id() << endl;
	std::promise<int> prom;
	std::thread threadA(mythread, std::ref(prom));
	cout << "main threadId = " << std::this_thread::get_id() << endl;
	threadA.join();
	std::future<int> fui = prom.get_future();
	auto result = fui.get();
	cout << "main thread result = " << result << endl;
}

void mythread(std::promise<int>& prom)
{
	cout << "begin thisThreadId = " << std::this_thread::get_id() << endl;
	prom.set_value(111);
	std::this_thread::sleep_for(std::chrono::milliseconds(5000));
	cout << "end thisThreadId = " << std::this_thread::get_id() << endl;
}

void testFuture()
{
	cout << "main thread id = " << std::this_thread::get_id() << endl;
	std::future<int> result = std::async([]() {
		cout << " begin  subThread id =  " << std::this_thread::get_id() << endl;
		std::this_thread::sleep_for(std::chrono::milliseconds(2000));
		cout << " begin  subThread id =  " << std::this_thread::get_id() << endl;
		return 10;
		});
	int num = 0;
	std::future_status status = result.wait_for(std::chrono::seconds(3));
	if (status == std::future_status::timeout)
	{
		cout << " sub thread execute exceed time!" << endl;
	}
	else if (status == std::future_status::ready)
	{
		cout << " sub thread execute on time!" << endl;
	}
	else if (status == std::future_status::deferred)
	{
		cout << " sub thread 延迟执行！" << endl;
	}
	cout << " main thread gets num  = " << num << endl;
}

void testFuture2()
{
	cout << "main thread id = " << std::this_thread::get_id() << endl;
	auto myFun1 = [](int)
	{
		cout << " begin subThreadA id =  " << std::this_thread::get_id() << endl;
		std::this_thread::sleep_for(std::chrono::milliseconds(5000));
		int num = 100;
		cout << " begin subThreadA id =  " << std::this_thread::get_id() << endl;
		return num;
	};
	std::packaged_task<int(int)> spd(myFun1);
	std::mutex mutexA;
	thread threadA(std::ref(spd), 100);
	thread threadB([](std::packaged_task<int(int)>& spd) {
		cout << " begin subThreadB id =  " << std::this_thread::get_id() << endl;
		std::future<int> sfi = spd.get_future();
		int num = sfi.get();
		cout << " begin subThreadB id =  " << std::this_thread::get_id() << " num = " << num << endl;
		}, std::ref(spd));
	threadA.join();
	threadB.join();
}

void testFuture3()
{
	cout << "main thread id = " << std::this_thread::get_id() << endl;
	auto myFun1 = [](int)
	{
		cout << " begin subThreadA id =  " << std::this_thread::get_id() << endl;
		std::this_thread::sleep_for(std::chrono::milliseconds(5000));
		int num = 100;
		cout << " begin subThreadA id =  " << std::this_thread::get_id() << endl;
		return num;
	};
	std::packaged_task<int(int)> spd(myFun1);
	std::mutex mutexA;
	thread threadA(std::ref(spd), 100);
	thread threadB([](std::packaged_task<int(int)>& spd) {
		cout << " begin subThreadB id =  " << std::this_thread::get_id() << endl;
		std::shared_future<int> sfi = spd.get_future();
		int num = sfi.get();
		int num2 = sfi.get();

		cout << " begin subThreadB id =  " << std::this_thread::get_id() << " num = " << num << " num2 = " << num2 << endl;
		}, std::ref(spd));
	threadA.join();
	threadB.join();
}

void testAtom()
{
	int my_count = 0;
	auto myFun1 = [](int& my_count)
	{
		for (size_t i = 0; i < 1000; ++i)
		{
			my_count++; //只有一行语句，但是cpu 会分几步执行
		}
	};
	thread threadA(myFun1, std::ref(my_count));
	thread threadB(myFun1, std::ref(my_count));
	threadA.join();
	threadB.join();
	cout << " result = " << my_count << endl;
}


void testAtom2()
{
	std::atomic<int> my_count = 0;
	auto myFun1 = [](std::atomic<int>& my_count)
	{
		for (size_t i = 0; i < 1000000; ++i)
		{
			//my_count++; //只有一行语句，但是cpu 会分几步执行
			my_count.fetch_add(1, std::memory_order_relaxed);
		}
	};
	thread threadA(myFun1, std::ref(my_count));
	thread threadB(myFun1, std::ref(my_count));
	threadA.join();
	threadB.join();
	cout << " result = " << my_count << endl;
}




#ifdef __WINDOWSJQ_
CRITICAL_SECTION my_winsec;
#endif


void testWinsec()
{
#ifdef __WINDOWSJQ_
	InitializeCriticalSection(&my_winsec); //使用临界区之前要先初始化
#endif

	std::mutex mutexA;
	std::atomic<bool> flag = true;
	std::list<int> mesList;
	thread threadA([&]() {
		for (size_t i = 0; i < 1000; ++i)
		{
			cout << "threadA i = " << i << endl;
#ifdef __WINDOWSJQ_
			EnterCriticalSection(&my_winsec);
			mesList.push_back(i);
			LeaveCriticalSection(&my_winsec);
#else
			std::unique_lock<std::mutex>  ulm(mutexA);
			mesList.push_back(i);
#endif
		}
		flag.store(false);
		});

	thread threadB([&]() {
		while (flag.load())
		{
#ifdef __WINDOWSJQ_
			EnterCriticalSection(&my_winsec);
			if (!mesList.empty())
			{
				cout << "threadB i = " << mesList.front() << endl;
				mesList.pop_front();
			}
			LeaveCriticalSection(&my_winsec);
#else
			std::unique_lock<std::mutex>  ulm(mutexA);
			if (!mesList.empty())
			{
				cout << "threadB i = " << mesList.front() << endl;
				mesList.pop_front();
			}
#endif
		}
		});
	threadA.join();
	threadB.join();
}

void testWinsec2()
{
#ifdef __WINDOWSJQ_
	CRITICAL_SECTION my_winsec2;
	InitializeCriticalSection(&my_winsec2); //使用临界区之前要先初始化
#endif
	std::mutex mutexA;
	std::atomic<bool> flag = true;
	std::list<int> mesList;
	thread threadA([&]() {
		for (size_t i = 0; i < 1000; ++i)
		{
			cout << "threadA i = " << i << endl;
#ifdef __WINDOWSJQ_
			CWinLock cl(&my_winsec2);
			mesList.push_back(i);
#else
			std::unique_lock<std::mutex>  ulm(mutexA);
			mesList.push_back(i);
#endif
		}
		flag.store(false);
		});

	thread threadB([&]() {
		while (flag.load())
		{
#ifdef __WINDOWSJQ_
			CWinLock cl(&my_winsec2);
			if (!mesList.empty())
			{
				cout << "threadB i = " << mesList.front() << endl;
				mesList.pop_front();
			}
#else
			std::unique_lock<std::mutex>  ulm(mutexA);
			if (!mesList.empty())
			{
				cout << "threadB i = " << mesList.front() << endl;
				mesList.pop_front();
			}
#endif
		}
		});
	threadA.join();
	threadB.join();
}

void testReentrantLock()
{


}
