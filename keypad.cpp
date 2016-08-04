#include "keypad.h"
#include "Arduino.h"
#include <math.h>

extern LiquidCrystal lcd;

parallelKeypad::parallelKeypad(int start)
{
	for (int i = start; i < start+12; i++)
		pinMode(i, INPUT_PULLUP);
		

  first = start;
	opCode = 0;
	x = 0;
	y = 0;
}

void parallelKeypad::inputOpCode()
{ 
  opCode = 0;
  x = 0;
  y = 0;
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Input Op Code");
  lcd.setCursor(0,1);
  lcd.blink();
  readInput(0);

  
  lcd.noBlink();
  lcd.clear();
}


void parallelKeypad::getInput(int i)
{
  
  for (i; i < 3; i++)
    readInput(i);


  
}

void parallelKeypad::readInput(int k)
{
  
  int olddata;
  char data[7] ={'0'};
  int j = 0;

  int z;
  

  
  
  do
  {
    olddata = -1;
    for (int i = 0; i < 12; i++)
    {
      z = i+first;
      int newdata = digitalRead(z);

      if (newdata == 0 && olddata != i)
      {
        data[j] = decodeInput(i);
        while (digitalRead(z) == 0)
        {
          delay(1);
        }
          
        lcd.setCursor(pow(7, k/2)-1+j,1);
        lcd.write(data[j]);
        j++;
        break;
      }

        
      olddata = i;
    }

  } while (data[j-1] != '#' || j == 6);

    
  switch(k)
  {
    case 0:
      opCode = parseInput(data);

      break;
    case 1:
      x = parseInput(data);
      break;
    case 2:
      y = parseInput(data);
      break;
  }
}

char parallelKeypad::decodeInput(int i)
{
  switch (i)
  {
    case 0:
      return '*';
      break;
    case 1:
      return '7';
      break;
    case 2:
      return '4';
      break;
    case 3:
      return '1';
      break;
    case 4:
      return '0';
      break;
    case 5:
      return '8';
      break;
    case 6:
      return '5';
      break;
    case 7:
      return '2';
      break;
    case 8:
      return '#';
      break;
    case 9:
      return '9';
      break;
    case 10:
      return '6';
      break;
    case 11:
      return '3';
      break;
  }
}

long parallelKeypad::parseInput(char input[])
{


  return atol(input);
}


long parallelKeypad::getX()
{
  return x;
}

long parallelKeypad::getY()
{
  return y;
}

int parallelKeypad::getOp()
{
  return opCode;
}
  
int parallelKeypad::gettoken()
{
  int z;
  char data;
  do
  {
    for (int i = 0; i < 12; i++)
    {
      z = i+first;
      int newdata = digitalRead(z);

      if (newdata == 0)
      {
        data = decodeInput(i);
        while (digitalRead(z) == 0)
        {
          delay(1);
        }

        return data-'0';
      }

        
    }

  } while (1);
}

