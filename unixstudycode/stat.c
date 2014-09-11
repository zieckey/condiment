#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>

void test_lstat_symbol_link();
void test_stat_socket();

int main( int argc, char* argv[] )
{

    test_stat_socket();
    test_lstat_symbol_link();

    return 0;
}


void test_lstat_symbol_link()
{
    struct stat st;
    const char* symbol_link_path = "tmp/rsa_link";
    int ret = lstat( symbol_link_path, &st );
    if ( ret != 0 )
    {
        printf("%s not exist\n", symbol_link_path);
        return 0;
    }

    if( S_ISLNK(st.st_mode) )
    {
        printf("%s exist, and it is a symbol link\n", symbol_link_path);

        char buf[1024] = {};
        ret = readlink(symbol_link_path, buf, sizeof(buf));
        printf("[%s] --> [%s]\n", symbol_link_path, buf);
    }   
    else
    {
        printf("%s exist, but it is not a symbol link\n", symbol_link_path);
    }   

}


void test_stat_socket()
{
    struct stat st;
    const char* sockfile = "/home/weizili/memcached.sock";
    int ret = stat( sockfile, &st );
    if ( ret == 0 )
    {
        printf("%s not exist\n", sockfile);
        return;
    }

    if( S_ISSOCK(st.st_mode) )
    {
        printf("%s exist, and it is a socket\n", sockfile);
    }
    else
    {
        printf("%s exist, but it is not a socket\n", sockfile);
    }
}



