#ifndef GOLIATHMAGNET1_H
#define GOLIATHMAGNET1_H

#include "FairModule.h"                 // for FairModule
#include "FairDetector.h"                  // for FairDetector

#include "Rtypes.h"                     // for ShipMuonShield::Class, Bool_t, etc

#include <string>                       // for string

#include "TVector3.h"
#include "TLorentzVector.h"

class FairVolume;

class GoliathMagnet:public FairModule
{
  public:
    GoliathMagnet(const char* name, const char* Title="Goliath");
    GoliathMagnet();
    virtual ~GoliathMagnet();

    void ConstructGeometry();
    void SetGoliathCentre(Double_t goliathcentre_to_beam);
    void SetGoliathCentreZ(Double_t goliathcentre);
    void SetGoliathSizes(Double_t H, Double_t TS, Double_t LS, Double_t BasisH);
    void SetCoilParameters(Double_t CoilR, Double_t UpCoilH, Double_t LowCoilH, Double_t CoilD);
    ClassDef(GoliathMagnet,1)

protected:
    //magnetic field intensity
    Double_t Bfield;

    //Goliath by Annarita

    Double_t Height;
    // Double_t zCenter;
    Double_t LongitudinalSize;
    Double_t TransversalSize;
    //Double_t GapFromTSpectro;
    Double_t CoilRadius;
    Double_t UpCoilHeight;
    Double_t LowCoilHeight;
    Double_t CoilDistance;
    Double_t BasisHeight;
    //
    Double_t       fgoliathcentre_to_beam;
    Double_t       fgoliathcentre;

    Int_t InitMedium(const char* name);

    GoliathMagnet(const GoliathMagnet&);
    GoliathMagnet& operator=(const GoliathMagnet&);

};
#endif
