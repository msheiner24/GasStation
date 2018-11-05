#pragma once
#include "Z:\RTExamples\rt.h"

#ifndef   __FuelTank__
#define   __FuelTank__

class 	FuelTank   {
private:
	double tankLevel87;
	double tankLevel89;
	double tankLevel91;
	double tankLevel93;
	int tankNumber;
	CMutex	*theMutex;	// a pointer to a hidden mutex protecting the ‘tankLevel’ variable above

public:
	BOOL WithdrawFuel (double amount, int FuelGrade) 
	{ 
		theMutex -> Wait() ;
		BOOL Status;
		switch (FuelGrade) {
		case 87:
			if (tankLevel87 >= amount) {
				Status = TRUE;
				tankLevel87 = tankLevel87 - amount;
			}
			else { Status = FALSE; };
			break;
		case 89:
			if (tankLevel89 >= amount) {
				Status = TRUE;
				tankLevel89 = tankLevel89 - amount;
			}
			else { Status = FALSE; };
			break;
		case 91:
			if (tankLevel91 >= amount) {
				Status = TRUE;
				tankLevel91 = tankLevel91 - amount;
			}
			else { Status = FALSE; };
			break;
		case 93:
			if (tankLevel93 >= amount) {
				Status = TRUE;
				tankLevel93 = tankLevel93 - amount;
			}
			else { Status = FALSE; };
			break;
		default:
			printf("YOUDUNGOOFED");
			break;
		}

		theMutex -> Signal() ;
		return Status ;
	} 

	void AddFuel (double amount, int FuelGrade)
	{
		theMutex -> Wait() ;
		BOOL Status;
		switch (FuelGrade) {
		case 87:
			if (tankLevel87 + amount < 500) {
				tankLevel87 = tankLevel87 + amount;
			}
			else { 
				tankLevel87 = 500; 
			}
			break;
		case 89:
			if (tankLevel89 + amount < 500) {
				tankLevel89 = tankLevel89 + amount;
			}
			else {
				tankLevel89 = 500;
			}
			break;
		case 91:
			if (tankLevel91 + amount < 500) {
				tankLevel91 = tankLevel91 + amount;
			}
			else {
				tankLevel91 = 500;
			}
			break;
		case 93:
			if (tankLevel93 + amount < 500) {
				tankLevel93 = tankLevel93 + amount;
			}
			else {
				tankLevel93 = 500;
			}
			break;
		default:
			printf("YOUDUNGOOFED");
			break;
		}
		theMutex -> Signal() ;
	}  
	
	void PrintTankLevel(int FuelGrade)
	{
		theMutex -> Wait() ;
		int tankIndex = 0;
		switch (FuelGrade) {
		case 87:
			printf("Tank #%d FuelGrade %d has = %gL remaining\n", tankNumber, 87, tankLevel87);
			break;
		case 89:
			printf("Tank #%d FuelGrade %d has = %gL remaining\n", tankNumber, 89, tankLevel89);
			break;
		case 91:
			printf("Tank #%d FuelGrade %d has = %gL remaining\n", tankNumber, 91, tankLevel91);
			break;
		case 93:
			printf("Tank #%d FuelGrade %d has = %gL remaining\n", tankNumber, 93, tankLevel93);
			break;
		default:
			printf("YOUDUNGOOFED");
			break;
		}
		theMutex -> Signal() ;
	}


	// constructor and destructor
	FuelTank (int _tankNumber) 
	{ 
		
		tankNumber = _tankNumber;
		theMutex = new CMutex ("MyFuelTank") ; 
		theMutex->Wait();
		tankLevel87 = 500.0;
		tankLevel89 = 500.0;
		tankLevel91 = 500.0;
		tankLevel93 = 500.0;
		theMutex->Signal();
	}

	~FuelTank () {
		delete theMutex; 
	}
} ;	

#endif