#include "WiciEvent.h"
#include "WiciVertex.h"
#include "WiciParticle.h"
//#include "WiciLepton.h"
#include <TClonesArray.h>
//#include <iostream>

//using namespace std;

ClassImp(WiciEvent)
//__________________________________________________________________________________________________
WiciEvent::WiciEvent(Bool_t bookArrays):TObject() {
  //cout << "WiciEvent:DefaultConstructor" << endl;
  fRunID          = -9999;
  fEventID        = -9999;
  
  fTriggers.clear();
  
  fVertex.SetIndex( -9999);
  fVertex.SetX(     -9999);
  fVertex.SetY(     -9999);
  fVertex.SetZ(     -9999);
  fVertex.SetRank(  -9999);
  fVpdVtxZ        = -9999;
  
  fMagField       = -9999;
  fRefMult        = -9999;
  fTofMult        = -9999;
  fNGlobals       = -9999;
  fNPrimaries     = -9999;
  fBinaryTriggers = 0;
  fZDCRate = -999;
  fBBCRate = -999;

  if(bookArrays==kTRUE) {
    fPosParticles =  new TClonesArray("WiciParticle", 500);
    fNegParticles =  new TClonesArray("WiciParticle", 500);
    //fElectrons    =  new TClonesArray("WiciLepton",   100);
  }
  else {
    fPosParticles = NULL;
    fNegParticles = NULL;
    //fElectrons    = NULL;
  }
}
//__________________________________________________________________________________________________
WiciEvent::WiciEvent(WiciEvent* event) {
  fRunID        = event->GetRunID();
  fEventID      = event->GetEventID();
  
  fTriggers     = event->GetTriggers();
  fBinaryTriggers = event->GetBinaryTriggers();
  
  fVertex       = event->GetVertex();
  fVpdVtxZ      = event->GetVpdVtxZ();
  
  fMagField     = event->GetMagField();
  fRefMult      = event->GetRefMult();
  fTofMult      = event->GetTofMult();
  fNGlobals     = event->GetNGlobals();
  fNPrimaries   = event->GetNPrimaries();
  fZDCRate = event->GetZDCRate();
  fBBCRate = event->GetBBCRate();
  
  fPosParticles = (TClonesArray*) event->GetPosParticles()->Clone();
  fNegParticles = (TClonesArray*) event->GetNegParticles()->Clone();
  //fElectrons    = (TClonesArray*) event->GetElectrons()->Clone();
}
//__________________________________________________________________________________________________
WiciEvent::~WiciEvent() {
  //cout << "WiciEvent:Destructor" << endl;
  Clear();
}
//__________________________________________________________________________________________________
void WiciEvent::Clear(const Option_t* opt) {
  //cout << "WiciEvent:Clear" << endl;
  fRunID          = -9999;
  fEventID        = -9999;
  fTriggers.clear();
  fVertex.SetIndex( -9999);
  fVertex.SetX(     -9999);
  fVertex.SetY(     -9999);
  fVertex.SetZ(     -9999);
  fVertex.SetRank(  -9999);
  fVpdVtxZ        = -9999;
  fMagField       = -9999;
  fRefMult        = -9999;
  fTofMult        = -9999;
  fNGlobals       = -9999;
  fNPrimaries     = -9999;
  fBinaryTriggers = 0;
  fZDCRate = -999;
  fBBCRate = -999;
  
  if(fPosParticles) {
    fPosParticles->SetOwner(kFALSE);
    fPosParticles->Clear();
    fPosParticles->Delete();
    delete fPosParticles;
    fPosParticles = NULL;
  }
  if(fNegParticles) {
    fNegParticles->SetOwner(kFALSE);
    fNegParticles->Clear();
    fNegParticles->Delete();
    delete fNegParticles;
    fNegParticles = NULL;
  }
  /*
  if(fElectrons) {
    fElectrons->SetOwner(kFALSE);
    fElectrons->Clear();
    fElectrons->Delete();
    delete fElectrons;
    fElectrons = NULL;
  }
  */
}
//__________________________________________________________________________________________________
WiciEvent& WiciEvent::operator=(WiciEvent& event) {
  Clear();
  fRunID        = event.GetRunID();
  fEventID      = event.GetEventID();
  
  fTriggers     = event.GetTriggers();
  
  fVertex       = event.GetVertex();
  fVpdVtxZ      = event.GetVpdVtxZ();
  
  fMagField     = event.GetMagField();
  fRefMult      = event.GetRefMult();
  fTofMult      = event.GetTofMult();
  fNGlobals     = event.GetNGlobals();
  fNPrimaries   = event.GetNPrimaries();
  
  fZDCRate = event.GetZDCRate();
  fBBCRate = event.GetBBCRate();
  
  fPosParticles = (TClonesArray*) event.GetPosParticles()->Clone();
  fNegParticles = (TClonesArray*) event.GetNegParticles()->Clone();
  //fElectrons    = (TClonesArray*) event.GetElectrons()->Clone();
  
  return *this;
}
//__________________________________________________________________________________________________
void WiciEvent::Reset() {
  fRunID          = -9999;
  fEventID        = -9999;
  
  fTriggers.clear();
  
  fVertex.SetIndex( -9999);
  fVertex.SetX(     -9999);
  fVertex.SetY(     -9999);
  fVertex.SetZ(     -9999);
  fVertex.SetRank(  -9999);
  fVpdVtxZ        = -9999;
  
  fMagField       = -9999;
  fRefMult        = -9999;
  fTofMult        = -9999;
  fNGlobals       = -9999;
  fNPrimaries     = -9999;
  fBinaryTriggers = 0;
  fZDCRate = -999;
  fBBCRate = -999;
  
  //fPosParticles->Clear();
  fPosParticles->Delete();
  //fNegParticles->Clear();
  fNegParticles->Delete();
  //fElectrons->Clear();
}
//__________________________________________________________________________________________________
void WiciEvent::AddParticle(WiciParticle* particle) {
  if(particle->GetCharge()>0) new ( (*fPosParticles)[fPosParticles->GetEntries()] ) WiciParticle(particle);
  else                        new ( (*fNegParticles)[fNegParticles->GetEntries()] ) WiciParticle(particle);
}
/*
//__________________________________________________________________________________________________
void WiciEvent::AddElectron(WiciLepton* electron) {
  new ( (*fElectrons)[fElectrons->GetEntries()] ) WiciLepton(electron);
}
*/
