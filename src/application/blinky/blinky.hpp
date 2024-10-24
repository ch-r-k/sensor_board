//============================================================================
// Blinky example
//============================================================================

#ifndef BLINKY_HPP_
#define BLINKY_HPP_

#include "i_user_indication.hpp"

namespace APP
{

class Blinky
{
   private:
    IUserIndication* userIndication = nullptr;

   public:
    Blinky();
    void setUserIndication(IUserIndication& initUserIndication);
    void run();
};

}  // namespace APP

#endif  // BLINKY_HPP_
