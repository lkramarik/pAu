#ifndef WICIVERTEX_H
#define WICIVERTEX_H
#include <TObject.h>

class WiciVertex : public TObject {
  public:
    WiciVertex();
    WiciVertex(WiciVertex*);
    virtual ~WiciVertex(){};
    WiciVertex& operator=(WiciVertex&);
    
    Int_t   GetIndex()            {return fIndex;};
    void    SetIndex(Int_t index) {fIndex=index;};
    Float_t GetX()                {return fX;};
    void    SetX(Float_t X)       {fX=X;};
    Float_t GetY()                {return fY;};
    void    SetY(Float_t Y)       {fY=Y;};
    Float_t GetZ()                {return fZ;};
    void    SetZ(Float_t Z)       {fZ=Z;};
    Float_t GetRank()             {return fRank;};
    void    SetRank(Float_t rank) {fRank=rank;};
  
  protected:
    Int_t   fIndex;
    Float_t fX;
    Float_t fY;
    Float_t fZ;
    Float_t fRank;
  
  ClassDef(WiciVertex,1)
};

#endif // WICIVERTEX_H

