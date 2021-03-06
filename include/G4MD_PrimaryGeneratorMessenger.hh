// *********************************************************************
// * G4MD_PrimaryGeneratorMessenger.hh                                 *
// *                                                                   *
// * Date : 15 June 2015                                               *
// * Author : Diane Martraire (IHEP)                                   *
// *                                                                   *
// * Description :                                                     *
// * This header declares the UI commands of the generator. Parameters *  
// * of the generator can be changed in the input file '.in'.          *
// *********************************************************************


#ifndef G4MD_PrimaryGeneratorMessenger_h
#define G4MD_PrimaryGeneratorMessenger_h 1

#include "G4UImessenger.hh"
#include "globals.hh"

class G4MD_PrimaryGeneratorAction;
class G4UIdirectory;
class G4UIcmdWithAString;


class G4MD_PrimaryGeneratorMessenger: public G4UImessenger
{
public:
  G4MD_PrimaryGeneratorMessenger(G4MD_PrimaryGeneratorAction*);
  virtual ~G4MD_PrimaryGeneratorMessenger();
    
  //  void SetNewValue(G4UIcommand*, G4String);
    
private:
  G4MD_PrimaryGeneratorAction* G4MD_Action;
  G4UIdirectory*               gunDir; 
  G4UIcmdWithAString*          RndmCmd;
};


#endif

