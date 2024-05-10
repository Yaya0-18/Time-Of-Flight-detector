#include "generator.hh"

MyPrimaryGenerator::MyPrimaryGenerator()
{
    fparticleGun = new G4ParticleGun(1);

    //define the particle
    G4ParticleTable *particleTable = G4ParticleTable::GetParticleTable();
    G4String particleName = "proton";
    G4ParticleDefinition *particle = particleTable->FindParticle("proton");

    //define the place where particle should be created

        //define postion and momentum
        G4ThreeVector pos(0.,0.,0.);
        G4ThreeVector mom(0.,0.,1.);

        fparticleGun->SetParticlePosition(pos);
        fparticleGun->SetParticleMomentumDirection(mom);
        fparticleGun->SetParticleMomentum(100.*GeV);
        fparticleGun->SetParticleDefinition(particle);

}

MyPrimaryGenerator::~MyPrimaryGenerator()
{
    delete fparticleGun;
}

void MyPrimaryGenerator::GeneratePrimaries(G4Event *anEvent)
{
    //G4ParticleDefinition *particle = fparticleGun->GetParticleDefinition();


    //tell Geant4 to generate 
    fparticleGun->GeneratePrimaryVertex(anEvent);




}