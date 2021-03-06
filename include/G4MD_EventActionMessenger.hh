// ********************************************************************
// * G4MD_EventActionMessenger.hh                                        *
// *                                                                  *
// * Date : 15 June 2015                                              *
// * Author : Diane Martraire (IHEP)                                  *
// *                                                                  *
// * Description :                                                    *
// * This  header defines ??????????                                  *
// * ????????????????????????????????                                 *
// ********************************************************************


#ifndef G4MD_EventActionMessenger_h
#define G4MD_EventActionMessenger_h 1

#include "globals.hh"
#include "G4UImessenger.hh"

class G4MD_EventAction;
class G4UIdirectory;
class G4UIcmdWithAnInteger;

class G4MD_EventActionMessenger: public G4UImessenger
{
public:
  G4MD_EventActionMessenger(G4MD_EventAction*);
  virtual ~G4MD_EventActionMessenger();
    
  void SetNewValue(G4UIcommand*, G4String);
    
private:
  G4MD_EventAction*     eventAction;
  G4UIdirectory*        eventDir;   
  G4UIcmdWithAnInteger* PrintCmd;    
};


#endif
