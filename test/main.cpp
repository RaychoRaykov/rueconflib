#include <stdio.h>

#include "logging.h"
#include "config.h"

int main ( int /*argc*/, char */*argv*/[] )
{LOG
	using namespace Config::MainSection::AA::AAA;
	TestInt = 678;
	TestDouble = 3.14;
	Config::Save();
	return 0;
}
