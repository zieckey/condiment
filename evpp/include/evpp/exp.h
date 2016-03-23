#ifndef LIBEVENTPP_ALL_EXPORT_H_
#define LIBEVENTPP_ALL_EXPORT_H_

#include "platform_config.h"

#include "libevent_watcher.h"

#ifdef H_OS_WINDOWS
//! Define import lib macro. Add it in ANY CPP file in target host module.
H_LINK_LIB( "libevpp")
#endif // end of #ifdef H_OS_WINDOWS

#endif



