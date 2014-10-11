#ifndef __LOGGING_H__
#define __LOGGING_H__

	#ifdef DOLOG
		#define LOG printf ( "\n[%04d] %s", __LINE__, __PRETTY_FUNCTION__ );
	#else
		#define LOG
	#endif

#endif