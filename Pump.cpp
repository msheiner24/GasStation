/*
	Susan He - 22333141
	Michael Sheiner - 39800140
*/
#include <stdio.h>
#include "Pump.h"
#include "rt.h"



Pump::Pump(int _PumpNumber, FuelTank &Tank)	// call base class constructor with number
{
	printf("Pump %d constructor being called\n", _PumpNumber);
	PumpNumber = _PumpNumber;
	pTank = &Tank;
}

Pump::Pump() 	// call default base class constructor 
{
	printf("Pump default constructor being called\n");
	PumpNumber = 0;
}

Pump::~Pump()
{
	printf("Pump destructor being called\n");
	PumpNumber = 0;
}


void Pump::SetFuelGrade() 	// call default base class constructor 
{
	if (FuelGrade == 89) {
		printf("MID-GRADE UNLEADED fuel grade (89) selected for Pump %d. Price is 1.30 cents/litre. \n", PumpNumber);
		Price = 1.30;
	}
	else if (FuelGrade == 91) {
		printf("PREMIUM UNLEADED fuel grade (91) selected for Pump %d. Price is 1.38 cents/litre. \n", PumpNumber);
		Price = 1.38;
	}
	else if (FuelGrade == 93) {
		printf("SUPER PREMIUM UNLEADED fuel grade (93) selected for Pump %d. Price is 1.47 cents/litre. \n", PumpNumber);
		Price = 1.47;
	}
	else {
		printf("REGULAR UNLEADED fuel grade (87) selected for Pump %d. Price is 1.22 cents/litre. \n", PumpNumber);
		Price = 1.22;
	}
}


void Pump::FillGas() 	// call default base class constructor 
{
	if (Gas > MaxGas) {
		printf("You have entered a value larger than the maximum fill amount of 70 litres\n");
		Gas = MaxGas;
	}
	printf("Filling Pump %d to %f litres\n", PumpNumber, Gas);
	State = 1;
	GasLevel = 0;
	Bill = 0;
}



