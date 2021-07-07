// -------------------------------------------------------------------
// -----       Generlal PG source file for SHiP                  -----
// -----      Version as of 0.0.1 by Evgenii Kurbatov            -----
// -----       mailto: ekurbatov(at)hse.ru                       -----
// -------------------------------------------------------------------

#ifndef GenPG_GENERATOR_H
#define GenPG_GENERATOR_H 1

#include "TROOT.h"
#include "FairGenerator.h"
#include "TRandom3.h"
#include "TF1.h"
#include "TMath.h"
#include "TH1.h"
#include <string>

using namespace std;
class FairPrimaryGenerator;

class GeneralGun : public FairGenerator{
 public:

  	/** constructor,destructor **/
	GeneralGun(){};  
	virtual ~GeneralGun(){
		delete rng; 
	};
  
	/** public method ReadEvent **/
	Bool_t ReadEvent(FairPrimaryGenerator*);  //!
	//  virtual Bool_t Init(); //!
	virtual Bool_t Init(double z_pos, double energy); //!
	Int_t GetNevents();
	
	double z0, yBox,xBox,zBox,xdist, zdist, minE;
	int fNevents;
  
 private:
	TRandom3 *rng;//!
  
 protected:

 	int position_pdf;
 	int momentum_pdf;
	double energy,P,px,py,pz,x,y,z,weighttest, weight, mass, theta;
	int PID;//!
	

	ClassDef(GeneralGun, 1);
};

#endif /* !GenPG_GENERATOR_H */
