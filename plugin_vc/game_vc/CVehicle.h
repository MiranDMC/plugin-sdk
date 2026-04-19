/*
Plugin-SDK (Grand Theft Auto Vice City) header file
Authors: GTA Community. See more here
https://github.com/DK22Pac/plugin-sdk
Do not delete this comment block. Respect others' work!
*/
#pragma once
#include "PluginBase.h"
#include "CAutoPilot.h"
#include "CPhysical.h"
#include "CStoredCollPoly.h"
#include "eComedyControlState.h"
#include "eDoors.h"
#include "eVehicleCreatedBy.h"
#include "eWeaponType.h"
#include "eWheelState.h"
#include "eWheelStatus.h"
#include "tBikeHandlingData.h"
#include "tFlyingHandlingData.h"
#include "tHandlingData.h"

class CColModel;
class CCopPed;
class CFire;
class CPed;
class CPlayerPed;

enum eBombState {
    BOMB_DETONATED = 0,
    BOMB_READY = 3,
    BOMB_NOT_SET = 8,
    BOMB_SET = 11
};

enum eCarWeapon {

};

enum eDoorLock {
    DOORLOCK_NOT_USED,
    DOORLOCK_UNLOCKED,
    DOORLOCK_LOCKED,
    DOORLOCK_LOCKOUT_PLAYER_ONLY,
    DOORLOCK_LOCKED_PLAYER_INSIDE,
    DOORLOCK_COP_CAR,
    DOORLOCK_FORCE_SHUT_DOORS,
    DOORLOCK_SKIP_SHUT_DOORS
};

enum eVehicleApperance {
    VEHICLE_APPEARANCE_AUTOMOBILE = 1,
    VEHICLE_APPEARANCE_BIKE,
    VEHICLE_APPEARANCE_HELI,
    VEHICLE_APPEARANCE_BOAT,
    VEHICLE_APPEARANCE_PLANE,
};

enum eVehicleLightsFlags {

};

// TODO enums
typedef int eOrdnanceType;
typedef int eFlightModel;
typedef int eBikeWheelSpecial;

class CVehicle : public CPhysical {
protected:
    CVehicle(plugin::dummy_func_t) : CPhysical(plugin::dummy) {}
public:
    tHandlingData*       m_pHandlingData;
    tFlyingHandlingData* m_pFlyingHandling;

    CAutoPilot m_autoPilot;
    CVehicle*  m_pVehicleToRam;

    uint8_t m_nPrimaryColor;
    uint8_t m_nSecondaryColor;
    uint8_t m_anExtras[2]; // idx of random variation elements some vehicles have

    int16_t m_nAlarmState; // -1 armed, 0 off, positive values are remaining time of triggered alarm
    uint16_t m_nRandomSeed; // if this is non-zero the random wander gets deterministic

    CPed*   m_pDriver;
    CPed*   m_apPassengers[8];
    uint8_t m_nNumPassengers;
    uint8_t m_nNumGettingIn;
    uint8_t m_nGettingInFlags;
    uint8_t m_nGettingOutFlags;
    uint8_t m_nMaxPassengers;

private:
    float field_1D0[4]; // unknown
public:

    CEntity* m_standingOnEntity;
    CFire* m_pFire;

    float m_fSteerAngle;
    float m_fGasPedal;
    float m_fBreakPedal;

    eVehicleCreatedBy m_nCreatedBy;

    union {
        uint8_t m_nVehicleFlags[6];
        struct {
            bool bIsLawEnforcer : 1;        // is this guy chasing the player at the moment
            bool bIsAmbulanceOnDuty : 1;    // ambulance trying to get to an accident
            bool bIsFireTruckOnDuty : 1;    // firetruck trying to get to a fire
            bool bIsLocked : 1;             // is this guy locked by the script (cannot be removed)
            bool bEngineOn : 1;             // for sound purposes. Parked cars have their engines switched off (so do destroyed cars)
            bool bIsHandbrakeOn : 1;        // how's the handbrake doing?
            bool bLightsOn : 1;             // are the lights switched on?
            bool bFreebies : 1;             // any freebies left in this vehicle?

            bool bIsVan : 1;                // is this vehicle a van (doors at back of vehicle)
            bool bIsBus : 1;                // is this vehicle a bus
            bool bIsBig : 1;                // is this vehicle a bus
            bool bLowVehicle : 1;           // need this for sporty type cars to use low getting-in/out anims
            bool bComedyControls : 1;       // simulates damaged steering by adding extra random control inputs
            bool bWarnedPeds : 1;           // has scan and warn peds of danger been processed?
            bool bCraneMessageDone : 1;     // a crane message has been printed for this car allready
            bool bExtendedRange : 1;        // vehicle needs to be a bit further away to get deleted

            bool bTakeLessDamage : 1;       // this vehicle is stronger (takes about 1/4 of damage)
            bool bIsDamaged : 1;            // this vehicle has been damaged and is displaying all its components
            bool bHasBeenOwnedByPlayer : 1; // player entering it is not a crime
            bool bFadeOut : 1;              // fade vehicle out
            bool bIsBeingCarJacked : 1;     // fade vehicle out
            bool bCreateRoadBlockPeds : 1;  // if this vehicle gets close enough we will create peds (coppers or gang members) round it
            bool bCanBeDamaged : 1;         // set to FALSE during cut scenes to avoid explosions
            bool bUsePlayerColModel : 1;    // is player vehicle using special collision model, stored in player strucure

            bool bOccupantsGenerated : 1;   // is true if the occupants have already been generated (shouldn't happen again)
            bool bGunSwitchedOff : 1;       // level designers can use this to switch off guns on boats
            bool bVehicleColProcessed : 1;  // has ProcessEntityCollision been processed for this car?
            bool bIsCarParkVehicle : 1;     // car has been created using the special CAR_PARK script command
            bool bHasAlreadyRecorded : 1;   // used for replays
            bool bPartOfConvoy : 1;
            bool bHeliMinimumTilt : 1;      // this heli should have almost no tilt really
            bool bAudioChangingGear : 1;    // sounds like vehicle is changing gear
    
            bool bIsDrowning : 1;           // is vehicle occupants taking damage in water (i.e. vehicle is dead in water)
            bool bTyresDontBurst : 1;       // if this is set the tyres are invincible
            bool bCreatedAsPoliceVehicle : 1;// true if this guy was created as a police vehicle (enforcer, policecar, miamivice car etc)
            bool bRestingOnPhysical : 1;    // don't go static cause car is sitting on a physical object that might get removed
            bool bParking : 1;
            bool bCanPark : 1;
            bool bDriverLastFrame : 1;      // was there a driver present last frame?
        };
    };

    uint8_t m_numPedsUseItAsCover; // number of peds hiding/attempting to hide behind it
    uint8_t m_nAmmoInClip; // used to make the guns onboard do a reload
    uint8_t m_nPacManPickupsCollected;

    uint8_t m_nRoadblockType;

    float m_fHealth; // 1000 = full, 250 = fire, 0 = explode

    uint8_t m_nCurrentGear;
    float   m_fChangeGearTime; // used as parameter for cTransmission::CalculateDriveAcceleration, but doesn't change

    CEntity* m_pBombRigger;
    uint32_t m_nTimeTillWeNeedThisCar; // don't remove before this timestamp
    uint32_t m_nGunFiringTime; // last time when gun was fired
    uint32_t m_nTimeOfDeath; // explosion timestamp. Game will try to delete vehicle after additional 60 seconds
    uint16_t m_nTimeBlocked; // time not moving. If reaches 2500 cops chassing this leave their vehicles
    uint16_t m_nBombTimer; // goes down with each frame
    CEntity* m_pBlowUpEntity; // m_pBombRigger or explosive thrower

    float m_fVehicleFrontGroundZ; // values from CCollision::IsStoredPolyStillValidVerticalLine
    float m_fVehicleRearGroundZ;  // or CWorld::ProcessVerticalLine

    eDoorLock m_eDoorLock;

    int8_t   m_nLastWeaponDamage; // see eWeaponType, -1 if no damage
    CEntity* pLastDamEntity;

    uint8_t m_nRadioStation;
    uint8_t m_nRainAudioCounter;
    uint8_t m_nRainSamplesCounter;

    uint32_t m_nHornCounter; // frames count until honking stops
    uint8_t  m_nHornPattern;
    bool     m_bSirenOrAlarm;
    uint8_t  m_nHornDelay;

    eComedyControlState m_comedyControlState;

    CStoredCollPoly m_frontCollPoly; // poly which is under front part of car
    CStoredCollPoly m_rearCollPoly; // poly which is under rear part of car

    float m_fSteerInput;
    eVehicleType m_nVehicleClass;

    // functions
    static void* operator new(unsigned int size);
    static void* operator new(unsigned int size, int arg1);
    static void operator delete(void* data);

    CVehicle(const CVehicle &) = delete;
    CVehicle &operator=(const CVehicle &) = delete;

    void ProcessControlInputs(uint8_t playerNum);

    void GetComponentWorldPosition(int componentId, CVector& posnOut); // component index in m_apModelNodes array
    bool IsComponentPresent(int componentId); // component index in m_apModelNodes array
    void SetComponentRotation(int componentId, CVector arg1);
 
    void OpenDoor(int componentId, eDoors door, float doorOpenRatio);
    void ProcessOpenDoor(unsigned int arg0, unsigned int arg1, float arg2);
    bool IsDoorReady(eDoors door);
    bool IsDoorFullyOpen(eDoors door);
    bool IsDoorClosed(eDoors door);
    bool IsDoorMissing(eDoors door);
    bool IsDoorReady(unsigned int door);
    bool IsDoorMissing(unsigned int door);
    bool IsOpenTopCar(); // check if car has roof as extra
 
    void RemoveRefsToVehicle(CEntity* entity); // remove ref to this entity

    void BlowUpCar(CEntity* damager);

    bool SetUpWheelColModel(CColModel* wheelCol);
    bool BurstTyre(uint8_t tyreComponentId, bool bPhysicalEffect); // false if it's not possible to burst tyre. bPhysicalEffect applies random moving force
    
    bool IsRoomForPedToLeaveCar(unsigned int arg0, CVector* arg1);
    bool IsClearToDriveAway();
    float GetHeightAboveRoad();

    void PlayCarHorn();

    void ActivateBomb();
    void ActivateBombWhenEntered();

    bool AddPassenger(CPed* passenger);
    bool AddPassenger(CPed* passenger, uint8_t seatNumber);

    void BladeColSectorList(CPtrList& ptrList, CColModel& colModel, CMatrix& matrix, short arg3, float arg4);
    bool CanBeDeleted();
    bool CanDoorsBeDamaged();
    bool CanPedEnterCar();
    bool CanPedExitCar(bool arg0);
    bool CanPedJumpOffBike();
    bool CanPedJumpOutCar();
    bool CanPedOpenLocks(CPed* ped);
    bool CarHasRoof();
    void ChangeLawEnforcerState(uint8_t state);
    void DoBladeCollision(CVector arg0, CMatrix& matrix, short arg2, float arg3, float arg4);
    void DoFixedMachineGuns();
    void DoSunGlare();
    void ExtinguishCarFire();
    int FindTyreNearestPoint(float x, float y); // return nearest wheel?
    void FireFixedMachineGuns();
    void FlyingControl(eFlightModel flightModel);
    eVehicleApperance GetVehicleAppearance();
    static void HeliDustGenerate(CEntity* arg0, float arg1, float arg2, int arg3);
    void InflictDamage(CEntity* damager, eWeaponType weapon, float intensity, CVector coords);
    bool IsDriver(CPed* ped);
    bool IsDriver(int modelIndex);
    bool IsLawEnforcementVehicle();
    bool IsOnItsSide(); // return this->m_pCoords->matrix.right.z >= 0.8 || this->m_pCoords->matrix.right.z <= -0.8;
    bool IsPassenger(CPed* ped);
    bool IsPassenger(int modelIndex);
    bool IsSphereTouchingVehicle(float x, float y, float z, float radius);
    bool IsUpsideDown(); // return this->m_pCoords->matrix.at.z <= -0.9;
    bool IsVehicleNormal();
    void KillPedsInVehicle();
    void MakeNonDraggedPedsLeaveVehicle(CPed* arg0, CPed* arg1, CPlayerPed*& arg2, CCopPed*& arg3);
    void ProcessBikeWheel(CVector& arg0, CVector& arg1, CVector& arg2, CVector& arg3, int arg4, float arg5, float arg6, float arg7, float arg8, char arg9, float* arg10, eWheelState* wheelState, eBikeWheelSpecial arg12, eWheelStatus wheelStatus);
    void ProcessCarAlarm();
    void ProcessDelayedExplosion();
    void ProcessWheel(CVector& arg0, CVector& arg1, CVector& arg2, CVector& arg3, int arg4, float arg5, float arg6, float arg7, char arg8, float* arg9, eWheelState* arg10, unsigned short arg11);
    float ProcessWheelRotation(eWheelState wheelState, CVector const& arg1, CVector const& arg2, float arg3);
    void RemoveDriver(bool arg0);
    void RemovePassenger(CPed* passenger);
    void SetComponentAtomicAlpha(RpAtomic* atomic, int alpha);
    void SetDriver(CPed* driver);
    CPed* SetUpDriver();
    CPed* SetupPassenger(int arg0);
    bool ShufflePassengersToMakeSpace();
    void UpdateClumpAlpha();
    void UpdatePassengerList();
    bool UsesSiren();
};

VALIDATE_OFFSET(CVehicle, m_pHandlingData, 0x120);
VALIDATE_OFFSET(CVehicle, m_pFlyingHandling, 0x124);
VALIDATE_OFFSET(CVehicle, m_autoPilot, 0x128);
VALIDATE_OFFSET(CVehicle, m_pVehicleToRam, 0x19C);
VALIDATE_OFFSET(CVehicle, m_nPrimaryColor, 0x1A0);
VALIDATE_OFFSET(CVehicle, m_nSecondaryColor, 0x1A1);
VALIDATE_OFFSET(CVehicle, m_anExtras, 0x1A2);
VALIDATE_OFFSET(CVehicle, m_nAlarmState, 0x1A4);
VALIDATE_OFFSET(CVehicle, m_nRandomSeed, 0x1A6);
VALIDATE_OFFSET(CVehicle, m_pDriver, 0x1A8);
VALIDATE_OFFSET(CVehicle, m_apPassengers, 0x1AC);
VALIDATE_OFFSET(CVehicle, m_nNumPassengers, 0x1CC);
VALIDATE_OFFSET(CVehicle, m_nNumGettingIn, 0x1CD);
VALIDATE_OFFSET(CVehicle, m_nGettingInFlags, 0x1CE);
VALIDATE_OFFSET(CVehicle, m_nGettingOutFlags, 0x1CF);
VALIDATE_OFFSET(CVehicle, m_nMaxPassengers, 0x1D0);
VALIDATE_OFFSET(CVehicle, m_standingOnEntity, 0x1E4);
VALIDATE_OFFSET(CVehicle, m_pFire, 0x1E8);
VALIDATE_OFFSET(CVehicle, m_fSteerAngle, 0x1EC);
VALIDATE_OFFSET(CVehicle, m_fGasPedal, 0x1F0);
VALIDATE_OFFSET(CVehicle, m_fBreakPedal, 0x1F4);
VALIDATE_OFFSET(CVehicle, m_nCreatedBy, 0x1F8);
VALIDATE_OFFSET(CVehicle, m_nVehicleFlags, 0x1F9);
VALIDATE_OFFSET(CVehicle, m_nAmmoInClip, 0x200);
VALIDATE_OFFSET(CVehicle, m_fHealth, 0x204);
VALIDATE_OFFSET(CVehicle, m_nCurrentGear, 0x208);
VALIDATE_OFFSET(CVehicle, m_fChangeGearTime, 0x20C);
VALIDATE_OFFSET(CVehicle, m_pBombRigger, 0x210);
VALIDATE_OFFSET(CVehicle, m_nTimeTillWeNeedThisCar, 0x214);
VALIDATE_OFFSET(CVehicle, m_nGunFiringTime, 0x218);
VALIDATE_OFFSET(CVehicle, m_nTimeOfDeath, 0x21C);
VALIDATE_OFFSET(CVehicle, m_nTimeBlocked, 0x220);
VALIDATE_OFFSET(CVehicle, m_nBombTimer, 0x222);
VALIDATE_OFFSET(CVehicle, m_pBlowUpEntity, 0x224);
VALIDATE_OFFSET(CVehicle, m_fVehicleFrontGroundZ, 0x228);
VALIDATE_OFFSET(CVehicle, m_fVehicleRearGroundZ, 0x22C);
VALIDATE_OFFSET(CVehicle, m_eDoorLock, 0x230);
VALIDATE_OFFSET(CVehicle, m_nLastWeaponDamage, 0x234);
VALIDATE_OFFSET(CVehicle, pLastDamEntity, 0x238);
VALIDATE_OFFSET(CVehicle, m_nRadioStation, 0x23C);
VALIDATE_OFFSET(CVehicle, m_nRainAudioCounter, 0x23D);
VALIDATE_OFFSET(CVehicle, m_nRainSamplesCounter, 0x23E);
VALIDATE_OFFSET(CVehicle, m_nHornCounter, 0x240);
VALIDATE_OFFSET(CVehicle, m_nHornPattern, 0x244);
VALIDATE_OFFSET(CVehicle, m_bSirenOrAlarm, 0x245);
VALIDATE_OFFSET(CVehicle, m_nHornDelay, 0x246);
VALIDATE_OFFSET(CVehicle, m_comedyControlState, 0x247);
VALIDATE_OFFSET(CVehicle, m_frontCollPoly, 0x248);
VALIDATE_OFFSET(CVehicle, m_rearCollPoly, 0x270);
VALIDATE_OFFSET(CVehicle, m_fSteerInput, 0x298);
VALIDATE_OFFSET(CVehicle, m_nVehicleClass, 0x29C);
VALIDATE_SIZE(CVehicle, 0x2A0);

extern float &fBurstTyreMod; // 0.13
extern float &fBurstSpeedMax; // 0.3
extern float &fBurstBikeTyreMod; // 0.05
extern float &fBurstBikeSpeedMax; // 0.12
extern float &fTweakBikeWheelTurnForce; // 2.0
extern float &ROTOR_SEMI_THICKNESS; // 0.05
extern float &ROTOR_TURN_SPEED; // 0.2
extern float &ROTOR_DISGUARD_MULT; // 0.3
extern float &ROTOR_COL_ELASTICITY; // 1.0
extern float &ROTOR_DEFAULT_DAMAGE; // 100.0
extern float &ROTOR_COL_TURNMULT; // -0.001
