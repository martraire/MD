// **********************************************************************
// * G4MD_DetectorMessenger.hh                                          *
// *                                                                    *
// * Date : 15 June 2015                                                *
// * Author : Diane Martraire (IHEP)                                    *
// *                                                                    *
// * Description :                                                      *
// * This header declares the UI commands of the geometry of detector.  *
// * Parameters of the detector can be changed in the input file '.in' .*
// **********************************************************************


#ifndef G4MD_DetectorMessenger_h
#define G4MD_DetectorMessenger_h 1

#include "globals.hh"
#include "G4UImessenger.hh"

class G4MD_DetectorConstruction;
class G4UIdirectory;
class G4UIcmdWithAString;
class G4UIcmdWithAnInteger;
class G4UIcmdWithADoubleAndUnit;
class G4UIcmdWithoutParameter;


class G4MD_DetectorMessenger: public G4UImessenger
{
public:
  G4MD_DetectorMessenger(G4MD_DetectorConstruction* );
  ~G4MD_DetectorMessenger();
  
  void SetNewValue(G4UIcommand*, G4String);
    
private:
  G4MD_DetectorConstruction* G4MD_Detector;
    
  G4UIdirectory*             MDDir;
  G4UIdirectory*             detDir;
  // G4UIcmdWithAString*        AbsMaterCmd;
  G4UIcmdWithoutParameter*   UpdateCmd;
};


#endif

