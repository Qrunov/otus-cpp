#pragma once
#include <iostream>
#include <memory>
#include <atomic>
#include <map>
#include <thread>
#include <shared_mutex>
#include "fileDestination.h"
#include "consoleDestination.h"
#include "wqDestination.h"
#include "parser.h"
#include "async.h"
#include "parser.h"
#include "source.h"
#include "collector.h"
#include "opencollector.h"
#include "async_queue.h"


struct async_context
{
    std::shared_ptr<cmdParser> m_parser;
    std::shared_ptr<ICollector> m_collector;
};

class asyncController
{
public:
    static std::shared_ptr<asyncController> getInstance();
 
    std::shared_ptr<workQueue<commandBlock>> fileQueue();
    std::shared_ptr<workQueue<commandBlock>> consoleQueue();
    void freeAllQueue();

    void initializeGlobalHandler(async::handle_t);

    template <class Collector = collector>
    async::handle_t connect_t(size_t bulk)
    {
        shared_ptr<ICollector> coll = make_shared<Collector>();
        return connect_private(bulk, coll);
    }

    async::handle_t connect(size_t bulk);
    void receiveEof(async::handle_t handle);
    void receive(async::handle_t handle, const char *data, size_t size);
    void disconnect(async::handle_t handle);
    ~asyncController();
    shared_ptr<ICollector> getCollector(async::handle_t handle);

private:
    explicit asyncController() {};

    void process_private(shared_ptr<workQueue<commandBlock>> &q, shared_ptr<observer> &h);
    async::handle_t connect_private(size_t bulk, shared_ptr<ICollector> &c);

    template <class handler>
    void process(shared_ptr<workQueue<commandBlock>> &q)
    {
        shared_ptr<observer> h = make_shared<handler>();
        process_private(q, h);
    }

    std::shared_ptr<workQueue<commandBlock>> m_consoleWorkQueue;
    std::shared_ptr<workQueue<commandBlock>> m_fileWorkQueue;
    static std::shared_ptr<asyncController> m_instance;

    std::shared_ptr<std::thread> m_t1;
    std::shared_ptr<std::thread> m_t2;
    std::shared_ptr<std::thread> m_t3;

    uint64_t m_seq{1};
    std::map<async::handle_t, std::shared_ptr<async_context>> m_connections;
    std::shared_mutex m_globalLock;

    async::handle_t	m_globalHandle;
    bool		m_isGeneralHandlerInit{false};
};
