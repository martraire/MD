// **********************************************************************
// * G4MD_RunAction.hh                                                  *
// *                                                                    *
// * Date : 15 July 2015                                                *
// * Author : Diane Martraire (IHEP)                                    *
// *                                                                    *
// * Description :                                                      *
// * This header declares the classes and functions which compose a run * 
// * but also parameters that we want to store in an output.            * 
// * A run can be composed of one event or more.                        *
// **********************************************************************


#ifndef G4MD_RunAction_h
#define G4MD_RunAction_h 1

#include <G4UserRunAction.hh>
#include <globals.hh>

class G4Run;

class G4MD_RunAction : public G4UserRunAction
{
public:
  G4MD_RunAction();
  virtual ~G4MD_RunAction();

protected:
  void BeginOfRunAction(const G4Run* aRun);
  void EndOfRunAction(const G4Run* aRun);
};

#endif

