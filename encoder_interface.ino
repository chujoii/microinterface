/*
  created 2013.01.26
  by Roman V. Prikhodchenko chujoii@gmail.com
  license GPL
*/



// strange but lcd.print(12);	not work - after upload only led blinking 
//             lcd.print(String(12)); not work - blinking
// wokr lcd.print("12"); work
//
//	char buf[12]; // "-2147483648\0"
//	itoa(7, buf, 10);
//	lcd.print(buf);
 

#include <encoder.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27,16,2);  // set the LCD address to 0x20 for a 16 chars and 2 line display


encoder enc(2, 3, 0);


int btn = 4;

const char* menu_items[]={"one", "two", "three"};
int menu_length = 3;
int menu_max_item_length = 6;
int menu_max_value_length = 10;
int menu_pointer = 0;
int menu_value[]={1, 2, 3};

char buf[9]; // menu_max_value_length


int menu_x = 0;
int menu_y = 0;
int max_menu_x = 16 - 1; // display width
int max_menu_y = menu_length - 1; //


int strLength(char *buffer)
{
	int i=0;
	while(buffer[i]){
		i++;
	}
	return i;
}


void display_menu(int lcd_x, int lcd_y, int item)
{
	lcd.setCursor(lcd_x, lcd_y);
	lcd.print(menu_items[item]);



	//itoa(enc.get_angle(), buf, 10);
	itoa(menu_value[item], buf, 10);//menu_value[item]); // fixme need use sprintf ?
	
	int offset = strLength(buf);
	lcd.setCursor(lcd_x + menu_max_item_length+menu_max_value_length - offset, lcd_y);
	lcd.print(buf);
}

int sign(int i)
{
	if (i<0) {
		return -1;
	} else {
		return 1;
	}
}



int loopconstrain(int x, int a, int b)
{
	if (x < a) {
		return b;
	}
	if (x > b) {
		return a;
	}
	return x;
}





void eintrptwrapper()
{
	int a;
	a = enc.encoderhalf();

	if ((a>=4) || (a<=-4)) {
		if (LOW == digitalRead(btn)) {
			// x-axis move
			menu_x +=  sign(a);// % max_menu_x;
			menu_x = loopconstrain(menu_x, 0, max_menu_x);
		} else {
			// y-axis move
			if (menu_x > menu_max_item_length - 1){
				// change value
				menu_value[menu_pointer] += sign(a);
			} else {
				// change item
				menu_pointer += sign(a);
				menu_pointer = loopconstrain(menu_pointer, 0, menu_length - 1);
			}
		}
		enc.set_angle(0);
	}
}

void setup ()
{

	lcd.init();                      // initialize the lcd 
	
	// Print a message to the LCD.
	lcd.backlight();
	//lcd.print("Hello, world!");
	
	pinMode(btn, INPUT);
	digitalWrite(btn, HIGH);

	attachInterrupt(0, eintrptwrapper, CHANGE);
	attachInterrupt(1, eintrptwrapper, CHANGE);
}



void loop ()
{
	lcd.clear();
        // set the cursor to column 0, line 1
        // (note: line 1 is the second row, since counting begins with 0):
	//lcd.setCursor(0, 1);
	
	//lcd.print(enc.get_angle()>>2);
	
	lcd.noCursor();



	display_menu(0, 1, menu_pointer);



	lcd.setCursor(menu_x, 1);
	// Turn off the cursor:
	
	//delay(200);
	// Turn on the cursor:
	lcd.cursor();
	delay(100);
	
	
}



