#ifdef STANDALONE

#include <iostream>
#include <vector>

#include "TFile.h"
#include "TChain.h"
#include "TH2D.h"

#include "QIE10Frame.h"
#include "QIE8Frame.h"

#define NUMBER_FRAMES 4

#define PEDESTAL 40

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

void test_qie10()
{
	TFile *out = new TFile("testqie10.root", "recreate");
	TH2D *hCorrelation = new TH2D("correlation", "correlation",
		256, 0, 256, 128, 0, 128);
	TH2D *hAnodeCorrelation = new TH2D("AnodeCorrelation", "AnodeCorrelation",
		256, 0, 256, 256, 0, 256);
	TH2D *hAnodeCorrelationTS[4];
	TH2D *hCorrelationTS[4];
	TH2D *hfCAnodeCorrelation[4];
	TH1D *hAnodeRatios[4];
	TH1D *hAnodeChargeRatio = new TH1D("AnodeChargeRatio", "AnodeChargeRatio",
		200, 0, 2);
	TH1D *hM4[4];
	TH2D *hM4vsRatios[4];
	for (int i=0; i<NUMBER_FRAMES; i++)
	{
		std::string num = to_string(i);
		std::string name1 = "AnodeCorrelationTS";
		name1+=num;
		std::string name2 = "CorrelationTS";
		name2+=num;
		hAnodeCorrelationTS[i] = new TH2D(name1.c_str(), name1.c_str(),
			256, 0, 256, 256, 0, 256);
		hCorrelationTS[i] = new TH2D(name2.c_str(), name2.c_str(),
			256, 0, 256, 128, 0, 128);

		std::string name3 = "fCAnodeCorrelation";
		name3+=num;
		hfCAnodeCorrelation[i] = new TH2D(name3.c_str(), name3.c_str(),
			10000, 0, 384000, 10000, 0, 384000);

		std::string name4 = "AnodeRatios";
		name4+=num;
		hAnodeRatios[i] = new TH1D(name4.c_str(), name4.c_str(),
			200, 0, 1);

		std::string name5 = "M4TS";
		name5+=num;
		hM4[i] = new TH1D(name5.c_str(), name5.c_str(),
			200, -1, 1);

		std::string name6 = "M4vsRatios";
		name6+=num;
		hM4vsRatios[i] = new TH2D(name6.c_str(), name6.c_str(),
			200, -1, 1, 200, 0, 1);
	}
	TH1D *hTDC = new TH1D("TDC", "TDC", 64, 0, 64);
	TH1D *hTDC_A1 = new TH1D("TDC_A1", "TDC_A1", 64, 0, 64);
	TH1D *hTDC_A2 = new TH1D("TDC_A2", "TDC_A2", 64, 0, 64);
	TH2D *hAnodeTDCCorrelation = new TH2D("AnodeTDCCorrelation", 
		"AnodeTDCCorrelation",
		64, 0, 64, 64, 0, 64);
	TH2D *hADCvsTDC_A1 = new TH2D("ADCvsTDC_A1", "ADCvsTDC_A1",
		256, 0, 256, 64, 0, 64);
	TH2D *hADCvsTDC_A2 = new TH2D("ADCvsTDC_A2", "ADCvsTDC_A2",
		256, 0, 256, 64, 0, 64);

	TChain *chain = new TChain("maker/Events");
	chain->Add("/Users/vk/software/Analysis/files/data/qie10/ntuplesmaking_qie10_ExpressPhysics_275376.root");
	std::cout << chain->GetEntries() << std::endl;

	using namespace analysis::core;
	QIE10Digis *qie10s = NULL;
	QIE8Digis *qie8s = NULL;
	chain->SetBranchAddress("QIE10Digis", &qie10s);
	chain->SetBranchAddress("QIE8Digis", &qie8s);
	
	int n=0;
	for (int i=0; i<chain->GetEntries(); i++)
	{
		chain->GetEntry(i);
		if (n%1000==0)
			std::cout << "### Event " << n << std::endl;
		QIE10Frame df1,df2;
		for (QIE10Digis::const_iterator it=qie10s->begin();
			it!=qie10s->end(); ++it)
		{
			for (int k=0; k<NUMBER_FRAMES; k++)
				hTDC->Fill(it->_ltdc[k]);

			for (QIE8Digis::const_iterator jt=qie8s->begin();
				jt!=qie8s->end(); ++jt)
			{
				if (it->_iphi==jt->_iphi && it->_ieta==jt->_ieta &&
					(it->_depth-2)==(jt->_depth))
				{
					for (int k=0; k<NUMBER_FRAMES; k++)
					{
						hCorrelation->Fill(it->_adc[k], jt->_adc[k]);
						hCorrelationTS[k]->Fill(it->_adc[k],
							jt->_adc[k]);
					}
				}
			}

			if (it->_iphi==39 && it->_depth==2)
			{
				if (it->_ieta==34)
				{
					for (int k=0; k<NUMBER_FRAMES; k++)
					{
						df1._adc[k] = it->_adc[k];
						df1._ltdc[k] = it->_ltdc[k];
						hTDC_A1->Fill(it->_ltdc[k]);
						hADCvsTDC_A1->Fill(it->_adc[k], it->_ltdc[k]);
					}
				}
				else if (it->_ieta==30)
				{
					for (int k=0; k<NUMBER_FRAMES; k++)
					{
						df2._adc[k] = it->_adc[k];
						df2._ltdc[k] = it->_ltdc[k];
						hTDC_A2->Fill(it->_ltdc[k]);
						hADCvsTDC_A2->Fill(it->_adc[k], it->_ltdc[k]);
					}
				}
				else
					std::cout << "### Something doesn't match" << std::endl;
			}
		}

		for (int k=0; k<NUMBER_FRAMES; k++)
		{
			double fc1 = adc2fC_QIE10[df1._adc[k]];
			double fc2 = adc2fC_QIE10[df2._adc[k]];
			hAnodeCorrelation->Fill(df1._adc[k], df2._adc[k]);
			hAnodeCorrelationTS[k]->Fill(df1._adc[k],
				df2._adc[k]);
			hAnodeTDCCorrelation->Fill(df1._ltdc[k], df2._ltdc[k]);
			hfCAnodeCorrelation[k]->Fill(fc1, fc2);
			
			if (fc1>PEDESTAL && fc2>PEDESTAL && k==2) 
				hAnodeChargeRatio->Fill(fc1/fc2);

			double s1,s2;
			s1 = fc1;
			s2 = fc2;
			if (fc1>fc2)
			{
				s1 = fc2;
				s2 = fc1;
			}
			double m4 = (fc1-fc2)/(fc1+fc2);
			hAnodeRatios[k]->Fill(s1/s2);
			hM4[k]->Fill(m4);
			hM4vsRatios[k]->Fill(m4, s1/s2);
		}
		n++;
	}
	std::cout << "Finished!" << std::endl;

	out->Write();
	out->Close();
}

int main(int argc, char** argv)
{
	test_qie10();
	return 0;
}

#endif
