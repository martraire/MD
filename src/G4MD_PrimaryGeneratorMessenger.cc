// **********************************************************************
// * G4MD_GeneratorMessenger.cc                                         *
// *                                                                    *
// * Date : 15 June 2015                                                *
// * Author : Diane Martraire (IHEP)                                    *
// *                                                                    *
// * Description :                                                      *
// * This file define the UI command available for the detector in the  *
// * input file.                                                        *
// **********************************************************************


#include <G4MD_PrimaryGeneratorMessenger.hh>

#include <G4MD_PrimaryGeneratorAction.hh>
#include <G4UIdirectory.hh>
#include <G4UIcmdWithAString.hh>




G4MD_PrimaryGeneratorMessenger::G4MD_PrimaryGeneratorMessenger(
                                          G4MD_PrimaryGeneratorAction* G4MD_Gun)
:G4MD_Action(G4MD_Gun)
{
  gunDir = new G4UIdirectory("/MD/gun/");
  gunDir->SetGuidance("PrimaryGenerator control");
   
  RndmCmd = new G4UIcmdWithAString("/MD/gun/rndm",this);
  RndmCmd->SetGuidance("Shoot randomly the incident particle.");
  RndmCmd->SetGuidance("  Choice : on(default), off");
  RndmCmd->SetParameterName("choice",true);
  RndmCmd->SetDefaultValue("on");
  RndmCmd->SetCandidates("on off");
  RndmCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
}




G4MD_PrimaryGeneratorMessenger::~G4MD_PrimaryGeneratorMessenger()
{
  delete RndmCmd;
  delete gunDir;
}




//void G4MD_PrimaryGeneratorMessenger::SetNewValue(G4UIcommand* command, G4String newValue)
//{ 
//  if( command == RndmCmd )
//   { G4MD_Action->SetRndmFlag(newValue);}
//}



