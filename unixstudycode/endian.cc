#include <stdio.h>
#include <stdint.h>
#include <endian.h>

inline uint32_t hostToNetwork32(uint32_t host32)
{
#ifdef __linux__
    return 0;
#else
      return htobe32(host32);
#endif
}

int main()
{
    uint32_t h = 0x1234;
    uint32_t n = hostToNetwork32(h);
    printf("h=%x, n=%x\n", h,n);
    return 0;
}

