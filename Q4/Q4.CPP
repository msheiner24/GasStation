//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#pragma once
#include 	<stdio.h>
#include	<string>
#include 	"..\\rt.h"		// change pathlist to "rt.h" file to suit location on disk
using namespace std;

//#include	"..\\FuelTank.h"

//
//	It's important to realise that all processes accessing the same datapool must
//	describe exactly the same datapool or structure template otherwise corruption
//	of data will occur. Ideally, the structure template would be put into a single header file
//	which would be '#include'd' into each program that used that datapool. However, for
//	simplicity, we have recreated the same template in each process
//

struct CustomerInfo {
	double CurrentGasLevel = 0.0;
	double MaxGasLevel = 0;
	double Bill = 0.0;
	double Price = 0.0;
	int FuelGrade = 87;
	int CreditCard = 0;
	bool Authorized = 0;
	bool newArrival = 0;
	bool customerRecieved = 0;
	bool PumpState = 0;
};

struct tankDataPool {
	int tankNumber;
	double tankLevel87;
	double tankLevel89;
	double tankLevel91;
	double tankLevel93;
	bool inUse;
	bool filling;
};

struct TankInfo {

};
UINT __stdcall PumpComm(void *args)
{
	int PumpNumber = *(int *)(args);
	int CursorY = (6 * (PumpNumber - 1)); // calculate Y cursor value
	string PumpName = std::to_string(PumpNumber);
	// initialize local variables
	CSemaphore PStatus(("Producer" + PumpName), 0);
	CSemaphore CStatus(("Consumer" + PumpName), 1);
	CDataPool 		dp("dataPoolPump" + PumpName, sizeof(struct CustomerInfo));	// Create a datapool to communicate with gsc
	struct CustomerInfo 	 *pumpDataPool = (struct CustomerInfo *)(dp.LinkDataPool());
	CMutex M("DOSMutex2"); // mutex to protect DOS window
	double CurrentGasLevel = 0.0;
	double MaxGasLevel = 0;
	double Bill = 0.0;
	double price = 0.0;
	int FuelGrade = 87;
	int CreditCard = 0;
	bool Authorized = 0;
	bool newArrival = 0;
	bool print = 1;
	bool pumpState = 0;
	// Print pump name to DOS
	M.Wait();
	MOVE_CURSOR(0, CursorY);             // move cursor to cords [x,y]
	printf("Pump %d", PumpNumber);
	fflush(stdout);		      	// force output to be written to screen
	M.Signal();
	while (1) {
		//PStatus.Wait();
		newArrival = pumpDataPool->newArrival;
		//CStatus.Signal();
		while (newArrival == 0) {
			if (print) {
				// Print default pump info
				M.Wait();
				MOVE_CURSOR(0, CursorY + 1);             // move cursor to cords [x,y]
				printf("\33[2K");
				printf("\rPump Status: Off                                                                                         \n");
				fflush(stdout);		      	// force output to be written to screen
				MOVE_CURSOR(0, CursorY + 2);             // move cursor to cords [x,y]
				printf("\33[2K");
				printf("\rCredit Card:\n");
				fflush(stdout);		      	// force output to be written to screen
				MOVE_CURSOR(0, CursorY + 3);             // move cursor to cords [x,y]
				printf("\33[2K");
				printf("\rPrice:\n");
				fflush(stdout);		      	// force output to be written to screen
				MOVE_CURSOR(0, CursorY + 4);             // move cursor to cords [x,y]
				printf("\33[2K");
				printf("\rFuel Grade:\n");
				fflush(stdout);		      	// force output to be written to screen
				MOVE_CURSOR(40, CursorY + 2);             // move cursor to cords [x,y]
				printf("Fill Level:\n");
				fflush(stdout);		      	// force output to be written to screen
				MOVE_CURSOR(40, CursorY + 3);             // move cursor to cords [x,y]
				printf("Bill:\n");
				fflush(stdout);		      	// force output to be written to screen
				M.Signal();
				print = 0;
			}
			SLEEP(2000);
			//PStatus.Wait();
			newArrival = pumpDataPool->newArrival;
			//CStatus.Signal();
		}

		if (newArrival == 1)
		{
			M.Wait();
			MOVE_CURSOR(0, 25);             // move cursor to cords [x,y]
			printf("Customer w/ Credit card %d waiting for authorization at Pump %d. Authorize? (0/1) ", pumpDataPool->CreditCard, PumpNumber);
			fflush(stdout);		      	// force output to be written to screen
			cin >> Authorized;
			MOVE_CURSOR(0, 25);             // move cursor to cords [x,y]
			printf("                                                                                                         \n");
			fflush(stdout);
			M.Signal();

			//M.Wait();
			//MOVE_CURSOR(0, CursorY + 1);             // move cursor to cords [x,y]
			//printf("\33[2K");
			//printf("\rCustomer w/ Credit card %d waiting for authorization at Pump %d. Authorize? (0/1) ", pumpDataPool->CreditCard, PumpNumber);
			//fflush(stdout);		      	// force output to be written to screen
			//M.Signal();
			//cin >> Authorized;
			//M.Signal();

			if (Authorized) {
				//CStatus.Wait();
				pumpDataPool->Authorized = 1;
				pumpDataPool->customerRecieved = 1;
				//PStatus.Signal();
				SLEEP(2000);
				//PStatus.Wait();
				pumpState = pumpDataPool->PumpState;
				//CStatus.Signal();
				while (pumpState) {
					//PStatus.Wait();
					pumpState = pumpDataPool->PumpState;
					CreditCard = pumpDataPool->CreditCard;
					price = pumpDataPool->Price;
					FuelGrade = pumpDataPool->FuelGrade;
					CurrentGasLevel = pumpDataPool->CurrentGasLevel;
					Bill = pumpDataPool->Bill;
					//CStatus.Signal();
					M.Wait();
					MOVE_CURSOR(0, CursorY + 1);             // move cursor to cords [x,y]
					printf("\33[2K");
					printf("\rPump Status: On                                                                                                   \n");
					fflush(stdout);		      	// force output to be written to screen
					MOVE_CURSOR(0, CursorY + 2);             // move cursor to cords [x,y]
					printf("\33[2K");
					printf("\rCredit Card: %d\n", CreditCard);
					fflush(stdout);		      	// force output to be written to screen
					MOVE_CURSOR(0, CursorY + 3);             // move cursor to cords [x,y]
					printf("\33[2K");
					printf("\rPrice: $%.2f/litre\n", price);
					fflush(stdout);		      	// force output to be written to screen
					MOVE_CURSOR(0, CursorY + 4);             // move cursor to cords [x,y]
					printf("\33[2K");
					printf("\rFuel Grade: %d\n", FuelGrade);
					fflush(stdout);		      	// force output to be written to screen
					MOVE_CURSOR(40, CursorY + 2);             // move cursor to cords [x,y]
					printf("Fill Level: %.2f Litres\n", CurrentGasLevel);
					fflush(stdout);		      	// force output to be written to screen
					MOVE_CURSOR(40, CursorY + 3);             // move cursor to cords [x,y]
					printf("Bill: $%.2f\n", Bill);
					fflush(stdout);		      	// force output to be written to screen
					M.Signal();
					SLEEP(500);
				}
				print = 1;
			}
			else {
				//CStatus.Wait();
				pumpDataPool->Authorized = 0;
				pumpDataPool->customerRecieved = 1;
				//PStatus.Signal();
			}
			//CStatus.Wait();
			pumpDataPool->newArrival = 0;
			//PStatus.Signal();
		}
	}
	return 0;
}						

UINT __stdcall TankComm(void *args)
{
	int TankNumber = *(int *)(args);
	std::string TankName = std::to_string(TankNumber);

	CDataPool 		dp2("dataPoolTank" + TankName, sizeof(struct tankDataPool));	// Create a datapool to communicate with gsc
	struct tankDataPool 	 *tankDataPool = (struct tankDataPool *)(dp2.LinkDataPool());
	CMutex M("DOSMutex2");
	CMutex F("MyFuelTank");
	//CursorY = std::stoi(TankName, nullptr, 10);
	int CursorY = (6 * (TankNumber - 1)) + 30; // calculate Y cursor value
	M.Wait();
	MOVE_CURSOR(0, CursorY);             // move cursor to cords [x,y]
	printf("Tank %d", TankNumber);
	fflush(stdout);		      	// force output to be written to screen
	M.Signal();
	char fill;
	while (1) {
		// Print default pump info
		M.Wait();
		MOVE_CURSOR(0, CursorY + 1);             // move cursor to cords [x,y]
		printf("\33[2K");
		printf("\rREGULAR UNLEADED: %f\n", tankDataPool->tankLevel87);
		fflush(stdout);		      	// force output to be written to screen
		MOVE_CURSOR(0, CursorY + 2);             // move cursor to cords [x,y]
		printf("\33[2K");
		printf("\rMID-GRADE UNLEADED: %f\n", tankDataPool->tankLevel89);
		fflush(stdout);		      	// force output to be written to screen
		MOVE_CURSOR(0, CursorY + 3);             // move cursor to cords [x,y]
		printf("\33[2K");
		printf("\rPREMIUM UNLEADED: %f\n", tankDataPool->tankLevel91);
		fflush(stdout);		      	// force output to be written to screen
		MOVE_CURSOR(0, CursorY + 4);             // move cursor to cords [x,y]
		printf("\33[2K");
		printf("\rSUPER PREMIUM UNLEADED: %f\n", tankDataPool->tankLevel93);
		fflush(stdout);		      	// force output to be written to screen
		SLEEP(800);
		if (tankDataPool->tankLevel87 < 200)
		{
			MOVE_CURSOR(0, 55);             // move cursor to cords [x,y]
			printf("\33[2K");
			printf("\rWARNING! FUELTANK %d IS BELOW 200L of 87, fill after current customer", TankNumber);

			while (tankDataPool->inUse)
			{
				SLEEP(500);
			}
			F.Wait();
			tankDataPool->filling = 1;
			printf("\rFuel Tank  Not In Use, press f to fill:										 ");
			cin >> fill;
			if (fill == 'f') {
				tankDataPool->tankLevel87 = 500;
				SLEEP(1000);
				tankDataPool->filling = 0;
				F.Signal();
				fflush(stdout);		      	// force output to be written to screen
				MOVE_CURSOR(0, 55);             // move cursor to cords [x,y]
				printf("\r															");
				fflush(stdout);		      	// force output to be written to screen
			}
		}
		else if (tankDataPool->tankLevel89 < 200) {
			MOVE_CURSOR(0, 56);             // move cursor to cords [x,y]
			printf("\33[2K");
			printf("\rWARNING! FUELTANK %d IS BELOW 200L of 87, fill after current customer", TankNumber);

			while (tankDataPool->inUse)
			{
				SLEEP(500);
			}
			F.Wait();
			tankDataPool->filling = 1;
			printf("\r															\n");
			fflush(stdout);		      	// force output to be written to screen
			printf("Fuel Tank Not In Use, press f to fill:     ");
			cin >> fill;
			if (fill == 'f') {
				tankDataPool->tankLevel87 = 500;
				SLEEP(1000);
				tankDataPool->filling = 0;
				F.Signal();
				MOVE_CURSOR(0, 56);
				fflush(stdout);		      	// force output to be written to screen
				printf("\33[2K");
				printf("%c[2K", 27);
			}
		}
		else if (tankDataPool->tankLevel91 < 200) {
			MOVE_CURSOR(0, 57);             // move cursor to cords [x,y]
			printf("\33[2K");
			printf("\rWARNING! FUELTANK %d IS BELOW 200L of 87, fill after current customer", TankNumber);
			while (tankDataPool->inUse)
			{
				SLEEP(500);
			}
			F.Wait();
			tankDataPool->filling = 1;
			tankDataPool->tankLevel87 = 500;
			tankDataPool->filling = 0;
			F.Signal();
			SLEEP(1000);
			fflush(stdout);		      	// force output to be written to screen
			printf("\33[2K");
			printf("\r															\n");
			fflush(stdout);		      	// force output to be written to screen
		}
		else if (tankDataPool->tankLevel93 < 200) {
			MOVE_CURSOR(0, 58);             // move cursor to cords [x,y]
			printf("\33[2K");
			printf("\rWARNING! FUELTANK %d IS BELOW 200L of 87, fill after current customer", TankNumber);

			while (tankDataPool->inUse)
			{
				SLEEP(500);
			}
			F.Wait();
			tankDataPool->filling = 1;
			tankDataPool->tankLevel87 = 500;
			SLEEP(1000);
			tankDataPool->filling = 0;
			F.Signal();
			fflush(stdout);		      	// force output to be written to screen
			printf("\33[2K");
			printf("\r															\n");
			fflush(stdout);		      	// force output to be written to screen
		}
		M.Signal();

	}
	}

int   main()
{
	int i ;
	int Num[4] = { 1,2,3,4 };

	CThread *PumpThreads[4];
	CThread *TankThreads[4];
	for (int i = 0; i <= 3; i++) {
		PumpThreads[i] = new CThread(PumpComm, ACTIVE, &Num[i]);
	}

	for (int i = 0; i <= 3; i++) {
		TankThreads[i] = new CThread(TankComm, ACTIVE, &Num[i]);
	}
	
	while (1) {
		int x = 1;
	}
	//getchar();			// pause seconds so programmer can view output before child dies

	return 0 ;
}