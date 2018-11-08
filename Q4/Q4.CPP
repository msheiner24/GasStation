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
};

struct TankInfo {

};
UINT __stdcall PumpComm(void *args)
{
	int PumpNumber = *(int *)(args);
	int CursorY = (6 * (PumpNumber - 1)); // calculate Y cursor value
	string PumpName = std::to_string(PumpNumber);
	// initialize local variables
	CDataPool 		dp("dataPoolPump" + PumpName, sizeof(struct CustomerInfo));	// Create a datapool to communicate with gsc
	struct CustomerInfo 	 *pumpDataPool = (struct CustomerInfo *)(dp.LinkDataPool());
	CMutex M("DOSMutex2"); // mutex to protect DOS window
	double CurrentGasLevel = 0.0;
	double MaxGasLevel = 0;
	double Bill = 0.0;
	int FuelGrade = 87;
	int CreditCard = 0;
	bool Authorized = 0;
	bool newArrival = 0;
	bool print = 1;
	// Print pump name to DOS
	M.Wait();
	MOVE_CURSOR(0, CursorY);             // move cursor to cords [x,y]
	printf("Pump %d", PumpNumber);
	fflush(stdout);		      	// force output to be written to screen
	M.Signal();
	while (1) {
		while (pumpDataPool->newArrival == 0) {
			if (print) {
				// Print default pump info
				M.Wait();
				MOVE_CURSOR(0, CursorY + 1);             // move cursor to cords [x,y]
				printf("\33[2K");
				printf("\rPump Status: Off  \n");
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
		}

		if (pumpDataPool->newArrival == 1)
		{
			//CustomerName = pumpDataPool->CustomerName;
			M.Wait();
			MOVE_CURSOR(0, 25);             // move cursor to cords [x,y]
			printf("Customer w/ Credit card %d waiting for authorization at Pump %d. Authorize? (0/1)", pumpDataPool->CreditCard, PumpNumber);
			fflush(stdout);		      	// force output to be written to screen
			MOVE_CURSOR(0, 26);             // move cursor to cords [x,y]
			cin >> Authorized;
			printf("    \n");
			fflush(stdout);
			M.Signal();
			if (Authorized) {
				pumpDataPool->Authorized = 1;
				pumpDataPool->customerRecieved = 1;
				SLEEP(2000);
				while (pumpDataPool->PumpState) {
					M.Wait();
					MOVE_CURSOR(0, CursorY + 1);             // move cursor to cords [x,y]
					printf("\33[2K");
					printf("\rPump Status: On  \n");
					fflush(stdout);		      	// force output to be written to screen
					MOVE_CURSOR(0, CursorY + 2);             // move cursor to cords [x,y]
					printf("\33[2K");
					printf("\rCredit Card: %d\n", pumpDataPool->CreditCard);
					fflush(stdout);		      	// force output to be written to screen
					MOVE_CURSOR(0, CursorY + 3);             // move cursor to cords [x,y]
					printf("\33[2K");
					printf("\rPrice: $%.2f/litre\n", pumpDataPool->Price);
					fflush(stdout);		      	// force output to be written to screen
					MOVE_CURSOR(0, CursorY + 4);             // move cursor to cords [x,y]
					printf("\33[2K");
					printf("\rFuel Grade: %d\n", pumpDataPool->FuelGrade);
					fflush(stdout);		      	// force output to be written to screen
					MOVE_CURSOR(40, CursorY + 2);             // move cursor to cords [x,y]
					printf("Fill Level: %.2f Litres\n", pumpDataPool->CurrentGasLevel);
					fflush(stdout);		      	// force output to be written to screen
					MOVE_CURSOR(40, CursorY + 3);             // move cursor to cords [x,y]
					printf("Bill: $%.2f\n", pumpDataPool->Bill);
					fflush(stdout);		      	// force output to be written to screen
					M.Signal();
					SLEEP(500);
				}
				print = 1;
			}
			else {
				pumpDataPool->Authorized = 0;
				pumpDataPool->customerRecieved = 1;
			}
			pumpDataPool->newArrival = 0;
		}
	}
	return 0;
}						

UINT __stdcall TankComm(void *args)
{
	int TankNumber = *(int *)(args);
	std::string TankName = std::to_string(TankNumber);

	CDataPool 		dp2("dataPoolTank"+TankName , sizeof(struct tankDataPool));	// Create a datapool to communicate with gsc
	struct tankDataPool 	 *tankDataPool = (struct tankDataPool *)(dp2.LinkDataPool());
	CMutex M("DOSMutex2");
	int CursorY;
	//CursorY = std::stoi(TankName, nullptr, 10);
	CursorY = TankNumber;
	CursorY = (6 * (CursorY - 1));

	//printf("Fuel grade 87 remaining at tank #%d:   %f\n", TankNumber, tankDataPool->tankLevel87);
	//printf("Fuel grade 89 remaining at tank #%d:   %f\n", TankNumber, tankDataPool->tankLevel89);
	//CMutex M("DOSMutex2"); // mutex to protect DOS window
	//while (1) {
	//	printf("Fuel grade 87 remaining at tank #%d:   %f\n",TankNumber, tankDataPool->tankLevel87);
	//	printf("Fuel grade 89 remaining at tank #%d:   %f\n", TankNumber, tankDataPool->tankLevel89);
	//	
	//	SLEEP(1000);
	//}

	// Print pump name to DOS
	//M.Wait();
	//MOVE_CURSOR(0, CursorY);             // move cursor to cords [x,y]
	//printf("Pump %s", TankName.c_str());
	//fflush(stdout);		      	// force output to be written to screen
	//if (pump_state) {
	//	MOVE_CURSOR(0, CursorY + 1);             // move cursor to cords [x,y]
	//	printf("Tank Number:\n");
	//	fflush(stdout);		      	// force output to be written to screen
	//	MOVE_CURSOR(0, CursorY + 2);             // move cursor to cords [x,y]
	//	printf("Regular Unleaded Fuel Remaining (87):\n");
	//	fflush(stdout);		      	// force output to be written to screen
	//	MOVE_CURSOR(0, CursorY + 3);             // move cursor to cords [x,y]
	//	printf("Mid-Grade Unleaded Fuel Remaining (87):\n");
	//	fflush(stdout);		      	// force output to be written to screen
	//	MOVE_CURSOR(0, CursorY + 4);             // move cursor to cords [x,y]
	//	printf("Premium Unleaded Fuel Remaining (87):\n");
	//	fflush(stdout);		      	// force output to be written to screen
	//	MOVE_CURSOR(0, CursorY + 5);             // move cursor to cords [x,y]
	//	printf("Super Premium Unleaded Fuel Remaining:\n");
	//	fflush(stdout);		      	// force output to be written to screen
	//
	//}
	//while(1) {
	//	MOVE_CURSOR(0, CursorY + 1);             // move cursor to cords [x,y]
	//	printf("Tank Number: %s\n", TankName.c_str());
	//	fflush(stdout);		      	// force output to be written to screen
	//	MOVE_CURSOR(0, CursorY + 2);             // move cursor to cords [x,y]
	//	printf("Regular Unleaded Fuel Remaining (87): %f\n", tankInfo->tankLevel87);
	//	fflush(stdout);		      	// force output to be written to screen
	//	MOVE_CURSOR(0, CursorY + 3);             // move cursor to cords [x,y]
	//	printf("Mid - Grade Unleaded Fuel Remaining(89): %f\n", tankInfo->tankLevel89);
	//	fflush(stdout);		      	// force output to be written to screen
	//	MOVE_CURSOR(0, CursorY + 4);             // move cursor to cords [x,y]
	//	printf("Premium Unleaded Fuel Remaining (91): %f\n", tankInfo->tankLevel91);
	//	fflush(stdout);		      	// force output to be written to screen
	//	MOVE_CURSOR(0, CursorY + 5);             // move cursor to cords [x,y]
	//	printf("Super Premium Unleaded Fuel Remaining (93): %f\n", tankInfo->tankLevel93);
	//	fflush(stdout);		      	// force output to be written to screen
	//	MOVE_CURSOR(0, CursorY + 6);             // move cursor to cords [x,y]
	//	if (State)
	//		printf("Pump Status: Filling\n");
	//	else
	//		printf("Pump Status: Off    \n");
	//	fflush(stdout);		      	// force output to be written to screen
	//	MOVE_CURSOR(40, CursorY + 3);             // move cursor to cords [x,y]
	//	printf("Fill Level: %.2f Litres\n", CurrentGasLevel);
	//	fflush(stdout);		      	// force output to be written to screen
	//	MOVE_CURSOR(40, CursorY + 4);             // move cursor to cords [x,y]
	//	printf("Bill: $%.2f\n", Bill);
	//	fflush(stdout);		      	// force output to be written to screen
	//}
	//M.Signal();
	return 0;
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