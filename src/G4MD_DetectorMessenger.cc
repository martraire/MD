// **********************************************************************
// * G4MD_DetectorMessenger.cc                                          *
// *                                                                    *
// * Date : 15 June 2015                                                *
// * Author : Diane Martraire (IHEP)                                    *
// *                                                                    *
// * Description :                                                      *
// * This file define the UI command available for the detector in the  * 
// * input file.                                                        *
// **********************************************************************



#include <G4MD_DetectorMessenger.hh>

#include <G4MD_DetectorConstruction.hh>
#include <G4UIdirectory.hh>
#include <G4UIcmdWithAString.hh>
#include <G4UIcmdWithAnInteger.hh>
#include <G4UIcmdWithADoubleAndUnit.hh>
#include <G4UIcmdWithoutParameter.hh>



G4MD_DetectorMessenger::G4MD_DetectorMessenger(G4MD_DetectorConstruction* G4MD_Det)
:G4MD_Detector(G4MD_Det)
{ 
  MDDir = new G4UIdirectory("/MD/");
  MDDir->SetGuidance("UI commands of this example");
  
  detDir = new G4UIdirectory("/MD/det/");
  detDir->SetGuidance("detector control");
  
  //  AbsMaterCmd = new G4UIcmdWithAString("/MD/det/setAbsMat",this);
  //  AbsMaterCmd->SetGuidance("Select Material of the Absorber.");
  //  AbsMaterCmd->SetParameterName("choice",false);
  //  AbsMaterCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
  
 //  UpdateCmd = new G4UIcmdWithoutParameter("/MD/det/update",this);
//   UpdateCmd->SetGuidance("Update calorimeter geometry.");
//   UpdateCmd->SetGuidance("This command MUST be applied before \"beamOn\" ");
//   UpdateCmd->SetGuidance("if you changed geometrical value(s).");
//   UpdateCmd->AvailableForStates(G4State_Idle);
}


G4MD_DetectorMessenger::~G4MD_DetectorMessenger()
{
  //  delete AbsMaterCmd; 
  // delete UpdateCmd;
  delete detDir;
  delete MDDir;  
}


void G4MD_DetectorMessenger::SetNewValue(G4UIcommand* command,G4String newValue)
{ 
  //  if( command == AbsMaterCmd )
  //   { G4MD_Detector->SetAbsorberMaterial(newValue);}
   
 //  if( command == UpdateCmd )
//     { G4MD_Detector->UpdateGeometry(); }
}


