
#ifndef LIBEVENTPP_PREREQUISITS_H__
#define LIBEVENTPP_PREREQUISITS_H__

#include "platform_config.h"

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
#endif // H_STATIC_LIB_
};

#endif