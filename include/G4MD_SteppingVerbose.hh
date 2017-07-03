// ********************************************************************
// * G4MD_SteppingVerbose.hh                                          *
// *                                                                  *
// * Date : 15 June 2015                                              *
// * Author : Diane Martraire (IHEP)                                  *
// *                                                                  *
// * Description :                                                    *
// * This header declares classes and functions permitting to print   *
// * informations inside the step of an event (e.g : step length, E). *
// ********************************************************************


class G4MD_SteppingVerbose;

#ifndef G4MD_SteppingVerbose_h
#define G4MD_SteppingVerbose_h 1

#include "G4SteppingVerbose.hh"


class G4MD_SteppingVerbose : public G4SteppingVerbose
{
 public:   

   G4MD_SteppingVerbose();
  ~G4MD_SteppingVerbose();

   void StepInfo();
   void TrackingStarted();

};


#endif
