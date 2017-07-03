// ********************************************************************
// * G4MD_TrackingAction.hh                                           *
// *                                                                  *
// * Date : 15 July 2015                                              *
// * Author : Diane Martraire (IHEP)                                  *
// *                                                                  *
// * Description :                                                    *
// * This file permit to get  declares information about the particle *
// * tracked and to store it in an output.                            *
// ********************************************************************


#ifndef G4MD_TrackingAction_h
#define G4MD_TrackingAction_h 1

#include <G4UserTrackingAction.hh>
#include <vector.h>

class G4Track;

class G4MD_TrackingAction : public G4UserTrackingAction
{
public:
  G4MD_TrackingAction(); 
  virtual ~G4MD_TrackingAction();

  void PreUserTrackingAction(const G4Track*);
  void PostUserTrackingAction(const G4Track*);
  void BeforeTracking(const G4Track*);
  
  static int fLoseLightEnergy;
  static int fLoseLightQE;
  
  static int fgNumC;
  static int fgNumCdelta;
  static int fgNbounces;
  
  static int fLoseLight;
  static double fDepositEnergy;
  static double tmp_fDepositEnergy;
  static int nph;
  
private:
  double fMinCerenkovEnergy;
  double fMaxCerenkovEnergy;

  double fCollectionEfficiency;

  vector<double> fEnergyVector;
  vector<double> fQEVector;

  double GetQE(double);  
};

#endif
