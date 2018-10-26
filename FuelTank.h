#pragma once
#include "Z:\RTExamples\rt.h"

#ifndef   __FuelTank__
#define   __FuelTank__

class 	FuelTank   {
private:
	double	tankLevel[4] ;	// the data to be protected, in this example a simple ‘double’, 
	int tankNumber;
	CMutex	*theMutex;	// a pointer to a hidden mutex protecting the ‘tankLevel’ variable above

public:
	BOOL WithdrawFuel (double amount, int FuelGrade) 
	{ 
		theMutex -> Wait() ;
		int tankIndex = getTankIndex(FuelGrade);
		BOOL Status = FALSE ;

		if (tankLevel[tankIndex] >= amount) 	{
		        Status = TRUE ;
		        tankLevel[tankIndex] = tankLevel[tankIndex] - amount ;
		}
		theMutex -> Signal() ;
		return Status ;
	} 

	void AddFuel (double amount, int FuelGrade)
	{
		theMutex -> Wait() ;
		int tankIndex = getTankIndex(FuelGrade);
		tankLevel[tankIndex] = tankLevel[tankIndex] + amount ;
		theMutex -> Signal() ;
	}  
	
	void PrintTankLevel(int FuelGrade)
	{
		theMutex -> Wait() ;
		int tankIndex = getTankIndex(FuelGrade);
		printf("Tank #%d FuelGrade %d has = %gL remaining\n", tankNumber, FuelGrade, tankLevel[tankIndex]) ;
		theMutex -> Signal() ;
	}

	int getTankIndex(int FuelGrade) {
		theMutex->Wait();

		int tankIndex = 0;
		switch (FuelGrade) {
		case 87:
			tankIndex = 0;
			break;
		case 90:
			tankIndex = 1;
			break;
		case 92:
			tankIndex = 2;
			break;
		case 94:
			tankIndex = 3;
			break;
		default:
			break;
		}
		theMutex->Signal();

		return tankIndex;
	}

	// constructor and destructor
	FuelTank (int _tankNumber) 
	{ 
		tankNumber = _tankNumber;
		theMutex = new CMutex ("MyFuelTank") ; 
		tankLevel[4] = { 500.0 };
	}

	~FuelTank () {
		delete theMutex; 
	}
} ;	

#endif