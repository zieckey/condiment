
//header file : cppsayhello.h 
#ifndef SAY_HELLO_H
#define SAY_HELLO_H


class Say;

extern "C" void  sayhello( Say* say );
extern "C" void* create_say();
extern "C" void  destroy_say( void* say );

class Say
{
	public:
		void hello();
};




#endif 


