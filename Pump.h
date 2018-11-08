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
		//std::string PumpName = std::to_string(PumpNumber);

		CursorY = std::stoi(PumpNumber, nullptr, 10);
		CursorY = (6 * (CursorY - 1));
		Print2Dos(1); // Print pump status to DOS
		//Initialize pipes, semaphores 
		CMutex M("DOSMutex"); // mutex to protect DOS window
		CTypedPipe <string> pipe(PumpNumber, 4);
		CSemaphore EntrySem(("entry" + PumpNumber), 0, 1);
		CSemaphore ExitSem(("exit" + PumpNumber), 0, 1);
		CSemaphore Full(("full" + PumpNumber), 0, 1);
		CSemaphore Empty(("empty" + PumpNumber), 0, 1);
		EntrySem.Signal();

		CDataPool 		dp("dataPoolPump" + PumpNumber , sizeof(struct CustomerInfo));	// Create a datapool to communicate with gsc
		CRendezvous     rvPump("rvPump", 4);		// Attempt to create a rendezvous object involving 4 threads
		rvPump.Wait();

		struct CustomerInfo 	 *newCustomer = (struct CustomerInfo *)(dp.LinkDataPool());

		newCustomer->newArrival = 0;
		newCustomer->Authorized = 0;
		for (int i = 0; i < 10000; i++) {
			if (State == 1) {
				if (newCustomer->CurrentGasLevel < newCustomer->MaxGasLevel) {
					newCustomer->CurrentGasLevel += 0.5;
					CurrentGasLevel = newCustomer->CurrentGasLevel;
					pTank->WithdrawFuel(0.5, newCustomer->FuelGrade);
					//pTank->PrintTankLevel(newCustomer->FuelGrade);
					Print2Dos(0); // Print pump status to DOS
					SLEEP(1000);
				}
				else {
					newCustomer->Bill = Price * newCustomer->MaxGasLevel;
					newCustomer->Authorized = 0;
					newCustomer->newArrival = 0;
					State = 0;
				}
			}
			else {			
				if (pipe.TestForData()) {
					newCustomer->newArrival = 1;

					Full.Wait();
					pipe.Read(&GasStr);
					newCustomer->MaxGasLevel = std::stod(GasStr);
					MaxGasLevel = newCustomer->MaxGasLevel;
					pipe.Read(&FuelGradeStr);
					newCustomer->FuelGrade = std::stoi(FuelGradeStr, nullptr, 10);
					FuelGrade = newCustomer->FuelGrade;
					pipe.Read(&CreditCardStr);
					newCustomer->CreditCard = std::stoi(CreditCardStr, nullptr, 10);
					pipe.Read(&CustomerName);
					ExitSem.Signal();
					Empty.Wait();
					EntrySem.Signal();
					//printf("NEW CUSTOMER ARRIVED AT PUMP %s\n", PumpNumber.c_str());
					//newCustomer->newArrival = 1; 
					while (newCustomer->Authorized != 1){
						SLEEP(500);
					}
					//printf("NEW CUSTOMER AUTHORIZED AT PUMP %s\n", PumpNumber.c_str());
					SetFuelGrade(newCustomer->FuelGrade);
					FillGas(newCustomer->MaxGasLevel);
					Print2Dos(0); // Print pump status to DOS

				}
			}
		}

		return 0;
	}
};

#endif 