#pragma once
#include <memory>
#include "async_queue.h"
#include "interfaces.h"

class wqDestination : public observer
{
public:
    void update(const block &b) override;

    void setQueue(std::shared_ptr<workQueue<block>> queue)
    {
	m_queue = queue;
    }

private:
    std::shared_ptr<workQueue<block>>	m_queue;
};
