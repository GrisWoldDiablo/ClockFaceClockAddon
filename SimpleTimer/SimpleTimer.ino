

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
#include <ArduboyPlaytune.h>

Arduboy2 arduboy;
ArduboyPlaytune ardtune(arduboy.audio.enabled);

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
bool clockDisplay = true;

bool diplayOnOff = true;	// Display On or Off, TRUE = ON, FALSE = OFF

int locX = 32, locY = 32;	// Clock location on display

int sAv, mAv, hAv; // Variable used to adjust time. Seconds, Minutes, Hours.
int * arrD; // Display Array pointer Seconds[0], Minutes[1], Hours[2]

// Counter used for when a button is held down.
long heldTime = 1500;	// Default 1.5 seconds button held down, count milliseconds
long timeHeld = 0;	// Used for counting
bool startCounting = true;	// Start the counter to change window
bool buttonHeld = false; // Check if button has been held long enough, TRUE = Change, FALSE = Keep counting

// Alarm variables
int hA = 12, mA = 1, sA = 0; // Alarm variables, h:Hours, m:Minutes, s:Seconds
String alarmText;	// Text variable for Alarm
bool alarmSetting = false;
bool ampmA = true;	// Used if clock type set to 12h, TRUE = AM, FALSE = PM
bool alarmOnSetting = false;

bool timerSetting = false;


const byte score[] PROGMEM = {
	2,154, 0x90,62, 0,166, 0x90,64, 0,166, 0x90,66, 0,166, 0x90,64, 0,166,
	0x90,67, 0,166, 0x90,66, 0,166, 0x90,64, 0,166, 0x90,62, 0,166, 0x90,69,
	0,166, 0x90,67, 0,166, 0x90,66, 0,166, 0x90,64, 0,166, 0x91,50, 0x90,66,
	0,166, 0x91,52, 0x90,62, 0,166, 0x91,54, 0x90,69, 0,166, 0x91,52, 0,166,
	0x90,71, 0x91,55, 0,166, 0x91,54, 0,166, 0x90,73, 0x91,52, 0,55, 0x90,71,
	0,55, 0x90,73, 0,55, 0x91,50, 0x90,74, 0,166, 0x91,57, 0x90,73, 0,83,
	0x90,74, 0,83, 0x91,55, 0x90,73, 0,166, 0x91,54, 0x90,71, 0,83, 0x90,69,
	0,83, 0x91,52, 0,125, 0x80, 0,41, 0x90,73, 0x91,54, 0,166, 0x90,74, 0x91,50,
	0,166, 0x90,76, 0x91,57, 0,166, 0x90,73, 0,166, 0x91,45, 0x90,78, 0,166,
	0x90,74, 0,166, 0x91,57, 0x90,73, 0,166, 0x90,71, 0,138, 0x81, 0,27, 0x91,57,
	0x90,76, 0,166, 0x90,73, 0,166, 0x91,45, 0x90,74, 0,166, 0x90,71, 0,166,
	0x91,57, 0x90,69, 0,166, 0x90,68, 0,138, 0x81, 0,27, 0x91,57, 0x90,76, 0,166,
	0x90,73, 0,166, 0x91,45, 0x90,78, 0,166, 0x90,74, 0,166, 0x91,57, 0x90,73,
	0,166, 0x90,71, 0,138, 0x81, 0,27, 0x91,57, 0x90,76, 0,166, 0x90,73, 0,166,
	0x91,45, 0x90,74, 0,166, 0x90,71, 0,166, 0x91,57, 0x90,69, 0,166, 0x90,68,
	0,138, 0x81, 0,27, 0x91,57, 0x90,73, 0,166, 0x90,71, 0,166, 0x91,59, 0x90,74,
	0,166, 0x90,73, 0,166, 0x91,61, 0x90,71, 0,166, 0x90,69, 0,166, 0x91,62,
	0x90,78, 0,166, 0x91,61, 0,166, 0x90,68, 0x91,64, 0,166, 0x91,62, 0,166,
	0x90,69, 0x91,61, 0,166, 0x91,59, 0,166, 0x91,61, 0,27, 0x80, 0,27, 0x90,71,
	0,55, 0x90,69, 0,55, 0x91,62, 0x90,68, 0,55, 0x90,69, 0,55, 0x90,71,
	0,27, 0x80, 0,27, 0x90,71, 0x91,64, 1,77, 0x91,52, 0,166, 0x90,69, 0,166,
	0x91,57, 0x90,69, 0,166, 0x91,52, 0,166, 0x91,54, 0,166, 0x91,56, 0,166,
	0x91,57, 0x80, 0,166, 0x91,59, 0,166, 0x91,61, 0,166, 0x91,59, 0,166, 0x90,76,
	0x91,62, 0,166, 0x91,61, 0,166, 0x90,81, 0x91,59, 0,166, 0x91,57, 0,166,
	0x91,64, 0,166, 0x90,71, 0,166, 0x91,52, 0x90,73, 0,166, 0x90,75, 0,166,
	0x90,76, 0x81, 0,166, 0x90,78, 0,166, 0x90,79, 0,166, 0x90,78, 0,166, 0x91,59,
	0x90,81, 0,166, 0x90,79, 0,166, 0x91,64, 0x90,78, 0,166, 0x90,76, 0,166,
	0x90,83, 0,166, 0x91,54, 0,166, 0x90,71, 0x91,56, 0,166, 0x91,58, 0,166,
	0x91,59, 0x80, 0,166, 0x91,61, 0,166, 0x91,62, 0,166, 0x91,61, 0,166, 0x90,66,
	0x91,64, 0,166, 0x91,62, 0,166, 0x90,71, 0x91,61, 0,166, 0x91,59, 0,166,
	0x91,66, 0,166, 0x90,70, 0,166, 0x91,54, 0x90,71, 0,166, 0x90,73, 0,166,
	0x91,64, 0x90,74, 0,166, 0x90,76, 0,166, 0x91,62, 0x90,78, 0,166, 0x90,76,
	0,166, 0x91,71, 0x90,79, 0,166, 0x91,70, 0x90,78, 0,166, 0x91,71, 0x90,76,
	0,166, 0x90,74, 0,166, 0x91,61, 0x90,76, 0,166, 0x90,74, 0,166, 0x91,70,
	0x90,78, 0,166, 0x91,68, 0x90,76, 0,166, 0x91,70, 0x90,74, 0,166, 0x90,73,
	0,166, 0x91,71, 0x90,74, 0,166, 0x91,69, 0x90,73, 0,166, 0x91,67, 0x90,76,
	0,166, 0x91,66, 0x90,74, 0,166, 0x91,64, 0x90,73, 0,166, 0x91,62, 0x90,71,
	0,166, 0x91,64, 0x90,73, 0,166, 0x91,62, 0,166, 0x90,70, 0x91,66, 0,83,
	0x90,68, 0,83, 0x91,64, 0x90,70, 0,166, 0x91,62, 0x90,71, 0,166, 0x91,61,
	0,166, 0x91,62, 0,166, 0x90,73, 0x91,64, 0,138, 0x80, 0,27, 0x90,73, 0x91,66,
	0,83, 0x90,74, 0,83, 0x90,73, 0,166, 0x91,54, 0,166, 0x90,71, 0,166,
	0x91,59, 0,166, 0x90,66, 0,166, 0x91,54, 0x90,68, 0,166, 0x90,70, 0,166,
	0x91,50, 0x90,71, 0,166, 0x90,73, 0,166, 0x91,47, 0x90,74, 0,166, 0x90,73,
	0,166, 0x90,76, 0,166, 0x90,74, 0,166, 0x90,73, 0x81, 0,166, 0x90,71, 0,166,
	0x90,79, 0,166, 0x91,47, 0,166, 0x91,49, 0,166, 0x91,51, 0,166, 0x91,52,
	0,166, 0x91,54, 0,166, 0x91,55, 0,166, 0x91,54, 0,166, 0x91,57, 0,166,
	0x91,55, 0,166, 0x91,54, 0,166, 0x91,52, 0,166, 0x91,57, 0,138, 0x80, 0,27,
	0x90,64, 0,166, 0x90,66, 0,166, 0x90,68, 0,166, 0x90,69, 0,166, 0x90,71,
	0,166, 0x90,73, 0,166, 0x90,71, 0,166, 0x90,74, 0,166, 0x90,73, 0,166,
	0x90,71, 0,166, 0x90,69, 0,166, 0x90,78, 0,138, 0x81, 0,27, 0x91,45, 0,166,
	0x91,47, 0,166, 0x91,49, 0,166, 0x91,50, 0,166, 0x91,52, 0,166, 0x91,54,
	0,166, 0x91,52, 0,166, 0x91,55, 0,166, 0x91,54, 0,166, 0x91,52, 0,166,
	0x91,50, 0,166, 0x91,56, 0,138, 0x80, 0,27, 0x90,71, 0,166, 0x90,76, 0,166,
	0x91,52, 0x90,74, 0,166, 0x91,54, 0x90,73, 0,166, 0x91,56, 0x90,71, 0,166,
	0x91,57, 0x90,73, 0,166, 0x91,56, 0x90,71, 0,166, 0x91,54, 0x90,74, 0,166,
	0x91,52, 0x90,73, 0,166, 0x91,50, 0x90,71, 0,166, 0x91,54, 0x90,69, 0,166,
	0x91,52, 0x90,68, 0,83, 0x90,69, 0,83, 0x91,50, 0x90,68, 0,166, 0x91,49,
	0x90,64, 0,166, 0x91,47, 0,166, 0x90,69, 0x91,49, 0,166, 0x90,71, 0x91,45,
	0,166, 0x90,73, 0x91,57, 0,166, 0x90,71, 0,166, 0x91,54, 0x90,74, 0,166,
	0x90,73, 0,166, 0x91,49, 0x90,71, 0,166, 0x90,69, 0,166, 0x91,50, 0x90,78,
	0,166, 0x91,49, 0,166, 0x91,52, 0,166, 0x90,68, 0x91,50, 0,166, 0x90,69,
	0x91,49, 0,166, 0x90,68, 0x91,47, 0,166, 0x90,69, 0x91,49, 0,166, 0x90,74,
	0x91,50, 0,166, 0x90,71, 0x91,52, 1,77, 0x91,40, 0,166, 0x90,69, 0,138,
	0x80, 0,27, 0x90,69, 0x91,45, 0,166, 0x91,49, 0,166, 0x91,50, 0,166, 0x90,73,
	0x91,52, 0,166, 0x90,74, 0x91,54, 0,166, 0x90,76, 0x91,55, 0,166, 0x90,66,
	0x91,57, 0,166, 0x91,55, 0,166, 0x90,67, 0x91,59, 0,166, 0x91,57, 0,166,
	0x90,71, 0x91,55, 0,83, 0x90,69, 0,83, 0x91,54, 0x90,67, 0,83, 0x90,69,
	0,83, 0x91,55, 0x90,71, 0,166, 0x91,54, 0,166, 0x90,74, 0x91,57, 0,83,
	0x90,73, 0,83, 0x91,55, 0x90,71, 0,83, 0x90,73, 0,83, 0x91,54, 0x90,74,
	0,166, 0x91,52, 0,166, 0x91,54, 0,166, 0x90,73, 0x91,52, 0,166, 0x90,76,
	0x91,55, 0,166, 0x90,74, 0x91,54, 0,166, 0x90,73, 0x91,52, 0,166, 0x90,74,
	0x91,50, 0,166, 0x90,76, 0x91,57, 0,166, 0x90,74, 0,166, 0x91,45, 0x90,73,
	0,166, 0x90,71, 0,166, 0x90,69, 0x81, 0,166, 0x90,67, 0,166, 0x90,66, 0,166,
	0x90,64, 0,166, 0x90,67, 0,166, 0x90,66, 0,166, 0x90,64, 0,166, 0x90,62,
	0,166, 0x90,69, 0,166, 0x90,67, 0,166, 0x90,66, 0,166, 0x90,64, 0,166,
	0x91,50, 0x90,66, 0,166, 0x91,52, 0x90,62, 0,166, 0x91,54, 0x90,69, 0,166,
	0x91,52, 0,166, 0x90,71, 0x91,55, 0,166, 0x91,54, 0,166, 0x90,73, 0x91,52,
	0,55, 0x90,71, 0,55, 0x90,73, 0,55, 0x91,50, 0x90,74, 0,166, 0x91,57,
	0x90,73, 0,83, 0x90,74, 0,83, 0x91,55, 0x90,73, 0,166, 0x91,54, 0x90,71,
	0,83, 0x90,69, 0,83, 0x91,52, 0,125, 0x80, 0,41, 0x90,74, 0x91,54, 0,166,
	0x91,50, 0,138, 0x80, 0,27, 0x90,74, 0x91,57, 0,166, 0x91,54, 0,166, 0x90,62,
	0x91,59, 0,166, 0x91,55, 0,166, 0x90,74, 0x91,54, 0,166, 0x91,52, 0,138,
	0x80, 0,27, 0x90,74, 0x91,57, 0,166, 0x91,54, 0,166, 0x90,62, 0x91,55, 0,166,
	0x91,52, 0,166, 0x90,74, 0x91,50, 0,166, 0x91,49, 0,138, 0x80, 0,27, 0x90,74,
	0x91,57, 0,166, 0x91,54, 0,166, 0x90,62, 0x91,59, 0,166, 0x91,55, 0,166,
	0x90,74, 0x91,54, 0,166, 0x91,52, 0,138, 0x80, 0,27, 0x90,74, 0x91,57, 0,166,
	0x91,54, 0,166, 0x90,62, 0x91,55, 0,166, 0x91,52, 0,166, 0x90,74, 0x91,50,
	0,166, 0x90,76, 0x91,49, 0,166, 0x90,78, 0x91,50, 0,166, 0x90,76, 0,166,
	0x91,52, 0x90,79, 0,166, 0x90,78, 0,166, 0x91,54, 0x90,76, 0,166, 0x90,74,
	0,166, 0x91,55, 0x90,83, 0,166, 0x91,54, 0,166, 0x90,73, 0x91,57, 0,166,
	0x91,55, 0,166, 0x90,74, 0x91,54, 0,166, 0x91,52, 0,166, 0x91,54, 0,27,
	0x80, 0,27, 0x90,76, 0,55, 0x90,74, 0,55, 0x91,55, 0x90,73, 0,55, 0x90,74,
	0,55, 0x90,76, 0,41, 0x80, 0,13, 0x90,76, 0x91,57, 1,77, 0x91,45, 0,166,
	0x90,74, 0,138, 0x80, 0,27, 0x90,74, 0x91,47, 0,166, 0x91,45, 0,166, 0x90,62,
	0x91,48, 0,166, 0x91,47, 0,166, 0x90,67, 0x91,45, 0,166, 0x91,43, 0,166,
	0x91,50, 0,166, 0x90,57, 0,166, 0x90,59, 0,166, 0x90,61, 0,166, 0x90,62,
	0,166, 0x90,64, 0,166, 0x90,66, 0,138, 0x81, 0,27, 0x91,49, 0x90,64, 0,166,
	0x91,47, 0x90,67, 0,166, 0x91,45, 0x90,66, 0,166, 0x91,43, 0x90,64, 0,166,
	0x91,42, 0x90,62, 0,166, 0x91,43, 0x90,71, 0,166, 0x91,42, 0,166, 0x91,45,
	0,166, 0x90,61, 0x91,43, 0,166, 0x90,62, 0x91,42, 0,166, 0x90,61, 0x91,40,
	0,166, 0x90,62, 0x91,42, 0,166, 0x90,67, 0x91,43, 0,166, 0x90,64, 0x91,45,
	1,244, 0x90,62, 0,138, 0x80, 0,27, 0x90,62, 0x91,38, 7,208, 0x80, 0x81,
	0xf0
};

void setup()
{
	ardtune.initChannel(PIN_SPEAKER_1);
	ardtune.initChannel(PIN_SPEAKER_2);
	arduboy.boot();
	arduboy.setFrameRate(frameRate);	// Time calculated based on set variable frames per second.
	arduboy.clear();
	arduboy.audio.on();
}

void loop()
{
	// Second incrementation, every 1000 millisecond 1 second is added
	if (millis() >= milSec)
	{
		milSec += fullSec;

		// Pause clock hold A and B, Only works with screen ON
		if (arduboy.pressed(A_BUTTON) && arduboy.pressed(B_BUTTON) && diplayOnOff && !alarmSetting && !timerSetting)
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

	// Calculate time and store it.
	clock();
	
	// Exit loop function if screen should be OFF
	ShowDisplay();
	if (!diplayOnOff)
	{
		return;
	}

	if (HeldDownButton() || alarmSetting)
	{
		HeldLeftButton();
		DisplayAlarm();		
	}
	else if (HeldUpButton() || timerSetting)
	{
		ardtune.stopScore();
	}
	else
	{
		arrD = AdjustTime(s, m, h, true);
		s = *(arrD);
		m = *(arrD + 1);
		h = *(arrD + 2);
		DisplayTime();
	}

	if (sA == s && mA == m && hA == h && ampmA == ampm && alarmOnSetting)
	{
		arduboy.digitalWriteRGB(RGB_ON, RGB_ON, RGB_ON);
		ardtune.playScore(score);
	}
	arduboy.display();
}

// Calculate time and store it.
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

}

// Display On or Off, TRUE = ON, FALSE = OFF
void ShowDisplay()
{
	// Press LEFT and RIGHT to turn screen OFF, clock still run in the background.
	if (arduboy.pressed(LEFT_BUTTON) && arduboy.pressed(RIGHT_BUTTON) && arduboy.notPressed(A_BUTTON) && arduboy.notPressed(B_BUTTON) && arduboy.notPressed(UP_BUTTON) && arduboy.notPressed(DOWN_BUTTON))
	{
		arduboy.clear();
		arduboy.display();
		diplayOnOff = false;

	}
	// Press UP and DOWN to turn screen ON and display clock.
	else if (arduboy.pressed(UP_BUTTON) && arduboy.pressed(DOWN_BUTTON) && arduboy.notPressed(A_BUTTON) && arduboy.notPressed(B_BUTTON) && arduboy.notPressed(LEFT_BUTTON) && arduboy.notPressed(RIGHT_BUTTON))
	{
		diplayOnOff = true;
	}
}

// Modify time sA=Seconds, mA=Minutes, hA=Hours, ClockOrTimer TRUE=Clock, FALSE=Alarm
int * AdjustTime(int sAA, int mAA, int hAA, bool ClockOrAlarm)
{
	sAv = sAA;
	mAv = mAA;
	hAv = hAA;
	static int adjT[3];
	

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
		if ((ampm || ampmA) && hAv == 12)
		{
			hAv = 0;
			if (ClockOrAlarm)
			{
				hA = 0;
			}
			else
			{
				h = 0;
			}
		}
		else if ((!ampm || !ampmA) && hAv != 12)
		{
			hAv += 12;
			if (ClockOrAlarm)
			{
				hA += 12;
			}
			else
			{
				h += 12;
			}
		}

	}
	// Press LEFT and DOWN to set clock type to 12h AM/PM
	if (arduboy.pressed(LEFT_BUTTON) && arduboy.pressed(DOWN_BUTTON) && !clockType&& arduboy.notPressed(A_BUTTON) && arduboy.notPressed(B_BUTTON) && arduboy.notPressed(RIGHT_BUTTON) && arduboy.notPressed(UP_BUTTON))
	{
		clockType = true;
		// Set AM PM according to current time
		if (hAv >= 12)
		{
			ampm = false;
			ampmA = false;
			
			if (hAv > 12 && ClockOrAlarm)
			{
				hAv -= 12;
				hA -= 12;
			}
			else if (hAv > 12 && !ClockOrAlarm)
			{
				hAv -= 12;
				h -= 12;
			}
		}
		else
		{	
			ampm = true;
			ampmA = true;
		}
		if (hAv == 0)
		{
			hAv = 12;
			if (ClockOrAlarm)
			{
				hA = 12;
			}
			else if (!ClockOrAlarm)
			{
				h = 12;
			}

		}
	}

	// Adjust Hours hold A, press UP or DOWN
	if (arduboy.pressed(A_BUTTON) && arduboy.pressed(UP_BUTTON) && arduboy.notPressed(B_BUTTON))
	{
		HourTurn(true, ClockOrAlarm);
	}
	else if (arduboy.pressed(A_BUTTON) && arduboy.pressed(DOWN_BUTTON) && arduboy.notPressed(B_BUTTON))
	{
		HourTurn(false, ClockOrAlarm);
	}
	// Adjust Minutes hold B, press UP or DOWN
	else if (arduboy.pressed(B_BUTTON) && arduboy.pressed(UP_BUTTON) && arduboy.notPressed(A_BUTTON))
	{
		MinuteTurn(true, ClockOrAlarm);
	}
	else if (arduboy.pressed(B_BUTTON) && arduboy.pressed(DOWN_BUTTON) && arduboy.notPressed(A_BUTTON))
	{
		MinuteTurn(false, ClockOrAlarm);
	}
	// Adjust Seconds hold A and B, press UP or DOWN
	else if (arduboy.pressed(A_BUTTON) && arduboy.pressed(B_BUTTON) && arduboy.pressed(UP_BUTTON))
	{
		SecondTurn(true, ClockOrAlarm);
	}
	else if (arduboy.pressed(A_BUTTON) && arduboy.pressed(B_BUTTON) && arduboy.pressed(DOWN_BUTTON))
	{
		SecondTurn(false, ClockOrAlarm);
	}

	adjT[0] = sAv;
	adjT[1] = mAv;
	adjT[2] = hAv;

	return adjT;
}

// Swap AM/PM depending of how it start, bool ClockOrTimer TRUE=Clock, FALSE=Alarm
boolean AmPmSwap(bool ClockOrAlarm) 
{
	if (ClockOrAlarm)
	{
		if (ampm)
		{
			ampm = false;
		}
		else
		{
			ampm = true;
		}
	}
	else if (!ClockOrAlarm)
	{
		if (ampmA)
		{
			ampmA = false;
		}
		else
		{
			ampmA = true;
		}
	}
}

// Increment(TRUE) or Decrement(FALSE) hours, ClockOrTimer TRUE=Clock, FALSE=Alarm
int HourTurn(bool changeH, bool ClockOrAlarm)
{
	if (changeH)
	{
		hAv++;
		if (clockType && hAv > 12)
		{
			hAv = 1;
		}
		else if (clockType && hAv == 12)
		{
			AmPmSwap(ClockOrAlarm);	
		}
		else if (!clockType && hAv == 24)
		{
			hAv = 0;
		}
	}
	else
	{
		hAv--;
		if (clockType && hAv < 1)
		{
			hAv = 12;
		}
		else if (clockType && hAv == 11)
		{
			AmPmSwap(ClockOrAlarm);
		}
		else if (!clockType && hAv < 0)
		{
			hAv = 23;
		}
	}

}

// Increment or decrement minutes
int MinuteTurn(bool changeM, bool ClockOrAlarm)
{
	if (changeM)
	{
		mAv++;
		if (mAv > 59)
		{
			mAv = 0;
			HourTurn(true, ClockOrAlarm);
		}
	}
	else
	{
		mAv--;
		if (mAv < 0)
		{
			mAv = 59;
			HourTurn(false, ClockOrAlarm);
		}
	}
}

// Increment or decrement seconds
int SecondTurn(bool changeS, bool ClockOrAlarm)
{
	if (changeS)
	{
		sAv++;
		if (sAv > 59)
		{
			sAv = 0;
			MinuteTurn(true, ClockOrAlarm);
		}
	}
	else
	{
		sAv--;
		if (sAv < 0)
		{
			sAv = 59;
			MinuteTurn(false, ClockOrAlarm);
		}
	}
}



// Combine necessary strings to present the time, bool ClockOrTimer TRUE=Clock, FALSE=Alarm
String CreateDisplayText(int sD, int mD, int hD, bool ClockOrAlarm)
{
	// Seconds diplay as 01 instead of 1
	if (sD >= 0 && sD <= 9)
	{
		secD = "0";
	}
	else
	{
		secD = "";
	}
	// Minutes diplay as 01 instead of 1
	if (mD >= 0 && mD <= 9)
	{
		minD = "0";
	}
	else
	{
		minD = "";
	}
	// Hour Space in front for time under 10
	if (hD < 10)
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
		if (ClockOrAlarm)
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
		else if (!ClockOrAlarm)
		{
			if (ampmA)
			{
				ampmText = " AM";
			}
			else
			{
				ampmText = " PM";
			}
		}
	}
	else
	{
		ampmText = "";
	}

	
	return hourD + hD + clockS + minD + mD + clockS + secD + sD + ampmText;	// Create clock string
}

// Isolate a button, return true when all the other button are not press except the one you've chosen
boolean NoButton()
{
	if (arduboy.notPressed(A_BUTTON) && arduboy.notPressed(B_BUTTON) && arduboy.notPressed(UP_BUTTON) && arduboy.notPressed(DOWN_BUTTON) && arduboy.notPressed(LEFT_BUTTON) && arduboy.notPressed(RIGHT_BUTTON))
	{
		return true;
	}
	return false;
}

// Isolate a button, return true when all the other button are not press except the one you've chosen
boolean SingleButton(String chosenButton)
{
	if (chosenButton == "A")
	{
		if (arduboy.notPressed(B_BUTTON) && arduboy.notPressed(UP_BUTTON) && arduboy.notPressed(DOWN_BUTTON) && arduboy.notPressed(LEFT_BUTTON) && arduboy.notPressed(RIGHT_BUTTON))
		{
			return true;
		}
	}
	else if (chosenButton == "B")
	{
		if (arduboy.notPressed(A_BUTTON) && arduboy.notPressed(UP_BUTTON) && arduboy.notPressed(DOWN_BUTTON) && arduboy.notPressed(LEFT_BUTTON) && arduboy.notPressed(RIGHT_BUTTON))
		{
			return true;
		}
	}
	else if (chosenButton == "UP")
	{
		if (arduboy.notPressed(B_BUTTON) && arduboy.notPressed(A_BUTTON) && arduboy.notPressed(DOWN_BUTTON) && arduboy.notPressed(LEFT_BUTTON) && arduboy.notPressed(RIGHT_BUTTON))
		{
			return true;
		}
	}
	else if (chosenButton == "DOWN")
	{
		if (arduboy.notPressed(B_BUTTON) && arduboy.notPressed(UP_BUTTON) && arduboy.notPressed(A_BUTTON) && arduboy.notPressed(LEFT_BUTTON) && arduboy.notPressed(RIGHT_BUTTON))
		{
			return true;
		}
	}
	else if (chosenButton == "LEFT")
	{
		if (arduboy.notPressed(B_BUTTON) && arduboy.notPressed(UP_BUTTON) && arduboy.notPressed(DOWN_BUTTON) && arduboy.notPressed(A_BUTTON) && arduboy.notPressed(RIGHT_BUTTON))
		{
			return true;
		}
	}
	else if (chosenButton == "RIGHT")
	{
		if (arduboy.notPressed(B_BUTTON) && arduboy.notPressed(UP_BUTTON) && arduboy.notPressed(DOWN_BUTTON) && arduboy.notPressed(LEFT_BUTTON) && arduboy.notPressed(A_BUTTON))
		{
			return true;
		}
	}
	return false;
}

// Detect for down button held
boolean HeldDownButton()
{
	if (arduboy.pressed(DOWN_BUTTON) && SingleButton("DOWN"))
	{
		if (millis() >= timeHeld && !startCounting && buttonHeld)
		{
			buttonHeld = false;
			timeHeld = 0;
			arduboy.digitalWriteRGB(RGB_ON, RGB_OFF, RGB_OFF);
			if (alarmSetting)
			{
				alarmSetting = false;
				return false;
			}
			else
			{
				alarmSetting = true;
				return true;
			}				
		}
		if (startCounting)
		{
			timeHeld = millis() + heldTime;
			startCounting = false;
			arduboy.digitalWriteRGB(RGB_OFF, RGB_ON, RGB_OFF);
		}
	}
	if (NoButton())
	{
		buttonHeld = true;
		startCounting = true;
		arduboy.digitalWriteRGB(RGB_OFF, RGB_OFF, RGB_OFF);
		return false;
	}
}

// Detect for down button held
boolean HeldUpButton()
{
	if (arduboy.pressed(UP_BUTTON) && SingleButton("UP"))
	{
		if (millis() >= timeHeld && !startCounting && buttonHeld)
		{
			buttonHeld = false;
			timeHeld = 0;
			arduboy.digitalWriteRGB(RGB_ON, RGB_OFF, RGB_ON);
			if (timerSetting)
			{
				timerSetting = false;
				return false;
			}
			else
			{
				timerSetting = true;
				return true;
			}
		}
		if (startCounting)
		{
			timeHeld = millis() + heldTime;
			startCounting = false;
			arduboy.digitalWriteRGB(RGB_ON, RGB_ON, RGB_OFF);
		}
	}
	if (NoButton())
	{
		buttonHeld = true;
		startCounting = true;
		arduboy.digitalWriteRGB(RGB_OFF, RGB_OFF, RGB_OFF);
		return false;
	}
}

// Detect for down button held
boolean HeldLeftButton()
{
	if (arduboy.pressed(LEFT_BUTTON) && SingleButton("LEFT"))
	{
		if (millis() >= timeHeld && !startCounting && buttonHeld)
		{
			buttonHeld = false;
			timeHeld = 0;
			arduboy.digitalWriteRGB(RGB_OFF, RGB_ON, RGB_ON);
			if (alarmOnSetting)
			{
				alarmOnSetting = false;
				return false;
			}
			else
			{
				alarmOnSetting = true;
				return true;
			}
		}
		if (startCounting)
		{
			timeHeld = millis() + heldTime;
			startCounting = false;
			arduboy.digitalWriteRGB(RGB_OFF, RGB_OFF, RGB_ON);
		}
	}
	if (NoButton())
	{
		buttonHeld = true;
		startCounting = true;
		arduboy.digitalWriteRGB(RGB_OFF, RGB_OFF, RGB_OFF);
		//return false; removed for alarm on setting to stay even if button is released.
	}
}

// Detect for down button held
boolean HeldRightButton()
{
	if (arduboy.pressed(RIGHT_BUTTON) && SingleButton("RIGHT"))
	{
		if (millis() >= timeHeld && !startCounting && buttonHeld)
		{
			buttonHeld = false;
			timeHeld = 0;
			arduboy.digitalWriteRGB(RGB_ON, RGB_OFF, RGB_ON);
			if (timerSetting)
			{
				timerSetting = false;
				clockDisplay = true;
				return false;
			}
			else
			{
				timerSetting = true;
				clockDisplay = false;
				return true;
			}
		}
		if (startCounting)
		{
			timeHeld = millis() + heldTime;
			startCounting = false;
			arduboy.digitalWriteRGB(RGB_ON, RGB_ON, RGB_OFF);
		}
	}
	if (NoButton())
	{
		buttonHeld = true;
		startCounting = true;
		arduboy.digitalWriteRGB(RGB_OFF, RGB_OFF, RGB_OFF);
		return false;
	}
}

//  Display the clock on screen
void DisplayTime()
{
	arduboy.setCursor(locX, locY);	// Set location for clock
	clockText = CreateDisplayText(s, m, h, true);
	arduboy.print(clockText); // Print clock
}

// Display the Alarm setting on screen.
void DisplayAlarm()
{
	String alarmStatusText;
	if (alarmOnSetting)
	{

		alarmStatusText = " Alarm ON";
	}
	else
	{
		alarmStatusText = " Alarm OFF";
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
	arduboy.print(clockTypeText + "  Alarm Setting\n\r" + alarmStatusText);

	arduboy.setCursor(locX, locY);
	arrD = AdjustTime(sA, mA, hA, false);
	sA = *(arrD);
	mA = *(arrD + 1);
	hA = *(arrD + 2);
	arduboy.print(CreateDisplayText(sA, mA, hA, false));
}