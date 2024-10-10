#ifdef STANDALONE

#include "Event.h"
#include "TFile.h"
#include "Streamer.h"
#include "TH1D.h"

#include "boost/program_options.hpp"
#include <signal.h>
#include <string>

/*
 *  Globals
 */
bool                        __continue = true;
std::string                 __input;
std::string                 __output;
int                         __bins;
std::string const           NTUPLEMAKER_NAME = "ntuplemaker_H2DiMuonMaker";

namespace po = boost::program_options;
using namespace analysis::core;
using namespace analysis::processing;

/*
 *  Functions
 */
void sigHandler(int sig)
{
    cout << "### Signal: " << sig << " caughter. Exiting..." << endl;
    __continue = false;
}

void printcmd()
{
    std::cout << "input = " << __input << std::endl
        << "output = " << __output << std::endl;
}

int main(int argc, char** argv)
{
    signal(SIGABRT, &sigHandler);
    signal(SIGTERM, &sigHandler);
    signal(SIGINT, &sigHandler);

    po::options_description desc("Allowed Program Options");
    desc.add_options()
        ("help", "produce help messages")
        ("input", po::value<std::string>(), "a file specifying all the ROOT files to process")
        ("output", po::value<std::string>(), "an output ROOT file")
        ("bins", po::value<int>(), "number of bins")
    ;

    po::variables_map vm;
    po::store(po::parse_command_line(argc, argv, desc), vm);
    po::notify(vm);

    if (vm.count("help") || argc<2)
    {
        std::cout << desc << std::endl;
        return 1;
    }

    __input = vm["input"].as<std::string>();
    __output = vm["output"].as<std::string>();
    __bins = vm["bins"].as<int>();

    printcmd();

    //  start PU hist generation
    Streamer s(__input, NTUPLEMAKER_NAME+"/Events");
    s.chainup();
    EventAuxiliary *aux = NULL;

    TFile *out = new TFile(__output.c_str(), "recreate");
    TH1D *h = new TH1D("pileup", "pileup", __bins, 0, __bins);

    s._chain->SetBranchAddress("EventAuxiliary", &aux);
    uint32_t n = s._chain->GetEntries();
    for (uint32_t i=0; i<n && __continue; i++)
    {
        s._chain->GetEntry(i);
        if (i%10000==0)
            std::cout << "Processing Event " << i << " / " << n << std::endl;
        h->Fill(aux->_nPU, aux->_genWeight);
    }

    out->Write();
    out->Close();

    return 0;
}

#endif
