#ifndef LCD3Wire_h
#define LCD3Wire_h

#include <inttypes.h>
#include "Print.h"
class  LCD3Wire : public Print {
public:
  LCD3Wire(int lcd_lines, int dat_pin, int str_pin, int clk_pin);
  void commandWrite(uint8_t value);
  void init();
 virtual size_t write(uint8_t value);
  void writeIn(char value[]);
  //non-core---------------
  void clear();
  void cursorTo(uint8_t line_num, uint8_t x);
  void leftScroll(uint8_t chars,uint8_t delay_time);
  //end of non-core--------

private:
  void _pushByte(uint8_t value, bool command);
  void _pushNibble(uint8_t nibble, bool command);
  void _pushOut(uint8_t value);
};

#endif
