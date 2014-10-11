#ifndef __CONFIG_H__
#define __CONFIG_H__

#include "Configuration.h"

CONFIGURATION ( Config, "test.cfg" )
	SECTION ( MainSection )
		PROPERTY ( bool, TestBool, true )
		SECTION ( AA )
			SECTION ( AAA )
				PROPERTY ( int, TestInt, 5 )
				PROPERTY ( double, TestDouble, 3.1415926 )
			END_SECTION
			SECTION ( AAB )
				PROPERTY ( std::string, TestString, "Test string" )
			END_SECTION
		END_SECTION
		SECTION ( AB )
		END_SECTION
	END_SECTION
END_CONFIGURATION

#endif
