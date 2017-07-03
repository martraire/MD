// **********************************************************************
// * G4MD_EventActionMessenger.cc                                       *
// *                                                                    *
// * Date : 15 June 2015                                                *
// * Author : Diane Martraire (IHEP)                                    *
// *                                                                    *
// * Description :                                                      *
// * This file define the UI command available for the detector in the  *
// * input file.                                                        *
// **********************************************************************


#include <G4MD_EventActionMessenger.hh>

#include <G4MD_EventAction.hh>
#include <G4UIdirectory.hh>
#include <G4UIcmdWithAnInteger.hh>
#include <globals.hh>




G4MD_EventActionMessenger::G4MD_EventActionMessenger(G4MD_EventAction* EvAct)
:eventAction(EvAct)
{
  eventDir = new G4UIdirectory("/MD/event/");
  eventDir->SetGuidance("event control");
   
  // PrintCmd = new G4UIcmdWithAnInteger("/MD/event/printModulo",this);
  // PrintCmd->SetGuidance("Print events modulo n");
  // PrintCmd->SetParameterName("EventNb",false);
  PrintCmd->SetRange("EventNb>0");
}


G4MD_EventActionMessenger::~G4MD_EventActionMessenger()
{
  delete PrintCmd;
  delete eventDir;   
}


void G4MD_EventActionMessenger::SetNewValue(G4UIcommand* command,G4String newValue)
{ 
  // if(command == PrintCmd)
//     {eventAction->SetPrintModulo(PrintCmd->GetNewIntValue(newValue));}
  ;
}

