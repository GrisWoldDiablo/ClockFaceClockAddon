

/*
Simple Clock 12h or 24h
Version : 1.0,  April 15, 2017
Author: GrisWold Diablo
Twitter: @GrisWoldDiablo

SCREEN:
Press LEFT and RIGHT to turn screen OFF, clock still run in the background.
Press UP and DOWN to turn screen ON.

12H or 24H:
Press LEFT and DOWN to set clock type to 12h AM/PM (Default)
Press RIGHT and UP to set clock type to 24h

ADJUST TIME:
Hours: hold A, press UP or DOWN
Minutes: hold B, press UP or DOWN
Seconds: hold A and B, press UP or DOWN

PAUSE CLOCK:
Pause clock hold A and B, Only works with screen ON

NOTES:
Feel free to use this code, no need to give me credit.
*/

#include <Arduboy2.h>

Arduboy2 arduboy;

int frameRate = 15;	// Frame rate, default 15 to save battery
int sc = frameRate; // Frame counter used for Pause and AM/PM swap

long milSec = 1000;	// Secound counter, 1 second = 1000 milliseconds
long fullSec = 1000;// Incrematation of second counter

String clockText;	// Text variable for Clock
String clockS = ":";// Seperator between clock digit
int h = 12, m = 0, s = 0;	// h:Hours, m:Minutes, s:Seconds
String secD = "0";	// Make seconds diplay as 01 instead of 1
String minD = "0";	// Make minutes diplay as 01 instead of 1
String hourD = " ";	// Add space in front of hour to center display

bool clockType = true;	// 12 or 24 hour clock type, TRUE = 12h AM/PM, FALSE = 24h 
int clockTypeVar;	// Either 12 or 24, variable for clock type calculation
String clockTypeText; // Display 12h or 24h depending of clock type
bool ampm = true;	// Used if clock type set to 12h, TRUE = AM, FALSE = PM
String ampmText = " AM";	// Show AM or PM to clock display

bool diplayOnOff = true;	// Display On or Off, TRUE = ON, FALSE = OFF

int locX = 32, locY = 32;	// Clock location on display





void setup()
{
	arduboy.boot();
	arduboy.setFrameRate(frameRate);	// Time calculated based on set variable frames per second.
	arduboy.clear();
}

void loop()
{
	// Second incrementation, every 1000 millisecond 1 second is added
	if (millis() >= milSec)
	{
		milSec += fullSec;

		// Pause clock hold A and B, Only works with screen ON
		if (arduboy.pressed(A_BUTTON) && arduboy.pressed(B_BUTTON) && diplayOnOff)
		{
			sc = frameRate;
			return;
		}
		s++;
	}

	if (!(arduboy.nextFrame()))
	{
		return;
	}

	arduboy.clear();
	clock();


	arduboy.display();


}

void clock()
{

	// Frame counter used for Pause and AM/PM swap
	if (sc == 0)
	{
		sc = frameRate;
	}
	else
	{
		sc--;
	}

	// Clock incrementation
	// Add 1 min every 60 seconds and reset second to 0
	if (s == 60)
	{
		s = 0;
		m++;
	}
	// Add 1 hour every 60 minutes and reset minute to 0
	if (m == 60 && s == 0)
	{
		m = 0;
		h++;
	}
	// Turn over clock 
	if (h == clockTypeVar && s == 0 && !clockType)
	{
		h = 0;
	}
	else if (h == clockTypeVar && s == 0 && ampm && sc == frameRate)
	{
		ampm = false;
	}
	else if (h == clockTypeVar && s == 0 && !ampm && sc == frameRate)
	{
		ampm = true;
	}

	// Press LEFT and RIGHT to turn screen OFF, clock still run in the background.
	if (arduboy.pressed(LEFT_BUTTON) && arduboy.pressed(RIGHT_BUTTON) && arduboy.notPressed(A_BUTTON) && arduboy.notPressed(B_BUTTON) && arduboy.notPressed(UP_BUTTON) && arduboy.notPressed(DOWN_BUTTON))
	{
		diplayOnOff = false;
		arduboy.clear();
		arduboy.display();
	}
	// Press UP and DOWN to turn screen ON and display clock.
	else if (arduboy.pressed(UP_BUTTON) && arduboy.pressed(DOWN_BUTTON) && arduboy.notPressed(A_BUTTON) && arduboy.notPressed(B_BUTTON) && arduboy.notPressed(LEFT_BUTTON) && arduboy.notPressed(RIGHT_BUTTON))
	{
		diplayOnOff = true;
	}
	// Exit loop function if screen should be OFF
	if (!diplayOnOff)
	{
		return;
	}

	// Set clock to a 12h or 24h type
	if (clockType)
	{
		clockTypeVar = 12;
	}
	else
	{
		clockTypeVar = 24;
	}
	// Press RIGHT and UP to set clock type to 24h
	if (arduboy.pressed(RIGHT_BUTTON) && arduboy.pressed(UP_BUTTON) && clockType&& arduboy.notPressed(A_BUTTON) && arduboy.notPressed(B_BUTTON) && arduboy.notPressed(LEFT_BUTTON) && arduboy.notPressed(DOWN_BUTTON))
	{
		clockType = false;
		if (ampm && h == 12)
		{
			h = 0;
		}
		else if (!ampm && h != 12)
		{
			h += 12;
		}

	}
	// Press LEFT and DOWN to set clock type to 12h AM/PM
	if (arduboy.pressed(LEFT_BUTTON) && arduboy.pressed(DOWN_BUTTON) && !clockType&& arduboy.notPressed(A_BUTTON) && arduboy.notPressed(B_BUTTON) && arduboy.notPressed(RIGHT_BUTTON) && arduboy.notPressed(UP_BUTTON))
	{
		clockType = true;
		// Set AM PM according to current time
		if (h >= 12)
		{
			ampm = false;
			if (h > 12)
			{
				h -= 12;
			}
		}
		else
		{
			ampm = true;
		}

		if (h == 0)
		{
			h = 12;
		}
	}

	// Adjust Hours hold A, press UP or DOWN
	if (arduboy.pressed(A_BUTTON) && arduboy.pressed(UP_BUTTON) && arduboy.notPressed(B_BUTTON))
	{
		HourTurn(true);
	}
	else if (arduboy.pressed(A_BUTTON) && arduboy.pressed(DOWN_BUTTON) && arduboy.notPressed(B_BUTTON))
	{
		HourTurn(false);
	}
	// Adjust Minutes hold B, press UP or DOWN
	else if (arduboy.pressed(B_BUTTON) && arduboy.pressed(UP_BUTTON) && arduboy.notPressed(A_BUTTON))
	{
		MinuteTurn(true);
	}
	else if (arduboy.pressed(B_BUTTON) && arduboy.pressed(DOWN_BUTTON) && arduboy.notPressed(A_BUTTON))
	{
		MinuteTurn(false);
	}
	// Adjust Seconds hold A and B, press UP or DOWN
	else if (arduboy.pressed(A_BUTTON) && arduboy.pressed(B_BUTTON) && arduboy.pressed(UP_BUTTON))
	{
		SecondTurn(true);
	}
	else if (arduboy.pressed(A_BUTTON) && arduboy.pressed(B_BUTTON) && arduboy.pressed(DOWN_BUTTON))
	{
		SecondTurn(false);
	}

	// Select clock type to display
	if (clockType)
	{
		clockTypeText = "12h";
	}
	else
	{
		clockTypeText = "24h";
	}
	
	arduboy.setCursor(0, 0);	// Print clock type top left corner
	arduboy.print(clockTypeText);

	arduboy.setCursor(locX, locY);	// Set location for clock
	clockText = CreateDisplayText();
	arduboy.print(clockText); // Print clock
}

// Swap AM/PM depending of how it start
boolean AmPmSwap() 
{
	if (ampm)
	{
		return false;
	}
	else
	{
		return true;
	}
}

// Increment or decrement hours
void HourTurn(bool changeH)
{
	if (changeH)
	{
		h++;
		if (clockType && h > 12)
		{
			h = 1;
		}
		else if (clockType && h == 12)
		{
			ampm = AmPmSwap();
		}
		else if (!clockType && h == 24)
		{
			h = 0;
		}
	}
	else
	{
		h--;
		if (clockType && h < 1)
		{
			h = 12;
		}
		else if (clockType && h == 11)
		{
			ampm = AmPmSwap();
		}
		else if (!clockType && h < 0)
		{
			h = 23;
		}
	}
}

// Increment or decrement minutes
void MinuteTurn(bool changeM)
{
	if (changeM)
	{
		m++;
		if (m > 59)
		{
			m = 0;
			HourTurn(true);
		}
	}
	else
	{
		m--;
		if (m < 0)
		{
			m = 59;
			HourTurn(false);
		}
	}
}

// Increment or decrement seconds
void SecondTurn(bool changeS)
{
	if (changeS)
	{
		s++;
		if (s > 59)
		{
			s = 0;
			MinuteTurn(true);
		}
	}
	else
	{
		s--;
		if (s < 0)
		{
			s = 59;
			MinuteTurn(false);
		}
	}
}

String CreateDisplayText()
{
	// Seconds diplay as 01 instead of 1
	if (s >= 0 && s <= 9)
	{
		secD = "0";
	}
	else
	{
		secD = "";
	}
	// Minutes diplay as 01 instead of 1
	if (m >= 0 && m <= 9)
	{
		minD = "0";
	}
	else
	{
		minD = "";
	}
	// Hour Space in front for time under 10
	if (h < 10)
	{
		hourD = " ";
	}
	else
	{
		hourD = "";
	}
	// Diplay AM or PM if clock set to 12h type
	if (clockType)
	{
		if (ampm)
		{
			ampmText = " AM";
		}
		else
		{
			ampmText = " PM";
		}
	}
	else
	{
		ampmText = "";
	}

	
	return hourD + h + clockS + minD + m + clockS + secD + s + ampmText;	// Create clock string
}

