#include "async.h"
#include "asyncController.h"

namespace async
{
    handle_t connect(size_t bulk)
    {
        return asyncController::getInstance()->connect(bulk);
    }

    void receive(handle_t handle, const char *data, std::size_t size)
    {
        asyncController::getInstance()->receive(handle, data, size);
    }

    void disconnect(handle_t handle)
    {
        asyncController::getInstance()->disconnect(handle);
    }
}
