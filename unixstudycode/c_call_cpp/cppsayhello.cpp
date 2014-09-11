//implematation file cppsayhello.cpp

#include "cppsayhello.h"

#include <iostream>
/*
#include "qoslib/include/QOSLibAllExp.h"
#include "qoslib/include/QMD5.h"
*/

/**
  * compile : g++ -c cppsayhello.cpp -o cppsayhello.o
  */
static void printhello()
{
	std::cout << "xxxhello, pretty_function=" 
        << __PRETTY_FUNCTION__ 
        << " FUNCTION=" << __FUNCTION__
        << " func=" << __func__ << std::endl;
}


void Say::hello()
{
	std::cout << "hello, pretty_function=" 
        << __PRETTY_FUNCTION__ 
        << " FUNCTION=" << __FUNCTION__
        << " func=" << __func__ << std::endl;

    printhello();
 //   osl::MD5 md5;
}



void sayhello( Say* say )
{
	say->hello();
}


void* create_say()
{
	Say* say = new Say();
	return (void*)say;
}

void destroy_say( void* say )
{
	Say* s = (Say*)say;
	delete s;
}
