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
    geckoStep(int disablepin, int directionpin, int steppin); // pins for disable, dir, and step
    void gstep(long hz);         // turns 1 step at some hz
    void genable();                // enable the motor
    void gdisable();               // disable the motor
    void gdir(int direc);            // change direction
    void sethome();           // sets pos = 0
    long getpos();            // returns pos
    void setcell(int);
    int getcell();
    void cellpp();
    void cellmm();
    void setpos(long);
	
  private:
    int disablePin;
    int dirPin;
    int stepPin;
    int dir;
    int cell;
    long pos;

};

#endif
