
#ifndef keypad_h
#define keypad_h

#include "Arduino.h"
#include <LiquidCrystal.h>

//#include <string>

class parallelKeypad
{
	public:
		parallelKeypad(int start); // initializes pins
		void getInput(int);          // gets x and y variables
    void inputOpCode();       // gets op code
		char decodeInput(int);    // changes from pin -> character
    void readInput(int);      // actually retrieves input
    long parseInput(char[]);   // parses input
    long getX();               // returns x
    long getY();               // returns y
    int getOp();
		
	private:
		int opCode;
		long x;
		long y;
    int first;
	
	
	
};


#endif
