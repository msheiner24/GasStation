/*
	Susan He - 22333141
	Michael Sheiner - 39800140
*/
#pragma once
#include <stdio.h>
#include "rt.h"	

#ifndef   __Customer__
#define   __Customer__


class Customer : public ActiveClass
{
	// put any attributes and member functions here that you like 
	// just like any other class along with any constructors and destructors

	int PumpNumber;
	int CreditCard;
	double Gas;
	int FuelGrade;

public:
	Customer(int _PumpNumber, int _CreditCard, double _Gas, int _FuelGrade);
	Customer();
	~Customer();

private:

	int main(void) {
		std::string PumpName = std::to_string(PumpNumber);
		CPipe	pipe(PumpName, 1024);		// Create a pipe 'p1' with the name "MyPipe2"
		CSemaphore		ps1(PumpName, 0, 1);    // semaphore with initial value 0 and max value 1
		CSemaphore		cs1(PumpName, 1, 1);    // semaphore with initial value 1 and max value 1
		cs1.Wait();
		pipe.Write(&Gas, sizeof(Gas));
		pipe.Write(&FuelGrade, sizeof(FuelGrade));
		pipe.Write(&CreditCard, sizeof(CreditCard));
		ps1.Signal();
		return 0;
	}
};

#endif 
