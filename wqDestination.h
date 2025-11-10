#pragma once
#include <memory>
#include "async_queue.h"
#include "interfaces.h"

class wqDestination : public observer
{
public:
    void update(const commandBlock &b) override;

    void setQueue(std::shared_ptr<workQueue<commandBlock>> queue)
    {
	m_queue = queue;
    }

private:
    std::shared_ptr<workQueue<commandBlock>>	m_queue;
};
