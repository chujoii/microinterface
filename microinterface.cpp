/*
  created 2013.01.26
  by Roman V. Prikhodchenko chujoii@gmail.com
  license GPL
*/

#include <microinterface.h>

microinterface::microinterface(int lcd_x, int lcd_y, int menu_length, int menu_max_item_length, int menu_max_value_length, int display_width)
{
	_menu_pointer = 0; // _menu_y
	_menu_x = 0;
	_lcd_x = lcd_x;
	_lcd_y = lcd_y;
	_menu_length = menu_length - 1;
	_menu_max_item_length = menu_max_item_length;
	_menu_max_value_length = menu_max_value_length;
	_display_width = display_width - 1;
}


int microinterface::strLength(char *buffer)
{
	int i=0;
	while(buffer[i]){
		i++;
	}
	return i;
}


int microinterface::get_item()
{
	return _menu_pointer;
}

int microinterface::get_cursor()
{
	return _menu_x;
}

int microinterface::get_position_for_value(int menu_value)
{
	// return offset for x
	itoa(menu_value, _buf, 10); // fixme need use sprintf ?
	
	int offset = strLength(_buf);
	return (_lcd_x + _menu_max_item_length + _menu_max_value_length - offset);
}

char microinterface::sign(char i)
{
	if (i<0) {
		return -1;
	} else {
		return 1;
	}
}


word microinterface::pow10(int a) // return 10^a
{
	if (a>5) {return 10000;} // 65535
	int b = 1;
	for (int c=0; c<a; c++){
		b *= 10;
	}
	return b;
}


int microinterface::loopconstrain(int x, int a, int b)
{
	if (x < a) {
		return b;
	}
	if (x > b) {
		return a;
	}
	return x;
}





int microinterface::input_data(char angle, byte btn_0, byte btn_1)
{
	int addition_value = 0;
	if ((angle>=1) || (angle<=-1)) {
		if (0 == btn_0) {
		        // x-axis move
			_menu_x +=  sign(angle);// % _display_width;
			_menu_x = loopconstrain(_menu_x, 0, _display_width);
		} else {
			// y-axis move
			if (_menu_x > _menu_max_item_length){
				// change value
				addition_value += sign(angle)*(pow10(_display_width - _menu_x));
			} else {
				// change item
				_menu_pointer += sign(angle);
				_menu_pointer = loopconstrain(_menu_pointer, 0, _menu_length);
			}
		}
	}
	return addition_value;
}
