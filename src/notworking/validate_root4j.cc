
#ifdef STANDALONE
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
    double arr1[2][2][2];
    float v2;
    float arr2[2][2][2];
    int v3;
    int arr3[2][2][2];
    char v4;
    char arr4[2][2][2];
    bool v5;
    bool arr5[2][2][2];
};

int main(int argc, char** argv)
{

    TFile *test_file = new TFile("test_root4j_tbranch.root", "recreate");
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

    int const dims=2;
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
    tree->Branch("multi1", multi1, "multi1[2][2][2]/I");
    tree->Branch("multi2", multi2, "multi2[2][2][2]/D");
    tree->Branch("multi3", multi3, "multi3[2][2][2]/F");
    tree->Branch("multi4", multi4, "multi4[2][2][2]/B");
    tree->Branch("multi5", multi5, "multi5[2][2][2]/O");

    tree->Branch("n", &n);
    tree->Branch("varr1", varr1, "varr1[n]/I");
    tree->Branch("varr2", varr2, "varr2[n]/D");

    //  Struct
    tree->Branch("someStruct", &someStruct, "v1/D:arr1[2][2][2]/D:v2/F:arr2[2][2][2]/F:v3/I:arr3[2][2][2]/I:v4/B:arr4[2][2][2]/B:v5/O:arr5[2][2][2]/O");
//    tree->Branch("someStruct", &someStruct, "v1/D:v2/F:v3/I:v4/B:v5/O");

    for (int i=0; i<100; i++)
    {
        a = i;
        b = (double)i;
        c = (float)i;
        f = i%2;
        someStruct.v1 = (double)i;
        someStruct.v2 = (float)i;
        someStruct.v3 = i;
        someStruct.v5 = i%2;
        n = i;
        for (int k=0; k<n; k++)
        {
            varr1[k] = k;
            varr2[k] = (double)k;
        }

        for (int j=0; j<2; j++)
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
                for (int n=0; n<dims; n++)
                {
                    multi1[j][m][n] = j*m*n;
                    multi2[j][m][n] = (double)j*m*n;
                    multi3[j][m][n] = (float)j*m*n;
                    multi4[j][m][n] = (char)(j*m*n % 256);
                    multi5[j][m][n] = j*m*n % 2;
                    
                    someStruct.arr1[j][m][n] = j*m*n;
                    someStruct.arr2[j][m][n] = (double)j*m*n;
                    someStruct.arr3[j][m][n] = (float)j*m*n;
                    someStruct.arr4[j][m][n] = 'x';
                    someStruct.arr5[j][m][n] = j*m*n % 2;

                }
            }
        }

        tree->Fill();
    }
    test_file->Write();
    test_file->Close();

    return 0;
}
#endif
