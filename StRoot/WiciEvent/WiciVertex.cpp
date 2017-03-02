#include "WiciVertex.h"
#include <iostream>

using namespace std;

ClassImp(WiciVertex)
//__________________________________________________________________________________________________
WiciVertex::WiciVertex():TObject() {
  fIndex  = -9999;
  fX      = -9999;
  fY      = -9999;
  fZ      = -9999;
  fRank   = -9999;
}
//__________________________________________________________________________________________________
WiciVertex::WiciVertex(WiciVertex* vertex) {
  fIndex  = vertex->GetIndex();
  fX      = vertex->GetX();
  fY      = vertex->GetY();
  fZ      = vertex->GetZ();
  fRank   = vertex->GetRank();
}
//__________________________________________________________________________________________________
WiciVertex& WiciVertex::operator=(WiciVertex& vertex) {
  fIndex  = vertex.GetIndex();
  fX      = vertex.GetX();
  fY      = vertex.GetY();
  fZ      = vertex.GetZ();
  fRank   = vertex.GetRank();
  
  return *this;
}

