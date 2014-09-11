
//main.c

struct Say;

/**
  * compile : gcc -c main.c -o main.o
  * compile : gcc main.o cppsayhello.o -lstdc++ -o a.out 
  */
void  destroy_say( void* say );
void* create_say();
void  sayhello( struct Say* say );

int main( int argc, char* argv[] )
{

	struct Say* say = (struct Say*)create_say();
	sayhello( say );
	destroy_say( say );

	return 0;
}
