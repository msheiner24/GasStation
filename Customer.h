/*
	Susan He - 22333141
	Michael Sheiner - 39800140
*/
#pragma once
#include <stdio.h>
#include "rt.h"	
using namespace std;

#ifndef   __Customer__
#define   __Customer__


class Customer : public ActiveClass
{
	// put any attributes and member functions here that you like 
	// just like any other class along with any constructors and destructors

	string PumpNumber;
	string CreditCard;
	string Gas;
	string FuelGrade;
	string CustomerName;

public:
	Customer(string _PumpNumber, string _CreditCard, string _Gas, string _FuelType, string _CustomerName);
	Customer();
	~Customer();

private:

	int main(void) {

		CTypedPipe <string> pipe(PumpNumber, 4);
		CSemaphore EntrySem(("entry" + PumpNumber), 0, 1);
		CSemaphore ExitSem(("exit" + PumpNumber), 0, 1);
		CSemaphore Full(("full" + PumpNumber), 0, 1);
		CSemaphore Empty(("empty" + PumpNumber), 0, 1);
		EntrySem.Wait();
		Full.Signal();
		pipe.Write(&Gas);
		pipe.Write(&FuelGrade);
		pipe.Write(&CreditCard);
		pipe.Write(&CustomerName);
		ExitSem.Wait();
		Empty.Signal();
		return 0;
	}
};

#endif 
