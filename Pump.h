/*
	Susan He - 22333141
	Michael Sheiner - 39800140
*/
#pragma once
#include <stdio.h>
#include <string>
#include "rt.h"	
#include "FuelTank.h"
using namespace std;

#ifndef   __Pump__
#define   __Pump__

class Pump : public ActiveClass
{

	// put any attributes and member functions here that you like 
	// just like any other class along with any constructors and destructors
	FuelTank *pTank;
	string PumpNumber;
	double MaxGas = 70.0;
	struct CustomerInfo {
		double CurrentGasLevel = 0.0;
		double MaxGasLevel = 0;
		double Bill = 0.0;
		int FuelGrade = 87;
		int CreditCard = 0;
		bool Authorized = 0;
		bool newArrival = 0;
	};
	double CurrentGasLevel = 0.0;
	double MaxGasLevel = 0;
	double Bill = 0.0;
	int FuelGrade = 87;
	int CreditCard = 0;

	int State = 0;
	double Price = 0.0;
	int CursorY;
	string FuelGradeType = "REGULAR UNLEADED";
	string CustomerName;
	string CreditCardStr;
	string FuelGradeStr;
	string GasStr;
	CMutex	*M; // mutex to protect DOS window
	
public:
	Pump(string _PumpNumber, FuelTank &Tank);
	Pump();
	~Pump();
	void SetFuelGrade(int FuelGrade);
	void FillGas(double Gas);
	void Print2Dos(int pump_state);
	void CustomerArrival();
	void CustomerAuthorized();
	void CustomerDeparture();

private:

	int main(void) {
		//Initialize pipes, semaphores , datapools, structs
		CTypedPipe <string> pipe(PumpNumber, 4);
		CSemaphore EntrySem(("entry" + PumpNumber), 0, 1);
		CSemaphore ExitSem(("exit" + PumpNumber), 0, 1);
		CSemaphore Full(("full" + PumpNumber), 0, 1);
		CSemaphore Empty(("empty" + PumpNumber), 0, 1);
		CSemaphore PStatus(("Producer" + PumpNumber), 1);
		CSemaphore CStatus(("Consumer" + PumpNumber), 1);
		CDataPool 		dp("dataPoolPump" + PumpNumber, sizeof(struct CustomerInfo));	// Create a datapool to communicate with gsc
		CRendezvous     rvPump("rvPump", 4);		// Attempt to create a rendezvous object involving 4 threads
		struct CustomerInfo 	 *newCustomer = (struct CustomerInfo *)(dp.LinkDataPool());
		// calculate Y cursor value
		CursorY = std::stoi(PumpNumber, nullptr, 10);
		CursorY = (6 * (CursorY - 1));
		// Print pump name to DOS
		M->Wait();
		MOVE_CURSOR(0, CursorY);             // move cursor to cords [x,y]
		printf("Pump %s", PumpNumber.c_str());
		fflush(stdout);		      	// force output to be written to screen
		MOVE_CURSOR(0, CursorY + 1);             // move cursor to cords [x,y]
		printf("\33[2K");
		printf("Pump off.\n"); // print status in default state (off)
		fflush(stdout);
		M->Signal();
		Print2Dos(1); // Print pump status to DOS

		EntrySem.Signal();
		rvPump.Wait();
		//CStatus.Wait();
		newCustomer->newArrival = 0;
		newCustomer->Authorized = 0;
		//PStatus.Signal();

		while (1) {
			if (State == 1) {
				if (CurrentGasLevel < MaxGasLevel) {
					CurrentGasLevel += 0.5;
					//CStatus.Wait();
					newCustomer->CurrentGasLevel = CurrentGasLevel;
					//PStatus.Signal();
					pTank->WithdrawFuel(0.5, FuelGrade);
					//pTank->PrintTankLevel(newCustomer->FuelGrade);
					Bill = Price * CurrentGasLevel;

					Print2Dos(0); // Print pump status to DOS
					SLEEP(1000);

				}
				else {
					Bill = Price * CurrentGasLevel;
					//CStatus.Wait();
					newCustomer->Bill = Bill;
					newCustomer->Authorized = 0;
					newCustomer->newArrival = 0;
					//PStatus.Signal();
					State = 0;
					Print2Dos(0); // Print pump status to DOS
					// Simulate Customer leaving pump
					CustomerDeparture();
				}
			}
			else {			
				if (pipe.TestForData()) {
					// Read data from customer-pump pipeline
					Full.Wait();
					pipe.Read(&GasStr);
					MaxGasLevel = std::stod(GasStr);
					pipe.Read(&FuelGradeStr);
					FuelGrade = std::stoi(FuelGradeStr, nullptr, 10);
					pipe.Read(&CreditCardStr);
					CreditCard = std::stoi(CreditCardStr, nullptr, 10);
					pipe.Read(&CustomerName);
					ExitSem.Signal();
					Empty.Wait();
					EntrySem.Signal();

					// Simulate Customer arriving to pump
					CustomerArrival();

					//CStatus.Wait();
					newCustomer->MaxGasLevel = MaxGasLevel;
					newCustomer->FuelGrade = FuelGrade;
					newCustomer->CreditCard = CreditCard;
					newCustomer->newArrival = 1;
					//PStatus.Signal();

					// Wait for authorization from gas station attendant
					while (newCustomer->Authorized != 1) {
						SLEEP(500);
					}
					// Simulate Customer authorization
					CustomerAuthorized();

					SetFuelGrade(FuelGrade);
					FillGas(MaxGasLevel);
					Print2Dos(0); // Print pump status to DOS
				}
			}
		}

		return 0;
	}
};

#endif 