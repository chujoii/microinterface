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
//                            menu_items[3]; 6          +          10             =       16

const int enc_a_pin = 2;
const int enc_b_pin = 3;
const int btn_c_pin = 4;
const int btn_d_pin = 5;

const char* menu_items[]={"one", "two", "three"};
word        menu_value[]={1,      2,     3};

char buf[9]; // menu_max_value_length

volatile byte changes = 1;

unsigned long timer = 0;
int dt = 500;
boolean cursor_status = true;



void eintrptwrapper()
{
	byte a;
        byte b;
	byte c;
	byte d;
	
        byte ec_port = PIND;

	if ((ec_port & B00001000) != 0) {a = 1;} else {a = 0;} // a = digitalRead (enc_a_pin);
        if ((ec_port & B00000100) != 0) {b = 1;} else {b = 0;} // b = digitalRead (enc_b_pin);
        if ((ec_port & B00010000) != 0) {c = 1;} else {c = 0;} // c = digitalRead (btn_c_pin);
        if ((ec_port & B00100000) != 0) {d = 1;} else {d = 0;} // d = digitalRead (btn_d_pin);

	char angle;
	changes = enc.encoderhalf(a, b);
	angle = enc.get_angle();
	int new_val = mi.input_data(angle, c, d);
	menu_value[mi.get_item()] += new_val;
	enc.set_angle(0);
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
	//lcd.blink(); // blink cursor with symbol :(

        
        delay(1000);


	pinMode(enc_a_pin, INPUT);
	digitalWrite(enc_a_pin, HIGH);

	pinMode(enc_b_pin, INPUT);
	digitalWrite(enc_b_pin, HIGH);

	pinMode(btn_c_pin, INPUT);
	digitalWrite(btn_c_pin, HIGH);

	pinMode(btn_d_pin, INPUT);
	digitalWrite(btn_d_pin, HIGH);

	attachInterrupt(0, eintrptwrapper, CHANGE);
	attachInterrupt(1, eintrptwrapper, CHANGE);



	Serial.begin(9600);
}



void loop ()
{
	if (0 == changes) { // return        0 - good: encoder rotated        1 - default, no changes (error)        2 - very strange error
		changes = 1;

		lcd.clear();
		// set the cursor to column 0, line 0
		lcd.setCursor(0, 0);

		lcd.print(menu_items[mi.get_item()]); // print item

		lcd.setCursor(mi.get_position_for_value(menu_value[mi.get_item()]), 0); // print value
		lcd.print(menu_value[mi.get_item()]);

		lcd.setCursor(mi.get_cursor(), 0);
		lcd.cursor();
		cursor_status = !cursor_status;

	}


	
	// see string "lcd.blink()" in function "setup" // blink cursor with symbol :(
 	if (timer < millis()){ // blink without delay. fixme: nee use hw timer
		timer = millis() + dt;
		if (cursor_status){
			// Turn off the cursor:
			lcd.noCursor();
		} else {
			// Turn on the cursor:
			lcd.cursor();
		}
		cursor_status = !cursor_status;
	}
}


