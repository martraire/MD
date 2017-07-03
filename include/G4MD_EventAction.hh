// ********************************************************************
// * G4MD_EventAction.hh                                              *
// *                                                                  *
// * Date : 15 June 2015                                              *
// * Author : Diane Martraire (IHEP)                                  *
// *                                                                  *
// * Description :                                                    *
// * This header declares the UI commands of the Event.               *
// * Parameters can be change in the input file '.in' .               *
// ********************************************************************


#ifndef G4MD_EventAction_h
#define G4MD_EventAction_h 1

#include "G4UserEventAction.hh"
#include "globals.hh"

class G4Event;
class G4Timer;
class G4MD_EventActionMessenger;

class G4MD_EventAction : public G4UserEventAction
{
public:
  G4MD_EventAction();
  virtual ~G4MD_EventAction();

  void BeginOfEventAction(const G4Event*);
  void EndOfEventAction(const G4Event*);
  static G4int GetPrimaryID() { return fgPrimaryID; };// sert à rien ?
  

  static G4int eventid;
  static G4int fgPrimaryID;

    
private:
  G4Timer* fTimer;
  G4MD_EventActionMessenger*  eventMessenger;
};


#endif

    
