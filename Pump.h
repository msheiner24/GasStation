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
	double GasLevel = 0.0;
	double Gas = 0;
	double Bill = 0.0;
	int FuelGrade = 87;
	int State = 0;
	double Price = 0.0;
	int CreditCard = 0;

public:
	Pump(int _PumpNumber, FuelTank &Tank);
	Pump();
	~Pump();
	void SetFuelGrade();
	void FillGas();

private:

	//  Must override main() inherited from ActiveClass. The base class constructor will then
	//	create a thread to run though the function main()

	int PumpStatus(void *ThreadArgs)
	{
		//int *Speed = (int *)(ThreadArgs);	// get the message passed to the thread. Note cast from 'void *' to 'char *'

		for (int i = 0; i < 10000; i++) {
			if (State == 1)
				printf("Pump %d is currently filling. Gas Level is at %f litres \n", PumpNumber, GasLevel);
			else {
				printf("Pump %d is currently off \n", PumpNumber);
			}
			SLEEP(1000);
		}
		return 0;									// thread ends here
	}

	int main(void) {
		printf("pump running");

		std::string PumpName = std::to_string(PumpNumber);
		CPipe	pipe(PumpName, 1024);		// Create a pipe 'p1' with the name "MyPipe2"
		CSemaphore		ps1(PumpName, 0, 1);    // semaphore with initial value 0 and max value 1
		CSemaphore		cs1(PumpName, 1, 1);    // semaphore with initial value 1 and max value 1
		CRendezvous     rvPump("rvPump", 4);		// Attempt to create a rendezvous object involving 4 threads
		cs1.Signal();
		rvPump.Wait();
		ClassThread<Pump> StatusThread(this, &Pump::PumpStatus, ACTIVE, NULL);
		for (int i = 0; i < 10000; i++) {
			if (State == 1) {
				if (GasLevel < Gas) {
					GasLevel += 0.5;
					//pTank->WithdrawFuel(0.5, FuelGrade);
					//pTank->PrintTankLevel(FuelGrade);
					SLEEP(1000);
				}
				else {
					Bill = Price * Gas;
					printf("Pump %d finished filling to %f litres. Bill is $ %f\n", PumpNumber, Gas, Bill);
					State = 0;
				}
			}
			else {			
				if ((pipe.TestForData() >= sizeof(Gas)) && (pipe.TestForData() >= sizeof(FuelGrade))) {
					ps1.Wait();
					pipe.Read(&Gas, sizeof(Gas));
					pipe.Read(&FuelGrade, sizeof(FuelGrade));
					pipe.Read(&CreditCard, sizeof(CreditCard));
					printf("Recieved a new customer at Pump %d. Credit Card %d has been authorized\n", PumpNumber, CreditCard);
					SetFuelGrade();
					cs1.Signal();
					FillGas();
				}
			}
		}

		StatusThread.WaitForThread();
		return 0;
	}
};

#endif 