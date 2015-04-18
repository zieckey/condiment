

#include "test_common.h"


#ifdef WIN32
#   ifdef _DEBUG
// #		pragma comment(lib, "qhsec.lib")
#   else
// #		pragma comment(lib, "qhsec.lib")
#   endif
#	pragma comment(lib,"Ws2_32.lib")
#endif

namespace
{
#ifdef WIN32
    struct OnAppExit
    {
        ~OnAppExit()
        {
            system("pause");
        }
    } __s_onexit_pause;
#endif
}
