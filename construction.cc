#include "construction.hh"

MyDetectorConstruction::MyDetectorConstruction()
{   
    nCols = 100; nRows=100;

    fMessenger = new G4GenericMessenger(this, "/detector/","Detector Construction");

    fMessenger->DeclareProperty("nCols", nCols, "Number of columns");
    fMessenger->DeclareProperty("nRows", nRows, "Number of rows");
    fMessenger->DeclareProperty("isCherenkov", isCherenkov, "Construct Cherenkov detector");

    DefineMaterial();

    isCherenkov = true;

    //define the size of world volume 
        //determine the dimension for x y and z
            xWorld = 0.5*m ;
            yWorld = 0.5*m  ;
            zWorld = 0.5*m ;

}

MyDetectorConstruction::~MyDetectorConstruction()
{}

void MyDetectorConstruction::DefineMaterial()
{
    G4NistManager *nist = G4NistManager::Instance();
////////////////////////////////////////////////////////////////////////////////////////////////////

//Material
    //Aerogel material 

        //starting with defining its components:

            //defining the silica

            SiO2 = new G4Material("SiO2",2.201 *g/cm3 , 2);
            SiO2->AddElement(nist->FindOrBuildElement("Si"),1);
            SiO2->AddElement(nist->FindOrBuildElement("O"), 2);

            //defining the water material 
            H2O = new G4Material("H2O",1.000 *g/cm3 , 2);
            H2O->AddElement(nist->FindOrBuildElement("H"),2);
            H2O->AddElement(nist->FindOrBuildElement("O"),1);  

            //defining the carboon element
            C = nist->FindOrBuildElement("C");

            //creating the Aerogel
            Aerogel = new G4Material("Aerogel",0.200*g/cm3,3);
            Aerogel->AddMaterial(SiO2,62.5*perCent);
            Aerogel->AddMaterial(H2O,37.4*perCent);
            Aerogel->AddElement(C,0.1*perCent);

        //Adding the optical parameter to the material

            //Defining parameters
                G4double energy[2] = {1.239841939*eV/0.9,1.239841939*eV/0.2};
                G4double rindexAerogel[2]= {1.1,1.1};

            //Add refractive index to the material
                G4MaterialPropertiesTable *mptAerogel = new G4MaterialPropertiesTable();
                mptAerogel->AddProperty("RINDEX",energy,rindexAerogel,2);
                Aerogel->SetMaterialPropertiesTable(mptAerogel);

    //World Material 
        //Defining the world Material
            worldMat = nist->FindOrBuildMaterial("G4_AIR");
        //Adding the optical parameter to the material

            //Defining parameters
                G4double rindexWorld[2]= {1.0,1.0};

            //Add refractive index to the material
                G4MaterialPropertiesTable *mptWorld = new G4MaterialPropertiesTable();
                mptWorld->AddProperty("RINDEX",energy,rindexWorld,2);
                worldMat->SetMaterialPropertiesTable(mptWorld);


}

void MyDetectorConstruction::ConstructCherenkove()
{
       //Creating the radiator (the optical detector)
        solidRadiator = new G4Box("solidRadiator",0.4*m,0.4*m,0.01*m);
        logicRadiator = new G4LogicalVolume(solidRadiator,Aerogel,"logicalRadiator");
        fScoringVolume = logicRadiator;
        physRadiator = new G4PVPlacement(0,G4ThreeVector(0.,0.,0.25*m),logicRadiator,"physRadiator",logicWorld,false,0,true);

    //Create the photon sensors /detector
        //First we define each detector how it looks like 

        solidDetector = new G4Box("solidDetector",xWorld/nRows,yWorld/nCols,0.01*m);
        //sensitive volume have to refer to this logical volume so we need to access it outside this constructor 
        logicDetector = new G4LogicalVolume(solidDetector,worldMat,"logicDetector");
        //Defining physical volume by for loop
        for(G4int i = 0; i < nRows ;i++)
        {
            for(G4int j = 0 ; j < nCols ; j++)
            {
                physDetector = new G4PVPlacement(0,G4ThreeVector(-0.5*m+(i+0.5)*m/nRows,-0.5*m+(j+0.5)*m/nCols,0.49*m),logicDetector,"physDetector",logicWorld,false ,j+i*nCols,true);
            } 
        }
}

G4VPhysicalVolume *MyDetectorConstruction::Construct()
{
   
//Boxes
    //Creating the room / the mother box which contain Everything


        solidWorld = new G4Box("solidWorld",xWorld,yWorld,zWorld);
        logicWorld = new G4LogicalVolume(solidWorld,worldMat,"logicWorld");
        physWorld = new G4PVPlacement(0,G4ThreeVector(0.,0.,0.),logicWorld,"physWorld",0,false,0,true);

    if(isCherenkov)
        ConstructCherenkove();

    return physWorld;
}

void MyDetectorConstruction::ConstructSDandField()
{
    MySensitiveDetector *sensDet = new MySensitiveDetector("SensitiveDetector");
    
    logicDetector->SetSensitiveDetector(sensDet);
        
}