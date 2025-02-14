#pragma once

#include "TString.h"
#include "TH1D.h"
#include "TH2F.h" // Add this line to include TH2F
#include "TDirectory.h"

namespace analysis
{
    namespace processing
    {
        struct DimuonSet
        {
            DimuonSet(TString const& postfix)
            {
                _postfix = postfix;
            }

            void init()
            {
                gDirectory->mkdir(_postfix);
                gDirectory->cd(_postfix);
                hDiJetMass = new TH1D("DiJetMass", "DiJetMass", 20, 0, 1000);
                hDiJetMass->Sumw2(kTRUE);
                hDiJetdeta = new TH1D("DiJetdeta", "DiJetdeta", 14, 0, 7);
                hDiJetdeta->Sumw2(kTRUE);
                hDiMuonpt = new TH1D("DiMuonpt", "DiMuonpt", 100, 0, 200);
                hDiMuonpt->Sumw2(kTRUE);
                hDiMuonMass = new TH1D("DiMuonMass", "DiMuonMass", 7000, 50, 400);
                hDiMuonMass->Sumw2(kTRUE);
                hDiMuoneta = new TH1D("DiMuoneta", "DiMuoneta", 50, -2.5, 2.5);
                hDiMuoneta->Sumw2(kTRUE);
                hDiMuondphi = new TH1D("DiMuondphi", "DiMoundphi", 18, -3.6, 3.6);
                hDiMuondphi->Sumw2(kTRUE);
                hMuonpt = new TH1D("Muonpt", "Muonpt", 50, 0, 100);
                hMuonpt->Sumw2(kTRUE);
                hMuoneta = new TH1D("Muoneta", "Muoneta", 50, -2.5, 2.5);
                hMuoneta->Sumw2(kTRUE);
                hMuonphi = new TH1D("Muonphi", "Muonphi", 36, -3.6, 3.6);
                hMuonphi->Sumw2(kTRUE);
                gDirectory->cd("../");
            }

            void init2()
            {
                gDirectory->mkdir(_postfix);
                gDirectory->cd(_postfix);
                hFsrPhotonpt = new TH1F("FsrPhotonpt", "FsrPhotonpt", 75, 0, 75);
                hFsrPhotoneta = new TH1F("FsrPhotoneta", "FsrPhotoneta", 50, -2.5, 2.5);
                hFsrPhotonphi = new TH1F("FsrPhotonphi", "FsrPhotonphi", 36, -3.6, 3.6);
                hFsrPhoton_pt_vs_eta = new TH2F("FsrPhoton_pt_vs_eta", "FsrPhoton eta vs pt; eta; pT [GeV]",
                                 50, -2.5, 2.5,  // 50 bins for eta ranging from -2.5 to 2.5
                                 75, 0, 75);     // 75 bins for pT ranging from 0 to 75 GeV
		h_isPFcand_vs_eta = new TH2F("isPFcand_vs_eta", "isPFcand vs eta; eta; isPFcand",
                                    50, -2.5, 2.5,  // 50 bins for eta ranging from -2.5 to 2.5
                                    2, -0.5, 1.5);  // 2 bins for isPFcand (0 or 1)
		hPhoton_hoe = new TH1F("Photon_hoe", "Photon H over E", 100, 0, 1.0);
                gDirectory->cd("../");
            }

            ~DimuonSet() {}

            TString _postfix;
            TH1D *hDiJetMass;
            TH1D *hDiJetdeta;
            TH1D *hDiMuonpt;
            TH1D *hDiMuonMass;
            TH1D *hDiMuoneta;
            TH1D *hDiMuondphi;
            TH1D *hMuonpt;
            TH1D *hMuoneta;
            TH1D *hMuonphi;
            TH1D *hNpv;
            TH1F *hFsrPhotonpt;
            TH1F *hFsrPhotonphi;
            TH1F *hFsrPhotoneta;
            TH1F *hPhoton_hoe;
            TH2F *hFsrPhoton_pt_vs_eta; // Declare TH2F histogram
       	    TH2F *h_isPFcand_vs_eta;
       	};
    }
}
