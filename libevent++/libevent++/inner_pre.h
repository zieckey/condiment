
#ifndef LIBEVENTPP_PREREQUISITS_H__
#define LIBEVENTPP_PREREQUISITS_H__

#include "platform_config.h"

#include <assert.h>
#include <stdint.h>
#include <iostream>

#include "gettimeofday.h"

#ifdef H_OS_WINDOWS
#include <functional>
#else
#include <tr1/functional>
#endif

#ifndef IMPORT_LOGGER_H
#define LOG_TRACE std::cout << __FILE__ << ":" << __LINE__
#define LOG_DEBUG std::cout << __FILE__ << ":" << __LINE__
#define LOG_INFO  std::cout << __FILE__ << ":" << __LINE__
#define LOG_WARN  std::cout << __FILE__ << ":" << __LINE__
#define LOG_ERROR std::cout << __FILE__ << ":" << __LINE__

#define CHECK_NOTNULL(val) \
    LOG_ERROR << "'" #val "' Must be non NULL";

#endif

namespace evqing
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