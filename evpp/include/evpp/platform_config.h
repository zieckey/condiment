#ifndef LIBEVENTPP_PLATFROM_CONFIG_H_
#define LIBEVENTPP_PLATFROM_CONFIG_H_

//! Define Operation System.
#if ( defined(WIN32) || defined(WIN64) )
#	ifndef H_OS_WINDOWS
#		define H_OS_WINDOWS
#	endif
#	ifndef H_WINDOWS_API
#		define H_WINDOWS_API
#	endif
#endif

#ifdef _DEBUG
#ifndef H_DEBUG_MODE
#define H_DEBUG_MODE
#endif
#endif


#include <assert.h>
#include <stdint.h>
#include <iostream>

#ifdef __cplusplus
#include "gettimeofday.h"

#define GOOGLE_GLOG_DLL_DECL           // 使用静态glog库时，必须定义这个
#define GLOG_NO_ABBREVIATED_SEVERITIES // 没这个编译会出错,传说因为和Windows.h冲突
#include <glog/logging.h>

#ifdef H_OS_WINDOWS
#include <functional>
#else
#include <tr1/functional>
#endif

#ifdef GOOGLE_LOG_INFO
#define LOG_TRACE LOG(INFO)
#define LOG_DEBUG LOG(INFO)
#define LOG_INFO  LOG(INFO)
#define LOG_WARN  LOG(WARNING)
#define LOG_ERROR LOG(ERROR)
#define LOG_FATAL LOG(FATAL)
#else
#define LOG_TRACE std::cout << __FILE__ << ":" << __LINE__
#define LOG_DEBUG std::cout << __FILE__ << ":" << __LINE__
#define LOG_INFO  std::cout << __FILE__ << ":" << __LINE__
#define LOG_WARN  std::cout << __FILE__ << ":" << __LINE__
#define LOG_ERROR std::cout << __FILE__ << ":" << __LINE__
#define LOG_FATAL std::cout << __FILE__ << ":" << __LINE__

#define CHECK_NOTNULL(val) \
    LOG_ERROR << "'" #val "' Must be non NULL";
#endif




//! Finds the compiler type and version.
#if defined( _MSC_VER )
#	ifndef H_COMPILER_MSVC
#		define H_COMPILER_MSVC
#	endif
#	ifndef H_COMPILER_VERSION
#		define H_COMPILER_VERSION _MSC_VER
#	endif
#	if H_COMPILER_VERSION < 1300
#		pragma error "Not supported compiler version. Abort! Abort!"
#	endif
#elif defined( __GNUC__ )
#	ifndef H_COMPILER_GNUC
#		define H_COMPILER_GNUC
#	endif
#	ifndef H_COMPILER_VERSION
#		define H_COMPILER_VERSION (((__GNUC__)*100) + \
                                    (__GNUC_MINOR__ * 10) + \
                                    __GNUC_PATCHLEVEL__)
#	endif
#else
#   pragma error "No known compiler. Abort! Abort!"
#endif


#ifdef xstd
    #error "xstd is already defined in another place. Please check it!"
#endif
#ifdef H_OS_WINDOWS
    #include <memory>
    #include <functional>
    #define xstd std
#else
    #if H_COMPILER_VERSION >= 472
        #include <memory>
        #include <functional>
        #define xstd std
    #else
        #include <tr1/memory>
        #include <tr1/functional>
        #define xstd std::tr1
    #endif
#endif

#endif // end of define __cplusplus

//////////////////////////////////////////////////////////////////////////
//                              Link Helper Macro                       //
//  Use: H_LINK_LIB(libname) to import a library.                       //
//////////////////////////////////////////////////////////////////////////
//------------------------------------------------------
#ifdef H_OS_WINDOWS
#	define H_LINK_OS_FLAG 1
#else
#	define H_LINK_OS_FLAG 0
#endif

#ifdef H_DEBUG_MODE
#	define H_LINK_FILE_DEBUG_FLAG 1
#else
#	define H_LINK_FILE_DEBUG_FLAG 0
#endif

//#define H_LINK_FILE_DEBUG_1( filename )	__pragma ( comment( lib , filename##"_d"##".lib" ))
#define H_LINK_FILE_DEBUG_1( filename )	__pragma ( comment( lib , filename##".lib" ))
#define H_LINK_FILE_DEBUG_0( filename )	__pragma (comment( lib , filename##".lib" ))

#define H_LINK_FILE_DEBUG_P(filename,y)   H_LINK_FILE_DEBUG_##y(filename)

#define H_LINK_FILE_0(filename,y)
#define H_LINK_FILE_1(filename,y)         H_LINK_FILE_DEBUG_P(filename,y)

#define H_LINK_FILE_PP( filename , sys )  H_LINK_FILE_##sys( filename , H_LINK_FILE_DEBUG_FLAG )
#define H_LINK_FILE_P( filename , sys )   H_LINK_FILE_PP( filename , sys )


//
#ifndef __MINGW32__
#   define H_LINK_LIB( filename )           H_LINK_FILE_P( filename , H_LINK_OS_FLAG )
#else
#   define H_LINK_LIB( filename )
#endif

#ifdef H_OS_WINDOWS
#define usleep(us) Sleep((us)/1000);
#endif

#ifdef H_OS_WINDOWS
#pragma warning( disable: 4251 )
#endif

#endif // end of #ifndef LIBEVENTPP_PLATFROM_CONFIG_H_
