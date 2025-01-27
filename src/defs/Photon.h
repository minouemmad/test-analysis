#pragma once

#include <vector>

#include "defs/Track.h"

namespace analysis
{
	namespace core
	{
		class FsrPhoton : public Track
		{
			public:
				FsrPhoton() : Track() {this->reset();}

				virtual void reset()
				{
					_dROverEt2 = 0;
					_muonIdx = 0;
					_relIso03 = 0;
					Track::reset();
					_track.reset();

				}
				virtual ~FsrPhoton() {}

				Track _track;
				float _dROverEt2;
				int _muonIdx;
				float _relIso03;


		};

		typedef std::vector<analysis::core::FsrPhoton> FsrPhotons;
	}
}
