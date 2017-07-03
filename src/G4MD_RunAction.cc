// **********************************************************************
// * G4MD_RunAction.cc                                                  *
// *                                                                    *
// * Date : 15 June 2015                                                *
// * Author : Diane Martraire (IHEP)                                    *
// *                                                                    *
// * Description :                                                      *
// * This file manage all the informations related to the run (print ..)*
// **********************************************************************


#include <G4MD_RunAction.hh>

#include <G4Run.hh>
#include <G4RunManager.hh>
#include <G4UnitsTable.hh>

using namespace std;

G4MD_RunAction::G4MD_RunAction()
{;}


G4MD_RunAction::~G4MD_RunAction()
{;}


void G4MD_RunAction::BeginOfRunAction(const G4Run* aRun)
{ 
  G4cout << "### Run " << aRun->GetRunID() << " start." << G4endl;
}


void G4MD_RunAction::EndOfRunAction(const G4Run* aRun)
{;}


