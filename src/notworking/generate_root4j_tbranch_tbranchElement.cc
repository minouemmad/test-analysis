
#ifdef STANDALONE
#include "TFile.h"
#include "TChain.h"
#include "TTree.h"
#include "TString.h"
#include "TLorentzVector.h"

#include "Muon.h"
#include "TestClass1.h"
#include "TestClass2.h"

#include <string>
#include <iostream>
#include <vector>
#include <map>
#include <list>
#include <deque>
#include <set>
#include <forward_list>
#include <unordered_set>
#include <unordered_map>
#include <string>
#include <array>

struct mystruct 
{
    double v1;
    double arr1[5][5][5];
    float v2;
    float arr2[5][5][5];
    int v3;
    int arr3[5][5][5];
    char v4;
    char arr4[5][5][5];
    bool v5;
    bool arr5[5][5][5];
};

int main(int argc, char** argv)
{

    TFile *test_file = new TFile("test_root4j_tbranch_tbranchElement.root", "recreate");
    TTree *tree = new TTree("TestTree", "TestTree");

    mystruct someStruct;
    someStruct.v1 = 1.;
    someStruct.v2 = 1.;
    someStruct.v3 = 1;
    someStruct.v4 = 'x';
    someStruct.v5 = false;

    int a = 0;
    double b = 5.;
    float c = 1.;
    char d = 'x';
    bool f = false;
    char *str = (char*)std::string("abc").c_str();

    Int_t arr1[100];
    Double_t arr2[100];
    Float_t arr3[100];
    Char_t arr4[100];
    Bool_t arr5[100];

    int const dims=5;
    int multi1[dims][dims][dims];
    double multi2[dims][dims][dims];
    float multi3[dims][dims][dims];
    char multi4[dims][dims][dims];
    bool multi5[dims][dims][dims];

    int n = 100;
    int varr1[1000];
    double varr2[1000];

    tree->Branch("a", &a);
    tree->Branch("b", &b);
    tree->Branch("c", &c);
    tree->Branch("d", &d);
    tree->Branch("f", &f);

    //  simple array of fixed dimensions with simple types of fixed sizes
    tree->Branch("arr1", arr1, "arr1[100]/I");
    tree->Branch("arr2", arr2, "arr2[100]/D");
    tree->Branch("arr3", arr3, "arr3[100]/F");
    tree->Branch("arr4", arr4, "arr4[100]/B");
    tree->Branch("arr5", arr5, "arr5[100]/O");
    tree->Branch("str", str, "str/C");

    // simple multi-dimensional arrays 
    tree->Branch("multi1", multi1, "multi1[5][5][5]/I");
    tree->Branch("multi2", multi2, "multi2[5][5][5]/D");
    tree->Branch("multi3", multi3, "multi3[5][5][5]/F");
    tree->Branch("multi4", multi4, "multi4[5][5][5]/B");
    tree->Branch("multi5", multi5, "multi5[5][5][5]/O");

    tree->Branch("n", &n);
    tree->Branch("varr1", varr1, "varr1[n]/I");
    tree->Branch("varr2", varr2, "varr2[n]/D");

    //  Struct
    tree->Branch("someStruct", &someStruct, "v1/D:arr1[5][5][5]/D:v2/F:arr2[5][5][5]/F:v3/I:arr3[5][5][5]/I:v4/B:arr4[5][5][5]/B:v5/O:arr5[5][5][5]/O");
//    tree->Branch("someStruct", &someStruct, "v1/D:v2/F:v3/I:v4/B:v5/O");

    // vectors of simple types
    typedef std::vector<int> vectorOfInt;
    typedef std::vector<double> vectorOfDouble;
    typedef std::vector<float> vectorOfFloat;
    typedef std::vector<std::vector<int> > vectorOfVectorOfInt;
    typedef std::vector<std::vector<std::vector<int> > > vectorOfVectorOfVectorOfInt;
    typedef std::vector<std::vector<double> > vectorOfVectorOfDouble;
    typedef std::vector<std::vector<float> > vectorOfVectorOfFloat;
    typedef std::vector<std::vector<analysis::core::Muon> > vOfvOfMuons;
    vectorOfInt vInt;
    vectorOfDouble vDouble;
    vectorOfFloat vFloat;
    vectorOfVectorOfInt vOfVOfInt;
    vectorOfVectorOfDouble vOfVOfDouble;
    vectorOfVectorOfFloat vOfVOfFloat;
    vectorOfVectorOfVectorOfInt vOfVectorOfVectorOfInt;
//    tree->Branch("vInt", (vectorOfInt*)&vInt);
//    tree->Branch("vDouble", (vectorOfDouble*)&vDouble);
//    tree->Branch("vFloat", (vectorOfFloat*)&vFloat);
//    tree->Branch("vOfVOfInt", (vectorOfVectorOfInt*)&vOfVOfInt);
//    tree->Branch("vOfVOfDouble", (vectorOfVectorOfDouble*)&vOfVOfDouble);
//    tree->Branch("vOfVOfFloat", (vectorOfVectorOfFloat*)&vOfVOfFloat);
   // tree->Branch("vectorOfVectorOfVectorOfInt", (vectorOfVectorOfVectorOfInt*)&vOfVectorOfVectorOfInt);

    // std::pair
    typedef std::vector<std::pair<int, int> > vectorOfPairOfIntInt;
    typedef std::vector<std::vector<std::pair<int, int> > > vectorOfVectorOfPairOfIntInt;
    typedef std::map<int, std::pair<int, int> > mapInt2PairOfIntInt;
    vectorOfPairOfIntInt vPairIntInt;
    mapInt2PairOfIntInt mInt2PairOfIntInt;
    vectorOfVectorOfPairOfIntInt vOfVectorOfPairOfIntInt;
//    tree->Branch("vPairIntInt", (vectorOfPairOfIntInt*)&vPairIntInt, 32000, 0);
//    tree->Branch("vOfVectorOfPairOfIntInt", (vectorOfVectorOfPairOfIntInt*)&vOfVectorOfPairOfIntInt);
//    tree->Branch("mInt2PairOfIntInt", (mapInt2PairOfIntInt*)&mInt2PairOfIntInt);
    std::pair<int,int> myIntIntPair;
//    tree->Branch("myIntIntPair", (std::pair<int, int>*)&myIntIntPair, 32000, 0);

    // std::map
    typedef std::map<int, int> mapInt2Int;
    typedef std::map<int, float> mapInt2Float;
    typedef std::map<int, double> mapInt2Double;
    typedef std::map<int, std::map<int, int> > mapInt2MapInt2Int;
    typedef std::map<int, std::map<int, float> > mapInt2MapInt2Float;
    typedef std::map<int, std::map<int, double> > mapInt2MapInt2Double;
    typedef std::map<int, std::map<int, std::map<int, int> > > mapInt2MapInt2MapInt2Int;
    typedef std::map<int, std::map<int, analysis::core::Muon> > mapMuons;

    typedef std::vector<std::map<int, int> > vectorOfMapInt2Int;
    vectorOfMapInt2Int vOfMapInt2Int;
//    tree->Branch("vOfMapInt2Int", (vectorOfMapInt2Int*)&vOfMapInt2Int);

    mapInt2Int mInt2Int;
    mapInt2Float mInt2Float;
    mapInt2Double mInt2Double;
    mapInt2MapInt2Int mInt2MapInt2Int;
    mapInt2MapInt2Float mInt2MapInt2Float;
    mapInt2MapInt2Double mInt2MapInt2Double;
    mapMuons mMuons;
    mapInt2MapInt2MapInt2Int mInt2MapInt2MapInt2Int;
//    tree->Branch("mInt2Int", (mapInt2Int*)&mInt2Int);
//    tree->Branch("mInt2Float", (mapInt2Float*)&mInt2Float);
//    tree->Branch("mInt2Double", (mapInt2Double*)&mInt2Double);
//    tree->Branch("mInt2MapInt2Int", (mapInt2MapInt2Int*)&mInt2MapInt2Int);
//    tree->Branch("mInt2MapInt2Float", (mapInt2MapInt2Float*)&mInt2MapInt2Float);
//    tree->Branch("mInt2MapInt2Double", (mapInt2MapInt2Double*)&mInt2MapInt2Double);
//    tree->Branch("mapInt2MapInt2MapInt2Int", (mapInt2MapInt2MapInt2Int*)&mInt2MapInt2MapInt2Int);
    //tree->Branch("mMuons", (mapMuons*)&mMuons);
    
    // cross-nesting
    typedef std::vector<std::map<int, std::vector<int> > > vectorOfMapInt2VectorOfInt;
    vectorOfMapInt2VectorOfInt vOfMapInt2VectorOfInt;
//        tree->Branch("vectorOfMapInt2VectorOfInt", (vectorOfMapInt2VectorOfInt*)&vOfMapInt2VectorOfInt);
    

    // std::string
    std::string myString;
//    tree->Branch("myString", (std::string*)&myString);

    // std::array
    typedef std::array<int, 10> arrOfInt;
    typedef std::array<float, 10> arrOfFloat;
    typedef std::array<double, 10> arrOfDouble;
    typedef std::array<std::array<int, 10>, 10> arrOfArrOfInt;
    typedef std::array<std::array<float, 10>, 10> arrOfArrOfFloat;
    typedef std::array<std::array<double, 10>, 10> arrOfArrOfDouble;
    arrOfInt aOfInt;
    arrOfFloat aOfFloat;
    arrOfDouble aOfDouble;
    arrOfArrOfInt aOfArrOfInt;
    arrOfArrOfFloat aOfArrOfFloat;
    arrOfArrOfDouble aOfArrOfDouble;
    /*
    tree->Branch("aOfInt", (arrOfInt*)&aOfInt);
    tree->Branch("aOfFloat", (arrOfFloat*)&aOfFloat);
    tree->Branch("aOfDouble", (arrOfDouble*)&aOfDouble);
    tree->Branch("aOfArrOfInt", (arrOfArrOfInt*)&aOfArrOfInt);
    tree->Branch("aOfArrOfFloat", (arrOfArrOfFloat*)&aOfArrOfFloat);
    tree->Branch("aOfArrOfDouble", (arrOfArrOfDouble*)&aOfArrOfDouble);
    */

    // std::list
    typedef std::list<int> listOfInt;
    typedef std::list<float> listOfFloat;
    typedef std::list<double> listOfDouble;
    typedef std::list<std::list<int> > listOfListOfInt;
    typedef std::list<std::list<float> > listOfListOfFloat;
    typedef std::list<std::list<double> > listOfListOfDouble;
    arrOfInt lOfInt;
    arrOfFloat lOfFloat;
    arrOfDouble lOfDouble;
    arrOfArrOfInt lOfListOfInt;
    arrOfArrOfFloat lOfListOfFloat;
    arrOfArrOfDouble lOfListOfDouble;
/*    tree->Branch("lOfInt", (listOfInt*)&lOfInt);
    tree->Branch("lOfFloat", (listOfFloat*)&lOfFloat);
    tree->Branch("lOfDouble", (listOfDouble*)&lOfDouble);
    tree->Branch("lOfListOfInt", (listOfListOfInt*)&lOfListOfInt);
    tree->Branch("lOfListOfFloat", (listOfListOfFloat*)&lOfListOfFloat);
    tree->Branch("lOfListOfDouble", (listOfListOfDouble*)&lOfListOfDouble);
    */

    // test composite
    analysis::core::TestClass1 testClass1Splitted;
    analysis::core::TestClass1 testClass1UnSplitted;

    // vector of composite class that is splittable
    analysis::core::Muons muons;
    analysis::core::Muon myMuon;
    analysis::core::Track myTrackUnSplitted;
    analysis::core::Muon myMuonNonSplit;
    vOfvOfMuons vvMuons;
    std::map<int, analysis::core::Muon> mmmMuons;
//    tree->Branch("mMuons", (std::map<int, analysis::core::Muon>*)&mmmMuons);
//    tree->Branch("Muons", (analysis::core::Muons*)&muons);
//    tree->Branch("Muon", (analysis::core::Muon*)&myMuon);
    
//    tree->Branch("MuonNonSplit", (analysis::core::Muon*) &myMuonNonSplit, 32000, 0);
//    tree->Branch("vvMuons", (vOfvOfMuons*)&vvMuons);
    
//    tree->Branch("myTrackUnSplitted", (analysis::core::Track*)&myTrackUnSplitted, 32000, 0);
//    tree->Branch("myTrackSplitted", (analysis::core::Track*)&myTrackUnSplitted);
    tree->Branch("testClass1Splitted", (analysis::core::TestClass1*)&testClass1Splitted);
    tree->Branch("testClass1UnSplitted", 
      (analysis::core::TestClass1*)&testClass1UnSplitted, 32000, 0);

    // testing the collection of composite
    std::vector<analysis::core::TestClass2> vTestClass2Splitted;
//    tree->Branch("vTestClass2Splitted", (std::vector<analysis::core::TestClass2>*)
//        &vTestClass2Splitted);

    std::vector<TLorentzVector> vLorentz;
  //  tree->Branch("vLorentz", (std::vector<TLorentzVector>*)&vLorentz);

    for (int i=0; i<100; i++)
    {
        // clean vectors
        vInt.clear(); vDouble.clear(); vFloat.clear();
        muons.clear();
        vOfVOfInt.clear(); vOfVOfDouble.clear(); vOfVOfFloat.clear();
        myMuon.reset();
        vvMuons.clear();
        myTrackUnSplitted.reset();
        testClass1Splitted.reset();
        testClass1UnSplitted.reset();
        vTestClass2Splitted.clear();
        mInt2Int.clear();
        mInt2Float.clear();
        mInt2Double.clear();
        vPairIntInt.clear();
        mmmMuons.clear();
        mInt2MapInt2Int.clear();
        mInt2MapInt2Float.clear();
        mInt2MapInt2Double.clear();
        vOfMapInt2Int.clear();
        mInt2PairOfIntInt.clear();
        vOfVectorOfPairOfIntInt.clear();
        vLorentz.clear();

        vLorentz.push_back(TLorentzVector(1,2,3,4));

        // simple values
        a = i;
        b = (double)i;
        c = (float)i;
        d = (char)i;
        f = i%2;

        /*
        for (int i=1; i<20; i++)
        {
            myMuon._isHLTMatched.push_back(i%2==0);
            myMuon._intCheck.push_back(i);
            myMuon._floatCheck.push_back((float)i);
        }*/

        // values of a struct
        someStruct.v1 = (double)i;
        someStruct.v2 = (float)i;
        someStruct.v3 = i;
        someStruct.v3 = (char)i;
        someStruct.v5 = i%2;

        //  set the varying sized stuff
        n = i;
        for (int k=0; k<n; k++)
        {
            varr1[k] = k;
            varr2[k] = (double)k;
        }

        //  1d arrays
        /*
        for (int j=0; j<i+2; j++)
        {
            arr1[j] = j;
            arr2[j] = (double)j;
            arr3[j] = (float)j;
            arr4[j] = (char)j;
            arr5[j] = j%2;

            analysis::core::Muon mmm;
            mmm._pt = 10;
            mmm._eta = 2.1;
            mmm._phi = 1.6;
            mmm._charge = -1;
            mmm._normChi2 = 0.94;
            mmmMuons[j] = mmm;

            vInt.push_back(j);
            vDouble.push_back((double)j);
            vFloat.push_back((float)j);

            mInt2Int[j] = j;
            mInt2Float[j] = (float)j;
            mInt2Double[j] = (double)j;

            std::vector<int> tmpInt;
            std::vector<double> tmpDouble;
            std::vector<float> tmpFloat;

            std::map<int, int> tmpMapInt2Int;
            std::map<int, float> tmpMapInt2Float;
            std::map<int, double> tmpMapInt2Double;
            for (int k=0; k<20; k++)
            {
                tmpInt.push_back(k);
                tmpDouble.push_back((double)k);
                tmpFloat.push_back((float)k);

                tmpMapInt2Int[k] = k;
                tmpMapInt2Float[k] = (float)k;
                tmpMapInt2Double[k] = (double)k;
            }
                vOfVOfInt.push_back(tmpInt);
                vOfVOfDouble.push_back(tmpDouble);
                vOfVOfFloat.push_back(tmpFloat);
                mInt2MapInt2Int[j] = tmpMapInt2Int;
                mInt2MapInt2Float[j] = tmpMapInt2Float;
                mInt2MapInt2Double[j] = tmpMapInt2Double;

                vOfMapInt2Int.push_back(tmpMapInt2Int);
            vPairIntInt.push_back(std::make_pair(j, j));
            mInt2PairOfIntInt[j] = std::make_pair(j,j);
        }
        vOfVectorOfPairOfIntInt.push_back(vPairIntInt);
        myIntIntPair.first = i;
        myIntIntPair.second = i;
        
        for (int j=0; j<dims; j++)
        {
            for (int m=0; m<dims; m++)
            {
                for (int l=0; l<dims; l++)
                {
                    multi1[j][m][l] = j*m*l;
                    multi2[j][m][l] = (double)j*m*l;
                    multi3[j][m][l] = (float)j*m*l;
                    multi4[j][m][l] = (char)(j*m*l % 256);
                    multi5[j][m][l] = j*m*l % 2;
                    
                    someStruct.arr1[j][m][l] = j*m*l;
                    someStruct.arr2[j][m][l] = (double)j*m*l;
                    someStruct.arr3[j][m][l] = (float)j*m*l;
                    someStruct.arr4[j][m][l] = (char)(j*m*l%256);
                    someStruct.arr5[j][m][l] = j*m*l % 2;

                }
            }
        }
    */

 /*       // fill Muons
        int nMuons = 5;
        myMuon._pt = 10;
        myMuon._eta = 2.1;
        myMuon._phi = 1.6;
        myMuon._charge = -1;
        myMuon._normChi2 = 0.94;
        myMuonNonSplit._pt = 10.;
        myMuonNonSplit._pterr = 0.01;
        myMuonNonSplit._eta = 2.4;
        myMuonNonSplit._phi = 1.6;
        myMuonNonSplit._charge = 10;

        myTrackUnSplitted._pt = 10;
        myTrackUnSplitted._charge = -1;
        for (int ii=0; ii<nMuons; ii++)
        {
            analysis::core::Muon muon;
            muon._pt = 10;
            muon._eta = 2.1;
            muon._phi = 1.6;
            muon._charge = -1;
            muon._nPLs = ii;
            muon._normChi2 = 0.94;
            muons.push_back(muon);

            std::vector<analysis::core::Muon> tmpMuons;
            for (int jj=0; jj<nMuons; jj++)
            {
                analysis::core::Muon tmpMuon;
                tmpMuon._pt = 10;
                tmpMuon._eta = 2.1;
                tmpMuon._phi = 1.6;
                tmpMuon._charge = -1;
                tmpMuon._normChi2 = 0.94;
                tmpMuons.push_back(tmpMuon);
            }
            vvMuons.push_back(tmpMuons);
        }
        */

        // test composite
        testClass1Splitted._a = i;
        testClass1Splitted._b = (float)i;
        testClass1Splitted._c = (double)i;
        testClass1Splitted._d = i%256;
        testClass1Splitted._e = i%2==0;
  //      testClass1Splitted._f = "abcdefgh";
        testClass1UnSplitted._a = i;
        testClass1UnSplitted._b = (float)i;
        testClass1UnSplitted._c = (double)i;
        testClass1UnSplitted._d = i%256;
        testClass1UnSplitted._e = i%2==0;
//        testClass1UnSplitted._f = "abcdefgh";

        for (int ii=0; ii<10; ii++)
        {
            testClass1Splitted._va.push_back(ii);
            testClass1Splitted._vb.push_back((float)ii);
            testClass1Splitted._vc.push_back((double)ii);
            testClass1Splitted._vd.push_back(ii%256);
            
            testClass1UnSplitted._va.push_back(ii);
            testClass1UnSplitted._vb.push_back((float)ii);
            testClass1UnSplitted._vc.push_back((double)ii);
            testClass1UnSplitted._vd.push_back(ii%256);
          
            std::vector<int> tmpint;
            std::vector<float> tmpfloat;
            std::vector<double> tmpdouble;
            std::vector<char> tmpchar;
            for (int jj=0; jj<10; jj++)
            {
                testClass1Splitted._aa[ii][jj] = ii*jj;
                testClass1Splitted._bb[ii][jj] = (float)ii*jj;
                testClass1Splitted._cc[ii][jj] = (double)ii*jj;
                testClass1Splitted._dd[ii][jj] = ii*jj % 256;
                testClass1Splitted._ee[ii][jj] = ii*jj % 2 == 0;
                
                testClass1UnSplitted._aa[ii][jj] = ii*jj;
                testClass1UnSplitted._bb[ii][jj] = (float)ii*jj;
                testClass1UnSplitted._cc[ii][jj] = (double)ii*jj;
                testClass1UnSplitted._dd[ii][jj] = ii*jj % 256;
                testClass1UnSplitted._ee[ii][jj] = ii*jj % 2 == 0;

                tmpint.push_back(ii*jj);
                tmpfloat.push_back((float)ii*jj);
                tmpdouble.push_back((double)ii*jj);
                tmpchar.push_back(ii*jj % 256);
            }
            /*
            testClass1Splitted._vva.push_back(tmpint);
            testClass1Splitted._vvb.push_back(tmpfloat);
            testClass1Splitted._vvc.push_back(tmpdouble);
            testClass1Splitted._vvd.push_back(tmpchar);
            */
        }

        // testing vector of composite
/*        for (int ii=0; ii<20; ii++)
        {
            analysis::core::TestClass2 tmp2;
            tmp2._a = ii;
            tmp2._b = (float)ii;
            tmp2._c = (double)ii;
            tmp2._d = ii;
            tmp2._e = ii%2==0;
            vTestClass2Splitted.push_back(tmp2);
//            testClass1UnSplitted._vTest2.push_back(tmp2);
        }
*/
        tree->Fill();
    }
    test_file->Write();
    test_file->Close();

    return 0;
}
#endif
