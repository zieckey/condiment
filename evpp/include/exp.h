#ifndef LIBEVENTPP_ALL_EXPORT_H_
#define LIBEVENTPP_ALL_EXPORT_H_

#include "platform_config.h"

#include "libevent_watcher.h"

#ifdef H_OS_WINDOWS
//! Define import lib macro. Add it in ANY CPP file in target host module.
H_LINK_LIB( "libevpp")
#endif // end of #ifdef H_OS_WINDOWS

namespace evpp
{

    //! Module symbol export
#ifdef H_WINDOWS_API
#	ifndef  H_STATIC_LIB_LIBEVENTPP
#		ifdef  H_LIBEVENTPP_EXPORTS
#			define _EXPORT_LIBEVENTPP __declspec(dllexport)
#		else
#			define _EXPORT_LIBEVENTPP __declspec(dllimport)
#		endif
#	else
#		define _EXPORT_LIBEVENTPP
#	endif
#else
#	define _EXPORT_LIBEVENTPP
#endif

}



#endif



