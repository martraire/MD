// ********************************************************************
// * G4MD_SteppingAction.hh                                           *
// *                                                                  *
// * Date : 15 July 2015                                              *
// * Author : Diane Martraire (IHEP)                                  *
// *                                                                  *
// * Description :                                                    *
// * This header declares classes and functions needed to store infos *
// * in an output during the step of an event.                        *
// ********************************************************************


#ifndef G4MD_SteppingAction_h
#define G4MD_SteppingAction_h 1

#include <G4UserSteppingAction.hh>

class G4Step;

class G4MD_SteppingAction : public G4UserSteppingAction
{
public:
  G4MD_SteppingAction();
  virtual ~G4MD_SteppingAction();

  void UserSteppingAction(const G4Step*);
  
  static double tot_edep;
  static double xout;
  static double yout;
  static double zout;
  
  static double xin;
  static double yin;
  static double zin;

protected:
  void WhileStepping(const G4Step*);
  

};


#endif
