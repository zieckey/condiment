#include "libtestprofiler.h"
extern "C"
{
    int loopop()
    {
        int n = 0;
        for(int i = 0; i < 10000; i++)
          for(int j = 0; j < 10000; j++)
          {  
              n |= i%100 + j/100;
          }  
        return n;
    }
}
