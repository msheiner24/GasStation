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
	
public:
	Pump(string _PumpNumber, FuelTank &Tank);
	Pump();
	~Pump();
	void SetFuelGrade(int FuelGrade);
	void FillGas(double Gas);
	void Print2Dos(int pump_state);

private:

	int main(void) {
		//Initialize pipes, semaphores , datapools, structs
		CMutex M("DOSMutex"); // mutex to protect DOS window
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
		// print status in default state (off)
		M.Wait();
		MOVE_CURSOR(0, CursorY + 1);             // move cursor to cords [x,y]
		printf("\33[2K");
		printf("Pump off.\n");
		fflush(stdout);
		M.Signal();
		Print2Dos(1); // Print pump status to DOS

		EntrySem.Signal();
		rvPump.Wait();
		CStatus.Wait();
		newCustomer->newArrival = 0;
		newCustomer->Authorized = 0;
<<<<<<< HEAD
		for (int i = 0; i < 10000; i++) {
			if (State == 1) {
				if (newCustomer->CurrentGasLevel < newCustomer->MaxGasLevel) {
					newCustomer->CurrentGasLevel += 0.5;
					CurrentGasLevel = newCustomer->CurrentGasLevel;
					pTank->WithdrawFuel(0.5, newCustomer->FuelGrade);
					//pTank->PrintTankLevel(newCustomer->FuelGrade);
=======
		newCustomer->CreditCard = 1111;
		PStatus.Signal();
		while (1) {
			if (State == 1) {
				if (CurrentGasLevel < MaxGasLevel) {
					CurrentGasLevel += 0.5;
					CStatus.Wait();
					newCustomer->CurrentGasLevel = CurrentGasLevel;
					PStatus.Signal();
					pTank->WithdrawFuel(0.5, FuelGrade);
					//pTank->PrintTankLevel(newCustomer->FuelGrade);
					Bill = Price * CurrentGasLevel;
>>>>>>> susandev
					Print2Dos(0); // Print pump status to DOS
					SLEEP(1000);

				}
				else {
					Bill = Price * CurrentGasLevel;
					CStatus.Wait();
					newCustomer->Bill = Bill;
					newCustomer->Authorized = 0;
					newCustomer->newArrival = 0;
					PStatus.Signal();
					State = 0;
					Print2Dos(0); // Print pump status to DOS
					// Simulate Customer leaving pump
					M.Wait();
					MOVE_CURSOR(0, CursorY + 1);             // move cursor to cords [x,y]
					printf("\33[2K");
					printf("Finished filling. Returning hose to pump...\n");
					fflush(stdout);
					M.Signal();
					SLEEP(3000);
					M.Wait();
					MOVE_CURSOR(0, CursorY + 1);             // move cursor to cords [x,y]
					printf("\33[2K");
					printf("Customer %s leaving pump...\n", CustomerName.c_str());
					fflush(stdout);
					M.Signal();
					SLEEP(3000);
					Print2Dos(1); // Print pump status to DOS
					M.Wait();
					MOVE_CURSOR(0, CursorY + 1);             // move cursor to cords [x,y]
					printf("\33[2K");
					printf("Pump off.\n");
					fflush(stdout);
					M.Signal();
					SLEEP(3000);
				}
			}
			else {			
				if (pipe.TestForData()) {
<<<<<<< HEAD
					newCustomer->newArrival = 1;

=======

					// Read data from customer-pump pipeline
>>>>>>> susandev
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
<<<<<<< HEAD
					//printf("NEW CUSTOMER ARRIVED AT PUMP %s\n", PumpNumber.c_str());
					//newCustomer->newArrival = 1; 
					while (newCustomer->Authorized != 1){
						SLEEP(500);
					}
					//printf("NEW CUSTOMER AUTHORIZED AT PUMP %s\n", PumpNumber.c_str());
					SetFuelGrade(newCustomer->FuelGrade);
					FillGas(newCustomer->MaxGasLevel);
					Print2Dos(0); // Print pump status to DOS
=======
					CStatus.Wait();
					newCustomer->MaxGasLevel = MaxGasLevel;
					newCustomer->FuelGrade = FuelGrade;
					newCustomer->CreditCard = CreditCard;
					newCustomer->newArrival = 1;
					PStatus.Signal();
					// Simulate Customer arriving to pump
					M.Wait();
					MOVE_CURSOR(0, CursorY + 1);             // move cursor to cords [x,y]
					printf("\33[2K");
					printf("Customer %s arrived at pump...\n", CustomerName.c_str());
					fflush(stdout);
					M.Signal();
					SLEEP(3000);
					M.Wait();
					MOVE_CURSOR(0, CursorY + 1);             // move cursor to cords [x,y]
					printf("\33[2K");
					printf("Swiping credit card...\n");
					fflush(stdout);
					M.Signal();
					SLEEP(3000);

					while (newCustomer->Authorized != 1){
						SLEEP(500);
					}
>>>>>>> susandev

					M.Wait();
					MOVE_CURSOR(0, CursorY + 1);             // move cursor to cords [x,y]
					printf("\33[2K");
					printf("Card authorized...\n");
					fflush(stdout);
					M.Signal();
					SLEEP(1000);
					M.Wait();
					MOVE_CURSOR(0, CursorY + 1);             // move cursor to cords [x,y]
					printf("\33[2K");
					printf("Removing gas pump...\n");
					fflush(stdout);
					M.Signal();
					SLEEP(3000);

					// Set fuel grade and start dispensing
					SetFuelGrade(FuelGrade);
					FillGas(MaxGasLevel);
					M.Wait();
					MOVE_CURSOR(0, CursorY + 1);             // move cursor to cords [x,y]
					printf("\33[2K");
					printf("Selected fuel grade %d (%s)...\n", FuelGrade, FuelGradeType.c_str());
					fflush(stdout);
					M.Signal();
					SLEEP(3000);
					M.Wait();
					MOVE_CURSOR(0, CursorY + 1);             // move cursor to cords [x,y]
					printf("\33[2K");
					printf("Pump on. Dispensing Fuel...\n");
					fflush(stdout);
					M.Signal();
					Print2Dos(0); // Print pump status to DOS
				}
			}
		}

		return 0;
	}
};

#endif 