#include "StWiciED0CorrLooper.h"
#include "StarClassLibrary/SystemOfUnits.h"
#include <iostream>
#include <bitset>

ClassImp(StWiciED0CorrLooper)
	//_______________________________________________________________________________________________
	const Double_t StWiciED0CorrLooper::kPdgMass[4] = {0.0, 0.13957018, 0.493677, 0.0}; //ELEC, PION, KAON, PROT
const Double_t StWiciED0CorrLooper::d0MassRegions[4][3] = {{0.0, 1.83, 1.9}, {7.0, 1.81, 1.92}, {10.0, 1.8, 1.93}, {14.0, 1.79, 1.94}};

//_______________________________________________________________________________________________
StWiciED0CorrLooper::StWiciED0CorrLooper(TTree* inputDataset, TString outFilePrefix) {
	/*
	t410008 = pow(2, 4); // MB - vpd-zdc-tac-protected
	t410102 = pow(2, 6); // central10-protected
	t410112 = pow(2, 7); // central10-protected
	*/
	t32 = pow(2, 0); // BHT1*VPDMB-30
	t470202 = pow(2, 1); // BHT1*VPDMB-30

	//f_hot_towers = new TFile("bemc_hot_towers_pp_run15.root","open");
	Eta_c = true;
	//Eta_c = false;
	Dstar = true;
	//Dstar = false;

	kDoLambda = false;
	//cout << "Start Constructor" << endl;
	fMatchToFastDet     = kFALSE;
	fUseAltTofPid       = kFALSE;
	fNonLinearKaonPid   = kFALSE;
	fPtDependentPid     = kFALSE;
	fUsePrimaryMom      = kFALSE;
	fFindMuVertex       = kFALSE;
	fCorellateEles      = kFALSE;
	fUseEventMixer      = kFALSE;
	fUseHighTower       = kFALSE;
	/***************************************/
	/*                 Cuts                */
	/***************************************/
	fTrgArraySize       = 0;
	fTrgArraySet        = kFALSE;
	fAbsVtxPosMaxSet    = kFALSE;
	fTPCVpdVtxZSet    = kFALSE;

	fMomKaonMinSet      = kFALSE;
	fPtKaonMinSet       = kFALSE;
	fSigDedxKaonMaxSet  = kFALSE;
	fSigBtofKaonMaxSet  = kFALSE;

	fMomPionMinSet      = kFALSE;
	fPtPionMinSet       = kFALSE;
	fSigDedxPionMaxSet  = kFALSE;
	fSigBtofPionMaxSet  = kFALSE;

	//fMomSoftPionMinSet      = kFALSE;
	//fPtSoftPionMinSet       = kFALSE;
	fSigDedxSoftPionMaxSet  = kFALSE;
	fSigBtofSoftPionMaxSet  = kFALSE;
	fKaonCosThMaxArraySet   = kFALSE;

	fDcaMaxSet          = kFALSE;
	fDcaProductMinSet   = kFALSE;
	fDcaProductMaxSet   = kFALSE;
	fDecLengthMaxSet    = kFALSE;
	fOpenAngleMaxSet    = kFALSE;
	fPointAngleMaxSet   = kFALSE;
	fKaonCosThMaxSet    = kFALSE;

	fEmcTreshold  = 0;
	fTrgPartPtMin = 0.0;
	fEtaKaonMax = 3.0;

	fInputDataset = inputDataset;
	fOutFilePrefix = outFilePrefix;

	fPtMaxSet = kFALSE;
	fRefMultMinSet = kFALSE;
	fRefMultMaxSet = kFALSE;

	check_triggers = 0;
	bad_run_list.clear();
	f_cond = -1;

	fPtSoftPionMin = 0.16;
	fPtSoftPionMinSet = true;
	fPtPionMin = 0.2;
	fMomSoftPionMinSet = true;

	// fun_lw and fun_up for PID as in AuAu collisions
	fun_lw = new TF1("fun_lw","pol2",0.2,1.3);
	fun_lw->SetParameters(-7.536934,5.828048,-1.307178);

	fun_up = new TF1("fun_up","pol2",0.2,2.);
	fun_up->SetParameters(8.690754,-6.023949,1.316979);

	for (int i = 0; i < 10; i++) {
		for (int j = 0; j < 10; j++) {
			n_mix[i][j] = 0;
		}
	}

	/***************************************/
	/*          Prepear histograms         */
	/***************************************/
	const char* kCorrelType[] = {"OS_0", "LS_0", "OS_pi", "LS_pi", "NO_CORR"};
	const char* kPairType[] = {"OS", "PP", "MM"};
	const char* kTrinType[] = {"RS", "WS", "SB"};
	const char* kElecType[] = {"PE", "NPE"};

	kaon_pt_dep = new double[4]{0.2, 1.3, 2.07, 100.};
	d0_pt_dep = new double[8]{0.0, 0.7, 1.1, 1.6, 2.2, 3.0, 5.0, 8.0};

	//QA - event
	fMagFiledQA = new TH1D("QaMagField",  "Magnetic Field", 120,   -0.6,     0.6);
	fVertexQA   = new TH1D("QaVertex",    "Vertex",         120, -60.0,   60.0);
	fRefMultQA  = new TH1D("QaRefMult",   "RefMult",        100,    0.0,   100.0);
	fVpdVertexQA = new TH1D("QaVpdVertex", "VpdVertex",     120, -60.0,   60.0);
	fTPCVpdVertexZ = new TH1D("QATPCVpdVertexZ", "TPC - Vpd Vertex Z", 100, -5.0, 5.0); 
	fDca = new TH1D("Dca", "DCA",      500, 0,   5);
	h_Dca_my = new TH1F("Dca_my", "my DCA",      500, 0,   5);
	h_DcaZ = new TH1F("h_DcaZ", "DCA Z",      500, -5,   5);
	h_DcaXY = new TH2F("h_DcaXY", "DCA XY", 500, -5, 5, 500, -5, 5);
	h_VertexXY = new TH2F("h_VertexXY","VertexXY", 500, -5, 5, 500, -5, 5);

	hTofMult_RefMult = new TH2D("hTofMult_RefMult", "hTofMult_RefMult", 100, 0., 100., 200., 0., 200.);

	fKaonSigmaTof                = new TH2D("KaonSigmaTof",     "#sigma_{ToF}(K) vs p",              1000,   0.0, 10.0, 1000, -10, 10);
	fKaonSigmaTofCut             = new TH2D("KaonSigmaTofCut",  "#sigma_{ToF}(K) vs p (cut)",              1000,   0.0, 10.0, 1000, -10, 10);

	fPionSigmaTof                = new TH2D("PionSigmaTof",     "#sigma_{ToF}(#pi) vs p",              1000,   0.0, 10.0, 1000, -10, 10);
	fPionSigmaTofCut             = new TH2D("PionSigmaTofCut",  "#sigma_{ToF}(#pi) vs p (cut)",              1000,   0.0, 10.0, 1000, -10, 10);

	hKaonSigmaDedx = new TH2D("hKaonSigmaDedx", "hKaonSigmaDedx", 500., 0.0, 10.0, 500., -10.0, 10.0);
	hPionSigmaDedx = new TH2D("hPionSigmaDedx", "hPionSigmaDedx", 500., 0.0, 10.0, 500., -10.0, 10.0);
	fKaonSigmaTof_afterDedx                = new TH2D("fKaonSigmaTof_afterDedx",     "#sigma_{ToF}(K) vs p (after dEdx)",              1000,   0.0, 10.0, 1000, -10, 10);

	h_run_refmult = new TH2I("run_refmult","runID vs. RefMultiplicity", 59972, 17037, 77009, 400, 0, 400);

	h_inv_mass_US = new TH1D("h_inv_mass_US", "h_inv_mass_US", 3000., 0., 3.0);
	h_inv_mass_MIX = new TH1D("h_inv_mass_MIX", "h_inv_mass_MIX", 3000., 0., 3.0);
	kaon_pt_dep = new double[4]{0.2, 1.3, 2.07, 100.};
	d0_pt_dep = new double[8]{0.0, 0.7, 1.1, 1.6, 2.2, 3.0, 5.0, 8.0};
	for(Int_t i = 0; i < N_KAON_PT_DEP-1; i++) {
		for(Int_t j=0; j < N_D0_PT_DEP-1; j++) {
			h_inv_mass_US_pt[i][j] = new TH1D( Form("h_inv_mass_US_pt_%d_%d",i,j), Form("h_inv_mass_US_Kaon_%f_%f_D0_%f_%f",kaon_pt_dep[i],kaon_pt_dep[i+1],d0_pt_dep[j],d0_pt_dep[j+1]), 3000., 0., 3.0);
			h_inv_mass_LS_pt[i][j] = new TH1D( Form("h_inv_mass_LS_pt_%d_%d",i,j), Form("h_inv_mass_LS_Kaon_%f_%f_D0_%f_%f",kaon_pt_dep[i],kaon_pt_dep[i+1],d0_pt_dep[j],d0_pt_dep[j+1]), 3000., 0., 3.0);
			h_inv_mass_MIX_pt[i][j] = new TH1D( Form("h_inv_mass_MIX_pt_%d_%d",i,j), Form("h_inv_mass_MIX_Kaon_%f_%f_D0_%f_%f",kaon_pt_dep[i],kaon_pt_dep[i+1],d0_pt_dep[j],d0_pt_dep[j+1]), 3000., 0., 3.0);
		}
	}

	h_inv_mass_DS_plus_vs_pt = new TH2F("h_inv_mass_DS_plus_vs_pt", "h_inv_mass_DS_plus_vs_pt", 3000., 0., 3.0, 1000., 0., 100.0);
	h_inv_mass_DS_minus_vs_pt = new TH2F("h_inv_mass_DS_minus_vs_pt", "h_inv_mass_DS_minus_vs_pt", 3000., 0., 3.0, 1000., 0., 100.0);

	h_inv_mass_US_Kplus = new TH1D("h_inv_mass_US_Kplus", "h_inv_mass_US_Kplus", 3000., 0., 3.0);
	h_inv_mass_US_Kminus = new TH1D("h_inv_mass_US_Kminus", "h_inv_mass_US_Kminus", 3000., 0., 3.0);
	h_inv_mass_US_Kplus_vs_pt = new TH2D("h_inv_mass_US_Kplus_vs_pt", "h_inv_mass_US_Kplus_vs_pt", 3000., 0., 3.0, 1000., 0., 10.0);
	h_inv_mass_US_Kplus_vs_pt_kaon = new TH2D("h_inv_mass_US_Kplus_vs_pt_kaon", "h_inv_mass_US_Kplus_vs_pt_kaon", 3000., 0., 3.0, 1000., 0., 10.0);
	h_inv_mass_US_Kplus_vs_pt_pion = new TH2D("h_inv_mass_US_Kplus_vs_pt_pion", "h_inv_mass_US_Kplus_vs_pt_pion", 3000., 0., 3.0, 1000., 0., 10.0);
	h_inv_mass_US_Kminus_vs_pt = new TH2D("h_inv_mass_US_Kminus_vs_pt", "h_inv_mass_US_Kminus_vs_pt", 3000., 0., 3.0, 1000., 0., 10.0);
	h_inv_mass_US_Kminus_vs_pt_kaon = new TH2D("h_inv_mass_US_Kminus_vs_pt_kaon", "h_inv_mass_US_Kminus_vs_pt_kaon", 3000., 0., 3.0, 1000., 0., 10.0);
	h_inv_mass_US_Kminus_vs_pt_pion = new TH2D("h_inv_mass_US_Kminus_vs_pt_pion", "h_inv_mass_US_Kminus_vs_pt_pion", 3000., 0., 3.0, 1000., 0., 10.0);

	h_inv_mass_LS_Kplus = new TH1D("h_inv_mass_LS_Kplus", "h_inv_mass_LS_Kplus", 3000., 0., 3.0);
	h_inv_mass_LS_Kminus = new TH1D("h_inv_mass_LS_Kminus", "h_inv_mass_LS_Kminus", 3000., 0., 3.0);
	h_inv_mass_LS_Kplus_vs_pt = new TH2D("h_inv_mass_LS_Kplus_vs_pt", "h_inv_mass_LS_Kplus_vs_pt", 3000., 0., 3.0, 1000., 0., 10.0);
	h_inv_mass_LS_Kplus_vs_pt_kaon = new TH2D("h_inv_mass_LS_Kplus_vs_pt_kaon", "h_inv_mass_LS_Kplus_vs_pt_kaon", 3000., 0., 3.0, 1000., 0., 10.0);
	h_inv_mass_LS_Kplus_vs_pt_pion = new TH2D("h_inv_mass_LS_Kplus_vs_pt_pion", "h_inv_mass_LS_Kplus_vs_pt_pion", 3000., 0., 3.0, 1000., 0., 10.0);
	h_inv_mass_LS_Kminus_vs_pt = new TH2D("h_inv_mass_LS_Kminus_vs_pt", "h_inv_mass_LS_Kminus_vs_pt", 3000., 0., 3.0, 1000., 0., 10.0);
	h_inv_mass_LS_Kminus_vs_pt_kaon = new TH2D("h_inv_mass_LS_Kminus_vs_pt_kaon", "h_inv_mass_LS_Kminus_vs_pt_kaon", 3000., 0., 3.0, 1000., 0., 10.0);
	h_inv_mass_LS_Kminus_vs_pt_pion = new TH2D("h_inv_mass_LS_Kminus_vs_pt_pion", "h_inv_mass_LS_Kminus_vs_pt_pion", 3000., 0., 3.0, 1000., 0., 10.0);

	h_inv_mass_ROT_Kplus = new TH1D("h_inv_mass_ROT_Kplus", "h_inv_mass_ROT_Kplus", 3000., 0., 3.0);
	h_inv_mass_ROT_Kminus = new TH1D("h_inv_mass_ROT_Kminus", "h_inv_mass_ROT_Kminus", 3000., 0., 3.0);
	h_inv_mass_ROT_Kplus_vs_pt = new TH2D("h_inv_mass_ROT_Kplus_vs_pt", "h_inv_mass_ROT_Kplus_vs_pt", 3000., 0., 3.0, 1000., 0., 10.0);
	h_inv_mass_ROT_Kplus_vs_pt_kaon = new TH2D("h_inv_mass_ROT_Kplus_vs_pt_kaon", "h_inv_mass_ROT_Kplus_vs_pt_kaon", 3000., 0., 3.0, 1000., 0., 10.0);
	h_inv_mass_ROT_Kplus_vs_pt_pion = new TH2D("h_inv_mass_ROT_Kplus_vs_pt_pion", "h_inv_mass_ROT_Kplus_vs_pt_pion", 3000., 0., 3.0, 1000., 0., 10.0);
	h_inv_mass_ROT_Kminus_vs_pt = new TH2D("h_inv_mass_ROT_Kminus_vs_pt", "h_inv_mass_ROT_Kminus_vs_pt", 3000., 0., 3.0, 1000., 0., 10.0);
	h_inv_mass_ROT_Kminus_vs_pt_kaon = new TH2D("h_inv_mass_ROT_Kminus_vs_pt_kaon", "h_inv_mass_ROT_Kminus_vs_pt_kaon", 3000., 0., 3.0, 1000., 0., 10.0);
	h_inv_mass_ROT_Kminus_vs_pt_pion = new TH2D("h_inv_mass_ROT_Kminus_vs_pt_pion", "h_inv_mass_ROT_Kminus_vs_pt_pion", 3000., 0., 3.0, 1000., 0., 10.0);

	h_inv_mass_LS = new TH1D("h_inv_mass_LS", "h_inv_mass_LS", 3000., 0., 3.0);
	h_inv_mass_LS_neg = new TH1D("h_inv_mass_LS_neg", "h_inv_mass_LS_neg", 3000., 0., 3.0);
	h_inv_mass_LS_pos = new TH1D("h_inv_mass_LS_pos", "h_inv_mass_LS_pos", 3000., 0., 3.0);
	h_inv_mass_ROT = new TH1D("h_inv_mass_ROT", "h_inv_mass_ROT", 3000., 0., 3.0);
	h_pt_US = new TH1D("h_pt_US", "h_pt_US", 1000., 0., 10.0);
	h_pt_LS = new TH1D("h_pt_LS", "h_pt_LS", 1000., 0., 10.0);

	h_pt_MIX = new TH1D("h_pt_MIX", "h_pt_MIX", 1000., 0., 10.0);
	h_inv_mass_MIX_Kplus = new TH1D("h_inv_mass_MIX_Kplus", "h_inv_mass_MIX_Kplus", 3000., 0., 3.0);
	h_inv_mass_MIX_Kminus = new TH1D("h_inv_mass_MIX_Kminus", "h_inv_mass_MIX_Kminus", 3000., 0., 3.0);
	h_inv_mass_MIX_Kplus_vs_pt = new TH2D("h_inv_mass_MIX_Kplus_vs_pt", "h_inv_mass_MIX_Kplus_vs_pt", 3000., 0., 3.0, 1000., 0., 10.0);
	h_inv_mass_MIX_Kplus_vs_pt_kaon = new TH2D("h_inv_mass_MIX_Kplus_vs_pt_kaon", "h_inv_mass_MIX_Kplus_vs_pt_kaon", 3000., 0., 3.0, 1000., 0., 10.0);
	h_inv_mass_MIX_Kplus_vs_pt_pion = new TH2D("h_inv_mass_MIX_Kplus_vs_pt_pion", "h_inv_mass_MIX_Kplus_vs_pt_pion", 3000., 0., 3.0, 1000., 0., 10.0);
	h_inv_mass_MIX_Kminus_vs_pt = new TH2D("h_inv_mass_MIX_Kminus_vs_pt", "h_inv_mass_MIX_Kminus_vs_pt", 3000., 0., 3.0, 1000., 0., 10.0);
	h_inv_mass_MIX_Kminus_vs_pt_kaon = new TH2D("h_inv_mass_MIX_Kminus_vs_pt_kaon", "h_inv_mass_MIX_Kminus_vs_pt_kaon", 3000., 0., 3.0, 1000., 0., 10.0);
	h_inv_mass_MIX_Kminus_vs_pt_pion = new TH2D("h_inv_mass_MIX_Kminus_vs_pt_pion", "h_inv_mass_MIX_Kminus_vs_pt_pion", 3000., 0., 3.0, 1000., 0., 10.0);



	h_kaon_N = new TH1I("h_kaon_N", "h_kaon_N", 100, 0, 100);
	h_kaon_N_neg = new TH1I("h_kaon_N_neg", "h_kaon_N_neg", 100, 0, 100);
	h_kaon_N_pos = new TH1I("h_kaon_N_pos", "h_kaon_N_pos", 100, 0, 100);
	h_kaon_pt = new TH1D("h_kaon_pt", "h_kaon_pt", 1000., 0., 10.0);
	h_kaon_pt_neg = new TH1D("h_kaon_pt_neg", "h_kaon_pt_neg", 1000., 0., 10.0);
	h_kaon_pt_pos = new TH1D("h_kaon_pt_pos", "h_kaon_pt_pos", 1000., 0., 10.0);
	h_kaon_pt_US = new TH1D("h_kaon_pt_US", "h_kaon_pt_US", 1000., 0., 10.0);
	h_kaon_pt_LS = new TH1D("h_kaon_pt_LS", "h_kaon_pt_LS", 1000., 0., 10.0);
	h_kaon_pt_MIX = new TH1D("h_kaon_pt_MIX", "h_kaon_pt_MIX", 1000., 0., 10.0);
	h_kaon_neg_phi = new TH1D("h_kaon_neg_phi", "h_kaon_neg_phi", 64., -3.2, 3.2);
	h_kaon_pos_phi = new TH1D("h_kaon_pos_phi", "h_kaon_pos_phi", 64., -3.2, 3.2);
	h_kaon_neg_eta = new TH1D("h_kaon_neg_eta", "h_kaon_neg_eta", 100., -2.0, 2.0);
	h_kaon_pos_eta = new TH1D("h_kaon_pos_eta", "h_kaon_pos_eta", 100., -2.0, 2.0);
	h_kaon_neg_pt = new TH1D("h_kaon_neg_pt", "h_kaon_neg_pt", 1000., 0., 10.0);
	h_kaon_pos_pt = new TH1D("h_kaon_pos_pt", "h_kaon_pos_pt", 1000., 0., 10.0);
	h_kaon_neg_eta_pt = new TH2D("h_kaon_neg_eta_pt", "h_kaon_neg_eta_pt", 400., -2.0, 2.0, 1000., 0., 10.0);
	h_kaon_pos_eta_pt = new TH2D("h_kaon_pos_eta_pt", "h_kaon_pos_eta_pt", 400., -2.0, 2.0, 1000., 0., 10.0);
	h_kaon_pt_vs_eta = new TH2D("h_kaon_pt_vs_eta", "h_kaon_pt_vs_eta", 500., 0.0, 10.0, 100., -2.0, 2.0);
	h_kaon_pt_vs_dedx = new TH2D("h_kaon_pt_vs_dedx", "h_kaon_pt_vs_dedx", 500., 0.0, 10.0, 500., -10.0, 10.0);
	h_kaon_pt_vs_oneOverBetaTof = new TH2D("h_kaon_pt_vs_oneOverBetaTof", "p_{T} vs 1/#beta(ToF)(K)", 500., 0.0, 10.0, 500., -10.0, 10.0);

	h_kaon_p_vs_dedx = new TH2D("h_kaon_p_vs_dedx", "h_kaon_p_vs_dedx", 500., 0.0, 10.0, 500., -10.0, 10.0);
	h_kaon_p_vs_oneOverBetaTof = new TH2D("h_kaon_p_vs_oneOverBetaTof", "h_kaon_p_vs_oneOverBetaTof", 500., 0.0, 10.0, 500., -10.0, 10.0);
	h_kaon_pt_vs_sigmaDedx = new TH2D("h_kaon_pt_vs_sigmaDedx", "h_kaon_pt_vs_sigmaDedx", 500., 0.0, 10.0, 500., -10.0, 10.0);
	h_kaon_pt_vs_sigmaTof = new TH2D("h_kaon_pt_vs_sigmaTof", "h_kaon_pt_vs_sigmaTof", 500., 0.0, 10.0, 500., -10.0, 10.0);
	h_kaon_p_vs_sigmaDedx = new TH2D("h_kaon_p_vs_sigmaDedx", "h_kaon_p_vs_sigmaDedx", 500., 0.0, 10.0, 500., -10.0, 10.0);
	h_kaon_p_vs_sigmaTof = new TH2D("h_kaon_p_vs_sigmaTof", "h_kaon_p_vs_sigmaTof", 500., 0.0, 10.0, 500., -10.0, 10.0);

	h_kaon_p_vs_dedx_phimeson = new TH2D("h_kaon_p_vs_dedx_phimeson", "h_kaon_p_vs_dedx_phimeson", 500., 0.0, 10.0, 500., -10.0, 10.0);
	h_kaon_p_vs_oneOverBetaTof_phimeson = new TH2D("h_kaon_p_vs_oneOverBetaTof_phimeson", "h_kaon_p_vs_oneOverBetaTof_phimeson", 500., 0.0, 10.0, 500., -10.0, 10.0);
	h_kaon_p_vs_sigmaDedx_phimeson = new TH2D("h_kaon_p_vs_sigmaDedx_phimeson", "h_kaon_p_vs_sigmaDedx_phimeson", 500., 0.0, 10.0, 500., -10.0, 10.0);
	h_kaon_p_vs_sigmaTof_phimeson = new TH2D("h_kaon_p_vs_sigmaTof_phimeson", "h_kaon_p_vs_sigmaTof_phimeson", 500., 0.0, 10.0, 500., -10.0, 10.0);

	h_pion_pt_vs_eta = new TH2D("h_pion_pt_vs_eta", "h_pion_pt_vs_eta", 500., 0.0, 10.0, 100., -2.0, 2.0);
	h_pion_pt_vs_dedx = new TH2D("h_pion_pt_vs_dedx", "h_pion_pt_vs_dedx", 500., 0.0, 10.0, 500., -10.0, 10.0);
	h_pion_p_vs_dedx = new TH2D("h_pion_p_vs_dedx", "h_pion_p_vs_dedx", 500., 0.0, 10.0, 500., -10.0, 10.0);
	h_pion_pt_vs_oneOverBetaTof = new TH2D("h_pion_pt_vs_oneOverBetaTof", "p_{T} vs 1/#beta(ToF)(K)", 500., 0.0, 10.0, 500., -10.0, 10.0);
	h_pion_p_vs_oneOverBetaTof = new TH2D("h_pion_p_vs_oneOverBetaTof", "h_pion_p_vs_oneOverBetaTof", 500., 0.0, 10.0, 500., -10.0, 10.0);
	h_pion_pt_vs_sigmaDedx = new TH2D("h_pion_pt_vs_sigmaDedx", "h_pion_pt_vs_sigmaDedx", 500., 0.0, 10.0, 500., -10.0, 10.0);
	h_pion_p_vs_sigmaDedx = new TH2D("h_pion_p_vs_sigmaDedx", "h_pion_p_vs_sigmaDedx", 500., 0.0, 10.0, 500., -10.0, 10.0);
	h_pion_pt_vs_sigmaTof = new TH2D("h_pion_pt_vs_sigmaTof", "h_pion_pt_vs_sigmaTof", 500., 0.0, 10.0, 500., -10.0, 10.0);
	h_pion_p_vs_sigmaTof = new TH2D("h_pion_p_vs_sigmaTof", "h_pion_p_vs_sigmaTof", 500., 0.0, 10.0, 500., -10.0, 10.0);


	h_all_pt_vs_dedx = new TH2D("h_all_pt_vs_dedx", "h_all_pt_vs_dedx", 1000., 0.0, 10.0, 1000., -10.0, 10.0);
	h_all_pt_vs_oneOverBetaTof = new TH2D("h_all_pt_vs_oneOverBetaTof", "p_{T} vs 1/#beta(ToF)", 1000., 0.0, 10.0, 1000., 0.0, 10.0);

	h_pion_N = new TH1I("h_pion_N", "h_pion_N", 1000, 0, 1000);
	h_pion_N_neg = new TH1I("h_pion_N_neg", "h_pion_N_neg", 1000, 0, 1000);
	h_pion_N_pos = new TH1I("h_pion_N_pos", "h_pion_N_pos", 1000, 0, 1000);
	h_pion_pt = new TH1D("h_pion_pt", "h_pion_pt", 1000., 0., 10.0);
	h_pion_pt_neg = new TH1D("h_pion_pt_neg", "h_pion_pt_neg", 1000., 0., 10.0);
	h_pion_pt_pos = new TH1D("h_pion_pt_pos", "h_pion_pt_pos", 1000., 0., 10.0);
	h_pion_phi_neg = new TH1D("h_pion_phi_neg", "h_pion_phi_neg", 64., -3.2, 3.2);
	h_pion_phi_pos = new TH1D("h_pion_phi_pos", "h_pion_phi_pos", 64., -3.2, 3.2);
	h_pion_pt_US = new TH1D("h_pion_pt_US", "h_pion_pt_US", 1000., 0., 10.0);
	h_pion_pt_LS = new TH1D("h_pion_pt_LS", "h_pion_pt_LS", 1000., 0., 10.0);
	h_pion_pt_MIX = new TH1D("h_pion_pt_MIX", "h_pion_pt_MIX", 1000., 0., 10.0);

	h_K0s_inv_mass_vs_pt = new TH2D("h_K0s_inv_mass_vs_pt", "h_K0s_inv_mass_vs_pt", 1000, 0., 1.0, 1000, 0., 10.0);
	h_K0s_phi = new TH1D("h_K0s_phi", "h_K0s_phi", 64, -3.2, 3.2);
	h_K0s_eta = new TH1D("h_K0s_eta", "h_K0s_eta", 100, -2.0, 2.0);
	h_K0sBckg_inv_mass_vs_pt = new TH2D("h_K0sBckg_inv_mass_vs_pt", "h_K0sBckg_inv_mass_vs_pt", 1000, 0., 1.0, 1000, 0., 10.0);

	h_Eta_c_inv_mass_vs_pt = new TH2D("h_Eta_c_inv_mass_vs_pt", "h_Eta_c_inv_mass_vs_pt", 1000, 2.0, 4.0, 1000, 0., 20.0);
	h_Eta_c_inv_mass_vs_eta = new TH2D("h_Eta_c_inv_mass_vs_eta", "h_Eta_c_inv_mass_vs_eta", 1000, 2.0, 4.0, 100, -2.0, 2.0);
	h_Eta_c_phi = new TH1D("h_Eta_c_phi", "h_Eta_c_phi", 64., -3.2, 3.2);
	h_Eta_c_eta = new TH1D("h_Eta_c_eta", "h_Eta_c_eta", 100., -2.0, 2.0);
	h_Eta_c_inv_mass = new TH1D("h_Eta_c_inv_mass", "h_Eta_c_inv_mass", 500., 2.0, 4.0);

	h_Eta_c_angle = new TH1D("h_Eta_c_angle", "h_Eta_c_angle", 68., -3.4, 3.4);
	h_Eta_c_cos_theta_star_pos = new TH1D("h_Eta_c_cos_theta_star_pos", "h_Eta_c_cos_theta_star_pos", 200., -2., 2.);
	h_Eta_c_cos_theta_star_neg = new TH1D("h_Eta_c_cos_theta_star_neg", "h_Eta_c_cos_theta_star_neg", 200., -2., 2.);

	h_KK_inv_mass_vs_pt = new TH2D("h_KK_inv_mass_vs_pt", "h_KK_inv_mass_vs_pt", 400., 0.8, 1.2, 1000., 0., 10.0);
	h_KK_inv_mass_vs_pt_2gev_us = new TH2F("h_KK_inv_mass_vs_pt_2gev_us", "h_KK_inv_mass_vs_pt_2gev_us", 1200., 0.8, 2.0, 1000., 0., 10.0);
	h_KK_inv_mass_vs_pt_2gev_lsp = new TH2F("h_KK_inv_mass_vs_pt_2gev_lsp", "h_KK_inv_mass_vs_pt_2gev_lsp", 1200., 0.8, 2.0, 1000., 0., 10.0);
	h_KK_inv_mass_vs_pt_2gev_lsm = new TH2F("h_KK_inv_mass_vs_pt_2gev_lsm", "h_KK_inv_mass_vs_pt_2gev_lsm", 1200., 0.8, 2.0, 1000., 0., 10.0);
	h_KK_phi = new TH1D("h_KK_phi", "h_KK_phi", 64., -3.2, 3.2);
	h_KK_eta = new TH1D("h_KK_eta", "h_KK_eta", 100., -2.0, 2.0);
	h_KK_pt = new TH1D("h_KK_pt", "h_KK_pt", 1000., 0., 10.0);
	//h_KK_inv_mass = new TH1D("h_KK_inv_mass", "h_KK_inv_mass", 400., 0.8, 1.2);
	//h_KK_minus_inv_mass = new TH1D("h_KK_minus_inv_mass", "h_KK_minus_inv_mass", 400, 0.8, 1.2);
	//h_KK_plus_inv_mass = new TH1D("h_KK_plus_inv_mass", "h_KK_plus_inv_mass", 400, 0.8, 1.2);
	h_KK_inv_mass = new TH1D("h_KK_inv_mass", "h_KK_inv_mass", 2000, 0., 20.0);
	//h_KK_inv_mass_phi = new TH1D("h_KK_inv_mass_phi", "h_KK_inv_mass_phi", 400, 0.8, 1.2);
	h_KK_minus_inv_mass = new TH1D("h_KK_minus_inv_mass", "h_KK_minus_inv_mass", 3200, 0.8, 4.0);
	h_KK_plus_inv_mass = new TH1D("h_KK_plus_inv_mass", "h_KK_plus_inv_mass", 3200, 0.8, 4.0);

	h_KK_angle = new TH1D("h_KK_angle", "h_KK_angle", 68., -3.4, 3.4);
	h_KK_cos_theta_star_pos = new TH1D("h_KK_cos_theta_star_pos", "h_KK_cos_theta_star_pos", 200., -2., 2.);
	h_KK_cos_theta_star_neg = new TH1D("h_KK_cos_theta_star_neg", "h_KK_cos_theta_star_neg", 200., -2., 2.);

	h_Dstar_RS_vs_pt = new TH2D("h_Dstar_RS_vs_pt", "h_Dstar_RS_vs_pt", 400, 0.135, 0.175, 100, 0.0, 10.0);
	h_Dstar_WS_vs_pt = new TH2D("h_Dstar_WS_vs_pt", "h_Dstar_WS_vs_pt", 400, 0.135, 0.175, 100, 0.0, 10.0);
	h_Dstar_Sideband_vs_pt = new TH2D("h_Dstar_Sideband_vs_pt", "h_Dstar_Sideband_vs_pt", 400, 0.135, 0.175, 100, 0.0, 10.0);
	h_Dstar_RS_vs_pt_softtest = new TH2D("h_Dstar_RS_vs_pt_softtest", "h_Dstar_RS_vs_pt_softtest", 400, 0.135, 0.175, 100, 0.0, 10.0);
	h_Dstar_WS_vs_pt_softtest = new TH2D("h_Dstar_WS_vs_pt_softtest", "h_Dstar_WS_vs_pt_softtest", 400, 0.135, 0.175, 100, 0.0, 10.0);
	h_Dstar_Sideband_vs_pt_softtest = new TH2D("h_Dstar_Sideband_vs_pt_softtest", "h_Dstar_Sideband_vs_pt_softtest", 400, 0.135, 0.175, 100, 0.0, 10.0);
	h_Dstar_RS_pt_vs_soft_pion_pt = new TH2D("h_Dstar_RS_pt_vs_soft_pion_pt", "h_Dstar_RS_pt_vs_soft_pion_pt", 100, 0.0, 10.0, 100, 0.0, 1.0);
	h_Dstar_WS_pt_vs_soft_pion_pt = new TH2D("h_Dstar_WS_pt_vs_soft_pion_pt", "h_Dstar_WS_pt_vs_soft_pion_pt", 100, 0.0, 10.0, 100, 0.0, 1.0);
	//h_kaon_cos_theta_star_vs_pt_D0 = new TH2D("h_kaon_cos_theta_star_vs_pt_D0", "h_kaon_cos_theta_star_vs_pt_D0", 100, 0., 1., 100, 0.0, 10.0);
	h_pt_D0_vs_kaon_cos_theta_star = new TH2D("h_pt_D0_vs_kaon_cos_theta_star", "h_pt_D0_vs_kaon_cos_theta_star", 100, 0., 10., 100, 0.0, 1.0);

	h_track_statistics = new TH1D("h_track_statistics", "h_track_statistics", 12, -1, 11);
	for (int i = 0; i < 6; i++) {
		h_track_statistics_pt[i] = new TH1D( Form("h_track_statistics_pt_%d", i), Form("h_track_statistics_pt_%d", i), 12, -1, 11);
	}

	//h_multi_trigg = new TH1I("h_multi_trigg", "h_multi_trigg", 10, -0.5, 9.5);
	h_multi_trigg = new TH1I("h_multi_trigg", "h_multi_trigg", 10, 0, 10);
	h_multi_trigg_2D = new TH2I("h_multi_trigg_2D", "h_multi_trigg_2D", 10, 0, 10, 10, 0, 10);

	h_BTofYLocal = new TH1F("h_BTofYLocal", "h_BTofYLocal", 100, -5., 5.);
	h_BTofYLocal_kaon = new TH1F("h_BTofYLocal_kaon", "h_BTofYLocal_kaon", 100, -5., 5.);
	h_BTofYLocal_pion = new TH1F("h_BTofYLocal_pion", "h_BTofYLocal_pion", 100, -5., 5.);

	h_inv_mass_US->Sumw2();
	h_inv_mass_MIX->Sumw2();
	h_inv_mass_LS->Sumw2();
	h_inv_mass_LS_neg->Sumw2();
	h_inv_mass_LS_pos->Sumw2();
	h_inv_mass_ROT->Sumw2();
	h_inv_mass_US_Kminus_vs_pt->Sumw2();
	h_inv_mass_US_Kplus_vs_pt->Sumw2();
	h_inv_mass_MIX_Kminus_vs_pt->Sumw2();
	h_inv_mass_MIX_Kplus_vs_pt->Sumw2();

	h_Dstar_RS_vs_pt->Sumw2();
	h_Dstar_WS_vs_pt->Sumw2();
	h_Dstar_Sideband_vs_pt->Sumw2();
	h_Dstar_RS_vs_pt_softtest->Sumw2();
	h_Dstar_WS_vs_pt_softtest->Sumw2();
	h_Dstar_Sideband_vs_pt_softtest->Sumw2();

	h_N_cuts = new TH1I("h_N_cuts", "h_N_cuts", 10, 0, 10);

	fADCvsP                      = new TH2D("QaACDvsP",         "ADC vs p",                           200,   0.0, 20.0,  200, 0.0, 200.0);
	fADCvsPcut                   = new TH2D("QaACDvsPcut",      "ADC vs p cut",                       200,   0.0, 20.0,  200, 0.0, 200.0);
	fADCvsPt                     = new TH2D("QaACDvsPt",        "ADC vs p_{T}",                       200,   0.0, 20.0,  200, 0.0, 200.0);
	fADCvsPtCut                  = new TH2D("QaACDvsPtCut",     "ADC vs p_{T} cut",                   200,   0.0, 20.0,  200, 0.0, 200.0);
	fEnergyVsP                   = new TH2D("QaEnergyVsP",      "Energy vs p",                        200,   0.0, 20.0, 200, 0.0, 20.0);
	fEnergyVsPcut                = new TH2D("QaEnergyVsPcut",   "Energy vs p cut",                    200,   0.0, 20.0, 200, 0.0, 20.0);
	fEnergyVsPt                  = new TH2D("QaEnergyVsPt",     "Energy vs p_{T}",                    200,   0.0, 20.0, 200, 0.0, 20.0);
	fEnergyVsPtCut               = new TH2D("QaEnergyVsPtCut",  "Energy vs p_{T} cut",                200,   0.0, 20.0, 200, 0.0, 20.0);

	h_zdc_pionN = new TH2I("h_zdc_pionN", "h_zdc_pionN", 500, 0, 50000, 100, 0, 100);
	h_zdc_pionN_dedx = new TH2I("h_zdc_pionN_dedx", "h_zdc_pionN_dedx", 500, 0, 50000, 100, 0, 100);
	h_zdc_refmult = new TH2I("h_zdc_refmult", "h_zdc_refmult", 500, 0, 50000, 100, 0, 100);
	h_bbc_pionN = new TH2I("h_bbc_pionN", "h_bbc_pionN", 500, 0, 5000000, 100, 0, 100);
	h_bbc_kaonN = new TH2I("h_bbc_kaonN", "h_bbc_kaonN", 500, 0, 5000000, 100, 0, 100);
	h_bbc_pionN_dedx = new TH2I("h_bbc_pionN_dedx", "h_bbc_pionN_dedx", 500, 0, 5000000, 100, 0, 100);
	h_bbc_refmult = new TH2I("h_bbc_refmult", "h_bbc_refmult", 500, 0, 5000000, 100, 0, 100);


	//inv_mass_us = new vector<float>;
	inv_mass_us = 0;
	pt_kaon_us = 0;
	pt_pion_us = 0;
	pt_D0_us = 0;
	inv_mass_ls = 0;
	pt_kaon_ls = 0;
	pt_pion_ls = 0;
	pt_D0_ls = 0;
	inv_mass_rot = 0;

	output_file = new TFile (Form("%s.root", fOutFilePrefix.Data()), "RECREATE");
	t_output = new TTree("t_output","D0 tree");
	t_output->Branch("RunId", &RunId, "RunId/I");
	t_output->Branch("EventId", &EventId, "EventId/I");
	t_output->Branch("RefMult", &RefMult, "RefMult/I");
	t_output->Branch("inv_mass_us", &inv_mass_us);
	t_output->Branch("pt_kaon_us", &pt_kaon_us);
	t_output->Branch("pt_pion_us", &pt_pion_us);
	t_output->Branch("pt_D0_us", &pt_D0_us);
	t_output->Branch("inv_mass_ls", &inv_mass_ls);
	t_output->Branch("pt_kaon_ls", &pt_kaon_ls);
	t_output->Branch("pt_pion_ls", &pt_pion_ls);
	t_output->Branch("pt_D0_ls", &pt_D0_ls);
	t_output->Branch("inv_mass_rot", &inv_mass_rot);

	v_kaons_pos.clear();
	v_kaons_neg.clear();
	v_pions_pos.clear();
	v_pions_neg.clear();
	v_pions_ht_neg.clear();
	v_pions_ht_pos.clear();
	v_kaons_neg_sigmaDedx.clear();
	v_kaons_neg_sigmaTof.clear();
	v_kaons_pos_sigmaDedx.clear();
	v_kaons_pos_sigmaTof.clear();
}
//_______________________________________________________________________________________________
StWiciED0CorrLooper::~StWiciED0CorrLooper() {
	delete fMagFiledQA;
	delete fVertexQA;
	delete fDca;
	delete h_VertexXY;
	delete h_DcaXY;
	delete h_DcaZ;
	delete h_Dca_my;
	delete fRefMultQA;
	delete fVpdVertexQA;
	delete fTPCVpdVertexZ;
	delete h_run_refmult;
	delete hTofMult_RefMult;
	delete fKaonSigmaTof;
	delete fKaonSigmaTof_afterDedx;
	delete fKaonSigmaTofCut;

	delete fPionSigmaTof;
	delete fPionSigmaTofCut;

	delete hKaonSigmaDedx;
	delete hPionSigmaDedx;

	for (int i_kaon_pt = 0; i_kaon_pt < N_KAON_PT_DEP-1; i_kaon_pt++) {
		for (int i_d0_pt = 0; i_d0_pt < N_D0_PT_DEP-1; i_d0_pt++) {
			delete h_inv_mass_US_pt[i_kaon_pt][i_d0_pt];
			delete h_inv_mass_LS_pt[i_kaon_pt][i_d0_pt];
			delete h_inv_mass_MIX_pt[i_kaon_pt][i_d0_pt];
		}
	}

	delete h_inv_mass_DS_plus_vs_pt;
	delete h_inv_mass_DS_minus_vs_pt;

	delete h_inv_mass_US;
	delete h_inv_mass_LS;
	delete h_inv_mass_LS_neg;
	delete h_inv_mass_LS_pos;
	delete h_inv_mass_ROT;
	delete h_inv_mass_US_Kplus;
	delete h_inv_mass_US_Kminus;
	delete h_inv_mass_US_Kplus_vs_pt;
	delete h_inv_mass_US_Kplus_vs_pt_kaon;
	delete h_inv_mass_US_Kplus_vs_pt_pion;
	delete h_inv_mass_US_Kminus_vs_pt;
	delete h_inv_mass_US_Kminus_vs_pt_kaon;
	delete h_inv_mass_US_Kminus_vs_pt_pion;

	delete h_inv_mass_LS_Kplus;
	delete h_inv_mass_LS_Kminus;
	delete h_inv_mass_LS_Kplus_vs_pt;
	delete h_inv_mass_LS_Kplus_vs_pt_kaon;
	delete h_inv_mass_LS_Kplus_vs_pt_pion;
	delete h_inv_mass_LS_Kminus_vs_pt;
	delete h_inv_mass_LS_Kminus_vs_pt_kaon;
	delete h_inv_mass_LS_Kminus_vs_pt_pion;

	delete h_inv_mass_ROT_Kplus;
	delete h_inv_mass_ROT_Kminus;
	delete h_inv_mass_ROT_Kplus_vs_pt;
	delete h_inv_mass_ROT_Kplus_vs_pt_kaon;
	delete h_inv_mass_ROT_Kplus_vs_pt_pion;
	delete h_inv_mass_ROT_Kminus_vs_pt;
	delete h_inv_mass_ROT_Kminus_vs_pt_kaon;
	delete h_inv_mass_ROT_Kminus_vs_pt_pion;

	delete h_inv_mass_MIX;
	delete h_inv_mass_MIX_Kplus;
	delete h_inv_mass_MIX_Kminus;
	delete h_inv_mass_MIX_Kplus_vs_pt;
	delete h_inv_mass_MIX_Kplus_vs_pt_kaon;
	delete h_inv_mass_MIX_Kplus_vs_pt_pion;
	delete h_inv_mass_MIX_Kminus_vs_pt;
	delete h_inv_mass_MIX_Kminus_vs_pt_kaon;
	delete h_inv_mass_MIX_Kminus_vs_pt_pion;

	delete h_kaon_pt;
	delete h_kaon_pt_neg;
	delete h_kaon_pt_pos;
	delete h_pion_pt;
	delete h_pion_pt_neg;
	delete h_pion_pt_pos;
	delete h_pion_phi_neg;
	delete h_pion_phi_pos;
	delete h_kaon_N; 
	delete h_kaon_N_neg;
	delete h_kaon_N_pos;
	delete h_pion_N;	
	delete h_pion_N_neg;	
	delete h_pion_N_pos;	
	delete h_kaon_pt_US;
	delete h_kaon_pt_LS;
	delete h_kaon_pt_MIX;
	delete h_pion_pt_US;
	delete h_pion_pt_LS;
	delete h_pion_pt_MIX;
	delete h_pt_US;
	delete h_pt_LS;
	delete h_pt_MIX;
	delete h_kaon_neg_eta_pt;
	delete h_kaon_neg_eta;
	delete h_kaon_pos_eta_pt;
	delete h_kaon_pos_eta;
	delete h_kaon_neg_phi;
	delete h_kaon_pos_phi;
	delete h_kaon_neg_pt;
	delete h_kaon_pos_pt;
	delete h_kaon_pt_vs_eta;
	delete h_kaon_pt_vs_dedx;
	delete h_kaon_pt_vs_oneOverBetaTof;
	delete h_kaon_p_vs_dedx;
	delete h_kaon_p_vs_oneOverBetaTof;
	delete h_kaon_pt_vs_sigmaDedx;
	delete h_kaon_pt_vs_sigmaTof;
	delete h_kaon_p_vs_sigmaDedx;
	delete h_kaon_p_vs_sigmaTof;

	delete h_kaon_p_vs_dedx_phimeson;
	delete h_kaon_p_vs_oneOverBetaTof_phimeson;
	delete h_kaon_p_vs_sigmaDedx_phimeson;
	delete h_kaon_p_vs_sigmaTof_phimeson;

	delete h_pion_pt_vs_eta;
	delete h_pion_pt_vs_dedx;
	delete h_pion_pt_vs_oneOverBetaTof;
	delete h_pion_p_vs_dedx;
	delete h_pion_p_vs_oneOverBetaTof;
	delete h_pion_pt_vs_sigmaDedx;
	delete h_pion_pt_vs_sigmaTof;
	delete h_pion_p_vs_sigmaDedx;
	delete h_pion_p_vs_sigmaTof;

	delete h_all_pt_vs_dedx;
	delete h_all_pt_vs_oneOverBetaTof;

	delete h_K0s_inv_mass_vs_pt;
	delete h_K0s_phi;
	delete h_K0s_eta;
	delete h_K0sBckg_inv_mass_vs_pt;

	delete h_Eta_c_inv_mass_vs_pt;
	delete h_Eta_c_inv_mass_vs_eta;
	delete h_Eta_c_phi;
	delete h_Eta_c_eta;
	delete h_Eta_c_inv_mass;

	delete h_Eta_c_angle;
	delete h_Eta_c_cos_theta_star_pos;
	delete h_Eta_c_cos_theta_star_neg;

	delete h_KK_inv_mass_vs_pt;
	delete h_KK_inv_mass_vs_pt_2gev_us;
	delete h_KK_inv_mass_vs_pt_2gev_lsp;
	delete h_KK_inv_mass_vs_pt_2gev_lsm;
	delete h_KK_phi;
	delete h_KK_eta;
	delete h_KK_pt;
	delete h_KK_inv_mass;
	delete h_KK_minus_inv_mass;
	delete h_KK_plus_inv_mass;

	delete h_KK_angle;
	delete h_KK_cos_theta_star_pos;
	delete h_KK_cos_theta_star_neg;

	delete h_Dstar_RS_vs_pt;
	delete h_Dstar_WS_vs_pt;
	delete h_Dstar_Sideband_vs_pt;
	delete h_Dstar_RS_vs_pt_softtest;
	delete h_Dstar_WS_vs_pt_softtest;
	delete h_Dstar_Sideband_vs_pt_softtest;
	delete h_Dstar_RS_pt_vs_soft_pion_pt;
	delete h_Dstar_WS_pt_vs_soft_pion_pt;
	delete h_pt_D0_vs_kaon_cos_theta_star;

	delete h_track_statistics;
	delete h_N_cuts;
	//delete[] h_track_statistics_pt;
	for (int i = 0; i < 6; i++) {
		delete h_track_statistics_pt[i];
	}

	delete h_multi_trigg;
	delete h_multi_trigg_2D;

	delete h_BTofYLocal;
	delete h_BTofYLocal_kaon;
	delete h_BTofYLocal_pion;

	delete fADCvsP;
	delete fADCvsPcut;
	delete fADCvsPt;
	delete fADCvsPtCut;
	delete fEnergyVsP;
	delete fEnergyVsPcut;
	delete fEnergyVsPt;
	delete fEnergyVsPtCut;

	delete h_zdc_pionN;
	delete h_zdc_pionN_dedx;
	delete h_zdc_refmult;
	delete h_bbc_pionN;
	delete h_bbc_kaonN;
	delete h_bbc_pionN_dedx;
	delete h_bbc_refmult;

	//delete[] pt_dep;
	delete[] kaon_pt_dep;
	delete[] d0_pt_dep;
	//delete t_output;
	delete output_file;
	delete h_hot_towers;
	delete f_hot_towers;
}
//_______________________________________________________________________________________________
void StWiciED0CorrLooper::Loop(const Int_t firstEvent, Int_t nEvents) {
	//loading tree
	Int_t nEntries = fInputDataset->GetEntries();
	cout << "Number of Entries: " << nEntries << endl;
	TBranch*  branch = NULL;
	WiciEvent* event = NULL;
	fInputDataset->SetBranchAddress("WiciEventBranch", &event, &branch);
	fInputDataset->LoadTree(firstEvent);

	if (f_cond != -1) {
		cout << "Conditions: " << f_cond << endl;
	}	

	/***************************************/
	/*           Perform analysis          */
	/***************************************/
	Long64_t frstEnt    = firstEvent;
	Long64_t lastEnt    = nEvents>0 ? firstEvent+nEvents : nEntries;
	if(lastEnt>nEntries) lastEnt = nEntries;
	Long64_t procEnt    = 0; //events processed up till now
	Long64_t currEnt    = frstEnt; //position in the dataset
	Long64_t allEnt     = nEntries;
	Long64_t totEnt     = lastEnt-frstEnt;

	Int_t    nSteps     = 100;
	Double_t perCentInc = 100.0 / (Double_t)nSteps;
	Double_t perCent    = perCentInc;
	Double_t stepInc    = totEnt / (Double_t)nSteps;
	Double_t step       = stepInc;
	cout<<"ok"<<endl;
	// variable check_triggers stores requested triggers in "binary mode"
	check_triggers = 0;
	if (fTrgArraySet==kTRUE) { 
		for(Int_t i=0; i<fTrgArraySize;i++) {
			//cout << id << " ";
			if     ( fTrgArray[i]==500206 ) check_triggers += pow(2, 0); // BHT0*VPDMB-5i
			//else if( fTrgArray[i]==480202 ) check_triggers += pow(2, 1); // BHT1*VPDMB-30
			// if     ( fTrgArray[i]==500904 ) check_triggers += pow(2, 0); // BHT2
			//else if( fTrgArray[i]==500215 ) check_triggers += pow(2, 1);     //
		}
		//if (check_triggers == 0) return false;
	} else {
		cout << "Trigger not setup, taking all events." << endl;
	}

	 cout<<"ok"<<endl;


	//loop over events
	int run_id = -9;
	int prev_run_id = -9;
	for(Int_t i=frstEnt; i<lastEnt; i++) {
		if(procEnt>=step) {
			cout << perCent << "\t" << procEnt << "/" << totEnt << "\t" << currEnt << "/" << allEnt << endl;
			perCent += perCentInc;
			step += stepInc;
		}
		//cout << i << endl;
		fInputDataset->LoadTree(i);
		fInputDataset->GetEntry(i);
		procEnt++;
		currEnt++;
		run_id = event->GetRunID();
		if (prev_run_id != run_id) {
			prev_run_id = run_id;
			if (f_hot_towers) {
				h_hot_towers = (TH1I*)f_hot_towers->Get(Form("hot_tow_av10_run_%d", run_id));
				cout << "run_id: " << run_id << endl;
				cout << "h_hot_towers->GetEntries(): " << h_hot_towers->GetEntries() << endl;
			}
		}
		ProcessEvent(event);
		event->Reset();

	}//for(fNEvents)
}//Loop()

//_______________________________________________________________________________________________
void StWiciED0CorrLooper::Finalize() {
	cout << "StWiciED0CorrLooper::Finalize: Saving events QA" << endl;
	output_file->cd();
	fMagFiledQA->Write();
	fVertexQA->Write();
	fDca->Write();
	h_VertexXY->Write();
	h_DcaXY->Write();
	h_DcaZ->Write();
	h_Dca_my->Write();
	fVpdVertexQA->Write();
	fTPCVpdVertexZ->Write();
	fRefMultQA->Write();
	h_run_refmult->Write();
	hTofMult_RefMult->Write();

	fKaonSigmaTof->Write();
	fKaonSigmaTof_afterDedx->Write();
	fKaonSigmaTofCut->Write();
	fPionSigmaTof->Write();
	fPionSigmaTofCut->Write();

	hKaonSigmaDedx->Write();
	hPionSigmaDedx->Write();

	h_inv_mass_US->Write();
	h_inv_mass_LS->Write();
	h_inv_mass_LS_neg->Write();
	h_inv_mass_LS_pos->Write();
	h_inv_mass_ROT->Write();
	h_inv_mass_MIX->Write();

	h_inv_mass_DS_plus_vs_pt->Write();
	h_inv_mass_DS_minus_vs_pt->Write();

	h_inv_mass_US_Kplus->Write();
	h_inv_mass_US_Kminus->Write();
	h_inv_mass_US_Kplus_vs_pt->Write();
	h_inv_mass_US_Kplus_vs_pt_kaon->Write();
	h_inv_mass_US_Kplus_vs_pt_pion->Write();
	h_inv_mass_US_Kminus_vs_pt->Write();
	h_inv_mass_US_Kminus_vs_pt_kaon->Write();
	h_inv_mass_US_Kminus_vs_pt_pion->Write();

	h_inv_mass_LS_Kplus->Write();
	h_inv_mass_LS_Kminus->Write();
	h_inv_mass_LS_Kplus_vs_pt->Write();
	h_inv_mass_LS_Kplus_vs_pt_kaon->Write();
	h_inv_mass_LS_Kplus_vs_pt_pion->Write();
	h_inv_mass_LS_Kminus_vs_pt->Write();
	h_inv_mass_LS_Kminus_vs_pt_kaon->Write();
	h_inv_mass_LS_Kminus_vs_pt_pion->Write();

	h_inv_mass_ROT_Kplus->Write();
	h_inv_mass_ROT_Kminus->Write();
	h_inv_mass_ROT_Kplus_vs_pt->Write();
	h_inv_mass_ROT_Kplus_vs_pt_kaon->Write();
	h_inv_mass_ROT_Kplus_vs_pt_pion->Write();
	h_inv_mass_ROT_Kminus_vs_pt->Write();
	h_inv_mass_ROT_Kminus_vs_pt_kaon->Write();
	h_inv_mass_ROT_Kminus_vs_pt_pion->Write();

	h_inv_mass_MIX_Kplus->Write();
	h_inv_mass_MIX_Kminus->Write();
	h_inv_mass_MIX_Kplus_vs_pt->Write();
	h_inv_mass_MIX_Kplus_vs_pt_kaon->Write();
	h_inv_mass_MIX_Kplus_vs_pt_pion->Write();
	h_inv_mass_MIX_Kminus_vs_pt->Write();
	h_inv_mass_MIX_Kminus_vs_pt_kaon->Write();
	h_inv_mass_MIX_Kminus_vs_pt_pion->Write();

	h_pion_N->Write(); 
	h_pion_N_neg->Write(); 
	h_pion_N_pos->Write(); 
	h_pion_pt->Write();
	h_pion_pt_neg->Write();
	h_pion_pt_pos->Write();
	h_pion_phi_neg->Write();
	h_pion_phi_pos->Write();
	h_pion_pt_US->Write();
	h_pion_pt_LS->Write();
	h_pion_pt_MIX->Write();

	h_pt_US->Write();
	h_pt_LS->Write();
	h_pt_MIX->Write();
	h_kaon_N->Write(); 
	h_kaon_N_neg->Write(); 
	h_kaon_N_pos->Write(); 
	h_kaon_pt->Write();
	h_kaon_pt_neg->Write();
	h_kaon_pt_pos->Write();
	h_kaon_pt_US->Write();
	h_kaon_pt_LS->Write();
	h_kaon_pt_MIX->Write();
	h_kaon_neg_eta_pt->Write();
	h_kaon_pos_eta_pt->Write();
	h_kaon_neg_eta->Write();
	h_kaon_pos_eta->Write();
	h_kaon_neg_phi->Write();
	h_kaon_pos_phi->Write();
	h_kaon_neg_pt->Write();
	h_kaon_pos_pt->Write();
	h_kaon_pt_vs_eta->Write();
	h_kaon_pt_vs_dedx->Write();
	h_kaon_pt_vs_oneOverBetaTof->Write();
	h_kaon_p_vs_dedx->Write();
	h_kaon_p_vs_oneOverBetaTof->Write();
	h_kaon_pt_vs_sigmaDedx->Write();
	h_kaon_pt_vs_sigmaTof->Write();
	h_kaon_p_vs_sigmaDedx->Write();
	h_kaon_p_vs_sigmaTof->Write();

	h_kaon_p_vs_dedx_phimeson->Write();
	h_kaon_p_vs_oneOverBetaTof_phimeson->Write();
	h_kaon_p_vs_sigmaDedx_phimeson->Write();
	h_kaon_p_vs_sigmaTof_phimeson->Write();

	h_pion_pt_vs_eta->Write();
	h_pion_pt_vs_dedx->Write();
	h_pion_pt_vs_oneOverBetaTof->Write();
	h_pion_p_vs_dedx->Write();
	h_pion_p_vs_oneOverBetaTof->Write();
	h_pion_pt_vs_sigmaDedx->Write();
	h_pion_pt_vs_sigmaTof->Write();
	h_pion_p_vs_sigmaDedx->Write();
	h_pion_p_vs_sigmaTof->Write();

	h_all_pt_vs_dedx->Write();
	h_all_pt_vs_oneOverBetaTof->Write();

	/*
	h_K0s_inv_mass_vs_pt->Write();
	h_K0s_phi->Write();
	h_K0s_eta->Write();
	h_K0sBckg_inv_mass_vs_pt->Write();
	*/

	h_Eta_c_inv_mass_vs_pt->Write();
	h_Eta_c_inv_mass_vs_eta->Write();
	h_Eta_c_phi->Write();
	h_Eta_c_eta->Write();
	h_Eta_c_inv_mass->Write();

	h_Eta_c_angle->Write();
	h_Eta_c_cos_theta_star_pos->Write();
	h_Eta_c_cos_theta_star_neg->Write();

	h_KK_inv_mass_vs_pt->Write();
	h_KK_inv_mass_vs_pt_2gev_us->Write();
	h_KK_inv_mass_vs_pt_2gev_lsp->Write();
	h_KK_inv_mass_vs_pt_2gev_lsm->Write();
	h_KK_phi->Write();
	h_KK_eta->Write();
	h_KK_pt->Write();
	h_KK_inv_mass->Write();
	h_KK_plus_inv_mass->Write();
	h_KK_minus_inv_mass->Write();

	h_KK_angle->Write();
	h_KK_cos_theta_star_pos->Write();
	h_KK_cos_theta_star_neg->Write();

	h_Dstar_RS_vs_pt->Write();
	h_Dstar_WS_vs_pt->Write();
	h_Dstar_Sideband_vs_pt->Write();
	h_Dstar_RS_vs_pt_softtest->Write();
	h_Dstar_WS_vs_pt_softtest->Write();
	h_Dstar_Sideband_vs_pt_softtest->Write();
	h_Dstar_RS_pt_vs_soft_pion_pt->Write();
	h_Dstar_WS_pt_vs_soft_pion_pt->Write();
	h_pt_D0_vs_kaon_cos_theta_star->Write();

	h_track_statistics->Write();
	h_N_cuts->Write();

	h_multi_trigg->Write();
	h_multi_trigg_2D->Write();

	h_BTofYLocal->Write();
	h_BTofYLocal_kaon->Write();
	h_BTofYLocal_pion->Write();

	fADCvsP->Write();
	fADCvsPcut->Write();
	fADCvsPt->Write();
	fADCvsPtCut->Write();
	fEnergyVsP->Write();
	fEnergyVsPcut->Write();
	fEnergyVsPt->Write();
	fEnergyVsPtCut->Write();

	h_zdc_pionN->Write();
	h_zdc_pionN_dedx->Write();
	h_zdc_refmult->Write();
	h_bbc_pionN->Write();
	h_bbc_kaonN->Write();
	h_bbc_pionN_dedx->Write();
	h_bbc_refmult->Write();

	for (int i = 0; i < 6; i++) {
		h_track_statistics_pt[i]->Write();
	}

	cout << "StWiciED0CorrLooper::Finalize: Writing and closing file" << endl;
	t_output->Write();
	output_file->Write();
	output_file->Close();
	//f_hot_towers->Close();
} //Finalize()

Bool_t StWiciED0CorrLooper::CheckTrigger(const vector<unsigned int>& trgs) {
	for(Int_t i=0; i<fTrgArraySize;i++) {
		for(std::vector<unsigned int>::size_type it=0; it!=trgs.size(); it++) {
			if(trgs[it]==fTrgArray[i]) return kTRUE;
		}
	}
	return kFALSE;
}

Bool_t StWiciED0CorrLooper::CheckBinaryTrigger(const unsigned int trgs) {
	bool b1 = false, b2 = false;
	if (trgs & t32) {
		b1 = true;
	}
	if (trgs & t470202) {
		b2 = true;
	}
	if (b1 && b2) {
		h_multi_trigg->Fill(1);
	}
	if (b1 && !b2) {
		h_multi_trigg->Fill(2);
	}
	if (!b1 && b2) {
		h_multi_trigg->Fill(3);
	}
	if (!b1 && !b2) {
		h_multi_trigg->Fill(4);
	}

	int n_trig = 6;
	unsigned int i_trig;
	unsigned int j_trig;
	for (int i=0; i < n_trig; i++) { 
		i_trig = pow(2, i);
		for (int j=i; j < n_trig; j++) { 
			j_trig = pow(2, j);
			if ((trgs & i_trig) && (trgs & j_trig)) { 
				h_multi_trigg_2D->Fill(i,j);
			}
			/*
			   if     ( fTrgArray[i]==32 ) check_triggers += pow(2, 0); // BHT1*VPDMB-30
			   else if( fTrgArray[i]==470202 ) check_triggers += pow(2, 1); // BHT1*VPDMB-30
			   else if( fTrgArray[i]==470904 ) check_triggers += pow(2, 2); //
			   else if( fTrgArray[i]==470206 ) check_triggers += pow(2, 3); //
			   else if( fTrgArray[i]==470001 ) check_triggers += pow(2, 4); //
			   else if( fTrgArray[i]==27 ) check_triggers += pow(2, 5);     //
			   */
		}
	}

	if (check_triggers & trgs) {
		//cout << "accepted trgigger" << endl;
		return true;
	}

	return kFALSE;
}

//_______________________________________________________________________________________________
void StWiciED0CorrLooper::ProcessEvent(WiciEvent* event, const Bool_t mixed) {
	//event cuts
	//CheckTrigger(event->GetTriggers());
	//if(fTrgArraySet==kTRUE && CheckTrigger(event->GetTriggers())==kFALSE) return;


	h_N_cuts->Fill(1);
	RunId = event->GetRunID();
	/*
	if (f_hot_towers) {
		h_hot_towers = (TH1I*)f_hot_towers->Get(Form("hot_tow_av10_run_%d", RunId));
		cout << "RunId: " << RunId << endl;
		cout << "h_hot_towers->GetEntries(): " << h_hot_towers->GetEntries() << endl;
	}
	*/
	int n_bad_run_list = bad_run_list.size();
	for (int i = 0; i < n_bad_run_list; i++) {
		if (RunId == bad_run_list.at(i)) {
			//cout << "Run on bad run list. " << endl;
			return;
		}
	}

	//h_N_cuts->Fill(2);
	if(fTrgArraySet==kTRUE && CheckBinaryTrigger(event->GetBinaryTriggers())==kFALSE) return;
	h_N_cuts->Fill(2);

	// -------------------------------
	//return;

	fVertexQA->Fill(event->GetVertex().GetZ());
	fVpdVertexQA->Fill(event->GetVpdVtxZ());
	h_VertexXY->Fill(event->GetVertex().GetX(), event->GetVertex().GetY());
	if(fAbsVtxPosMaxSet==kTRUE && TMath::Abs(event->GetVertex().GetZ())>fAbsVtxPosMax) return;
	h_N_cuts->Fill(3);
	//if(TMath::Abs(event->GetVertex().GetZ())<20 || TMath::Abs(event->GetVertex().GetZ())>40) return;

	//if(TMath::Abs(event->GetVertex().GetZ() - event->GetVpdVtxZ())>6) return;
	//if(event->GetVertex().GetRank() < 0.) return;

	if (fRefMultMinSet == kTRUE && event->GetRefMult() < fRefMultMin ) return;
	if (fRefMultMaxSet == kTRUE && event->GetRefMult() > fRefMultMax ) return;

	/*
	int run = RunId - 13100000;
	h_run_refmult->Fill(run, RefMult);
	*/

	RefMult = event->GetRefMult();
	hTofMult_RefMult->Fill(event->GetRefMult(), event->GetTofMult());
	//if (!((event->GetTofMult() < 95 + 5.3*event->GetRefMult()) && (event->GetTofMult() > -65 + 2.8*event->GetRefMult()))) return;
	//hTofMult_RefMult->Fill(event->GetRefMult(), event->GetTofMult());
	//h_N_cuts->Fill(5);

	fRefMultQA->Fill(event->GetRefMult());
	EventId = event->GetEventID();
	//cout << RefMult << " " << RunId << " " << EventId << endl;
	//fTPCVpdVertexZ->Fill(TMath::Abs(event->GetVertex().GetZ() - event->GetVpdVtxZ()));
	if (fTPCVpdVtxZSet==kTRUE && TMath::Abs(event->GetVertex().GetZ() - event->GetVpdVtxZ()) > fTPCVpdVtxZ) return;
	fTPCVpdVertexZ->Fill(event->GetVertex().GetZ() - event->GetVpdVtxZ());
	//h_N_cuts->Fill(6);
	h_N_cuts->Fill(4);
	fMagFiledQA->Fill(event->GetMagField());

	//cout << "before" << endl;
	//TClonesArray* posParts;
	//posParts = (TClonesArray*)event->GetPosParticles();
	//posParts->Clear();
	//posParts = (TClonesArray*)event->GetPosParticles();
	TClonesArray* posParts = event->GetPosParticles();
	TClonesArray* negParts = event->GetNegParticles();
	Int_t nPosParts = posParts->GetEntries();
	//Int_t nNegParts = negParts->GetEntries();
	Int_t nAllParts = event->GetNParticles();
	//cout << "pos, neg, all: " << nPosParts << " " << nNegParts << " " << nAllParts << " " << nPosParts+nNegParts << endl; 

	int N_kaons = 0, N_pions = 0;
	int N_kaons_neg = 0, N_pions_neg = 0;
	int N_kaons_pos = 0, N_pions_pos = 0;
	//WiciLepton* electron = NULL;
	//vector<StLorentzVectorD> v_kaons, v_K0s, v_kaons_pos, v_kaons_neg, v_pions_pos, v_pions_neg, v_pions_ht_neg, v_pions_ht_pos;
	v_pions_pos_overlap.clear();
	v_pions_neg_overlap.clear();
	v_kaons_pos.clear();
	v_kaons_neg.clear();
	v_pions_pos.clear();
	v_pions_neg.clear();
	v_pions_ht_neg.clear();
	v_pions_ht_pos.clear();
	v_kaons_neg_sigmaDedx.clear();
	v_kaons_neg_sigmaTof.clear();
	v_kaons_pos_sigmaDedx.clear();
	v_kaons_pos_sigmaTof.clear();
	//vector<StLorentzVectorD> v_kaons, v_K0s;
	//loop over charged particles
	for(Int_t p1=0; p1<nAllParts; p1++) {
		/*
		cout << "particle "<< p1 << endl;
		if (p1 > 3) break;
		*/
		/*
		if (p1<nPosParts) {
			posParts->Clear();
		} else {
			negParts->Clear();
		}
		*/
		WiciParticle* fstPart = (WiciParticle*) (p1<nPosParts ? posParts->UncheckedAt(p1) : negParts->UncheckedAt(p1-nPosParts));
		
		//UShort_t nfitpoint = fstPart->GetNFitPoints();
		if (fstPart->GetNFitPoints() < 15) continue;
		if(TMath::Abs(GetParticleMomentum(fstPart).Eta()) > 1.0) continue;

		Int_t       fstTowADC = fstPart->GetBemcMatchTowerEnergy()>fstPart->GetBemcHighTowerEnergy() ? fstPart->GetBemcMatchTowerADC() : fstPart->GetBemcHighTowerADC();
		Double_t fstTowEnergy = fstPart->GetBemcMatchTowerEnergy()>fstPart->GetBemcHighTowerEnergy() ? fstPart->GetBemcMatchTowerEnergy() : fstPart->GetBemcHighTowerEnergy();
		Bool_t    matchHigher = fstPart->GetBemcMatchTowerEnergy()>fstPart->GetBemcHighTowerEnergy();
		//filling histos for high tower analysis
		if(fUseHighTower==kTRUE) {
			fADCvsP->Fill( GetParticleMomentum(fstPart).Mag(), matchHigher ? fstPart->GetBemcMatchTowerADC() : fstPart->GetBemcHighTowerADC() );
			fADCvsPt->Fill( GetParticleMomentum(fstPart).Pt(), matchHigher ? fstPart->GetBemcMatchTowerADC() : fstPart->GetBemcHighTowerADC() );
			fEnergyVsP->Fill( GetParticleMomentum(fstPart).Mag(), matchHigher ? fstPart->GetBemcMatchTowerEnergy() : fstPart->GetBemcHighTowerEnergy() );
			fEnergyVsPt->Fill( GetParticleMomentum(fstPart).Pt(), matchHigher ? fstPart->GetBemcMatchTowerEnergy() : fstPart->GetBemcHighTowerEnergy() );
			if(fstTowADC>fEmcTreshold && fstTowEnergy<2*GetParticleMomentum(fstPart).Mag()) {
				fADCvsPcut->Fill( GetParticleMomentum(fstPart).Mag(), matchHigher ? fstPart->GetBemcMatchTowerADC() : fstPart->GetBemcHighTowerADC() );
				fADCvsPtCut->Fill( GetParticleMomentum(fstPart).Pt(), matchHigher ? fstPart->GetBemcMatchTowerADC() : fstPart->GetBemcHighTowerADC() );
				fEnergyVsPcut->Fill( GetParticleMomentum(fstPart).Mag(), matchHigher ? fstPart->GetBemcMatchTowerEnergy() : fstPart->GetBemcHighTowerEnergy() );
				fEnergyVsPtCut->Fill( GetParticleMomentum(fstPart).Pt(), matchHigher ? fstPart->GetBemcMatchTowerEnergy() : fstPart->GetBemcHighTowerEnergy() );
				//fParticleDedx->Fill(GetParticleMomentum(fstPart).Pt(), TMath::Log(fstPart->GetDedx()) );
			}
		}//if(fUseHighTower)

		/*
		h_BTofYLocal->Fill(fstPart->GetBTofYLocal());
		if (TMath::Abs(fstPart->GetBTofYLocal()) > 1.8) continue;
		*/
		//if(fstPart->GetTofMatchFlag()<0) continue;
		if(fPtMaxSet==kTRUE && GetParticleMomentum(fstPart).Pt()>fPtMax) continue;

		/*
		cout << "particle i : " << p1 << endl;
		cout << "fstPart->Get_fDca(): " << fstPart->Get_fDca() << endl;
		cout << "Dca X Y Z: " << fstPart->GetDcaX() << " " << fstPart->GetDcaY() << " " << fstPart->GetDcaZ() << endl;
		cout << "fstPart->GetDca(): " << fstPart->GetDca() << endl;
		float my_dca = TMath::Sqrt(fstPart->GetDcaX()*fstPart->GetDcaX() + fstPart->GetDcaY()*fstPart->GetDcaY() + fstPart->GetDcaZ()*fstPart->GetDcaZ());
		cout << "my Dca: " << my_dca << endl;
		cout << "fstPart->GetDca_new(): " << fstPart->GetDca_new() << endl;
		cout << "again fstPart->GetDca(): " << fstPart->GetDca() << endl;
		cout << "again fstPart->Get_fDca(): " << fstPart->Get_fDca() << endl;
		cout << "fNPointsRatio: " << fstPart->GetPointsRatio_new() << endl;
		float ppp = fstPart->GetNMaxPoints()!=0 ? (Float_t)fstPart->GetNFitPoints()/(Float_t)fstPart->GetNMaxPoints() : -8888;
		cout << "Pocitam: " << fstPart->GetNFitPoints() << " " << fstPart->GetNMaxPoints() << " " << ppp << endl;
		cout << "GetPointsRatio(): " << fstPart->GetPointsRatio() << endl;
		cout << endl;
		*/
		//h_Dca_my->Fill(my_dca);
		//fstPart->Delete();
		//cout << p1 << " " << nPosParts << " " << nAllParts << endl;
		//fstPart = NULL;
		//continue;
		if(fDcaMaxSet==kTRUE && fstPart->GetDca()>fDcaMax) continue;
		fDca->Fill(fstPart->GetDca());
		h_DcaXY->Fill(fstPart->GetDcaX(), fstPart->GetDcaY());
		h_DcaZ->Fill(fstPart->GetDcaZ());

		h_all_pt_vs_dedx->Fill(GetParticleMomentum(fstPart).Pt(), fstPart->GetDedx());

		/*
		if (fstPart->GetBetaTof()>0.) {
			if(fstPart->GetTofMatchFlag()==0) {
				cout << "BTOF xxx" << endl;
			}
		}

		if(fstPart->GetTofMatchFlag()==0) {
			if (fstPart->GetBetaTof()>0.) {
				cout << "BTOF yyy" << endl;
			}
		}
		*/

		//if (fUseHighTower==false && fstPart->GetBetaTof()>0.) {
		if (fstPart->GetBetaTof()>0.) {
			h_all_pt_vs_oneOverBetaTof->Fill(GetParticleMomentum(fstPart).Pt(), 1.0/fstPart->GetBetaTof());
			h_BTofYLocal->Fill(fstPart->GetBTofYLocal());
			//if (TMath::Abs(fstPart->GetBTofYLocal()) > 1.8) continue;
		}

		if (fMatchToFastDet==kTRUE) {
			if ( fstPart->GetTofMatchFlag()<=0 && fstPart->GetBemcMatchFlag()==kFALSE) {
				cout << "not fast matching" << endl;
				continue;
			} 
		}

		double pt_track = GetParticleMomentum(fstPart).Pt();
		double p_track = GetParticleMomentum(fstPart).Mag();
		bool kaon_OK = true;
		double nsigtofk = GetSigmaTof(fstPart, KAON);
		if(fPtKaonMinSet==kTRUE && GetParticleMomentum(fstPart).Pt()<fPtKaonMin) kaon_OK = false;
		if(TMath::Abs(GetParticleMomentum(fstPart).Eta()) > fEtaKaonMax) kaon_OK = false;
		hKaonSigmaDedx->Fill(GetParticleMomentum(fstPart).Pt(), fstPart->GetSigmaDedxKaon());
		if (f_cond == -1) {
			if (kaon_OK == true) {
				Double_t Res = 8.83957e-01+1.74093e-02/TMath::Power(GetParticleMomentum(fstPart).Mag()+8.38958e-02,4.23066e+00);
				Double_t Pos = 3.16018e-02+1.36866e-03/TMath::Power(GetParticleMomentum(fstPart).Mag()+1.01344e-01,6.89333e+00);
				Double_t sigmaTofMin = Pos-2*Res;
				Double_t sigmaTofMax = Pos+2*Res;
				/*
				 * commented 28oct
				 if (GetParticleMomentum(fstPart).Pt() > 1.3) {
				 sigmaTofMin = -2.4 + GetParticleMomentum(fstPart).Mag() * 0.6;
				 sigmaTofMax =  2.4 - GetParticleMomentum(fstPart).Mag() * 0.6;
				 }
				 */
				if(IsKaon(fstPart, sigmaTofMin, sigmaTofMax)==kFALSE) kaon_OK = false;
			}	
		} else if (f_cond == 1) {
			if (kaon_OK == true) {
				if (fstPart->GetBetaTof()>0.) {
					fKaonSigmaTof->Fill(GetParticleMomentum(fstPart).Mag(), nsigtofk);
				}
				if(fabs(fstPart->GetSigmaDedxKaon())<2.) {
					if (fstPart->GetBetaTof()>0.) {
						fKaonSigmaTof_afterDedx->Fill(GetParticleMomentum(fstPart).Mag(), nsigtofk);
					}
					//if (pt_track < 1.6) {
					if (pt_track < 1.3) {
						if (fstPart->GetBetaTof()>0.) {
							if (TMath::Abs(fstPart->GetBTofYLocal()) > 1.8) continue; //kontrola hitu TOF
							//if (TMath::Abs(nsigtofk) > 2) 
							//if (nsigtofk > 5-1.875*pt_track || nsigtofk < -5+1.875*pt_track) {
							if (nsigtofk > 5-2.308*pt_track || nsigtofk < -5+2.308*pt_track) {
								kaon_OK = false;
							}
						} else {
							kaon_OK = false;
						}
					} else if (fstPart->GetBetaTof()>0. && TMath::Abs(nsigtofk) > 2.) {
						kaon_OK = false;
					}
				} else {
					kaon_OK = false;
				}
			}
		}

		bool pion_OK = true;
		double nsigtofpi = GetSigmaTof(fstPart, PION);
		//if(fPtPionMinSet==kTRUE && GetParticleMomentum(fstPart).Pt()<fPtPionMin) pion_OK = false;
		if(fPtSoftPionMinSet==kTRUE && GetParticleMomentum(fstPart).Pt() < fPtSoftPionMin) pion_OK = false;
		if (fstPart->GetBetaTof()>0.) {
			fPionSigmaTof->Fill(GetParticleMomentum(fstPart).Mag(), nsigtofpi);
		}
		hPionSigmaDedx->Fill(GetParticleMomentum(fstPart).Pt(), fstPart->GetSigmaDedxPion());
		if (f_cond == -1) {
			{
				if(fSigBtofPionMaxSet==kTRUE && fstPart->GetBetaTof()>0 && TMath::Abs(GetSigmaTof(fstPart, PION))>fSigBtofPionMax) pion_OK = false;
				if(fSigDedxPionMaxSet==kTRUE && TMath::Abs(fstPart->GetSigmaDedxPion())>fSigDedxPionMax) pion_OK = false;

			}
		} else if (f_cond == 1) {
			if (pion_OK == true) {
				if (fabs(fstPart->GetSigmaDedxPion())<2.) {
					if (pt_track < 1.6) {
						if (fstPart->GetBetaTof() > 0.) {
							if (TMath::Abs(fstPart->GetBTofYLocal()) > 1.8) continue;
							if (nsigtofpi > 5-1.875*pt_track || nsigtofpi < -5+1.875*pt_track) {
								pion_OK = false;
							}
						} else {
							pion_OK = false;
						}
					} else if (fstPart->GetBetaTof() > 0. && TMath::Abs(nsigtofpi) > 2.) {
						pion_OK = false;
					}
				} else {
					pion_OK = false;
				}
			}
		}


		bool proton_OK = true;
		double fSigBtofProtonMax = 2.0;
		double fSigDedxProtonMax = 2.0;
		if(fstPart->GetBetaTof()>0 && TMath::Abs(fstPart->GetSigmaTofProton())>fSigBtofProtonMax) proton_OK = false;
		if(TMath::Abs(fstPart->GetSigmaDedxProton())>fSigDedxProtonMax) proton_OK = false;

		//track_statistics_pt(pion_OK, kaon_OK, proton_OK, GetParticleMomentum(fstPart).Pt());

		h_track_statistics->Fill(0); //0 - all tracks

		/*
		   if (pion_OK  == true && kaon_OK == true && proton_OK == true) {
		//N_track_kaon_pion_together++;
		h_track_statistics->Fill(7); //7 - kaon & pion & proton
		continue;
		}

		if (pion_OK  == true && kaon_OK == true && proton_OK == false) {
		//N_track_kaon_pion_together++;
		h_track_statistics->Fill(4); // 1 - kaon, 2 - pion, 3 - proton, 4 - kaon & pion, 5 - kaon & proton, 6 - pion & proton
		continue;
		}

		if (kaon_OK == true && proton_OK == true && pion_OK  == false) {
		h_track_statistics->Fill(5); // 1 - kaon, 2 - pion, 3 - proton, 4 - kaon & pion, 5 - kaon & proton, 6 - pion & proton
		continue;
		}
		if (pion_OK  == true && proton_OK == true && kaon_OK == false) {
		h_track_statistics->Fill(6); // 1 - kaon, 2 - pion, 3 - proton, 4 - kaon & pion, 5 - kaon & proton, 6 - pion & proton
		continue;
		}

		if (proton_OK == true && pion_OK == false && kaon_OK == false) {
		h_track_statistics->Fill(3); // 1 - kaon, 2 - pion, 3 - proton, 4 - kaon & pion, 5 - kaon & proton, 6 - pion & proton
		continue;
		}

		if (proton_OK == false && pion_OK == false && kaon_OK == false) {
		h_track_statistics->Fill(8); // 1 - kaon, 2 - pion, 3 - proton, 4 - kaon & pion, 5 - kaon & proton, 6 - pion & proton
		continue;
		}
		*/

		if (pion_OK == false && kaon_OK == false) continue;
		//if (pion_OK == true  && kaon_OK == true) continue;
		if (pion_OK == true  && kaon_OK == true) {
			//pion_OK = false;
			//kaon_OK = false;
			//continue;
		}

		if (kaon_OK == true) {
			h_track_statistics->Fill(2); // 1 - kaon, 2 - pion, 3 - proton, 4 - kaon & pion, 5 - kaon & proton, 6 - pion & proton
			N_kaons++;
			//h_kaon_pt->Fill(GetParticleMomentum(fstPart).Pt());
			h_kaon_pt_vs_eta->Fill(GetParticleMomentum(fstPart).Pt(), GetParticleMomentum(fstPart).Eta());
			h_kaon_pt_vs_dedx->Fill(GetParticleMomentum(fstPart).Pt(), fstPart->GetDedx());
			h_kaon_p_vs_dedx->Fill(GetParticleMomentum(fstPart).Mag(), fstPart->GetDedx());
			h_kaon_pt_vs_sigmaDedx->Fill(GetParticleMomentum(fstPart).Pt(), fstPart->GetSigmaDedxKaon());
			h_kaon_p_vs_sigmaDedx->Fill(GetParticleMomentum(fstPart).Mag(), fstPart->GetSigmaDedxKaon());
			if(fstPart->GetBetaTof()>0) {
				h_kaon_pt_vs_oneOverBetaTof->Fill(GetParticleMomentum(fstPart).Pt(), 1.0/fstPart->GetBetaTof());
				h_kaon_p_vs_oneOverBetaTof->Fill(GetParticleMomentum(fstPart).Mag(), 1.0/fstPart->GetBetaTof());
				h_kaon_p_vs_sigmaTof->Fill(GetParticleMomentum(fstPart).Mag(), GetSigmaTof(fstPart, KAON));
				h_kaon_pt_vs_sigmaTof->Fill(GetParticleMomentum(fstPart).Pt(), GetSigmaTof(fstPart, KAON));
				fKaonSigmaTofCut->Fill(GetParticleMomentum(fstPart).Mag(), nsigtofk);
				h_BTofYLocal_kaon->Fill(fstPart->GetBTofYLocal());
			}

			StThreeVectorD kaonMomAtDca = StThreeVectorD(GetParticleMomentum(fstPart).X(), GetParticleMomentum(fstPart).Y(), GetParticleMomentum(fstPart).Z());
			StLorentzVectorD kaon4P(kaonMomAtDca, kaonMomAtDca.massHypothesis(kPdgMass[KAON]));
			if (fstPart->GetCharge() < 0) {
				N_kaons_neg++;
				v_kaons_neg.push_back(kaon4P);
				v_kaons_neg_sigmaDedx.push_back(fstPart->GetSigmaDedxKaon());
				if(fstPart->GetTofMatchFlag() > 0) {
					v_kaons_neg_sigmaTof.push_back(GetSigmaTof(fstPart, KAON));
				} else {
					v_kaons_neg_sigmaTof.push_back(-100.);
				}
				//h_kaon_pt_neg->Fill(GetParticleMomentum(fstPart).Pt());
			} else {
				N_kaons_pos++;
				v_kaons_pos.push_back(kaon4P);
				v_kaons_pos_sigmaDedx.push_back(fstPart->GetSigmaDedxKaon());
				if(fstPart->GetTofMatchFlag() > 0) {
					v_kaons_pos_sigmaTof.push_back(GetSigmaTof(fstPart, KAON));
				} else {
					v_kaons_pos_sigmaTof.push_back(-100.);
				}
				//h_kaon_pt_pos->Fill(GetParticleMomentum(fstPart).Pt());
			}
		}


		if (pion_OK == true) {
			StThreeVectorD pionMomAtDca = StThreeVectorD(GetParticleMomentum(fstPart).X(), GetParticleMomentum(fstPart).Y(), GetParticleMomentum(fstPart).Z());
			StLorentzVectorD pion4P(pionMomAtDca, pionMomAtDca.massHypothesis(kPdgMass[PION]));

			Int_t    sndTowADC    = fstPart->GetBemcMatchTowerEnergy()>fstPart->GetBemcHighTowerEnergy() ? fstPart->GetBemcMatchTowerADC() : fstPart->GetBemcHighTowerADC();
			Double_t sndTowEnergy = fstPart->GetBemcMatchTowerEnergy()>fstPart->GetBemcHighTowerEnergy() ? fstPart->GetBemcMatchTowerEnergy() : fstPart->GetBemcHighTowerEnergy();
			if (h_hot_towers) {
				if (h_hot_towers->GetBinContent(fstPart->GetBemcMatchTowerID()) == 1 && h_hot_towers->GetBinContent(fstPart->GetBemcHighTowerID()) == 1) {
					sndTowADC = -9;
					sndTowEnergy = -9.;
				} else {
					if (h_hot_towers->GetBinContent(fstPart->GetBemcMatchTowerID()) == 1) {
						sndTowADC = fstPart->GetBemcHighTowerADC();
						sndTowEnergy = fstPart->GetBemcHighTowerEnergy();
					} else if (h_hot_towers->GetBinContent(fstPart->GetBemcHighTowerID()) == 1) {
						sndTowADC = fstPart->GetBemcMatchTowerADC();
						sndTowEnergy = fstPart->GetBemcMatchTowerEnergy();
					}
				}
			}
			bool pion_ht = false;
			if(fUseHighTower==kTRUE) {
				//if (fstPart->GetBemcMatchTowerID() == 2974 || fstPart->GetBemcHighTowerID() == 2974) continue;
				if (sndTowADC>fEmcTreshold && sndTowEnergy<2*GetParticleMomentum(fstPart).Mag() && GetParticleMomentum(fstPart).Pt()>fTrgPartPtMin) {
					pion_ht = true;
				} 
				/*else {
					continue;
				}
				*/
				//if (sndTowADC>fEmcTreshold && sndTowEnergy<2*GetParticleMomentum(fstPart).Mag() && sndTowEnergy > 2) pion_ht = true;
			} else {
				pion_ht = true;
			}
			if(fPtPionMinSet==kTRUE && GetParticleMomentum(fstPart).Pt()<fPtPionMin) pion_ht = false;
			

			h_track_statistics->Fill(1); // 1 - kaon, 2 - pion, 3 - proton, 4 - kaon & pion, 5 - kaon & proton, 6 - pion & proton
			N_pions++;
			h_pion_pt_vs_eta->Fill(GetParticleMomentum(fstPart).Pt(), GetParticleMomentum(fstPart).Eta());
			h_pion_pt->Fill(GetParticleMomentum(fstPart).Pt());
			h_pion_pt_vs_dedx->Fill(GetParticleMomentum(fstPart).Pt(), fstPart->GetDedx());
			h_pion_pt_vs_oneOverBetaTof->Fill(GetParticleMomentum(fstPart).Pt(), 1.0/fstPart->GetBetaTof());
			h_pion_p_vs_dedx->Fill(GetParticleMomentum(fstPart).Mag(), fstPart->GetDedx());
			h_pion_p_vs_oneOverBetaTof->Fill(GetParticleMomentum(fstPart).Mag(), 1.0/fstPart->GetBetaTof());
			h_pion_pt_vs_sigmaDedx->Fill(GetParticleMomentum(fstPart).Pt(), fstPart->GetSigmaDedxPion());
			h_pion_pt_vs_sigmaTof->Fill(GetParticleMomentum(fstPart).Pt(), GetSigmaTof(fstPart, PION));
			h_pion_p_vs_sigmaDedx->Fill(GetParticleMomentum(fstPart).Mag(), fstPart->GetSigmaDedxPion());
			h_pion_p_vs_sigmaTof->Fill(GetParticleMomentum(fstPart).Mag(), GetSigmaTof(fstPart, PION));
			if (fstPart->GetBetaTof()>0.) {
				fPionSigmaTofCut->Fill(GetParticleMomentum(fstPart).Mag(), nsigtofpi);
				h_BTofYLocal_pion->Fill(fstPart->GetBTofYLocal());
			}

			if (fstPart->GetCharge() < 0) {
				v_pions_neg.push_back(pion4P);
				h_pion_pt_neg->Fill(GetParticleMomentum(fstPart).Pt());
				h_pion_phi_neg->Fill(pion4P.phi());
				N_pions_neg++;
				if (pion_ht == true) {
					v_pions_ht_neg.push_back(pion4P);
					if (kaon_OK == true) {
						v_pions_neg_overlap.push_back(N_kaons_neg-1);
						//cout << "neg pion overlap with neg kaon #: " << N_kaons_neg-1 << endl;
					} else {
						v_pions_neg_overlap.push_back(-1);
					}

				}
			} else {
				N_pions_pos++;
				v_pions_pos.push_back(pion4P);
				h_pion_pt_pos->Fill(GetParticleMomentum(fstPart).Pt());
				h_pion_phi_pos->Fill(pion4P.phi());
				if (pion_ht == true) {
					v_pions_ht_pos.push_back(pion4P);
					if (kaon_OK == true) {
						v_pions_pos_overlap.push_back(N_kaons_pos-1);
						//cout << "pos pion overlap with pos kaon #: " << N_kaons_pos-1 << endl;
					} else {
						v_pions_pos_overlap.push_back(-1);
					}

				}
			}
		}
	}//for(fstPart)
	h_kaon_N->Fill(N_kaons); 
	h_kaon_N_neg->Fill(N_kaons_neg); 
	h_kaon_N_pos->Fill(N_kaons_pos); 
	h_pion_N->Fill(N_pions); 
	h_pion_N_neg->Fill(N_pions_neg); 
	h_pion_N_pos->Fill(N_pions_pos); 

	int zdc_rate = event->GetZDCRate();
	h_zdc_pionN->Fill(zdc_rate, N_pions);
	//h_zdc_pionN_dedx->Fill(zdc_rate, N_pions);
	h_zdc_refmult->Fill(zdc_rate, RefMult);
	int bbc_rate = event->GetBBCRate();
	h_bbc_pionN->Fill(bbc_rate, N_pions);
	h_bbc_kaonN->Fill(bbc_rate, N_kaons);
	//h_bbc_pionN_dedx->Fill(bbc_rate, N_pions);
	h_bbc_refmult->Fill(bbc_rate, RefMult);

	//cout << "N_kaons: " << N_kaons << " N_pions: " << N_pions << endl;
	//h_kaon_N_neg->Fill(v_kaons_neg.size()); 
	//h_kaon_N_pos->Fill(v_kaons_pos.size()); 
	//t_output->Fill();

	//do_D0(v_kaons_pos, v_kaons_neg, v_pions_pos, v_pions_neg, false);
	do_D0(false);
	if(fUseEventMixer==kTRUE) {
		int vz_bin = Vz_bin(event->GetVertex().GetZ());
		if (vz_bin > 9 || vz_bin < 0) {
			cout << "vz_bin = " << vz_bin << endl;	
			return;	
		}
		int mult_bin = MultRef_bin(RefMult);
		if (mult_bin > 9 || mult_bin < 0) {
			cout << "mult_bin = " << mult_bin << endl;	
			return;
		}
		// fill vector until 10 events, then analys and clear 
		if (n_mix[vz_bin][mult_bin] < 9) {
			v_kaons_pos_mix[vz_bin][mult_bin][n_mix[vz_bin][mult_bin]] = v_kaons_pos;
			v_kaons_neg_mix[vz_bin][mult_bin][n_mix[vz_bin][mult_bin]] = v_kaons_neg;
			//v_pions_pos_mix[vz_bin][mult_bin][n_mix[vz_bin][mult_bin]] = v_pions_pos;
			//v_pions_neg_mix[vz_bin][mult_bin][n_mix[vz_bin][mult_bin]] = v_pions_neg;
			v_pions_pos_mix[vz_bin][mult_bin][n_mix[vz_bin][mult_bin]] = v_pions_ht_pos;
			v_pions_neg_mix[vz_bin][mult_bin][n_mix[vz_bin][mult_bin]] = v_pions_ht_neg;
			n_mix[vz_bin][mult_bin]++;
		} else {
			for (int i_kaon = 0; i_kaon < 10; i_kaon++) {
				for (int i_pion = 0; i_pion < 10; i_pion++) {
					if (i_kaon == i_pion) {
						continue;
					}
					v_kaons_pos = v_kaons_pos_mix[vz_bin][mult_bin][i_kaon];
					v_kaons_neg = v_kaons_neg_mix[vz_bin][mult_bin][i_kaon];
					v_pions_pos = v_pions_pos_mix[vz_bin][mult_bin][i_pion];
					v_pions_neg = v_pions_neg_mix[vz_bin][mult_bin][i_pion];
					//do_D0(v_kaons_pos, v_kaons_neg, v_pions_pos, v_pions_neg, true);
					do_D0(true);
					//do_D0(v_kaons_pos_mix[vz_bin][mult_bin][i_kaon], v_kaons_neg, v_pions_pos, v_pions_neg, true);
				}
			}
			n_mix[vz_bin][mult_bin] = 0;
			/*
			   v_kaons_pos_mix[vz_bin][mult_bin].clear();
			   v_kaons_neg_mix[vz_bin][mult_bin].clear();
			   v_pions_pos_mix[vz_bin][mult_bin].clear();
			   v_pions_neg_mix[vz_bin][mult_bin].clear();
			   */
		}
	}

} //ProcessEvent()

void StWiciED0CorrLooper::track_statistics_pt(bool pion, bool kaon, bool proton, double pt){
	int bin = -1;
	if (pt > 0.0 && pt < 0.4) {
		bin = 0;
	} else if (pt < 1.3) {
		bin = 1;
	} else if (pt < 2.0) {
		bin = 2;
	} else if (pt < 3.0) {
		bin = 3;
	} else if (pt < 5.0) {
		bin = 4;
	} else if (pt < 100.0) {
		bin = 5;
	} 
	if (bin == -1) {
		return;
	}
	//cout << "pt bin: " << bin << " pt: " << pt << endl;

	h_track_statistics_pt[bin]->Fill(0); //0 - all tracks

	if (pion  == true && kaon == true && proton == true) {
		//N_track_kaon_pion_together++;
		h_track_statistics_pt[bin]->Fill(7); //7 - kaon & pion & proton
		return;
	}

	if (pion  == true && kaon == true && proton == false) {
		//N_track_kaon_pion_together++;
		h_track_statistics_pt[bin]->Fill(4); // 4 - kaon & pion
		return;
	}

	if (kaon == true && proton == true && pion  == false) {
		h_track_statistics_pt[bin]->Fill(5); // 5 - kaon & proton
		return;
	}
	if (pion  == true && proton == true && kaon == false) {
		h_track_statistics_pt[bin]->Fill(6); // 6 - pion & proton
		return;
	}

	if (proton == true && pion == false && kaon == false) {
		h_track_statistics_pt[bin]->Fill(3); // 3 - proton
		return;
	}

	if (proton == false && pion == false && kaon == false) {
		h_track_statistics_pt[bin]->Fill(8); // 8 - no pi,K,p
		return;
	}

	if (kaon == true) {
		h_track_statistics_pt[bin]->Fill(2); // 2 - pion
		return;
	}

	if (pion == true) {
		h_track_statistics_pt[bin]->Fill(1); // 1 - kaon
		return;
	}
}

Int_t StWiciED0CorrLooper::Vz_bin(Double_t Vz){
	double a = (fAbsVtxPosMax*2/10.);
	int res = 0;
	double b = a;
	for (int i = 0; i < 10; i++){
		if (Vz < -fAbsVtxPosMax+b) {
			return res;
		} else {
			b+=a;
			res++;
		}
	}
	//cout << Vz << " " << fAbsVtxPosMax << " " << res << endl;
	return res;
}

Int_t StWiciED0CorrLooper::MultRef_bin(Int_t Mult){
	double a = (400. - fRefMultMin)/10.;
	int res = 0;
	double b = a;
	//for (int i = 0; i < 10; i++){
	for (int i = 0; i < 9; i++){
		if (Mult < b+fRefMultMin) {
			//cout << "multult, bin: " << Mult << " " << res << endl;
			return res;
		} else {
			b+=a;
			res++;
		}
	}	
	return res;
}

bool StWiciED0CorrLooper::Soft_pion_pt_test(double soft_pion_pt, double dstar_pt){
	float pt_max = 0.05 + dstar_pt*0.1;
	float pt_min = -0.05 + dstar_pt*0.05;
	if (soft_pion_pt < pt_max && soft_pion_pt > pt_min) {
		return true;
	}
	return false;
}

//void StWiciED0CorrLooper::do_D0(vector<StLorentzVectorD> v_kaons_pos, vector<StLorentzVectorD> v_kaons_neg, vector<StLorentzVectorD> v_pions_pos, vector<StLorentzVectorD> v_pions_neg, bool do_mix = false){
void StWiciED0CorrLooper::do_D0(bool do_mix = false){
	if(do_mix == false) {
		vector<StLorentzVectorD> v_KK;
		vector<int> v_kaon_used_pos;
		inv_mass_us->clear();
		pt_kaon_us->clear();
		pt_pion_us->clear();
		pt_D0_us->clear();
		inv_mass_ls->clear();
		pt_kaon_ls->clear();
		pt_pion_ls->clear();
		pt_D0_ls->clear();
		inv_mass_rot->clear();

		for (unsigned int i = 0; i < v_kaons_neg.size(); i++) {
			StLorentzVectorD kaon4P = v_kaons_neg[i];
			//rotational background
			StThreeVectorD kaon_3P(-(kaon4P.x()), -(kaon4P.y()), -(kaon4P.z()));
			//cout << kaon_3P.phi() << endl;
			StThreeVectorD kaon_3P_rot_phi(kaon4P.x(), kaon4P.y(), kaon4P.z());
			//cout << kaon_3P_rot_phi.phi() << endl;
			kaon_3P_rot_phi.setPhi(-kaon_3P_rot_phi.phi());
			//cout << kaon_3P_rot_phi.phi() << endl;
			StLorentzVectorD kaonRot4P(kaon_3P, kaon_3P.massHypothesis(kPdgMass[KAON]));
			StLorentzVectorD kaonRotPhi4P(kaon_3P_rot_phi, kaon_3P_rot_phi.massHypothesis(kPdgMass[KAON]));
			//cout << kaonRot4P.x() << " " << kaonRot4P.y() << " " << kaonRot4P.z() << endl;
			//cout << kaonRotPhi4P.x() << " " << kaonRotPhi4P.y() << " " << kaonRotPhi4P.z() << endl;
			//
			kaonRot4P = kaonRotPhi4P;
			//cout << kaon_3P.massHypothesis(kPdgMass[KAON]) << " " << kaon4P.e() << " " << kaonRot4P.e() << endl;
			int j_kaon_pt = FindHistoIndex(kaon4P.perp(), kaon_pt_dep, N_KAON_PT_DEP);
			//cout << "original method: " << j_kaon_pt << " new method: " << index << endl;
			if (j_kaon_pt < 1) continue;
			//for (unsigned int j = 0; j < v_pions_pos.size(); j++) {
			for (unsigned int j = 0; j < v_pions_ht_pos.size(); j++) {
				if (v_pions_ht_pos[j].perp() < 0.2) continue;
				StLorentzVectorD pion4P = v_pions_ht_pos[j];
				StLorentzVectorD D0_4P = kaon4P + pion4P;

				//Calculating invariant mass
				double inv_mass = D0_4P.m();

				const StThreeVectorD deonMomAtDca = D0_4P.vect();
				double PT  = deonMomAtDca.perp();

				/*
				int j_d0_pt = FindHistoIndex(D0_4P.perp(), d0_pt_dep, N_D0_PT_DEP);
				if (j_d0_pt < 1) continue;
				*/
				//cout << "original method: " << j_d0_pt << " new method: " << index << endl;

				h_inv_mass_US->Fill(inv_mass);
				/*
				inv_mass_us->push_back(inv_mass);
				pt_kaon_us->push_back(kaon4P.perp());
				pt_pion_us->push_back(pion4P.perp());
				pt_D0_us->push_back(D0_4P.perp());
				h_inv_mass_US_pt[j_kaon_pt-1][j_d0_pt-1]->Fill(inv_mass);
				h_pt_US->Fill(PT);
				h_kaon_pt_US->Fill(kaon4P.perp());
				h_pion_pt_US->Fill(pion4P.perp());
				*/
				h_inv_mass_US_Kminus->Fill(inv_mass);
				h_inv_mass_US_Kminus_vs_pt->Fill(inv_mass, PT);
				/*
				h_inv_mass_US_Kminus_vs_pt_kaon->Fill(inv_mass, kaon4P.perp());
				h_inv_mass_US_Kminus_vs_pt_pion->Fill(inv_mass, pion4P.perp());
				*/
				double m_rot = (kaonRot4P+pion4P).m();
				h_inv_mass_ROT->Fill(m_rot);
				inv_mass_rot->push_back(m_rot);
				h_inv_mass_ROT_Kminus->Fill(m_rot);
				h_inv_mass_ROT_Kminus_vs_pt->Fill(m_rot, PT);
				/*
				h_inv_mass_ROT_Kminus_vs_pt_kaon->Fill(m_rot, kaon4P.perp());
				h_inv_mass_ROT_Kminus_vs_pt_pion->Fill(m_rot, pion4P.perp());
				*/

				//bool Dstar = true;
				if (Dstar == true) {
					//Boosting back the daughters
					StLorentzVectorD kaonMomAtRF = kaon4P.boost(-D0_4P);
					//StLorentzVectorD pionMomAtRF = pion4P.boost(-D0_4P);

					//Calculating ThetaStar angles for the daughters
					double kaon_cos_theta_star = TMath::Cos( deonMomAtDca.angle(kaonMomAtRF.vect()) );
					//h_kaon_cos_theta_star_vs_pt_D0->Fill(kaon_cos_theta_star, PT);

					//if(inv_mass < 1.89 && inv_mass > 1.84) {
					if (inv_mass < 1.90 && inv_mass > 1.82) {
						h_pt_D0_vs_kaon_cos_theta_star->Fill(PT, kaon_cos_theta_star);
						if(fKaonCosThMaxSet==kTRUE && kaon_cos_theta_star > fKaonCosThMax) continue;
						for (unsigned int i_soft = 0; i_soft < v_pions_pos.size(); i_soft++) {
							//if (j == i_soft) continue;
							if (TMath::Abs(v_pions_pos[i_soft].perp() - pion4P.perp()) < 1e-9) continue;

							StLorentzVectorD soft_pion4P = v_pions_pos[i_soft];
							if (fPtSoftPionMin > v_pions_pos[i_soft].perp()) continue;
							StLorentzVectorD Dstar_4P = D0_4P + soft_pion4P;
							double inv_mass_Dstar = Dstar_4P.m();
							//Dstar_RS->Fill(inv_mass_Dstar - inv_mass);
							h_Dstar_RS_vs_pt->Fill(inv_mass_Dstar - inv_mass, Dstar_4P.perp());
							if (Soft_pion_pt_test(soft_pion4P.perp(), Dstar_4P.perp()) == true) {
								h_Dstar_RS_vs_pt_softtest->Fill(inv_mass_Dstar - inv_mass, Dstar_4P.perp());
							}
							h_Dstar_RS_pt_vs_soft_pion_pt->Fill(Dstar_4P.perp(), soft_pion4P.perp());
						}
						for (unsigned int i_soft = 0; i_soft < v_pions_neg.size(); i_soft++) {
							//if (j == i_soft) continue;
							StLorentzVectorD soft_pion4P = v_pions_neg[i_soft];
							if (fPtSoftPionMin > v_pions_pos[i_soft].perp()) continue;
							StLorentzVectorD Dstar_4P = D0_4P + soft_pion4P;
							double inv_mass_Dstar = Dstar_4P.m();
							//Dstar_WS->Fill(inv_mass_Dstar - inv_mass);
							h_Dstar_WS_vs_pt->Fill(inv_mass_Dstar - inv_mass, Dstar_4P.perp());
							if (Soft_pion_pt_test(soft_pion4P.perp(), Dstar_4P.perp()) == true) {
								h_Dstar_WS_vs_pt_softtest->Fill(inv_mass_Dstar - inv_mass, Dstar_4P.perp());
							}
							h_Dstar_WS_pt_vs_soft_pion_pt->Fill(Dstar_4P.perp(), soft_pion4P.perp());
						}
					} else if ((inv_mass < 1.88 && inv_mass > 1.72) || (inv_mass < 1.92 && inv_mass > 1.202)) {
						if(fKaonCosThMaxSet==kTRUE && kaon_cos_theta_star > fKaonCosThMax) continue;
						for (unsigned int i_soft = 0; i_soft < v_pions_neg.size(); i_soft++) {
							if (TMath::Abs(v_pions_neg[i_soft].perp() - pion4P.perp()) < 1e-9) continue;

							StLorentzVectorD soft_pion4P = v_pions_pos[i_soft];
							if (fPtSoftPionMin > v_pions_pos[i_soft].perp()) continue;
							StLorentzVectorD Dstar_4P = D0_4P + soft_pion4P;
							double inv_mass_Dstar = Dstar_4P.m();

							h_Dstar_Sideband_vs_pt->Fill(inv_mass_Dstar - inv_mass, Dstar_4P.perp());
							if (Soft_pion_pt_test(soft_pion4P.perp(), Dstar_4P.perp()) == true) {
								h_Dstar_Sideband_vs_pt_softtest->Fill(inv_mass_Dstar - inv_mass, Dstar_4P.perp());
							}
						}
					}
				}
			}
			//for (unsigned int j = 0; j < v_pions_neg.size(); j++) {
			for (unsigned int j = 0; j < v_pions_ht_neg.size(); j++) {
				if (v_pions_ht_neg[j].perp() < 0.2) continue;

				if (i == v_pions_neg_overlap.at(j)) {
					//cout << "skip this neg pion" << endl;
					continue;
				}
				StLorentzVectorD pion4P = v_pions_ht_neg[j];
				StLorentzVectorD D0_4P = kaon4P + pion4P;
				//Calculating invariant mass
				double inv_mass = D0_4P.m();
				const StThreeVectorD deonMomAtDca = D0_4P.vect();
				double PT  = deonMomAtDca.perp();

				/*
				int j_d0_pt = FindHistoIndex(D0_4P.perp(), d0_pt_dep, N_D0_PT_DEP);
				if (j_d0_pt < 1) continue;
				*/
				//h_inv_mass_LS_pt[j_kaon_pt-1][j_d0_pt-1]->Fill(inv_mass);
				h_inv_mass_LS->Fill(inv_mass);
				/*
				inv_mass_ls->push_back(inv_mass);
				pt_kaon_ls->push_back(kaon4P.perp());
				pt_pion_ls->push_back(pion4P.perp());
				pt_D0_ls->push_back(D0_4P.perp());
				*/
				h_inv_mass_LS_neg->Fill(inv_mass);
				//h_pt_LS->Fill(PT);
				//if (inv_mass > 0.722 && inv_mass < 0.725) {
				/*
				h_kaon_pt_LS->Fill(kaon4P.perp());
				h_pion_pt_LS->Fill(pion4P.perp());
				*/
				//}
				//h_inv_mass_LS_Kplus->Fill(inv_mass);
				h_inv_mass_LS_Kplus_vs_pt->Fill(inv_mass, PT);
				/*
				h_inv_mass_LS_Kplus_vs_pt_kaon->Fill(inv_mass, kaon4P.perp());
				h_inv_mass_LS_Kplus_vs_pt_pion->Fill(inv_mass, pion4P.perp());
				*/
			}
		}

		for (unsigned int i = 0; i < v_kaons_pos.size(); i++) {
			StLorentzVectorD kaon4P = v_kaons_pos[i];
			//rotational background: rotate whole vector over 180 deg.
			StThreeVectorD kaon_3P(-(kaon4P.x()), -(kaon4P.y()), -(kaon4P.z()));
			StLorentzVectorD kaonRot4P(kaon_3P, kaon_3P.massHypothesis(kPdgMass[KAON]));

			//rotational background: rotate phi over 180 deg.
			StThreeVectorD kaon_3P_rot_phi(kaon4P.x(), kaon4P.y(), kaon4P.z());
			kaon_3P_rot_phi.setPhi(-kaon_3P_rot_phi.phi());
			StLorentzVectorD kaonRotPhi4P(kaon_3P_rot_phi, kaon_3P_rot_phi.massHypothesis(kPdgMass[KAON]));

			kaonRot4P = kaonRotPhi4P;
			int j_kaon_pt = FindHistoIndex(kaon4P.perp(), kaon_pt_dep, N_KAON_PT_DEP);
			//cout << "original method: " << j_kaon_pt << " new method: " << index << endl;
			if (j_kaon_pt < 1) continue;
			//for (unsigned int j = 0; j < v_pions_neg.size(); j++) {
			for (unsigned int j = 0; j < v_pions_ht_neg.size(); j++) {
				if (v_pions_ht_neg[j].perp() < 0.2) continue;
				StLorentzVectorD pion4P = v_pions_ht_neg[j];
				StLorentzVectorD D0_4P = kaon4P + pion4P;

				//Calculating invariant mass
				double inv_mass = D0_4P.m();

				const StThreeVectorD deonMomAtDca = D0_4P.vect();
				double PT  = deonMomAtDca.perp();

				//Boosting back the daughters
				StLorentzVectorD kaonMomAtRF = kaon4P.boost(-D0_4P);
				StLorentzVectorD pionMomAtRF = pion4P.boost(-D0_4P);

				//Calculating ThetaStar angles for the daughters
				//double R_KAON_COS_theta_star = TMath::Cos( deonMomAtDca.angle(kaonMomAtRF.vect()) );
				//if(fKaonCosThMaxSet==kTRUE && R_KAON_COS_theta_star > fKaonCosThMax) continue;

				/*
				int j_d0_pt = FindHistoIndex(D0_4P.perp(), d0_pt_dep, N_D0_PT_DEP);
				if (j_d0_pt < 1) continue;
				*/
				//h_inv_mass_US_pt[j_kaon_pt-1][j_d0_pt-1]->Fill(inv_mass);

				h_inv_mass_US->Fill(inv_mass);
				/*
				inv_mass_us->push_back(inv_mass);
				pt_kaon_us->push_back(kaon4P.perp());
				pt_pion_us->push_back(pion4P.perp());
				pt_D0_us->push_back(D0_4P.perp());
				h_pt_US->Fill(PT);
				h_kaon_pt_US->Fill(kaon4P.perp());
				h_pion_pt_US->Fill(pion4P.perp());
				*/
				h_inv_mass_US_Kplus->Fill(inv_mass);
				h_inv_mass_US_Kplus_vs_pt->Fill(inv_mass, PT);
				/*
				h_inv_mass_US_Kplus_vs_pt_kaon->Fill(inv_mass, kaon4P.perp());
				h_inv_mass_US_Kplus_vs_pt_pion->Fill(inv_mass, pion4P.perp());
				*/
				double m_rot = (kaonRot4P+pion4P).m();
				h_inv_mass_ROT->Fill(m_rot);
				//inv_mass_rot->push_back(m_rot);
				//h_inv_mass_ROT_Kplus->Fill(m_rot);
				h_inv_mass_ROT_Kplus_vs_pt->Fill(m_rot, PT);
				/*
				h_inv_mass_ROT_Kplus_vs_pt_kaon->Fill(m_rot, kaon4P.perp());
				h_inv_mass_ROT_Kplus_vs_pt_pion->Fill(m_rot, pion4P.perp());
				*/

				//bool Dstar = true;
				if (Dstar == true) {
					//Boosting back the daughters
					StLorentzVectorD kaonMomAtRF = kaon4P.boost(-D0_4P);
					//StLorentzVectorD pionMomAtRF = pion4P.boost(-D0_4P);

					//Calculating ThetaStar angles for the daughters
					double kaon_cos_theta_star = TMath::Cos( deonMomAtDca.angle(kaonMomAtRF.vect()) );
					//h_kaon_cos_theta_star_vs_pt_D0->Fill(kaon_cos_theta_star, PT);

					//if(inv_mass < 1.89 && inv_mass > 1.84) {
					if (inv_mass < 1.90 && inv_mass > 1.82) {
						h_pt_D0_vs_kaon_cos_theta_star->Fill(PT, kaon_cos_theta_star);
						if(fKaonCosThMaxSet==kTRUE && kaon_cos_theta_star > fKaonCosThMax) continue;
						for (unsigned int i_soft = 0; i_soft < v_pions_neg.size(); i_soft++) {
							//if (j == i_soft) continue;
							if (TMath::Abs(v_pions_neg[i_soft].perp() - pion4P.perp()) < 1e-9) continue;
							StLorentzVectorD soft_pion4P = v_pions_neg[i_soft];
							if (fPtSoftPionMin > soft_pion4P.perp()) continue;
							StLorentzVectorD Dstar_4P = D0_4P + soft_pion4P;
							double inv_mass_Dstar = Dstar_4P.m();
							//Dstar_RS->Fill(inv_mass_Dstar - inv_mass);
							h_Dstar_RS_vs_pt->Fill(inv_mass_Dstar - inv_mass, Dstar_4P.perp());
							if (Soft_pion_pt_test(soft_pion4P.perp(), Dstar_4P.perp()) == true) {
								h_Dstar_RS_vs_pt_softtest->Fill(inv_mass_Dstar - inv_mass, Dstar_4P.perp());
							}
							h_Dstar_RS_pt_vs_soft_pion_pt->Fill(Dstar_4P.perp(), soft_pion4P.perp());
						}
						for (unsigned int i_soft = 0; i_soft < v_pions_pos.size(); i_soft++) {
							//if (j == i_soft) continue;
							StLorentzVectorD soft_pion4P = v_pions_pos[i_soft];
							if (fPtSoftPionMin > soft_pion4P.perp()) continue;
							StLorentzVectorD Dstar_4P = D0_4P + soft_pion4P;
							double inv_mass_Dstar = Dstar_4P.m();
							//Dstar_WS->Fill(inv_mass_Dstar - inv_mass);
							h_Dstar_WS_vs_pt->Fill(inv_mass_Dstar - inv_mass, Dstar_4P.perp());
							if (Soft_pion_pt_test(soft_pion4P.perp(), Dstar_4P.perp()) == true) {
								h_Dstar_WS_vs_pt_softtest->Fill(inv_mass_Dstar - inv_mass, Dstar_4P.perp());
							}
							h_Dstar_WS_pt_vs_soft_pion_pt->Fill(Dstar_4P.perp(), soft_pion4P.perp());
						}
					} else if ((inv_mass < 1.88 && inv_mass > 1.72) || (inv_mass < 1.92 && inv_mass > 1.202)) {
						if(fKaonCosThMaxSet==kTRUE && kaon_cos_theta_star > fKaonCosThMax) continue;
						for (unsigned int i_soft = 0; i_soft < v_pions_neg.size(); i_soft++) {
							if (TMath::Abs(v_pions_neg[i_soft].perp() - pion4P.perp()) < 1e-9) continue;

							StLorentzVectorD soft_pion4P = v_pions_pos[i_soft];
							if (fPtSoftPionMin > v_pions_pos[i_soft].perp()) continue;
							StLorentzVectorD Dstar_4P = D0_4P + soft_pion4P;
							double inv_mass_Dstar = Dstar_4P.m();
							h_Dstar_Sideband_vs_pt->Fill(inv_mass_Dstar - inv_mass, Dstar_4P.perp());
							if (Soft_pion_pt_test(soft_pion4P.perp(), Dstar_4P.perp()) == true) {
								h_Dstar_Sideband_vs_pt_softtest->Fill(inv_mass_Dstar - inv_mass, Dstar_4P.perp());
							}
						}
					}
				}

			}
			//for (unsigned int j = 0; j < v_pions_pos.size(); j++) {
			for (unsigned int j = 0; j < v_pions_ht_pos.size(); j++) {
				if (v_pions_ht_pos[j].perp() < 0.2) continue;

				if (i == v_pions_pos_overlap.at(j)) {
					//cout << "skip this pos pion " << endl;
					continue;
				}
				StLorentzVectorD pion4P = v_pions_ht_pos[j];
				StLorentzVectorD D0_4P = kaon4P + pion4P;
				//Calculating invariant mass
				double inv_mass = D0_4P.m();
				const StThreeVectorD deonMomAtDca = D0_4P.vect();
				double PT  = deonMomAtDca.perp();

				/*
				int j_d0_pt = FindHistoIndex(D0_4P.perp(), d0_pt_dep, N_D0_PT_DEP);
				if (j_d0_pt < 1) continue;
				*/
				//h_inv_mass_LS_pt[j_kaon_pt-1][j_d0_pt-1]->Fill(inv_mass);
				h_inv_mass_LS->Fill(inv_mass);
				/*
				inv_mass_ls->push_back(inv_mass);
				pt_kaon_ls->push_back(kaon4P.perp());
				pt_pion_ls->push_back(pion4P.perp());
				pt_D0_ls->push_back(D0_4P.perp());
				*/
				h_inv_mass_LS_pos->Fill(inv_mass);
				/*
				h_pt_LS->Fill(PT);
				//if (inv_mass > 0.722 && inv_mass < 0.725) {
				h_kaon_pt_LS->Fill(kaon4P.perp());
				h_pion_pt_LS->Fill(pion4P.perp());
				//}
				h_inv_mass_LS_Kminus->Fill(inv_mass);
				*/
				h_inv_mass_LS_Kminus_vs_pt->Fill(inv_mass, PT);
				/*
				h_inv_mass_LS_Kminus_vs_pt_kaon->Fill(inv_mass, kaon4P.perp());
				h_inv_mass_LS_Kminus_vs_pt_pion->Fill(inv_mass, pion4P.perp());
				*/
			}
		}




		//if (v_kaons_neg.size() > 0 && v_kaons_pos.size() > 0) 
		if (v_kaons_neg.size() != v_kaons_neg_sigmaDedx.size() || v_kaons_neg.size() != v_kaons_neg_sigmaTof.size()) {
			cout << "vector kaon neg size diff!!! " << endl;
			return;
		}
		if (v_kaons_pos.size() != v_kaons_pos_sigmaDedx.size() || v_kaons_pos.size() != v_kaons_pos_sigmaTof.size()) {
			cout << "vector kaon pos size diff!!! " << endl;
			return;
		}
		for (unsigned int i = 0; i < v_kaons_neg.size(); i++) {
			h_kaon_neg_eta_pt->Fill(v_kaons_neg[i].pseudoRapidity(), v_kaons_neg[i].perp());
			h_kaon_neg_eta->Fill(v_kaons_neg[i].pseudoRapidity());
			h_kaon_neg_phi->Fill(v_kaons_neg[i].phi());
			h_kaon_neg_pt->Fill(v_kaons_neg[i].perp());
			for (unsigned int j = 0; j < v_kaons_pos.size(); j++) {
				StLorentzVectorD KK = v_kaons_neg[i] + v_kaons_pos[j];
				h_KK_inv_mass_vs_pt->Fill(KK.m(), KK.perp());
				h_KK_inv_mass_vs_pt_2gev_us->Fill(KK.m(), KK.perp());
				h_KK_pt->Fill(KK.perp());
				h_KK_phi->Fill(KK.phi());
				h_KK_eta->Fill(KK.pseudoRapidity());
				h_KK_inv_mass->Fill(KK.m());

				//Boosting back the daughters
				StLorentzVectorD k_neg_rf = v_kaons_neg[i].boost(-KK);  
				StLorentzVectorD k_pos_rf = v_kaons_pos[j].boost(-KK);  
				//Calculating angles between the kaons
				double angle = k_neg_rf.vect().angle(k_pos_rf.vect());
				h_KK_angle->Fill(angle);
				//Calculating ThetaStar
				//if (KK.m() < 1.04 && KK.m() > 1.0) 
				//if (KK.m() < 1.029 && KK.m() > 1.009) // res_2jul2015.root  
				//if (KK.m() < 1.022 && KK.m() > 1.017) {
				if (KK.m() < 1.024 && KK.m() > 1.015) {
					for (unsigned int i_pion = 0; i_pion < v_pions_ht_pos.size(); i_pion++) {
						if (v_pions_ht_pos[i_pion].perp() < 0.2) continue;
						StLorentzVectorD pion4P = v_pions_ht_pos[i_pion];
						StLorentzVectorD DS_4P = KK + pion4P;
						h_inv_mass_DS_plus_vs_pt->Fill(DS_4P.m(), DS_4P.perp());
					}
					for (unsigned int i_pion = 0; i_pion < v_pions_ht_neg.size(); i_pion++) {
						if (v_pions_ht_neg[i_pion].perp() < 0.2) continue;
						StLorentzVectorD pion4P = v_pions_ht_pos[i_pion];
						StLorentzVectorD DS_4P = KK + pion4P;
						h_inv_mass_DS_minus_vs_pt->Fill(DS_4P.m(), DS_4P.perp());
					}
					double cos_theta_star_pos = TMath::Cos(KK.vect().angle(k_pos_rf.vect()));
					double cos_theta_star_neg = TMath::Cos(KK.vect().angle(k_neg_rf.vect()));
					h_KK_cos_theta_star_pos->Fill(cos_theta_star_pos);
					h_KK_cos_theta_star_neg->Fill(cos_theta_star_neg);
					//			if (TMath::Abs(cos_theta_star_pos) < 0.8 && TMath::Abs(cos_theta_star_neg) < 0.8) {
					v_kaon_used_pos.push_back(j);
					v_KK.push_back(KK);

					//TH2D *h_kaon_p_vs_dedx_phimeson;
					//TH2D *h_kaon_p_vs_oneOverBetaTof_phimeson;
					h_kaon_p_vs_sigmaDedx_phimeson->Fill(v_kaons_neg[i].vect().mag(), v_kaons_neg_sigmaDedx[i]);
					h_kaon_p_vs_sigmaDedx_phimeson->Fill(v_kaons_pos[j].vect().mag(), v_kaons_pos_sigmaDedx[j]);
					/*
					cout << "phi meson: " << endl;
					cout << v_kaons_neg[i].vect().mag() << " " << v_kaons_neg_sigmaDedx[i] << endl;
					cout << v_kaons_pos[i].vect().mag() << " " << v_kaons_pos_sigmaDedx[i] << endl;
					*/
					if (v_kaons_neg_sigmaTof[i] > -99)
						h_kaon_p_vs_sigmaTof_phimeson->Fill(v_kaons_neg[i].vect().mag(), v_kaons_neg_sigmaTof[i]);
					if (v_kaons_pos_sigmaTof[i] > -99)
						h_kaon_p_vs_sigmaTof_phimeson->Fill(v_kaons_pos[j].vect().mag(), v_kaons_pos_sigmaTof[j]);

					/*
					   h_KK_inv_mass_vs_pt->Fill(KK.m(), KK.perp());
					   h_KK_pt->Fill(KK.perp());
					   h_KK_phi->Fill(KK.phi());
					   h_KK_eta->Fill(KK.pseudoRapidity());
					   h_KK_inv_mass->Fill(KK.m());
					   */
					//			}
				}
			}
			for (unsigned int j = i; j < v_kaons_neg.size(); j++) {
				if (i == j) continue;
				StLorentzVectorD KK = v_kaons_neg[i] + v_kaons_neg[j];
				h_KK_minus_inv_mass->Fill(KK.m());
				h_KK_inv_mass_vs_pt_2gev_lsm->Fill(KK.m(), KK.perp());
			}
		}

		for (unsigned int i = 0; i < v_kaons_pos.size(); i++) {
			h_kaon_pos_eta_pt->Fill(v_kaons_pos[i].pseudoRapidity(), v_kaons_pos[i].perp());
			h_kaon_pos_eta->Fill(v_kaons_pos[i].pseudoRapidity());
			h_kaon_pos_phi->Fill(v_kaons_pos[i].phi());
			h_kaon_pos_pt->Fill(v_kaons_pos[i].perp());
			for (unsigned int j = i; j < v_kaons_pos.size(); j++) {
				if (i == j) continue;
				StLorentzVectorD KK = v_kaons_pos[i] + v_kaons_pos[j];
				h_KK_plus_inv_mass->Fill(KK.m());
				h_KK_inv_mass_vs_pt_2gev_lsp->Fill(KK.m(), KK.perp());
			}
		}
		if (Eta_c) {
			bool b_eta = true;
			for (unsigned int i = 0; i < v_kaon_used_pos.size(); i++) {
				if (b_eta == false) break;
				for (unsigned int j = i; j < v_kaon_used_pos.size(); j++) {
					if (i == j) continue;
					if (v_kaon_used_pos[i] == v_kaon_used_pos[j]) {
						b_eta = false;
						break;
					}
				}
			}
			if (b_eta == true) {
				if (v_KK.size() == 2) {
					for (unsigned int i = 0; i < v_KK.size(); i++) {
						//if (TMath::Abs(v_KK[i].pseudoRapidity()) > 0.5) continue;
						for (unsigned int j = i; j < v_KK.size(); j++) {
							if (i == j) continue;
							//if (TMath::Abs(v_KK[j].pseudoRapidity()) > 0.5) continue;
							StLorentzVectorD eta_c = v_KK[i] + v_KK[j];
							h_Eta_c_inv_mass_vs_pt->Fill(eta_c.m(), eta_c.perp());
							h_Eta_c_inv_mass_vs_eta->Fill(eta_c.m(), eta_c.pseudoRapidity());
							h_Eta_c_phi->Fill(eta_c.phi());
							h_Eta_c_eta->Fill(eta_c.pseudoRapidity());
							h_Eta_c_inv_mass->Fill(eta_c.m());
							//Boosting back the daughters
							StLorentzVectorD k_neg_rf = v_KK[i].boost(-eta_c);  
							StLorentzVectorD k_pos_rf = v_KK[j].boost(-eta_c);  
							//Calculating angles between the kaons
							double angle = k_neg_rf.vect().angle(k_pos_rf.vect());
							h_Eta_c_angle->Fill(angle);
							//Calculating ThetaStar
							double cos_theta_star_pos = TMath::Cos(eta_c.vect().angle(k_pos_rf.vect()));
							double cos_theta_star_neg = TMath::Cos(eta_c.vect().angle(k_neg_rf.vect()));
							h_Eta_c_cos_theta_star_pos->Fill(cos_theta_star_pos);
							h_Eta_c_cos_theta_star_neg->Fill(cos_theta_star_neg);
						}
					}
				}
			}
		}
	} else {
		for (unsigned int i = 0; i < v_kaons_neg.size(); i++) {
			StLorentzVectorD kaon4P = v_kaons_neg[i];
			int j_kaon_pt = FindHistoIndex(kaon4P.perp(), kaon_pt_dep, N_KAON_PT_DEP);
			if (j_kaon_pt < 1) continue;
			for (unsigned int j = 0; j < v_pions_pos.size(); j++) {
				StLorentzVectorD pion4P = v_pions_pos[j];
			/*
			for (unsigned int j = 0; j < v_pions_ht_pos.size(); j++) {
				StLorentzVectorD pion4P = v_pions_ht_pos[j];
				*/
				StLorentzVectorD D0_4P = kaon4P + pion4P;

				//Calculating invariant mass
				double inv_mass = D0_4P.m();

				double PT  = D0_4P.perp();

				/*
				int j_d0_pt = FindHistoIndex(D0_4P.perp(), d0_pt_dep, N_D0_PT_DEP);
				if (j_d0_pt < 1) continue;
				*/
				//cout << "original method: " << j_d0_pt << " new method: " << index << endl;

				h_inv_mass_MIX->Fill(inv_mass);
				//h_inv_mass_MIX_pt[j_kaon_pt-1][j_d0_pt-1]->Fill(inv_mass);
				h_pt_MIX->Fill(PT);
				/*
				h_kaon_pt_MIX->Fill(kaon4P.perp());
				h_pion_pt_MIX->Fill(pion4P.perp());
				*/
				h_inv_mass_MIX_Kminus->Fill(inv_mass);
				h_inv_mass_MIX_Kminus_vs_pt->Fill(inv_mass, PT);
				/*
				h_inv_mass_MIX_Kminus_vs_pt_kaon->Fill(inv_mass, kaon4P.perp());
				h_inv_mass_MIX_Kminus_vs_pt_pion->Fill(inv_mass, pion4P.perp());
				*/
			}
		}

		for (unsigned int i = 0; i < v_kaons_pos.size(); i++) {
			StLorentzVectorD kaon4P = v_kaons_pos[i];
			int j_kaon_pt = FindHistoIndex(kaon4P.perp(), kaon_pt_dep, N_KAON_PT_DEP);
			if (j_kaon_pt < 1) continue;
			for (unsigned int j = 0; j < v_pions_neg.size(); j++) {
				StLorentzVectorD pion4P = v_pions_neg[j];
			/*
			for (unsigned int j = 0; j < v_pions_ht_neg.size(); j++) {
				StLorentzVectorD pion4P = v_pions_ht_neg[j];
				*/
				StLorentzVectorD D0_4P = kaon4P + pion4P;

				//Calculating invariant mass
				double inv_mass = D0_4P.m();

				double PT  = D0_4P.perp();

				/*
				int j_d0_pt = FindHistoIndex(D0_4P.perp(), d0_pt_dep, N_D0_PT_DEP);
				if (j_d0_pt < 1) continue;
				*/
				//h_inv_mass_MIX_pt[j_kaon_pt-1][j_d0_pt-1]->Fill(inv_mass);

				h_inv_mass_MIX->Fill(inv_mass);
				h_pt_MIX->Fill(PT);
				/*
				h_kaon_pt_MIX->Fill(kaon4P.perp());
				h_pion_pt_MIX->Fill(pion4P.perp());
				*/
				h_inv_mass_MIX_Kplus->Fill(inv_mass);
				h_inv_mass_MIX_Kplus_vs_pt->Fill(inv_mass, PT);
				/*
				h_inv_mass_MIX_Kplus_vs_pt_kaon->Fill(inv_mass, kaon4P.perp());
				h_inv_mass_MIX_Kplus_vs_pt_pion->Fill(inv_mass, pion4P.perp());
				*/
			}
		}
	}
}

//_______________________________________________________________________________________________
// method to find index of histo which will be fill
Int_t StWiciED0CorrLooper::FindHistoIndex(double pt, double *array, int range) {
	Int_t j_d0_pt = -1;
	if (pt < array[0]) {
		return j_d0_pt;
	}
	for (int i_d0_pt = 0; i_d0_pt < range; i_d0_pt++) {
		//cout << pt << " " << array[i_d0_pt] << " " << range << endl;
		if (pt > array[i_d0_pt]) {
			continue;
		} else {
			return i_d0_pt;
		}
	}
	return j_d0_pt;
}

//_______________________________________________________________________________________________
Bool_t StWiciED0CorrLooper::GetK0s(const WiciParticle* kaon, const WiciParticle* pion, WiciVertex& primVtx, const Double_t& magField, StLorentzVectorD& deon4P, Double_t results[]) {

	return kTRUE;
} 

Double_t StWiciED0CorrLooper::GetArmEllipseValue(Double_t alpha, Double_t shift) {
	Double_t val = 1-TMath::Power((alpha-fArmPhi)/((1.0+shift)*fArmZeta),2);
	if(val<0) return 0;
	return (1.0+shift)*fArmP*TMath::Sqrt(val);
}



/*
//_______________________________________________________________________________________________
Bool_t StWiciED0CorrLooper::IsPion(const WiciParticle* particle, const Double_t sigmaTofMin, const Double_t sigmaTofMax) {

}
*/

//_______________________________________________________________________________________________
Bool_t StWiciED0CorrLooper::IsKaon(const WiciParticle* particle, const Double_t sigmaTofMin, const Double_t sigmaTofMax) {
	if(fPtDependentPid==kTRUE) {
		fKaonSigmaTof->Fill(GetParticleMomentum(particle).Mag(), particle->GetSigmaTofKaon());
		//if(GetSigmaTof(particle, KAON)>=sigmaTofMin && GetSigmaTof(particle, KAON)<=sigmaTofMax) fKaonSigmaTofCut->Fill(GetParticleMomentum(particle).Mag(), particle->GetSigmaTofKaon());

		//pT \in (0.0; 1.3] -> ToF
		if(GetParticleMomentum(particle).Pt()<=1.3) {
			if(particle->GetBetaTof()>0) {
				if(GetSigmaTof(particle, KAON)<sigmaTofMin || GetSigmaTof(particle, KAON)>sigmaTofMax) return kFALSE;
			} else {
				return false;
			}
		}
		//pT \in (1.3; 2.07] -> ToF || dE/dx
		else if(GetParticleMomentum(particle).Pt()>1.3 && GetParticleMomentum(particle).Pt()<=2.07) {
			if(particle->GetBetaTof()>0) {
				if(GetSigmaTof(particle, KAON)<sigmaTofMin || GetSigmaTof(particle, KAON)>sigmaTofMax) return kFALSE;
			}
			else if(fSigDedxKaonMaxSet==kTRUE && TMath::Abs(particle->GetSigmaDedxKaon())>fSigDedxKaonMax) return kFALSE;
		}
		//pT \in (2.07; inf] -> dE/dx
		else if(GetParticleMomentum(particle).Pt()>2.07) {
				if(particle->GetBetaTof()>0) {
				if(GetSigmaTof(particle, KAON)<sigmaTofMin || GetSigmaTof(particle, KAON)>sigmaTofMax) return kFALSE;
			}
			if(fSigDedxKaonMaxSet==kTRUE && TMath::Abs(particle->GetSigmaDedxKaon())>fSigDedxKaonMax) return kFALSE;
		}
	}//if(fPtDependentPid)
	else {
		fKaonSigmaTof->Fill(GetParticleMomentum(particle).Mag(), particle->GetSigmaTofKaon());
		if(particle->GetBetaTof()>0 && ( GetSigmaTof(particle, KAON)<sigmaTofMin || GetSigmaTof(particle, KAON)>sigmaTofMax ) ) return kFALSE;
		if(fSigDedxKaonMaxSet==kTRUE && TMath::Abs(particle->GetSigmaDedxKaon())>fSigDedxKaonMax) return kFALSE;
	}
	return kTRUE;
}
//_______________________________________________________________________________________________
inline const TVector3& StWiciED0CorrLooper::GetParticleMomentum(const WiciParticle* particle) {
	//return fUsePrimaryMom == kTRUE ? particle->GetPrimaryPVector() : particle->GetGlobalPVector();
	return particle->GetPrimaryPVector();
}
//_______________________________________________________________________________________________
inline const Float_t StWiciED0CorrLooper::GetSigmaTof(const WiciParticle* particle, const Int_t partSpec) {
	Float_t sigmaTof = -9999;
	if(fUseAltTofPid==kTRUE) {
		sigmaTof = (1.0/particle->GetBetaTof() - TMath::Sqrt( kPdgMass[partSpec]*kPdgMass[partSpec] / (GetParticleMomentum(particle).Mag()*GetParticleMomentum(particle).Mag()) + 1.0 ) ) / 0.012;
	}
	else switch(partSpec) {
		//case ELEC: sigmaTof = particle->GetSigmaTofElectron(); break;
		case PION: sigmaTof = particle->GetSigmaTofPion(); break;
		case KAON: sigmaTof = particle->GetSigmaTofKaon(); break;
			   //case PROT: sigmaTof = particle->GetSigmaTofProton();
	}

	return sigmaTof;
}
