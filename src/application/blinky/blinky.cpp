//============================================================================
// QP/C++ Real-Time Embedded Framework
//============================================================================
//! @date Last updated on: 2022-08-25
//! @version Last updated Zephyr 3.1.99 and @ref qpcpp_7_1_0
//!
//! @file
//! @brief Blinky example
//!
#include "blinky.hpp"
#include <common.hpp>

// unnamed namespace for local definitions with internal linkage
namespace
{
// Q_DEFINE_THIS_FILE

}  // unnamed namespace

namespace APP
{
//............................................................................
Blinky::Blinky()
{
    // empty
}

void Blinky::run()
{
    for(;;)
    {
        userIndication->reset();
        //busyWaiting(100);
        userIndication->set();
        //busyWaiting(100);
    }
}

//............................................................................
void Blinky::setUserIndication(IUserIndication& initUserIndication)
{
    userIndication = &initUserIndication;
}

}  // namespace APP
