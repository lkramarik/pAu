#ifndef WICIEVENT_H
#define WICIEVENT_H

#include <TObject.h>
#include <TObjArray.h>
#include "WiciEvent/WiciVertex.h"
#include <TClonesArray.h>

class WiciParticle;

using namespace std;

class WiciEvent : public TObject {
  public:
    WiciEvent(Bool_t=kTRUE);
    WiciEvent(WiciEvent*);
    virtual ~WiciEvent();
    virtual void Clear(const Option_t* = "");
    WiciEvent& operator=(WiciEvent&);
    
    void Reset();
    
    inline Int_t          GetRunID()                                {return fRunID;};
    void                  SetRunID(Int_t runID)                     {fRunID=runID;};
    inline Int_t          GetEventID()                              {return fEventID;};
    void                  SetEventID(Int_t eventID)                 {fEventID=eventID;};
    
    vector<unsigned int>& GetTriggers()                                     {return fTriggers;};
    void                  SetTriggers(const vector<unsigned int>& triggers) {fTriggers=triggers;};
    unsigned int GetBinaryTriggers()                                     {return fBinaryTriggers;};
    void                  SetBinaryTriggers(const unsigned int triggers) {fBinaryTriggers=triggers;};
    
    inline WiciVertex&    GetVertex()                               {return fVertex;};
    void                  SetVertex(WiciVertex& vertex)             {fVertex=vertex;};
    inline Float_t        GetVpdVtxZ()                              {return fVpdVtxZ;};
    void                  SetVpdVtxZ(Float_t vpdVtxZ)               {fVpdVtxZ=vpdVtxZ;};
    
    inline Float_t        GetMagField()                             {return fMagField;};
    void                  SetMagField(Float_t magField)             {fMagField=magField;};
    inline Int_t          GetRefMult()                              {return fRefMult;};
    void                  SetRefMult(Int_t refMult)                 {fRefMult=refMult;};
    inline Int_t          GetTofMult()                              {return fTofMult;};
    void                  SetTofMult(Int_t tofMult)                 {fTofMult=tofMult;};
    inline Int_t          GetNGlobals()                             {return fNGlobals;};
    void                  SetNGlobals(Int_t nGlobals)               {fNGlobals=nGlobals;};
    inline Int_t          GetNPrimaries()                           {return fNPrimaries;};
    void                  SetNPrimaries(Int_t nPrimaries)           {fNPrimaries = nPrimaries;};
    
    inline Int_t          GetZDCRate()                           {return fZDCRate;};
    void                  SetZDCRate(Int_t nPrimaries)           {fZDCRate = nPrimaries;};
    inline Int_t          GetBBCRate()                           {return fBBCRate;};
    void                  SetBBCRate(Int_t nPrimaries)           {fBBCRate = nPrimaries;};

    inline Int_t          GetNParticles()                           {return fPosParticles->GetEntries()+fNegParticles->GetEntries();};
    inline TClonesArray*  GetPosParticles()                         {return fPosParticles;};
    inline TClonesArray*  GetNegParticles()                         {return fNegParticles;};
    void                  AddParticle(WiciParticle*);
    
    //inline Int_t          GetNElectrons()                           {return fElectrons->GetEntries();};
    //inline TClonesArray*  GetElectrons()                            {return fElectrons;};
    //void                  AddElectron(WiciLepton*);
    
  protected:
    Int_t                 fRunID;
    Int_t                 fEventID;
    
    vector<unsigned int>  fTriggers;
    unsigned int	  fBinaryTriggers;
    
    WiciVertex            fVertex;
    Float_t               fVpdVtxZ;
    
    Float_t               fMagField;
    Int_t                 fRefMult;
    Int_t                 fTofMult;
    Int_t                 fNGlobals;
    Int_t                 fNPrimaries;
    Int_t	fZDCRate;
    Int_t	fBBCRate;
    /*
    Int_t	fZDCE;
    Int_t	fZDCW;
    Int_t	fBBCE;
    Int_t	fBBCW;
    */
    
    TClonesArray*         fPosParticles;  //->
    TClonesArray*         fNegParticles;  //->
    //TClonesArray*         fElectrons;     //->
    
    ClassDef(WiciEvent,4)
};

#endif // WICIEVENT_H

