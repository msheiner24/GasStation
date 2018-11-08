#pragma once
#include "Z:\RTExamples\rt.h"
#include <stdio.h>
#include <string>

#ifndef   __FuelTank__
#define   __FuelTank__

class 	FuelTank   {


private:
	double tankLevel87;
	double tankLevel89;
	double tankLevel91;
	double tankLevel93;
	int tankNumber;
	string tankName;
	CMutex	*theMutex;	// a pointer to a hidden mutex protecting the ‘tankLevel’ variable above
	
	struct tankDataPool {
		int tankNumber;
		double tankLevel87;
		double tankLevel89;
		double tankLevel91;
		double tankLevel93;
	};

	//struct tankDataPool 	 tankInfo;
	//struct tankDataPool     *ptankInfo;

	

public:
	BOOL WithdrawFuel (double amount, int FuelGrade) 
	{ 
		CDataPool 		dp("dataPoolTank" + tankName, sizeof(struct tankDataPool));	// Create a datapool to communicate with gsc
		struct tankDataPool *ptankInfo = (struct tankDataPool *)(dp.LinkDataPool());

		theMutex -> Wait() ;
		BOOL Status;

		switch (FuelGrade) {
		case 87:
			if (ptankInfo->tankLevel87 >= amount) {
				Status = TRUE;
				ptankInfo->tankLevel87 = ptankInfo->tankLevel87 - amount;
			}
			else { Status = FALSE; };
			break;
		case 89:
			if (ptankInfo->tankLevel89 >= amount) {
				Status = TRUE;
				ptankInfo->tankLevel89 = ptankInfo->tankLevel89 - amount;
			}
			else { Status = FALSE; };
			break;
		case 91:
			if (ptankInfo->tankLevel91 >= amount) {
				Status = TRUE;
				ptankInfo->tankLevel91 = ptankInfo->tankLevel91 - amount;
			}
			else { Status = FALSE; };
			break;
		case 93:
			if (ptankInfo->tankLevel93 >= amount) {
				Status = TRUE;
				ptankInfo->tankLevel93 = ptankInfo->tankLevel93 - amount;
			}
			else { Status = FALSE; };
			break;
		default:
			printf("YOUDUNGOOFED");
			break;
		}

		theMutex -> Signal() ;
		//updateDataPool();
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
		//updateDataPool();
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

	void updateDataPool(){
		//CDataPool 		dp("dataPoolTank" + tankName, sizeof(struct tankDataPool));	// Create a datapool to communicate with gsc
		//struct tankDataPool 	 *tankInfo = (struct tankDataPool *)(dp.LinkDataPool());
		
		theMutex->Wait();
		//ptankInfo->tankNumber  = tankNumber;
		//ptankInfo->tankLevel87 = tankLevel87;
		//ptankInfo->tankLevel89 = tankLevel89;
		//ptankInfo->tankLevel91 = tankLevel91;
		//ptankInfo->tankLevel93 = tankLevel93;
		theMutex->Signal();
	}


	// constructor and destructor
	FuelTank (int _tankNumber) 
	{ 		
		tankNumber = _tankNumber;
		tankName = std::to_string(tankNumber);
		

		
		//
		//ptankInfo = &tankInfo;
		
		//CDataPool 		dp2("dataPoolTank" + tankName, sizeof(struct tankDataPool));	// Create a datapool to communicate with gsc
		//ptankInfo = (struct tankDataPool *)(dp2.LinkDataPool());
		CDataPool 		dp2("dataPoolTank"+tankName, sizeof(struct tankDataPool));	// Create a datapool to communicate with gsc
		struct tankDataPool 	 *ptankInfo = (struct tankDataPool *)(dp2.LinkDataPool());
														//struct tankDataPool *ptankInfo = (struct tankDataPool *)(dp2.LinkDataPool());
		//printf("Creating datapool dataPoolTank%s", tankName);
		//dpTank = (struct tankDataPool *)(dp.LinkDataPool());

		theMutex = new CMutex ("MyFuelTank") ; 
		//theMutex->Wait();
		ptankInfo->tankNumber = tankNumber;
		ptankInfo->tankLevel87 = 500.0;
		ptankInfo->tankLevel89 = 500.0;
		ptankInfo->tankLevel91 = 500.0;
		ptankInfo->tankLevel93 = 500.0;
		//pTankInfo = &tankInfo;
		//updateDataPool();
		//theMutex->Signal();
		
	}

	~FuelTank () {
		delete theMutex; 
	}
} ;	

#endif