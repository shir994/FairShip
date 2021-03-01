// -------------------------------------------------------------------
// -----       Generlal PG source file for SHiP                  -----
// -----      Version as of 0.0.1 by Evgenii Kurbatov            -----
// -----       mailto: ekurbatov(at)hse.ru                       -----
// -------------------------------------------------------------------

#include "TROOT.h"
#include <random>
#include "FairPrimaryGenerator.h"
#include "GeneralGun.h"
#include "TDatabasePDG.h"               // for TDatabasePDG
#include "TMath.h"
//#include <boost/json/src.hpp>
// #include <G4INCLRandom.hh>

using namespace std;

Bool_t GeneralGun::Init(string mOption){
	position_pdf = 0;
	momentum_pdf = 0;
	rng  = new TRandom3(gRandom->GetSeed());
	n_EVENTS = 0;
	return kTRUE;
}
// -----   Passing the event   -----------------------------------------
Bool_t GeneralGun::ReadEvent(FairPrimaryGenerator* cpg){

	//Position
	double cm = 1.;
	double sigma = 1.*cm;
	double range = 1.*cm;

	double x_shift = 0.*cm;
	double y_shift = 0.*cm;
	double x_limit = 1.*cm;
	double y_limit = 1.*cm;

	int sample_count = 0;
	bool success_flag = false;
	// pdf_name = GAUS;
	switch(position_pdf){
		case 0:
			x = rng->Gaus(0, sigma);//G4INCL::Random::gauss(sigma);
			y = rng->Gaus(0, sigma);//G4INCL::Random::gauss(sigma);

		case 1:
			x = rng->Uniform(-range, range);//(G4INCL::Random::shoot()-0.5) * range;
			y = rng->Uniform(-range, range); //(G4INCL::Random::shoot()-0.5) * range;

		case 2:

			while (true){
				x = rng->Gaus(x_shift, sigma); //G4INCL::Random::gauss(sigma) + x_shift;
				y = rng->Gaus(y_shift, sigma); //G4INCL::Random::gauss(sigma) + y_shift;
				if ((TMath::Abs(x) < x_limit) && (TMath::Abs(y) < y_limit)){
					success_flag = true;
					break;
				}
				sample_count++;
				if sample_count > 50{
					cout<<"Check your config, too many resamples";
					sample_count = 0;
				}
			}
	}
	//Energy
	switch (momentum_pdf){
		case 0:
			px = 0. * GeV;
			py = 0. * GeV;
			pz = 10. * GeV;
	}
	z = -400.;
	PID = 13;
	TDatabasePDG* pdgBase = TDatabasePDG::Instance();
	mass = pdgBase->GetParticle(PID)->Mass(); // muons!
	weight = 1;

	P = TMath::Sqrt(px*px + py*py + pz*pz);
	cpg->AddTrack(PID,px,py,pz,x,y,z,-1,true,TMath::Sqrt(P*P+mass*mass),0,weight);  // -1 = Mother ID, true = tracking, SQRT(x) = Energy, 0 = t
	n_EVENTS++;
	return kTRUE;
}
// ---------------------------------------------------------------------
Int_t GeneralGun::GetNevents()
{
 return n_EVENTS;
}

ClassImp(GeneralGun)
