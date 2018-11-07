/*
	Susan He - 22333141
	Michael Sheiner - 39800140
*/
#pragma once
#include <stdio.h>
#include "rt.h"	
#include "FuelTank.h"

#ifndef   __Pump__
#define   __Pump__


class Pump : public ActiveClass
{

	// put any attributes and member functions here that you like 
	// just like any other class along with any constructors and destructors
	FuelTank *pTank;
	int PumpNumber;
	double MaxGas = 70.0;
	struct CustomerInfo {
		double CurrentGasLevel = 0.0;
		double MaxGasLevel = 0;
		double Bill = 0.0;
		int FuelGrade = 87;
		int CreditCard = 0;
	};
	int State = 0;
	double Price = 0.0;

public:
	Pump(int _PumpNumber, FuelTank &Tank);
	Pump();
	~Pump();
	void SetFuelGrade(int FuelGrade);
	void FillGas(double Gas);

private:

	//  Must override main() inherited from ActiveClass. The base class constructor will then
	//	create a thread to run though the function main()

	int PumpStatus(void *ThreadArgs)
	{
		//int *Speed = (int *)(ThreadArgs);	// get the message passed to the thread. Note cast from 'void *' to 'char *'

		for (int i = 0; i < 10000; i++) {
			if (State == 1)
				//printf("Pump %d is currently filling. Gas Level is at %f litres \n", PumpNumber, newCustomer->CurrentGasLevel);
				printf("Pump %d is currently filling", PumpNumber);
			else {
				printf("Pump %d is currently off \n", PumpNumber);
			}
			SLEEP(1000);
		}
		return 0;									// thread ends here
	}

	int main(void) {
		std::string PumpName = std::to_string(PumpNumber);

		CPipe	pipe(PumpName, 1024);			// Create a pipe to communicate with customer
		CDataPool 		dp("pump1", sizeof(struct CustomerInfo));	// Create a datapool to communicate with gsc

		CreateDataPool();
		CSemaphore		ps1(PumpName, 0, 1);    // semaphore with initial value 0 and max value 1
		CSemaphore		cs1(PumpName, 1, 1);    // semaphore with initial value 1 and max value 1
		CRendezvous     rvPump("rvPump", 4);		// Attempt to create a rendezvous object involving 4 threads
		cs1.Signal();
		rvPump.Wait();

		struct CustomerInfo 	 *newCustomer = (struct CustomerInfo *)(dp.LinkDataPool());
		
		ClassThread<Pump> StatusThread(this, &Pump::PumpStatus, ACTIVE, NULL);
		for (int i = 0; i < 10000; i++) {
			if (State == 1) {
				if (newCustomer->CurrentGasLevel < newCustomer->MaxGasLevel) {
					newCustomer->CurrentGasLevel += 0.5;
					pTank->WithdrawFuel(0.5, newCustomer->FuelGrade);
					pTank->PrintTankLevel(newCustomer->FuelGrade);
					SLEEP(1000);
				}
				else {
					newCustomer->Bill = Price * newCustomer->MaxGasLevel;
					printf("Pump %d finished filling to %f litres. Bill is $ %f\n", PumpNumber, newCustomer->MaxGasLevel, newCustomer->Bill);
					State = 0;
				}
			}
			else {			
				if ((pipe.TestForData() >= sizeof(newCustomer->MaxGasLevel)) && (pipe.TestForData() >= sizeof(newCustomer->FuelGrade))) {
					ps1.Wait();
					pipe.Read(&newCustomer->MaxGasLevel, sizeof(newCustomer->MaxGasLevel));
					pipe.Read(&newCustomer->FuelGrade, sizeof(newCustomer->FuelGrade));
					pipe.Read(&newCustomer->CreditCard, sizeof(newCustomer->CreditCard));
					printf("Recieved a new customer at Pump %d. Credit Card %d has been authorized\n", PumpNumber, newCustomer->CreditCard);
					SetFuelGrade(newCustomer->FuelGrade);
					cs1.Signal();
					FillGas(newCustomer->MaxGasLevel);
				}
			}
		}

		StatusThread.WaitForThread();
		return 0;
	}

	void CreateDataPool() {
		if (PumpNumber == 1) {
			CDataPool 		dp("pump1", sizeof(struct CustomerInfo));	// Create a datapool to communicate with gsc
		}
		else if (PumpNumber == 2) {
			CDataPool 		dp("pump2", sizeof(struct CustomerInfo));	// Create a datapool to communicate with gsc
		}
		else if (PumpNumber == 3) {
			CDataPool 		dp("pump3", sizeof(struct CustomerInfo));	// Create a datapool to communicate with gsc
		}
		else if (PumpNumber == 4) {
			CDataPool 		dp("pump4", sizeof(struct CustomerInfo));	// Create a datapool to communicate with gsc
		}
		else {
			printf("Could not create datapool");
		}
	}
};

#endif 