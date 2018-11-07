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


	Customer Customer1(1, 721891, 9, 87);
	Customer Customer2(2, 182901, 5, 89);
	Customer Customer3(1, 491891, 35, 91);
	
	FuelTank Tank1(1), Tank2(2), Tank3(3), Tank4(4);
	FuelTank *pTank1, *pTank2, *pTank3, *pTank4;
	pTank1 = &Tank1;
	pTank2 = &Tank2;
	pTank3 = &Tank3;
	pTank4 = &Tank4;

	Pump  Pump1(1, *pTank1), Pump2(2, *pTank2), Pump3(3, *pTank3), Pump4(4, *pTank4);

	CProcess gsc(".\\Q4\\debug\\q4.exe",	// pathlist to child program executable
		NORMAL_PRIORITY_CLASS,			// priority
		OWN_WINDOW,						// process has its own window
		ACTIVE							// process is active immediately
	);

	Customer1.Resume();
	Customer2.Resume();
	Customer3.Resume();

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

	gsc.WaitForProcess();
	
	return 0;
}
