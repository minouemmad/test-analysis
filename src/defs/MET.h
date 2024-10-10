#ifndef Analysis_Core_MET_h
#define Analysis_Core_MET_h

#include "Object.h"

namespace analysis
{
	namespace core
	{
		class MET : public Object
		{
			public:
				MET() : Object() {this->reset();}

				virtual void reset()
				{
					_px = 0;
					_py = 0;
					_pt = 0; 
					_phi = 0;
					_sumEt = 0;
				}
				virtual ~MET() {}

				float _px;
				float _py;
				float _pt;
				float _phi;
				float _sumEt;

		};

		typedef std::vector<analysis::core::MET> METs;
	}
}

#endif
