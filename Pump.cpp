/*
	Susan He - 22333141
	Michael Sheiner - 39800140
*/
#include <stdio.h>
#include "Pump.h"
#include "rt.h"



Pump::Pump(string _PumpNumber, FuelTank &Tank)	// call base class constructor with number
{
	//printf("Pump %d constructor being called\n", _PumpNumber);
	PumpNumber = _PumpNumber;
	pTank = &Tank;
}

Pump::Pump() 	// call default base class constructor 
{
	printf("Pump default constructor being called\n");
	PumpNumber = "0";
}

Pump::~Pump()
{
	printf("Pump destructor being called\n");
	PumpNumber = "0";
}


void Pump::SetFuelGrade(int FuelGrade) 	// call default base class constructor 
{
	if (FuelGrade == 89) {
		FuelGradeType = "MID-GRADE UNLEADED";
		Price = 1.30;
	}
	else if (FuelGrade == 91) {
		FuelGradeType = "PREMIUM UNLEADED";
		Price = 1.38;
	}
	else if (FuelGrade == 93) {
		FuelGradeType = "SUPER PREMIUM UNLEADED";
		Price = 1.47;
	}
	else {
		FuelGradeType = "REGULAR UNLEADED";
		Price = 1.22;
	}
}


void Pump::FillGas(double Gas) 	
{
//	printf("Filling Pump %d to %f litres\n", PumpNumber, Gas);
	State = 1;
}


void Pump::Print2Dos(int pump_state) 	// call default base class constructor 
{
	CMutex M("DOSMutex"); // mutex to protect DOS window

	// Print pump name to DOS
	M.Wait();
	MOVE_CURSOR(0, CursorY);             // move cursor to cords [x,y]
	printf("Pump %s", PumpNumber.c_str());
	fflush(stdout);		      	// force output to be written to screen
	if (pump_state) {
		MOVE_CURSOR(0, CursorY + 1);             // move cursor to cords [x,y]
		printf("Customer Name:\n");
		fflush(stdout);		      	// force output to be written to screen
		MOVE_CURSOR(0, CursorY + 2);             // move cursor to cords [x,y]
		printf("Credit Card:\n");
		fflush(stdout);		      	// force output to be written to screen
		MOVE_CURSOR(0, CursorY + 3);             // move cursor to cords [x,y]
		printf("Selected Fill Level:\n");
		fflush(stdout);		      	// force output to be written to screen
		MOVE_CURSOR(0, CursorY + 4);             // move cursor to cords [x,y]
		printf("Price:\n");
		fflush(stdout);		      	// force output to be written to screen
		MOVE_CURSOR(40, CursorY + 1);             // move cursor to cords [x,y]
		printf("Selected Fuel Grade:\n");
		fflush(stdout);		      	// force output to be written to screen
		MOVE_CURSOR(40, CursorY + 2);             // move cursor to cords [x,y]
		printf("Pump Status : Off    \n");
		fflush(stdout);		      	// force output to be written to screen
		MOVE_CURSOR(40, CursorY + 3);             // move cursor to cords [x,y]
		printf("Fill Level:\n");
		fflush(stdout);		      	// force output to be written to screen
		MOVE_CURSOR(40, CursorY + 4);             // move cursor to cords [x,y]
		printf("Bill:\n");
		fflush(stdout);		      	// force output to be written to screen
	}
	else {
		MOVE_CURSOR(0, CursorY + 1);             // move cursor to cords [x,y]
		printf("Customer Name: %s\n", CustomerName.c_str());
		fflush(stdout);		      	// force output to be written to screen
		MOVE_CURSOR(0, CursorY + 2);             // move cursor to cords [x,y]
		printf("Credit Card: %d\n", CreditCard);
		fflush(stdout);		      	// force output to be written to screen
		MOVE_CURSOR(0, CursorY + 3);             // move cursor to cords [x,y]
		printf("Selected Fill Level: %.2f Litres\n", MaxGasLevel);
		fflush(stdout);		      	// force output to be written to screen
		MOVE_CURSOR(0, CursorY + 4);             // move cursor to cords [x,y]
		printf("Price: $%.2f/litre\n", Price);
		fflush(stdout);		      	// force output to be written to screen
		MOVE_CURSOR(40, CursorY + 1);             // move cursor to cords [x,y]
		printf("Selected Fuel Grade: %d (%s)\n", FuelGrade, FuelGradeType.c_str());
		fflush(stdout);		      	// force output to be written to screen
		MOVE_CURSOR(40, CursorY + 2);             // move cursor to cords [x,y]
		if (State)
			printf("Pump Status: Filling\n");
		else
			printf("Pump Status: Off    \n");
		fflush(stdout);		      	// force output to be written to screen
		MOVE_CURSOR(40, CursorY + 3);             // move cursor to cords [x,y]
		printf("Fill Level: %.2f Litres\n", CurrentGasLevel);
		fflush(stdout);		      	// force output to be written to screen
		MOVE_CURSOR(40, CursorY + 4);             // move cursor to cords [x,y]
		printf("Bill: $%.2f\n", Bill);
		fflush(stdout);		      	// force output to be written to screen
	}
	M.Signal();
}


