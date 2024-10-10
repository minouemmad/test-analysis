#pragma once

#include "TRandom3.h"
#include "TMath.h"

struct CrystalBall{
    static const double pi;
    static const double SPiO2;
    static const double S2;

    double m;
    double s;
    double a;
    double n;

    double B;
    double C;
    double D;
    double N;

    double NA;
    double Ns;
    double NC;
    double F;
    double G;
    double k;

    double cdfMa;
    double cdfPa;

    CrystalBall(){
	init(0, 1, 10, 10);
    }
    CrystalBall(double m_, double s_, double a_, double n_){
	init(m_, s_, a_, n_);
    }

    void init(double m_, double s_, double a_, double n_){
	m=m_;
	s=s_;
	a=a_;
	n=n_;

	double fa   = fabs(a);
	double expa = exp(-fa*fa/2);
	double A    = pow(n/fa, n)*expa;
	double C1   = n/fa/(n-1)*expa; 
	double D1   = 2*SPiO2*erf(fa/S2);

	B  = n/fa-fa;
	C  = (D1+2*C1)/C1;   
	D  = (D1+2*C1)/2;   

	N  = 1.0/s/(D1+2*C1); 
	k  = 1.0/(n-1);  

	NA = N*A;       
	Ns = N*s;       
	NC = Ns*C1;     
	F  = 1-fa*fa/n; 
	G  = s*n/fa;    

	cdfMa=cdf(m-a*s);
	cdfPa=cdf(m+a*s);
    }

    double pdf(double x) const{ 
	double d=(x-m)/s;
	if(d<-a) return NA*pow(B-d, -n);
	if(d> a) return NA*pow(B+d, -n);
	return N*exp(-d*d/2);
    }

    double pdf(double x, double ks, double dm) const{ 
	double d=(x-m-dm)/(s*ks);
	if(d<-a) return NA/ks*pow(B-d, -n);
	if(d> a) return NA/ks*pow(B+d, -n);
	return N/ks*exp(-d*d/2);
    }

    double cdf(double x) const{
	double d = (x-m)/s;
	if(d<-a) return NC / pow(F-s*d/G, n-1);
	if(d> a) return NC * (C - pow(F+s*d/G, 1-n) );
	return Ns*(D-SPiO2*erf(-d/S2));
    }

    double invcdf(double u) const{
	if(u<cdfMa) return m + G*(F - pow(NC/u,    k) );
	if(u>cdfPa) return m - G*(F - pow(C-u/NC, -k) );
	return m - S2*s*TMath::ErfInverse((D - u/Ns ) / SPiO2);
    }
};
const double CrystalBall::pi    = TMath::Pi();
const double CrystalBall::SPiO2 = sqrt(TMath::Pi()/2.0);
const double CrystalBall::S2    = sqrt(2.0);


class RocRes{
    private:
	static const int NMAXETA=12;
	static const int NMAXTRK=12;

	int NETA;
	int NTRK;
	int NMIN;

	double BETA[NMAXETA+1];
	double ntrk[NMAXETA][NMAXTRK+1];
	double dtrk[NMAXETA][NMAXTRK+1];

	double width[NMAXETA][NMAXTRK];
	double alpha[NMAXETA][NMAXTRK];
	double power[NMAXETA][NMAXTRK];

	double rmsA[NMAXETA][NMAXTRK];
	double rmsB[NMAXETA][NMAXTRK];
	double rmsC[NMAXETA][NMAXTRK];

	double kDat[NMAXETA];
	double kRes[NMAXETA];

	int getBin(double x, const int NN, const double *b) const;


    public:
	enum TYPE {MC, Data, Extra};

	CrystalBall  cb[NMAXETA][NMAXTRK]; 

	RocRes();
	int getEtaBin(double feta) const;
	int getNBinDT(double v, int H) const;
	int getNBinMC(double v, int H) const;
	double getUrnd(int H, int F, double v) const;
	void dumpParams();
	void init(std::string filename);

	void reset();

	~RocRes(){}

	double Sigma(double pt, int H, int F) const;
	double kSpread(double gpt, double rpt, double eta, int nlayers, double w) const;
	double kSmear(double pt, double eta, TYPE type, double v, double u) const;
	double kSmear(double pt, double eta, TYPE type, double v, double u, int n) const;
	double kExtra(double pt, double eta, int nlayers, double u, double w) const;
	double getkDat(int H) const{return kDat[H];}
	double getkRes(int H) const{return kRes[H];}
};


class RocOne{
    private:
	static const int NMAXETA=22;
	static const int NMAXPHI=16;
	static const double MPHI;

	int NETA;
	int NPHI;

	double BETA[NMAXETA+1];
	double DPHI;

	double M[2][NMAXETA][NMAXPHI];
	double A[2][NMAXETA][NMAXPHI];
	double D[2][NMAXETA];

	RocRes RR;

	int getBin(double x, const int NN, const double *b) const;
	int getBin(double x, const int nmax, const double xmin, const double dx) const;

    public:
	enum TYPE{MC, DT};

	RocOne();
	~RocOne(){}

	RocOne(std::string filename, int iTYPE=0, int iSYS=0, int iMEM=0);
	bool checkSYS(int iSYS, int iMEM, int kSYS=0, int kMEM=0);
	bool checkTIGHT(int iTYPE, int iSYS, int iMEM, int kTYPE=0, int kSYS=0, int kMEM=0);
	void reset();
	void init(std::string filename, int iTYPE=0, int iSYS=0, int iMEM=0);

	double kScaleDT(int Q, double pt, double eta, double phi) const;
	double kScaleMC(int Q, double pt, double eta, double phi, double kSMR=1) const;
	double kScaleAndSmearMC(int Q, double pt, double eta, double phi, int n, double u, double w) const;
	double kScaleFromGenMC(int Q, double pt, double eta, double phi, int n, double gt, double w) const;
	double kGenSmear(double pt, double eta, double v, double u, RocRes::TYPE TT=RocRes::Data) const;

	double getM(int T, int H, int F) const{return M[T][H][F];}
	double getA(int T, int H, int F) const{return A[T][H][F];}
	double getK(int T, int H) const{return T==DT?RR.getkDat(H):RR.getkRes(H);}
	RocRes& getR() {return RR;}
};


class RoccoR{
    public:
	RoccoR(); 
	RoccoR(std::string dirname); 
	~RoccoR();

	void init(std::string dirname);

	double kGenSmear(double pt, double eta, double v, double u, RocRes::TYPE TT=RocRes::Data, int s=0, int m=0) const;
	double kScaleDT(int Q, double pt, double eta, double phi, int s=0, int m=0) const;

	double kScaleAndSmearMC(int Q, double pt, double eta, double phi, int n, double u, double w, int s=0, int m=0) const;  
	double kScaleFromGenMC(int Q, double pt, double eta, double phi, int n, double gt, double w, int s=0, int m=0) const; 


	double getM(int T, int H, int F, int E=0, int m=0) const{return RC[E][m].getM(T,H,F);}
	double getA(int T, int H, int F, int E=0, int m=0) const{return RC[E][m].getA(T,H,F);}
	double getK(int T, int H, int E=0, int m=0)        const{return RC[E][m].getK(T,H);}

	int Nset() const{return RC.size();}
	int Nmem(int s=0) const{return RC[s].size();}

    private:
	std::vector<std::vector<RocOne> > RC;
};

#include <fstream>
#include <sstream>
#include "TSystem.h"
#include "TMath.h"


int RocRes::getBin(double x, const int NN, const double *b) const{
    for(int i=0; i<NN; ++i) if(x<b[i+1]) return i;
    return NN-1;
}

RocRes::RocRes(){
    reset();
}

void RocRes::reset(){
    NETA=1;
    NTRK=1;
    NMIN=1;
    for(int H=0; H<NMAXETA; ++H){
	BETA[H]=0;
	kDat[H]=1.0;
	kRes[H]=1.0; //this is important
	for(int F=0; F<NMAXTRK+1; ++F){
	    ntrk[H][F]=0;
	    dtrk[H][F]=0;
	}
	for(int F=0; F<NMAXTRK; ++F){
	    width[H][F]=1;
	    alpha[H][F]=10;
	    power[H][F]=10;
	    cb[H][F].init(0.0, width[H][F], alpha[H][F], power[H][F]);
	}
    }
    BETA[NMAXETA]=0;
}

int RocRes::getEtaBin(double feta) const{
    return getBin(feta,NETA,BETA);
}

int RocRes::getNBinDT(double v, int H) const{
    return getBin(v,NTRK,dtrk[H]);
}

int RocRes::getNBinMC(double v, int H) const{
    return getBin(v,NTRK,ntrk[H]);
}

void RocRes::dumpParams(){
    using namespace std;

    cout << NMIN << endl;
    cout << NTRK << endl;
    cout << NETA << endl;
    for(int H=0; H<NETA+1; ++H) cout << BETA[H] << " ";
    cout << endl;
    for(int H=0; H<NETA; ++H){
	for(int F=0; F<NTRK; ++F){
	    cout << Form("%8.4f %8.4f %8.4f | ", width[H][F], alpha[H][F], power[H][F]);
	}
	cout << endl;
    }
    for(int H=0; H<NETA; ++H){
	for(int F=0; F<NTRK+1; ++F){
	    cout << Form("%8.4f %8.4f| ", ntrk[H][F], dtrk[H][F]);
	}
	cout << endl;
    }
    for(int H=0; H<NETA; ++H){
	for(int F=0; F<NTRK; ++F){
	    cb[H][F].init(0.0, width[H][F], alpha[H][F], power[H][F]);
	    cout << Form("%8.4f %8.4f %8.4f | ", rmsA[H][F], rmsB[H][F], rmsC[H][F]);
	}
	cout << endl;
    }
}


	
void RocRes::init(std::string filename){
    std::ifstream in(filename.c_str());
    char tag[4];
    int type, sys, mem, isdt, var, bin;	
    std::string s;
    while(std::getline(in, s)){
	std::stringstream ss(s); 
	if(s.substr(0,4)=="RMIN")       ss >> tag >> NMIN;
	else if(s.substr(0,4)=="RTRK")  ss >> tag >> NTRK;
	else if(s.substr(0,4)=="RETA")  {
	    ss >> tag >> NETA;
	    for(int i=0; i< NETA+1; ++i) ss >> BETA[i];
	}
	else if(s.substr(0,1)=="R")  {
	    ss >> tag >> type >> sys >> mem >> isdt >> var >> bin; 
	    if(var==0) for(int i=0; i<NTRK; ++i) ss >> rmsA[bin][i];  
	    if(var==1) for(int i=0; i<NTRK; ++i) ss >> rmsB[bin][i];  
	    if(var==2) for(int i=0; i<NTRK; ++i) {
		ss >> rmsC[bin][i];  
		rmsC[bin][i]/=100;
	    }
	    if(var==3) for(int i=0; i<NTRK; ++i) ss >> width[bin][i];  
	    if(var==4) for(int i=0; i<NTRK; ++i) ss >> alpha[bin][i];  
	    if(var==5) for(int i=0; i<NTRK; ++i) ss >> power[bin][i];  
	}
	else if(s.substr(0,1)=="T")  {
	    ss >> tag >> type >> sys >> mem >> isdt >> var >> bin; 
	    if(isdt==0) for(int i=0; i<NTRK+1; ++i) ss >> ntrk[bin][i];  
	    if(isdt==1) for(int i=0; i<NTRK+1; ++i) ss >> dtrk[bin][i];  
	}
	else if(s.substr(0,1)=="F")  {
	    ss >> tag >> type >> sys >> mem >> isdt >> var >> bin; 
	    if(var==0){
		if(isdt==0) for(int i=0; i<NETA; ++i) ss >> kRes[i];  
		if(isdt==1) for(int i=0; i<NETA; ++i) ss >> kDat[i];  
	    }
	}
    }

    for(int H=0; H<NETA; ++H){
	for(int F=0; F<NTRK; ++F){
	    cb[H][F].init(0.0, width[H][F], alpha[H][F], power[H][F]);
	}
    }
    in.close();
}

double RocRes::Sigma(double pt, int H, int F) const{
    double dpt=pt-45;
    return rmsA[H][F] + rmsB[H][F]*dpt + rmsC[H][F]*dpt*dpt;
}

double RocRes::getUrnd(int H, int F, double w) const{
    return ntrk[H][F]+(ntrk[H][F+1]-ntrk[H][F])*w; 
}

double RocRes::kSpread(double gpt, double rpt, double eta, int n, double w) const{
    int     H = getBin(fabs(eta), NETA, BETA);
    int     F = n>NMIN ? n-NMIN : 0;
    double  v = getUrnd(H, F, w);
    int     D = getBin(v, NTRK, dtrk[H]);
    double  kold = gpt / rpt;
    double  u = cb[H][F].cdf( (kold-1.0)/kRes[H]/Sigma(gpt,H,F) ); 
    double  knew = 1.0 + kDat[H]*Sigma(gpt,H,D)*cb[H][D].invcdf(u);
    if(knew<0) return 1.0;
    return kold/knew;
}

double RocRes::kSmear(double pt, double eta, TYPE type, double v, double u) const{
    int H = getBin(fabs(eta), NETA, BETA);
    int F = type==Data? getNBinDT(v, H) : getNBinMC(v, H);
    double K = type==Data ? kDat[H] : kRes[H]; 
    double x = K*Sigma(pt, H, F)*cb[H][F].invcdf(u);
    return 1.0/(1.0+x);
}

double RocRes::kSmear(double pt, double eta, TYPE type, double w, double u, int n) const{
    int H = getBin(fabs(eta), NETA, BETA);
    int F = n>NMIN ? n-NMIN : 0;
    if(type==Data) F = getNBinDT(getUrnd(H, F, w), H);
    double K = type==Data ? kDat[H] : kRes[H]; 
    double x = K*Sigma(pt, H, F)*cb[H][F].invcdf(u);
    return 1.0/(1.0+x);
}

double RocRes::kExtra(double pt, double eta, int n, double u, double w) const{
    int H = getBin(fabs(eta), NETA, BETA);
    int F = n>NMIN ? n-NMIN : 0;
    double  v = ntrk[H][F]+(ntrk[H][F+1]-ntrk[H][F])*w;
    int     D = getBin(v, NTRK, dtrk[H]);
    double RD = kDat[H]*Sigma(pt, H, D);
    double RM = kRes[H]*Sigma(pt, H, F);
    if(RD<=RM) return 1.0; 
    double r=cb[H][F].invcdf(u);
    if(fabs(r)>5) return 1.0; //protection against too large smearing
    double x = sqrt(RD*RD-RM*RM)*r;
    if(x<=-1) return 1.0;
    return 1.0/(1.0 + x); 
}


//-------------------------------------

const double RocOne::MPHI=-TMath::Pi();

int RocOne::getBin(double x, const int NN, const double *b) const{
    for(int i=0; i<NN; ++i) if(x<b[i+1]) return i;
    return NN-1;
}

int RocOne::getBin(double x, const int nmax, const double xmin, const double dx) const{
    int ibin=(x-xmin)/dx;
    if(ibin<0) return 0; 
    if(ibin>=nmax) return nmax-1;
    return ibin;
}

RocOne::RocOne(){
    reset();
}

RocOne::RocOne(std::string filename, int iTYPE, int iSYS, int iMEM){
    init(filename, iTYPE, iSYS, iMEM);
}


bool RocOne::checkSYS(int iSYS, int iMEM, int kSYS, int kMEM){
    if(iSYS==0 && iMEM==0)	      return true;
    if(iSYS==kSYS && iMEM==kMEM)      return true;
    return false;
}

bool RocOne::checkTIGHT(int iTYPE, int iSYS, int iMEM, int kTYPE, int kSYS, int kMEM){
    if(iTYPE!=kTYPE) return false;
    if(iSYS!=kSYS)   return false;
    if(iMEM!=kMEM)   return false;
    return true;
}

void RocOne::reset(){
    RR.reset();

    NETA=1;
    NPHI=1;
    DPHI=2*TMath::Pi()/NPHI;
    for(int H=0; H<NMAXETA; ++H){
	BETA[H]=0;
	D[MC][H]=1.0;
	D[DT][H]=1.0;
	for(int F=0; F<NMAXPHI; ++F){
	    for(int T=0; T<2; ++T){
		M[T][H][F]=1;
		A[T][H][F]=0;
	    }
	}
    }
    BETA[NMAXETA]=0;
}

void RocOne::init(std::string filename, int iTYPE, int iSYS, int iMEM){

    reset();

    RR.init(filename);

    std::ifstream in(filename.c_str());
    char tag[4];
    int type, sys, mem, isdt, var, bin;	

    bool initialized=false;

    std::string s;
    while(std::getline(in, s)){
	std::stringstream ss(s); 
	if(s.substr(0,4)=="CPHI")       {
	    ss >> tag >> NPHI;
	    DPHI=2*TMath::Pi()/NPHI;
	}
	else if(s.substr(0,4)=="CETA")  {
	    ss >> tag >> NETA;
	    for(int i=0; i< NETA+1; ++i) ss >> BETA[i];
	}
	else if(s.substr(0,1)=="C")  {
	    ss >> tag >> type >> sys >> mem >> isdt >> var >> bin; 
	    if(!checkTIGHT(type,sys,mem,iTYPE,iSYS,iMEM)) continue;
	    initialized=true;
	    if(var==0) for(int i=0; i<NPHI; ++i) { ss >> M[isdt][bin][i]; M[isdt][bin][i]/=100; M[isdt][bin][i]+=1.0;} 
	    if(var==1) for(int i=0; i<NPHI; ++i) { ss >> A[isdt][bin][i]; A[isdt][bin][i]/=100;} 

	}
	else if(s.substr(0,1)=="F")  {
	    ss >> tag >> type >> sys >> mem >> isdt >> var >> bin; 
	    if(var==1){
		for(int i=0; i<NETA; ++i) { 
		    ss >> D[isdt][i];  
		    D[isdt][i]/=10000;
		    D[isdt][i]+=1.0;
		}
	    }
	}
    }
    if(!initialized) std::cout << "Problem with input file: " << filename << std::endl;
    in.close();
}

double RocOne::kScaleDT(int Q, double pt, double eta, double phi) const{
    int H=getBin(eta, NETA, BETA);
    int F=getBin(phi, NPHI, MPHI, DPHI);
    double m=M[DT][H][F];
    double a=A[DT][H][F];
    double d=D[DT][H];

    double k=d/(m+Q*a*pt);
    return k;
}


double RocOne::kScaleMC(int Q, double pt, double eta, double phi, double kSMR) const{
    int H=getBin(eta, NETA, BETA);
    int F=getBin(phi, NPHI, MPHI, DPHI);
    double m=M[MC][H][F];
    double a=A[MC][H][F];
    double d=D[MC][H];
    double k=d/(m+Q*a*pt);
    return k*kSMR;
}

double RocOne::kScaleAndSmearMC(int Q, double pt, double eta, double phi, int n, double u, double w) const{
    double k=kScaleMC(Q, pt, eta, phi);
    return k*RR.kExtra(k*pt, eta, n, u, w);
}


double RocOne::kScaleFromGenMC(int Q, double pt, double eta, double phi, int n, double gt, double w) const{
    double k=kScaleMC(Q, pt, eta, phi);
    return k*RR.kSpread(gt, k*pt, eta, n, w);
}


double RocOne::kGenSmear(double pt, double eta, double v, double u, RocRes::TYPE TT) const{
    return RR.kSmear(pt, eta, TT, v, u);
}


//-------------------------------------


RoccoR::RoccoR(){}

RoccoR::RoccoR(std::string dirname){
    init(dirname);
}


void 
RoccoR::init(std::string dirname){

    std::string filename=Form("%s/config.txt", dirname.c_str());

    std::ifstream in(filename.c_str());
    std::string s;
    std::string tag;
    int si;
    int sn;
    while(std::getline(in, s)){
	std::stringstream ss(s); 
	ss >> tag >> si >> sn; 
	std::vector<RocOne> v;
	for(int m=0; m<sn; ++m){
	    std::string inputfile=Form("%s/%d.%d.txt", dirname.c_str(), si, m);
	    if(gSystem->AccessPathName(inputfile.c_str())) {
		std::cout << Form("Missing %8d %3d, using default instead...", si, m) << std::endl;  
		v.push_back(RocOne(Form("%s/%d.%d.txt", dirname.c_str(),0,0),0,0,0));
	    }
	    else{
		v.push_back(RocOne(inputfile, 0, si, m));
	    }
	}
	RC.push_back(v);
    }

    in.close();
}

RoccoR::~RoccoR(){}



double RoccoR::kGenSmear(double pt, double eta, double v, double u, RocRes::TYPE TT, int s, int m) const{
    return RC[s][m].kGenSmear(pt, eta, v, u, TT);
}

double RoccoR::kScaleDT(int Q, double pt, double eta, double phi, int s, int m) const{
    return RC[s][m].kScaleDT(Q, pt, eta, phi);
}

double RoccoR::kScaleAndSmearMC(int Q, double pt, double eta, double phi, int n, double u, double w, int s, int m) const{
    return RC[s][m].kScaleAndSmearMC(Q, pt, eta, phi, n, u, w);
}

double RoccoR::kScaleFromGenMC(int Q, double pt, double eta, double phi, int n, double gt, double w, int s, int m) const{
    return RC[s][m].kScaleFromGenMC(Q, pt, eta, phi, n, gt, w);
}
