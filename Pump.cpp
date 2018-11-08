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
	M = new CMutex("MyFuelTank");
}

Pump::Pump() 	// call default base class constructor 
{
	//printf("Pump default constructor being called\n");
	PumpNumber = "0";
}

Pump::~Pump()
{
	//printf("Pump destructor being called\n");
	PumpNumber = "0";
}


void Pump::SetFuelGrade(int FuelGrade) 	// set fuel grade chosen by customer
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
	//Set pump state to ON and reset bill and gas level
	State = 1;
	CurrentGasLevel = 0;
	Bill = 0;
}


void Pump::Print2Dos(int pump_state) 	// call default base class constructor 
{
	M->Wait();
	if (pump_state) {
		MOVE_CURSOR(0, CursorY + 2);             // move cursor to cords [x,y]
		printf("\33[2K");
		printf("Customer Name:\n");
		fflush(stdout);		      	// force output to be written to screen
		MOVE_CURSOR(0, CursorY + 3);             // move cursor to cords [x,y]
		printf("\33[2K");
		printf("Credit Card:\n");
		fflush(stdout);		      	// force output to be written to screen
		MOVE_CURSOR(0, CursorY + 4);             // move cursor to cords [x,y]
		printf("\33[2K");
		printf("Price:\n");
		fflush(stdout);		      	// force output to be written to screen
		MOVE_CURSOR(40, CursorY + 2);             // move cursor to cords [x,y]
		printf("Fuel Grade:\n");
		fflush(stdout);		      	// force output to be written to screen
		/*
		MOVE_CURSOR(40, CursorY + 3);             // move cursor to cords [x,y]
		printf("Pump Status : Off\n");
		fflush(stdout);		      	// force output to be written to screen
		*/
		MOVE_CURSOR(40, CursorY + 3);             // move cursor to cords [x,y]
		printf("Fill Level:\n");
		fflush(stdout);		      	// force output to be written to screen
		MOVE_CURSOR(40, CursorY + 4);             // move cursor to cords [x,y]
		printf("Bill:\n");
		fflush(stdout);		      	// force output to be written to screen
	}
	else {
		MOVE_CURSOR(0, CursorY + 2);             // move cursor to cords [x,y]
		printf("\33[2K");
		printf("Customer Name: %s\n", CustomerName.c_str());
		fflush(stdout);		      	// force output to be written to screen
		MOVE_CURSOR(0, CursorY + 3);             // move cursor to cords [x,y]
		printf("\33[2K");
		printf("Credit Card: %d\n", CreditCard);
		fflush(stdout);		      	// force output to be written to screen
		MOVE_CURSOR(0, CursorY + 4);             // move cursor to cords [x,y]
		printf("\33[2K");
		printf("Price: $%.2f/litre\n", Price);
		fflush(stdout);		      	// force output to be written to screen
		MOVE_CURSOR(40, CursorY + 2);             // move cursor to cords [x,y]
		printf("Fuel Grade: %d (%s)\n", FuelGrade, FuelGradeType.c_str());
		fflush(stdout);		      	// force output to be written to screen
		/*
		MOVE_CURSOR(40, CursorY + 3);             // move cursor to cords [x,y]
		if (State)
			printf("Pump Status: Filling\n");
		else
			printf("Pump Status: Off    \n");
		*/
		fflush(stdout);		      	// force output to be written to screen
		MOVE_CURSOR(40, CursorY + 3);             // move cursor to cords [x,y]
		printf("Fill Level: %.2f Litres\n", CurrentGasLevel);
		fflush(stdout);		      	// force output to be written to screen
		MOVE_CURSOR(40, CursorY + 4);             // move cursor to cords [x,y]
		printf("Bill: $%.2f\n", Bill);
		fflush(stdout);		      	// force output to be written to screen
	}
	M->Signal();
}

void Pump::CustomerArrival()
{
	// Simulate Customer arriving to pump
	M->Wait();
	MOVE_CURSOR(0, CursorY + 1);             // move cursor to cords [x,y]
	printf("\33[2K");
	printf("Customer %s arrived at pump...\n", CustomerName.c_str());
	fflush(stdout);
	M->Signal();
	SLEEP(3000);
	M->Wait();
	MOVE_CURSOR(0, CursorY + 1);             // move cursor to cords [x,y]
	printf("\33[2K");
	printf("Swiping credit card...\n");
	fflush(stdout);
	M->Signal();
	SLEEP(3000);
}

void Pump::CustomerAuthorized()
{
	// Simulate Customer authorization
	M->Wait();
	MOVE_CURSOR(0, CursorY + 1);             // move cursor to cords [x,y]
	printf("\33[2K");
	printf("Card authorized...\n");
	fflush(stdout);
	M->Signal();
	SLEEP(1000);
	M->Wait();
	MOVE_CURSOR(0, CursorY + 1);             // move cursor to cords [x,y]
	printf("\33[2K");
	printf("Removing gas pump...\n");
	fflush(stdout);
	M->Signal();
	SLEEP(3000);

	// Set fuel grade and start dispensing
	SetFuelGrade(FuelGrade);
	FillGas(MaxGasLevel);
	M->Wait();
	MOVE_CURSOR(0, CursorY + 1);             // move cursor to cords [x,y]
	printf("\33[2K");
	printf("Selected fuel grade %d (%s)...\n", FuelGrade, FuelGradeType.c_str());
	fflush(stdout);
	M->Signal();
	SLEEP(3000);
	M->Wait();
	MOVE_CURSOR(0, CursorY + 1);             // move cursor to cords [x,y]
	printf("\33[2K");
	printf("Pump on. Dispensing Fuel...\n");
	fflush(stdout);
	M->Signal();
	SLEEP(1000);
}

void Pump::CustomerDeparture()
{
	// Simulate Customer leaving pump
	M->Wait();
	MOVE_CURSOR(0, CursorY + 1);             // move cursor to cords [x,y]
	printf("\33[2K");
	printf("Finished filling. Returning hose to pump...\n");
	fflush(stdout);
	M->Signal();
	SLEEP(3000);
	M->Wait();
	MOVE_CURSOR(0, CursorY + 1);             // move cursor to cords [x,y]
	printf("\33[2K");
	printf("Customer %s leaving pump...\n", CustomerName.c_str());
	fflush(stdout);
	M->Signal();
	SLEEP(3000);
	Print2Dos(1); // Print pump status to DOS
	M->Wait();
	MOVE_CURSOR(0, CursorY + 1);             // move cursor to cords [x,y]
	printf("\33[2K");
	printf("Pump off.\n");
	fflush(stdout);
	M->Signal();
	SLEEP(3000);
}

