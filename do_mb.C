void do_mb(TString input_file = "/gpfs01/star/pwg/lkramarik/pAu/analysis/Chain_pAu_bhtvpdnobsmd.root", Char_t* fileName="test.root", Int_t firstEvent=0, Int_t nEvents=1000) {
	TStopwatch timer;
	timer.Start();
	gROOT->ProcessLine(".x $STAR/StRoot/StMuDSTMaker/COMMON/macros/loadSharedLibraries.C");
	gSystem->Load("libWiciEvent");
	//gSystem->Load("libWiciMixer");
	gSystem->Load("libStWiciED0CorrLooper"); //nacitanie z priecinku .sl_gcc

	TFile file(input_file);

	StWiciED0CorrLooper* corr = new StWiciED0CorrLooper(WiciEventTree, fileName);
	//corr->SetCorellateEles(kTRUE);
	corr->SetUseAltTofPid(kFALSE);
	corr->SetUsePrimaryMom(kTRUE);
	//corr->SetNonLinearKaonPid(kFALSE);
	corr->SetNonLinearKaonPid(kTRUE);
	//corr->SetPtDependentPid(kFALSE);
	corr->SetPtDependentPid(kTRUE);
	//corr->SetFindMuVertex(kTRUE);
	//corr->SetUseEventMixer(kTRUE);
	corr->SetUseEventMixer(false);
	//corr->SetRefMultMin(0);
	//corr->SetRefMultMin(8);
	//corr->SetRefMultMin(78);
	//corr->SetRefMultMin(118);
	//corr->SetRefMultMin(235);
	//corr->SetRefMultMax(26);

	//corr->SetMatchToFastDet(kTRUE);
	/*
	corr->SetMatchToFastDet(kFALSE);
	corr->SetUseHighTower(false);
	corr->SetEmcTreshold(0);
	corr->SetTrgPartPtMin(0.);
	*/
	corr->SetUseHighTower(kFALSE); // dodatocny cut na trigger z BEMC, ci to triggeroval pion z mojho rozpadu D (iba triggerovane piony idu dalej)
	//corr->SetEmcTreshold(1);
//	corr->SetEmcTreshold(11); // priamo hodnota z triggeru k tomu predtym, v ADC
	//corr->SetEmcTreshold(12);
//	corr->SetTrgPartPtMin(2); // triggerovany pion

	//corr->SetPtMax(1.6);

	/*
	corr->AddTrigger(320503);
	corr->AddTrigger(330503);
	*/
	//corr->AddTrigger(480201); // BHT0*VPDMB-5
	corr->AddTrigger(500206); // BHT1*VPDMB-30
//	corr->AddTrigger(500205); // BHT2*BBCMB- trigger id 
//	corr->AddTrigger(500215); // BHT2*BBCMB-30
	corr->SetAbsVtxPosMax(50.0); //cm
	//corr->SetAbsVtxPosMax(30.0);
	corr->SetTPCVpdVtxZ(5.0); //rozdiel medzi TPC a VPD PV v Z
	//corr->SetMomKaonMin(0.8);
	corr->SetPtKaonMin(0.2); //gev
	//corr->SetPtKaonMin(0.4);
	//corr->SetPtKaonMin(0.5);
	corr->SetSigDedxKaonMax(2.0); //tpc
	corr->SetSigBtofKaonMax(2.0); //tof
	//corr->SetEtaKaonMax(0.5);
	corr->SetEtaKaonMax(1.0);

	//corr->SetMomPionMin(0.8);
	//corr->SetPtPionMin(0.2);
	corr->SetPtPionMin(0.16);
	corr->SetSigDedxPionMax(2.0);
	corr->SetSigBtofPionMax(2.0);

	//corr->SetPtSoftPionMin(0.8);
	corr->SetPtSoftPionMin(0.16);
	corr->SetSigDedxSoftPionMax(3.0);
	//corr->SetSigBtofSoftPionMax(5.0);

	//corr->SetDcaMax(3.0);
	corr->SetDcaMax(2.0); //global to PV
	//corr->SetDcaMin(1.0);
	//corr->SetDecLengthMax(0.5);
	//corr->SetDcaProductMax(0.0);
	//corr->SetPointAngleMax(0.5);
	//corr->SetKaonCosThMax(0.60);
	//corr->SetKaonCosThMax(0.77);
	corr->SetKaonCosThMax(0.80); //uhol kaon vs D0

	/*
	corr->AddBadRun(13139063);
	corr->AddBadRun(13139064);
	corr->AddBadRun(13139065);
	corr->AddBadRun(13139066);
	corr->AddBadRun(13139067);
	corr->AddBadRun(13139072);
	corr->AddBadRun(13139073);
	corr->AddBadRun(13139074);
	//corr->AddBadRun(13139080);
	corr->AddBadRun(13147030);
	*/

	corr->SetConditions(1); 
	corr->Loop(firstEvent,nEvents);
	corr->Finalize();
	timer.Stop();
	std::cout << " Time for analysis: " << timer.RealTime() << "  " << timer.CpuTime() << std::endl;
}


