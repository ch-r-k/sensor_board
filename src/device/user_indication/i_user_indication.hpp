#ifndef I_USER_INDICATION_HPP
#define I_USER_INDICATION_HPP

class IUserIndication
{
   private:
   public:
    virtual void set() = 0;
    virtual void reset() = 0;
};

#endif  // OUTPUT_PIN_HPP