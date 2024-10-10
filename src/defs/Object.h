#ifndef Analysis_Core_Object_h
#define Analysis_Core_Object_h

#include <iostream>
#include <string>
#include <vector>
#include <list>
#include <set>
#include <array>
#include <map>

using namespace std;

#include "TObject.h"

namespace analysis
{
	namespace core
	{
		class Object
		{
			public:
				Object(){this->reset();}
				virtual ~Object() {}

				virtual void reset() {}

		};

		typedef std::vector<analysis::core::Object> Objects;
	}
}

#endif


