/*
  created 2013.01.26
  by Roman V. Prikhodchenko chujoii@gmail.com
  license GPL
*/


#ifndef microinterface_h
#define microinterface_h

#include "Arduino.h"

class microinterface
{
public:
        microinterface(int lcd_x, int lcd_y, int menu_length, int menu_max_item_length, int menu_max_value_length, int display_width, int detent);
	int get_item();
	int get_cursor();
	int get_position_for_value(int menu_value);
	int input_data(char angle, byte btn_0, byte btn_1);
	int strLength(char *buffer);
private:
	int _menu_pointer;  // _menu_y
	int _menu_x;
	int _lcd_x;
	int _lcd_y;
	int _menu_length;
	int _menu_max_item_length;
	int _menu_max_value_length;
	int _display_width;
	int _detent;
	char _buf[9]; // menu_max_value_length
	int loopconstrain(int x, int a, int b);
	char sign(char i);
	word pow10(int a); // return 10^a
};

#endif
