
//	
#ifdef STANDALONE
#include "Muon.h"

//	ROOT headers
#include "TFile.h"
#include "TChain.h"
#include "TString.h"

void test_dimuon()
{
	
	TString filename = "/Users/vk/software/HiggsAnalysis/files/data/dimuon/ntuplemaking_singleMuon_Run2016B_PromptReco_v2_MINIAOD.root";
	TChain *chain = new TChain("ntuplemaker_H2DiMuonMaker/Events");
	chain->Add(filename);
	std::cout << "Total #Entries: "<< chain->GetEntries() << std::endl;

	using namespace analysis::core;
	Muons *muons1=NULL;
	chain->SetBranchAddress("Muons1", &muons1);
	for (int i=0; i<chain->GetEntries(); i++)
	{
		chain->GetEntry(i);
		std::cout << "#Muons1: " << muons1->size() << std::endl;
		int n = 0;
		for (Muons::const_iterator it=muons1->begin(); it!=muons1->end(); ++it)
		{
			std::cout << it->_pt << "  " << it->_pterr << "  "
				<< it->_eta << "  " << it->_phi << std::endl;
			n++;
		}
		std::cout << "#muons=" << n << std::endl;
	}
	std::cout << "Total #Events: " << chain->GetEntries() << std::endl;

	return;
}

int main(int argc, char** argv)
{
	test_dimuon();
	return 0;
}

#endif
