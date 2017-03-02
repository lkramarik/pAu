#include "WiciParticle.h"
#include <iostream>

//using namespace std;

ClassImp(WiciParticle)
//__________________________________________________________________________________________________
WiciParticle::WiciParticle():TObject() {
  //cout << "WiciParticle:DefaultConstructor" << endl;

	fBTofYLocal = -9999.;

  fID                 = -9999;
  
  fCharge             = -9999;
  
  /*
  fGlobalP            = -9999;
  fGlobalPT           = -9999;
  fGlobalEta          = -9999;
  fGlobalPhi          = -9999;
  */
  
  fPrimaryP           = -9999;
  fPrimaryPT          = -9999;
  fPrimaryEta         = -9999;
  fPrimaryPhi         = -9999;
  
  //fCovMatrix = new StDcaGeometry();
  
  fDca                  = -9999;
  fDcaD                 = -9999;
  /*
  fSigmaDcaX            = -9999;
  fSigmaDcaY            = -9999;
  fSigmaDcaZ            = -9999;
  fSigmaDcaD            = -9999;
  */
  
  fDedx                 = -9999;
  fSigmaDedxElectron    = -9999;
  fSigmaDedxPion        = -9999;
  fSigmaDedxKaon        = -9999;
  fSigmaDedxProton      = -9999;
  
  fTofMatchFlag         = -9999;
  fBetaTof              = -9999;
  fSigmaTofElectron     = -9999;
  fSigmaTofPion         = -9999;
  fSigmaTofKaon         = -9999;
  fSigmaTofProton       = -9999;
  
  fNFitPoints           = -9999;
  fNMaxPoints           = -9999;
  fNPointsRatio         = -9999;
  fBemcMatchFlag        = kFALSE;
  
  fNHitsDedx = -9999;
  fBemcMatchTowerID     = -9999;
  fBemcMatchTowerADC    = -9999;
  fBemcMatchTowerEnergy = -9999;
  fBemcHighTowerID      = -9999;
  fBemcHighTowerADC     = -9999;
  fBemcHighTowerEnergy  = -9999;
  
  //for(Int_t i=0; i<4; i++) fNSiPoints[i] = -9999;
  
  fRecoFlag             = -9999;
}
//__________________________________________________________________________________________________
WiciParticle::WiciParticle(WiciParticle* particle) {
	fBTofYLocal = particle->GetBTofYLocal();
  fID                   = particle->GetID();
  
  fCharge               = particle->GetCharge();
  
  /*
  fGlobalPVector        = particle->GetGlobalPVector();
  fGlobalP              = -9999;
  fGlobalPT             = -9999;
  fGlobalEta            = -9999;
  fGlobalPhi            = -9999;
  */

  fPrimaryPVector       = particle->GetPrimaryPVector();
  fPrimaryP             = -9999;
  fPrimaryPT            = -9999;
  fPrimaryEta           = -9999;
  fPrimaryPhi           = -9999;
  
  ////fCovMatrix = new StDcaGeometry();
  ////fCovMatrix->set(particle->GetCovMatrix()->params(), particle->GetCovMatrix()->errMatrix());
  //SetCovMatrix(particle->GetCovMatrix());
  
  fDca                  = -9999;
  fDcaVector            = particle->GetDcaVector();
  fDcaD                 = particle->GetDcaD();
  /*
  fSigmaDcaX            = -9999;
  fSigmaDcaY            = -9999;
  fSigmaDcaZ            = -9999;
  fSigmaDcaD            = -9999;
  */
  
  fDedx                 = particle->GetDedx();
  fSigmaDedxElectron    = particle->GetSigmaDedxElectron();
  fSigmaDedxPion        = particle->GetSigmaDedxPion();
  fSigmaDedxKaon        = particle->GetSigmaDedxKaon();
  fSigmaDedxProton      = particle->GetSigmaDedxProton();
  
  fTofMatchFlag         = particle->GetTofMatchFlag();
  fBetaTof              = particle->GetBetaTof();
  fSigmaTofElectron     = particle->GetSigmaTofElectron();
  fSigmaTofPion         = particle->GetSigmaTofPion();
  fSigmaTofKaon         = particle->GetSigmaTofKaon();
  fSigmaTofProton       = particle->GetSigmaTofProton();
  
  fNFitPoints           = particle->GetNFitPoints();
  fNMaxPoints           = particle->GetNMaxPoints();
  fNPointsRatio         = -9999;
  fNHitsDedx = particle->GetNHitsDedx();

  fBemcMatchFlag        = particle->GetBemcMatchFlag();
  fBemcMatchTowerID     = particle->GetBemcMatchTowerID();
  fBemcMatchTowerADC    = particle->GetBemcMatchTowerADC();
  fBemcMatchTowerEnergy = particle->GetBemcMatchTowerEnergy();
  fBemcHighTowerID      = particle->GetBemcHighTowerID();
  fBemcHighTowerADC     = particle->GetBemcHighTowerADC();
  fBemcHighTowerEnergy  = particle->GetBemcHighTowerEnergy();
  
  //for(Int_t i=0; i<4; i++) fNSiPoints[i] = particle->GetNSiPoints(i);
  
  fRecoFlag             = particle->GetRecoFlag();
}
//__________________________________________________________________________________________________
WiciParticle::~WiciParticle() {
  //cout << "WiciParticle:Destructor" << endl;
  //delete fCovMatrix;
}
//__________________________________________________________________________________________________
WiciParticle& WiciParticle::operator=(WiciParticle& particle) {
	fBTofYLocal = particle.GetBTofYLocal();
  fID                   = particle.GetID();
  
  fCharge               = particle.GetCharge();
  
  /*
  fGlobalPVector        = particle.GetGlobalPVector();
  fGlobalP              = -9999;
  fGlobalPT             = -9999;
  fGlobalEta            = -9999;
  fGlobalPhi            = -9999;
  */
  
  fPrimaryPVector       = particle.GetPrimaryPVector();
  fPrimaryP             = -9999;
  fPrimaryPT            = -9999;
  fPrimaryEta           = -9999;
  fPrimaryPhi           = -9999;
  
  ////fCovMatrix->set(particle.GetCovMatrix()->params(), particle.GetCovMatrix()->errMatrix());
  //SetCovMatrix(particle.GetCovMatrix());
  
  fDca                  = -9999;
  fDcaVector            = particle.GetDcaVector();
  fDcaD                 = particle.GetDcaD();
  /*
  fSigmaDcaX            = -9999;
  fSigmaDcaY            = -9999;
  fSigmaDcaZ            = -9999;
  fSigmaDcaD            = -9999;
  */

  fDedx                 = particle.GetDedx();
  fSigmaDedxElectron    = particle.GetSigmaDedxElectron();
  fSigmaDedxPion        = particle.GetSigmaDedxPion();
  fSigmaDedxKaon        = particle.GetSigmaDedxKaon();
  fSigmaDedxProton      = particle.GetSigmaDedxProton();
  
  fTofMatchFlag         = particle.GetTofMatchFlag();
  fBetaTof              = particle.GetBetaTof();
  fSigmaTofElectron     = particle.GetSigmaTofElectron();
  fSigmaTofPion         = particle.GetSigmaTofPion();
  fSigmaTofKaon         = particle.GetSigmaTofKaon();
  fSigmaTofProton       = particle.GetSigmaTofProton();
  
  fNFitPoints           = particle.GetNFitPoints();
  fNMaxPoints           = particle.GetNMaxPoints();
  fNPointsRatio         = -9999;
  fBemcMatchFlag        = particle.GetBemcMatchFlag();

  fNHitsDedx = particle.GetNHitsDedx();
  fBemcMatchTowerID     = particle.GetBemcMatchTowerID();
  fBemcMatchTowerADC    = particle.GetBemcMatchTowerADC();
  fBemcMatchTowerEnergy = particle.GetBemcMatchTowerEnergy();
  fBemcHighTowerID      = particle.GetBemcHighTowerID();
  fBemcHighTowerADC     = particle.GetBemcHighTowerADC();
  fBemcHighTowerEnergy  = particle.GetBemcHighTowerEnergy();
  
  //for(Int_t i=0; i<4; i++) fNSiPoints[i] = particle.GetNSiPoints(i);
  
  fRecoFlag           = particle.GetRecoFlag();
  
  return *this;
}

