#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

#include <iostream>

using namespace std;


void writefile( const int blocksize, const int nmemb, const int filesize )
{
    char* pbuf = new char[filesize];
    memset( pbuf, 'A', filesize );

    struct timeval tb;
    gettimeofday( &tb, 0 );

    static int ni = 0;
    const char* name[10] = { "a", "b", "c", "d", "e", "f", "g", "h", "i", "j" };
    FILE* fp = ::fopen( name[ni++%10], "w+" );
    int count = filesize /(blocksize * nmemb);
    for ( int i = 0; i < count; ++i )
    {
        ::fwrite( pbuf + i * blocksize * nmemb, blocksize, nmemb, fp );
    }

    int remain = blocksize * nmemb - count * blocksize * nmemb;
    if ( remain > 0 )
    {

        ::fwrite( pbuf + count * blocksize * nmemb, remain, nmemb, fp );
    }

    ::fclose( fp );
    fp = 0;

    struct timeval te;
    gettimeofday( &te, 0 );

    double diff = te.tv_sec + (double(te.tv_usec))/1000000.0f
                  - tb.tv_sec - (double(tb.tv_usec))/1000000.0f;
    cout<< "size=" << blocksize
        << "  nmemb=" << nmemb
        << "  filesize=" << filesize
        << "  write time=" << diff << "s\n";


}


int main( int argc, char* argv[] )
{
    if ( argc != 4 )
    {
        cout << "usage:" << argv[0] << "\tfilesize blocksize blocksize2\n\t\tfilesize is in MB, others in bytes\n" << endl;
        return 1;
    }

    int filesize = 1024 * 1024 * atoi( argv[1] );
    int blocksize1 = atoi( argv[2] );
    int blocksize2 = atoi( argv[3] );

    writefile( blocksize1, 1, filesize );
    writefile( blocksize2, 1, filesize );

    writefile( 1, blocksize1, filesize );
    writefile( 1, blocksize2, filesize );
    return 0;
}


