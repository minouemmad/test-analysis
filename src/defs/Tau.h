#ifndef Analysis_Core_Tau_h
#define Analysis_Core_Tau_h

#include "Track.h"

namespace analysis
{
    namespace core
    {
        class Tau : public Track
        {
            public:
                Tau() : Track() {this->reset();}
                virtual ~Tau() {}
                virtual void reset()
                {
                    Track::reset();
                    _ids.clear();
                    _isPF = false;
                }

                std::vector<float> _ids;
                bool _isPF;

        };

        typedef std::vector<analysis::core::Tau> Taus;
    }
}

#endif
