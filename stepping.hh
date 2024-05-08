#ifndef STEPPING_HH
#define STEPPING_HH

#include "G4UserSteppingAction.hh"
#include "G4Step.hh"

#include "construction.hh"
#include "event.hh"

class MysteppingAction : public G4UserSteppingAction
{

public:
    MysteppingAction(MyEventAction* eventAction);
    ~MysteppingAction();

    virtual void UserSteppingAction(const G4Step*);
    
private:
    MyEventAction *fEventAction;
};


#endif