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
	std::string PumpName = std::to_string(PumpNumber);
	/*std::string datapoolPref = "datapoolPump";
	std::string dataPoolName = datapoolPref + PumpName;
	cout << "Resultant String = " << dataPoolName;
	*/
	char buffer[128];
	std::string datapoolPref = "datapoolPump";
	snprintf(buffer, sizeof(buffer), "%s%d", datapoolPref.c_str(), PumpNumber);

	CPipe	pipe(PumpName, 1024);			// Create a pipe to communicate with customer
	printf("%.*s", 13, buffer);

	//printf("attempting to create datapool %s", dataPoolName);
	CDataPool 		dp(buffer, sizeof(struct CustomerInfo));	// Create a datapool to communicate with gsc
	printf("Child Thread Created to Communicate with Pump Number: %d\n", PumpNumber);
	/*
	CDataPool 		dp("pum", sizeof(struct CustomerInfo));	// Create a datapool to communicate with gsc
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
	}*/
	struct CustomerInfo 	 *pumpDataPool = (struct CustomerInfo *)(dp.LinkDataPool());
	printf("GSC linked to datapool %d at address %p.....\n", PumpNumber, pumpDataPool);

	printf("Child Read value %d for New Arrival From Pump Number %d\n", pumpDataPool->newArrival, PumpNumber);

	while (pumpDataPool->newArrival == 0) {
		SLEEP(500);
	}
	if (pumpDataPool->newArrival == 1)
	{
		printf("Child Read value %d for Credit Card From Pump Number %d\n", pumpDataPool->CreditCard, PumpNumber);
		printf("Child Read value %d for FuelGrade Card From Pump Number %d\n", pumpDataPool->FuelGrade, PumpNumber);

		if (pumpDataPool->MaxGasLevel < 70) {
			pumpDataPool->Authorized = 1;
		}
	}
	while (pumpDataPool->newArrival == 1)
	{
		printf("Pump %d is currently filling. Gas Level is at %f litres \n", PumpNumber, pumpDataPool->CurrentGasLevel);
		SLEEP(3000);
	}

	// The CDatapool object 'dp' created at the start of the program will now be destroyed
	// and provided there are no other processes using the same named datapool, then that datapool
	// will also be destroyed

	//getchar();			// pause seconds so programmer can view output before child dies

	return 0;
}						// end of structure template

int   main()
{
	int i ;

// 	Attempt to make the datapool 'Car1'. This process will not know
//	whether the datapool exists or not yet (we know that it does because we only
//	set the process running after the parent program had created it and stored data
//	in it) but that shouldn't matter, since the program can just attempt to make it. If it already exists
//	then it will simply use it. If it doesn't exist, then it will be created below


	printf("Child attempting to create/use the datapool.....\n") ;
	CDataPool 		pump1dp("pump1", sizeof(struct CustomerInfo)) ;
	CDataPool 		pump2dp("pump1", sizeof(struct CustomerInfo));
	CDataPool 		pump3dp("pump1", sizeof(struct CustomerInfo));
	CDataPool 		pump4dp("pump1", sizeof(struct CustomerInfo));

	int Num[4] = { 1,2,3,4 };
	CThread *Threads[4];
	for (int i = 0; i <= 3; i++) {
		Threads[i] = new CThread(PumpComm, ACTIVE, &Num[i]);
	}

	
	getchar();			// pause seconds so programmer can view output before child dies

	return 0 ;
}