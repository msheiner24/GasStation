//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include 	<stdio.h>
#include	<string>
#include 	"..\\rt.h"		// change pathlist to "rt.h" file to suit location on disk

  
struct CustomerInfo {
	double CurrentGasLevel = 0.0;
	double MaxGasLevel = 0;
	double Bill = 0.0;
	int FuelGrade = 87;
	int CreditCard = 0;
	bool Authorized = 0;
	bool newArrival = 0;
};

UINT __stdcall PumpComm(void *args)
{
	int PumpNumber = *(int *)(args);
	int key;
	std::string PumpName = std::to_string(PumpNumber);

	CDataPool 		dp("dataPoolPump" + PumpName , sizeof(struct CustomerInfo));	// Create a datapool to communicate with gsc
	struct CustomerInfo 	 *pumpDataPool = (struct CustomerInfo *)(dp.LinkDataPool());
	// initialize local variables
	double CurrentGasLevel = 0.0;
	double MaxGasLevel = 0;
	double Bill = 0.0;
	int FuelGrade = 87;
	int CreditCard = 0;
	bool Authorized = 0;
	bool newArrival = 0;

	while (1) {
		if (pumpDataPool->newArrival == 1)
		{
			printf("New Customer Arrived at Pump %d", PumpNumber);
			printf("Credit Card: From Pump Number %d\n", pumpDataPool->CreditCard, PumpNumber);
			printf("Child Read value %d for FuelGrade Card From Pump Number %d\n", pumpDataPool->FuelGrade, PumpNumber);

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
				printf("authorized pump %d", PumpNumber);
				pumpDataPool->Authorized = 1;
			}
		}
		while (pumpDataPool->newArrival == 1)
		{
			printf("Pump %d is currently filling. Gas Level is at %f litres \n", PumpNumber, pumpDataPool->CurrentGasLevel);
			SLEEP(3000);
		}
	}
	return 0;
}					

int   main()
{
	int i ;
	int Num[4] = { 1,2,3,4 };
	CThread *Threads[4];

	for (int i = 0; i <= 3; i++) {
		Threads[i] = new CThread(PumpComm, ACTIVE, &Num[i]);
	}
	
	getchar();			// pause seconds so programmer can view output before child dies

	return 0 ;
}