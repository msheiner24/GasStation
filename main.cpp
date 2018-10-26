// Lab3.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include <stdio.h>
#include "Pump.h"
#include "Customer.h"
// #include "FuelTank.h"

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
	
	Pump Pump1(1), Pump2(2), Pump3(3), Pump4(4);


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
	
	return 0;
}
