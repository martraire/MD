// **********************************************************************
// * G4MD_PhysicsList.cc                                                *
// *                                                                    *
// * Date : 15 June 2015                                                *
// * Author : Diane Martraire (IHEP)                                    *
// *                                                                    *
// * Description :                                                      *
// * This file defines all the classes and funtions needed for the      *
// * physics processes but also the cuts for each particles.            *
// **********************************************************************

#include <G4MD_PhysicsList.hh>

#include <G4ParticleDefinition.hh>
#include <G4ParticleTypes.hh>
#include <G4ParticleTable.hh>

#include <G4ParticleWithCuts.hh>

#include <G4Material.hh>
#include <G4MaterialTable.hh>

#include <G4ProcessManager.hh>
#include <G4ProcessVector.hh>

#include <G4MuonMinusCaptureAtRest.hh>
#include <G4ios.hh>

// a modifier  ptet
#include <G4BosonConstructor.hh>
#include <G4LeptonConstructor.hh>
#include <G4MesonConstructor.hh>
#include <G4BosonConstructor.hh>
#include <G4BaryonConstructor.hh>
#include <G4IonConstructor.hh>


G4MD_PhysicsList::G4MD_PhysicsList():  G4VUserPhysicsList()
{
  currentDefaultCut = defaultCutValue = 1.0*mm;
  cutForGamma       = 50.*cm;
  cutForElectron    = 0.5*mm;
  cutForMuon        = 0.01*mm;
  cutForProton      = defaultCutValue;

  SetVerboseLevel(5);
}


G4MD_PhysicsList::~G4MD_PhysicsList()
{;}


void G4MD_PhysicsList::ConstructParticle()
{
  // In this method, static member functions should be called for all particles which you use.
  // This ensures that objects of these particle types will be created in the program. 
  
  G4BosonConstructor  pBosonConstructor;
  pBosonConstructor.ConstructParticle();

  G4LeptonConstructor pLeptonConstructor;
  pLeptonConstructor.ConstructParticle();

  G4MesonConstructor pMesonConstructor;
  pMesonConstructor.ConstructParticle();

  G4BaryonConstructor pBaryonConstructor;
  pBaryonConstructor.ConstructParticle();

}


void G4MD_PhysicsList::ConstructProcess()
{
  AddTransportation();
  ConstructDecay();
  ConstructEM();
  ConstructOp();
}


#include <G4Decay.hh>

void G4MD_PhysicsList::ConstructDecay()
{
  // Add Decay Process
  // -----------------
  G4Decay* theDecayProcess = new G4Decay();
  theParticleIterator->reset();
  while( (*theParticleIterator)() )
    {
      G4ParticleDefinition* particle = theParticleIterator->value();
      G4ProcessManager* pmanager = particle->GetProcessManager();
      if (theDecayProcess->IsApplicable(*particle)) 
	{ 
	  pmanager ->AddDiscreteProcess(theDecayProcess);
	  // set ordering for PostStepDoIt and AtRestDoIt
	  pmanager ->SetProcessOrdering(theDecayProcess, idxPostStep);
	  pmanager ->SetProcessOrdering(theDecayProcess, idxAtRest);
	}
    }
}


#include <G4ComptonScattering.hh>
#include <G4GammaConversion.hh>
#include <G4PhotoElectricEffect.hh>
#include <G4RayleighScattering.hh>

#include <G4eplusAnnihilation.hh>
#include <G4eIonisation.hh>
#include <G4eBremsstrahlung.hh>
#include <G4eMultipleScattering.hh>

#include <G4MuMultipleScattering.hh>
#include <G4MuIonisation.hh>
#include <G4MuBremsstrahlung.hh>
#include <G4MuPairProduction.hh>

#include <G4hMultipleScattering.hh>
#include <G4hBremsstrahlung.hh>
#include <G4hPairProduction.hh>
#include <G4hIonisation.hh>

#include <G4Cerenkov.hh>


void G4MD_PhysicsList::ConstructEM()
{
  theParticleIterator->reset();
  while( (*theParticleIterator)() ){
    G4ParticleDefinition* particle = theParticleIterator->value();
    G4ProcessManager* pmanager = particle->GetProcessManager();
    G4String particleName = particle->GetParticleName();
    
    // NB: At low energy, where dE/dX is large for the muon, it is best 
    // to set the following number to something low G4int MaxNumPhotons = 300;
    
    if (particleName == "gamma") {
      // gamma         
      pmanager->AddDiscreteProcess(new G4GammaConversion);
      pmanager->AddDiscreteProcess(new G4ComptonScattering);
      pmanager->AddDiscreteProcess(new G4RayleighScattering);
      pmanager->AddDiscreteProcess(new G4PhotoElectricEffect);
      
    } else if (particleName == "e-") {
      //electron
      pmanager->AddProcess(new G4eMultipleScattering, -1, 1, 1);
      pmanager->AddProcess(new G4eIonisation, -1, 2, 2);
      pmanager->AddProcess(new G4eBremsstrahlung, -1, 3, 3);      

    } else if (particleName == "e+") {
      //positron
      pmanager->AddProcess(new G4eMultipleScattering,-1, 1, 1);
      pmanager->AddProcess(new G4eIonisation,        -1, 2, 2);
      pmanager->AddProcess(new G4eBremsstrahlung,    -1, 3, 3);
      pmanager->AddProcess(new G4eplusAnnihilation,   0,-1, 4);
    
    } else if( particleName == "mu+" ||  particleName == "mu-"    ) {
      //muon  
      pmanager->AddProcess(new G4MuMultipleScattering,-1, 1, 1);
      pmanager->AddProcess(new G4MuIonisation,       -1, 2, 2);
      pmanager->AddProcess(new G4MuBremsstrahlung,   -1, 3, 3);
      pmanager->AddProcess(new G4MuPairProduction,   -1, 4, 4);
      pmanager->AddProcess(new G4MuonMinusCaptureAtRest, 0,-1,-1);
             
    } else if( particleName == "proton" ||
	       particleName == "pi-" ||
	       particleName == "pi+"    ) {
      // proton  
      pmanager->AddProcess(new G4hMultipleScattering, -1, 1, 1);
      pmanager->AddProcess(new G4hIonisation,         -1, 2, 2);
      pmanager->AddProcess(new G4hBremsstrahlung,     -1, 3, 3);
      pmanager->AddProcess(new G4hPairProduction,     -1, 4, 4); 
      
    } else if ((!particle->IsShortLived()) &&
	       (particle->GetPDGCharge() != 0.0) &&
	       (particle->GetParticleName() != "chargedgeantino")) {
      // all others charged particles except geantino
      pmanager->AddProcess(new G4hMultipleScattering, -1, 1, 1);
      pmanager->AddProcess(new G4hIonisation,         -1, 2, 2);
    }
  }
}

#include <G4Cerenkov.hh>
#include <G4OpAbsorption.hh>
#include <G4OpRayleigh.hh>
#include <G4OpBoundaryProcess.hh>

void G4MD_PhysicsList::ConstructOp()
{  
    G4Cerenkov*  theCerenkovProcess = new G4Cerenkov("Cerenkov");
    G4OpAbsorption* theAbsorptionProcess         = new G4OpAbsorption();
    G4OpRayleigh*   theRayleighScatteringProcess = new G4OpRayleigh(); 
    G4OpBoundaryProcess* theBoundaryProcess      = new G4OpBoundaryProcess();
 
    theCerenkovProcess->SetMaxNumPhotonsPerStep(300);
    theCerenkovProcess->SetTrackSecondariesFirst(true);
    // ATTENTION : Isn't this set in the DetectorConstruction as well?
    G4OpticalSurfaceModel themodel = unified;
    theBoundaryProcess->SetModel(themodel);
  
    theParticleIterator->reset();
    while( (*theParticleIterator)() ) {      
	  G4ParticleDefinition* particle = theParticleIterator->value();
	  G4ProcessManager* pmanager = particle->GetProcessManager();
	  G4String particleName = particle->GetParticleName();
   
          if (theCerenkovProcess->IsApplicable(*particle)) {
             //pmanager->AddContinuousProcess(theCerenkovProcess);
             pmanager->AddProcess(theCerenkovProcess);
             pmanager->SetProcessOrdering(theCerenkovProcess,idxPostStep);
          }
	  if (particleName == "opticalphoton") {
	     G4cout << " AddDiscreteProcess to OpticalPhoton "<< G4endl;
	    pmanager->AddDiscreteProcess(theAbsorptionProcess);
	    pmanager->AddDiscreteProcess(theRayleighScatteringProcess);
	    pmanager->AddDiscreteProcess(theBoundaryProcess);
	  }
    }
}


void G4MD_PhysicsList::SetCuts()
{
  //SetCutsWithDefault();   
  // reactualise cutValues
  if (currentDefaultCut != defaultCutValue)
    {
     if(cutForGamma    == currentDefaultCut) cutForGamma    = defaultCutValue;
     if(cutForElectron == currentDefaultCut) cutForElectron = defaultCutValue;
     if(cutForProton   == currentDefaultCut) cutForProton   = defaultCutValue;
     if(cutForMuon     == currentDefaultCut) cutForMuon     = defaultCutValue;
     currentDefaultCut = defaultCutValue;
    }

  if (verboseLevel > 0){
    G4cout << "G4MD_PhysicsList::SetCuts:";
    G4cout << "CutLength : " << G4BestUnit(defaultCutValue,"Length") << G4endl;

    // Set cut values for gamma at first and for e- second and next for e+,
    // because some processes for e+/e- need cut values for gamma
    SetCutValue(cutForGamma, "gamma");
    SetCutValue(cutForElectron, "e-");
    SetCutValue(cutForElectron, "e+");
    SetCutValue(cutForMuon, "mu-");
    SetCutValue(cutForMuon, "mu+");
    
    // Set cut values for proton and anti_proton before all other hadrons
    // because some processes for hadrons need cut values for proton/anti_proton 
    SetCutValue(cutForProton, "proton");
    SetCutValue(cutForProton, "anti_proton");
  }    

  DumpCutValuesTable();
}



void G4MD_PhysicsList::SetCutForGamma(G4double cut)
{
  ResetCuts();
  cutForGamma = cut;
}

void G4MD_PhysicsList::SetCutForElectron(G4double cut)
{
  ResetCuts();
  cutForElectron = cut;
}

void G4MD_PhysicsList::SetCutForProton(G4double cut)
{
  ResetCuts();
  cutForProton = cut;
}

void G4MD_PhysicsList::SetCutForMuon(G4double cut)
{
  ResetCuts();
  cutForMuon = cut;
}

G4double G4MD_PhysicsList::GetCutForGamma() const
{
  return cutForGamma;
}

G4double G4MD_PhysicsList::GetCutForElectron() const
{
  return cutForElectron;
}

G4double G4MD_PhysicsList::GetCutForProton() const
{
  return cutForProton;
}

G4double G4MD_PhysicsList::GetCutForMuon() const
{
  return cutForMuon;
}

