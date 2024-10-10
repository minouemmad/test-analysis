
//	
#ifdef STANDALONE
#include "Muon.h"
#include "Electron.h"
#include "Jet.h"
#include "Vertex.h"
#include "Event.h"
#include "MET.h"
#include "Constants.h"
#include "Streamer.h"
#include "MetaHiggs.h"

//	ROOT headers
#include "TFile.h"
#include "TChain.h"
#include "TString.h"
#include "TMath.h"
#include "TH1D.h"
#include "TLorentzVector.h"
#include "LumiReweightingStandAlone.h"
#include "HistogramSets.h"

#include "boost/program_options.hpp"
#include <signal.h>
#include <fstream>

#define FILL_JETS(set) \
	set.hDiJetMass->Fill(dijetmass, puweight); \
	set.hDiJetdeta->Fill(TMath::Abs(deta), puweight); \
	set.hDiMuonpt->Fill(p4dimuon.Pt(), puweight); \
	set.hDiMuonMass->Fill(p4dimuon.M(), puweight); \
	set.hDiMuoneta->Fill(p4dimuon.Eta(), puweight); \
	set.hDiMuondphi->Fill(dphi, puweight); \
	set.hMuonpt->Fill(p4m1.Pt(), puweight); \
	set.hMuonpt->Fill(p4m2.Pt(), puweight); \
	set.hMuoneta->Fill(p4m1.Eta(), puweight); \
	set.hMuoneta->Fill(p4m2.Eta(), puweight); \
	set.hMuonphi->Fill(p4m1.Phi(), puweight); \
	set.hMuonphi->Fill(p4m2.Phi(), puweight);

#define FILL_NOJETS(set) \
	set.hDiMuonpt->Fill(p4dimuon.Pt(), puweight); \
	set.hDiMuonMass->Fill(p4dimuon.M(), puweight); \
	set.hDiMuoneta->Fill(p4dimuon.Eta(), puweight); \
	set.hDiMuondphi->Fill(dphi, puweight); \
	set.hMuonpt->Fill(p4m1.Pt(), puweight); \
	set.hMuonpt->Fill(p4m2.Pt(), puweight); \
	set.hMuoneta->Fill(p4m1.Eta(), puweight); \
	set.hMuoneta->Fill(p4m2.Eta(), puweight); \
	set.hMuonphi->Fill(p4m1.Phi(), puweight); \
	set.hMuonphi->Fill(p4m2.Phi(), puweight);

/*
 *	Declare/Define all the service globals
 */
std::string __inputfilename;
std::string __outputfilename;
bool __isMC;
bool __genPUMC;
std::string __puMCfilename;
std::string __puDATAfilename;
bool __continueRunning = true;

/*
 *  Define all the Constants
 */
double _muonMatchedPt = 26.;
double _muonMatchedEta = 2.4;
double _muonPt = 10.;
double _muonEta = 2.4;
double _muonIso = 0.25;
double _leadJetPt = 40.;
double _subleadJetPt = 30.;
double _metPt = 40.;
double _dijetMass_VBFTight = 650;
double _dijetdEta_VBFTight = 3.5;
double _dijetMass_ggFTight = 250.;
double _dimuonPt_ggFTight = 50.;
double _dimuonPt_01JetsTight = 10.;
int __nVBFTight = 0;
int __nggFTight = 0;
int __nggFLoose = 0;
int __n01JetsTight = 0;
int __n01JetsLoose = 0;
int __nTotalCategorization = 0;
int __nTotalOverall = 0;
int __nTotalPassMuonSelections = 0;
int __nTotalPassElectronJetVetos = 0;
std::vector<int> vVBFTight, vggFTight, vggFLoose, v01JetsTight, v01JetsLoose,
    vTotal;

std::string const NTUPLEMAKER_NAME =  "ntuplemaker_H2DiMuonMaker";

namespace po = boost::program_options;
using namespace analysis::core;
using namespace analysis::dimuon;
using namespace analysis::processing;

bool passVertex(Vertices* v)
{
	if (v->size()==0)
		return false;
	int n=0;
	for (Vertices::const_iterator it=v->begin();
		it!=v->end() && n<20; ++it)
	{
		if (TMath::Abs(it->_z)<24 &&
			it->_ndf>4)
			return true;
		n++;
	}

	return false;
}

bool passMuon(Muon const& m)
{
    double muonIsolation = (m._sumChargedHadronPtR04 + std::max(0., 
        m._sumNeutralHadronEtR04 + m._sumPhotonEtR04 - 0.5*m._sumPUPtR04)) / m._pt;

	if (m._isGlobal && m._isTracker &&
		m._pt>10 && TMath::Abs(m._eta)<2.4 &&
		m._isMedium
//        && (m._trackIsoSumPt/m._pt)<_muonIso
        && muonIsolation < _muonIso
        )
		return true;
	return false;
}

bool passMuonHLT(Muon const& m)
{
	if ((m._isHLTMatched[1] || m._isHLTMatched[0]) &&
		m._pt>26 && TMath::Abs(m._eta)<2.4)
		return true;

	return false;
}

bool passMuons(Muon const& m1, Muon const& m2)
{
	if (m1._charge!=m2._charge &&
		passMuon(m1) && passMuon(m2))
		if (passMuonHLT(m1) || passMuonHLT(m2))
			return true;

	return false;
}

float jetMuondR(float jeta,float jphi, float meta, float mphi)
{
	TLorentzVector p4j,p4m;
	p4j.SetPtEtaPhiM(10, jeta, jphi, 0);
	p4m.SetPtEtaPhiM(10, meta, mphi, 0);
	return p4j.DeltaR(p4m);
}

bool passElectronVeto(Electrons *electrons, Muon m1, Muon m2)
{
    for (Electrons::const_iterator it=electrons->begin();
        it!=electrons->end(); ++it)
    {
        // if there is an electron of medium id
        // with > pt and in a certain eta
        // and isolated
        // does not pass
        if (it->_ids[2] && it->_pt>10. && 
            (TMath::Abs(it->_eta)<1.4442 || (1.566<TMath::Abs(it->_eta) && TMath::Abs(it->_eta)<2.5)) && jetMuondR(it->_eta, it->_phi, m1._eta, m1._phi)>0.4
            && jetMuondR(it->_eta, it->_phi, m2._eta, m2._phi)>0.4)
            return false;
    }

    return true;
}

bool passBTaggedJetVeto(Jets *jets)
{
    for (Jets::const_iterator it=jets->begin(); it!=jets->end(); ++it)
        if (it->_pt>30. & TMath::Abs(it->_eta)<2.4 && it->_btag[0]>0.8484)
            return false;
    return true;
}

void categorize(Jets* jets, Muon const& mu1, Muon const&  mu2, 
	MET const& met, Event const& event)
{
	TLorentzVector p4m1, p4m2;
	p4m1.SetPtEtaPhiM(mu1._pt, mu1._eta, 
		mu1._phi, PDG_MASS_Mu);
	p4m2.SetPtEtaPhiM(mu2._pt, mu2._eta, 
		mu2._phi, PDG_MASS_Mu);
	TLorentzVector p4dimuon = p4m1 + p4m2;

    __nTotalCategorization++;

    /*
    if (!(mu1._isPF && mu2._isPF))
        return;
	if (!(p4dimuon.M()>110 && p4dimuon.M()<160 &&
		mu1._isPF && mu2._isPF))
		return;
        */

    // jets selection
	std::vector<TLorentzVector> p4jets;
	for (Jets::const_iterator it=jets->begin(); it!=jets->end(); ++it)
	{
		if (it->_pt>30 && TMath::Abs(it->_eta)<4.7)
		{
			if ((jetMuondR(it->_eta, it->_phi, mu1._eta, mu1._phi)>0.4) &&
				(jetMuondR(it->_eta, it->_phi, mu2._eta, mu2._phi)>0.4))
			{
				TLorentzVector p4;
				p4.SetPtEtaPhiM(it->_pt, it->_eta, it->_phi, it->_mass);
				p4jets.push_back(p4);
			}
		}
	}

	bool isPreSelected = false;
        bool vbf = false;
        bool ggFT = false;
        bool ggFL = false;
	if (p4jets.size()>=2)
	{

        for (int i=0; i<p4jets.size(); i++) 
            for (int j=i+1; j<p4jets.size(); j++)
            {
        TLorentzVector p4lead = p4jets[i]; 
		TLorentzVector p4sub = p4jets[j];
		TLorentzVector dijet = p4lead + p4sub;

		float deta = p4lead.Eta() - p4sub.Eta();
		float dijetmass = dijet.M();
			
		if (p4lead.Pt()>40)
		{
			isPreSelected = true;

			//	categorize
			if (dijetmass>650. && TMath::Abs(deta)>3.5)
			{
				//	VBF Tight
                vbf = true;
			}
			if (dijetmass>250. && p4dimuon.Pt()>50.)
			{
				//	ggF Tight
                ggFT = true;
            }
			else
			{	
                //	ggF Loose
                ggFL = true;
			}
		}
            }
	}
    if (isPreSelected) 
    {
        if (vbf) 
        {
            __nVBFTight++;
            vVBFTight.push_back(event._event);
            return;
        }
        else if (ggFT)
        {
            __nggFTight++;
            vggFTight.push_back(event._event);
            return;
        }
        else if (ggFL)
        {
            __nggFLoose++;
            vggFLoose.push_back(event._event);
            return;
        }
    }
	if (!isPreSelected)
	{
		//	separate loose vs tight
		if (p4dimuon.Pt()>=25.)
		{
            // 01JetsTight
            __n01JetsTight++;
            v01JetsTight.push_back(event._event);
		}
		else
		{
			//	01Jet Loose
            __n01JetsLoose++;
            v01JetsLoose.push_back(event._event);
		}
	}
	
	return;
}

float sampleinfo(std::string const& inputname)
{
	Streamer s(inputname, NTUPLEMAKER_NAME+"/Meta");
	s.chainup();

	using namespace analysis::dimuon;
	MetaHiggs *meta=NULL;
	s._chain->SetBranchAddress("Meta", &meta);

	long long int numEvents = 0;
	long long int numEventsWeighted = 0;
	for (int i=0; i<s._chain->GetEntries(); i++)
	{
		s._chain->GetEntry(i);
		numEvents+=meta->_nEventsProcessed;
		numEventsWeighted+=meta->_sumEventWeights;
	}
	std::cout 
		<< "#events processed total = " << numEvents << std::endl
		<< "#events weighted total = " << numEventsWeighted << std::endl;

	return numEventsWeighted;
}

void dumpEventList(ofstream& out, std::vector<int> const& v)
{
    for (std::vector<int>::const_iterator it=v.begin(); it!=v.end(); ++it)
        out << *it << std::endl;
}

void process()
{
	//	out ...
	TFile *outroot = new TFile(__outputfilename.c_str(), "recreate");
    TH1D *hEventWeights = new TH1D("eventWeights", "eventWeights", 1, 0, 1);

	//	get the total events, etc...
	long long int numEventsWeighted = sampleinfo(__inputfilename);
    hEventWeights->Fill(0.5, numEventsWeighted);

	Streamer streamer(__inputfilename, NTUPLEMAKER_NAME+"/Events");
	streamer.chainup();

    Muons *muons=NULL;
	Muons muons1;
	Muons muons2;
	Jets *jets=NULL;
    Electrons *electrons=NULL;
	Vertices *vertices=NULL;
	Event *event=NULL;
	EventAuxiliary *aux=NULL;
	MET *met=NULL;
	streamer._chain->SetBranchAddress("Muons", &muons);
	streamer._chain->SetBranchAddress("Jets", &jets);
	streamer._chain->SetBranchAddress("Vertices", &vertices);
	streamer._chain->SetBranchAddress("Event", &event);
    streamer._chain->SetBranchAddress("Electrons", &electrons);
	streamer._chain->SetBranchAddress("EventAuxiliary", &aux);
	streamer._chain->SetBranchAddress("MET", &met);

	//	Main Loop
	uint32_t numEntries = streamer._chain->GetEntries();
	for (uint32_t i=0; i<numEntries && __continueRunning; i++)
	{
        muons1.clear(); muons2.clear();
		streamer._chain->GetEntry(i);
		if (i%1000==0)
			std::cout << "### Event " << i << " / " << numEntries
				<< std::endl;

		//
		//	Selections
		//
		if (!passVertex(vertices))
			continue;
		if (!(aux->_hasHLTFired[0] || aux->_hasHLTFired[1]))
			continue;

        __nTotalOverall++;
        std::vector<std::pair<Muon, Muon> > muonPairs;
        for (Muons::iterator it=muons->begin(); it!=muons->end(); ++it)
            for (Muons::iterator jt=(it+1); jt!=muons->end(); ++jt)
                if (passMuons(*it, *jt))
                    muonPairs.push_back(std::make_pair(*it, *jt));
        if (muonPairs.size()==0 || muonPairs.size()>1) continue;

            __nTotalPassMuonSelections++;
            if (passElectronVeto(electrons, muonPairs[0].first, muonPairs[0].second) 
                && passBTaggedJetVeto(jets))
            {
                __nTotalPassElectronJetVetos++;
		        categorize(jets, muonPairs[0].first, muonPairs[0].second, *met, *event);
                vTotal.push_back(event->_event);

            }
	}

    ofstream fVBFTight, fggFT, fggFL, f01JetsTight, f01JetsLoose, fTotal;
    fVBFTight.open("VBFTight.txt");
    fggFT.open("ggFT.txt");
    fggFL.open("ggFL.txt");
    f01JetsTight.open("01JetsTight.txt");
    f01JetsLoose.open("01JetsLoose.txt");
    fTotal.open("Total.txt");
    dumpEventList(fVBFTight, vVBFTight);
    dumpEventList(fggFT, vggFTight);
    dumpEventList(fggFL, vggFLoose);
    dumpEventList(f01JetsTight, v01JetsTight);
    dumpEventList(f01JetsLoose, v01JetsLoose);
    dumpEventList(fTotal, vTotal);

	outroot->Write();
	outroot->Close();

    std::cout << "Yields: " << std::endl
        << "--------------------------------" << std::endl
        << "__nVBFTight = " << __nVBFTight << std::endl
        << "__nggFTight = " << __nggFTight << std::endl
        << "__nggFLoose = " << __nggFLoose << std::endl
        << "__n01JetsTight = " << __n01JetsTight << std::endl
        << "__n01JetsLoose = " << __n01JetsLoose << std::endl
        << "__nTotalCategorization = " << __nTotalCategorization << std::endl
        << "__nTotalOverall = " << __nTotalOverall << std::endl
        << "__nTotalPassMuonSelections = " << __nTotalPassMuonSelections << std::endl
        << "__nTotalPassElectronJetVetos = " << __nTotalPassElectronJetVetos << std::endl
        << "--------------------------------" << std::endl
        << "TOTAL = " << __nVBFTight + __nggFTight + __nggFLoose + __n01JetsLoose + __n01JetsTight << std::endl;

	return;
}

void sigHandler(int sig)
{
	cout << "### Signal: " << sig << " caughter. Exiting..." << endl;
	__continueRunning = false;
}

void printCuts()
{
    std::cout << "Cuts:" << std::endl
        << "_muonMatchedPt = " << _muonMatchedPt << std::endl
        << "_muonMatchedEta = " << _muonMatchedEta << std::endl
        << "_muonPt = " << _muonPt << std::endl
        << "_muonEta = " << _muonEta << std::endl
        << "_muonIso = " << _muonIso << std::endl
        << "_leadJetPt = " << _leadJetPt << std::endl
        << "_subleadJetPt = " << _subleadJetPt << std::endl
        << "_metPt = " << _metPt << std::endl
        << "_dijetMass_VBFTight = " << _dijetMass_VBFTight << std::endl
        << "_dijetdEta_VBFTight = " << _dijetdEta_VBFTight << std::endl
        << "_dijetMass_ggFTight = " << _dijetMass_ggFTight << std::endl
        << "_dimuonPt_ggFTight = " << _dimuonPt_ggFTight << std::endl
        << "_dimuonPt_01JetsTight = " << _dimuonPt_01JetsTight << std::endl;
}

int main(int argc, char** argv)
{
	/*
	 *	Register signals
	 */
	signal(SIGABRT, &sigHandler);
	signal(SIGTERM, &sigHandler);
	signal(SIGINT, &sigHandler);

	std::string none;
	bool genPUMC = false;

	/*
	 *	Pare Options
	 */
	po::options_description desc("Allowed Program Options");
	desc.add_options()
		("help", "produce help messages")
		("input", po::value<std::string>(), "a file specifying all the ROOT files to process")
		("isMC", po::value<bool>(), "type of data: DATA vs MC")
		("output", po::value<std::string>(), "output file name")
	;

	po::variables_map vm;
	po::store(po::parse_command_line(argc, argv, desc), vm);
	po::notify(vm);

	if (vm.count("help") || argc<2)
	{
		std::cout << desc << std::endl;
		return 1;
	}

	//	Assign globals
	__inputfilename = vm["input"].as<std::string>();
	__isMC = vm["isMC"].as<bool>();
	__outputfilename = vm["output"].as<std::string>();
    printCuts();

	//	start processing
	process();
	return 0;
}

#endif
