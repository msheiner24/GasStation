//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	This example Pocess makes/uses a datapool and reads data out
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include 	<stdio.h>
#include	<string>
#include 	"Z:\RTExamples\rt.h"		// change pathlist to "rt.h" file to suit location on disk


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

UINT __stdcall PumpComm(void *args)
{
	int PumpNumber = *(int *)(args);
	int key;
	std::string PumpName = std::to_string(PumpNumber);

	CDataPool 		dp("dataPoolPump" + PumpName , sizeof(struct CustomerInfo));	// Create a datapool to communicate with gsc
	struct CustomerInfo 	 *pumpDataPool = (struct CustomerInfo *)(dp.LinkDataPool());

	printf("Thread created to communicate with Pump %d", PumpNumber);
	printf("reading credit card number : %d", pumpDataPool->CreditCard);
	while (pumpDataPool->newArrival == 0) {
		printf("new arrival at pump %d:   %d", PumpNumber, pumpDataPool->newArrival);

		SLEEP(2000);
	}

	if (pumpDataPool->newArrival == 1)
	{
		printf("New Customer Arrived at Pump %d", PumpNumber);
		printf("Child Read value %d for Credit Card From Pump Number %d\n", pumpDataPool->CreditCard, PumpNumber);
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


	//getchar();			// pause seconds so programmer can view output before child dies

	return 0;
}						// end of structure template

//UINT __stdcall TankComm(void *args)
//{
//
//}


int   main()
{
	int i ;

// 	Attempt to make the datapool 'Car1'. This process will not know
//	whether the datapool exists or not yet (we know that it does because we only
//	set the process running after the parent program had created it and stored data
//	in it) but that shouldn't matter, since the program can just attempt to make it. If it already exists
//	then it will simply use it. If it doesn't exist, then it will be created below


	int Num[4] = { 1,2,3,4 };
	CThread *Threads[4];
	for (int i = 0; i <= 3; i++) {
		Threads[i] = new CThread(PumpComm, ACTIVE, &Num[i]);
	}
	//CThread	TankThread(TankComm,			// name of function representing the thread
	//	ACTIVE,				// thread is created in ACTIVE state (could use SUSPENDED)
	//	NULL					// Pass no arguments to the thread function
	//);


	
	getchar();			// pause seconds so programmer can view output before child dies

	return 0 ;
}