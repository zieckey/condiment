
#include <openssl/rsa.h>
#include <openssl/engine.h>


namespace
{
    /**
     * Generate a random RSA, remember to RSA_free the return pointer
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
            RSA_free( rsa );
            return NULL;
        }
    }
}//end of anonymous namespace

bool GenerateKey( size_t keylen, 
            unsigned char public_key[], size_t* public_key_len, 
            unsigned char private_key[] , size_t* private_key_len )
{
    RSA* rsa = gen_rsa( keylen );
    if ( !rsa )
    {
        return false;
    }

    BIO* bio = BIO_new( BIO_s_mem() );
    int ret = i2d_RSAPublicKey_bio( bio, rsa );//create a public key
    if ( 1 == ret )
    {
        *public_key_len = BIO_read( bio, public_key, keylen );//read the key data into public_key buffer
        ret = i2d_RSAPrivateKey_bio( bio, rsa );//create a private key
        if ( 1 == ret )
        {
            *private_key_len = BIO_read( bio, private_key, keylen );//read the key data into private buffer
        }
    }

    BIO_free( bio ); bio = NULL;
    RSA_free( rsa ); rsa = NULL;

    return (1 == ret);
}


int main(int argc, char* argv[]) 
{
    int keylen = 1024;
    if (argc > 1) {
        keylen = atoi(argv[1]);
    }

    unsigned char public_key[4096] = {0};
    unsigned char private_key[4096] = {0};
    size_t public_key_len = 4096;
    size_t private_key_len = 4096;
    if (GenerateKey(keylen, public_key, &public_key_len, private_key, &private_key_len)) {
        printf("generate key OK, keylen=%d\n", keylen);
    } else {
        printf("generate key failed, keylen=%d\n", keylen);
    }

    return 0;
}
