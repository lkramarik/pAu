#include "WiciLepton.h"
#include <iostream>

using namespace std;

ClassImp(WiciLepton)
//__________________________________________________________________________________________________
WiciLepton::WiciLepton():WiciParticle() {
  //cout << "WiciLepton:DefaultConstructor" << endl;
  fCloseTowerID       = -9999;
  fCloseTowerEnergy   = -9999;
  fCloseTowerTrigger  = kFALSE;
  fCloseTowerDist     =  9999;
  fCloseTowerDistPhi  =  9999;
  fCloseTowerDistZ    =  9999;
  
  fHighTowerID        = -9999;
  fHighTowerEnergy    = -9999;
  fHighTowerTrigger   = kFALSE;
  fHighTowerDist      =  9999;
  fHighTowerDistPhi   =  9999;
  fHighTowerDistZ     =  9999;
  
  fSmdeHits           = -9999;
  fSmdeDist           =  9999;
  fSmdeDistPhi        =  9999;
  fSmdeDistZ          =  9999;
  
  fSmdpHits           = -9999;
  fSmdpDist           =  9999;
  fSmdpDistPhi        =  9999;
  fSmdpDistZ          =  9999;
  
  fPhotonic           = 0;
}
//__________________________________________________________________________________________________
WiciLepton::WiciLepton(WiciLepton* lepton):WiciParticle(lepton) {
  fCloseTowerID       = lepton->GetCloseTowerID();
  fCloseTowerEnergy   = lepton->GetCloseTowerEnergy();
  fCloseTowerTrigger  = lepton->IsCloseTowerTrigger();
  fCloseTowerDist     = lepton->GetCloseTowerDist();
  fCloseTowerDistPhi  = lepton->GetCloseTowerDistPhi();
  fCloseTowerDistZ    = lepton->GetCloseTowerDistZ();
  
  fHighTowerID        = lepton->GetHighTowerID();
  fHighTowerEnergy    = lepton->GetHighTowerEnergy();
  fHighTowerTrigger   = lepton->IsHighTowerTrigger();
  fHighTowerDist      = lepton->GetHighTowerDist();
  fHighTowerDistPhi   = lepton->GetHighTowerDistPhi();
  fHighTowerDistZ     = lepton->GetHighTowerDistZ();
  
  fSmdeHits           = lepton->GetSmdeHits();
  fSmdeDist           = lepton->GetSmdeDist();
  fSmdeDistPhi        = lepton->GetSmdeDistPhi();
  fSmdeDistZ          = lepton->GetSmdeDistZ();
  
  fSmdpHits           = lepton->GetSmdpHits();
  fSmdpDist           = lepton->GetSmdpDist();
  fSmdpDistPhi        = lepton->GetSmdpDistPhi();
  fSmdpDistZ          = lepton->GetSmdpDistZ();
  
  fPhotonic           = lepton->IsPhotonic();
}
//__________________________________________________________________________________________________
WiciLepton::WiciLepton(WiciParticle* particle):WiciParticle(particle) {
  fCloseTowerID       = -9999;
  fCloseTowerEnergy   = -9999;
  fCloseTowerTrigger  = kFALSE;
  fCloseTowerDist     =  9999;
  fCloseTowerDistPhi  =  9999;
  fCloseTowerDistZ    =  9999;
  
  fHighTowerID        = -9999;
  fHighTowerEnergy    = -9999;
  fHighTowerTrigger   = kFALSE;
  fHighTowerDist      =  9999;
  fHighTowerDistPhi   =  9999;
  fHighTowerDistZ     =  9999;
  
  fSmdeHits           = -9999;
  fSmdeDist           =  9999;
  fSmdeDistPhi        =  9999;
  fSmdeDistZ          =  9999;
  
  fSmdpHits           = -9999;
  fSmdpDist           =  9999;
  fSmdpDistPhi        =  9999;
  fSmdpDistZ          =  9999;
  
  fPhotonic           = 0;
}
//__________________________________________________________________________________________________
WiciLepton::~WiciLepton() {
  //cout << "WiciLepton:Destructor" << endl;
  
}
//__________________________________________________________________________________________________
WiciLepton& WiciLepton::operator=(WiciLepton& lepton) {
  WiciParticle::operator=(lepton); //letting WiciParticle do it's part
  
  fCloseTowerID       = lepton.GetCloseTowerID();
  fCloseTowerEnergy   = lepton.GetCloseTowerEnergy();
  fCloseTowerTrigger  = lepton.IsCloseTowerTrigger();
  fCloseTowerDist     = lepton.GetCloseTowerDist();
  fCloseTowerDistPhi  = lepton.GetCloseTowerDistPhi();
  fCloseTowerDistZ    = lepton.GetCloseTowerDistZ();
  
  fHighTowerID        = lepton.GetHighTowerID();
  fHighTowerEnergy    = lepton.GetHighTowerEnergy();
  fHighTowerTrigger   = lepton.IsHighTowerTrigger();
  fHighTowerDist      = lepton.GetHighTowerDist();
  fHighTowerDistPhi   = lepton.GetHighTowerDistPhi();
  fHighTowerDistZ     = lepton.GetHighTowerDistZ();
  
  fSmdeHits           = lepton.GetSmdeHits();
  fSmdeDist           = lepton.GetSmdeDist();
  fSmdeDistPhi        = lepton.GetSmdeDistPhi();
  fSmdeDistZ          = lepton.GetSmdeDistZ();
  
  fSmdpHits           = lepton.GetSmdpHits();
  fSmdpDist           = lepton.GetSmdpDist();
  fSmdpDistPhi        = lepton.GetSmdpDistPhi();
  fSmdpDistZ          = lepton.GetSmdpDistZ();
  
  fPhotonic           = lepton.IsPhotonic();
  
  return *this;
}

