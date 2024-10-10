#include "TFile.h"
#include "TChain.h"
#include "TTree.h"
#include "TString.h"

#include "Muon.h"

#include <string>
#include <iostream>
#include <vector>

struct mystruct 
{
    double v1;
    double arr1[5][5][5];
    float v2;
    float arr2[5][5][5];
    int v3;
    int arr3[5][5][5];
//    char v4;
//    char arr4[5][5][5];
//    bool v5;
//    bool arr5[5][5][5];
};

int main(int argc, char** argv)
{

    TFile *test_file = new TFile("test_root4j_tbranch_simple.root", "recreate");
    TTree *tree = new TTree("TestTree", "TestTree");

    mystruct someStruct;
   // someStruct.v1 = 1.;
    someStruct.v2 = 1.;
    someStruct.v3 = 1;
//    someStruct.v4 = 'x';
//    someStruct.v5 = false;

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
//    tree->Branch("b", &b);
    tree->Branch("c", &c);
 //   tree->Branch("d", &d);
 //   tree->Branch("f", &f);

    //  simple array of fixed dimensions with simple types of fixed sizes
    tree->Branch("arr1", arr1, "arr1[100]/I");
//    tree->Branch("arr2", arr2, "arr2[100]/D");
    tree->Branch("arr3", arr3, "arr3[100]/F");
//    tree->Branch("arr4", arr4, "arr4[100]/B");
//    tree->Branch("arr5", arr5, "arr5[100]/O");
//    tree->Branch("str", str, "str/C");

    // simple multi-dimensional arrays 
    tree->Branch("multi1", multi1, "multi1[5][5][5]/I");
  //  tree->Branch("multi2", multi2, "multi2[5][5][5]/D");
    tree->Branch("multi3", multi3, "multi3[5][5][5]/F");
  //  tree->Branch("multi4", multi4, "multi4[5][5][5]/B");
 //   tree->Branch("multi5", multi5, "multi5[5][5][5]/O");

 //   tree->Branch("n", &n);
 //   tree->Branch("varr1", varr1, "varr1[n]/I");
 //   tree->Branch("varr2", varr2, "varr2[n]/D");

    //  Struct
    tree->Branch("someStruct", &someStruct, "v2/F:arr2[5][5][5]/F:v3/I:arr3[5][5][5]/I");
 //   tree->Branch("someStruct", &someStruct, "v1/D:arr1[5][5][5]/D:v2/F:arr2[5][5][5]/F:v3/I:arr3[5][5][5]/I:v4/B:arr4[5][5][5]/B:v5/O:arr5[5][5][5]/O");
//    tree->Branch("someStruct", &someStruct, "v1/D:v2/F:v3/I:v4/B:v5/O");

    for (int i=0; i<100; i++)
    {
        // simple values
        a = i;
        b = (double)i;
        c = (float)i;
        d = (char)i;
        f = i%2;

        // values of a struct
//        someStruct.v1 = (double)i;
        someStruct.v2 = (float)i;
        someStruct.v3 = i;
 //       someStruct.v3 = (char)i;
 //       someStruct.v5 = i%2;

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
            arr5[j] = j%2;
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
                    
                   // someStruct.arr1[j][m][l] = j*m*l;
                    someStruct.arr2[j][m][l] = (float)j*m*l;
                    someStruct.arr3[j][m][l] = j*m*l;
                   // someStruct.arr4[j][m][l] = (char)(j*m*l%256);
                   // someStruct.arr5[j][m][l] = j*m*l % 2;

                }
            }
        }

        tree->Fill();
    }
    test_file->Write();
    test_file->Close();

    return 0;
}
