#include <PinChangeInterrupt.h>
#include <PinChangeInterruptBoards.h>
#include <PinChangeInterruptPins.h>
#include <PinChangeInterruptSettings.h>

#include <LiquidCrystal_I2C.h>
#define BTN 4

LiquidCrystal_I2C lcd(0x27, 16, 2);

unsigned long current_time;
unsigned long BTN_pressed = 0;
unsigned long BTN_pressed_time = 0;
unsigned long remember_time3;
unsigned long remember_time4;
int pace = 100;
int obstacle_x_random;
int period = 50;
const int minimal_distance = 5;
int i;
int score = 0;
float j = 16;
int cursor_obstacle_1;
int cursor_obstacle_2;
int cursor_obstacle_3;
int cursor_obstacle_4;
int x = 2;
int jump_y;
bool BTN_S = true;
bool start_game = false;
//ground//
byte customChar0[8] =
{
	0b00000,
	0b00000,
	0b00000,
	0b00000,
	0b00000,
	0b00000,
	0b00000,
	0b11111
};
//player//
//byte customChar1[8] = {
//	0b00000,
//	0b00000,
//	0b00000,
//	0b00000,
//	0b00011,
//	0b01110,
//	0b01010,
//	0b10001
//};
byte customChar1[8] = {
  0b00000,
  0b00000,
  0b00000,
  0b10111,
  0b10100,
  0b11111,
  0b00101,
  0b11101
};

byte customChar2[8] = {
	0b00000,
	0b10101,
	0b11111,
	0b01110,
	0b00100,
	0b00101,
	0b00110,
	0b00100
};

void setup()
{
	pinMode(BTN, INPUT_PULLUP);
	lcd.init();
	lcd.backlight();
	Serial.begin(9600);
	lcd.createChar(0, customChar0);
	lcd.createChar(1, customChar1);
  lcd.createChar(2, customChar2);

	attachPCINT(digitalPinToPCINT(BTN), BTN_FN1, CHANGE);

}

void screen_Hz()
{
	if (current_time - remember_time3 >= period)
	{
		remember_time3 = current_time;
		lcd.clear();
    lcd.setCursor(2,jump_y);
    lcd.write((byte)1); 
    score_lcd ();
    if (cursor_obstacle_1 <=16)
    {
      lcd.setCursor(cursor_obstacle_1,1);
      lcd.write((byte)2);
    }
    if (cursor_obstacle_2 <=16)
    {
      lcd.setCursor(cursor_obstacle_2,1);
      lcd.write((byte)2);
    }
    if (cursor_obstacle_3 <=16)
    {
      lcd.setCursor(cursor_obstacle_3,1);
      lcd.write((byte)2);
    }
    if (cursor_obstacle_4 <=16)
    {
      lcd.setCursor(cursor_obstacle_4,1);
      lcd.write((byte)2);
    }
    
  }
	
}

void BTN_FN1()
{
	BTN_S = !BTN_S;
  
  if (start_game == false)
  {
    start_game = true;
  }
}


void jump()//bool BTN_S)
{
  bool timer_running;

	if (BTN_S == true)
  {
    BTN_pressed = millis();
    timer_running = true;
    BTN_S = false;
  }
  if (timer_running == true)
  {
    BTN_pressed_time = millis() - BTN_pressed ;
  }
  if (BTN_pressed_time > 0 & BTN_pressed_time <1000)
  {
    jump_y = 0;
  }
  else
  {
    jump_y = 1;
  }
  
}



void colision()
{
  int obstacle_colision [] = {cursor_obstacle_1, cursor_obstacle_2, cursor_obstacle_3, cursor_obstacle_4};
  for(i = 0; i < 4; i++)
    {
      if (obstacle_colision[i] == 2)
      {
        if (jump_y == 1)
          {
            start_game = false;
            lcd.clear();
            score = 0;
            return;
          }
        if ( jump_y == 0)
          {
            score +10;
          }
      }

    }
}

void title_screen ()
{
  if (start_game == false)
  {
    lcd.setCursor(5,0);
    lcd.print("START");
  }

}

class obstacle
{
public:
    double position_x = 16 + (rand() % 15) + 5 ;

    double random_position()
    {
        position_x = 16+ rand() % (rand() % 15) + 5;
        return position_x;
    }
    double position_changing()
    {
        if (position_x > -2)
        {
            position_x--;
        }
        if (position_x == -2)
        {
            random_position();
        }
        return position_x;
    }
    
};

obstacle obstacle1;
obstacle obstacle2;
obstacle obstacle3;
obstacle obstacle4;

void score_lcd ()
{
  if (score < 100)
    {
      lcd.setCursor(14,0);
      lcd.print(score);
    }
  else 
    {
      lcd.setCursor(13,0);
      lcd.print(score);
    }
}

void loop()
{
	current_time = millis();
  title_screen();
  if(start_game == true)
  {
    jump();//BTN_S);
     if(current_time - remember_time4 >= pace)
      {
        remember_time4 = current_time;
      cursor_obstacle_1 = obstacle1.position_changing();
      cursor_obstacle_2 = obstacle2.position_changing();
      cursor_obstacle_3 = obstacle3.position_changing();
      cursor_obstacle_4 = obstacle4.position_changing(); 
      }
    screen_Hz();
    colision();
    Serial.println(BTN_pressed_time);
  }
  else 
    {
      return 0;
    }
  }


