#ifndef WICI_PARTICLE_H
#define WICI_PARTICLE_H

#include <TObject.h>
#include <TVector3.h>
//#include <StEvent/StDcaGeometry.h>
#include <TMath.h>

class WiciParticle : public TObject {
  public:
    WiciParticle();
    WiciParticle(WiciParticle*);
    virtual ~WiciParticle();
    WiciParticle& operator=(WiciParticle&);
    
    inline Short_t              GetID() const                                     {return fID;};
    void                        SetID(Short_t id)                                 {fID=id;};
    inline Short_t              GetCharge() const                                 {return fCharge;};
    void                        SetCharge(Short_t charge)                         {fCharge=charge;};
    
    /*
    inline Float_t              GetGlobalP()                                      {if(fGlobalP<0) fGlobalP = GetGlobalPVector().Mag();
                                                                                   return fGlobalP;};
    void                        SetGlobalP(Float_t pX, Float_t pY, Float_t pZ)    {fGlobalPVector.SetXYZ(pX, pY, pZ);
                                                                                   fGlobalEta=fGlobalPhi=fGlobalPT=fGlobalP=-9999;};
    inline Float_t              GetGlobalPT()                                     {if(fGlobalPT<0) fGlobalPT=GetGlobalPVector().Pt();
                                                                                   return fGlobalPT;};
    inline const TVector3&      GetGlobalPVector() const                          {return fGlobalPVector;};
    inline Float_t              GetGlobalPX() const                               {return GetGlobalPVector().X();};
    inline Float_t              GetGlobalPY() const                               {return GetGlobalPVector().Y();};
    inline Float_t              GetGlobalPZ() const                               {return GetGlobalPVector().Z();};
    inline Float_t              GetGlobalEta()                                    {if(fGlobalEta<-5) fGlobalEta=GetGlobalPVector().Eta();
    inline Float_t              GetGlobalPhi()                                    {if(fGlobalPhi<-TMath::Pi()) fGlobalPhi=GetGlobalPVector().Phi();
                                                                                   return fGlobalPhi;};
										   */
    
    inline Float_t              GetPrimaryP()                                      {if(fPrimaryP<0) fPrimaryP = GetPrimaryPVector().Mag();
                                                                                    return fPrimaryP;};
    void                        SetPrimaryP(Float_t pX, Float_t pY, Float_t pZ)    {fPrimaryPVector.SetXYZ(pX, pY, pZ);
                                                                                    fPrimaryEta=fPrimaryPhi=fPrimaryPT=fPrimaryP=-9999;};
    inline Float_t              GetPrimaryPT()                                     {if(fPrimaryPT<0) fPrimaryPT=GetPrimaryPVector().Pt();
                                                                                    return fPrimaryPT;};
    inline const TVector3&      GetPrimaryPVector() const                          {return fPrimaryPVector;};
    inline Float_t              GetPrimaryPX() const                               {return GetPrimaryPVector().X();};
    inline Float_t              GetPrimaryPY() const                               {return GetPrimaryPVector().Y();};
    inline Float_t              GetPrimaryPZ() const                               {return GetPrimaryPVector().Z();};
    inline Float_t              GetPrimaryEta()                                    {if(fPrimaryEta<-5) fPrimaryEta=GetPrimaryPVector().Eta();
                                                                                   return fPrimaryEta;};
    inline Float_t              GetPrimaryPhi()                                    {if(fPrimaryPhi<-TMath::Pi()) fPrimaryPhi=GetPrimaryPVector().Phi();
                                                                                    return fPrimaryPhi;};
    
										    
/*
    inline const StDcaGeometry& GetCovMatrix() const                              {return fCovMatrix;};
    void                        SetCovMatrix(const StDcaGeometry& covMatrix)      {fCovMatrix.set(covMatrix.params(), covMatrix.errMatrix());};
    */
    
    inline Float_t              GetDca()                                          {if(fDca<0) fDca = GetDcaVector().Mag();
                                                                                   return fDca;};
    void                        SetDca(Float_t dcaX, Float_t dcaY, Float_t dcaZ)  {fDcaVector.SetXYZ(dcaX, dcaY, dcaZ); fDca=-9999;};
    inline const TVector3&      GetDcaVector() const                              {return fDcaVector;};
    inline Float_t              GetDcaX() const                                   {return GetDcaVector().X();};
    inline Float_t              GetDcaY() const                                   {return GetDcaVector().Y();};
    inline Float_t              GetDcaZ() const                                   {return GetDcaVector().Z();};
    inline Float_t              GetDcaD() const                                   {return fDcaD;};
    void                        SetDcaD(Float_t dcaD)                             {fDcaD=dcaD;};
    /*
    inline Float_t              GetSigmaDcaX() const                              {return fSigmaDcaX;};
    inline Float_t              GetSigmaDcaY() const                              {return fSigmaDcaY;};
    inline Float_t              GetSigmaDcaZ() const                              {return fSigmaDcaZ;};
    inline Float_t              GetSigmaDcaD() const                              {return fSigmaDcaD;};
    */
    
    inline Float_t              GetDedx() const                                   {return fDedx;};
    void                        SetDedx(Double_t dedx)                            {fDedx=dedx;};
    inline Float_t              GetSigmaDedxElectron() const                      {return fSigmaDedxElectron;};
    void                        SetSigmaDedxElectron(Double_t sigmaDedxElectron)  {fSigmaDedxElectron=sigmaDedxElectron;};
    inline Float_t              GetSigmaDedxPion() const                          {return fSigmaDedxPion;};
    void                        SetSigmaDedxPion(Double_t sigmaDedxPion)          {fSigmaDedxPion=sigmaDedxPion;};
    inline Float_t              GetSigmaDedxKaon() const                          {return fSigmaDedxKaon;};
    void                        SetSigmaDedxKaon(Double_t sigmaDedxKaon)          {fSigmaDedxKaon=sigmaDedxKaon;};
    inline Float_t              GetSigmaDedxProton() const                        {return fSigmaDedxProton;};
    void                        SetSigmaDedxProton(Double_t sigmaDedxProton)      {fSigmaDedxProton=sigmaDedxProton;};
    
    inline Bool_t               GetTofMatchFlag() const                           {return fTofMatchFlag;};
    void                        SetTofMatchFlag(Bool_t flag)                      {fTofMatchFlag=flag;};
    inline Float_t              GetBetaTof() const                                {return fBetaTof;};
    void                        SetBetaTof(Double_t betaTof)                      {fBetaTof=betaTof;};
    inline Float_t              GetSigmaTofElectron() const                       {return fSigmaTofElectron;};
    void                        SetSigmaTofElectron(Double_t sigmaTofElectron)    {fSigmaTofElectron=sigmaTofElectron;};
    inline Float_t              GetSigmaTofPion() const                           {return fSigmaTofPion;};
    void                        SetSigmaTofPion(Double_t sigmaTofPion)            {fSigmaTofPion=sigmaTofPion;};
    inline Float_t              GetSigmaTofKaon() const                           {return fSigmaTofKaon;};
    void                        SetSigmaTofKaon(Double_t sigmaTofKaon)            {fSigmaTofKaon=sigmaTofKaon;};
    inline Float_t              GetSigmaTofProton() const                         {return fSigmaTofProton;};
    void                        SetSigmaTofProton(Double_t sigmaTofProton)        {fSigmaTofProton=sigmaTofProton;};
    
    inline UShort_t             GetNFitPoints() const                             {return fNFitPoints;};
    void                        SetNFitPoints(UShort_t nFitPoints)                {fNFitPoints=nFitPoints; fNPointsRatio=-9999;};
    inline UShort_t             GetNMaxPoints() const                             {return fNMaxPoints;};
    void                        SetNMaxPoints(UShort_t nMaxPoints)                {fNMaxPoints=nMaxPoints; fNPointsRatio=-9999;};
    Float_t                     GetPointsRatio()                                  {if(fNPointsRatio<0) fNPointsRatio = fNMaxPoints!=0 ? (Float_t)fNFitPoints/(Float_t)fNMaxPoints : -9999;
                                                                                   return fNPointsRatio;};

	inline Float_t GetNHitsDedx() const {return fNHitsDedx;};
	void SetNHitsDedx(Float_t NHitsDedx) {fNHitsDedx = NHitsDedx;};

    inline Bool_t               GetBemcMatchFlag() const                          {return fBemcMatchFlag;};
    void                        SetBemcMatchFlag(Bool_t flag)                     {fBemcMatchFlag=flag;};
    inline Int_t                GetBemcMatchTowerID() const                       {return fBemcMatchTowerID;};
    void                        SetBemcMatchTowerID(Int_t towID)                  {fBemcMatchTowerID=towID;};
    inline Int_t                GetBemcMatchTowerADC() const                      {return fBemcMatchTowerADC;};
    void                        SetBemcMatchTowerADC(Int_t towADC)                {fBemcMatchTowerADC=towADC;};
    inline Float_t              GetBemcMatchTowerEnergy() const                   {return fBemcMatchTowerEnergy;};
    void                        SetBemcMatchTowerEnergy(Float_t towEnergy)        {fBemcMatchTowerEnergy=towEnergy;};
    inline Int_t                GetBemcHighTowerID() const                        {return fBemcHighTowerID;};
    void                        SetBemcHighTowerID(Int_t towID)                   {fBemcHighTowerID=towID;};
    inline Int_t                GetBemcHighTowerADC() const                       {return fBemcHighTowerADC;};
    void                        SetBemcHighTowerADC(Int_t towADC)                 {fBemcHighTowerADC=towADC;};
    inline Float_t              GetBemcHighTowerEnergy() const                    {return fBemcHighTowerEnergy;};
    void                        SetBemcHighTowerEnergy(Float_t towEnergy)         {fBemcHighTowerEnergy=towEnergy;};
  //Uchar_t*                    GetNSiPoints() const                              {return fNSiPoints;};
  //void                        SetNSiPoints(Uchar_t* nSiPoints)                  {for(Int_t i=0; i<4; i++) fNSiPoints[i]=nSiPoints[i];};
  //Uchar_t                     GetNSiPoints(Int_t layer) const                   {return fNSiPoints[layer];};
  //void                        SetNSiPoints(Int_t layer, UChar_t value)          {if(layer>=0 && layer<4)fNSiPoints[layer]=value;};
    
    Short_t                     GetRecoFlag() const                               {return fRecoFlag;};
    void                        SetRecoFlag(Short_t recoFlag)                     {fRecoFlag=recoFlag;};

    float			GetBTofYLocal()					{return fBTofYLocal;};
    void			SetBTofYLocal(float BTofYLocal)			{fBTofYLocal = BTofYLocal;};
    
  protected:
	float	fBTofYLocal;

    Short_t       fID;
    Short_t       fCharge;
    
    /*
    TVector3      fGlobalPVector;   //momentum Vector
    Float_t       fGlobalP;         //! absolute momentum value
    Float_t       fGlobalPT;        //! absolute transverse momentum value
    Float_t       fGlobalEta;       //!
    Float_t       fGlobalPhi;       //!
    */
    
    TVector3      fPrimaryPVector;  //momentum Vector
    Float_t       fPrimaryP;        //! absolute momentum value
    Float_t       fPrimaryPT;       //! absolute transverse momentum value
    Float_t       fPrimaryEta;      //!
    Float_t       fPrimaryPhi;      //!
    
    //StDcaGeometry fCovMatrix;
    
    TVector3      fDcaVector;         //DCA vector
    Float_t       fDca;               //! absolute DCA value
    Float_t       fDcaD;              //signed radial component of global DCA (projected)
    /*
    Float_t       fSigmaDcaX;         //!
    Float_t       fSigmaDcaY;         //!
    Float_t       fSigmaDcaZ;         //!
    Float_t       fSigmaDcaD;         //!
    */
    
    
    Float_t       fDedx;
    Float_t       fSigmaDedxElectron;
    Float_t       fSigmaDedxPion;
    Float_t       fSigmaDedxKaon;
    Float_t       fSigmaDedxProton;
    
    Int_t         fTofMatchFlag;
    Float_t       fBetaTof;
    Float_t       fSigmaTofElectron;
    Float_t       fSigmaTofPion;
    Float_t       fSigmaTofKaon;
    Float_t       fSigmaTofProton;
    
    UShort_t      fNFitPoints;            //total number of points (Si+TPC) used by the fitter
    UShort_t      fNMaxPoints;            //number of possible points on the track
    Float_t       fNPointsRatio;          //! fNFitPoints/fNMaxPoints
    
    Float_t	 fNHitsDedx;
    
    Bool_t        fBemcMatchFlag;         //kTRUE when there is a matching tower with a signal, kFALSE otherwise
    Int_t         fBemcMatchTowerID;      //ID of the tower that track matches to
    Int_t         fBemcMatchTowerADC;     //ADC of that tower
    Float_t       fBemcMatchTowerEnergy;
    Int_t         fBemcHighTowerID;       //ID of the highest tower among the neighbors
    Int_t         fBemcHighTowerADC;      //ADC of that tower
    Float_t       fBemcHighTowerEnergy;
  //UChar_t       fNSiPoints[4];          //number of points in the n-th layer of Silicon
    
    Short_t       fRecoFlag;              //flag of the track asigned durig the reconstruction process
    
    ClassDef(WiciParticle,2)
};

#endif

