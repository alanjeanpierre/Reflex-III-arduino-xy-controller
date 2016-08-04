#include "keypad.h"
#include "geckodrive.h"
#include <LiquidCrystal.h>

#define UP 1
#define RIGHT 1
#define LEFT 0
#define DOWN 0

#define DELAY 2000
#define MAX 40000


void calibrate();
void xy(long, long);
void eject();
void eject2();



geckoStep m[2] = // declares 2 objects to control the motors
{
  geckoStep(49, 51, 53),
  geckoStep(48, 50, 52)// motor test
};

parallelKeypad pad(2); // object to read keypad data



int hz = 2000;       // steps per second
char control; // for serial control
long value; // serial control
long value2; // serial control

int i = 0;

char ser[500];

extern LiquidCrystal lcd(24, 26, 28, 30, 32, 34);




void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial2.begin(9600,SERIAL_7O1);
  Serial3.begin(9600,SERIAL_7O1);
  lcd.begin(16, 2);
  m[0].genable(); // turn on x motor
  m[1].genable(); // turn on y motor

  pinMode(22, INPUT);

  
  //calibrate();
}

void loop() {
  // put your main code here, to run repeatedly:
        // send data only when you receive data:
        if (Serial.available() > 0) {
                // read the incoming byte:
                control = Serial.read();
                value = Serial.parseInt();
                value2 = Serial.parseInt();

                // say what you got:
                Serial.print("I received: ");
                Serial.println(control);
                Serial.println(value);
                Serial.println(value2);
        
          switch(control)
          {
            case 'D':
              if (value)
                m[i].genable();
              else
                m[i].gdisable();
              break;
            case 'd':
              m[i].gdir(value);
              break;
            case 's':
              for (int i = 0; i < value; i++)
                m[i].gstep(hz);
              break;
            case 'r':
              hz = value;
              break;
            case 'c':
              i = value;
              break;
            case 'C':
              calibrate();
              break;
            case 'x':
              xy(long (value), long (value2));
              break;
            case 'e':
              eject();
              break;
            case 'E':
              eject2();
              break;
            case 'y':
              xy(450+value*900, 30300-value2*1800);
              break;
            
              
          } // end case

          
          
  } // end controls

      if (digitalRead(22) == 0)
      {
      }
      else
      {
        
        for (int i = 0; i < 500; i++)
          Serial.println(ser[i]);
          
        pad.inputOpCode(); 
        
        lcd.setCursor(0,0);
        switch(pad.getOp())
        {
          case 0:// calibrate
                calibrate();
                break;
          case 1:// disable motors
                lcd.print("Disable Mode");
                lcd.setCursor(0,1);
                lcd.blink();
                pad.getInput(2);
                lcd.noBlink();
      
                lcd.clear();
                lcd.home();
  
                if (pad.getY() == 999)
                  break;
                
                if (pad.getY() == 0)
                  {
                    m[0].gdisable();
                    m[1].gdisable();
                    lcd.print("Motors disabled");
                    delay(DELAY);
                    
                  }
                else
                {
                  m[0].genable();
                  m[1].genable();
                  
                  lcd.print("Motors Enabled");
                  delay(DELAY);
                }
                break;
          case 2: // change step hz
                lcd.print("Change Hz");
                lcd.setCursor(0,1);
                lcd.blink();
                pad.getInput(2);
                lcd.noBlink();
                lcd.clear();
                lcd.home();
                
                if (pad.getY() == 999)
                  break;
                  
                hz = pad.getY();
                lcd.print("Hz changed to ");
                lcd.setCursor(0,1);
                lcd.print(hz);
                delay(DELAY);
                break;
          case 3: // xy
                lcd.print("Enter XY");
                lcd.setCursor(0,1);
                lcd.blink();
                pad.getInput(1);
                lcd.noBlink();
                lcd.clear();
                lcd.home();
  
                
                if (pad.getX() == 999)
                  break;
                  
                lcd.setCursor(0,1);
                lcd.print(pad.getX());
                lcd.print(", ");
                lcd.print(pad.getY());
                lcd.setCursor(0,0);
                lcd.print("Moving to...");
                lcd.blink();
                xy(pad.getX(), pad.getY());
                lcd.noBlink();
                break;
          case 33:
          {
              int token = 0;
              int stepCnt = 1;
              do
              {
                lcd.clear();
                lcd.print("Step++ mode");
                lcd.setCursor(0,1);
                lcd.print((char)((m[0].getpos())/900+62));
                lcd.print(", ");
                lcd.print((m[0].getpos()-30300)/1800);
                lcd.print("  ");
                lcd.print(stepCnt);
                lcd.blink();
                token = pad.gettoken();
                lcd.noBlink();
                switch(token)
                {
                  case 7:
                    if (stepCnt > 1)
                      stepCnt--;
                    break;
                  case 9:
                    stepCnt++;
                    break;
                  case 8:
                    xy(m[0].getpos(), m[1].getpos()+stepCnt);
                    break;
                  case 2:
                    xy(m[0].getpos(), m[1].getpos() - stepCnt);
                    break;
                  case 4:                  
                    xy(m[0].getpos()+stepCnt, m[1].getpos());
                    m[0].cellpp();
                    break;
                  case 6:                  
                    xy(m[0].getpos() - stepCnt, m[1].getpos());
                    m[0].cellmm();
                    break;
                }
              } while (pad.getY() != 5);
              break;
            }
          case 4: //eject
                lcd.print("Eject Mode");
                lcd.setCursor(0,1);
                lcd.blink();
                pad.getInput(2);
                lcd.noBlink();
                lcd.home();
  
                
                if (pad.getY() == 999)
                  break;
  
                if (pad.getY() == 1)
                {
                  lcd.clear();
                  lcd.print("Moving to Eject");
                  lcd.setCursor(0,1);
                  lcd.print("Position...");
                  lcd.blink();
                  eject();
  
                  lcd.noBlink();
                  lcd.clear();
                  lcd.home();
                  lcd.print("Ready to Eject");
                  delay(DELAY);
                }
                else if (pad.getY() == 3)
                {
                  lcd.print("Ejecting...");
                  lcd.blink();
                  eject2();
                  lcd.noBlink();
                  lcd.clear();
                  lcd.home();
                  lcd.print("Ejected");
                  delay(DELAY);
                }
                break;
          case 5: // move to cell
                
                lcd.print("Move to Cell");
                lcd.setCursor(0,1);
                lcd.blink();
                pad.getInput(1);
                lcd.noBlink();
                lcd.home();
                
                if (pad.getX() == 999)
                  break;
                m[0].setcell(pad.getX());
                m[1].setcell(pad.getX());
                lcd.clear();
                lcd.print("Moving to ");
                lcd.print((char)(pad.getX()+62));
                lcd.print(", ");
                lcd.print(pad.getY());
                lcd.setCursor(0,1);
                lcd.blink();
                
                xy(750+900*(pad.getX()-1), 30300-1800*(pad.getY()-1));
                
                lcd.noBlink();
                lcd.clear();
                lcd.home();
                lcd.print("Done! :)");
                delay(DELAY);
                break;
          case 55:
          {
              int token = 0;
              do
              {
                lcd.clear();
                lcd.print("Cell++ mode");
                lcd.setCursor(0,1);
                lcd.print((char)((m[0].getpos()-750)/900+62));
                lcd.print(", ");
                lcd.print((m[0].getpos()-30300)/1800);
                lcd.blink();
                token = pad.gettoken();
                lcd.noBlink();
                switch(token)
                {
                  case 8:
                    xy(m[0].getpos(), m[1].getpos() + 1800);
                    m[1].cellpp();
                    break;
                  case 2:
                    xy(m[0].getpos(), m[1].getpos() - 1800);
                    m[1].cellmm();
                    break;
                  case 4:                  
                    xy(m[0].getpos()+900, m[1].getpos());
                    m[0].cellpp();
                    break;
                  case 6:                  
                    xy(m[0].getpos() - 900, m[1].getpos());
                    m[0].cellmm();
                    break;
                }
              } while (token != 5);
              break;
          }
            
          case 90: // set direction
                
                lcd.print("Manual Direction");
                lcd.setCursor(0,1);
                lcd.blink();
                pad.getInput(1);
                lcd.noBlink();
                lcd.clear();
                lcd.home();
  
                
                if (pad.getX() == 999)
                  break;
  
                  
                switch(pad.getX())
                {
                  case 1:
                    if (pad.getY() == 1)
                    {
                      lcd.print("X Motor->Left");
                      m[0].gdir(0);
                      delay(DELAY);
                    }
                    else if (pad.getY() == 3)
                    {
                      lcd.print("X Motor->Right");
                      m[0].gdir(1);
                      delay(DELAY);
                    }
                    else
                    ;
                    break;
                  case 2:
                    if (pad.getY() == 1)
                    {
                      lcd.print("Y Motor->Down");
                      m[1].gdir(0);
                      delay(DELAY);
                    }
                    else if (pad.getY() == 3)
                    {
                      lcd.print("Y Motor->Up");
                      m[1].gdir(1);
                      delay(DELAY);
                    }
                    else
                    ;
                 
                }
            break;
          case 91: // manual step
          
                lcd.print("Manual Stepping");
                lcd.setCursor(0,1);
                lcd.blink();
                pad.getInput(1);
                lcd.noBlink();
                lcd.clear();
                lcd.home();
                
                if (pad.getY() == 999)
                  break;
  
                  
                  lcd.print(pad.getY());
                  lcd.print(" steps...");
                  lcd.blink();
                if (pad.getX() == 1)
                {
                  for (int i = 0; i < pad.getY(); i++)
                  {
                    m[0].gstep(hz);
                  }
                }
                else if (pad.getX() == 2)
                  for (int i = 0; i < pad.getY(); i++)
                  {
                    m[1].gstep(hz);
                  }
                else
                  ;
  
                  
                  lcd.noBlink();
                  lcd.setCursor(0,1);
                  lcd.print("Done");
                  delay(DELAY);
                break;
          
        }
      }
  

}


void calibrate() // resets plate to 0, upper right. Changes Y first
{
  
  lcd.print("Calibrating...");
  lcd.setCursor(0,1);
  lcd.blink();

  int k = 0;
  
  m[1].gdir(UP);
  for (long i = 0; i < 40000; i++)
  {
    if (i%718 == 0) // %counter
    {
      
      lcd.setCursor(0,1);
      lcd.print(k);
      lcd.write('%');
      k++;
    }
    
    m[1].gstep(hz);
  }
  m[1].sethome();
  
  m[0].gdir(RIGHT);
  for (long i = 0; i< 31800; i++)
  {
    if ((i+510)%718 == 0) // %counter
    {
      
      lcd.setCursor(0,1);
      lcd.print(k);
      lcd.write('%');
      k++;
    }
    m[0].gstep(hz);
  }
  m[0].sethome();
  
  lcd.noBlink();
  lcd.setCursor(0,1);
  lcd.print("Done!");
  delay (DELAY);
}


void xy(long x, long y) // moves plate to given XY position. x y = steps from 0
{

  if (x > MAX || y > MAX)
    return;

    
  long curx=m[0].getpos(), cury=m[1].getpos();
  
  long toX, toY;

  if (x-curx > 0)
  {
    m[0].gdir(LEFT);
    toX = x-curx;
  }
  else
  {
    m[0].gdir(RIGHT);
    toX = curx-x;
  }
  
  if (y-cury > 0)
  {
    m[1].gdir(DOWN);
    toY = y-cury;
  }
  else
  {
    m[1].gdir(UP);
    toY = cury - y;
  }
  
  for (long i = 0; i < toX; i++)
    m[0].gstep(hz);
  for (long i = 0; i < toY; i++)
    m[1].gstep(hz);
  
  
}

void eject() // sets plate over hooking mechanism and partially ejects
{
  if (m[1].getpos() > 29000)
  {
    xy(m[0].getpos(), 25000);
  }

  xy(16000, 36000);
  hz = 500;
  xy(23000, 36000);
  
}

void eject2() // fully ejects plate
{
  xy(29000, 36000);
}



