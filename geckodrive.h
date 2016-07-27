/*
Arduino controls for geckodrive G213V
Alan Jeanpierre
20 6 2016

*/


#ifndef geckodrive_h
#define geckodrive_h

#include "Arduino.h"

class geckoStep
{
  public:
    geckoStep(int, int, int); // pins for disable, dir, and step
    void gstep(long);         // turns 1 step at some hz
    void genable();                // enable the motor
    void gdisable();               // disable the motor
    void gdir(int);            // change direction
    void sethome();           // sets pos = 0
    long getpos();            // returns pos
	
  private:
    int disablePin;
    int dirPin;
    int stepPin;
    int dir;
    long pos;

};

#endif
