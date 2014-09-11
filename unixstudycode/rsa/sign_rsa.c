/**
 * compile : gcc rsa_sign_512.c -lcrypto
 */

#include <stdio.h>
#include <openssl/rsa.h>
#include <openssl/engine.h>
#include <openssl/md5.h>
//#include <md5.h>
#define  MD5Init    MD5_Init
#define  MD5Update  MD5_Update
#define  MD5Final   MD5_Final


//int ibits = MAX_BUFFER_LEN;//MAX_BUFFER_LENbits
int ibits = 512;//512 bits
//const size_t MAX_BUFFER_LEN = 8096;
#define MAX_BUFFER_LEN 8096

typedef struct {
	unsigned char* data;
	size_t         data_len;
} mem_data_t;

/**
 * Gengerate a ramdon RSA 
 */
RSA* gen_rsa( int ibits )
{
	BIGNUM* bne = BN_new();
	int ret = BN_set_word( bne, RSA_F4 );
	RSA* rsa = RSA_new();
	ret = RSA_generate_key_ex( rsa, ibits, bne, NULL );
    BN_free( bne );

	if( ret == 1 )
	{
		return rsa;
	}
	else
	{
		return NULL;
	}
}


/**
 * Create a RSA from the giving key string
* rsa_type = 1, return privatekey
* rsa_type = 2, return publickey
*/
RSA* create_rsa( const u_char* key, size_t key_len, int rsa_type )
{/*{{{*/

    BIO* b = BIO_new( BIO_s_mem() );
    BIO_write( b, key, key_len );

    RSA* rsa = NULL;
    if( rsa_type == 1 )
    {
        rsa = d2i_RSAPrivateKey_bio( b, NULL );
    }
    else if( rsa_type == 2 )
    {
        rsa = d2i_RSAPublicKey_bio( b, NULL );
    }

    BIO_free( b );

    return rsa;
}/*}}}*/

void sign_data( RSA* rsa, const unsigned char* bData, unsigned int iDatalen, mem_data_t* out_buf )
{/*{{{*/
	int ret, i;

	unsigned int iSignlen = RSA_size( rsa );

    //printf( "\nisignlen=RSA_size(rsa)=%d\n", iSignlen );

	ret = RSA_sign( NID_sha1, bData, iDatalen, out_buf->data, &iSignlen, rsa );

    out_buf->data_len = iSignlen;
	
	//printf("\nsign_len:%d\n",iSignlen);
	//for(i=0;i<iSignlen;i++)
	    //printf("%d ", out_buf[i]);
	//printf("\n");
	
	if( ret != 1 )
	{
		int l;
		char s[MAX_BUFFER_LEN];
		FILE* f;

		f = fopen( "/dev/stdout", "w" );
		ERR_print_errors_fp( f );

		l = ERR_get_error();
		ERR_error_string_n( l, s, sizeof( s ) );
		perror( s );
		exit( 1 );
	}
}/*}}}*/

int main ( int argc, const char * argv[] )
{
    if ( argc != 2 )
    {
        fprintf( stderr, "Usage: %s [rsa_bits]\n", argv[0] );
        fprintf( stderr, "\te.g.: %s 512\n", argv[0] );
        fprintf( stderr, "\te.g.: %s MAX_BUFFER_LEN\n", argv[0] );
        return -1;
    }

    ibits = atoi(argv[1]);

	char raw_data[] = "59f0aa0a1c3f65f459f0aa0a1c3f65f4";

    printf( "#ifndef _RSA_SIGN_KEY_H_ \n");
    printf( "#define _RSA_SIGN_KEY_H_ \n");
    int jj = 0;
    for (jj = 0; jj < 10; jj++)
    {
        //generate public and private key
        unsigned char public_key[MAX_BUFFER_LEN] = { 0 };
        unsigned char private_key[MAX_BUFFER_LEN] = { 0 };
        BIO* bio = BIO_new( BIO_s_mem() );
        RSA* rsa = gen_rsa( ibits );
        int ret = i2d_RSAPublicKey_bio( bio, rsa );//create a public key
        int public_key_len = BIO_read( bio, public_key, sizeof(public_key) );//read the key data into public_key buffer
        ret = i2d_RSAPrivateKey_bio( bio, rsa );//create a private key
        int private_key_len = BIO_read( bio, private_key, sizeof(private_key) );//read the key data into private buffer
        BIO_free( bio );
        RSA_free( rsa );

        //print public and private key
        int i = 0;
        printf( "\nstatic const u_char g_rsa_public_key%d[%lu] = {//{{{\n", jj, public_key_len );
        for( i = 0; i < public_key_len; i++ )
        {
            if (i != 0)
            {
                printf(", ");
                //if ( i % 16 == 0 )
                 // printf("\n\t");
            }
            else
            {
                printf("\t");
            }
            printf( "0x%0.2x", public_key[i] );
        }
        printf( "};//}}}\n" );
        printf("static const size_t g_rsa_public_key%d_len = %lu;\n\n", jj, public_key_len);
        printf( "\nstatic const u_char g_rsa_private_key%d[%lu] = {//{{{\n", jj, private_key_len );
        for( i = 0; i < private_key_len; i++ )
        {
            if (i != 0)
            {
                printf(", ");
                //if ( i % 16 == 0 )
                 // printf("\n\t");
            }
            else
            {
                printf("\t");
            }
            printf( "0x%0.2x", private_key[i] );
        }
        printf( "};//}}}\n" );
        printf("static const size_t g_rsa_private_key%d_len = %lu;\n\n", jj, private_key_len);
        printf( "\n" );

    }

    printf("#endif\n");

    ///test
    {
        //generate public and private key
        unsigned char public_key[MAX_BUFFER_LEN] = { 0 };
        unsigned char private_key[MAX_BUFFER_LEN] = { 0 };
        BIO* bio = BIO_new( BIO_s_mem() );
        RSA* rsa = gen_rsa( ibits );
        int ret = i2d_RSAPublicKey_bio( bio, rsa );//create a public key
        int public_key_len = BIO_read( bio, public_key, sizeof(public_key) );//read the key data into public_key buffer
        ret = i2d_RSAPrivateKey_bio( bio, rsa );//create a private key
        int private_key_len = BIO_read( bio, private_key, sizeof(private_key) );//read the key data into private buffer
        BIO_free( bio );
        RSA_free( rsa );

        //print public and private key
        int i = 0;
        //printf( "\nstatic const u_char g_rsa_public_key%d[%lu] = {\n", jj, public_key_len );
        //printf( "};\n" );
        //printf("static const size_t g_rsa_private_key%d_len = %lu;\n\n", jj, private_key_len);
        //printf( "\n" );


    //using private key to sign data
    mem_data_t datasign_private;
    datasign_private.data_len = MAX_BUFFER_LEN;
    datasign_private.data = (char*) malloc( MAX_BUFFER_LEN );
    RSA* rsa_private = create_rsa( private_key, private_key_len, 1 );
    sign_data( rsa_private, raw_data, sizeof( raw_data ), &datasign_private );
    

    //using public key to sign data
    mem_data_t datasign_public;
    datasign_public.data_len = MAX_BUFFER_LEN;
    datasign_public.data = (char*) malloc( MAX_BUFFER_LEN );
    RSA* rsa_public = create_rsa( public_key, public_key_len, 2 );
    int verifyret = RSA_verify( NID_sha1, raw_data, sizeof( raw_data ), datasign_private.data, datasign_private.data_len, rsa_public );
    //if ( verifyret == 1 )
      //printf( "verify ok\n" );
      //else
      //printf( "verify failed\n" );


    //print the data sign compare result
    RSA_free( rsa_private );
    RSA_free( rsa_public );

    }
    return 0;
}
