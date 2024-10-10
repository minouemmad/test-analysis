
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
    
    // test composite
    analysis::core::TestClass1 testClass1Splitted;
    analysis::core::TestClass1 testClass1UnSplitted;
    typedef std::vector<analysis::core::TestClass1> vectorTestClass1;
    typedef std::map<int, analysis::core::TestClass1> mapInt2TestClass1;
    vectorTestClass1 vTestClass1;
    mapInt2TestClass1 mInt2TestClass1;
//    tree->Branch("vTestClass1", (vectorTestClass1*)&vTestClass1, 32000, 0);
//    tree->Branch("mInt2TestClass1", (mapInt2TestClass1*)&mInt2TestClass1);

//    tree->Branch("testClass1Splitted", (analysis::core::TestClass1*)&testClass1Splitted);
//    tree->Branch("testClass1UnSplitted", 
//      (analysis::core::TestClass1*)&testClass1UnSplitted, 32000, 0);

    analysis::core::Muon muon;
 //   tree->Branch("muon", (analysis::core::Muon*)&muon, 32000, 0);

    typedef std::vector<analysis::core::TestClass2> vectorOfTestClass2;
    typedef std::map<int, analysis::core::TestClass2> mapOfInt2TestClass2;
    typedef std::vector<std::vector<analysis::core::TestClass2> > vectorOfVectorOfTestClass2;
    vectorOfTestClass2 vOfTestClass2;
    vectorOfVectorOfTestClass2 vOfVectorOfTestClass2;
    mapOfInt2TestClass2 mOfInt2TestClass2;
//    tree->Branch("vOfVectorOfTestClass2", (vectorOfVectorOfTestClass2*)&vOfVectorOfTestClass2, 32000, 0);
    tree->Branch("vOfTestClass2", (vectorOfTestClass2*)&vOfTestClass2, 32000, 0);
 //   tree->Branch("mOfInt2TestClass2", (mapOfInt2TestClass2*)&mOfInt2TestClass2);

    std::vector<int> vInt;

    std::vector<std::pair<int, int> > vPairOfIntInt;
//    tree->Branch("vPairOfIntInt", (std::vector<std::pair<int, int> >*)&vPairOfIntInt,
 //       32000, 0);
    std::map<int, std::pair<int,int> > mOfInt2PairOfIntInt;
 //   tree->Branch("mOfInt2PairOfIntInt", (std::map<int, std::pair<int,int> >*)&mOfInt2PairOfIntInt);

    std::pair<int, int> pOfIntInt;
    tree->Branch("pOfIntInt", (std::pair<int,int>*)&pOfIntInt);
 //   tree->Branch("vInt", (std::vector<int>*)&vInt);

///std::vector<TLorentzVector> vLorentz;
  //  tree->Branch("vLorentz", (std::vector<TLorentzVector>*)&vLorentz);

    for (int i=0; i<100; i++)
    {
        testClass1Splitted.reset();
        testClass1UnSplitted.reset();
        vTestClass1.clear();
        mInt2TestClass1.clear();
        vInt.clear();
        vOfTestClass2.clear();
        vOfVectorOfTestClass2.clear();
        vPairOfIntInt.clear();

        pOfIntInt.first = i;
        pOfIntInt.second = i;

        vInt.push_back(1);
        vInt.push_back(2);
        vInt.push_back(3);

        muon._charge = -1;
        muon._pt = 1.5;
        muon._pterr = 0.1;
        muon._eta = 2.1;
        muon._phi = 1.1;

        muon._track._charge = 1;
        muon._track._pt = 15.;
            
        for (int jj=2; jj<7; jj++)
            {
                analysis::core::TestClass2 tmp2;
                tmp2._a = jj;
                tmp2._b = (float)jj;
                tmp2._c = (double)jj;
                tmp2._d = jj % 256;
                tmp2._e = jj%2 == 0;
                vOfTestClass2.push_back(tmp2);
                mOfInt2TestClass2[jj] = tmp2;
                testClass1Splitted._vTest2.push_back(tmp2);

                vPairOfIntInt.push_back(std::make_pair(jj, jj));
                mOfInt2PairOfIntInt[jj] = std::make_pair(jj, jj);
            }
        testClass1Splitted._a = i;
        testClass1Splitted._b = (float)i;
        testClass1Splitted._c = (double)i;
        testClass1Splitted._d = i%256;
        testClass1Splitted._e = i%2==0;
        for (int jj=0; jj<5; jj++)
            vTestClass1.push_back(testClass1Splitted);

        for (int ii=0; ii<5; ii++)
        {
            std::vector<analysis::core::TestClass2> vtmp2;
            for (int jj=2; jj<7; jj++)
            {
                analysis::core::TestClass2 tmp2;
                tmp2._a = jj;
                tmp2._b = (float)jj;
                tmp2._c = (double)jj;
                tmp2._d = jj % 256;
                tmp2._e = jj%2 == 0;
                vtmp2.push_back(tmp2);
            }
            vOfVectorOfTestClass2.push_back(vtmp2);
        }

        // simple values
        a = i;
        b = (double)i;
        c = (float)i;
        d = (char)i;
        f = i%2;

        // values of a struct
        someStruct.v1 = (double)i;
        someStruct.v2 = (float)i;
        someStruct.v3 = i;
        someStruct.v3 = (char)i;
        someStruct.v5 = i%2 == 0;

        //  set the varying sized stuff
        n = i;
        for (int k=0; k<n; k++)
        {
            varr1[k] = k;
            varr2[k] = (double)k;
        }

        //  1d arrays
        for (int j=0; j<100; j++)
        {
            arr1[j] = j;
            arr2[j] = (double)j;
            arr3[j] = (float)j;
            arr4[j] = (char)j;
            arr5[j] = j%2 == 0;
        }
        
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
/*
        for (int kk=0; kk<5; kk++) 
        {
            testClass1Splitted.reset();
            testClass1UnSplitted.reset();

        // test composite
        testClass1Splitted._a = i;
        testClass1Splitted._b = (float)i;
        testClass1Splitted._c = (double)i;
        testClass1Splitted._d = i%256;
        testClass1Splitted._e = i%2==0;
        
        testClass1Splitted._test2._a = i;
        testClass1Splitted._test2._b = (float)i;
        testClass1Splitted._test2._c = (double)i;
        testClass1Splitted._test2._d = i%256;
        testClass1Splitted._test2._e = i%2==0;

  //      testClass1Splitted._f = "abcdefgh";
        testClass1UnSplitted._a = i;
        testClass1UnSplitted._b = (float)i;
        testClass1UnSplitted._c = (double)i;
        testClass1UnSplitted._d = i%256;
        testClass1UnSplitted._e = i%2==0;
        
        testClass1UnSplitted._test2._a = i;
        testClass1UnSplitted._test2._b = (float)i;
        testClass1UnSplitted._test2._c = (double)i;
        testClass1UnSplitted._test2._d = i%256;
        testClass1UnSplitted._test2._e = i%2==0;
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
            
            testClass1Splitted._vva.push_back(tmpint);
            testClass1Splitted._vvb.push_back(tmpfloat);
            testClass1Splitted._vvc.push_back(tmpdouble);
            testClass1Splitted._vvd.push_back(tmpchar);
            
            testClass1UnSplitted._vva.push_back(tmpint);
            testClass1UnSplitted._vvb.push_back(tmpfloat);
            testClass1UnSplitted._vvc.push_back(tmpdouble);
            testClass1UnSplitted._vvd.push_back(tmpchar);

            testClass1Splitted._ma[ii] = ii;
            testClass1Splitted._mb[ii] = (float)ii;
            testClass1Splitted._mc[ii] = (double)ii;
            testClass1Splitted._md[ii] = ii % 256;
            
            testClass1UnSplitted._ma[ii] = ii;
            testClass1UnSplitted._mb[ii] = (float)ii;
            testClass1UnSplitted._mc[ii] = (double)ii;
            testClass1UnSplitted._md[ii] = ii % 256;
        }
        
            vTestClass1.push_back(testClass1Splitted);
            mInt2TestClass1[kk] = testClass1Splitted;
        }
*/
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
