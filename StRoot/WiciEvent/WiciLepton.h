#ifndef WICI_LEPTON_H
#define WICI_LEPTON_H

#include "WiciEvent/WiciParticle.h"

class WiciLepton : public WiciParticle {
  public:
    WiciLepton();
    WiciLepton(WiciLepton*);
    WiciLepton(WiciParticle*);
    virtual ~WiciLepton();
    WiciLepton& operator=(WiciLepton&);
    
    inline Int_t    GetCloseTowerID() const                                   {return fCloseTowerID;};
    void            SetCloseTowerID(Int_t towerID)                            {fCloseTowerID=towerID;};
    inline Float_t  GetCloseTowerEnergy() const                               {return fCloseTowerEnergy;};
    void            SetCloseTowerEnergy(Float_t energy)                       {fCloseTowerEnergy=energy;};
    inline Bool_t   IsCloseTowerTrigger() const                               {return fCloseTowerTrigger;};
    void            SetCloseTowerTrigger(Bool_t trigger)                      {fCloseTowerTrigger=trigger;};
    void            SetCloseTower(Int_t towID, Float_t e, Bool_t trig)        {fCloseTowerID=towID; fCloseTowerEnergy=e; fCloseTowerTrigger=trig;};
    inline Float_t  GetCloseTowerDist() const                                 {return fCloseTowerDist;};
    void            SetCloseTowerDist(Float_t dist)                           {fCloseTowerDist=dist;};
    inline Float_t  GetCloseTowerDistPhi() const                              {return fCloseTowerDistPhi;};
    void            SetCloseTowerDistPhi(Float_t distPhi)                     {fCloseTowerDistPhi=distPhi;};
    inline Float_t  GetCloseTowerDistZ() const                                {return fCloseTowerDistZ;};
    void            SetCloseTowerDistZ(Float_t distZ)                         {fCloseTowerDistZ=distZ;};
    void            SetCloseTowerDist(Float_t dist, Float_t phi, Float_t z)   {fCloseTowerDist=dist; fCloseTowerDistPhi=phi; fCloseTowerDistZ=z;};
    
    inline Int_t    GetHighTowerID() const                                    {return fHighTowerID;};
    void            SetHighTowerID(Int_t towerID)                             {fHighTowerID=towerID;};
    inline Float_t  GetHighTowerEnergy() const                                {return fHighTowerEnergy;};
    void            SetHighTowerEnergy(Float_t energy)                        {fHighTowerEnergy=energy;};
    inline Bool_t   IsHighTowerTrigger() const                                {return fHighTowerTrigger;};
    void            SetHighTowerTrigger(Bool_t trigger)                       {fHighTowerTrigger=trigger;};
    void            SetHighTower(Int_t towID, Float_t e, Bool_t trig)         {fHighTowerID=towID; fHighTowerEnergy=e; fHighTowerTrigger=trig;};
    inline Float_t  GetHighTowerDist() const                                  {return fHighTowerDist;};
    void            SetHighTowerDist(Float_t dist)                            {fHighTowerDist=dist;};
    inline Float_t  GetHighTowerDistPhi() const                               {return fHighTowerDistPhi;};
    void            SetHighTowerDistPhi(Float_t distPhi)                      {fHighTowerDistPhi=distPhi;};
    inline Float_t  GetHighTowerDistZ() const                                 {return fHighTowerDistZ;};
    void            SetHighTowerDistZ(Float_t distZ)                          {fHighTowerDistZ=distZ;};
    void            SetHighTowerDist(Float_t dist, Float_t phi, Float_t z)    {fHighTowerDist=dist; fHighTowerDistPhi=phi; fHighTowerDistZ=z;};
    
    inline Int_t    GetSmdeHits() const                                       {return fSmdeHits;};
    void            SetSmdeHits(Int_t hits)                                   {fSmdeHits=hits;};
    inline Float_t  GetSmdeDist() const                                       {return fSmdeDist;};
    void            SetSmdeDist(Float_t dist)                                 {fSmdeDist=dist;};
    inline Float_t  GetSmdeDistPhi() const                                    {return fSmdeDistPhi;};
    void            SetSmdeDistPhi(Float_t distPhi)                           {fSmdeDistPhi=distPhi;};
    inline Float_t  GetSmdeDistZ() const                                      {return fSmdeDistZ;};
    void            SetSmdeDistZ(Float_t distZ)                               {fSmdeDistZ=distZ;};
    void            SetSmde(Int_t hits, Float_t dist, Float_t phi, Float_t z) {fSmdeHits=hits; fSmdeDist=dist; fSmdeDistPhi=phi; fSmdeDistZ=z;};
    
    inline Int_t    GetSmdpHits() const                                       {return fSmdpHits;};
    void            SetSmdpHits(Int_t hits)                                   {fSmdpHits=hits;};
    inline Float_t  GetSmdpDist() const                                       {return fSmdpDist;};
    void            SetSmdpDist(Float_t dist)                                 {fSmdpDist=dist;};
    inline Float_t  GetSmdpDistPhi() const                                    {return fSmdpDistPhi;};
    void            SetSmdpDistPhi(Float_t distPhi)                           {fSmdpDistPhi=distPhi;};
    inline Float_t  GetSmdpDistZ() const                                      {return fSmdpDistZ;};
    void            SetSmdpDistZ(Float_t distZ)                               {fSmdpDistZ=distZ;};
    void            SetSmdp(Int_t hits, Float_t dist, Float_t phi, Float_t z) {fSmdpHits=hits; fSmdpDist=dist; fSmdpDistPhi=phi; fSmdpDistZ=z;};
    
    inline Bool_t   IsPhotonic() const                                        {return fPhotonic;};
    void            SetPhotonic(Bool_t photonic)                              {fPhotonic=photonic;};
    
  protected:
    Int_t   fCloseTowerID;
    Float_t fCloseTowerEnergy;
    Bool_t  fCloseTowerTrigger;
    Float_t fCloseTowerDist;
    Float_t fCloseTowerDistZ;
    Float_t fCloseTowerDistPhi;
    
    Int_t   fHighTowerID;
    Float_t fHighTowerEnergy;
    Bool_t  fHighTowerTrigger;
    Float_t fHighTowerDist;
    Float_t fHighTowerDistZ;
    Float_t fHighTowerDistPhi;
    
    Int_t   fSmdeHits;
    Float_t fSmdeDist;
    Float_t fSmdeDistPhi;
    Float_t fSmdeDistZ;
    
    Int_t   fSmdpHits;
    Float_t fSmdpDist;
    Float_t fSmdpDistPhi;
    Float_t fSmdpDistZ;
    
    Bool_t  fPhotonic;
    ClassDef(WiciLepton,1)
};

#endif

