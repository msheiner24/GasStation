#pragma once
#include "Z:\RTExamples\rt.h"

#ifndef   __FuelTank__
#define   __FuelTank__

class 	FuelTank   {
private:
	double	tankLevel ;	// the data to be protected, in this example a simple ‘double’, 
	int tankNumber;
	CMutex	*theMutex;	// a pointer to a hidden mutex protecting the ‘tankLevel’ variable above

public:
	BOOL WithdrawFuel (double amount) 
	{ 
		theMutex -> Wait() ;
		BOOL Status = FALSE ;
		if (tankLevel >= amount) 	{
		        Status = TRUE ;
		        tankLevel = tankLevel - amount ;	
		}
		theMutex -> Signal() ;
		return Status ;
	} 

	void AddFuel (double amount) 
	{
		theMutex -> Wait() ;
		tankLevel = tankLevel + amount ;
		theMutex -> Signal() ;
	}  
	
	void PrintTankLevel()
	{
		theMutex -> Wait() ;
		printf("Tank #%d Level = %g\n", tankNumber, tankLevel) ;
		theMutex -> Signal() ;
	}

	// constructor and destructor
	FuelTank (int _tankNumber) 
	{ 
		tankNumber = _tankNumber;
		theMutex = new CMutex ("MyFuelTank") ; 
		tankLevel = 2000.0 ; 
	}

	~FuelTank () {
		delete theMutex; 
	}
} ;	

#endif