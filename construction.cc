#include "construction.hh"

MyDetectorConstruction::MyDetectorConstruction()
{   
    nCols = 100; nRows=100;

    fMessenger = new G4GenericMessenger(this, "/detector/","Detector Construction");

    fMessenger->DeclareProperty("nCols", nCols, "Number of columns");
    fMessenger->DeclareProperty("nRows", nRows, "Number of rows");
    
    DefineMaterial();

    isTOF = true;

    //define the size of world volume 
        //determine the dimension for x y and z
            xWorld = 5*m ;
            yWorld = 5*m  ;
            zWorld = 5*m ;

}

MyDetectorConstruction::~MyDetectorConstruction()
{}

void MyDetectorConstruction::DefineMaterial()
{
    G4NistManager *nist = G4NistManager::Instance();
////////////////////////////////////////////////////////////////////////////////////////////////////

//Material

    //World Material 
        //Defining the world Material
            worldMat = nist->FindOrBuildMaterial("G4_AIR");
        //Adding the optical parameter to the material

            //Defining parameters
                G4double energy[2] = {1.239841939*eV/0.9,1.239841939*eV/0.2};

                G4double rindexWorld[2]= {1.0,1.0};

            //Add refractive index to the material
                G4MaterialPropertiesTable *mptWorld = new G4MaterialPropertiesTable();
                mptWorld->AddProperty("RINDEX",energy,rindexWorld,2);
                worldMat->SetMaterialPropertiesTable(mptWorld);



}

void MyDetectorConstruction::ConstructTOF()
{
    solidDetector = new G4Box("solidDetector",1.*m,1.*m,0.1*m);
    logicDetector = new G4LogicalVolume(solidDetector,worldMat , "logicDetector");

    physDetector = new G4PVPlacement(0,G4ThreeVector(0.*m,0.*m,-4.*m),logicDetector,"physDetector", logicWorld , false , 0, true);

    physDetector = new G4PVPlacement(0,G4ThreeVector(0.*m,0.*m,3.*m),logicDetector,"physDetector", logicWorld , false , 1, true);


}

G4VPhysicalVolume *MyDetectorConstruction::Construct()
{
   
//Boxes
    //Creating the room / the mother box which contain Everything


        solidWorld = new G4Box("solidWorld",xWorld,yWorld,zWorld);
        logicWorld = new G4LogicalVolume(solidWorld,worldMat,"logicWorld");
        physWorld = new G4PVPlacement(0,G4ThreeVector(0.,0.,0.),logicWorld,"physWorld",0,false,0,true);

    if(isTOF)
        ConstructTOF();

    return physWorld;
}

void MyDetectorConstruction::ConstructSDandField()
{

    MySensitiveDetector *sensDet = new MySensitiveDetector("SensitiveDetector");

    
    logicDetector->SetSensitiveDetector(sensDet);
        
}