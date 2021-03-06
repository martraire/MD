// ***********************************************************************
// * G4MD_PrimaryGeneratorAction.cc                                      *
// *                                                                     *
// * Date : 15 June 2015                                                 *
// * Author : Diane Martraire (IHEP)                                     *
// *                                                                     *
// * Description :                                                       *
// * This file defines the generator (ID particle, Nb, E, position, dir) *
// ***********************************************************************


#include <G4MD_PrimaryGeneratorAction.hh>
#include <G4MD_PrimaryGeneratorMessenger.hh>
#include <G4MD_EventAction.hh>
#include <G4MD_ParticleIDManager.h>
#include <G4MD_DetectorConstruction.hh>

#include <G4Event.hh>
#include <G4ParticleGun.hh>
#include <G4ParticleTable.hh>
#include <G4ParticleDefinition.hh>
#include <G4Point3D.hh>
#include <G4Vector3D.hh>
#include <G4Transform3D.hh>
#include <G4UnitsTable.hh>
#include <G4RunManager.hh>
#include <G4String.hh>
#include <math.h>
#include <Reader.h>
#include <iostream>
#include <stdlib.h>
#include <cmath>
#include <Randomize.hh>
#include <TH1F.h>
#include <TH1.h>
#include <TH2.h>
#include <TFile.h>
#include <TRandom.h>

G4int G4MD_PrimaryGeneratorAction:: isample=0;
G4int G4MD_PrimaryGeneratorAction:: nsample=0;
G4int G4MD_PrimaryGeneratorAction:: fParticleID=0;

G4double G4MD_PrimaryGeneratorAction:: particle_ekin=0.;
G4double G4MD_PrimaryGeneratorAction:: sample_theta=0.;
G4double G4MD_PrimaryGeneratorAction:: sample_phi=0.;

G4double G4MD_PrimaryGeneratorAction:: xin=0.;
G4double G4MD_PrimaryGeneratorAction:: yin=0.;
G4double G4MD_PrimaryGeneratorAction:: zin=0.;


G4MD_PrimaryGeneratorAction::G4MD_PrimaryGeneratorAction()
{
  // Create a messenger for this class
  // ---------------------------------
  gunMessenger = new G4MD_PrimaryGeneratorMessenger(this);


  // Fetch parameters from injector.xml file
  // ---------------------------------------
  bool validate = false;
  
  Reader controlReader("xml/control.xml", validate);
  Branch tB = controlReader.GetTopBranch().GetChild("controlParameters");
  
  G4String injectorFile;
  tB.GetChild("injectorFile").GetData(injectorFile);
  Reader reader(injectorFile, validate);
  Branch branch = reader.GetTopBranch().GetChild("injectorParameters");
  
  G4int numberofparticles;
  branch.GetChild("numberofparticles").GetData(numberofparticles);
  
  G4String particleName;
  branch.GetChild("particleName").GetData(particleName);
  fParticleName = particleName;    
  G4cout << "particle name = "<< fParticleName << G4endl;

  G4int mode;
  branch.GetChild("mode").GetData(mode);
  fMode = mode;
  
  G4double particleEnergy;
  branch.GetChild("particleEnergy").GetData(particleEnergy);
  fParticleEnergy = particleEnergy*eV; // Attention ev -> GeV !!
  //fParticleEnergy = 30.*GeV;

  G4double particleX;
  G4double particleY;
  G4double particleZ;
    
  branch.GetChild("particleX").GetData(particleX);
  branch.GetChild("particleY").GetData(particleY);
  branch.GetChild("particleZ").GetData(particleZ);
  
  fParticleX = particleX*m;
  fParticleY = particleY*m;
  fParticleZ = particleZ*m;
  
  G4double particlePX;
  G4double particlePY;
  G4double particlePZ;

  branch.GetChild("particlePX").GetData(particlePX);
  branch.GetChild("particlePY").GetData(particlePY);
  branch.GetChild("particlePZ").GetData(particlePZ);
  
  fParticlePX = particlePX;
  fParticlePY = particlePY;
  fParticlePZ = particlePZ;
  
  G4double particleTheta;
  branch.GetChild("particleTheta").GetData(particleTheta);
  fParticleTheta = particleTheta;

  G4double particleTime;
  branch.GetChild("particleTime").GetData(particleTime);	
  fParticleTime = particleTime*ns;
  
  
  // Define generator
  // ----------------
  // if(fMode == 5)
//     fParticleGun2 = new G4GeneralParticleSource();
//   else
//     {
      fParticleGun = new G4ParticleGun(numberofparticles);
      
      // Default particle kinematic
      G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
      G4ParticleDefinition* particle = particleTable->FindParticle(fParticleName);
      fParticleGun->SetParticleDefinition(particle);
      //if(fMode != 1 || fMode != 2)
      fParticleGun->SetParticleEnergy(fParticleEnergy);
      //}
}


G4MD_PrimaryGeneratorAction::~G4MD_PrimaryGeneratorAction()
{
  delete fParticleGun;
  //delete fParticleGun2;
  delete gunMessenger;
}


void G4MD_PrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{
  // fParticleGun->GeneratePrimaryVertex(anEvent);
  G4double x = fParticleX;
  G4double y = fParticleY;
  G4double z = fParticleZ;
  
  G4double px = fParticlePX;
  G4double py = fParticlePY;
  G4double pz = fParticlePZ;

  G4double theta = fParticleTheta;
  G4double phi;
  G4double r, i;
  
  G4double radius_tank = 3.4*m;
  
  // Mode 1 : VEM & center of the tank (vertical muon with an energy of 1 GeV)
  if (fMode == 1)
    {
      fParticleGun->SetParticleEnergy(1.*GeV);
      
      theta = 0.;
      phi = 2 * 3.14 * G4UniformRand();
      px = sin(theta) * cos(phi);
      py = sin(theta) * sin(phi);
      pz = -cos(theta);
      fParticleGun->SetParticlePosition(G4ThreeVector(x,y,z));
      
      fParticleGun->SetParticleMomentumDirection(G4ThreeVector(px,py,pz));
      fParticleGun->GeneratePrimaryVertex(anEvent);
      
      G4cout << "###### INPUT G1 : E = " << fParticleEnergy  << " & (x,y,z) = (" << x << ", " << y << ", " << z << ") & (px,py,pz) =  (" << px << ", " << py << ", " << pz << ")" << G4endl;     
    }
  
  // Mode 2 : VEM (Vertical muon with an energy of 1 GeV, random position & direction)
  else if (fMode == 2) 
    {
      fParticleGun->SetParticleEnergy(1.*GeV);
      
      r = radius_tank * G4UniformRand();
      i = 2 * 3.14 * G4UniformRand();
      x = r * cos(i);
      y = r * sin(i);
      fParticleGun->SetParticlePosition(G4ThreeVector(x,y,z));
      
      theta = 0.;
      phi = 2 * 3.14 * G4UniformRand();
      px = sin(theta) * cos(phi);
      py = sin(theta) * sin(phi);
      pz = -cos(theta);
      fParticleGun->SetParticleMomentumDirection(G4ThreeVector(px,py,pz));
      
      fParticleGun->GeneratePrimaryVertex(anEvent);
      
      G4cout << "###### INPUT G2 : E = " << fParticleEnergy  << " & (x,y,z) = (" << x << ", " << y << ", " << z << ") & (px,py,pz) =  (" << px << ", " << py << ", " << pz << ")" << G4endl;
    }
  
  // Mode 3 : Theta fixed, Energy fixed, Random position & direction
  else if (fMode == 3)
    {
      r = radius_tank * G4UniformRand();
      i = 2 * 3.14 * G4UniformRand();
      x = r * cos(i);
      y = r * sin(i);
      fParticleGun->SetParticlePosition(G4ThreeVector(x,y,z));
      
      theta = theta * 3.14 / 180.; 
      phi = 2 * 3.14 * G4UniformRand();
      px = sin(theta) * cos(phi);
      py = sin(theta) * sin(phi);
      pz = -cos(theta);
      fParticleGun->SetParticleMomentumDirection(G4ThreeVector(px,py,pz));
      
      fParticleGun->GeneratePrimaryVertex(anEvent);
      
      G4cout << "###### INPUT G3 : E = " << fParticleEnergy  << " & (x,y,z) = (" << x << ", " << y << ", " << z << ") & (px,py,pz) =  (" << px << ", " << py << ", " << pz << ")" << G4endl;     

    }
  
  // Mode 4 : Theta Random, Energy Random, Random position & direction
  else if (fMode == 4)
    {
      r = radius_tank * G4UniformRand();
      i = 2 * 3.14 * G4UniformRand();
      x = r * cos(i);
      y = r * sin(i);
      fParticleGun->SetParticlePosition(G4ThreeVector(x,y,z));
      
      theta = 3.14 * G4UniformRand(); 
      phi = 2 * 3.14 * G4UniformRand();
      px = sin(theta) * cos(phi);
      py = sin(theta) * sin(phi);
      pz = -cos(theta);
      fParticleGun->SetParticleMomentumDirection(G4ThreeVector(px,py,pz));
      
      fParticleGun->GeneratePrimaryVertex(anEvent);
      
      G4cout << "###### INPUT G4 : E = " << fParticleEnergy  << " & (x,y,z) = (" << x << ", " << y << ", " << z << ") & (px,py,pz) =  (" << px << ", " << py << ", " << pz << ")" << G4endl;     
    }

  // Mode 5 : Random everything and different for each particle (via vis.mac)
  else if(fMode == 5)
    {
      fParticleGun2->GeneratePrimaryVertex(anEvent);
     
      G4cout << "###### INPUT G5 : E = " << fParticleEnergy  << " & (x,y,z) = (" << x << ", " << y << ", " << z << ") & (px,py,pz) =  (" << px << ", " << py << ", " << pz << ")" << G4endl; }
  
}




void G4MD_PrimaryGeneratorAction::PrepareCores() 
{  
  // Uniform distribution in xy
  // --------------------------
  G4double y_max = 75.*m;
  G4double x_max = 75.*m;
  shift_core = G4Vector3D(0,0,0);
  // G4Vector3D((2*G4UniformRand()-1)*x_max ,(2*G4UniformRand()-1)*y_max,0);
}

