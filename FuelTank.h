#pragma once
#include "rt.h"
#include <stdio.h>
#include <string>

#ifndef   __FuelTank__
#define   __FuelTank__

class 	FuelTank{

private:


	struct tankDataPool {
		int tankNumber;
		double tankLevel87;
		double tankLevel89;
		double tankLevel91;
		double tankLevel93;
	};

	int tankNumber;
	std::string tankName;

	CMutex	*theMutex;	// a pointer to a hidden mutex protecting the ‘tankLevel’ variable above
	
	CDataPool *FuelTankDataPool;

	struct tankDataPool *pTankInfo;

public:

	BOOL WithdrawFuel (double amount, int FuelGrade) 
	{ 
		theMutex -> Wait() ;
		BOOL Status;

		switch (FuelGrade) {
		case 87:
			if (pTankInfo->tankLevel87 >= amount) {
				Status = TRUE;
				pTankInfo->tankLevel87 = pTankInfo->tankLevel87 - amount;
			}
			else { Status = FALSE; };
			break;
		case 89:
			if (pTankInfo->tankLevel89 >= amount) {
				Status = TRUE;
				pTankInfo->tankLevel89 = pTankInfo->tankLevel89 - amount;
			}
			else { Status = FALSE; };
			break;
		case 91:
			if (pTankInfo->tankLevel91 >= amount) {
				Status = TRUE;
				pTankInfo->tankLevel91 = pTankInfo->tankLevel91 - amount;
			}
			else { Status = FALSE; };
			break;
		case 93:
			if (pTankInfo->tankLevel93 >= amount) {
				Status = TRUE;
				pTankInfo->tankLevel93 = pTankInfo->tankLevel93 - amount;
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




	// constructor and destructor
	FuelTank (int _tankNumber) 
	{ 		
		tankNumber = _tankNumber;
		tankName = std::to_string(tankNumber);
		
		FuelTankDataPool = new CDataPool("dataPoolTank" + tankName, sizeof(struct tankDataPool));	// Create a datapool to communicate with gsc
		pTankInfo = (struct tankDataPool *)(FuelTankDataPool->LinkDataPool());
		
		theMutex = new CMutex ("MyFuelTank") ; 
		theMutex->Wait();
		pTankInfo->tankNumber = tankNumber;
		pTankInfo->tankLevel87 = 500.0;
		pTankInfo->tankLevel89 = 500.0;
		pTankInfo->tankLevel91 = 500.0;
		pTankInfo->tankLevel93 = 500.0;
		theMutex->Signal();
		
	}

	~FuelTank () {
		delete theMutex; 
		delete FuelTankDataPool;
	}
} ;	

#endif