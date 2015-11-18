#ifndef VIRTUALCONNECTION_H
#define VIRTUALCONNECTION_H

#include <memory>
#include "ConnectionInterface.h"

namespace Staff {
    std::unique_ptr<FSInterface::ConnectionInterface> empty();
    std::unique_ptr<FSInterface::ConnectionInterface> hello();
}
#endif // VIRTUALCONNECTION_H
