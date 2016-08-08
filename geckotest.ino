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
void cellpp();
void cell();
void disable();
void changeHz();
void manualXY();
void keypadEject();



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
              eject2(value);
              break;
            case 'y':
              xy(450+value*900, 30300-value2*1800);
              break;
            
              
          } // end case

          
          
  } // end controls

        
          
        pad.inputOpCode(); 
        
        lcd.setCursor(0,0);
        switch(pad.getOp())
        {
          case 0:// calibrate
                calibrate();
                break;
          case 1:// disable motors
                disable();
                break;
          case 2: // change step hz
                changeHz;
                break;
          case 3: // xy
                manualXY();
                break;
          case 33:
            manualStep();
            break;
          case 4: //eject
                keypadEject();
                break;
          case 5: // move to cell
                cell();
                break;
          case 55:
                cellpp();
                break;    
          case 92:
                lcd.print("Manual (X,Y)");
                lcd.setCursor(0,1);
                lcd.blink();
                pad.getInput(1);
                lcd.noBlink();
                lcd.clear();
                lcd.home();
                
                if (pad.getY() == 999)
                  break;
            
                m[0].setpos(pad.getX());
                m[1].setpos(pad.getY());
                lcd.noBlink();
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

void eject2(int value) // fully ejects plate
{
  switch (value)
  {
    case 1: // eject
      xy(29000, 36000);
      break;
    case 2: // accept
      xy(23000, 36000);
      break;
    case 3: // return
      xy(16000, 36000);
      hz = 2000;
      xy(16000, 30000);
      break;
  }
}



void disable()
{

  lcd.print("Disable Mode");
  lcd.setCursor(0,1);
  lcd.blink();
  pad.getInput(2);
  lcd.noBlink();

  lcd.clear();
  lcd.home();

  if (pad.getY() == 999)
    return;
  
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
}

void changeHz()
{

  lcd.print("Change Hz");
  lcd.setCursor(0,1);
  lcd.blink();
  pad.getInput(2);
  lcd.noBlink();
  lcd.clear();
  lcd.home();
  
  if (pad.getY() == 999)
    return;
    
  hz = pad.getY();
  lcd.print("Hz changed to ");
  lcd.setCursor(0,1);
  lcd.print(hz);
  delay(DELAY);
}

void manualXY()
{
  lcd.print("Enter XY");
  lcd.setCursor(0,1);
  lcd.blink();
  pad.getInput(1);
  lcd.noBlink();
  lcd.clear();
  lcd.home();

  
  if (pad.getX() == 999 || pad.getY())
    return;
    
  lcd.setCursor(0,1);
  lcd.print(pad.getX());
  lcd.print(", ");
  lcd.print(pad.getY());
  lcd.setCursor(0,0);
  lcd.print("Moving to...");
  lcd.blink();
  xy(pad.getX(), pad.getY());
  lcd.noBlink();
}

void manualStep()
{
  int token = 0;
  int stepCnt = 1;
  do
  {
    lcd.clear();
    lcd.print("Step++ mode");
    lcd.setCursor(0,1);
    lcd.print(m[0].getpos());
    lcd.print(", ");
    lcd.print(m[1].getpos());
    lcd.print("  ");
    lcd.print((int)(log(stepCnt)/log(2)));
    lcd.blink();
    token = pad.gettoken();
    lcd.noBlink();
    switch(token)
    {
      case 1:
      if (stepCnt > 1)
        stepCnt/=2;
        break;
      case 3:
      if (stepCnt < 4000)
         stepCnt*=2;
      break;
      case 8:
        xy(m[0].getpos(), m[1].getpos()+stepCnt);
        break;
      case 2:
        if (m[1].getpos() - stepCnt > 0)
          xy(m[0].getpos(), m[1].getpos() - stepCnt);
        break;
      case 6:                  
        xy(m[0].getpos()+stepCnt, m[1].getpos());
        
        break;
      case 4:         
        if (m[0].getpos() - stepCnt > 0)       
          xy(m[0].getpos() - stepCnt, m[1].getpos());
      
        break;
    }
  } while (token != 5);
}

void keypadEject()
{
  lcd.print("Eject Mode");
  lcd.setCursor(0,1);
  lcd.blink();
  pad.getInput(2);
  lcd.noBlink();
  lcd.home();

  
  if (pad.getY() == 999)
    return;;

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
  else if (pad.getY() == 2)
  {
    lcd.print("Ejecting...");
    lcd.blink();
    eject2(1);
    lcd.noBlink();
    lcd.clear();
    lcd.home();
    lcd.print("Ejected");
    delay(DELAY);
  }
  else if (pad.getY() == 3)
  {
    lcd.print("Accepting...");
    lcd.blink();
    eject2(2);
    lcd.noBlink();
    lcd.clear();
    lcd.home();
    lcd.print("Accepted");
    delay(DELAY);
    
  }
  else if (pad.getY() == 4)
  {
    
    lcd.print("Returning...");
    lcd.blink();
    eject2(3);
    lcd.noBlink();
    lcd.clear();
    lcd.home();
    lcd.print("Returned");
    delay(DELAY);
  }
  else
  {
    
  }
    
}

void cell()
{
  lcd.print("Move to Cell");
  lcd.setCursor(0,1);
  lcd.blink();
  pad.getInput(1);
  lcd.noBlink();
  lcd.home();
  
  if (pad.getX() == 999||pad.getY() == 999)
    return;
  m[1].setcell(pad.getX());
  m[0].setcell(pad.getY());
  lcd.clear();
  lcd.print("Moving to ");
  lcd.print((char)(pad.getY()+64));
  lcd.print(", ");
  lcd.print(pad.getX());
  lcd.setCursor(0,1);
  lcd.blink();
  
  xy(750+900*(pad.getY()-1), 30300-1800*(pad.getX()-1));
  
  lcd.noBlink();
  lcd.clear();
  lcd.home();
  lcd.print("Done! :)");
  delay(DELAY);
}

void cellpp()
{
  int token = 0;
  do
  {
    lcd.clear();
    lcd.print("Cell++ mode");
    lcd.setCursor(0,1);
    lcd.print((char)(m[1].getcell()+64));
    lcd.print(", ");
    lcd.print(m[0].getcell());
    lcd.blink();
    token = pad.gettoken();
    lcd.noBlink();
    switch(token)
    {
      case 8:
        if (m[1].getcell() + 1 < 17)
        {
          xy(m[0].getpos(), m[1].getpos() - 1800);
          m[1].cellpp();
        }
        break;
      case 2:
        if (m[1].getcell() - 1 > 0)
        {
          xy(m[0].getpos(), m[1].getpos() + 1800);
          m[1].cellmm();
        }
        break;
      case 4:     
        if (m[0].getcell() + 1 < 25)
        {             
          xy(m[0].getpos()-900, m[1].getpos());
          m[0].cellpp();
        }
        break;
      case 6:     
        if (m[0].getcell() - 1 > 0)
        {             
          xy(m[0].getpos() + 900, m[1].getpos());
          m[0].cellmm();
        }
        break;
    }
    
  } while (token != 5);
}




