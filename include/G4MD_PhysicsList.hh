// ********************************************************************
// * G4MD_PhysicsList.hh                                              *
// *                                                                  *
// * Date : 15 June 2015                                              *
// * Author : Diane Martraire (IHEP)                                  *
// *                                                                  *
// * Description :                                                    *
// * This header declares the classes and functions used by the       *
// * different processes associated to the particles.                 *
// ********************************************************************


#ifndef G4MD_PhysicsList_h
#define G4MD_PhysicsList_h 1

#include <G4VUserPhysicsList.hh>

#include <globals.hh>



// Hadronics
#include "G4HadronElasticProcess.hh"
#include "G4HadronInelasticProcess.hh"
#include "G4HadronCaptureProcess.hh"
#include "G4HadronFissionProcess.hh"

// Inelastic Processes
#include "G4ProtonInelasticProcess.hh"
#include "G4AntiProtonInelasticProcess.hh"
#include "G4NeutronInelasticProcess.hh"
#include "G4AntiNeutronInelasticProcess.hh"
#include "G4PionPlusInelasticProcess.hh"
#include "G4PionMinusInelasticProcess.hh"
#include "G4KaonPlusInelasticProcess.hh"
#include "G4KaonZeroSInelasticProcess.hh"
#include "G4KaonZeroLInelasticProcess.hh"
#include "G4KaonMinusInelasticProcess.hh"
#include "G4DeuteronInelasticProcess.hh"
#include "G4TritonInelasticProcess.hh"
#include "G4AlphaInelasticProcess.hh"
#include "G4LambdaInelasticProcess.hh"
#include "G4AntiLambdaInelasticProcess.hh"
#include "G4OmegaMinusInelasticProcess.hh"
#include "G4AntiOmegaMinusInelasticProcess.hh"
#include "G4SigmaMinusInelasticProcess.hh"
#include "G4AntiSigmaMinusInelasticProcess.hh"
#include "G4SigmaPlusInelasticProcess.hh"
#include "G4AntiSigmaPlusInelasticProcess.hh"
#include "G4XiZeroInelasticProcess.hh"
#include "G4AntiXiZeroInelasticProcess.hh"
#include "G4XiMinusInelasticProcess.hh"
#include "G4AntiXiMinusInelasticProcess.hh"

// Low-energy Parameterised Models: 1 to 25 GeV
#include "G4LElastic.hh"
#include "G4LEPionPlusInelastic.hh"
#include "G4LEPionMinusInelastic.hh"
#include "G4LEKaonPlusInelastic.hh"
#include "G4LEKaonZeroSInelastic.hh"
#include "G4LEKaonZeroLInelastic.hh"
#include "G4LEKaonMinusInelastic.hh"
#include "G4LEProtonInelastic.hh"
#include "G4LEAntiProtonInelastic.hh"
#include "G4LENeutronInelastic.hh"
#include "G4LEAntiNeutronInelastic.hh"
#include "G4LEDeuteronInelastic.hh"
#include "G4LETritonInelastic.hh"
#include "G4LEAlphaInelastic.hh"
#include "G4LELambdaInelastic.hh"
#include "G4LEAntiLambdaInelastic.hh"
#include "G4LEOmegaMinusInelastic.hh"
#include "G4LEAntiOmegaMinusInelastic.hh"
#include "G4LESigmaMinusInelastic.hh"
#include "G4LEAntiSigmaMinusInelastic.hh"
#include "G4LESigmaPlusInelastic.hh"
#include "G4LEAntiSigmaPlusInelastic.hh"
#include "G4LEXiZeroInelastic.hh"
#include "G4LEAntiXiZeroInelastic.hh"
#include "G4LEXiMinusInelastic.hh"
#include "G4LEAntiXiMinusInelastic.hh"
// neutrons
#include "G4LCapture.hh"
#include "G4LFission.hh"

#include "G4HEAntiProtonInelastic.hh"
#include "G4HEAntiNeutronInelastic.hh"
#include "G4HELambdaInelastic.hh"
#include "G4HEAntiLambdaInelastic.hh"
#include "G4HEOmegaMinusInelastic.hh"
#include "G4HEAntiOmegaMinusInelastic.hh"
#include "G4HESigmaMinusInelastic.hh"
#include "G4HEAntiSigmaMinusInelastic.hh"
#include "G4HESigmaPlusInelastic.hh"
#include "G4HEAntiSigmaPlusInelastic.hh"
#include "G4HEXiZeroInelastic.hh"
#include "G4HEAntiXiZeroInelastic.hh"
#include "G4HEXiMinusInelastic.hh"
#include "G4HEAntiXiMinusInelastic.hh"

// Stopping processes
#include "G4PiMinusAbsorptionAtRest.hh"
#include "G4KaonMinusAbsorptionAtRest.hh"
#include "G4AntiProtonAnnihilationAtRest.hh"
#include "G4AntiNeutronAnnihilationAtRest.hh"

// Generator models: HE
#include "G4TheoFSGenerator.hh"
#include "G4Evaporation.hh"
#include "G4CompetitiveFission.hh"
#include "G4FermiBreakUp.hh"
#include "G4StatMF.hh"
#include "G4ExcitationHandler.hh"
#include "G4PreCompoundModel.hh"
#include "G4GeneratorPrecompoundInterface.hh"
#include "G4QGSModel.hh"
#include "G4QGSParticipants.hh"
#include "G4QGSMFragmentation.hh"
#include "G4ExcitedStringDecay.hh"

// Kinetic Model
#include "G4BinaryCascade.hh"
#include "G4BinaryLightIonReaction.hh"
#include "G4TripathiCrossSection.hh"
#include "G4IonsShenCrossSection.hh"


///////////////////////////
// ElectroNuclear Physics

// photonuclear and electronuclear reaction
#include "G4PhotoNuclearProcess.hh"
#include "G4ElectronNuclearProcess.hh"
#include "G4PositronNuclearProcess.hh"
#include "G4GammaNuclearReaction.hh"
#include "G4ElectroNuclearReaction.hh"

// CHIPS fragmentation model
#include "G4TheoFSGenerator.hh"
#include "G4StringChipsParticleLevelInterface.hh"
#include "G4QGSModel.hh"
#include "G4GammaParticipants.hh"
#include "G4QGSMFragmentation.hh"
#include "G4ExcitedStringDecay.hh"

// muon photonuclear reaction
#include "G4MuNuclearInteraction.hh"



class G4MD_PhysicsList: public G4VUserPhysicsList
{
public:
  G4MD_PhysicsList();
  virtual ~G4MD_PhysicsList();

protected:
  // General function to construct particles and physics 
  void ConstructParticle();
  void ConstructProcess();
  void SetCuts();

  // Construct particles
  void ConstructBosons();
  void ConstructLeptons();
  void ConstructMesons();
  void ConstructBaryons();
   
  // Construct physics processes and register them
  void ConstructDecay();
  void ConstructEM();
  void ConstructOp();
  void ConstructHadronicPhysics();
  void ConstructElectroNuclearPhysics();

public:
 // Set & Get cut values 
  void SetCutForGamma(G4double);
  void SetCutForElectron(G4double);
  void SetCutForProton(G4double);
  void SetCutForMuon(G4double);
  
  G4double GetCutForGamma() const;
  G4double GetCutForElectron() const;
  G4double GetCutForProton() const;
  G4double GetCutForMuon() const;


private:
  G4double cutForGamma;
  G4double cutForElectron;
  G4double cutForProton;
  G4double cutForMuon;
  G4double currentDefaultCut;
};

#endif



