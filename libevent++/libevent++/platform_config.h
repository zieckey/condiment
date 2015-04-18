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


#endif