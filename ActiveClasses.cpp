#include <stdio.h>
#include "C:\RTExamples\rt.h"

//
//	To create an active class with its own thread running through it. Derive a new class
//	from the base class 'ActiveClass' and then you MUST override the inherited base class function int main(void)
//	if you do not override main() with your own function it will not compile since the class will be abstract
//
//	Note that all active classes are created in the SUSPENDED state to avoid races between the constructor for the class
//	and the kernel which otherwise might try to run a thread through an incompleted constructed class object
//
//	Therefore you must forcible Resume() the class to allow its thread to run.
//


class MyActiveClass1 : public ActiveClass 
{

	// put any attributes and member functions here that you like 
	// just like any other class along with any constructors and destructors

private:

	int MyNumber ;
	
public:
	MyActiveClass1(int _MyNumber) { MyNumber = _MyNumber; }

	//  Must override main() inherited from ActiveClass. The base class constructor will then
	//	create a thread execute main()

	int main(void)	
	{
		for(int i = 0; i < 100; i ++)	{
			printf("I am an ActiveClass1 Object/Thread. My Data is %d.....\n", MyNumber) ;
			SLEEP(400) ;
		}
		
		return 0 ;
	}
} ;

class MyActiveClass2 : public ActiveClass 
{

	// put any attributes and member functions here that you like 
	// just like any other class along with any constructors and destructors

	int MyNumber ;

public:
	MyActiveClass2(int _MyNumber) { MyNumber = _MyNumber; }

private:

	//  Must override main() inherited from ActiveClass. The base class constructor will then
	//	create a thread to run though the function main()

	int main(void)	
	{
		for(int i = 0; i < 100; i ++)	{
			printf("I am an ActiveClass2 Object/Thread. My Data is %d.....\n", MyNumber) ;
			SLEEP(400) ;
		}
		
		return 0 ;
	}
} ;



//
//	The program main()
//

int main(void)
{
	// create instance of the above active object on the stack
	// then allow it to run and then wait for it to terminate

	MyActiveClass1	Object1(1), Object2(2), Object3(3) ;		// create an instance of the above class and pass constructor the value 1 just to show how to do it
	MyActiveClass2	Object4(4), Object5(5), Object6(6);		// create an instance of the above class and pass constructor the value 2 just to show how to do it

	Object1.Resume() ;				// allow thread to run
	Object2.Resume() ;				// allow thread to run
	Object3.Resume() ;				// allow thread to run
	Object4.Resume() ;				// allow thread to run
	Object5.Resume() ;				// allow thread to run
	Object6.Resume() ;				// allow thread to run

	Object1.WaitForThread() ;
	Object2.WaitForThread() ;
	Object3.WaitForThread() ;
	Object4.WaitForThread() ;
	Object5.WaitForThread() ;
	Object6.WaitForThread() ;

	printf("Finished\n") ;

	// object1/2 destructor called at the end of program main()

	return 0 ;
}


