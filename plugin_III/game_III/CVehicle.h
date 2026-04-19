/*
    Plugin-SDK (Grand Theft Auto 3) header file
    Authors: GTA Community. See more here
    https://github.com/DK22Pac/plugin-sdk
    Do not delete this comment block. Respect others' work!
*/
#pragma once
#include "PluginBase.h"
#include "CAutoPilot.h"
#include "CPhysical.h"
#include "CStoredCollPoly.h"
#include "CVector.h"
#include "eDoors.h"
#include "eComedyControlState.h"
#include "eVehicleCreatedBy.h"
#include "eWeaponType.h"
#include "eWheelState.h"
#include "eWheelStatus.h"
#include "tHandlingData.h"

class CColModel;
class CFire;
class CPed;

enum eBombState {
    BOMB_TIMED_NOT_ACTIVATED = 1,
    BOMB_IGNITION,
    BOMB_STICKY,
    BOMB_TIMED_ACTIVATED,
    BOMB_IGNITION_ACTIVATED
};

enum eDoorLock : unsigned int {
    DOORLOCK_NOT_USED,
    DOORLOCK_UNLOCKED,
    DOORLOCK_LOCKED,
    DOORLOCK_LOCKOUT_PLAYER_ONLY,
    DOORLOCK_LOCKED_PLAYER_INSIDE,
    DOORLOCK_LOCKED_INITIALLY,
    DOORLOCK_FORCE_SHUT_DOORS
};

enum eFlightModel {
    FLIGHT_MODEL_DODO,
    FLIGHT_MODEL_RCPLANE, // not used in III
    FLIGHT_MODEL_HELI,
    FLIGHT_MODEL_SEAPLANE
};

enum eVehicleLightsFlags {
    VEHICLE_LIGHTS_TWIN = 1,
    VEHICLE_LIGHTS_IGNORE_DAMAGE = 4,
    VEHICLE_LIGHTS_DISABLE_FRONT = 16,
    VEHICLE_LIGHTS_DISABLE_REAR = 32
};

class PLUGIN_API CVehicle : public CPhysical {
public:
    tHandlingData* m_pHandlingData;
    CAutoPilot m_autoPilot;

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
    char field_1CD[19]; // unknown
public:

    CEntity* m_standingOnEntity;
    CFire* m_pCarFire;

    float m_fSteerAngle;
    float m_fGasPedal;
    float m_fBrakePedal;

    eVehicleCreatedBy m_nCreatedBy;

    union {
        uint8_t m_nVehicleFlags[4];
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
        };
    };

    uint8_t m_numPedsUseItAsCover; // number of peds hiding/attempting to hide behind it
    uint8_t m_nAmmoInClip; // used to make the guns onboard do a reload
    uint8_t m_nPacManPickupsCollected;

    uint8_t m_nRoadblockType;
    short m_nRoadblockNode;

    float m_fHealth; // 1000 = full, 250 = fire, 0 = explode

    uint8_t m_nCurrentGear;
    float   m_fChangeGearTime; // used as parameter for cTransmission::CalculateDriveAcceleration, but doesn't change

    uint32_t m_nGunFiringTime; // last time when gun on vehicle was fired
    uint32_t m_nTimeOfDeath; // explosion timestamp. Game will try to delete vehicle after additional 60 seconds
    uint16_t m_nTimeBlocked; // time not moving. If reaches 2500 cops chassing this leave their vehicles
    uint16_t m_nBombTimer; // goes down with each frame
    CEntity* m_pBlowUpEntity; // m_pBombRigger or explosive thrower

    float m_fVehicleFrontGroundZ; // values from CCollision::IsStoredPolyStillValidVerticalLine
    float m_fVehicleRearGroundZ;  // or CWorld::ProcessVerticalLine

    eDoorLock m_eDoorLock;
    int8_t m_nLastWeaponDamage; // see eWeaponType, -1 if no damage

    uint8_t m_nRadioStation;
    uint8_t m_nRainAudioCounter;
    uint8_t m_nRainSamplesCounter;

    uint8_t m_nHornCounter; // frames count until honking stops
    uint8_t m_nHornPattern;
    bool m_bSirenOrAlarm;

    eComedyControlState m_comedyControlState;

    CStoredCollPoly m_frontCollPoly; // poly which is under front part of car
    CStoredCollPoly m_rearCollPoly; // poly which is under rear part of car

    float m_fSteerInput;
    eVehicleType m_nVehicleClass;

    SUPPORTED_10EN_11EN_STEAM static bool &m_bDisableMouseSteering;
    SUPPORTED_10EN_11EN_STEAM static bool &bCheat5;
    SUPPORTED_10EN_11EN_STEAM static bool &bCheat4;
    SUPPORTED_10EN_11EN_STEAM static bool &bCheat3;
    SUPPORTED_10EN_11EN_STEAM static bool &bAllDodosCheat;
    SUPPORTED_10EN_11EN_STEAM static bool &bWheelsOnlyCheat;

    // functions
    void* operator new(size_t size);
    void operator delete(void* data);

    //PLUGIN_NO_DEFAULT_CONSTRUCTION(CVehicle) - TODO: make constructor protected

    SUPPORTED_10EN_11EN_STEAM void SetModelIndex(unsigned int modelIndex);

    SUPPORTED_10EN_11EN_STEAM bool SetupLighting();
    SUPPORTED_10EN_11EN_STEAM void RemoveLighting(bool reset);
    SUPPORTED_10EN_11EN_STEAM void FlagToDestroyWhenNextProcessed();

    SUPPORTED_10EN_11EN_STEAM void ProcessControlInputs(uint8_t padNumber);
    SUPPORTED_10EN_11EN_STEAM void GetComponentWorldPosition(int component, CVector &pos);
    SUPPORTED_10EN_11EN_STEAM bool IsComponentPresent(int component);
    SUPPORTED_10EN_11EN_STEAM void SetComponentRotation(int component, CVector rotation);
    SUPPORTED_10EN_11EN_STEAM void OpenDoor(int component, eDoors door, float angle);
    SUPPORTED_10EN_11EN_STEAM void ProcessOpenDoor(unsigned int component, unsigned int anim, float angle);
    SUPPORTED_10EN_11EN_STEAM bool IsDoorReady(eDoors door);
    SUPPORTED_10EN_11EN_STEAM bool IsDoorFullyOpen(eDoors door);
    SUPPORTED_10EN_11EN_STEAM bool IsDoorClosed(eDoors door);
    SUPPORTED_10EN_11EN_STEAM bool IsDoorMissing(eDoors door);
    SUPPORTED_10EN_11EN_STEAM void RemoveRefsToVehicle(CEntity *entity);
    SUPPORTED_10EN_11EN_STEAM void BlowUpCar(CEntity *entity);
    SUPPORTED_10EN_11EN_STEAM bool SetUpWheelColModel(CColModel *colModel);
    SUPPORTED_10EN_11EN_STEAM void BurstTyre(uint8_t wheel);
    SUPPORTED_10EN_11EN_STEAM bool IsRoomForPedToLeaveCar(unsigned int component, CVector *doorOffset);
    SUPPORTED_10EN_11EN_STEAM float GetHeightAboveRoad();
    SUPPORTED_10EN_11EN_STEAM void PlayCarHorn();

    SUPPORTED_10EN_11EN_STEAM bool AddPassenger(CPed *passenger);
    SUPPORTED_10EN_11EN_STEAM bool AddPassenger(CPed *passenger, uint8_t number);
    SUPPORTED_10EN_11EN_STEAM bool CanBeDeleted();
    SUPPORTED_10EN_11EN_STEAM bool CanPedEnterCar();
    SUPPORTED_10EN_11EN_STEAM bool CanPedExitCar();
    SUPPORTED_10EN_11EN_STEAM bool CanPedOpenLocks(CPed *ped);
    SUPPORTED_10EN_11EN_STEAM bool CarHasRoof();
    SUPPORTED_10EN_11EN_STEAM void ChangeLawEnforcerState(uint8_t enable);
    SUPPORTED_10EN_11EN_STEAM void DoFixedMachineGuns();
    SUPPORTED_10EN_11EN_STEAM void ExtinguishCarFire();
    SUPPORTED_10EN_11EN_STEAM void FlyingControl(eFlightModel flightModel);
    SUPPORTED_10EN_11EN_STEAM void InflictDamage(CEntity *entity, eWeaponType weaponType, float damage);
    SUPPORTED_10EN_11EN_STEAM bool IsLawEnforcementVehicle();
    SUPPORTED_10EN_11EN_STEAM bool IsOnItsSide();
    SUPPORTED_10EN_11EN_STEAM bool IsSphereTouchingVehicle(float x, float y, float z, float radius);
    SUPPORTED_10EN_11EN_STEAM bool IsUpsideDown();
    SUPPORTED_10EN_11EN_STEAM bool IsVehicleNormal();
    SUPPORTED_10EN_11EN_STEAM void ProcessCarAlarm();
    SUPPORTED_10EN_11EN_STEAM void ProcessDelayedExplosion();
    SUPPORTED_10EN_11EN_STEAM void ProcessWheel(CVector &wheelFwd, CVector &wheelRight, CVector &wheelContactSpeed, CVector &wheelContactPoint, int wheelsOnGround, float thrust, float brake, float adhesion, char wheelId, float* wheelSpeed, eWheelState* wheelState, eWheelStatus wheelStatus);
    SUPPORTED_10EN_11EN_STEAM float ProcessWheelRotation(eWheelState state, CVector const &fwd, CVector const &speed, float radius);
    SUPPORTED_10EN_11EN_STEAM void RemoveDriver();
    SUPPORTED_10EN_11EN_STEAM void RemovePassenger(CPed *passenger);
    SUPPORTED_10EN_11EN_STEAM void SetDriver(CPed *driver);
    SUPPORTED_10EN_11EN_STEAM CPed *SetUpDriver();
    SUPPORTED_10EN_11EN_STEAM CPed *SetupPassenger(int number);
    SUPPORTED_10EN_11EN_STEAM bool ShufflePassengersToMakeSpace();
    SUPPORTED_10EN_11EN_STEAM bool UsesSiren(unsigned int vehicleModel);
};

VALIDATE_OFFSET(CVehicle, m_pHandlingData, 0x128);
VALIDATE_OFFSET(CVehicle, m_autoPilot, 0x12C);
VALIDATE_OFFSET(CVehicle, m_nPrimaryColor, 0x19C);
VALIDATE_OFFSET(CVehicle, m_nSecondaryColor, 0x19D);
VALIDATE_OFFSET(CVehicle, m_anExtras, 0x19E);
VALIDATE_OFFSET(CVehicle, m_nAlarmState, 0x1A0);
VALIDATE_OFFSET(CVehicle, m_nRandomSeed, 0x1A2);
VALIDATE_OFFSET(CVehicle, m_pDriver, 0x1A4);
VALIDATE_OFFSET(CVehicle, m_apPassengers, 0x1A8);
VALIDATE_OFFSET(CVehicle, m_nNumPassengers, 0x1C8);
VALIDATE_OFFSET(CVehicle, m_nNumGettingIn, 0x1C9);
VALIDATE_OFFSET(CVehicle, m_nGettingInFlags, 0x1CA);
VALIDATE_OFFSET(CVehicle, m_nGettingOutFlags, 0x1CB);
VALIDATE_OFFSET(CVehicle, m_nMaxPassengers, 0x1CC);
VALIDATE_OFFSET(CVehicle, m_standingOnEntity, 0x1E0);
VALIDATE_OFFSET(CVehicle, m_pCarFire, 0x1E4);
VALIDATE_OFFSET(CVehicle, m_fSteerAngle, 0x1E8);
VALIDATE_OFFSET(CVehicle, m_fGasPedal, 0x1EC);
VALIDATE_OFFSET(CVehicle, m_fBrakePedal, 0x1F0);
VALIDATE_OFFSET(CVehicle, m_nCreatedBy, 0x1F4);
VALIDATE_OFFSET(CVehicle, m_nVehicleFlags, 0x1F5);
VALIDATE_OFFSET(CVehicle, m_numPedsUseItAsCover, 0x1F9);
VALIDATE_OFFSET(CVehicle, m_nAmmoInClip, 0x1FA);
VALIDATE_OFFSET(CVehicle, m_nPacManPickupsCollected, 0x1FB);
VALIDATE_OFFSET(CVehicle, m_nRoadblockType, 0x1FC);
VALIDATE_OFFSET(CVehicle, m_nRoadblockNode, 0x1FE);
VALIDATE_OFFSET(CVehicle, m_fHealth, 0x200);
VALIDATE_OFFSET(CVehicle, m_nCurrentGear, 0x204);
VALIDATE_OFFSET(CVehicle, m_fChangeGearTime, 0x208);
VALIDATE_OFFSET(CVehicle, m_nGunFiringTime, 0x20C);
VALIDATE_OFFSET(CVehicle, m_nTimeOfDeath, 0x210);
VALIDATE_OFFSET(CVehicle, m_nTimeBlocked, 0x214);
VALIDATE_OFFSET(CVehicle, m_nBombTimer, 0x216);
VALIDATE_OFFSET(CVehicle, m_pBlowUpEntity, 0x218);
VALIDATE_OFFSET(CVehicle, m_fVehicleFrontGroundZ, 0x21C);
VALIDATE_OFFSET(CVehicle, m_fVehicleRearGroundZ, 0x220);
VALIDATE_OFFSET(CVehicle, m_eDoorLock, 0x224);
VALIDATE_OFFSET(CVehicle, m_nLastWeaponDamage, 0x228);
VALIDATE_OFFSET(CVehicle, m_nRadioStation, 0x229);
VALIDATE_OFFSET(CVehicle, m_nRainAudioCounter, 0x22A);
VALIDATE_OFFSET(CVehicle, m_nRainSamplesCounter, 0x22B);
VALIDATE_OFFSET(CVehicle, m_nHornCounter, 0x22C);
VALIDATE_OFFSET(CVehicle, m_nHornPattern, 0x22D);
VALIDATE_OFFSET(CVehicle, m_bSirenOrAlarm, 0x22E);
VALIDATE_OFFSET(CVehicle, m_comedyControlState, 0x22F);
VALIDATE_OFFSET(CVehicle, m_frontCollPoly, 0x230);
VALIDATE_OFFSET(CVehicle, m_rearCollPoly, 0x258);
VALIDATE_OFFSET(CVehicle, m_fSteerInput, 0x280);
VALIDATE_OFFSET(CVehicle, m_nVehicleClass, 0x284);
VALIDATE_SIZE(CVehicle, 0x288);
VTABLE_DESC(CVehicle, 0x6028A8, 35);

SUPPORTED_10EN_11EN_STEAM extern float &fBurstTyreMod; // 0.1f
SUPPORTED_10EN_11EN_STEAM extern float &fBurstSpeedMax; // 0.3f
SUPPORTED_10EN_11EN_STEAM extern float &fSpeedResistanceY; // 500.0f
SUPPORTED_10EN_11EN_STEAM extern float &fSpeedResistanceZ; // 500.0f
SUPPORTED_10EN_11EN_STEAM extern float &fThrustVar; // 0.3f
SUPPORTED_10EN_11EN_STEAM extern float &fRotorFallOff; // 0.75f
SUPPORTED_10EN_11EN_STEAM extern float &fStabiliseVar; // 0.015f
SUPPORTED_10EN_11EN_STEAM extern float &fPitchVar; // 0.006f
SUPPORTED_10EN_11EN_STEAM extern float &fRollVar; // 0.006f
SUPPORTED_10EN_11EN_STEAM extern float &fYawVar; // -0.001f
SUPPORTED_10EN_11EN_STEAM extern float &fPitchBrake; // 10.0f
SUPPORTED_10EN_11EN_STEAM extern float &fRollBrake; // 10.0f
SUPPORTED_10EN_11EN_STEAM extern float &fSpinSpeedRes; // 20.0f
SUPPORTED_10EN_11EN_STEAM extern float &fRCAeroThrust; // 0.003f
SUPPORTED_10EN_11EN_STEAM extern float &fRCPropFallOff; // 3.0f
SUPPORTED_10EN_11EN_STEAM extern float &fRCYawMult; // -0.01f
SUPPORTED_10EN_11EN_STEAM extern float &fRCRudderMult; // 0.2f
SUPPORTED_10EN_11EN_STEAM extern float &fRCSideSlipMult; // 0.1f
SUPPORTED_10EN_11EN_STEAM extern float &fRCRollMult; // 0.02f
SUPPORTED_10EN_11EN_STEAM extern float &fRCRollStabilise; // -0.08f
SUPPORTED_10EN_11EN_STEAM extern float &fRCPitchMult; // 0.005f
SUPPORTED_10EN_11EN_STEAM extern float &fRCTailMult; // 0.3f
SUPPORTED_10EN_11EN_STEAM extern float &fRCFormLiftMult; // 0.02f
SUPPORTED_10EN_11EN_STEAM extern float &fRCAttackLiftMult; // 0.25f
SUPPORTED_10EN_11EN_STEAM extern float &fSeaThrust; // 0.002f
SUPPORTED_10EN_11EN_STEAM extern float &fSeaPropFallOff; // 2.3f
SUPPORTED_10EN_11EN_STEAM extern float &fSeaYawMult; // -0.0003f
SUPPORTED_10EN_11EN_STEAM extern float &fSeaRudderMult; // 0.01f
SUPPORTED_10EN_11EN_STEAM extern float &fSeaSideSlipMult; // 0.1f
SUPPORTED_10EN_11EN_STEAM extern float &fSeaRollMult; // 0.0015f
SUPPORTED_10EN_11EN_STEAM extern float &fSeaRollStabilise; // -0.01f
SUPPORTED_10EN_11EN_STEAM extern float &fSeaPitchMult; // 0.0002f
SUPPORTED_10EN_11EN_STEAM extern float &fSeaTailMult; // 0.01f
SUPPORTED_10EN_11EN_STEAM extern float &fSeaFormLiftMult; // 0.012f
SUPPORTED_10EN_11EN_STEAM extern float &fSeaAttackLiftMult; // 0.1f
SUPPORTED_10EN_11EN_STEAM extern CVector &vecHeliMoveRes;
SUPPORTED_10EN_11EN_STEAM extern CVector &vecRCAeroResistance;
SUPPORTED_10EN_11EN_STEAM extern CVector &vecRCHeliMoveRes;
SUPPORTED_10EN_11EN_STEAM extern CVector &vecHeliResistance;
SUPPORTED_10EN_11EN_STEAM extern CVector &vecRCHeliResistance;
SUPPORTED_10EN_11EN_STEAM extern CVector &vecSeaAeroResistance;

SUPPORTED_10EN_11EN_STEAM void DestroyVehicleAndDriverAndPassengers(CVehicle *vehicle);

#include "meta/meta.CVehicle.h"
