#include "Arduino.h"
#include "geckodrive.h"
#include <math.h>

  

geckoStep::geckoStep(int a, int b, int c)
{
  disablePin = a;
  dirPin = b;
  stepPin = c;

  pinMode(disablePin, OUTPUT);
  pinMode(dirPin, OUTPUT);
  pinMode(stepPin, OUTPUT);
  pos = 0;
  
  
}

void geckoStep::gstep(long hz)
{
      digitalWrite(stepPin, HIGH);
      digitalWrite(stepPin, LOW);
      delayMicroseconds(1000000/hz);
      pos += pow(-1, (2-dir));
}

void geckoStep::genable()
{
  digitalWrite(disablePin, LOW);
}

void geckoStep::gdisable()
{
  digitalWrite(disablePin, HIGH);
}

void geckoStep::gdir(int d)
{
	if (d != dir) 
	{
		if (d == 0)
	  		digitalWrite(dirPin, LOW);
	  	else
	  		digitalWrite(dirPin, HIGH);
	  	
	  	
  		dir = d;
  	}
  	
}

void geckoStep::sethome()
{
  pos = 0;
}

long geckoStep::getpos()
{
  return pos;
}

void geckoStep::setcell(int location)
{
  cell = location;
}

int geckoStep::getcell()
{
  return cell;
}

void geckoStep::cellpp()
{
  cell++;
}

void geckoStep::cellmm()
{
  cell--;
}

