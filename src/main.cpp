//============================================================================
// APP example
//============================================================================
#include "qpcpp.hpp"  // QP/C++ real-time embedded framework
#include "system_manager.hpp"

//............................................................................
int main()
{
    static SystemManager systemManager;
    QP::QF::init();  // initialize the framework

    systemManager.run();
    return QP::QF::run();  // run the QF application
}
