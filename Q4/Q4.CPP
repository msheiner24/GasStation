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
	int FuelGrade = 87;
	int CreditCard = 0;
	bool Authorized = 0;
	bool newArrival = 0;
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
	int key;
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

	// Print pump name to DOS
	M.Wait();
	MOVE_CURSOR(0, CursorY);             // move cursor to cords [x,y]
	printf("Pump %d", PumpNumber);
	fflush(stdout);		      	// force output to be written to screen
	M.Signal();

	while (pumpDataPool->newArrival == 0) {
		// Print default pump info
		M.Wait();
		MOVE_CURSOR(0, CursorY + 1);             // move cursor to cords [x,y]
		printf("\33[2K");
		printf("\rPump Status: Off\n");
		fflush(stdout);		      	// force output to be written to screen
		MOVE_CURSOR(0, CursorY + 2);             // move cursor to cords [x,y]
		printf("\33[2K");
		printf("\rCustomer Name:\n");
		fflush(stdout);		      	// force output to be written to screen
		MOVE_CURSOR(0, CursorY + 3);             // move cursor to cords [x,y]
		printf("\33[2K");
		printf("\rCredit Card:\n");
		fflush(stdout);		      	// force output to be written to screen
		MOVE_CURSOR(0, CursorY + 4);             // move cursor to cords [x,y]
		printf("\33[2K");
		printf("\rPrice:\n");
		fflush(stdout);		      	// force output to be written to screen
		MOVE_CURSOR(40, CursorY + 2);             // move cursor to cords [x,y]
		printf("Fuel Grade:\n");
		fflush(stdout);		      	// force output to be written to screen
		MOVE_CURSOR(40, CursorY + 3);             // move cursor to cords [x,y]
		printf("Fill Level:\n");
		fflush(stdout);		      	// force output to be written to screen
		MOVE_CURSOR(40, CursorY + 4);             // move cursor to cords [x,y]
		printf("Bill:\n");
		fflush(stdout);		      	// force output to be written to screen
		M.Signal();
		SLEEP(2000);
	}

	if (pumpDataPool->newArrival == 1)
	{
		/*printf("New Customer Arrived at Pump %d", PumpNumber);
		printf("Child Read value %d for Credit Card From Pump Number %d\n", pumpDataPool->CreditCard, PumpNumber);
		printf("Child Read value %d for FuelGrade Card From Pump Number %d\n", pumpDataPool->FuelGrade, PumpNumber);
*/

		// Print pump name to DOS
		//M.Wait();
		//MOVE_CURSOR(0, CursorY);             // move cursor to cords [x,y]
		//printf("Pump %d", PumpNumber);
		//fflush(stdout);		      	// force output to be written to screen
		//if (pump_state) {
		//	move_cursor(0, cursory + 2);             // move cursor to cords [x,y]
		//	printf("\33[2k");
		//	printf("customer name:\n");
		//	fflush(stdout);		      	// force output to be written to screen
		//	move_cursor(0, cursory + 3);             // move cursor to cords [x,y]
		//	printf("\33[2k");
		//	printf("credit card:\n");
		//	fflush(stdout);		      	// force output to be written to screen
		//	move_cursor(0, cursory + 4);             // move cursor to cords [x,y]
		//	printf("\33[2k");
		//	printf("price:\n");
		//	fflush(stdout);		      	// force output to be written to screen
		//	move_cursor(40, cursory + 2);             // move cursor to cords [x,y]
		//	printf("fuel grade:\n");
		//	fflush(stdout);		      	// force output to be written to screen
		//	/*
		//	move_cursor(40, cursory + 3);             // move cursor to cords [x,y]
		//	printf("pump status : off\n");
		//	fflush(stdout);		      	// force output to be written to screen
		//	*/
		//	move_cursor(40, cursory + 3);             // move cursor to cords [x,y]
		//	printf("fill level:\n");
		//	fflush(stdout);		      	// force output to be written to screen
		//	move_cursor(40, cursory + 4);             // move cursor to cords [x,y]
		//	printf("bill:\n");
		//	fflush(stdout);		      	// force output to be written to screen
		//}
		//else {
		//	move_cursor(0, cursory + 2);             // move cursor to cords [x,y]
		//	printf("\33[2k");
		//	printf("customer name: %s\n", customername.c_str());
		//	fflush(stdout);		      	// force output to be written to screen
		//	move_cursor(0, cursory + 3);             // move cursor to cords [x,y]
		//	printf("\33[2k");
		//	printf("credit card: %d\n", creditcard);
		//	fflush(stdout);		      	// force output to be written to screen
		//	move_cursor(0, cursory + 4);             // move cursor to cords [x,y]
		//	printf("\33[2k");
		//	printf("price: $%.2f/litre\n", price);
		//	fflush(stdout);		      	// force output to be written to screen
		//	move_cursor(40, cursory + 2);             // move cursor to cords [x,y]
		//	printf("fuel grade: %d (%s)\n", fuelgrade, fuelgradetype.c_str());
		//	fflush(stdout);		      	// force output to be written to screen
		//	/*
		//	move_cursor(40, cursory + 3);             // move cursor to cords [x,y]
		//	if (state)
		//		printf("pump status: filling\n");
		//	else
		//		printf("pump status: off    \n");
		//	*/
		//	fflush(stdout);		      	// force output to be written to screen
		//	move_cursor(40, cursory + 3);             // move cursor to cords [x,y]
		//	printf("fill level: %.2f litres\n", currentgaslevel);
		//	fflush(stdout);		      	// force output to be written to screen
		//	move_cursor(40, cursory + 4);             // move cursor to cords [x,y]
		//	printf("bill: $%.2f\n", bill);
		//	fflush(stdout);		      	// force output to be written to screen
		//}
		//M.Signal();



		if (pumpDataPool->MaxGasLevel < 70) {
			while (GetAsyncKeyState(0x46) == 0)
			{
				SLEEP(500);
			}
			if (PumpNumber == 1) {
				key = 0x31;
			}
			else if (PumpNumber == 2) {
				key = 0x32;
			}
			else if (PumpNumber == 3) {
				key = 0x33;
			}
			else {
				key = 0x34;
			}
			while (GetAsyncKeyState(key) == 0)
			{
				SLEEP(500);
			}
			//printf("authorized pump %d", PumpNumber);
			pumpDataPool->Authorized = 1;
		}
	}


	//getchar();			// pause seconds so programmer can view output before child dies

	return 0;
}						// end of structure template

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
	
	getchar();			// pause seconds so programmer can view output before child dies

	return 0 ;
}