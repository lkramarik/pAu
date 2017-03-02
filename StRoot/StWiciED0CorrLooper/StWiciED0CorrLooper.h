#include <vector>
#include "WiciEvent/WiciEvent.h"
#include "WiciEvent/WiciVertex.h"
#include "WiciEvent/WiciParticle.h"
#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
#include <TVector3.h>
#include "StarClassLibrary/StLorentzVectorD.hh"
#include <TH1D.h>
#include <TH1F.h>
#include <TH2D.h>
#include <TH2F.h>
#include <TF1.h>

class StWiciED0CorrLooper {

	public:
		StWiciED0CorrLooper(TTree*, TString = "name");
		virtual ~StWiciED0CorrLooper();
		void Loop(const Int_t = 0, const Int_t = -1);
		void Finalize();
		//---------------------------------settings---------------------------------//
		void            SetMatchToFastDet(Bool_t match)     {fMatchToFastDet = match;};
		inline Bool_t   IsMatchToFastDet() const            {return fMatchToFastDet;};

		void            SetUseAltTofPid(Bool_t altPid)      {fUseAltTofPid = altPid;};
		inline Bool_t   IsUseAltTofPid() const              {return fUseAltTofPid;};

		void            SetNonLinearKaonPid(Bool_t kaonPid) {fNonLinearKaonPid = kaonPid;};
		inline Bool_t   IsNonLinearKaonPid() const          {return fNonLinearKaonPid;};

		void            SetPtDependentPid(Bool_t ptPid)     {fPtDependentPid = ptPid;};
		inline Bool_t   IsPtDependentPid() const            {return fPtDependentPid;};

		void            SetUsePrimaryMom(Bool_t primMom)    {fUsePrimaryMom = primMom;};
		inline Bool_t   IsUsePrimaryMom() const             {return fUsePrimaryMom;};

		void            SetFindMuVertex(Bool_t muVtx)       {fFindMuVertex = muVtx;};
		inline Bool_t   IsFindMuVertex() const              {return fFindMuVertex;};

		void            SetCorellateEles(Bool_t corrEle)    {fCorellateEles = corrEle;};
		inline Bool_t   IsCorellateEles() const             {return fCorellateEles;};

		void            SetUseEventMixer(Bool_t useMixer)   {fUseEventMixer = useMixer;};
		inline Bool_t   IsUseEventMixer() const             {return fUseEventMixer;};

		void            SetUseHighTower(Bool_t useHiTow)    {fUseHighTower = useHiTow;};
		inline Bool_t   IsUseHighTower() const              {return fUseHighTower;};

		inline Bool_t   IsDoLambda() const                  {return kDoLambda;};
		//-----------------------------------cuts-----------------------------------//
		inline const unsigned int* GetTrgArray() const      {return fTrgArray;};
		void            AddTrigger(unsigned int trg)        {fTrgArray[fTrgArraySize++]=trg; fTrgArraySet=kTRUE;};
		inline Bool_t   IsTrgArray() const                  {return fTrgArraySet;};
		void            DisableTrgArray()                   {fTrgArraySet=kFALSE; fTrgArraySize=0;};
		inline Double_t GetAbsVtxPosMax() const             {return fAbsVtxPosMax;};
		void            SetAbsVtxPosMax(Double_t absVtx)    {fAbsVtxPosMax = absVtx; fAbsVtxPosMaxSet=kTRUE;};
		inline Bool_t   IsAbsVtxPosMax() const              {return fAbsVtxPosMaxSet;};
		void            DisableAbsVtxPosMax()               {fAbsVtxPosMaxSet=kFALSE;};
		inline Int_t    GetEmcTreshold() const              {return fEmcTreshold;};
		void            SetEmcTreshold(Int_t thold)         {fEmcTreshold = thold;};
		inline Double_t GetTrgPartPtMin() const             {return fTrgPartPtMin;};
		void            SetTrgPartPtMin(Double_t trgPt)     {fTrgPartPtMin = trgPt;};
		inline Double_t GetTPCVpdVtxZ() const             {return fTPCVpdVtxZ;};
		void            SetTPCVpdVtxZ(Double_t Vtx)    {fTPCVpdVtxZ = Vtx; fTPCVpdVtxZSet=kTRUE;};
		//-----------------------------------kaon-----------------------------------//
		inline Double_t GetMomKaonMin() const               {return fMomKaonMin;};
		void            SetMomKaonMin(Double_t mom)         {fMomKaonMin = mom; fMomKaonMinSet=kTRUE;};
		inline Bool_t   IsMomKaonMin() const                {return fMomKaonMinSet;};
		void            DisableMomKaonMin()                 {fMomKaonMinSet=kFALSE;};

		inline Double_t GetDcaKaonMin() const               {return fDcaKaonMin;};
		void            SetDcaKaonMin(Double_t dca)         {fDcaKaonMin = dca; fDcaKaonMinSet = kTRUE;};

		inline Double_t GetPtKaonMin() const                {return fPtKaonMin;};
		void            SetPtKaonMin(Double_t pt)           {fPtKaonMin = pt; fPtKaonMinSet=kTRUE;};
		inline Bool_t   IsPtKaonMin() const                 {return fPtKaonMinSet;};
		void            DisablePtKaonMin()                  {fPtKaonMinSet=kFALSE;};

		inline Double_t GetSigDedxKaonMax() const           {return fSigDedxKaonMax;};
		void            SetSigDedxKaonMax(Double_t sig)     {fSigDedxKaonMax = sig; fSigDedxKaonMaxSet=kTRUE;};
		inline Bool_t   IsSigDedxKaonMax() const            {return fSigDedxKaonMaxSet;};
		void            DisableSigDedxKaonMax()             {fSigDedxKaonMaxSet=kFALSE;};

		inline Double_t GetSigBtofKaonMax() const           {return fSigBtofKaonMax;};
		void            SetSigBtofKaonMax(Double_t sig)     {fSigBtofKaonMax = sig; fSigBtofKaonMaxSet=kTRUE;};
		inline Bool_t   IsSigBtofKaonMax() const            {return fSigBtofKaonMaxSet;};
		void            DisableSigBtofKaonMax()             {fSigBtofKaonMaxSet=kFALSE;};

		void            SetEtaKaonMax(Double_t eta)         {fEtaKaonMax = eta;};
		//-----------------------------------pion-----------------------------------//

		inline Double_t GetDcaPionMin() const               {return fDcaPionMin;};
		void            SetDcaPionMin(Double_t dca)         {fDcaPionMin = dca; fDcaPionMinSet = kTRUE;};

		inline Double_t GetMomPionMin() const               {return fMomPionMin;};
		void            SetMomPionMin(Double_t mom)         {fMomPionMin = mom; fMomPionMinSet=kTRUE;};
		inline Bool_t   IsMomPionMin() const                {return fMomPionMinSet;};
		void            DisableMomPionMin()                 {fMomPionMinSet=kFALSE;};

		inline Double_t GetPtPionMin() const                {return fPtPionMin;};
		void            SetPtPionMin(Double_t pt)           {fPtPionMin = pt; fPtPionMinSet=kTRUE;};
		inline Bool_t   IsPtPionMin() const                 {return fPtPionMinSet;};
		void            DisablePtPionMin()                  {fPtPionMinSet=kFALSE;};

		inline Double_t GetSigDedxPionMax() const           {return fSigDedxPionMax;};
		void            SetSigDedxPionMax(Double_t sig)     {fSigDedxPionMax = sig; fSigDedxPionMaxSet=kTRUE;};
		inline Bool_t   IsSigDedxPionMax() const            {return fSigDedxPionMaxSet;};
		void            DisableSigDedxPionMax()             {fSigDedxPionMaxSet=kFALSE;};

		inline Double_t GetSigBtofPionMax() const           {return fSigBtofPionMax;};
		void            SetSigBtofPionMax(Double_t sig)     {fSigBtofPionMax = sig; fSigBtofPionMaxSet=kTRUE;};
		inline Bool_t   IsSigBtofPionMax() const            {return fSigBtofPionMaxSet;};
		void            DisableSigBtofPionMax()             {fSigBtofPionMaxSet=kFALSE;};
		//--------------------------------soft pion---------------------------------//
		inline Double_t GetMomSoftPionMin() const               {return fMomSoftPionMin;};
		void            SetMomSoftPionMin(Double_t mom)         {fMomSoftPionMin = mom; fMomSoftPionMinSet=kTRUE;};
		inline Bool_t   IsMomSoftPionMin() const                {return fMomSoftPionMinSet;};
		void            DisableMomSoftPionMin()                 {fMomSoftPionMinSet=kFALSE;};

		inline Double_t GetPtSoftPionMin() const                {return fPtSoftPionMin;};
		void            SetPtSoftPionMin(Double_t pt)           {fPtSoftPionMin = pt; fPtSoftPionMinSet=kTRUE;};
		inline Bool_t   IsPtSoftPionMin() const                 {return fPtSoftPionMinSet;};
		void            DisablePtSoftPionMin()                  {fPtSoftPionMinSet=kFALSE;};

		inline Double_t GetSigDedxSoftPionMax() const           {return fSigDedxSoftPionMax;};
		void            SetSigDedxSoftPionMax(Double_t sig)     {fSigDedxSoftPionMax = sig; fSigDedxSoftPionMaxSet=kTRUE;};
		inline Bool_t   IsSigDedxSoftPionMax() const            {return fSigDedxSoftPionMaxSet;};
		void            DisableSigDedxSoftPionMax()             {fSigDedxSoftPionMaxSet=kFALSE;};

		inline Double_t GetSigBtofSoftPionMax() const           {return fSigBtofSoftPionMax;};
		void            SetSigBtofSoftPionMax(Double_t sig)     {fSigBtofSoftPionMax = sig; fSigBtofSoftPionMaxSet=kTRUE;};
		inline Bool_t   IsSigBtofSoftPionMax() const            {return fSigBtofSoftPionMaxSet;};
		void            DisableSigBtofSoftPionMax()             {fSigBtofSoftPionMaxSet=kFALSE;};
		//--------------------------------mu vertex---------------------------------//
		inline Double_t GetDcaMax() const                   {return fDcaMax;};
		void            SetDcaMax(Double_t dca)             {fDcaMax = dca; fDcaMaxSet=kTRUE;};
		inline Bool_t   IsDcaMax() const                    {return fDcaMaxSet;};
		void            DisableDcaMax()                     {fDcaMaxSet=kFALSE;};

		inline Double_t GetDecLengthMax() const             {return fDecLengthMax;};
		void            SetDecLengthMax(Double_t decLength) {fDecLengthMax = decLength; fDecLengthMaxSet=kTRUE;};
		inline Bool_t   IsDecLengthMax() const              {return fDecLengthMaxSet;};
		void            DisableDecLengthMax()               {fDecLengthMaxSet=kFALSE;};

		inline Double_t GetDecLengthMin() const             {return fDecLengthMin;};
		void            SetDecLengthMin(Double_t decLength) {fDecLengthMin = decLength; fDecLengthMinSet=kTRUE;};
		inline Bool_t   IsDecLengthMin() const              {return fDecLengthMinSet;};
		void            DisableDecLengthMin()               {fDecLengthMinSet=kFALSE;};

		inline Double_t GetDcaProductMax() const            {return fDcaProductMax;};
		void            SetDcaProductMax(Double_t dcaProd)  {fDcaProductMax = dcaProd; fDcaProductMaxSet=kTRUE;};
		inline Bool_t   IsDcaProductMax() const             {return fDcaProductMaxSet;};
		void            DisableDcaProductMax()              {fDcaProductMaxSet=kFALSE;};

		inline Double_t GetDcaProductMin() const            {return fDcaProductMin;};
		void            SetDcaProductMin(Double_t dcaProd)  {fDcaProductMin = dcaProd; fDcaProductMinSet=kTRUE;};
		inline Bool_t   IsDcaProductMin() const             {return fDcaProductMinSet;};
		void            DisableDcaProductMin()              {fDcaProductMinSet=kFALSE;};

		inline Double_t GetOpenAngleMax() const             {return fOpenAngleMax;};
		void            SetOpenAngleMax(Double_t angle)     {fOpenAngleMax = angle; fOpenAngleMaxSet=kTRUE;};
		inline Bool_t   IsOpenAngleMax() const              {return fOpenAngleMaxSet;};
		void            DisableOpenAngleMax()               {fOpenAngleMaxSet=kFALSE;};

		inline Double_t GetPointAngleMax() const            {return fPointAngleMax;};
		void            SetPointAngleMax(Double_t angle)    {fPointAngleMax = angle; fPointAngleMaxSet=kTRUE;};
		inline Bool_t   IsPointAngleMax() const             {return fPointAngleMaxSet;};
		void            DisablePointAngleMax()              {fPointAngleMaxSet=kFALSE;};

		inline Double_t GetDcaToPrimVtxMax() const          {return fDcaToPrimVtxMax;};
		void            SetDcaToPrimVtxMax(Double_t dca)    {fDcaToPrimVtxMax = dca; fDcaToPrimVtxMaxSet=kTRUE;};
		inline Bool_t   IsDcaToPrimVtxMax() const           {return fDcaToPrimVtxMaxSet;};
		void            DisableDcaToPrimVtxMax()            {fDcaToPrimVtxMaxSet=kFALSE;};

		inline Double_t GetKaonCosThMax() const             {return fKaonCosThMax;};
		void            SetKaonCosThMax(Double_t cosTheta)  {fKaonCosThMax = cosTheta; fKaonCosThMaxSet=kTRUE;};
		inline Bool_t   IsKaonCosThMax() const              {return fKaonCosThMaxSet;};
		void            DisableKaonCosThMax()               {fKaonCosThMaxSet=kFALSE;};

		inline Double_t**  GetKaonCosThMaxArray() const                           {return fKaonCosThMaxArray;};
		void               SetKaonCosThMaxArray(Int_t size, Double_t** cosTheta)  {fKaonCosThMaxArraySize = size; fKaonCosThMaxArray = cosTheta; fKaonCosThMaxArraySet=kTRUE;};
		inline Bool_t      IsKaonCosThMaxArray() const                            {return fKaonCosThMaxArraySet;};
		void               DisableKaonCosThMaxArray()                             {fKaonCosThMaxArraySet=kFALSE;};

		inline Double_t    GetInvMassMin() const                                  {return kInvMassMin;};
		inline Double_t    GetInvMassMax() const                                  {return kInvMassMax;};

		inline Double_t GetArmP() const                                           {return fArmP;};
		inline Double_t GetArmPhi() const                                         {return fArmPhi;};
		inline Double_t GetArmZeta() const                                        {return fArmZeta;};
		void            SetArmParams(Double_t p, Double_t phi, Double_t zeta)     {fArmP = p; fArmPhi = phi; fArmZeta = zeta;};
		inline Double_t GetArmEllipseCut() const                                  {return fArmEllipseCut;};
		void            SetArmEllispeCut(Double_t shift)                          {fArmEllipseCut = shift; fArmEllipseCutSet = kTRUE;};
		inline Bool_t   IsArmEllipseCut() const                                   {return fArmEllipseCutSet;};
		void            DisableArmEllispeCut()                                    {fArmEllipseCutSet=kFALSE;};
		inline Double_t GetArmAlphaMin() const                                    {return fArmAlphaMin;};
		void            SetArmAlphaMin(Double_t alpha)                            {fArmAlphaMin = alpha; fArmAlphaMinSet = kTRUE;};
		inline Bool_t   IsArmAlphaMin() const                                     {return fArmAlphaMinSet;};
		void            DisableArmAlphaMin()                                      {fArmAlphaMinSet=kFALSE;};
		inline Double_t GetArmAlphaMax() const                                    {return fArmAlphaMax;};
		void            SetArmAlphaMax(Double_t alpha)                            {fArmAlphaMax = alpha; fArmAlphaMaxSet = kTRUE;};
		inline Bool_t   IsArmAlphaMax() const                                     {return fArmAlphaMaxSet;};
		void            DisableArmAlphaMax()                                      {fArmAlphaMaxSet=kFALSE;};

		inline Double_t GetArmQtMin() const                                       {return fArmQtMin;};
		void            SetArmQtMin(Double_t qt)                                  {fArmQtMin = qt; fArmQtMinSet = kTRUE;};
		inline Bool_t   IsArmQtMin() const                                        {return fArmQtMinSet;};
		void            DisableArmQtMin()                                         {fArmQtMinSet=kFALSE;};
		inline Double_t GetArmQtMax() const                                       {return fArmQtMax;};
		void            SetArmQtMax(Double_t qt)                                  {fArmQtMax = qt; fArmQtMaxSet = kTRUE;};
		inline Bool_t   IsArmQtMax() const                                        {return fArmQtMaxSet;};
		void            DisableArmQtMax()                                         {fArmQtMaxSet=kFALSE;};

		inline Double_t GetPtMax() const               {return fPtMax;};
		void            SetPtMax(Double_t mom)         {fPtMax = mom; fPtMaxSet=kTRUE;};

		inline Double_t GetRefMultMin() const               {return fRefMultMin;};
		void            SetRefMultMin(Double_t mult)         {fRefMultMin = mult; fRefMultMinSet=kTRUE;};
		inline Double_t GetRefMultMax() const               {return fRefMultMax;};
		void            SetRefMultMax(Double_t mult)         {fRefMultMax = mult; fRefMultMaxSet=kTRUE;};

		void		AddBadRun(Int_t run) {bad_run_list.push_back(run);};

		void SetConditions(Int_t cond) {f_cond = cond;};
		inline Int_t GetCondition() {return f_cond;};

	private:
		void                    ProcessEvent(WiciEvent*, const Bool_t=kFALSE);
		Bool_t                  CheckTrigger(const vector<unsigned int>&);
		Bool_t                  CheckBinaryTrigger(const unsigned int);
		Bool_t                  IsKaon(const WiciParticle*, const Double_t, const Double_t);
		inline const TVector3&  GetParticleMomentum(const WiciParticle*);
		inline const Float_t    GetSigmaTof(const WiciParticle*, const Int_t);
		//Bool_t                  GetPairDecayTopology(const WiciParticle*, const WiciParticle*, WiciVertex&, const Double_t&, StLorentzVectorD&, Double_t[]);
		Double_t                GetArmEllipseValue(Double_t, Double_t);
		//Bool_t                  GetTrinDecayTopology(const WiciParticle*, const StLorentzVectorD&, const Double_t&, Double_t[]);
		//void                    AnalyzeKPiPair(const WiciParticle*, const WiciParticle*, Double_t[], const Bool_t);
		//void                    FillMesonPairQA(const Int_t, const WiciParticle*, const WiciParticle*, const Double_t[]);
		//void                    FillMesonTrinQA(const Int_t, const WiciParticle*, const Double_t[]);
		Bool_t                  GetK0s(const WiciParticle*, const WiciParticle*, WiciVertex&, const Double_t&, StLorentzVectorD&, Double_t[]);
		Int_t 		    FindHistoIndex(double pt, double *array, int range);
		//void		    do_D0(vector<StLorentzVectorD> kaon_pos, vector<StLorentzVectorD> kaon_neg, vector<StLorentzVectorD> pion_pos, vector<StLorentzVectorD> pion_neg, bool );
		void		    do_D0 (bool);
		Int_t		Vz_bin(Double_t);
		Int_t		MultRef_bin(Int_t);
		bool            Soft_pion_pt_test(double, double);
		void            track_statistics_pt(bool pion, bool kaon, bool proton, double pt);

		Bool_t Eta_c;
		Bool_t Dstar;

		unsigned int check_triggers;
		vector<int> bad_run_list;
		Int_t f_cond;
		TF1 *fun_lw;
		TF1 *fun_up;

		vector<StLorentzVectorD> v_kaons_pos_mix[10][10][10];
		vector<StLorentzVectorD> v_kaons_neg_mix[10][10][10];
		vector<StLorentzVectorD> v_pions_pos_mix[10][10][10];
		vector<StLorentzVectorD> v_pions_neg_mix[10][10][10];
		int n_mix[10][10];
		vector<int> v_pions_pos_overlap, v_pions_neg_overlap;
		vector<StLorentzVectorD> v_kaons_pos, v_kaons_neg, v_pions_pos, v_pions_neg, v_pions_ht_neg, v_pions_ht_pos;
		vector<float> v_kaons_neg_sigmaDedx, v_kaons_neg_sigmaTof, v_kaons_pos_sigmaDedx, v_kaons_pos_sigmaTof;

		TString fOutFilePrefix;
		TTree* fInputDataset;

		static const Double_t kPdgMass[4];

		//settings
		Bool_t    fMatchToFastDet;
		Bool_t      fUseAltTofPid;
		Bool_t  fNonLinearKaonPid;
		Bool_t    fPtDependentPid;
		Bool_t     fUsePrimaryMom;
		Bool_t      fFindMuVertex;
		Bool_t     fCorellateEles;
		Bool_t     fUseEventMixer;
		Bool_t      fUseHighTower;
		Bool_t     kDoLambda;
		Double_t kInvMassMin;
		Double_t kInvMassMax;

		Bool_t      fPtMaxSet;
		Double_t       fPtMax;

		Bool_t fTPCVpdVtxZSet;
		Double_t fTPCVpdVtxZ;

		Int_t fRefMultMin;
		Bool_t fRefMultMinSet;
		Int_t fRefMultMax;
		Bool_t fRefMultMaxSet;

		Bool_t      fDcaKaonMinSet;
		Double_t       fDcaKaonMin;
		Double_t       fEtaKaonMax;

		Bool_t      fDcaPionMinSet;
		Double_t       fDcaPionMin;

		//D0 mass region
		static const Double_t d0MassRegions[4][3];
		//cuts
		Bool_t       fTrgArraySet;
		unsigned int fTrgArray[9];
		Int_t       fTrgArraySize;

		Bool_t   fAbsVtxPosMaxSet;
		Double_t    fAbsVtxPosMax;

		Int_t        fEmcTreshold;
		Double_t    fTrgPartPtMin;

		Bool_t     fMomKaonMinSet;
		Double_t      fMomKaonMin;
		Bool_t      fPtKaonMinSet;
		Double_t       fPtKaonMin;

		Bool_t fSigDedxKaonMaxSet;
		Double_t  fSigDedxKaonMax;
		Bool_t fSigBtofKaonMaxSet;
		Double_t  fSigBtofKaonMax;

		Bool_t     fMomPionMinSet;
		Double_t      fMomPionMin;
		Bool_t      fPtPionMinSet;
		Double_t       fPtPionMin;

		Bool_t fSigDedxPionMaxSet;
		Double_t  fSigDedxPionMax;
		Bool_t fSigBtofPionMaxSet;
		Double_t  fSigBtofPionMax;

		Bool_t     fMomSoftPionMinSet;
		Double_t      fMomSoftPionMin;
		Bool_t      fPtSoftPionMinSet;
		Double_t       fPtSoftPionMin;

		Bool_t fSigDedxSoftPionMaxSet;
		Double_t  fSigDedxSoftPionMax;
		Bool_t fSigBtofSoftPionMaxSet;
		Double_t  fSigBtofSoftPionMax;

		Bool_t         fDcaMaxSet;
		Double_t          fDcaMax;
		Bool_t    fDecLengthMinSet;
		Double_t     fDecLengthMin;
		Bool_t   fDcaProductMinSet;
		Double_t    fDcaProductMin;
		Bool_t   fDecLengthMaxSet;
		Double_t    fDecLengthMax;
		Bool_t  fDcaProductMaxSet;
		Double_t   fDcaProductMax;
		Double_t    fOpenAngleMax;
		Bool_t   fOpenAngleMaxSet;
		Double_t   fPointAngleMax;
		Bool_t  fPointAngleMaxSet;
		Bool_t fDcaToPrimVtxMaxSet;
		Double_t  fDcaToPrimVtxMax;


		Bool_t     fKaonCosThMaxSet;
		Double_t   fKaonCosThMax;
		Bool_t     fKaonCosThMaxArraySet;
		Double_t** fKaonCosThMaxArray;
		Int_t      fKaonCosThMaxArraySize;

		Double_t   fArmP;
		Double_t   fArmPhi;
		Double_t   fArmZeta;
		Bool_t     fArmEllipseCutSet;
		Double_t   fArmEllipseCut;
		Bool_t     fArmAlphaMinSet;
		Double_t   fArmAlphaMin;
		Bool_t     fArmAlphaMaxSet;
		Double_t   fArmAlphaMax;
		Bool_t     fArmQtMinSet;
		Double_t   fArmQtMin;
		Bool_t     fArmQtMaxSet;
		Double_t   fArmQtMax;

		//usefull enumerators
		enum PART_SPEC {
			ELEC = 0,
			PION = 1,
			KAON = 2,
			PROT = 3
		};

		enum PART_TYPE {
			MATTER      = 0,
			ANTIMATTER  = 1
		};

		enum ELEC_TYPE {
			PE  = 0,
			NPE = 1
		};

		enum PAIR_TYPE {
			OS = 0,
			PP = 1,
			MM = 2,
			N_PAIR_TYPES = 3
		};

		enum TRIN_TYPE {
			RS = 0,
			WS = 1,
			SB = 2,
			N_TRIN_TYPES = 3
		};

		enum CORREL_TYPE {
			OS_0    = 0,
			LS_0    = 1,
			OS_PI   = 2,
			LS_PI   = 3,
			NO_CORR = 4,
			N_CORR_TYPES = 5
		};

		enum ELEC_QA {
			ELEC_RIG          = 0,
			ELEC_RGIT         = 1,
			ELEC_PHI          = 2,
			ELEC_DEDX_RIG     = 3,
			ELEC_DEDX_SIG     = 4,
			ELEC_BTOF_RIG     = 5,
			ELEC_BTOF_SIG     = 6,
			ELEC_ENERGY       = 7,
			ELEC_ARRAY_SIZE   = 8
		};

		enum TOF_EFF {
			KAON_TO_EMC = 0,
			KAON_TO_ALL = 1,
			PION_TO_EMC = 2,
			PION_TO_ALL = 3,
		};

		enum MESON_QA {
			KAON_RIG            =  0,
			PION_RIG            =  1,
			KAON_RIGT           =  2,
			PION_RIGT           =  3,
			KAON_RIGT_TRG       =  4,
			PION_RIGT_TRG       =  5,
			KAON_PHI            =  6,
			PION_PHI            =  7,
			KAON_DEDX_RIG       =  8,
			PION_DEDX_RIG       =  9,
			KAON_SIG_DEDX       = 10,
			PION_SIG_DEDX       = 11,
			KAON_BTOF_RIG       = 12,
			PION_BTOF_RIG       = 13,
			KAON_SIG_BTOF       = 14,
			PION_SIG_BTOF       = 15,

			DCA_BTWN_TRKS       = 16,
			DEC_LENGTH          = 17,
			DCA_PRODUCT         = 18,
			IMP_PARAM_PROD      = 19,
			OPEN_ANGLE          = 20,
			OPEN_ANGLE_PHI      = 21,
			OPEN_ANGLE_TH       = 22,
			POINT_ANGLE_D       = 23,
			POINT_ANGLE_PHI_D   = 24,
			POINT_ANGLE_TH_D    = 25,
			POINT_ANGLE_AD      = 26,
			POINT_ANGLE_PHI_AD  = 27,
			POINT_ANGLE_TH_AD   = 28,
			ANGLE_BTWN_TRKS     = 29,
			KAON_COS_TH_ST_RIG  = 30,
			PION_COS_TH_ST_RIG  = 31,
			KAON_COS_TH_ST_D    = 32,
			PION_COS_TH_ST_D    = 33,
			KAON_COS_TH_ST_AD   = 34,
			PION_COS_TH_ST_AD   = 35,
			KAON_RIG_AT_DCA     = 36,
			PION_RIG_AT_DCA     = 37,

			DEON_P              = 38,
			DEON_PT             = 39,
			DEON_PHI            = 40,

			ANTIDEON_P          = 41,
			ANTIDEON_PT         = 42,
			ANTIDEON_PHI        = 43,

			SOFT_PION_RIG       = 44,
			SOFT_PION_RIGT      = 45,
			SOFT_PION_PHI       = 46,
			SOFT_PION_DEDX_RIG  = 47,
			SOFT_PION_SIG_DEDX  = 48,
			SOFT_PION_BTOF_RIG  = 49,
			SOFT_PION_SIG_BTOF  = 50,

			DEON_INV_MASS       = 51,

			DEXT_P              = 52,
			DEXT_PT             = 53,
			DEXT_PHI            = 54,

			ARMANTEROS            = 55,

			MESON_ARRAY_SIZE    = 56
		};

		TH1D * fMagFiledQA;
		TH1D * fVertexQA;
		TH1D * fRefMultQA;
		TH1D * fVpdVertexQA;
		TH1D * fTPCVpdVertexZ;
		TH1D * fDca;


		TH2I *h_run_refmult;
		TH1*  fElecQA[ELEC_ARRAY_SIZE][2]; //for PEs and NPEs
		TH1*  fTofEfficiency[4];
		TH2D *fParticleDedx;
		TH2D *fADCvsP;
		TH2D *fADCvsPcut;
		TH2D *fADCvsPt;
		TH2D *fADCvsPtCut;
		TH2D *fEnergyVsP;
		TH2D *fEnergyVsPcut;
		TH2D *fEnergyVsPt;
		TH2D *fEnergyVsPtCut;
		TH2D *fKaonSigmaTof;
		TH2D *fKaonSigmaTof_afterDedx;
		TH2D *fKaonSigmaTofCut;
		TH2D *fPionSigmaTof;
		TH2D *fPionSigmaTofCut;

		enum {N_KAON_PT_DEP = 4};
		enum {N_D0_PT_DEP = 8};
		//double pt_dep[2];
		//double *pt_dep;
		double *kaon_pt_dep;
		double *d0_pt_dep;
		TH1D *h_inv_mass_US;
		//TH1D *h_inv_mass_US_pt[N_PT_DEP];
		TH1D *h_inv_mass_US_pt[N_KAON_PT_DEP][N_D0_PT_DEP];
		TH1D *h_inv_mass_US_Kplus;
		TH1D *h_inv_mass_US_Kminus;
		TH2D *h_inv_mass_US_Kplus_vs_pt;
		TH2D *h_inv_mass_US_Kplus_vs_pt_kaon;
		TH2D *h_inv_mass_US_Kplus_vs_pt_pion;
		TH2D *h_inv_mass_US_Kminus_vs_pt;
		TH2D *h_inv_mass_US_Kminus_vs_pt_kaon;
		TH2D *h_inv_mass_US_Kminus_vs_pt_pion;

		TH1D *h_inv_mass_LS_Kplus;
		TH1D *h_inv_mass_LS_Kminus;
		TH2D *h_inv_mass_LS_Kplus_vs_pt;
		TH2D *h_inv_mass_LS_Kplus_vs_pt_kaon;
		TH2D *h_inv_mass_LS_Kplus_vs_pt_pion;
		TH2D *h_inv_mass_LS_Kminus_vs_pt;
		TH2D *h_inv_mass_LS_Kminus_vs_pt_kaon;
		TH2D *h_inv_mass_LS_Kminus_vs_pt_pion;

		TH1D *h_inv_mass_ROT_Kplus;
		TH1D *h_inv_mass_ROT_Kminus;
		TH2D *h_inv_mass_ROT_Kplus_vs_pt;
		TH2D *h_inv_mass_ROT_Kplus_vs_pt_kaon;
		TH2D *h_inv_mass_ROT_Kplus_vs_pt_pion;
		TH2D *h_inv_mass_ROT_Kminus_vs_pt;
		TH2D *h_inv_mass_ROT_Kminus_vs_pt_kaon;
		TH2D *h_inv_mass_ROT_Kminus_vs_pt_pion;

		TH1D *h_inv_mass_LS;
		TH1D *h_inv_mass_LS_pt[N_KAON_PT_DEP][N_D0_PT_DEP];
		TH1D *h_inv_mass_LS_neg;
		TH1D *h_inv_mass_LS_pos;
		TH2D *h_inv_mass_LS_vs_pt;

		TH1D *h_inv_mass_ROT;

		TH1D *h_pt_US;
		TH1D *h_pt_LS;

		TH1D *h_pt_MIX;
		TH1D *h_inv_mass_MIX;
		TH1D *h_inv_mass_MIX_pt[N_KAON_PT_DEP][N_D0_PT_DEP];
		TH1D *h_inv_mass_MIX_Kplus;
		TH1D *h_inv_mass_MIX_Kminus;
		TH2D *h_inv_mass_MIX_Kplus_vs_pt;
		TH2D *h_inv_mass_MIX_Kplus_vs_pt_kaon;
		TH2D *h_inv_mass_MIX_Kplus_vs_pt_pion;
		TH2D *h_inv_mass_MIX_Kminus_vs_pt;
		TH2D *h_inv_mass_MIX_Kminus_vs_pt_kaon;
		TH2D *h_inv_mass_MIX_Kminus_vs_pt_pion;

		TH1I *h_pion_N;
		TH1I *h_pion_N_neg;
		TH1I *h_pion_N_pos;
		TH1D *h_pion_pt;
		TH1D *h_pion_pt_neg;
		TH1D *h_pion_pt_pos;
		TH1D *h_pion_phi_neg;
		TH1D *h_pion_phi_pos;
		TH1D *h_pion_pt_US;
		TH1D *h_pion_pt_LS;
		TH1D *h_pion_pt_MIX;
		TH2D *h_pion_pt_vs_eta;
		TH2D *h_pion_pt_vs_dedx;
		TH2D *h_pion_p_vs_dedx;
		TH2D *h_pion_pt_vs_oneOverBetaTof;
		TH2D *h_pion_p_vs_oneOverBetaTof;
		TH2D *h_pion_pt_vs_sigmaDedx;
		TH2D *h_pion_p_vs_sigmaDedx;
		TH2D *h_pion_pt_vs_sigmaTof;
		TH2D *h_pion_p_vs_sigmaTof;

		TH1I *h_kaon_N;
		TH1I *h_kaon_N_neg;
		TH1I *h_kaon_N_pos;
		TH1D *h_kaon_pt;
		TH1D *h_kaon_pt_neg;
		TH1D *h_kaon_pt_pos;
		TH2D *h_kaon_pt_vs_eta;
		TH2D *h_kaon_pt_vs_dedx;
		TH2D *h_kaon_pt_vs_oneOverBetaTof;
		TH2D *h_kaon_p_vs_dedx;
		TH2D *h_kaon_p_vs_oneOverBetaTof;
		TH2D *h_kaon_pt_vs_sigmaDedx;
		TH2D *h_kaon_pt_vs_sigmaTof;
		TH2D *h_kaon_p_vs_sigmaDedx;
		TH2D *h_kaon_p_vs_sigmaTof;
		TH1D *h_kaon_pt_US;
		TH1D *h_kaon_pt_LS;
		TH1D *h_kaon_pt_MIX;
		TH2D *h_kaon_neg_eta_pt;
		TH1D *h_kaon_neg_eta;
		TH2D *h_kaon_pos_eta_pt;
		TH1D *h_kaon_pos_eta;
		TH1D *h_kaon_neg_phi;
		TH1D *h_kaon_pos_phi;
		TH1D *h_kaon_neg_pt;
		TH1D *h_kaon_pos_pt;

		TH2D *h_kaon_p_vs_dedx_phimeson;
		TH2D *h_kaon_p_vs_oneOverBetaTof_phimeson;
		TH2D *h_kaon_p_vs_sigmaDedx_phimeson;
		TH2D *h_kaon_p_vs_sigmaTof_phimeson;

		TH2D *h_all_pt_vs_dedx;
		TH2D *h_all_pt_vs_oneOverBetaTof;

		TH2D *h_K0s_inv_mass_vs_pt;
		TH1D *h_K0s_phi;
		TH1D *h_K0s_eta;
		TH2D *h_K0sBckg_inv_mass_vs_pt;

		TH2D *h_Eta_c_inv_mass_vs_pt;
		TH2D *h_Eta_c_inv_mass_vs_eta;
		TH1D *h_Eta_c_phi;
		TH1D *h_Eta_c_eta;
		TH1D *h_Eta_c_inv_mass;

		TH1D *h_Eta_c_angle;
		TH1D *h_Eta_c_cos_theta_star_pos;
		TH1D *h_Eta_c_cos_theta_star_neg;

		TH2D *h_KK_inv_mass_vs_pt;
		TH1D *h_KK_phi;
		TH1D *h_KK_eta;
		TH1D *h_KK_pt;
		TH1D *h_KK_inv_mass;
		TH1D *h_KK_minus_inv_mass;
		TH1D *h_KK_plus_inv_mass;

		TH1D *h_KK_angle;
		TH1D *h_KK_cos_theta_star_pos;
		TH1D *h_KK_cos_theta_star_neg;

		TH2D *h_Dstar_RS_vs_pt;
		TH2D *h_Dstar_WS_vs_pt;
		TH2D *h_Dstar_Sideband_vs_pt;
		TH2D *h_Dstar_RS_vs_pt_softtest;
		TH2D *h_Dstar_WS_vs_pt_softtest;
		TH2D *h_Dstar_Sideband_vs_pt_softtest;
		TH2D *h_Dstar_RS_pt_vs_soft_pion_pt;
		TH2D *h_Dstar_WS_pt_vs_soft_pion_pt;
		TH2D *h_pt_D0_vs_kaon_cos_theta_star;

		TH1D *h_track_statistics;
		TH1D *h_track_statistics_pt[6];

		TH1I *h_multi_trigg;
		TH2I *h_multi_trigg_2D;

		TH2D *hTofMult_RefMult;
		TH2D *hTofMult_RefMult_cut;

		TH1F *h_BTofYLocal;
		TH1F *h_BTofYLocal_kaon;
		TH1F *h_BTofYLocal_pion;

		TH2D *hKaonSigmaDedx;
		TH2D *hPionSigmaDedx;

		TH1I *h_N_cuts;

		TH2F *h_VertexXY;
		TH2F *h_DcaXY;
		TH1F *h_DcaZ;
		TH1F *h_Dca_my;
		TH2I *h_zdc_pionN;
		TH2I *h_zdc_pionN_dedx;
		TH2I *h_zdc_refmult;
		TH2I *h_bbc_refmult;
		TH2I *h_bbc_pionN;
		TH2I *h_bbc_kaonN;
		TH2I *h_bbc_pionN_dedx;


		TH2F *h_inv_mass_DS_plus_vs_pt;
		TH2F *h_inv_mass_DS_minus_vs_pt;

		TH2F *h_KK_inv_mass_vs_pt_2gev_us;
		TH2F *h_KK_inv_mass_vs_pt_2gev_lsp; 
		TH2F *h_KK_inv_mass_vs_pt_2gev_lsm; 

		TFile *output_file;
		TTree *t_output;
		Int_t RefMult;
		Int_t RunId;
		Int_t EventId;
		vector<float> *inv_mass_us;
		vector<float> *pt_kaon_us;
		vector<float> *pt_pion_us;
		vector<float> *pt_D0_us;
		vector<float> *inv_mass_ls;
		vector<float> *pt_kaon_ls;
		vector<float> *pt_pion_ls;
		vector<float> *pt_D0_ls;
		vector<float> *inv_mass_rot;

		/*
		unsigned int t410008;
		unsigned int t410102;
		unsigned int t410112;
		*/
		unsigned int t32;
		unsigned int t470202;

		TFile *f_hot_towers;
		TH1I *h_hot_towers;

		ClassDef(StWiciED0CorrLooper,2)
};
