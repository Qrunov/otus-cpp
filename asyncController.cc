#include "asyncController.h"
#include <shared_mutex>
using namespace std;
using namespace async;
shared_ptr<asyncController> asyncController::m_instance;


void asyncController::initializeGlobalHandler(async::handle_t g_handle)
{
    m_globalHandle = g_handle;
    m_connections[g_handle] -> m_parser -> setThreadSave(true);
    m_isGeneralHandlerInit = true;
}

shared_ptr<ICollector> asyncController::getCollector(async::handle_t h)
{
    remove_reference_t<decltype(m_connections[h])> p_conn;
    {
        std::shared_lock lock(m_globalLock);
        p_conn = m_connections[h];
    }
    shared_ptr<ICollector> coll = dynamic_pointer_cast<ICollector>(p_conn->m_collector);
    return coll;
}

void asyncController::process_private(shared_ptr<workQueue<commandBlock>> &q, shared_ptr<observer> &h)
{
    while (auto block = q->get())
        h->update(*block);
}

shared_ptr<asyncController> asyncController::getInstance()
{
    if (!m_instance)
        m_instance.reset(new asyncController());
    return m_instance;
}

shared_ptr<workQueue<commandBlock>> asyncController::fileQueue()
{
    if (!m_fileWorkQueue)
    {
        m_fileWorkQueue = make_shared<workQueue<commandBlock>>();
        auto fileLambda = [&]()
        {
            process<fileDestination>(m_fileWorkQueue);
        };

        m_t2 = make_shared<thread>(fileLambda);
        m_t3 = make_shared<thread>(fileLambda);
    }
    return m_fileWorkQueue;
}

shared_ptr<workQueue<commandBlock>> asyncController::consoleQueue()
{
    if (!m_consoleWorkQueue)
    {
        m_consoleWorkQueue = make_shared<workQueue<commandBlock>>();
        m_t1 = make_shared<thread>([&]()
                                   { process<consoleDestination>(m_consoleWorkQueue); });
    }
    return m_consoleWorkQueue;
}

void asyncController::freeAllQueue()
{
    if (m_consoleWorkQueue)
    {
        m_consoleWorkQueue->stopQueue();
        m_t1->join();
        m_consoleWorkQueue.reset();
    }

    if (m_fileWorkQueue)
    {
        m_fileWorkQueue->stopQueue();
        m_t2->join();
        m_t3->join();
        m_fileWorkQueue.reset();
    }
}

handle_t asyncController::connect_private(size_t bulk, shared_ptr<ICollector> &collector_)
{
    shared_ptr<async_context> p_con = make_shared<async_context>();
    p_con->m_collector = collector_;
    p_con->m_parser = make_shared<cmdParser>(p_con->m_collector, bulk);

    handle_t newHandle;
    {
        std::unique_lock lock(m_globalLock);
        m_seq++;
        newHandle = (handle_t)(m_seq);
        m_connections[newHandle] = p_con;
        if (m_isGeneralHandlerInit)
	    p_con->m_parser->setupNext(m_connections[m_globalHandle] -> m_parser);
    }
    return newHandle;
}

handle_t asyncController::connect(size_t bulk)
{
    static bool alreadyIn{false};
    if (!m_isGeneralHandlerInit && !alreadyIn)
    {
	alreadyIn = true;
        auto h = connect(bulk);
        initializeGlobalHandler(h);
	alreadyIn = false;
    }

    auto h = connect_t(bulk);
    remove_reference_t<decltype(m_connections[h])> p_conn;
    {
        std::shared_lock lock(m_globalLock);
        p_conn = m_connections[h];
    }

    shared_ptr<collector> coll = dynamic_pointer_cast<collector>(p_conn->m_collector);

    auto consoleDistination = make_shared<wqDestination>();
    consoleDistination->setQueue(consoleQueue());
    coll->registration(consoleDistination);

    auto fileDistination = make_shared<wqDestination>();
    fileDistination->setQueue(fileQueue());
    coll->registration(fileDistination);
    return h;
}

void asyncController::receiveEof(handle_t handle)
{
    remove_reference_t<decltype(m_connections[handle])> p_conn;
    {
        std::shared_lock lock(m_globalLock);
        if (!m_connections.count(handle))
        {
            cerr << "no appropriate client handler has found" << endl;
            return;
        }
        p_conn = m_connections[handle];
    }
    p_conn->m_parser->parseEnd();
}

void asyncController::receive(handle_t handle, const char *data, size_t size)
{
    remove_reference_t<decltype(m_connections[handle])> p_conn;
    {
        if (!m_connections.count(handle))
        {
            cerr << "no appropriate client handler has found" << endl;
            return;
        }

        p_conn = m_connections[handle];
    }
    stringstream sstr(string(data, size));
    while (!sstr.eof())
    {
        string oneLine;
        sstr >> oneLine;
        p_conn->m_parser->parseByString(oneLine);
    }
}

void asyncController::disconnect(handle_t handle)
{

    receiveEof(handle);
    {
        std::unique_lock lock(m_globalLock, defer_lock);
        lock.lock();
	m_connections.erase(handle);

//	if (m_isGeneralHandlerInit && 1 == m_connections.size() && m_connections.find(m_globalHandle) != m_connections.end())
//	{	
//	    lock.unlock();
//	    m_isGeneralHandlerInit = false;
//	    disconnect(m_globalHandle);
//	}
    }
}

asyncController::~asyncController()
{
    freeAllQueue();
}