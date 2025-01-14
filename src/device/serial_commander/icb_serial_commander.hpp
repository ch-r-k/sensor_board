#ifndef ICB_SERIAL_COMMANDER_HPP
#define ICB_SERIAL_COMMANDER_HPP

class IcbSerialCommander
{
   private:
   public:
    virtual ~IcbSerialCommander() = default;
    virtual void done() = 0;
};

#endif  // ICB_SERIAL_COMMANDER_HPP