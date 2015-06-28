/*
  created 2015-05-28
  by Roman V. Prikhodchenko chujoii@gmail.com
  license GPL
*/

#include <microinterface.h>

#include <encoder.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

const int i2c_lcd_addr = 0x20;
LiquidCrystal_I2C lcd(i2c_lcd_addr,16,2);  // set the LCD address to 0x20 for a 16 chars and 2 line display


encoder enc(0);
microinterface mi(0,     0,     3,           6,                    10,                    16);
//                lcd_x, lcd_y, menu_length, menu_max_item_length, menu_max_value_length, display_width);

const int enc_a = 2;
const int enc_b = 3;
const int btn_c = 4;
const int btn_d = 5;

const char* menu_items[]={"one", "two", "three"};
word        menu_value[]={1,      2,     3};

char buf[9]; // menu_max_value_length

boolean changes = false;



void eintrptwrapper()
{
	byte a;
        byte b;
	byte c;
	byte d;
	
        byte ec_port = PIND;

        if ((ec_port & B00001000) != 0) {a = 1;} else {a = 0;} // a = digitalRead (enc_a);
        if ((ec_port & B00000100) != 0) {b = 1;} else {b = 0;} // b = digitalRead (enc_b);
        if ((ec_port & B00010000) != 0) {c = 1;} else {c = 0;} // c = digitalRead (btn_c);
        if ((ec_port & B00100000) != 0) {d = 1;} else {d = 0;} // d = digitalRead (btn_d);

	char angle;
	angle = enc.encoderhalf(a, b);

	menu_value[mi.get_item()] += mi.input_data(angle, c, d);
	
	enc.set_angle(0);
	changes = true;
}



void setup ()
{
	Wire.begin();
	lcd.init();                      // initialize the lcd 
	
	// Print a message to the LCD.
	lcd.backlight();
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("hello");

        
        delay(1000);


	pinMode(enc_a, INPUT);
	digitalWrite(enc_a, HIGH);

	pinMode(enc_b, INPUT);
	digitalWrite(enc_b, HIGH);

	pinMode(btn_c, INPUT);
	digitalWrite(btn_c, HIGH);

	pinMode(btn_d, INPUT);
	digitalWrite(btn_d, HIGH);

	attachInterrupt(0, eintrptwrapper, CHANGE);
	attachInterrupt(1, eintrptwrapper, CHANGE);
}



void loop ()
{
	if (changes) {
		changes = false;
		
		lcd.clear();
		// set the cursor to column 0, line 1
		// (note: line 1 is the second row, since counting begins with 0):
		//lcd.setCursor(0, 1);
		
		//lcd.print(enc.get_angle()>>2);
		
		lcd.noCursor();
		
		
		lcd.setCursor(0, 0);
		lcd.print(menu_items[mi.get_item()]);

		lcd.setCursor(mi.get_cursor(menu_value[mi.get_item()]), 0);
		
		itoa(menu_value[mi.get_item()], buf, 10); // fixme need use sprintf ?
		int offset = mi.strLength(buf);
		lcd.print(buf);


		lcd.setCursor(mi.get_cursor(menu_value[mi.get_item()]), 1);

		// Turn off the cursor:
		
		//delay(200);
		// Turn on the cursor:
		lcd.cursor();
	}
	delay(100);
	
	
}



