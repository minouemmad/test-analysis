#ifndef Analysis_Core_Event_h
#define Analysis_Core_Event_h

#include "Object.h"

namespace analysis
{
	namespace core
	{
		class Event : public Object
		{
			public:
				Event() : Object() {this->reset();}

				virtual void reset()
				{
					_run = 0;
					_lumi = 0;
					_event = 0;
					_bx = 0;
					_orbit = 0;
				}
				virtual ~Event() {}

				int _run;
				int _lumi;
				long long int _event;
				int _bx;
				int _orbit;

		};

		class EventAuxiliary : public Object
		{
			public:
				EventAuxiliary() :  Object() {this->reset();}
				virtual ~EventAuxiliary() {}

				virtual void reset()
				{
					_nPU = 0;
					_genWeight = 0;
					_hasHLTFired.clear();
				}

				int _nPU;
				int _genWeight;
				std::vector<bool> _hasHLTFired;

		};

		typedef std::vector<analysis::core::EventAuxiliary> EventAuxiliaries;
		typedef std::vector<analysis::core::Event> Events;
	}
}

#endif
