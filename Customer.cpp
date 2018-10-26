/*
	Susan He - 22333141
	Michael Sheiner - 39800140
*/
#include <stdio.h>
#include "Customer.h"
#include "rt.h"


Customer::Customer(int _PumpNumber, int _CreditCard, double _Gas, int _FuelGrade)	// call base class constructor with number
{
	printf("Customer constructor being called\n");
	PumpNumber = _PumpNumber;
	CreditCard = _CreditCard;
	Gas = _Gas;
	FuelGrade = _FuelGrade;
}

Customer::Customer() 	// call default base class constructor 
{
	printf("Customer default constructor being called\n");
	PumpNumber = 0;
	CreditCard = 0;
	Gas = 0;
	FuelGrade = 0;
}

Customer::~Customer()
{
	printf("Customer destructor being called\n");
	PumpNumber = 0;
}



