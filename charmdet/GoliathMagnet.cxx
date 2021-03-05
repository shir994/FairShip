#include "GoliathMagnet.h"
//#include "MagneticSpectrometer.h"
#include "SpectrometerPoint.h"
#include "TGeoManager.h"
#include "FairRun.h"                    // for FairRun
#include "FairRuntimeDb.h"              // for FairRuntimeDb
#include <iosfwd>                    // for ostream
#include "TList.h"                      // for TListIter, TList (ptr only)
#include "TObjArray.h"                  // for TObjArray
#include "TString.h"                    // for TString
#include "TClonesArray.h"
#include "TVirtualMC.h"

#include "TGeoBBox.h"
#include "TGeoTrd1.h"
#include "TGeoCompositeShape.h"
#include "TGeoTube.h"
#include "TGeoArb8.h"
#include "TGeoMaterial.h"
#include "TGeoMedium.h"
#include "TParticle.h"
#include "TVector3.h"

#include "FairVolume.h"
#include "FairGeoVolume.h"
#include "FairGeoNode.h"
#include "FairRootManager.h"
#include "FairGeoLoader.h"
#include "FairGeoInterface.h"
#include "FairGeoMedia.h"
#include "FairGeoBuilder.h"
#include "FairRun.h"
#include "FairRuntimeDb.h"

#include "ShipDetectorList.h"
#include "ShipUnit.h"
#include "ShipStack.h"

#include "TGeoUniformMagField.h"
#include <stddef.h>                     // for NULL
#include <iostream>                     // for operator<<, basic_ostream, etc

using std::cout;
using std::endl;
using namespace ShipUnit;


GoliathMagnet::~GoliathMagnet()
{
}


GoliathMagnet::GoliathMagnet(const char* name, const char* Title)
  : FairModule(name, Title)
{
    Bfield = 1.45 * tesla;
}

GoliathMagnet::GoliathMagnet()
  : FairModule("Goliath", "")
{
    Bfield = 1.45 * tesla;
}

void GoliathMagnet::SetGoliathSizes(Double_t H, Double_t TS, Double_t LS, Double_t BasisH)
{
    LongitudinalSize = LS;
    TransversalSize = TS;
    Height = H;
    BasisHeight = BasisH;
}

void GoliathMagnet::SetCoilParameters(Double_t CoilR, Double_t UpCoilH, Double_t LowCoilH, Double_t CoilD)
{
    CoilRadius = CoilR;
    UpCoilHeight = UpCoilH;
    LowCoilHeight = LowCoilH;
    CoilDistance = CoilD;
}

void GoliathMagnet::SetGoliathCentre(Double_t goliathcentre_to_beam)
{
      fgoliathcentre_to_beam=goliathcentre_to_beam;
}

void GoliathMagnet::SetGoliathCentreZ(Double_t goliathcentre)
{
      fgoliathcentre=goliathcentre;
}

Int_t GoliathMagnet::InitMedium(const char* name)
{
   static FairGeoLoader *geoLoad=FairGeoLoader::Instance();
   static FairGeoInterface *geoFace=geoLoad->getGeoInterface();
   static FairGeoMedia *media=geoFace->getMedia();
   static FairGeoBuilder *geoBuild=geoLoad->getGeoBuilder();

   FairGeoMedium *ShipMedium=media->getMedium(name);

   if (!ShipMedium)
   {
     Fatal("InitMedium","Material %s not defined in media file.", name);
     return -1111;
   }
   TGeoMedium* medium=gGeoManager->GetMedium(name);
   if (medium!=NULL)
     return ShipMedium->getMediumIndex();
   return geoBuild->createMedium(ShipMedium);
}


void GoliathMagnet::ConstructGeometry() {
//***********************************************************************************************
    //*****************************************   GOLIATH BY ANNARITA *****************************************
    //***********************************************************************************************

    InitMedium("iron");
    TGeoMedium *Fe =gGeoManager->GetMedium("iron");

    InitMedium("CoilCopper");
    TGeoMedium *Cu  = gGeoManager->GetMedium("CoilCopper");

    InitMedium("CoilAluminium");
    TGeoMedium *Al  = gGeoManager->GetMedium("CoilAluminium");

    //put drift tubes in air
    InitMedium("air");
    TGeoMedium *air               = gGeoManager->GetMedium("air");

    InitMedium("tungsten");
    TGeoMedium *tungsten          = gGeoManager->GetMedium("tungsten");

    InitMedium("ShipSens");
    TGeoMedium *Sens =gGeoManager->GetMedium("ShipSens");

    TGeoVolume *top = gGeoManager->GetTopVolume();

    gGeoManager->SetTopVisible();


    TGeoBBox *BoxGoliath = new TGeoBBox(TransversalSize/2,Height/2,LongitudinalSize/2);
    TGeoVolume *volGoliath = new TGeoVolume("volGoliath",BoxGoliath,air);
    TGeoRotation ry90;
    TGeoTranslation gtrans;

    ry90.RotateY(90);
    //From latest (2017) field measurements: beam coordinates x=-1.4mm, y=-178.6mm, hence need to move Goliath up
    //gtrans.SetTranslation(1.4*mm,fgoliathcentre_to_beam,350.75);
    //edms 1834065 v.1 :


    //gtrans.SetTranslation(1.4*mm,fgoliathcentre_to_beam-0.75*cm,fgoliathcentre);
    // This is setup for ideal field, with field map use the above
    gtrans.SetTranslation(0., 0., fgoliathcentre);
    TGeoCombiTrans cg(gtrans,ry90);
    TGeoHMatrix *mcg = new TGeoHMatrix(cg);

    TGeoUniformMagField* mag_field = new TGeoUniformMagField(0.,
                                                             Bfield,
                                                             0.);
    volGoliath->SetField(mag_field);
    top->AddNode(volGoliath,1,mcg);


    //
    //******* UPPER AND LOWER BASE *******
    //

    TGeoBBox *Base = new TGeoBBox(TransversalSize/2,BasisHeight/2,LongitudinalSize/2);
    TGeoVolume *volBase = new TGeoVolume("volBase",Base,Fe);
    volBase->SetLineColor(kRed);
    volGoliath->AddNode(volBase,1,new TGeoTranslation(0, Height/2 - BasisHeight/2, 0)); //upper part
    volGoliath->AddNode(volBase,2,new TGeoTranslation(0, -Height/2 + BasisHeight/2, 0)); //lower part

    //
    //**************************** MAGNETS ******************************
    //

    TGeoRotation *r1 = new TGeoRotation();
    r1->SetAngles(0,90,0);
    TGeoCombiTrans t1(0, Height/2 - BasisHeight - UpCoilHeight/2, 0,r1);
    TGeoHMatrix *m1 = new TGeoHMatrix(t1);

    TGeoTube *magnetUp = new TGeoTube(0,CoilRadius,UpCoilHeight/2);
    TGeoVolume *volmagnetUp = new TGeoVolume("volmagnetUp",magnetUp,Cu);
    volmagnetUp->SetLineColor(kGreen);
    volGoliath->AddNode(volmagnetUp,1,m1); //upper part


    TGeoCombiTrans t2(0, -Height/2 + BasisHeight + LowCoilHeight/2, 0,r1);
    TGeoHMatrix *m_2 = new TGeoHMatrix(t2);


    TGeoTube *magnetDown = new TGeoTube(0,CoilRadius,LowCoilHeight/2);
    TGeoVolume *volmagnetDown = new TGeoVolume("volmagnetDown",magnetDown,Al);
    volmagnetDown->SetLineColor(kGreen);
    volGoliath->AddNode(volmagnetDown,1,m_2); //lower part

    //
    //********************* LATERAL SURFACES ****************************
    //

    Double_t base1 = 135, base2 = 78; //basis of the trapezoid
    Double_t side1 = 33, side2 = 125, side3 = 57, side4 = 90; //Sides of the columns

    //***** SIDE Left Front ****

    //LONGER RECTANGLE
    TGeoBBox *LateralS1 = new TGeoBBox("LateralS1",side1/2,UpCoilHeight/2,base1/2);
    TGeoTranslation *tr1 = new TGeoTranslation(-TransversalSize/2 + side1/2, Height/2 - BasisHeight - UpCoilHeight/2, -LongitudinalSize/2 + base1/2);
    TGeoVolume *volLateralS1 = new TGeoVolume("volLateralS1",LateralS1,Fe);
    volLateralS1->SetLineColor(kRed);
    //volLateralS1->SetField(magField);
    volGoliath->AddNode(volLateralS1, 1, tr1);

    //TRAPEZOID

    TGeoArb8 *LateralS2 = new TGeoArb8("LateralS2", UpCoilHeight/2);
    LateralS2->SetVertex(0, side4, 0);
    LateralS2->SetVertex(1, side1, 0);
    LateralS2->SetVertex(2, side1, base1);
    LateralS2->SetVertex(3, side4, base2);
    LateralS2->SetVertex(4, side4, 0);
    LateralS2->SetVertex(5, side1, 0);
    LateralS2->SetVertex(6, side1, base1);
    LateralS2->SetVertex(7, side4, base2);

    TGeoVolume *volLateralS2 = new TGeoVolume("volLateralS2",LateralS2,Fe);
    volLateralS2->SetLineColor(kRed);
    //volLateralS2->SetField(magField);

    TGeoRotation *r2 = new TGeoRotation();
    r2->SetAngles(0,90,0);
    TGeoCombiTrans tr3(-TransversalSize/2, Height/2 - BasisHeight - UpCoilHeight/2, -LongitudinalSize/2,r2);
    TGeoHMatrix *m3_a = new TGeoHMatrix(tr3);
    volGoliath->AddNode(volLateralS2, 1, m3_a);


    //LOWER LATERAL SURFACE

    //LONGER RECTANGLE
    TGeoBBox *LateralSurface1low = new TGeoBBox("LateralSurface1low",side1/2,(CoilDistance + LowCoilHeight)/2,side2/2);
    TGeoVolume *volLateralSurface1low = new TGeoVolume("volLateralSurface1low",LateralSurface1low,Fe);
    volLateralSurface1low->SetLineColor(kRed);
    //volLateralSurface1low->SetField(magField);
    TGeoTranslation *tr1low = new TGeoTranslation(-TransversalSize/2 +side1/2, Height/2 - BasisHeight - UpCoilHeight - (CoilDistance + LowCoilHeight)/2, -LongitudinalSize/2 + side2/2);
    volGoliath->AddNode(volLateralSurface1low, 1, tr1low);;


    //SHORTER RECTANGLE
    TGeoBBox *LateralSurface2low = new TGeoBBox("LateralSurface2low",side3/2,(CoilDistance + LowCoilHeight)/2,base2/2);
    TGeoVolume *volLateralSurface2low = new TGeoVolume("volLateralSurface2low",LateralSurface2low,Fe);
    volLateralSurface2low->SetLineColor(kRed);
    TGeoTranslation *tr2low = new TGeoTranslation(-TransversalSize/2 +side1 + side3/2, Height/2 - BasisHeight - UpCoilHeight - (CoilDistance + LowCoilHeight)/2, -LongitudinalSize/2 + base2/2);
    volGoliath->AddNode(volLateralSurface2low, 1, tr2low);
    //volLateralSurface2low->SetField(magField);

    //***** SIDE Right Front ****

    //LONGER RECTANGLE
    TGeoTranslation *tr1_b = new TGeoTranslation(-TransversalSize/2 + side1/2, Height/2 - BasisHeight - UpCoilHeight/2, LongitudinalSize/2 - base1/2);
    TGeoVolume *volLateralS1_b = new TGeoVolume("volLateralS1_b",LateralS1,Fe);
    volLateralS1_b->SetLineColor(kRed);
    //volLateralS1_b->SetField(magField);
    volGoliath->AddNode(volLateralS1_b, 1, tr1_b);

    //TRAPEZOID
    TGeoArb8 *LateralS2_b = new TGeoArb8("LateralS2_b",UpCoilHeight/2);
    LateralS2_b ->SetVertex(0, side4, 0);
    LateralS2_b ->SetVertex(1, side1, 0);
    LateralS2_b ->SetVertex(2, side1, base1);
    LateralS2_b ->SetVertex(3, side4, base2);
    LateralS2_b ->SetVertex(4, side4, 0);
    LateralS2_b ->SetVertex(5, side1, 0);
    LateralS2_b ->SetVertex(6, side1, base1);
    LateralS2_b ->SetVertex(7, side4, base2);

    TGeoVolume *volLateralS2_b = new TGeoVolume("volLateralS2_b",LateralS2_b,Fe);
    volLateralS2_b->SetLineColor(kRed);
    //volLateralS2_b->SetField(magField);

    TGeoRotation *r2_b = new TGeoRotation();
    r2_b->SetAngles(0,270,0);
    TGeoCombiTrans tr2_b(-TransversalSize/2 , Height/2 - BasisHeight - UpCoilHeight/2, LongitudinalSize/2,r2_b);
    TGeoHMatrix *m3_b = new TGeoHMatrix(tr2_b);
    volGoliath->AddNode(volLateralS2_b, 1, m3_b);


    //LOWER LATERAL SURFACE

    //LONGER RECTANGLE
    TGeoVolume *volLateralSurface1blow = new TGeoVolume("volLateralSurface1blow",LateralSurface1low,Fe);
    volLateralSurface1blow->SetLineColor(kRed);
    //volLateralSurface1blow->SetField(magField);
    TGeoTranslation *tr1blow = new TGeoTranslation(-TransversalSize/2 +side1/2, Height/2 - BasisHeight - UpCoilHeight - (CoilDistance + LowCoilHeight)/2, LongitudinalSize/2 - side2/2);
    volGoliath->AddNode(volLateralSurface1blow, 1, tr1blow);;


    //SHORTER RECTANGLE
    TGeoVolume *volLateralSurface2blow = new TGeoVolume("volLateralSurface2blow",LateralSurface2low,Fe);
    volLateralSurface2blow->SetLineColor(kRed);
    //volLateralSurface2blow->SetField(magField);
    TGeoTranslation *tr2blow = new TGeoTranslation(-TransversalSize/2 +side1 + side3/2, Height/2 - BasisHeight - UpCoilHeight - (CoilDistance + LowCoilHeight)/2, LongitudinalSize/2 - base2/2);
    volGoliath->AddNode(volLateralSurface2blow, 1, tr2blow);


    //***** SIDE left Back ****


    //LONGER RECTANGLE
    TGeoBBox *LateralS1_d = new TGeoBBox("LateralS1_d",side1/2,(UpCoilHeight + LowCoilHeight + CoilDistance)/2,base1/2);
    TGeoTranslation *tr1_d = new TGeoTranslation(TransversalSize/2 - side1/2, Height/2 - BasisHeight - (UpCoilHeight + LowCoilHeight + CoilDistance)/2, -LongitudinalSize/2 + base1/2);
    TGeoVolume *volLateralS1_d = new TGeoVolume("volLateralS1_d",LateralS1_d,Fe);
    volLateralS1_d->SetLineColor(kRed);
    //volLateralS1_d->SetField(magField);
    volGoliath->AddNode(volLateralS1_d, 1, tr1_d);

    //TRAPEZOID

    TGeoArb8 *LateralS2_d = new TGeoArb8("LateralS2_d",(UpCoilHeight + LowCoilHeight + CoilDistance)/2);
    LateralS2_d->SetVertex(0, side4, 0);
    LateralS2_d->SetVertex(1, side1, 0);
    LateralS2_d->SetVertex(2, side1, base1);
    LateralS2_d->SetVertex(3, side4, base2);
    LateralS2_d->SetVertex(4, side4, 0);
    LateralS2_d->SetVertex(5, side1, 0);
    LateralS2_d->SetVertex(6, side1, base1);
    LateralS2_d->SetVertex(7, side4, base2);


    TGeoVolume *volLateralS2_d = new TGeoVolume("volLateralS2_d",LateralS2_d,Fe);
    volLateralS2_d->SetLineColor(kRed);
    //volLateralS2_d->SetField(magField);

    TGeoRotation *r2_d = new TGeoRotation();
    r2_d->SetAngles(0,270,180);
    TGeoCombiTrans tr2_d(TransversalSize/2 , Height/2 - BasisHeight - (UpCoilHeight + LowCoilHeight + CoilDistance)/2, -LongitudinalSize/2,r2_d);
    TGeoHMatrix *m3_d = new TGeoHMatrix(tr2_d);
    volGoliath->AddNode(volLateralS2_d, 1, m3_d);


    //***** SIDE right Back ****


    //LONGER RECTANGLE
    TGeoBBox *LateralS1_c = new TGeoBBox("LateralS1_c",side1/2,(UpCoilHeight + LowCoilHeight + CoilDistance)/2,base1/2);
    TGeoTranslation *tr1_c = new TGeoTranslation(TransversalSize/2 - side1/2, Height/2 - BasisHeight - (UpCoilHeight + LowCoilHeight + CoilDistance)/2, LongitudinalSize/2 - base1/2);
    TGeoVolume *volLateralS1_c = new TGeoVolume("volLateralS1_c",LateralS1_c,Fe);
    volLateralS1_c->SetLineColor(kRed);
    //volLateralS1_c->SetField(magField);
    volGoliath->AddNode(volLateralS1_c, 1, tr1_c);

    //TRAPEZOID

    TGeoArb8 *LateralS2_c = new TGeoArb8("LateralS2_c",(UpCoilHeight + LowCoilHeight + CoilDistance)/2);
    LateralS2_c ->SetVertex(0, side4, 0);
    LateralS2_c ->SetVertex(1, side1, 0);
    LateralS2_c ->SetVertex(2, side1, base1);
    LateralS2_c ->SetVertex(3, side4, base2);
    LateralS2_c ->SetVertex(4, side4, 0);
    LateralS2_c ->SetVertex(5, side1, 0);
    LateralS2_c ->SetVertex(6, side1, base1);
    LateralS2_c ->SetVertex(7, side4, base2);

    TGeoVolume *volLateralS2_c = new TGeoVolume("volLateralS2_c",LateralS2_c,Fe);
    volLateralS2_c->SetLineColor(kRed);
    //volLateralS2_c->SetField(magField);

    TGeoRotation *r2_c = new TGeoRotation();
    r2_c->SetAngles(0,90,180);
    TGeoCombiTrans tr2_c(TransversalSize/2 , Height/2 - BasisHeight - (UpCoilHeight + LowCoilHeight + CoilDistance)/2, LongitudinalSize/2,r2_c);
    TGeoHMatrix *m3_c = new TGeoHMatrix(tr2_c);
    volGoliath->AddNode(volLateralS2_c, 1, m3_c);

    //END GOLIATH PART BY ANNARITA
}


ClassImp(GoliathMagnet)