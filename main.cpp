// Lab3.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include <stdio.h>
#include "Pump.h"
#include "Customer.h"
#include "FuelTank.h"

struct 	mydatapooldata {
	int pumpnumber;
	int fuelgrade;
	double gas;
};


int main()
{

	Customer Customer1("1", "1190", "9", "87", "Obama");
	Customer Customer2("2", "1275", "5", "89", "Hilary Clinton");
	Customer Customer3("2", "2168", "6", "91", "Justin Trudeau");
	Customer Customer4("3", "3948", "12", "93", "Justin Bieber");
	Customer Customer5("4", "5637", "10", "91", "Donald Trump");
	Customer Customer6("1", "2536", "30", "87", "Kanye");
	Customer Customer7("1", "1182", "52", "89", "Obama");
	Customer Customer8("3", "4672", "58", "89", "Beyonce");
	Customer Customer9("4", "4843", "16", "93", "Snoop Dogg");
	Customer Customer10("1", "2683", "39", "89", "Bill Clinton");
	Customer Customer11("1", "7683", "41", "87", "Stephen Harper");
	Customer Customer12("2", "4623", "49", "91", "Kim Kardashian");

	FuelTank Tank1(1), Tank2(2), Tank3(3), Tank4(4);
	FuelTank *pTank1, *pTank2, *pTank3, *pTank4;
	pTank1 = &Tank1;
	pTank2 = &Tank2;
	pTank3 = &Tank3;
	pTank4 = &Tank4;

	Pump  Pump1("1", *pTank1), Pump2("2", *pTank2), Pump3("3", *pTank3), Pump4("4", *pTank4);


	CProcess gsc(".\\Q4\\debug\\q4.exe",	// pathlist to child program executable
		NORMAL_PRIORITY_CLASS,			// priority
		OWN_WINDOW,						// process has its own window
		ACTIVE							// process is active immediately
	);

	Customer1.Resume();
	Customer2.Resume();
	Customer3.Resume();
	Customer4.Resume();
	Customer5.Resume();
	Customer6.Resume();
	Customer7.Resume();
	Customer8.Resume();
	Customer9.Resume();
	Customer10.Resume();
	Customer11.Resume();
	Customer12.Resume();

	Pump1.Resume();
	Pump2.Resume();
	Pump3.Resume();
	Pump4.Resume();

	Pump1.WaitForThread();
	Pump2.WaitForThread();
	Pump3.WaitForThread();
	Pump4.WaitForThread();

	Customer1.WaitForThread();
	Customer2.WaitForThread();
	Customer3.WaitForThread();
	Customer4.WaitForThread();
	Customer5.WaitForThread();
	Customer6.WaitForThread();
	Customer7.WaitForThread();
	Customer8.WaitForThread();
	Customer9.WaitForThread();
	Customer10.WaitForThread();
	Customer11.WaitForThread();
	Customer12.WaitForThread();

	gsc.WaitForProcess();
	
	return 0;
}
