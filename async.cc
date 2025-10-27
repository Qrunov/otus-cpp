#include <iostream>
#include <memory>
#include <atomic>
#include <map>
#include <thread>
#include "fileDestination.h"
#include "consoleDestination.h"
#include "wqDestination.h"
#include "parser.h"
#include "async.h"
#include "parser.h"
#include "source.h"
#include "collector.h"
#include "opencollector.h"
#include "async_context.h"
#include "async_queue.h"
using namespace async;
using namespace std;

/*
void	processConsoleWorkQueue(shared_ptr<workQueue<block>> &q)
{
    auto console = make_shared<consoleDestination>();
    while(auto block = q -> get())
	console -> update(*block);
}

void	processFileWorkQueue(shared_ptr<workQueue<block>> &q)
{
    auto file = make_shared<fileDestination>();
    while(auto block = q -> get())
	file -> update(*block);
}
*/

template <class handler>
void 	process(shared_ptr<workQueue<block>> &q)
{
    auto h = make_shared<handler>();
    while(auto block = q -> get())
	h -> update(*block);
}

shared_ptr<thread>	t1;
shared_ptr<thread>	t2;
shared_ptr<thread>	t3;
mutex			async_lock;


class	queueHolder
{
public:
    static shared_ptr<queueHolder> getInstance()
    {
	if (!m_instance)
	    m_instance.reset(new queueHolder());
	return m_instance;
    }

    shared_ptr<workQueue<block>> fileQueue()
    {
	if (!m_fileWorkQueue)
	{
	    m_fileWorkQueue = make_shared<workQueue<block>>();
	    auto fileLambda = [&]()
		{
		    process<fileDestination>(m_fileWorkQueue);
		};

	    t2 = make_shared<thread>(fileLambda);
	    t3 = make_shared<thread>(fileLambda);
	}
	return m_fileWorkQueue;
    }

    shared_ptr<workQueue<block>> consoleQueue()
    {
	if (!m_consoleWorkQueue)
	{
	    m_consoleWorkQueue = make_shared<workQueue<block>>();
	    t1 = make_shared<thread>([&]()
		{
		    process<consoleDestination>(m_consoleWorkQueue);
		}
	    );
	}
	return m_consoleWorkQueue;
    }

    void freeAllQueue()
    {
	if (m_consoleWorkQueue)
	{
	    m_consoleWorkQueue -> stopQueue();
	    t1 -> join();
	    m_consoleWorkQueue.reset();
	}

	if (m_fileWorkQueue)
	{
	    m_fileWorkQueue -> stopQueue();
	    t2 -> join();
	    t3 -> join();
	    m_fileWorkQueue.reset();
	}
    }
    ~queueHolder()
    {
	freeAllQueue();
    }
private:
    explicit queueHolder() {};

    shared_ptr<workQueue<block>>	m_consoleWorkQueue;
    shared_ptr<workQueue<block>>	m_fileWorkQueue;
    static shared_ptr<queueHolder> m_instance;

};

shared_ptr<queueHolder> queueHolder::m_instance;

namespace async {

atomic<handle_t>	seq{(handle_t)1};
map<handle_t, shared_ptr<async_context>> connections;

template <class Collector = collector>
handle_t connect_t(size_t bulk)
{

    handle_t  newHandle = atomic_fetch_add(&seq, (uint32_t)1);

    shared_ptr<async_context> p_con =  make_shared<async_context>();
    auto coll = make_shared<Collector>();
    p_con -> m_collector = coll;
    p_con -> m_parser = make_shared<cmdParser>(p_con -> m_collector, bulk);
/*    auto consoleDistination = make_shared<wqDestination>();
    consoleDistination -> setQueue(queueHolder::getInstance() -> consoleQueue());
    coll -> registration(consoleDistination);

    auto fileDistination = make_shared<wqDestination>();
    fileDistination -> setQueue(queueHolder::getInstance() -> fileQueue());
    coll -> registration(fileDistination);*/
//    coll -> registration(queueHolder::getInstance() -> consoleQueue());

    connections[newHandle] = p_con;
    return newHandle;
}

template handle_t connect_t<collector>(size_t);
template handle_t connect_t<openCollector>(size_t);

handle_t connect(size_t bulk)
{
    lock_guard<mutex>	lock(async_lock);
    auto h = connect_t(bulk);
    auto p_conn = connections[h];
    shared_ptr<collector> coll = dynamic_pointer_cast<collector>(p_conn -> m_collector);

    auto consoleDistination = make_shared<wqDestination>();
    consoleDistination -> setQueue(queueHolder::getInstance() -> consoleQueue());
    coll -> registration(consoleDistination);

    auto fileDistination = make_shared<wqDestination>();
    fileDistination -> setQueue(queueHolder::getInstance() -> fileQueue());
    coll -> registration(fileDistination);

    return h;
}

void receiveEof(handle_t handle)
{
    if (!connections.count(handle))
    {
	cerr << "no appropriate client handler has found" << endl;
	return;
    }
    auto p_con = connections[handle];

    shared_ptr<grubFromString>	graber = make_shared<grubFromString>();
    string empty;
    graber -> setData(empty);
    p_con -> m_parser -> parseIt(graber, true);
}

void receive(handle_t handle,const char *data,size_t size)
{
    lock_guard<mutex>	lock(async_lock);
    if (!connections.count(handle))
    {
	cerr << "no appropriate client handler has found" << endl;
	return;
    }

    auto p_con = connections[handle];

    shared_ptr<grubFromString>	graber = make_shared<grubFromString>();
    graber -> setData(string(data,size));
    p_con -> m_parser -> parseIt(graber, false);
}

void disconnect(handle_t handle)
{
    lock_guard<mutex>	lock(async_lock);
    receiveEof(handle);
    connections.erase(handle);
}

}