#ifndef Analysis_Core_MetaHigss_h
#define Analysis_Core_MetaHigss_h

#include "Object.h"

namespace analysis
{
	namespace dimuon
	{
        class Auxiliary : public analysis::core::Object
        {
            public: 
                Auxiliary() : analysis::core::Object() {this->reset();}
                virtual ~Auxiliary() {}

                virtual void reset() 
                {
                    m_ptSum = 0;
                    m_numHT = 0;
                }

                int m_numHT;
                double m_ptSum;

        };

		class MetaHiggs : public analysis::core::Object
		{
			public:
				MetaHiggs() : analysis::core::Object() {this->reset();}
				virtual ~MetaHiggs() {}
	
				virtual void reset()
				{
					_nEventsProcessed = 0;
					_sumEventWeights = 0;
					_isMC = 0;
					_triggerNames.clear();
                    _btagNames.clear();
                    _tauIDNames.clear();
					_nMuons = 0;
					_checkTrigger = 0;
	
					//	cuts used at ntuple production stage
					_isGlobalMuon = 0;
					_isTrackerMuon = 0;
					_isStandAloneMuon = 0;
					_minPt = 0;
					_maxeta = 0;
					_maxNormChi2 = 0;
					_minMuonHits = 0;
					_minPixelHits = 0;
					_minStripHits = 0;
					_minTrackerHits = 0;
					_minSegmentMatches = 0;
					_minMatchedStations = 0;
					_minPixelLayers = 0;
					_minTrackerLayers = 0;
					_minStripLayers = 0;
					_minValidFractionTracker = 0;
					_maxd0 = 0;
					_maxTrackIsoSumPt = 0;
					_maxRelCombIso = 0;
				}

				int _nEventsProcessed;
				int _sumEventWeights;
				bool _isMC;
				std::vector<std::string> _triggerNames;
                std::vector<std::string> _btagNames;
                std::vector<std::string> _tauIDNames;
				uint32_t _nMuons;
				bool _checkTrigger;
	
				//	cuts used at ntuple production stage
				bool _isGlobalMuon;
				bool _isTrackerMuon;
				bool _isStandAloneMuon;
				float _minPt;
				float _maxeta;
				float _maxNormChi2;
				int _minMuonHits;
				int _minPixelHits;
				int _minStripHits;
				int _minTrackerHits;
				int _minSegmentMatches;
				int _minMatchedStations;
				int _minPixelLayers;
				int _minTrackerLayers;
				int _minStripLayers;
				float _minValidFractionTracker;
				float _maxd0;
				float _maxTrackIsoSumPt;
				float _maxRelCombIso;

		};
	}
}

#endif
