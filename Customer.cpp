/*
	Susan He - 22333141
	Michael Sheiner - 39800140
*/
#include <stdio.h>
#include "Customer.h"
#include "rt.h"
using namespace std;


Customer::Customer(string _PumpNumber, string _CreditCard, string _Gas, string _FuelGrade, string _CustomerName)	// call base class constructor with number
{
	//printf("Customer constructor being called\n");
	PumpNumber = _PumpNumber;
	CreditCard = _CreditCard;
	Gas = _Gas;
	FuelGrade = _FuelGrade;
	CustomerName = _CustomerName;
}

Customer::Customer() 	// call default base class constructor 
{
	//printf("Customer default constructor being called\n");
	PumpNumber = "0";
	CreditCard = "0000000000";
	Gas = "0";
	FuelGrade = "87";
	CustomerName = "Default Customer";
}

Customer::~Customer()
{
	//printf("Customer destructor being called\n");
	PumpNumber = "0";
}



