#include <TFile.h>
#include <TChain.h>
#include <TSystem.h> 
#include <TROOT.h> 

#include <iostream>

using namespace std;

void Chain_new(TString out="Chain_pAu_bhtvpdnobsmd.root", TString dir="/gpfs01/star/pwg/lkramarik/pAu/produced/BHT1VPDMB30nobsmd/data/roots/") {
	gROOT->ProcessLine(".x $STAR/StRoot/StMuDSTMaker/COMMON/macros/loadSharedLibraries.C");
	gSystem->Load("WiciEvent");
	TFile* file = new TFile(out, "RECREATE");
	TChain* chain = new TChain("WiciEventTree","");
	chain->Add(dir + "*_tree.root/WiciEventTree");
	cout << chain->GetEntries() << endl;
	chain->Write();
	file->Write();
	file->Close();
	delete file;
	//delete chain;
}
