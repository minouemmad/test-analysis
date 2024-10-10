#ifdef STANDALONE

#include <iostream>
#include <fstream>
#include <vector>

#include "TCanvas.h"
#include "TFile.h"
#include "TChain.h"
#include "TH2D.h"
#include "TString.h"
#include "TPaveStats.h"
#include "TF1.h"
#include "TLegend.h"
#include "TStyle.h"

#include "Streamer.h"
#include "QIE10Frame.h"
#include "QIE8Frame.h"

// NEEDS UPDATING
double adc2fC_QIE10[256]={
  // - - - - - - - range 0 - - - - - - - -
  //subrange0 
  1.58, 4.73, 7.88, 11.0, 14.2, 17.3, 20.5, 23.6, 
  26.8, 29.9, 33.1, 36.2, 39.4, 42.5, 45.7, 48.8,
  //subrange1
  53.6, 60.1, 66.6, 73.0, 79.5, 86.0, 92.5, 98.9,
  105, 112, 118, 125, 131, 138, 144, 151,
  //subrange2
  157, 164, 170, 177, 186, 199, 212, 225,
  238, 251, 264, 277, 289, 302, 315, 328,
  //subrange3
  341, 354, 367, 380, 393, 406, 418, 431,
  444, 464, 490, 516, 542, 568, 594, 620,

  // - - - - - - - range 1 - - - - - - - -
  //subrange0
  569, 594, 619, 645, 670, 695, 720, 745,
  771, 796, 821, 846, 871, 897, 922, 947,
  //subrange1
  960, 1010, 1060, 1120, 1170, 1220, 1270, 1320,
  1370, 1430, 1480, 1530, 1580, 1630, 1690, 1740,
  //subrange2
  1790, 1840, 1890, 1940,  2020, 2120, 2230, 2330,
  2430, 2540, 2640, 2740, 2850, 2950, 3050, 3150,
  //subrange3
  3260, 3360, 3460, 3570, 3670, 3770, 3880, 3980,
  4080, 4240, 4450, 4650, 4860, 5070, 5280, 5490,
  
  // - - - - - - - range 2 - - - - - - - - 
  //subrange0
  5080, 5280, 5480, 5680, 5880, 6080, 6280, 6480,
  6680, 6890, 7090, 7290, 7490, 7690, 7890, 8090,
  //subrange1
  8400, 8810, 9220, 9630, 10000, 10400, 10900, 11300,
  11700, 12100, 12500, 12900, 13300, 13700, 14100, 14500,
  //subrange2
  15000, 15400, 15800, 16200, 16800, 17600, 18400, 19300,
  20100, 20900, 21700, 22500, 23400, 24200, 25000, 25800,
  //subrange3
  26600, 27500, 28300, 29100, 29900, 30700, 31600, 32400,
  33200, 34400, 36100, 37700, 39400, 41000, 42700, 44300,

  // - - - - - - - range 3 - - - - - - - - -
  //subrange0
  41100, 42700, 44300, 45900, 47600, 49200, 50800, 52500,
  54100, 55700, 57400, 59000, 60600, 62200, 63900, 65500,
  //subrange1
  68000, 71300, 74700, 78000, 81400, 84700, 88000, 91400,
  94700, 98100, 101000, 105000, 108000, 111000, 115000, 118000,
  //subrange2
  121000, 125000, 128000, 131000, 137000, 145000, 152000, 160000,
  168000, 176000, 183000, 191000, 199000, 206000, 214000, 222000,
  //subrange3
  230000, 237000, 245000, 253000, 261000, 268000, 276000, 284000,
  291000, 302000, 316000, 329000, 343000, 356000, 370000, 384000

};

void analyze(std::string const& inputname);

int main(int argc, char** argv)
{
	analyze(argv[1]);
		
	return 0;
}

void analyze(std::string const& inputname)
{
	TFile *out = new TFile("analyzeqie10.root", "recreate");
	TH1D *hADC_30_4 = new TH1D("ADC_30_4", 
		"ADCDistribution_30_4", 256, 0, 256);
	TH1D *hADC_30_2 = new TH1D("ADC_30_2", 
		"ADCDistribution_30_4", 256, 0, 256);
	TH1D *hADC_34_4 = new TH1D("ADC_34_4", 
		"ADCDistribution_34_4", 256, 0, 256);
	TH1D *hADC_34_2 = new TH1D("ADC_34_2", 
		"ADCDistribution_34_4", 256, 0, 256);

	//	fC
	TH1D *hfC_30_2 = new TH1D("fC_30_2", 
		"fCDistribution_30_2", 200, 0, 1000);
	TH1D *hfC_30_4 = new TH1D("fC_30_4", 
		"fCDistribution_30_4", 200, 0, 1000);
	TH1D *hfC_34_2 = new TH1D("fC_34_2", 
		"fCDistribution_34_2", 200, 0, 1000);
	TH1D *hfC_34_4 = new TH1D("fC_34_4", 
		"fCDistribution_34_4", 200, 0, 1000);

	//	fC after PED 
	TH1D *hfC_30_2_PED = new TH1D("fC_30_2_PED", "fC_30_2_PED", 200, -50, 1000);
	TH1D *hfC_30_4_PED = new TH1D("fC_30_4_PED", "fC_30_4_PED", 200, -50, 1000);
	TH1D *hfC_34_2_PED = new TH1D("fC_34_2_PED", "fC_34_2_PED", 200, -50, 1000);
	TH1D *hfC_34_4_PED = new TH1D("fC_34_4_PED", "fC_34_4_PED", 200, -50, 1000);

	//	TDC
	TH1D *hTDC_30_2 = new TH1D("TDC_30_2", "TDC_30_2", 64, 0, 64);
	TH1D *hTDC_30_4 = new TH1D("TDC_30_4", "TDC_30_4", 64, 0, 64);
	TH1D *hTDC_34_2 = new TH1D("TDC_34_2", "TDC_34_2", 64, 0, 64);
	TH1D *hTDC_34_4 = new TH1D("TDC_34_4", "TDC_34_4", 64, 0, 64);

	//	ADC Correlation
	TH2D *hADCC_30 = new TH2D("ADCC_30", "ADCC_30", 256, 0, 256, 256, 0, 256);
	TH2D *hADCC_34 = new TH2D("ADCC_34", "ADCC_34", 256, 0, 256, 256, 0, 256);

	//	fC Correlation
	TH2D *hfCC_30 = new TH2D("ADCC_30", "ADCC_30", 200, 0, 1000, 200, 0, 1000);
	TH2D *hfCC_34 = new TH2D("ADCC_34", "ADCC_34", 200, 0, 1000, 200, 0, 1000);
	
	//	fC PED Correlation
	TH2D *hfCC_PED_30 = new TH2D("fCC_PED_30", "fCC_PED_30", 200, -50, 1000, 200, -50, 1000);
	TH2D *hfCC_PED_34 = new TH2D("fCC_PED_34", "fCC_PED_34", 200, -50, 1000, 200, -50, 1000);

	//	Correlation for TDClt60
	TH2D *hfCC_PED_30_TDClt60 = new TH2D("fCC_PED_30_TDClt60", "fCC_PED_30", 200, -50, 1000, 200, -50, 1000);
	TH2D *hfCC_PED_34_TDClt60 = new TH2D("fCC_PED_34_TDClt60", "fCC_PED_34", 200, -50, 1000, 200, -50, 1000);

	//	TDC Correlation
	TH2D *hTDCC_30 = new TH2D("TDCC_30", "TDCC_30", 64, 0, 64, 64, 0, 64);
	TH2D *hTDCC_34 = new TH2D("TDCC_34", "TDCC_34", 64, 0, 64, 64, 0, 64);

	//	Ratios
	TH1D *hR_30 = new TH1D("R_30", "R_30", 100, 0, 2);
	TH1D *hR_34 = new TH1D("R_34", "R_34", 100, 0, 2);

	//	Correlations for TDC id
	TH2D *hfCC_30_TDCCut2 = new TH2D("fCC_30_TDCCut2", "fCC_30_TDCCut2",
		200, -50, 1000, 200, -50, 1000);	
	TH2D *hfCC_30_TDCCut4 = new TH2D("fCC_30_TDCCut4", "fCC_30_TDCCut4",
		200, -50, 1000, 200, -50, 1000);	
	TH2D *hfCC_34_TDCCut2 = new TH2D("fCC_34_TDCCut2", "fCC_34_TDCCut2",
		200, -50, 1000, 200, -50, 1000);	
	TH2D *hfCC_34_TDCCut4 = new TH2D("fCC_34_TDCCut4", "fCC_34_TDCCut4",
		200, -50, 1000, 200, -50, 1000);	

	TH1D *hM1_30 = new TH1D("M1_30", "M1_30", 200, 0, 100);
	TH1D *hM2_30 = new TH1D("M2_30", "M2_30", 200, 0, 1);
	TH1D *hM3_30 = new TH1D("M3_30", "M3_30", 200, 0, 1);
	TH1D *hM4_30 = new TH1D("M4_30", "M4_30", 200, 0, 1);
	TH1D *hM1_34 = new TH1D("M1_34", "M1_34", 200, 0, 100);
	TH1D *hM2_34 = new TH1D("M2_34", "M2_34", 200, 0, 1);
	TH1D *hM3_34 = new TH1D("M3_34", "M3_34", 200, 0, 1);
	TH1D *hM4_34 = new TH1D("M4_34", "M4_34", 200, 0, 1);

	TH1D *hS_30 = new TH1D("S_30", "S_30", 400, 0, 2000);
	TH1D *hSM1_30 = new TH1D("SM1_30", "SM1_30", 400, 0, 2000);
	TH1D *hSM2_30 = new TH1D("SM2_30", "SM2_30", 400, 0, 2000);
	TH1D *hSM3_30 = new TH1D("SM3_30", "SM3_30", 400, 0, 2000);
	TH1D *hSM4_30 = new TH1D("SM4_30", "SM4_30", 400, 0, 2000);

	TH1D *hS_34 = new TH1D("S_34", "S_34", 400, 0, 2000);
	TH1D *hSM1_34 = new TH1D("SM1_34", "SM1_34", 400, 0, 2000);
	TH1D *hSM2_34 = new TH1D("SM2_34", "SM2_34", 400, 0, 2000);
	TH1D *hSM3_34 = new TH1D("SM3_34", "SM3_34", 400, 0, 2000);
	TH1D *hSM4_34 = new TH1D("SM4_34", "SM4_34", 400, 0, 2000);

	TH1D *hTDC_30_2_ALL = new TH1D ("TDC_30_2_ALL", "TDC_30_2_ALL", 64, 0, 64);
	TH1D *hTDC_30_2_PMT = new TH1D ("TDC_30_2_PMT", "TDC_30_2_PMT", 64, 0, 64);
	TH1D *hTDC_30_4_ALL = new TH1D ("TDC_30_4_ALL", "TDC_30_4_ALL", 64, 0, 64);
	TH1D *hTDC_30_4_PMT = new TH1D ("TDC_30_4_PMT", "TDC_30_4_PMT", 64, 0, 64);
	
	TH1D *hTDC_34_2_ALL = new TH1D ("TDC_34_2_ALL", "TDC_34_2_ALL", 64, 0, 64);
	TH1D *hTDC_34_2_PMT = new TH1D ("TDC_34_2_PMT", "TDC_34_2_PMT", 64, 0, 64);
	TH1D *hTDC_34_4_ALL = new TH1D ("TDC_34_4_ALL", "TDC_34_4_ALL", 64, 0, 64);
	TH1D *hTDC_34_4_PMT = new TH1D ("TDC_34_4_PMT", "TDC_34_4_PMT", 64, 0, 64);

	/*
	TH1D *hfC1_new = new TH1D("fC1_new", "fC1_new", 40, 0, 200);
	TH1D *hfC2_new = new TH1D("fC2_new", "fC2_new", 40, 0, 200);
	TH1D *hRatio_PED = new TH1D("Ratio_PED", "Ratio_PED", 200, 0, 2);
	TH1D *hRatio_new = new TH1D("Ratio_new", "Ratio_new", 200, 0, 2);
	TH2D *hCorrelation = new TH2D("Correlation", "Correlation",
		40, 0, 200, 40, 0, 200);
	TH2D *hADCCorrelationTS2 = new TH2D("ADCCorrelationTS2", "ADCCorrelationTS2",
		256, 0, 256, 256, 0, 256);
	TH2D *hADCCorrelationTS2_TDClt60 = new TH2D("ADCCorrelationTS2_TDClt60", "ADCCorrelationTS2_TDClt60",
		256, 0, 256, 256, 0, 256);
	TH1D *hfC1_fC2zs = new TH1D("fC1_fC2zs", "fC1_fC2zs",
		40, 0, 200);
	TH1D *hfC2_fC1zs = new TH1D("fC2_fC1zs", "fC2_fC1zs",
		40, 0, 200);
	TH1D *hTDC1_PMT = new TH1D("TDC1_PMT", "TDC1_PMT",
		64, 0, 64);
	TH1D *hTDC2_PMT = new TH1D("TDC2_PMT", "TDC2_PMT",
		64, 0, 64);
	TH1D *hTDC1_ALL = new TH1D("TDC1_ALL", "TDC1_ALL",
		64, 0, 64);
	TH1D *hTDC2_ALL = new TH1D("TDC2_ALL", "TDC2_ALL",
		64, 0, 64);
	TH2D *hTDCvsfC_1 = new TH2D("TDCvsfC1", "TDCvsfC1",
		50, -50, 200, 64, 0, 64);
	TH2D *hTDCvsfC_2 = new TH2D("TDCvsfC2", "TDCvsfC2",
		50, -50, 200, 64,0,64);
	TH2D *hTDCvsfC_1_CUT = new TH2D("TDCvsfC1_CUT", "TDCvsfC1_CUT",
		50, -50, 200, 64, 0, 64);
	TH2D *hTDCvsfC_2_CUT = new TH2D("TDCvsfC2_CUT", "TDCvsfC2_CUT",
		50, -50, 200, 64,0,64);
	TH1D *hTDC1 = new TH1D("TDC1", "TDC1", 64, 0, 64);
	TH1D *hTDC2 = new TH1D("TDC2", "TDC2", 64, 0, 64);
	TH2D *hfC2vsfC1_1 = new TH2D("fC2vsfC1_1", "fC2vsfC1_1",
		40, 0, 200, 40, 0, 200);
	TH2D *hfC2vsfC1_2 = new TH2D("fC2vsfC1_2", "fC2vsfC1_2",
		40, 0, 200, 40, 0, 200);
	TH1D *hM1 = new TH1D("M1", "M1", 200, 0, 100);
	TH1D *hM2 = new TH1D("M2", "M2", 200, 0, 1);
	TH1D *hM3 = new TH1D("M3", "M3", 200, 0, 1);
	TH1D *hM4 = new TH1D("M4", "M4", 200, 0, 1);

	TH1D *hSum = new TH1D("Sum", "Sum", 40, 0, 400);
	TH1D *hSumM1 = new TH1D("SumM1", "SumM1", 40, 0, 400);
	TH1D *hSumM2 = new TH1D("SumM2", "SumM2", 40, 0, 400);
	TH1D *hSumM3 = new TH1D("SumM3", "SumM3", 40, 0, 400);
	TH1D *hSumM4 = new TH1D("SumM4", "SumM4", 40, 0, 400);
	*/

	int numl18 = 0;
	int numl18andg18 = 0;
	int mclass[4] = {0,0,0,0};
	int mclass1[4] = {0,0,0,0};
	int mclass2[4] = {0,0,0,0};


	int m1class_30 = 0;
	int m1class_tdc_30 = 0;
	int m1class_34 = 0;
	int m1class_tdc_34 = 0;

	using namespace analysis::core;
	using namespace analysis::processing;
	Streamer s(inputname, "maker/Events");
	s.chainup();
	ofstream out_txt("events_qie10.txt");
	out_txt << "fC1    fC2    tdc1    tdc2    m1    m2    m3    m4"
		<< std::endl;

	QIE10Digis *qie10s = NULL;
	s._chain->SetBranchAddress("QIE10Digis", &qie10s);

	ofstream out_30("events_30.txt");
	ofstream out_34("events_34.txt");

	for (int i=0; i<s._chain->GetEntries(); i++)
	{
		s._chain->GetEntry(i);
		if (i%1000==0)
			std::cout << "### Event " << i << std::endl;

		QIE10Frame df302, df304, df342, df344;
		for (QIE10Digis::const_iterator it=qie10s->begin();
			it!=qie10s->end(); ++it)
		{
			if (it->_iphi==39 && it->_ieta==34)
			{
				if (it->_depth==2)
				{
					for (int k=0; k<4; k++)
					{
						df342._adc.push_back(it->_adc[k]);
						df342._ltdc.push_back(it->_ltdc[k]);
					}
				}
				else if (it->_depth==4)
				{
					for (int k=0; k<4; k++)
					{
						df344._adc.push_back(it->_adc[k]);
						df344._ltdc.push_back(it->_ltdc[k]);
					}
				}
			}
			if (it->_iphi==39 && it->_ieta==30)
			{
				if (it->_depth==2)
				{
					for (int k=0; k<4; k++)
					{
						df302._adc.push_back(it->_adc[k]);
						df302._ltdc.push_back(it->_ltdc[k]);
					}
				}
				else if (it->_depth==4)
				{
					for (int k=0; k<4; k++)
					{
						df304._adc[k]=it->_adc[k];
						df304._ltdc[k]=it->_ltdc[k];
					}
				}
			}
		}

		int adc302 = df302._adc[2];
		int adc304 = df304._adc[2];
		int adc342 = df342._adc[2];
		int adc344 = df344._adc[2];

		hADC_30_2->Fill(adc302);
		hADC_30_4->Fill(adc304);
		hADC_34_2->Fill(adc342);
		hADC_34_4->Fill(adc344);

		double fC302 = adc2fC_QIE10[df302._adc[2]];
		double fC304 = adc2fC_QIE10[df304._adc[2]];
		double fC342 = adc2fC_QIE10[df342._adc[2]];
		double fC344 = adc2fC_QIE10[df344._adc[2]];

		hfC_30_2->Fill(fC302);
		hfC_30_4->Fill(fC304);
		hfC_34_2->Fill(fC342);
		hfC_34_4->Fill(fC344);
		
		double tdc302 = df302._ltdc[2];
		double tdc304 = df304._ltdc[2];
		double tdc342 = df342._ltdc[2];
		double tdc344 = df344._ltdc[2];

		hTDC_30_2->Fill(tdc302);
		hTDC_30_4->Fill(tdc304);
		hTDC_34_2->Fill(tdc342);
		hTDC_34_4->Fill(tdc344);

		double ped302 = adc2fC_QIE10[4];
		double ped304 = adc2fC_QIE10[5];
		double ped342 = adc2fC_QIE10[5];
		double ped344 = adc2fC_QIE10[5];

		double fC302_PED = fC302-ped302;
		double fC304_PED = fC304-ped304;
		double fC342_PED = fC342-ped342;
		double fC344_PED = fC344-ped344;

		hfC_30_2_PED->Fill(fC302_PED);
		hfC_30_4_PED->Fill(fC304_PED);
		hfC_34_2_PED->Fill(fC342_PED);
		hfC_34_4_PED->Fill(fC344_PED);

		hADCC_30->Fill(adc302, adc304);
		hADCC_34->Fill(adc342, adc344);

		hfCC_30->Fill(fC302, fC304);
		hfCC_34->Fill(fC342, fC344);
		
		hfCC_PED_30->Fill(fC302_PED, fC304_PED);
		hfCC_PED_34->Fill(fC342_PED, fC344_PED);

		hTDCC_30->Fill(tdc302, tdc304);
		hTDCC_34->Fill(tdc342, tdc344);

		if (tdc302<60 && tdc304<60)
			hfCC_PED_30_TDClt60->Fill(fC302_PED, fC304_PED);
		if (tdc342<60 && tdc344<60)
			hfCC_PED_34_TDClt60->Fill(fC342_PED, fC344_PED);

		if (fC302_PED>30 && fC304_PED>30)
			hR_30->Fill(fC304_PED/fC302_PED);
		if (fC342_PED>30 && fC344_PED>30)
			hR_34->Fill(fC344_PED/fC342_PED);

		int tdccut302 = 18;
		int tdccut304 = 21;
		int tdccut342 = 20;
		int tdccut344 = 24;

		if (tdc302>tdccut302 && tdc304<tdccut304)
			hfCC_30_TDCCut2->Fill(fC302_PED, fC304_PED);
		if (tdc302<tdccut302 && tdc304>tdccut304)
			hfCC_30_TDCCut4->Fill(fC302_PED, fC304_PED);
		
		if (tdc342>tdccut342 && tdc344<tdccut344)
			hfCC_34_TDCCut2->Fill(fC342_PED, fC344_PED);
		if (tdc342<tdccut342 && tdc344>tdccut344)
			hfCC_34_TDCCut4->Fill(fC342_PED, fC344_PED);

		double m1cut_30 = 4.0;
		double m1cut_34 = 5.6;

		//	do it for ieta30
		if (fC302_PED>10 && fC304_PED>10)
		{
			double s1 = max(fC302_PED, fC304_PED);
			double s2 = min(fC302_PED, fC304_PED);

			double m1 = s1/s2;
			double mean = (s1+s2)/2.;
			double m2 = sqrt((s1-mean)*(s1-mean)+(s2-mean)*(s2-mean))/(s1+s2);
			double m3 = (s1-mean)/(s1+s2);
			double m4 = (s1-s2)/(s1+s2);

			hM1_30->Fill(m1);
			hM2_30->Fill(m2);
			hM3_30->Fill(m3);
			hM4_30->Fill(m4);

			hTDC_30_2_ALL->Fill(tdc302);
			hTDC_30_4_ALL->Fill(tdc304);

			hS_30->Fill(s1+s2);
			
			if ((s1+s2)>=200)
			{
				out_30  << fC302_PED << "  " << fC304_PED << "  " << tdc302 << "  " << tdc304
					<< "  " << m1 << "  " << m2 << "  " << m3 << "  " << m4
					<< std::endl;
			}

			if (m1>m1cut_30)
			{
				hTDC_30_2_PMT->Fill(tdc302);
				hTDC_30_4_PMT->Fill(tdc304);
				m1class_30++;
				hSM1_30->Fill(2*s2);
				if (tdc302>tdccut302 && tdc304<tdccut304)
					m1class_tdc_30++;
				if (tdc302<tdccut302 && tdc304>tdccut304)
					m1class_tdc_30++;
			}
			else
				hSM1_30->Fill(s1+s2);
			if (m2>0.3)
				hSM2_30->Fill(2*s2);
			else 
				hSM2_30->Fill(s1+s2);
			if (m3>0.25)
				hSM3_30->Fill(2*s2);
			else
				hSM3_30->Fill(s1+s2);
			if (m4>0.4)
				hSM4_30->Fill(2*s2);
			else
				hSM4_30->Fill(s1+s2);
		}

		//	do it for ieta34
		if (fC342_PED>10 && fC344_PED>10)
		{
			double s1 = max(fC342_PED, fC344_PED);
			double s2 = min(fC342_PED, fC344_PED);

			double m1 = s1/s2;
			double mean = (s1+s2)/2.;
			double m2 = sqrt((s1-mean)*(s1-mean)+(s2-mean)*(s2-mean))/(s1+s2);
			double m3 = (s1-mean)/(s1+s2);
			double m4 = (s1-s2)/(s1+s2);

			hM1_34->Fill(m1);
			hM2_34->Fill(m2);
			hM3_34->Fill(m3);
			hM4_34->Fill(m4);

			hTDC_34_2_ALL->Fill(tdc342);
			hTDC_34_4_ALL->Fill(tdc344);

			hS_34->Fill(s1+s2);
			
			if ((s1+s2)>=200)
			{
				out_34  << fC342_PED << "  " << fC344_PED << "  " << tdc342 << "  " << tdc344
					<< "  " << m1 << "  " << m2 << "  " << m3 << "  " << m4
					<< std::endl;
			}

			if (m1>m1cut_34)
			{
				hTDC_34_2_PMT->Fill(tdc342);
				hTDC_34_4_PMT->Fill(tdc344);
				m1class_34++;
				hSM1_34->Fill(2*s2);
				if (tdc342>tdccut342 && tdc344<tdccut344)
					m1class_tdc_34++;
				if (tdc342<tdccut342 && tdc344>tdccut344)
					m1class_tdc_34++;
			}
			else
				hSM1_34->Fill(s1+s2);
			if (m2>0.3)
				hSM2_34->Fill(2*s2);
			else 
				hSM2_34->Fill(s1+s2);
			if (m3>0.25)
				hSM3_34->Fill(2*s2);
			else
				hSM3_34->Fill(s1+s2);
			if (m4>0.4)
				hSM4_34->Fill(2*s2);
			else
				hSM4_34->Fill(s1+s2);
		}

		/*
		hADC1Distribution->Fill(df1._adc[2]);
		hADC2Distribution->Fill(df2._adc[2]);
		hfC1Distribution->Fill(fC1);
		hfC2Distribution->Fill(fC2);
		hfC1_PED->Fill(fC1_ped);
		hfC2_PED->Fill(fC2_ped);
		hCorrelation->Fill(fC1_ped, fC2_ped);
		if (fC1_ped>30 && fC2_ped>30)
			hRatio_PED->Fill(fC2_ped/fC1_ped);
		double scale = 0.9726;
		double fC2_new = fC2_ped/scale;
		double fC1_new = fC1_ped;
		hfC1_new->Fill(fC1_new);
		hfC2_new->Fill(fC2_new);

		double tdc1 = df1._ltdc[2];
		double tdc2 = df2._ltdc[2];
		hTDC1->Fill(df1._ltdc[2])w;
		hTDC2->Fill(df2._ltdc[2]);
		hADCCorrelationTS2->Fill(df1._adc[2], df2._adc[2]);
		if (tdc1<60 && tdc2<60)
			hADCCorrelationTS2_TDClt60->Fill(df1._adc[2], df2._adc[2]);
		if (fC1_new>45)
			hTDC1_ALL->Fill(df1._ltdc[2]);
		if (fC2_new>45)
			hTDC2_ALL->Fill(df2._ltdc[2]);
		hTDCvsfC_1->Fill(fC1_new, df1._ltdc[2]);
		hTDCvsfC_2->Fill(fC2_new, df2._ltdc[2]);
		if (fC1_new<45 && fC2_new>100)
		{
			hTDCvsfC_2_CUT->Fill(fC2_new, df2._ltdc[2]);
			hfC2_fC1zs->Fill(fC2_new);
			hTDC2_PMT->Fill(df2._ltdc[2]);
			if (tdc2<18)
				numl18++;
			if (tdc2<18 && tdc1>18)
				numl18andg18++;
		}
		if (fC2_new<45 && fC1_new>100)
		{
			hTDCvsfC_1_CUT->Fill(fC1_new, df1._ltdc[2]);
			hfC1_fC2zs->Fill(fC1_new);
			hTDC1_PMT->Fill(df1._ltdc[2]);
			if (tdc1<18)
				numl18++;
			if (tdc1<18 && tdc2>18)
				numl18andg18++;
		}

		if (tdc1<18 && tdc2>18)
			hfC2vsfC1_1->Fill(fC1_new, fC2_new);
		if (tdc2<18 && tdc1>18)
			hfC2vsfC1_2->Fill(fC1_new, fC2_new);


		if (fC1_new>10 && fC2_new>10)
		{
			double s1 = max(fC1_new, fC2_new);
			double s2 = min(fC1_new, fC2_new);

			double m1 = s1/s2;
			double mean = (s1+s2)/2.;
			double m2 = sqrt((s1-mean)*(s1-mean)+(s2-mean)*(s2-mean))/(s1+s2);
			double m3 = (s1-mean)/(s1+s2);
			double m4 = (s1-s2)/(s1+s2);

			hM1->Fill(m1);
			hM2->Fill(m2);
			hM3->Fill(m3);
			hM4->Fill(m4);
			hSum->Fill(s1+s2);

			if ((s1+s2)>=200)
			{
				out_txt  << fC1 << "  " << fC2 << "  " << tdc1 << "  " << tdc2
					<< "  " << m1 << "  " << m2 << "  " << m3 << "  " << m4
					<< std::endl;
			}

			if (m1>3)
			{
				hSumM1->Fill(2*s2);
				mclass[0]++;
				if (tdc1<18 && tdc2>18)
					mclass2[0]++;
				if (tdc2<18 && tdc1>18)
					mclass2[0]++;
				if (tdc1<18 || tdc2<18)
					mclass1[0]++;
			}
			else
				hSumM1->Fill(s1+s2);
			if (m2>0.3)
			{
				hSumM2->Fill(2*s2);
				mclass[1]++;
				if (tdc1<18 && tdc2>18)
					mclass2[1]++;
				if (tdc2<18 && tdc1>18)
					mclass2[1]++;
				if (tdc1<18 || tdc2<18)
					mclass1[1]++;
			}
			else
				hSumM2->Fill(s1+s2);
			if (m3>0.25)
			{
				hSumM3->Fill(2*s2);
				mclass[2]++;
				if (tdc1<18 && tdc2>18)
					mclass2[2]++;
				if (tdc2<18 && tdc1>18)
					mclass2[2]++;
				if (tdc1<18 || tdc2<18)
					mclass1[2]++;
			}
			else
				hSumM3->Fill(s1+s2);
			if (m4>0.4)
			{
				hSumM4->Fill(2*s2);
				mclass[3]++;
				if (tdc1<18 && tdc2>18)
					mclass2[3]++;
				if (tdc2<18 && tdc1>18)
					mclass2[3]++;
				if (tdc1<18 || tdc2<18)
					mclass1[3]++;
			}
			else
				hSumM4->Fill(s1+s2);


		}
		*/
	}	

	/*
	hfC2_fC1zs->Scale(1./hfC2_fC1zs->Integral());
	hfC1_fC2zs->Scale(1./hfC1_fC2zs->Integral());
	hTDC1_ALL->Scale(1./hTDC1_ALL->Integral());
	hTDC2_ALL->Scale(1./hTDC2_ALL->Integral());
	hTDC1_PMT->Scale(1./hTDC1_PMT->Integral());
	hTDC2_PMT->Scale(1./hTDC2_PMT->Integral());
	*/

	//	dir for pics
	TString dir="/Users/vk/software/Analysis/docs/qie10/pics/results_4p0_5p0/";

	//	plot with styles
	TCanvas *c1 = new TCanvas("c1", "c1", 600, 400);
	c1->Divide(1,2);
	c1->cd(1);
	gPad->SetLogy();
	hADC_30_4->GetXaxis()->SetTitle("adc");
	hADC_30_2->GetXaxis()->SetTitle("adc");
	hADC_30_4->GetYaxis()->SetTitle("#Events");
	hADC_30_2->GetYaxis()->SetTitle("#Events");
	hADC_30_4->SetLineColor(kRed);
	hADC_30_2->SetLineColor(kBlack);
	hADC_30_4->Draw();
	gPad->Update();
	TPaveStats* stats1 = (TPaveStats*)hADC_30_4->FindObject("stats");
	hADC_30_2->Draw("sames");
	gPad->Update();
	TPaveStats* stats2 = (TPaveStats*)hADC_30_2->FindObject("stats");
	stats1->SetTextColor(kRed);
	stats2->SetTextColor(kBlack);
	stats2->SetX1NDC(stats1->GetX1NDC()); stats2->SetX2NDC(stats1->GetX2NDC());
	stats2->SetY1NDC(-stats1->GetY2NDC()+2*stats1->GetY1NDC()); stats2->SetY2NDC(stats1->GetY1NDC());
	gPad->Modified();
	
	c1->cd(2);
	gPad->SetLogy();
	hADC_34_4->GetXaxis()->SetTitle("adc");
	hADC_34_2->GetXaxis()->SetTitle("adc");
	hADC_34_4->GetYaxis()->SetTitle("#Events");
	hADC_34_2->GetYaxis()->SetTitle("#Events");
	hADC_34_4->SetLineColor(kRed);
	hADC_34_2->SetLineColor(kBlack);
	hADC_34_4->Draw();
	gPad->Update();
	stats1 = (TPaveStats*)hADC_34_4->FindObject("stats");
	hADC_34_2->Draw("sames");
	gPad->Update();
	stats2 = (TPaveStats*)hADC_34_2->FindObject("stats");
	stats1->SetTextColor(kRed);
	stats2->SetTextColor(kBlack);
	stats2->SetX1NDC(stats1->GetX1NDC()); stats2->SetX2NDC(stats1->GetX2NDC());
	stats2->SetY1NDC(-stats1->GetY2NDC()+2*stats1->GetY1NDC()); stats2->SetY2NDC(stats1->GetY1NDC());
	gPad->Modified();

	c1->SetLogy();
	c1->SaveAs(dir+"ADC1_ADC2_overlaid.pdf");

	// TDC
	TCanvas *c4 = new TCanvas("c4", "c4", 600, 400);
	c4->Divide(2,2);
	c4->cd(1);
	gPad->SetLogy();
	hTDC_30_4->GetXaxis()->SetTitle("tdc");
	hTDC_30_2->GetXaxis()->SetTitle("tdc");
	hTDC_30_4->GetYaxis()->SetTitle("#Events");
	hTDC_30_2->GetYaxis()->SetTitle("#Events");
	hTDC_30_4->SetLineColor(kRed);
	hTDC_30_2->SetLineColor(kBlack);
	hTDC_30_4->Draw();
	gPad->Update();
	stats1 = (TPaveStats*)hTDC_30_4->FindObject("stats");
	hTDC_30_2->Draw("sames");
	gPad->Update();
	stats2 = (TPaveStats*)hTDC_30_2->FindObject("stats");
	stats1->SetTextColor(kRed);
	stats2->SetTextColor(kBlack);
	stats2->SetX1NDC(stats1->GetX1NDC()); stats2->SetX2NDC(stats1->GetX2NDC());
	stats2->SetY1NDC(-stats1->GetY2NDC()+2*stats1->GetY1NDC()); stats2->SetY2NDC(stats1->GetY1NDC());
	gPad->Modified();
	
	c4->cd(2);
	gPad->SetLogy();
	hTDC_34_4->GetXaxis()->SetTitle("tdc");
	hTDC_34_2->GetXaxis()->SetTitle("tdc");
	hTDC_34_4->GetYaxis()->SetTitle("#Events");
	hTDC_34_2->GetYaxis()->SetTitle("#Events");
	hTDC_34_4->SetLineColor(kRed);
	hTDC_34_2->SetLineColor(kBlack);
	hTDC_34_4->Draw();
	gPad->Update();
	stats1 = (TPaveStats*)hTDC_34_4->FindObject("stats");
	hTDC_34_2->Draw("sames");
	gPad->Update();
	stats2 = (TPaveStats*)hTDC_34_2->FindObject("stats");
	stats1->SetTextColor(kRed);
	stats2->SetTextColor(kBlack);
	stats2->SetX1NDC(stats1->GetX1NDC()); stats2->SetX2NDC(stats1->GetX2NDC());
	stats2->SetY1NDC(-stats1->GetY2NDC()+2*stats1->GetY1NDC()); stats2->SetY2NDC(stats1->GetY1NDC());
	gPad->Modified();

	c4->cd(3);
	gPad->SetLogz();
	hTDCC_30->GetXaxis()->SetTitle("tdc2");
	hTDCC_30->GetYaxis()->SetTitle("tdc4");
	hTDCC_30->Draw("colz");

	c4->cd(4);
	gPad->SetLogz();
	hTDCC_34->GetXaxis()->SetTitle("tdc2");
	hTDCC_34->GetYaxis()->SetTitle("tdc4");
	hTDCC_34->Draw("colz");

	c4->SaveAs(dir+"TDC1_TDC2_overlaid.pdf");

	//	fC
	TCanvas *c2 = new TCanvas("c2", "c2", 600, 400);
	c2->Divide(1,2);
	c2->cd(1);
	gPad->SetLogy();
	hfC_30_4->GetXaxis()->SetTitle("fC");
	hfC_30_2->GetXaxis()->SetTitle("fC");
	hfC_30_4->GetYaxis()->SetTitle("#Events");
	hfC_30_2->GetYaxis()->SetTitle("#Events");
	hfC_30_4->SetLineColor(kRed);
	hfC_30_2->SetLineColor(kBlack);
	hfC_30_4->Draw();
	gPad->Update();
	stats1 = (TPaveStats*)hfC_30_4->FindObject("stats");
	hfC_30_2->Draw("sames");
	gPad->Update();
	stats2 = (TPaveStats*)hfC_30_2->FindObject("stats");
	stats1->SetTextColor(kRed);
	stats2->SetTextColor(kBlack);
	stats2->SetX1NDC(stats1->GetX1NDC()); stats2->SetX2NDC(stats1->GetX2NDC());
	stats2->SetY1NDC(-stats1->GetY2NDC()+2*stats1->GetY1NDC()); stats2->SetY2NDC(stats1->GetY1NDC());
	gPad->Modified();
	
	c2->cd(2);
	gPad->SetLogy();
	hfC_34_4->GetXaxis()->SetTitle("fC");
	hfC_34_2->GetXaxis()->SetTitle("fC");
	hfC_34_4->GetYaxis()->SetTitle("#Events");
	hfC_34_2->GetYaxis()->SetTitle("#Events");
	hfC_34_4->SetLineColor(kRed);
	hfC_34_2->SetLineColor(kBlack);
	hfC_34_4->Draw();
	gPad->Update();
	stats1 = (TPaveStats*)hfC_34_4->FindObject("stats");
	hfC_34_2->Draw("sames");
	gPad->Update();
	stats2 = (TPaveStats*)hfC_34_2->FindObject("stats");
	stats1->SetTextColor(kRed);
	stats2->SetTextColor(kBlack);
	stats2->SetX1NDC(stats1->GetX1NDC()); stats2->SetX2NDC(stats1->GetX2NDC());
	stats2->SetY1NDC(-stats1->GetY2NDC()+2*stats1->GetY1NDC()); stats2->SetY2NDC(stats1->GetY1NDC());
	gPad->Modified();

	c2->SetLogy();
	c2->SaveAs(dir+"fC1_fC2_overlaid.pdf");

	//	fC with ped
	TCanvas *c3 = new TCanvas("c3", "c3", 600, 400);
	c3->Divide(1,2);
	c3->cd(1);
	gPad->SetLogy();
	hfC_30_4_PED->GetXaxis()->SetTitle("fC");
	hfC_30_2_PED->GetXaxis()->SetTitle("fC");
	hfC_30_4_PED->GetYaxis()->SetTitle("#Events");
	hfC_30_2_PED->GetYaxis()->SetTitle("#Events");
	hfC_30_4_PED->SetLineColor(kRed);
	hfC_30_2_PED->SetLineColor(kBlack);
	hfC_30_4_PED->Draw();
	gPad->Update();
	stats1 = (TPaveStats*)hfC_30_4_PED->FindObject("stats");
	hfC_30_2_PED->Draw("sames");
	gPad->Update();
	stats2 = (TPaveStats*)hfC_30_2_PED->FindObject("stats");
	stats1->SetTextColor(kRed);
	stats2->SetTextColor(kBlack);
	stats2->SetX1NDC(stats1->GetX1NDC()); stats2->SetX2NDC(stats1->GetX2NDC());
	stats2->SetY1NDC(-stats1->GetY2NDC()+2*stats1->GetY1NDC()); stats2->SetY2NDC(stats1->GetY1NDC());
	gPad->Modified();
	
	c3->cd(2);
	gPad->SetLogy();
	hfC_34_4_PED->GetXaxis()->SetTitle("fC");
	hfC_34_2_PED->GetXaxis()->SetTitle("fC");
	hfC_34_4_PED->GetYaxis()->SetTitle("#Events");
	hfC_34_2_PED->GetYaxis()->SetTitle("#Events");
	hfC_34_4_PED->SetLineColor(kRed);
	hfC_34_2_PED->SetLineColor(kBlack);
	hfC_34_4_PED->Draw();
	gPad->Update();
	stats1 = (TPaveStats*)hfC_34_4_PED->FindObject("stats");
	hfC_34_2_PED->Draw("sames");
	gPad->Update();
	stats2 = (TPaveStats*)hfC_34_2_PED->FindObject("stats");
	stats1->SetTextColor(kRed);
	stats2->SetTextColor(kBlack);
	stats2->SetX1NDC(stats1->GetX1NDC()); stats2->SetX2NDC(stats1->GetX2NDC());
	stats2->SetY1NDC(-stats1->GetY2NDC()+2*stats1->GetY1NDC()); stats2->SetY2NDC(stats1->GetY1NDC());
	gPad->Modified();

	c3->SetLogy();
	c3->SaveAs(dir+"fC1PED_fC2PED_overlaid.pdf");

	TCanvas *c5 = new TCanvas("c5", "c5", 600, 400);
	c5->Divide(2,2);
	c5->cd(1);
	gPad->SetLogz();
	hfCC_PED_30->Draw("colz");

	c5->cd(2);
	gPad->SetLogz();
	hfCC_PED_30_TDClt60->Draw("colz");

	c5->cd(3);
	gPad->SetLogz();
	hfCC_PED_34->Draw("colz");

	c5->cd(4);
	gPad->SetLogz();
	hfCC_PED_34_TDClt60->Draw("colz");

	c5->SaveAs(dir+"fCC_30_fCC_34_wTDClt60.pdf");

	//	fC correlations for TDC id cuts
	TCanvas *c6 = new TCanvas("c6", "c6", 600, 400);
	c6->Divide(2,2);
	c6->cd(1);
//	gPad->SetLogz();
	hfCC_30_TDCCut2->Draw("colz");

	c6->cd(2);
//	gPad->SetLogz();
	hfCC_30_TDCCut4->Draw("colz");

	c6->cd(3);
//	gPad->SetLogz();
	hfCC_34_TDCCut2->Draw("colz");

	c6->cd(4);
//	gPad->SetLogz();
	hfCC_34_TDCCut4->Draw("colz");
	c6->SaveAs(dir+"fCC_TDCCuts_PMTID.pdf");
	
	//	M1-M4
	TCanvas *c7 = new TCanvas("c7", "c7", 600, 400);
	c7->Divide(2,2);
	c7->cd(1);
	gPad->SetLogy();
	hM1_30->Draw();

	c7->cd(2);
	gPad->SetLogy();
	hM2_30->Draw();

	c7->cd(3);
	gPad->SetLogy();
	hM3_30->Draw();

	c7->cd(4);
	gPad->SetLogy();
	hM4_30->Draw();
	c7->SaveAs(dir+"M1to4_30.pdf");
	
	TCanvas *c8 = new TCanvas("c8", "c8", 600, 400);
	c8->Divide(2,2);
	c8->cd(1);
	gPad->SetLogy();
	hM1_34->Draw();

	c8->cd(2);
	gPad->SetLogy();
	hM2_34->Draw();

	c8->cd(3);
	gPad->SetLogy();
	hM3_34->Draw();

	c8->cd(4);
	gPad->SetLogy();
	hM4_34->Draw();
	c8->SaveAs(dir+"M1to4_34.pdf");

	//	M1-M4
	TCanvas *c9 = new TCanvas("c9", "c9", 600, 400);
	c9->Divide(2,3);
	hS_30->SetLineColor(kBlack);
	hSM1_30->SetLineColor(kRed);
	hSM2_30->SetLineColor(kGreen);
	hSM3_30->SetLineColor(kBlue);
	hSM4_30->SetLineColor(kGray);
	hSM1_30->SetStats(0);
	hSM2_30->SetStats(0);
	hSM3_30->SetStats(0);
	hSM4_30->SetStats(0);
	c9->cd(1);
	gPad->SetLogy();
	
	hS_30->Draw();
	c9->cd(2);
	gPad->SetLogy();
	hSM1_30->Draw();
	c9->cd(3);
	gPad->SetLogy();
	hSM2_30->Draw();
	c9->cd(4);
	gPad->SetLogy();
	hSM3_30->Draw();
	c9->cd(5);
	gPad->SetLogy();
	hSM4_30->Draw();

	c9->cd(6);
	gPad->SetLogy();
	hS_30->Draw();
	hSM1_30->Draw("same");
	hSM2_30->Draw("same");
	hSM3_30->Draw("same");
	hSM4_30->Draw("same");
	TLegend *leg = new TLegend(0.79, 0.5, 0.99, 0.7);
	leg->AddEntry(hS_30, "S1+S2");
	leg->AddEntry(hSM1_30, "M1");
	leg->AddEntry(hSM2_30, "M2");
	leg->AddEntry(hSM3_30, "M3");
	leg->AddEntry(hSM4_30, "M4");
	leg->Draw();

	//	M1-M4 for ieta40
	TCanvas *c10 = new TCanvas("c10", "c10", 600, 400);
	c10->Divide(2,3);
	hS_34->SetLineColor(kBlack);
	hSM1_34->SetLineColor(kRed);
	hSM2_34->SetLineColor(kGreen);
	hSM3_34->SetLineColor(kBlue);
	hSM4_34->SetLineColor(kGray);
	hSM1_34->SetStats(0);
	hSM2_34->SetStats(0);
	hSM3_34->SetStats(0);
	hSM4_34->SetStats(0);
	c10->cd(1);
	gPad->SetLogy();
	
	hS_34->Draw();
	c10->cd(2);
	gPad->SetLogy();
	hSM1_34->Draw();
	c10->cd(3);
	gPad->SetLogy();
	hSM2_34->Draw();
	c10->cd(4);
	gPad->SetLogy();
	hSM3_34->Draw();
	c10->cd(5);
	gPad->SetLogy();
	hSM4_34->Draw();

	c10->cd(6);
	gPad->SetLogy();
	hS_34->Draw();
	hSM1_34->Draw("same");
	hSM2_34->Draw("same");
	hSM3_34->Draw("same");
	hSM4_34->Draw("same");
	leg = new TLegend(0.79, 0.5, 0.99, 0.7);
	leg->AddEntry(hS_34, "S1+S2");
	leg->AddEntry(hSM1_34, "M1");
	leg->AddEntry(hSM2_34, "M2");
	leg->AddEntry(hSM3_34, "M3");
	leg->AddEntry(hSM4_34, "M4");
	leg->Draw();
	
	c9->SaveAs(dir+"SignalM1to4_30.pdf");
	c10->SaveAs(dir+"SignalM1to4_34.pdf");
	

	/*
	TCanvas *c2 = new TCanvas("c2", "c2", 600, 400);
	c2->cd();
	hRatio_PED->GetXaxis()->SetTitle("fC1 / fC2");
	hRatio_PED->GetYaxis()->SetTitle("#Events");
	hRatio_PED->SetLineColor(kBlack);
	hRatio_PED->Rebin(20);
	TF1 *fitRatio_PED = new TF1("mygauss", "gaus", 0, 2);
	fitRatio_PED->SetLineColor(kRed);
	hRatio_PED->Draw();
	hRatio_PED->Fit("mygauss", "R", "sames");
	gStyle->SetOptFit(1111);
	gPad->Update();
	c2->SaveAs(dir+"fC1_over_fC2_ratio.pdf");

	TCanvas *c3 = new TCanvas("c3", "c3", 600, 400);
	c3->cd();
	c3->SetLogz();
	hTDCvsfC_1->GetXaxis()->SetTitle("fC");
	hTDCvsfC_1->GetYaxis()->SetTitle("TDC");
	hTDCvsfC_2->GetXaxis()->SetTitle("fC");
	hTDCvsfC_2->GetYaxis()->SetTitle("TDC");
	hTDCvsfC_1->Draw("colz");
	c3->SaveAs(dir+"TDCvsfC1.pdf");
	hTDCvsfC_2->Draw("colz");
	c3->SaveAs(dir+"TDCvsfC2.pdf");

	TCanvas *c4 = new TCanvas("c4", "c4", 600, 400);
	c4->cd();
	c4->SetLogy();
	hTDC1_ALL->GetXaxis()->SetTitle("TDC");
	hTDC1_ALL->GetYaxis()->SetTitle("#Events");
	hTDC1_PMT->GetXaxis()->SetTitle("TDC");
	hTDC1_PMT->GetYaxis()->SetTitle("#Events");
	hTDC2_ALL->GetXaxis()->SetTitle("TDC");
	hTDC2_ALL->GetYaxis()->SetTitle("#Events");
	hTDC2_PMT->GetXaxis()->SetTitle("TDC");
	hTDC2_PMT->GetYaxis()->SetTitle("#Events");
	hTDC1_ALL->SetLineColor(kBlack);
	hTDC1_PMT->SetLineColor(kRed);
	hTDC1_ALL->Draw();
	gPad->Update();
	stats1 = (TPaveStats*)hTDC1_ALL->FindObject("stats");
	hTDC1_PMT->Draw("sames");
	gPad->Update();
	stats2 = (TPaveStats*)hTDC1_PMT->FindObject("stats");
	stats1->SetTextColor(kBlack);
	stats2->SetTextColor(kRed);
//	stats1->SetX1NDC(stats1->GetX1NDC()); stats2->SetX2NDC(stats1->GetX2NDC());
	stats2->SetX1NDC(stats1->GetX1NDC()); stats2->SetX2NDC(stats1->GetX2NDC());
	stats2->SetY1NDC(-stats1->GetY2NDC()+2*stats1->GetY1NDC()); stats2->SetY2NDC(stats1->GetY1NDC());
	gPad->Modified();
	c4->SaveAs(dir+"TDC_distribution_overlaidwithPMT_A1.pdf");

	TCanvas *c5 = new TCanvas("c5", "c5", 600, 400);
	c5->cd();
	c5->SetLogy();
	hTDC2_ALL->SetLineColor(kBlack);
	hTDC2_PMT->SetLineColor(kRed);
	hTDC2_ALL->Draw();
	gPad->Update();
	stats1 = (TPaveStats*)hTDC2_ALL->FindObject("stats");
	hTDC2_PMT->Draw("sames");
	gPad->Update();
	stats2 = (TPaveStats*)hTDC2_PMT->FindObject("stats");
	stats1->SetTextColor(kBlack);
	stats2->SetTextColor(kRed);
//	stats1->SetX1NDC(stats1->GetX1NDC()); stats2->SetX2NDC(stats1->GetX2NDC());
	stats2->SetX1NDC(stats1->GetX1NDC()); stats2->SetX2NDC(stats1->GetX2NDC());
	stats2->SetY1NDC(-stats1->GetY2NDC()+2*stats1->GetY1NDC()); stats2->SetY2NDC(stats1->GetY1NDC());
	gPad->Modified();
	c5->SaveAs(dir+"TDC_distribution_overlaidwithPMT_A2.pdf");

	TCanvas *c6 = new TCanvas("c6", "c6", 600, 400);
	c6->cd();
	c6->SetLogz();
	hfC2vsfC1_1->GetXaxis()->SetTitle("fC1");
	hfC2vsfC1_1->GetYaxis()->SetTitle("fC2");
	hfC2vsfC1_2->GetXaxis()->SetTitle("fC1");
	hfC2vsfC1_2->GetYaxis()->SetTitle("fC2");
	hfC2vsfC1_1->Draw("colz");
	c6->SaveAs(dir+"fC2vsfC1_1.pdf");
	hfC2vsfC1_2->Draw("colz");
	c6->SaveAs(dir+"fC2vsfC1_2.pdf");

	TCanvas *c7 = new TCanvas("c7", "c7", 600, 400);
	c7->cd();
	c7->SetLogy();
	hM1->GetXaxis()->SetTitle("M1");
	hM2->GetXaxis()->SetTitle("M2");
	hM3->GetXaxis()->SetTitle("M3");
	hM4->GetXaxis()->SetTitle("M4");
	hM1->GetYaxis()->SetTitle("#Events");
	hM2->GetYaxis()->SetTitle("#Events");
	hM3->GetYaxis()->SetTitle("#Events");
	hM4->GetYaxis()->SetTitle("#Events");
	hM1->Draw();
	c7->SaveAs(dir+"M1.pdf");
	hM2->Draw();
	c7->SaveAs(dir+"M2.pdf");
	hM3->Draw();
	c7->SaveAs(dir+"M3.pdf");
	hM4->Draw();
	c7->SaveAs(dir+"M4.pdf");

	hSum->GetXaxis()->SetTitle("S1+S2");
	hSumM1->GetXaxis()->SetTitle("Signal");
	hSumM2->GetXaxis()->SetTitle("Signal");
	hSumM3->GetXaxis()->SetTitle("Signal");
	hSumM4->GetXaxis()->SetTitle("Signal");
	hSum->GetYaxis()->SetTitle("#Events");
	hSumM1->GetYaxis()->SetTitle("#Events");
	hSumM2->GetYaxis()->SetTitle("#Events");
	hSumM3->GetYaxis()->SetTitle("#Events");
	hSumM4->GetYaxis()->SetTitle("#Events");

	hSumM1->SetStats(0);
	hSumM2->SetStats(0);
	hSumM3->SetStats(0);
	hSumM4->SetStats(0);

	hSum->SetLineColor(kBlack);
	hSumM1->SetLineColor(kRed);
	hSumM2->SetLineColor(kGreen);
	hSumM3->SetLineColor(kBlue);
	hSumM4->SetLineColor(kOrange);
	hSum->Draw();
	hSumM1->Draw("same");
	hSumM2->Draw("same");
	hSumM3->Draw("same");
	hSumM4->Draw("same");
	TLegend *leg = new TLegend(0.79, 0.3, 0.99, 0.5);
	leg->SetHeader("Methods");
	leg->SetTextSize(0.03);
	leg->AddEntry(hSum, "S1+S2");
	leg->AddEntry(hSumM1, "M1");
	leg->AddEntry(hSumM2, "M2");
	leg->AddEntry(hSumM3, "M3");
	leg->AddEntry(hSumM4, "M4");
	leg->Draw();
	c7->SaveAs(dir+"Sums.pdf");
	*/

	out->Write();
	out->Close();

	std::cout << "ieta=30" << std::endl;
	std::cout << "m1class = " << m1class_30 << "  m1class_tdc = "
		<< m1class_tdc_30 << std::endl;
	std::cout << "ieta=34" << std::endl;
	std::cout << "m1class = " << m1class_34 << "  m1class_tdc = "
		<< m1class_tdc_34 << std::endl;

	/*
	std::cout << "#events with tdc<18 = " << numl18 << std::endl;
	std::cout << "#events with tdc<18 && tdc>18 for the other anode - "
		<< numl18andg18 << std::endl;
	for (int i=0; i<4; i++)
		std::cout << mclass[i] << "  ";
	std::cout << std::endl;
	for (int i=0; i<4; i++)	
		std::cout << mclass2[i] << "  ";
	std::cout << std::endl;
	for (int i=0; i<4; i++)	
		std::cout << mclass1[i] << "  ";
	std::cout << std::endl;
	*/
}

#endif





