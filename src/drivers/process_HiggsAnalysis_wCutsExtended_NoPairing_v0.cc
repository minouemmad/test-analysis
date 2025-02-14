#include "Muon.h"
#include "FsrPhoton.h"
#include "Jet.h"
#include "Vertex.h"
#include "Event.h"
#include "MET.h"
#include "Constants.h"
#include "Streamer.h"
#include "MetaHiggs.h"
#include "defs/Macros.h"

//	ROOT headers
#include "TFile.h"
#include "TChain.h"
#include "TString.h"
#include "TMath.h"
#include "TH1D.h"
#include "TH2F.h"
#include "TLorentzVector.h"
#include "LumiReweightingStandAlone.h"
#include "HistogramSets.h"

#include "boost/program_options.hpp"
#include <signal.h>

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
std::atomic<bool> __continueRunning = true;

/*
 *  Define all the Constants
 *  for Muon:
 *  lead - id 0
 *  sublead - is 1
 */
enum MuonType {kLead=0, kSubLead=1};
double _muonMatchedPt = 24.;
double _muonMatchedEta = 2.4;
double _leadmuonPt = 10.;
double _subleadmuonPt = 10.;
double _leadmuonEta = 2.4;
double _subleadmuonEta = 2.4;
double _leadmuonIso = 0.1;
double _subleadmuonIso = 0.1;
double _leadJetPt = 40.;
double _subleadJetPt = 30.;
double _metPt = 40.;
double _dijetMass_VBFTight = 650;
double _dijetdEta_VBFTight = 3.5;
double _dijetMass_ggFTight = 250.;
double _dimuonPt_ggFTight = 50.;
double _dimuonPt_01JetsTight = 10.;

//std::string const NTUPLEMAKER_NAME =  "ntuplemaker_H2DiMuonMaker";
std::string const NTUPLEMAKER_NAME =  "";

namespace po = boost::program_options;
using namespace analysis::core;
using namespace analysis::dimuon;
using namespace analysis::processing;

TH1D *hEventWeights = NULL;
DimuonSet setNoCats("NoCats");
DimuonSet set2Jets("2Jets");
DimuonSet setVBFTight("VBFTight");
DimuonSet setggFTight("ggFTight");
DimuonSet setggFLoose("ggFLoose");

DimuonSet set01Jets("01Jets");
DimuonSet set01JetsTight("01JetsTight");
DimuonSet set01JetsLoose("01JetsLoose");
DimuonSet set01JetsBB("01JetsBB");
DimuonSet set01JetsBO("01JetsBO");
DimuonSet set01JetsBE("01JetsBE");
DimuonSet set01JetsOO("01JetsOO");
DimuonSet set01JetsOE("01JetsOE");
DimuonSet set01JetsEE("01JetsEE");
DimuonSet set01JetsTightBB("01JetsTightBB");
DimuonSet set01JetsTightBO("01JetsTightBO");
DimuonSet set01JetsTightBE("01JetsTightBE");
DimuonSet set01JetsTightOO("01JetsTightOO");
DimuonSet set01JetsTightOE("01JetsTightOE");
DimuonSet set01JetsTightEE("01JetsTightEE");
DimuonSet set01JetsLooseBB("01JetsLooseBB");
DimuonSet set01JetsLooseBO("01JetsLooseBO");
DimuonSet set01JetsLooseBE("01JetsLooseBE");
DimuonSet set01JetsLooseOO("01JetsLooseOO");
DimuonSet set01JetsLooseOE("01JetsLooseOE");
DimuonSet set01JetsLooseEE("01JetsLooseEE");
DimuonSet setFsrPhotons("FsrPhotons"); 
DimuonSet setGlobalMuon("GlobalMuon");
DimuonSet setPFMuon("PFMuon");
DimuonSet setTrackerMuon("TrackerMuon");

bool isBarrel(Muon const& m)
{
	return TMath::Abs(m._eta)<0.8;
}

bool isOverlap(Muon const& m)
{
	return TMath::Abs(m._eta)>=0.8 && TMath::Abs(m._eta)<1.6;
}

bool isEndcap(Muon const& m)
{
	return TMath::Abs(m._eta)>=1.6 && TMath::Abs(m._eta)<2.1;
}

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

bool passMuon(Muon const& m, MuonType id=kLead)
{
    if (id==kLead)
    {
	    if (m._isGlobal && m._isTracker &&
		    m._pt>_leadmuonPt && TMath::Abs(m._eta)<_leadmuonEta &&
		    m._isTight /*&& (m._trackIsoSumPt/m._pt)<_leadmuonIso*/)
		    return true;
    }
    else
    {
	    if (m._isGlobal && m._isTracker &&
		    m._pt>_subleadmuonPt && TMath::Abs(m._eta)<_subleadmuonEta &&
		    m._isTight /*&& (m._trackIsoSumPt/m._pt)<_subleadmuonIso*/)
		    return true;
    }
	return false;
}

bool passMuonHLT(Muon const& m)
{
	if (/*(m._isHLTMatched[1] || m._isHLTMatched[0]) &&*/
		m._pt>_muonMatchedPt && TMath::Abs(m._eta)<_muonMatchedEta)
		return true;

	return false;
}

bool passMuons(Muon const& m1, Muon const& m2)
{
    Muon const& leadmuon = m1._pt>m2._pt ? m1 : m2;
    Muon const& subleadmuon = m1._pt<m2._pt ? m1 : m2;
	if (m1._charge!=m2._charge &&
		passMuon(leadmuon, kLead) && passMuon(subleadmuon, kSubLead))
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

void categorize(Jets* jets, Muon const& mu1, Muon const&  mu2, /*FsrPhoton const& gamma,*/
	MET const& met, Event const& event, float puweight=1.)
{
	TLorentzVector p4m1, p4m2;
	p4m1.SetPtEtaPhiM(mu1._pt, mu1._eta, 
		mu1._phi, PDG_MASS_Mu);
	p4m2.SetPtEtaPhiM(mu2._pt, mu2._eta, 
		mu2._phi, PDG_MASS_Mu);
    
	/*TLorentzVector p4g;                    //
	p4g.SetPtEtaPhiM(gamma._pt, gamma._eta,  //
		gamma._phi, 0);*/                    //
    
    //Printing out invariant mass dimuon
	double mass_muon1 = p4m1.M();
    double mass_muon2 = p4m2.M();

    TLorentzVector p4dimuon = p4m1 + p4m2;

    double mass_dimuon = p4dimuon.M();

//**************************************************************
    /*for (int imuon = 0;imuon<nMuon;imuon++){
        if(Muon_tightId == true) {
            TLorentzVector muonVec;
            muonVec.SetPtEtaPhiM(muon[imuon].pt, muon[imuon].eta, muon[imuon].phi, muon[imuon].mass);
	        setNoCats.hDiMuonpt->Fill(muonVec.Pt(), puweight);}}
    
    for (int iphoton = 0;iphoton<nFsrPhoton;iphoton++){
            cout<<iphoton<<endl;
            float fsrPhoton_pt = fsrPhotons[iphoton].pt;
            setNoCats.hDiMuonMass->Fill(nFsrPhoton_pt, puweight);}*/

//**************************************************************
	//	Fill the No Categorization Set
	double dphi = p4m1.DeltaPhi(p4m2);
	setNoCats.hDiMuonpt->Fill(p4dimuon.Pt(), puweight);
	setNoCats.hDiMuonMass->Fill(p4dimuon.M(), puweight);
	setNoCats.hDiMuoneta->Fill(p4dimuon.Eta(), puweight);
	setNoCats.hDiMuondphi->Fill(dphi, puweight);
	setNoCats.hMuonpt->Fill(p4m1.Pt(), puweight);
	setNoCats.hMuonpt->Fill(p4m2.Pt(), puweight);
	setNoCats.hMuoneta->Fill(p4m1.Eta(), puweight);
	setNoCats.hMuoneta->Fill(p4m2.Eta(), puweight);
	setNoCats.hMuonphi->Fill(p4m1.Phi(), puweight);
	setNoCats.hMuonphi->Fill(p4m2.Phi(), puweight);

    //ADDITIONAL THINGS TO PUT IN...
    //Isolation Cuts 
    //  Track/Calorimeter Isolation
    //Impact Paraimeter Cuts
    //  Transverse Impact Parameter (d_0)
    //Event Topology Cuts
    //  Number of Jets 
    //  MET
    //Muon ID and Trigger Cuts
    //  Muon Quality Cuts --> Ex. # of hits in tracker/muon system
    //  Triggers --> Ex. Single-Muon Triggers 
    //Background Rejection Cuts 
    //  Drell-Yan Processes 
    //Pileup Mitigation
   
        if (mu1._isGlobal && mu2._isGlobal) {
                setGlobalMuon.hDiMuonpt->Fill(p4dimuon.Pt(), puweight);
                setGlobalMuon.hDiMuonMass->Fill(p4dimuon.M(), puweight);
                setGlobalMuon.hDiMuoneta->Fill(p4dimuon.Eta(), puweight);
                setGlobalMuon.hDiMuondphi->Fill(dphi, puweight);
                setGlobalMuon.hMuonpt->Fill(p4m1.Pt(), puweight);
                setGlobalMuon.hMuonpt->Fill(p4m2.Pt(), puweight);
                setGlobalMuon.hMuoneta->Fill(p4m1.Eta(), puweight);
                setGlobalMuon.hMuoneta->Fill(p4m2.Eta(), puweight);
                setGlobalMuon.hMuonphi->Fill(p4m2.Phi(), puweight);
        }

	if (!(p4dimuon.M()>110 && p4dimuon.M()<160 &&
		mu1._isPF && mu2._isPF))
		return;

	if (mu1._isPF && mu2._isPF) {
   		setPFMuon.hDiMuonpt->Fill(p4dimuon.Pt(), puweight);
    		setPFMuon.hDiMuonMass->Fill(p4dimuon.M(), puweight);
    		setPFMuon.hDiMuoneta->Fill(p4dimuon.Eta(), puweight);
    		setPFMuon.hDiMuondphi->Fill(dphi, puweight);
    		setPFMuon.hMuonpt->Fill(p4m1.Pt(), puweight);
    		setPFMuon.hMuonpt->Fill(p4m2.Pt(), puweight);
    		setPFMuon.hMuoneta->Fill(p4m1.Eta(), puweight);
    		setPFMuon.hMuoneta->Fill(p4m2.Eta(), puweight);
    		setPFMuon.hMuonphi->Fill(p4m1.Phi(), puweight);
    		setPFMuon.hMuonphi->Fill(p4m2.Phi(), puweight);
	}

	if (mu1._isTracker && mu2._isTracker) {
    		setTrackerMuon.hDiMuonpt->Fill(p4dimuon.Pt(), puweight);
    		setTrackerMuon.hDiMuonMass->Fill(p4dimuon.M(), puweight);
    		setTrackerMuon.hDiMuoneta->Fill(p4dimuon.Eta(), puweight);
  		setTrackerMuon.hDiMuondphi->Fill(dphi, puweight);
    		setTrackerMuon.hMuonpt->Fill(p4m1.Pt(), puweight);
   		setTrackerMuon.hMuonpt->Fill(p4m2.Pt(), puweight);
    		setTrackerMuon.hMuoneta->Fill(p4m1.Eta(), puweight);	
    		setTrackerMuon.hMuoneta->Fill(p4m2.Eta(), puweight);
	 	setTrackerMuon.hMuonphi->Fill(p4m1.Phi(), puweight);
		setTrackerMuon.hMuonphi->Fill(p4m2.Phi(), puweight);
	}

	std::vector<TLorentzVector> p4jets;
	for (Jets::const_iterator it=jets->begin(); it!=jets->end(); ++it)
	{
		if (it->_pt>30 && TMath::Abs(it->_eta)<4.7)
		{
			if (!(jetMuondR(it->_eta, it->_phi, mu1._eta, mu1._phi)<0.3) && 
				!(jetMuondR(it->_eta, it->_phi, mu2._eta, mu2._phi)<0.3))
			{
				TLorentzVector p4;
				p4.SetPtEtaPhiM(it->_pt, it->_eta, it->_phi, it->_mass);
				p4jets.push_back(p4);
			
            // Print out the jet's properties
           /* std::cout << "Jet passing selection: " << std::endl;
            std::cout << "  pT:  " << it->_pt << " GeV" << std::endl;
            std::cout << "  eta: " << it->_eta << std::endl;
            std::cout << "  phi: " << it->_phi << std::endl;
            std::cout << "  mass: " << it->_mass << " GeV" << std::endl;*/
            }
		}
	}
//	if (p4jets.size()>2)
//		return;

	bool isPreSelected = false;
	if (p4jets.size()>=2)
	{
		TLorentzVector p4lead = p4jets[0]; 
		TLorentzVector p4sub = p4jets[1];
		TLorentzVector dijet = p4lead + p4sub;

		float deta = p4lead.Eta() - p4sub.Eta();
		float dijetmass = dijet.M();
			
		if (p4lead.Pt()>_leadJetPt && p4sub.Pt()>_subleadJetPt &&
			met._pt<_metPt)
		{
			isPreSelected = true;

			set2Jets.hDiJetMass->Fill(dijetmass, puweight);
			set2Jets.hDiJetdeta->Fill(TMath::Abs(deta), puweight);
			set2Jets.hDiMuonpt->Fill(p4dimuon.Pt(), puweight);
			set2Jets.hDiMuonMass->Fill(p4dimuon.M(), puweight);
			set2Jets.hDiMuoneta->Fill(p4dimuon.Eta(), puweight);
			set2Jets.hDiMuondphi->Fill(dphi, puweight);
			set2Jets.hMuonpt->Fill(p4m1.Pt(), puweight);
			set2Jets.hMuonpt->Fill(p4m2.Pt(), puweight);
			set2Jets.hMuoneta->Fill(p4m1.Eta(), puweight);
			set2Jets.hMuoneta->Fill(p4m2.Eta(), puweight);
			set2Jets.hMuonphi->Fill(p4m1.Phi(), puweight);
			set2Jets.hMuonphi->Fill(p4m2.Phi(), puweight);

			//	categorize
			if (dijetmass>_dijetMass_VBFTight && TMath::Abs(deta)>_dijetdEta_VBFTight)
			{
				//	VBF Tight
				setVBFTight.hDiJetMass->Fill(dijetmass, puweight);
				setVBFTight.hDiJetdeta->Fill(TMath::Abs(deta), puweight);
				setVBFTight.hDiMuonpt->Fill(p4dimuon.Pt(), puweight);
				setVBFTight.hDiMuonMass->Fill(p4dimuon.M(), puweight);
				setVBFTight.hDiMuoneta->Fill(p4dimuon.Eta(), puweight);
				setVBFTight.hDiMuondphi->Fill(dphi, puweight);
				setVBFTight.hMuonpt->Fill(p4m1.Pt(), puweight);
				setVBFTight.hMuonpt->Fill(p4m2.Pt(), puweight);
				setVBFTight.hMuoneta->Fill(p4m1.Eta(), puweight);
				setVBFTight.hMuoneta->Fill(p4m2.Eta(), puweight);
				setVBFTight.hMuonphi->Fill(p4m1.Phi(), puweight);
				setVBFTight.hMuonphi->Fill(p4m2.Phi(), puweight);
				return;
			}
			if (dijetmass>_dijetMass_ggFTight && p4dimuon.Pt()>_dimuonPt_ggFTight)
			{
				//	ggF Tight
				setggFTight.hDiJetMass->Fill(dijetmass, puweight);
				setggFTight.hDiJetdeta->Fill(TMath::Abs(deta), puweight);
				setggFTight.hDiMuonpt->Fill(p4dimuon.Pt(), puweight);
				setggFTight.hDiMuonMass->Fill(p4dimuon.M(), puweight);
				setggFTight.hDiMuoneta->Fill(p4dimuon.Eta(), puweight);
				setggFTight.hDiMuondphi->Fill(dphi, puweight);
				setggFTight.hMuonpt->Fill(p4m1.Pt(), puweight);
				setggFTight.hMuonpt->Fill(p4m2.Pt(), puweight);
				setggFTight.hMuoneta->Fill(p4m1.Eta(), puweight);
				setggFTight.hMuoneta->Fill(p4m2.Eta(), puweight);
				setggFTight.hMuonphi->Fill(p4m1.Phi(), puweight);
				setggFTight.hMuonphi->Fill(p4m2.Phi(), puweight);
				return;}
			else
			{	//	ggF Loose
				setggFLoose.hDiJetMass->Fill(dijetmass, puweight);
				setggFLoose.hDiJetdeta->Fill(TMath::Abs(deta), puweight);
				setggFLoose.hDiMuonpt->Fill(p4dimuon.Pt(), puweight);
				setggFLoose.hDiMuonMass->Fill(p4dimuon.M(), puweight);
				setggFLoose.hDiMuoneta->Fill(p4dimuon.Eta(), puweight);
				setggFLoose.hDiMuondphi->Fill(dphi, puweight);
				setggFLoose.hMuonpt->Fill(p4m1.Pt(), puweight);
				setggFLoose.hMuonpt->Fill(p4m2.Pt(), puweight);
				setggFLoose.hMuoneta->Fill(p4m1.Eta(), puweight);
				setggFLoose.hMuoneta->Fill(p4m2.Eta(), puweight);
				setggFLoose.hMuonphi->Fill(p4m1.Phi(), puweight);
				setggFLoose.hMuonphi->Fill(p4m2.Phi(), puweight);
				return;
			}
		}
	}
	if (!isPreSelected)
	{
		set01Jets.hDiMuonpt->Fill(p4dimuon.Pt(), puweight);
		set01Jets.hDiMuonMass->Fill(p4dimuon.M(), puweight);
		set01Jets.hDiMuoneta->Fill(p4dimuon.Eta(), puweight);
		set01Jets.hDiMuondphi->Fill(dphi, puweight);
		set01Jets.hMuonpt->Fill(p4m1.Pt(), puweight);
		set01Jets.hMuonpt->Fill(p4m2.Pt(), puweight);
		set01Jets.hMuoneta->Fill(p4m1.Eta(), puweight);
		set01Jets.hMuoneta->Fill(p4m2.Eta(), puweight);
		set01Jets.hMuonphi->Fill(p4m1.Phi(), puweight);
		set01Jets.hMuonphi->Fill(p4m2.Phi(), puweight);
		if (isBarrel(mu1) && isBarrel(mu2))
		{
			//	BB
			FILL_NOJETS(set01JetsBB);
		}
		else if ((isBarrel(mu1) && isOverlap(mu2)) ||
			(isBarrel(mu2) && isOverlap(mu1)))
		{
			//	BO
			FILL_NOJETS(set01JetsBO);
		}
		else if ((isBarrel(mu1) & isEndcap(mu2)) || 
			(isBarrel(mu2) && isEndcap(mu1)))
		{
			//	BE
			FILL_NOJETS(set01JetsBE);
		}
		else if (isOverlap(mu1) && isOverlap(mu2))
		{
			//	OO
			FILL_NOJETS(set01JetsOO);
		}
		else if ((isOverlap(mu1) && isEndcap(mu2)) ||
			(isOverlap(mu2) && isEndcap(mu1)))
		{
			//	OE
			FILL_NOJETS(set01JetsOE);
		}
		else if (isEndcap(mu1) && isEndcap(mu2))
		{
			//	EE
			FILL_NOJETS(set01JetsEE);
		}

		//	separate loose vs tight
		if (p4dimuon.Pt()>=_dimuonPt_01JetsTight)
		{
			//	01Jet Tight
			set01JetsTight.hDiMuonpt->Fill(p4dimuon.Pt(), puweight);
			set01JetsTight.hDiMuonMass->Fill(p4dimuon.M(), puweight);
			set01JetsTight.hDiMuoneta->Fill(p4dimuon.Eta(), puweight);
			set01JetsTight.hDiMuondphi->Fill(dphi, puweight);
			set01JetsTight.hMuonpt->Fill(p4m1.Pt(), puweight);
			set01JetsTight.hMuonpt->Fill(p4m2.Pt(), puweight);
			set01JetsTight.hMuoneta->Fill(p4m1.Eta(), puweight);
			set01JetsTight.hMuoneta->Fill(p4m2.Eta(), puweight);
			set01JetsTight.hMuonphi->Fill(p4m1.Phi(), puweight);
			set01JetsTight.hMuonphi->Fill(p4m2.Phi(), puweight);
			if (isBarrel(mu1) && isBarrel(mu2))
			{
				//	BB
				FILL_NOJETS(set01JetsTightBB);
			}
			else if ((isBarrel(mu1) && isOverlap(mu2)) ||
				(isBarrel(mu2) && isOverlap(mu1)))
			{
				//	BO
				FILL_NOJETS(set01JetsTightBO);
			}
			else if ((isBarrel(mu1) & isEndcap(mu2)) || 
				(isBarrel(mu2) && isEndcap(mu1)))
			{
				//	BE
				FILL_NOJETS(set01JetsTightBE);
			}
			else if (isOverlap(mu1) && isOverlap(mu2))
			{
				//	OO
				FILL_NOJETS(set01JetsTightOO);
			}
			else if ((isOverlap(mu1) && isEndcap(mu2)) ||
				(isOverlap(mu2) && isEndcap(mu1)))
			{
				//	OE
				FILL_NOJETS(set01JetsTightOE);
			}
			else if (isEndcap(mu1) && isEndcap(mu2))
			{
				//	EE
				FILL_NOJETS(set01JetsTightEE);
			}
			return;
		}
		else
		{
			//	01Jet Loose
			set01JetsLoose.hDiMuonpt->Fill(p4dimuon.Pt(), puweight);
			set01JetsLoose.hDiMuonMass->Fill(p4dimuon.M(), puweight);
			set01JetsLoose.hDiMuoneta->Fill(p4dimuon.Eta(), puweight);
			set01JetsLoose.hDiMuondphi->Fill(dphi, puweight);
			set01JetsLoose.hMuonpt->Fill(p4m1.Pt(), puweight);
			set01JetsLoose.hMuonpt->Fill(p4m2.Pt(), puweight);
			set01JetsLoose.hMuoneta->Fill(p4m1.Eta(), puweight);
			set01JetsLoose.hMuoneta->Fill(p4m2.Eta(), puweight);
			set01JetsLoose.hMuonphi->Fill(p4m1.Phi(), puweight);
			set01JetsLoose.hMuonphi->Fill(p4m2.Phi(), puweight);
			if (isBarrel(mu1) && isBarrel(mu2))
			{
				//	BB
				FILL_NOJETS(set01JetsLooseBB);
			}
			else if ((isBarrel(mu1) && isOverlap(mu2)) ||
				(isBarrel(mu2) && isOverlap(mu1)))
			{
				//	BO
				FILL_NOJETS(set01JetsLooseBO);
			}
			else if ((isBarrel(mu1) & isEndcap(mu2)) || 
				(isBarrel(mu2) && isEndcap(mu1)))
			{
				//	BE
				FILL_NOJETS(set01JetsLooseBE);
			}
			else if (isOverlap(mu1) && isOverlap(mu2))
			{
				//	OO
				FILL_NOJETS(set01JetsLooseOO);
			}
			else if ((isOverlap(mu1) && isEndcap(mu2)) ||
				(isOverlap(mu2) && isEndcap(mu1)))
			{
				//	OE
				FILL_NOJETS(set01JetsLooseOE);
			}
			else if (isEndcap(mu1) && isEndcap(mu2))
			{
				//	EE
				FILL_NOJETS(set01JetsLooseEE);
			}
			return;
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

void generatePUMC()
{
    std::cout << "### Generate PU MC file...." << std::endl;
    TFile *pufile = new TFile(__puMCfilename.c_str(), "recreate");
    
    // Declare the first histogram with a unique name
    TH1D *h_pileup = new TH1D("pileup", "pileup", 99, 0, 99);

    // Declare the second histogram with a unique name
    //TH2F *h_fsr_pt_eta = new TH2F("fsr_pt_eta", "FSR Photon eta vs pT", 50, -2.5, 2.5, 75, 0, 75);

    //TH2F *h_isPFcand_vs_eta = new TH2F("isPFcand_vs_eta", "isPFcand vs eta; eta; isPFcand", 50, -2.5, 2.5, 2, -0.5, 1.5);

    Streamer s(__inputfilename, NTUPLEMAKER_NAME+"/Events");
    s.chainup();

    EventAuxiliary *aux = NULL;
    s._chain->SetBranchAddress("EventAuxiliary", &aux);
    int numEvents = s._chain->GetEntries();
    for (uint32_t i = 0; i < numEvents; i++)
    {
        s._chain->GetEntry(i);
        h_pileup->Fill(aux->_nPU, aux->_genWeight);
    }

    pufile->Write();
    pufile->Close();
}

void process()
{
	int totalEvents = 0;
	int passedEvents = 0;

	//	out ...
	TFile *outroot = new TFile(__outputfilename.c_str(), "recreate");
    hEventWeights = new TH1D("eventWeights", "eventWeights", 1, 0, 1);
	setNoCats.init();
	set2Jets.init();
	set01Jets.init();
	setVBFTight.init();
	setggFTight.init();
	setggFLoose.init();
	set01JetsTight.init();
	set01JetsLoose.init();
	set01JetsBB.init();
	set01JetsBO.init();
	set01JetsBE.init();
	set01JetsOO.init();
	set01JetsOE.init();
	set01JetsEE.init();
	set01JetsTightBB.init();
	set01JetsTightBO.init();
	set01JetsTightBE.init();
	set01JetsTightOO.init();
	set01JetsTightOE.init();
	set01JetsTightEE.init();
	set01JetsLooseBB.init();
	set01JetsLooseBO.init();
	set01JetsLooseBE.init();
	set01JetsLooseOO.init();
	set01JetsLooseOE.init();
	set01JetsLooseEE.init();
    	setFsrPhotons.init(); 
    	setFsrPhotons.init2();   
    	setGlobalMuon.init(); 
	setPFMuon.init();
	setTrackerMuon.init();
	//	get the total events, etc...
#if 0
	long long int numEventsWeighted = sampleinfo(__inputfilename);
#endif
    /// TODO understand what this is
    /// how was filled
    /// how used downstream
    auto numEventsWeighted = 100;
    hEventWeights->Fill(0.5, numEventsWeighted);

	//	generate the MC Pileup histogram
	if (__genPUMC && __isMC)
		generatePUMC();

	Streamer streamer(__inputfilename, "Events");
	streamer.chainup();

    Muons *muons=NULL;
	Muons muons1;
	Muons muons2;
	Jets *jets=NULL;
    FsrPhotons *fsrphotons=NULL;
	Vertices *vertices=NULL;
	Event *event=NULL;
	EventAuxiliary *aux=NULL;
	MET *met=NULL;

#define ROOT_CHAIN streamer._chain

    SET_BRANCH_UINT(nMuon);
    SET_BRANCH_FLOAT_ARRAY(Muon_pt);
    SET_BRANCH_FLOAT_ARRAY(Muon_phi);
    SET_BRANCH_FLOAT_ARRAY(Muon_eta);
    SET_BRANCH_INT_ARRAY(Muon_charge);
    SET_BRANCH_BOOL_ARRAY(Muon_isGlobal);
    SET_BRANCH_BOOL_ARRAY(Muon_isTracker);
    //SET_BRANCH_UCHAR_ARRAY(Muon_miniIsoId);
    SET_BRANCH_BOOL_ARRAY(Muon_tightId);
    SET_BRANCH_BOOL_ARRAY(Muon_isPFcand);

    SET_BRANCH_FLOAT(PV_ndof);
    SET_BRANCH_FLOAT(PV_z);

    SET_BRANCH_FLOAT(MET_pt);

    SET_BRANCH_UINT(nJet);
    SET_BRANCH_FLOAT_ARRAY(Jet_eta);
    SET_BRANCH_FLOAT_ARRAY(Jet_mass);
    SET_BRANCH_FLOAT_ARRAY(Jet_pt);
    SET_BRANCH_FLOAT_ARRAY(Jet_phi);

    SET_BRANCH_UINT(nFsrPhoton);
    SET_BRANCH_FLOAT_ARRAY(FsrPhoton_pt);
    SET_BRANCH_FLOAT_ARRAY(FsrPhoton_phi);
    SET_BRANCH_FLOAT_ARRAY(FsrPhoton_eta);
    SET_BRANCH_INT_ARRAY(FsrPhoton_muonIdx);
    TH1F *fsr_pt = new TH1F("fsr_pt","FSR Photon pt",75,0,75);
    TH1F *fsr_eta = new TH1F("fsr_eta","FSR Photon eta",50,-4.5,4.5);
    TH1F *fsr_phi = new TH1F("fsr_phi","FSR Photon phi",36,-3.6,3.6);

    TH2F *fsr_pt_eta = new TH2F("fsr_pt_eta", "FSR Photon eta vs pT; eta; pT [GeV]",
                            50, -2.5, 2.5,  // 50 bins for eta ranging from -2 to 2.5
                            75, 0, 75);     // 75 bins for pT ranging from 0 to 75 GeV

    TH2F *h_isPFcand_vs_eta = new TH2F("isPFcand_vs_eta", "isPFcand vs eta; eta; isPFcand",
                                    50, -2.5, 2.5,  // 50 bins for eta ranging from -2.5 to 2.5
                                    2, -0.5, 1.5);  // 2 bins for isPFcand (0 or 1)
    SET_BRANCH_UINT(nPhoton);
    SET_BRANCH_FLOAT_ARRAY(Photon_pt);
    SET_BRANCH_FLOAT_ARRAY(Photon_eta);
    SET_BRANCH_FLOAT_ARRAY(Photon_phi);
    TH1F *gamma_pt = new TH1F("gamma_pt","Photon pt",150,0,150);
    TH1F *gamma_eta = new TH1F("gamma_eta","Photon pt",50,-2.5,2.5);
    TH1F *gamma_phi = new TH1F("gamma_phi","Photon pt",36,-3.6,3.6);

    muons = new std::vector<Muon>();
    jets = new std::vector<Jet>();
    fsrphotons = new std::vector<FsrPhoton>();
    vertices = new std::vector<Vertex>();
    met = new MET();

#if 0
	streamer._chain->SetBranchAddress("Muons", &muons);
	streamer._chain->SetBranchAddress("Jets", &jets);
	streamer._chain->SetBranchAddress("FsrPhotons", &fsrphotons);
	streamer._chain->SetBranchAddress("Vertices", &vertices);
	streamer._chain->SetBranchAddress("Event", &event);
	streamer._chain->SetBranchAddress("EventAuxiliary", &aux);
	streamer._chain->SetBranchAddress("MET", &met);
#endif

	//	init the PU reweighter
	reweight::LumiReWeighting *weighter = NULL;
	if (__isMC)
	{
		std::cout << "mcPU=" << __puMCfilename << "  dataPU="
			<< __puDATAfilename << std::endl;
		TString mc_pileupfile = __puMCfilename.c_str();
		TString data_pileupfile = __puDATAfilename.c_str();
		weighter = new reweight::LumiReWeighting(
		mc_pileupfile.Data(), data_pileupfile.Data(), "pileup", "pileup");
	}

	//	Main Loop
	uint32_t numEntries = streamer._chain->GetEntries();
	for (uint32_t i=0; i<numEntries && __continueRunning; i++)
	{   int jjj=i;//if(i>1e5) continue;
        muons->clear();
        jets->clear();
        fsrphotons->clear();
        vertices->clear();
        muons1.clear(); muons2.clear();
		streamer._chain->GetEntry(i);
totalEvents++;

	// cut: at least one FSR photon with pt > 20 GeV
	bool passesCuts = false;
	for (const auto& fsrphoton : *fsrphotons) {
    		if (fsrphoton._pt > 20.0) {
        		passesCuts = true;
        		break;
    		}
	}

	if (passesCuts) {
    		passedEvents++;
	}

	// In the loop where muons are processed
	for (size_t i = 0; i < nMuon; i++) {
    		Muon mu;
    		mu._pt = Muon_pt[i];
    		mu._eta = Muon_eta[i];
    		mu._phi = Muon_phi[i];
    		mu._charge = Muon_charge[i];
    		mu._isGlobal = Muon_isGlobal[i];
    		mu._isTracker = Muon_isTracker[i];
    		mu._isTight = Muon_tightId[i];
    		mu._isPF = Muon_isPFcand[i];
    		muons->push_back(std::move(mu));

    		// Fill the 2D histogram
    		h_isPFcand_vs_eta->Fill(mu._eta, mu._isPF);
	}	

        for (size_t i=0; i<nJet; i++) {
            Jet jet;
            jet._pt = Jet_pt[i];
            jet._eta = Jet_eta[i];
            jet._phi = Jet_phi[i];
            jet._mass = Jet_mass[i];
            jets->push_back(std::move(jet));
        }

	for (size_t i = 0; i < nFsrPhoton; i++) {
    		FsrPhoton fsrphoton;
    		fsrphoton._pt = FsrPhoton_pt[i];
    		fsrphoton._eta = FsrPhoton_eta[i];
    		fsrphoton._phi = FsrPhoton_phi[i];
  		fsrphoton._muonIdx = FsrPhoton_muonIdx[i];
   		fsrphotons->push_back(std::move(fsrphoton));

    		// Fill the 2D histogram
    		fsr_pt_eta->Fill(fsrphoton._eta, fsrphoton._pt); // Use fsr_pt_eta
	}
        Vertex vtx;
        vtx._ndf = PV_ndof;
        vtx._z = PV_z;
        vertices->push_back(std::move(vtx));

        met->_pt = MET_pt;

		if (i%1000==0)
			std::cout << "### Event " << i << " / " << numEntries
				<< std::endl;

		float puweight = __isMC ? weighter->weight(aux->_nPU)*aux->_genWeight :
			1.;

		//
		//	Selections
		//
		if (!passVertex(vertices))
			continue;
            #if 0
		if (!(aux->_hasHLTFired[0] || aux->_hasHLTFired[1]))
			continue;
            #endif
        //  prepare the pairs of muons
        for (analysis::core::Muons::const_iterator it=muons->begin();
            it!=muons->end(); ++it)
            for (analysis::core::Muons::const_iterator jt=(it+1);
                jt!=muons->end(); ++jt)
            {
                muons1.push_back(*it); muons2.push_back(*jt);
            }
		for (uint32_t im=0; im<muons1.size(); im++)
		{
			if (!passMuons(muons1.at(im), muons2.at(im)))
				continue;
			categorize(jets, muons1.at(im), muons2.at(im), *met, *event,
				puweight);
		}

	// Process FSR photons
        for (size_t ifsr = 0; ifsr < nFsrPhoton; ifsr++) {
            fsr_pt->Fill(FsrPhoton_pt[ifsr]);
            fsr_eta->Fill(FsrPhoton_eta[ifsr]);
            fsr_phi->Fill(FsrPhoton_phi[ifsr]);

            // Fill 1D histograms
            setFsrPhotons.hFsrPhotonpt->Fill(FsrPhoton_pt[ifsr], puweight);
            setFsrPhotons.hFsrPhotoneta->Fill(FsrPhoton_eta[ifsr], puweight);
            setFsrPhotons.hFsrPhotonphi->Fill(FsrPhoton_phi[ifsr], puweight);

            // Fill 2D histogram
            setFsrPhotons.hFsrPhoton_pt_vs_eta->Fill(FsrPhoton_pt[ifsr], FsrPhoton_eta[ifsr], puweight);
        }

        for(int ig=0;ig<nPhoton;ig++)
        {
            gamma_pt->Fill(Photon_pt[ig]);
        }
	}

	double efficiency = (totalEvents > 0) ? (static_cast<double>(passedEvents) / totalEvents) * 100.0 : 0.0;
	std::cout << "Total Events: " << totalEvents << std::endl;
	std::cout << "Passed Events: " << passedEvents << std::endl;
	std::cout << "Efficiency: " << efficiency << "%" << std::endl;

	fsr_pt_eta->Write();
	h_isPFcand_vs_eta->Write();
	outroot->Write();
	outroot->Close();

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
        << "_leadmuonPt = " << _leadmuonPt << std::endl
        << "_subleadmuonPt = " << _subleadmuonPt << std::endl
        << "_leadmuonEta = " << _leadmuonEta << std::endl
        << "_subleadmuonEta = " << _subleadmuonEta << std::endl
        << "_leadmuonIso = " << _leadmuonIso << std::endl
        << "_subleadmuonIso = " << _subleadmuonIso << std::endl
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
		("genPUMC", po::value<bool>(&genPUMC)->default_value(false), "true if should generate the MC PU file")
		("puMC", po::value<std::string>(&none)->default_value("None"), "MC PU Reweight file")
		("puDATA", po::value<std::string>(&none)->default_value("None"), "DATA PU Reweight file")
        ("muonMatchedPt", po::value<double>(&_muonMatchedPt)->default_value(_muonMatchedPt), "Muon Matched Pt Cut")
        ("muonMatchedEta", po::value<double>(&_muonMatchedEta)->default_value(_muonMatchedEta), "Muon Matched Eta Cut")
        ("leadmuonPt", po::value<double>(&_leadmuonPt)->default_value(_leadmuonPt), "Muon Pt Cut")
        ("subleadmuonPt", po::value<double>(&_subleadmuonPt)->default_value(_subleadmuonPt), "Muon Pt Cut")
        ("leadmuonEta", po::value<double>(&_leadmuonEta)->default_value(_leadmuonEta), "Muon Eta Cut")
        ("subleadmuonEta", po::value<double>(&_subleadmuonEta)->default_value(_subleadmuonEta), "Muon Eta Cut")
        ("leadmuonIso", po::value<double>(&_leadmuonIso)->default_value(_leadmuonIso), "Muon Isolation Cut")
        ("subleadmuonIso", po::value<double>(&_subleadmuonIso)->default_value(_subleadmuonIso), "Muon Isolation Cut")
        ("leadJetPt", po::value<double>(&_leadJetPt)->default_value(_leadJetPt), "Lead Jet Pt Cut")
        ("subleadJetPt", po::value<double>(&_subleadJetPt)->default_value(_subleadJetPt), "SubLeading Jet Pt Cut")
        ("metPt", po::value<double>(&_metPt)->default_value(_metPt), "MET Pt Cut")
        ("dijetMass_VBFTight", po::value<double>(&_dijetMass_VBFTight)->default_value(_dijetMass_VBFTight), "DiJet Mass VBFTight-Category Cut")
        ("dijetdEta_VBFTight", po::value<double>(&_dijetdEta_VBFTight)->default_value(_dijetdEta_VBFTight), "DiJet deta VBFTight-Category Cut")
        ("dijetMass_ggFTight", po::value<double>(&_dijetMass_ggFTight)->default_value(_dijetMass_ggFTight), "DiJet Mass ggFTight-Category Cut")
        ("dimuonPt_ggFTight", po::value<double>(&_dimuonPt_ggFTight)->default_value(_dimuonPt_ggFTight), "DiMuon Pt ggFTight-Category Cut")
        ("dimuonPt_01JetsTight", po::value<double>(&_dimuonPt_01JetsTight)->default_value(_dimuonPt_01JetsTight), "DiMuon Pt 01JetsTight-Category Cut")
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
	__genPUMC = vm["genPUMC"].as<bool>();
	__puMCfilename = vm["puMC"].as<std::string>();
	__puDATAfilename = vm["puDATA"].as<std::string>();
    _muonMatchedPt = vm["muonMatchedPt"].as<double>();
    _muonMatchedEta = vm["muonMatchedEta"].as<double>();
    _leadmuonPt = vm["leadmuonPt"].as<double>();
    _subleadmuonPt = vm["subleadmuonPt"].as<double>();
    _leadmuonEta = vm["leadmuonEta"].as<double>();
    _subleadmuonEta = vm["subleadmuonEta"].as<double>();
    _leadmuonIso = vm["leadmuonIso"].as<double>();
    _subleadmuonIso = vm["subleadmuonIso"].as<double>();
    _leadJetPt = vm["leadJetPt"].as<double>();
    _subleadJetPt = vm["subleadJetPt"].as<double>();
    _metPt = vm["metPt"].as<double>();
    _dijetMass_VBFTight = vm["dijetMass_VBFTight"].as<double>();
    _dijetdEta_VBFTight = vm["dijetdEta_VBFTight"].as<double>();
    _dijetMass_ggFTight = vm["dijetMass_ggFTight"].as<double>();
    _dimuonPt_ggFTight = vm["dimuonPt_ggFTight"].as<double>();
    _dimuonPt_01JetsTight = vm["dimuonPt_01JetsTight"].as<double>();
    printCuts();

	//	start processing
	process();
	return 0;
}
