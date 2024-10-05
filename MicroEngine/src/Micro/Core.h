#pragma once

//#ifdef DEBUG
//	#define MC_ENABLE_ASSERTS
//#endif

#ifdef MC_PLATFORM_WINDOWS
	#ifdef MC_BUILD_DLL
		#define MICRO_API __declspec(dllexport)
	#else
		#define MICRO_API __declspec(dllimport)
	#endif 
#else
	#error Windows is the only supported platform
#endif