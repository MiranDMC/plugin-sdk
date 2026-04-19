/*
    Plugin-SDK (Grand Theft Auto San Andreas) header file
    Authors: GTA Community. See more here
    https://github.com/DK22Pac/plugin-sdk
    Do not delete this comment block. Respect others' work!
*/
#pragma once
#include "PluginBase.h"
#include "CAEVehicleAudioEntity.h"
#include "CAutoPilot.h"
#include "CPhysical.h"
#include "CPtrList.h"
#include "CRideAnimData.h"
#include "CStoredCollPoly.h"
#include "CVehicleModelInfo.h"
#include "eComedyControlState.h"
#include "eDoors.h"
#include "eVehicleClass.h"
#include "eVehicleCreatedBy.h"
#include "eVehicleHandlingFlags.h"
#include "eWheelState.h"
#include "FxSystem_c.h"
#include "tBoatHandlingData.h"
#include "tFlyingHandlingData.h"
#include "tHandlingData.h"

class CColModel;
class CFire;
class CPed;
class CWeapon;

enum eBombState {
    BOMB_TIMED_NOT_ACTIVATED = 1,
    BOMB_IGNITION,
    BOMB_STICKY,
    BOMB_TIMED_ACTIVATED,
    BOMB_IGNITION_ACTIVATED
};

enum eCarWeapon : int8_t {
    CAR_WEAPON_NOT_USED,
    CAR_WEAPON_HEAVY_GUN,
    CAR_WEAPON_FREEFALL_BOMB,
    CAR_WEAPON_LOCK_ON_ROCKET,
    CAR_WEAPON_DOUBLE_ROCKET
};

enum eDoorLock : unsigned int {
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
    VEHICLE_LIGHTS_TWIN = 1,
    VEHICLE_LIGHTS_IGNORE_DAMAGE = 4,
    VEHICLE_LIGHTS_DISABLE_FRONT = 16,
    VEHICLE_LIGHTS_DISABLE_REAR = 32
};

// TODO enums
typedef int eOrdnanceType;
typedef int eFlightModel;
typedef int eBikeWheelSpecial;

class PLUGIN_API  CVehicle : public CPhysical {
protected:
    CVehicle(plugin::dummy_func_t) : CPhysical(plugin::dummy) {}
public:
    CAEVehicleAudioEntity m_vehicleAudio;
    tHandlingData*        m_pHandlingData;
    tFlyingHandlingData*  m_pFlyingHandlingData;
    
    union {
        eVehicleHandlingFlags m_nHandlingFlagsIntValue;
        struct {
            bool b1gBoost : 1;
            bool b2gBoost : 1;
            bool bNpcAntiRoll : 1;
            bool bNpcNeutralHandl : 1;
            bool bNoHandbrake : 1;
            bool bSteerRearwheels : 1;
            bool bHbRearwheelSteer : 1;
            bool bAltSteerOpt : 1;
            bool bWheelFNarrow2 : 1;
            bool bWheelFNarrow : 1;
            bool bWheelFWide : 1;
            bool bWheelFWide2 : 1;
            bool bWheelRNarrow2 : 1;
            bool bWheelRNarrow : 1;
            bool bWheelRWide : 1;
            bool bWheelRWide2 : 1;
            bool bHydraulicGeom : 1;
            bool bHydraulicInst : 1;
            bool bHydraulicNone : 1;
            bool bNosInst : 1;
            bool bOffroadAbility : 1;
            bool bOffroadAbility2 : 1;
            bool bHalogenLights : 1;
            bool bProcRearwheelFirst : 1;
            bool bUseMaxspLimit : 1;
            bool bLowRider : 1;
            bool bStreetRacer : 1;
            bool bUnused1 : 1;
            bool bSwingingChassis : 1;
        } m_nHandlingFlags;
    };
    
    CAutoPilot m_autoPilot;
    
    union {
        uint8_t m_nVehicleFlags[8];
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
            bool bTakeLessDamage : 1;       // this vehicle is stronger (takes about 1/4 of damage)

            bool bIsDamaged : 1;            // this vehicle has been damaged and is displaying all its components
            bool bHasBeenOwnedByPlayer : 1; // to work out whether stealing it is a crime
            bool bFadeOut : 1;              // fade vehicle out
            bool bIsBeingCarJacked : 1;     // fade vehicle out
            bool bCreateRoadBlockPeds : 1;  // if this vehicle gets close enough we will create peds (coppers or gang members) round it
            bool bCanBeDamaged : 1;         // set to FALSE during cut scenes to avoid explosions
            bool bOccupantsHaveBeenGenerated : 1; // is true if the occupants have already been generated (shouldn't happen again)
            bool bGunSwitchedOff : 1;       // level designers can use this to switch off guns on boats

            bool bVehicleColProcessed : 1;  // has ProcessEntityCollision been processed for this car?
            bool bIsCarParkVehicle : 1;     // car has been created using the special CAR_PARK script command
            bool bHasAlreadyBeenRecorded : 1; // used for replays
            bool bPartOfConvoy : 1;
            bool bHeliMinimumTilt : 1;      // this heli should have almost no tilt really
            bool bAudioChangingGear : 1;    // sounds like vehicle is changing gear
            bool bIsDrowning : 1;           // is vehicle occupants taking damage in water (i.e. vehicle is dead in water)
            bool bTyresDontBurst : 1;       // if this is set the tyres are invincible

            bool bCreatedAsPoliceVehicle : 1;// true if this guy was created as a police vehicle (enforcer, policecar, miamivice car etc)
            bool bRestingOnPhysical : 1;    // don't go static cause car is sitting on a physical object that might get removed
            bool bParking : 1;
            bool bCanPark : 1;
            bool bFireGun : 1;              // does the AI of this vehicle want to fire it's gun?
            bool bDriverLastFrame : 1;      // was there a driver present last frame?
            bool bNeverUseSmallerRemovalRange : 1;// some vehicles (like planes) we don't want to remove just behind the camera
            bool bIsRCVehicle : 1;          // is this a remote controlled (small) vehicle. True whether the player or AI controls it

            bool bAlwaysSkidMarks : 1;      // this vehicle leaves skidmarks regardless of the wheels' states
            bool bEngineBroken : 1;         // engine doesn't work. Player can get in but the vehicle won't drive
            bool bVehicleCanBeTargetted : 1;// the ped driving this vehicle can be targetted, (for Torenos plane mission)
            bool bPartOfAttackWave : 1;     // this car is used in an attack during a gang war
            bool bWinchCanPickMeUp : 1;     // this car cannot be picked up by any ropes
            bool bImpounded : 1;            // has this vehicle been in a police impounding garage
            bool bVehicleCanBeTargettedByHS : 1;// heat seeking missiles will not target this vehicle
            bool bSirenOrAlarm : 1;         // set to TRUE if siren or alarm active, else FALSE

            bool bHasGangLeaningOn : 1;
            bool bGangMembersForRoadBlock : 1;// will generate gang members if NumPedsForRoadBlock > 0
            bool bDoesProvideCover : 1;     // if this is false this particular vehicle can not be used to take cover behind
            bool bMadDriver : 1;            // this vehicle is driving like a lunatic
            bool bUpgradedStereo : 1;       // this vehicle has an upgraded stereo
            bool bConsideredByPlayer : 1;   // this vehicle is considered by the player to enter
            bool bPetrolTankIsWeakPoint : 1;// if false shootong the petrol tank will NOT blow up the car
            bool bDisableParticles : 1;     // disable particles from this car. Used in garage

            bool bHasBeenResprayed : 1;     // has been resprayed in a respray garage. Reset after it has been checked
            bool bUseCarCheats : 1;         // if this is true will set the car cheat stuff up in ProcessControl()
            bool bDontSetColourWhenRemapping : 1;// if the texture gets remapped we don't want to change the colour with it
            bool bUsedForReplay : 1;        // this car is controlled by replay and should be removed when replay is done
        };
    };

    uint32_t m_nCreationTime;

    uint8_t m_nPrimaryColor;
    uint8_t m_nSecondaryColor;
    uint8_t m_nTertiaryColor;
    uint8_t m_nQuaternaryColor;
    uint8_t m_anExtras[2]; // idx of random variation elements some vehicles have
    int16_t m_anUpgrades[15]; // -1 if not installed
    float   m_fWheelScale;

    int16_t m_nAlarmState; // -1 armed, 0 off, positive values are remaining time of triggered alarm
    uint16_t m_nRandomSeed; // if this is non-zero the random wander gets deterministic

    CPed*   m_pDriver;
    CPed*   m_apPassengers[8];
    uint8_t m_nNumPassengers;
    uint8_t m_nNumGettingIn;
    uint8_t m_nGettingInFlags;
    uint8_t m_nGettingOutFlags;
    uint8_t m_nMaxPassengers;
    uint8_t m_nWindowsOpenFlags; // initialised, but not used?
    uint8_t m_nNitroBoosts;

    uint8_t  m_nSpecialColModel; // custom collision model
    CEntity* m_standingOnEntity; // entity under us, only static entities (buildings or roads)

    CFire* m_pFire;

    float m_fSteerAngle;
    float m_fSteerAngleB; // used for steering 2nd set of wheels, elevators etc.
    float m_fGasPedal;
    float m_fBreakPedal;

    eVehicleCreatedBy m_nCreatedBy;

    int16_t m_nExtendedRemovalRange; // when game wants to delete a vehicle, it gets min(m_wExtendedRemovalRange, 170.0)

    uint8_t m_nBombOnBoard : 3; // 0 none, 1 timed, 2 on ignition, 3 remote, 4 timed activated, 5 on ignition activated
    uint8_t m_nOverrideLights : 2; // uses enum NO_CAR_LIGHT_OVERRIDE, FORCE_CAR_LIGHTS_OFF, FORCE_CAR_LIGHTS_ON
    uint8_t m_nWinchType : 2; // does this vehicle use a winch?

    uint8_t m_nGunsCycleIndex : 2; // cycle through alternate gun hardpoints on planes/helis
    uint8_t m_nOrdnanceCycleIndex : 2; // cycle through alternate ordnance hardpoints on planes/helis

    uint8_t m_numPedsUseItAsCover; // number of peds hiding/attempting to hid behind it
    uint8_t m_nAmmoInClip; // used to make the guns on boat do a reload (20 by default)
    uint8_t m_nPacManPickupsCollected;

    uint8_t m_nRoadblockType; // 0, 1 or 2
    uint8_t m_nNumCopsForRoadBlock;

    float m_fDirtLevel; // body dirt texture level: 0.0 clean, 15.0 dirty

    uint8_t m_nCurrentGear;
    float   m_fChangeGearTime; // used as parameter for cTransmission::CalculateDriveAcceleration, but doesn't change
    float   m_fWheelSpinForAudio;

    float m_fHealth; // 1000 = full, 250 = fire, 0 = explode

    CVehicle* m_pTractor;
    CVehicle* m_pTrailer;

    CEntity* m_pBombRigger;
    uint32_t m_nTimeTillWeNeedThisCar; // don't remove before this timestamp
    uint32_t m_nGunFiringTime; // last time when gun was fired
    uint32_t m_nTimeOfDeath; // explosion timestamp. Game will try to delete vehicle after additional 60 seconds
    uint16_t m_nTimeBlocked; // time not moving. If reaches 2500 cops chassing this leave their vehicles
    uint16_t m_nBombTimer; // goes down with each frame
    CEntity* m_pBlowUpEntity; // m_pBombRigger or explosive thrower

    float m_fVehicleFrontGroundZ; // values from CCollision::IsStoredPolyStillValidVerticalLine
    float m_fVehicleRearGroundZ;  // or CWorld::ProcessVerticalLine

private:
    int8_t field_4EC[12]; // not used?
public:
    eDoorLock m_eDoorLock;

    uint32_t m_nProjectileWeaponFiringTime; // manual-aimed projectiles for hunter, lock-on projectile for hydra
    uint32_t m_nAdditionalProjectileWeaponFiringTime; // manual-aimed projectiles for hydra
    uint32_t m_nTimeForMinigunFiring; // minigun on hunter

    int8_t   m_nLastWeaponDamage; // see eWeaponType, -1 if no damage
    CEntity* pLastDamEntity;

    uint8_t m_nRadioStation; // not used?
    uint8_t m_nRainAudioCounter; // not used?
    uint8_t m_nRainSamplesCounter; // not used?

    eCarWeapon m_nVehicleWeaponInUse;

    uint32_t m_nHornCounter; // frames count until honking stops
    uint8_t  m_nHornPattern;
    uint8_t  m_nHornDelay;

    eComedyControlState m_comedyControlState;
    int8_t m_nHasslePosId;

    CStoredCollPoly m_frontCollPoly; // poly which is under front part of car
    CStoredCollPoly m_rearCollPoly; // poly which is under rear part of car
    uint8_t         m_aCollisionLighting[4]; // left front, left rear, right front, right rear

    FxSystem_c* m_pOverheatParticle;
    FxSystem_c* m_pFireParticle;
    FxSystem_c* m_pDustParticle;

    union {
        uint8_t m_nRenderLightsFlags;
        struct {
            bool m_bRightFront : 1;
            bool m_bLeftFront : 1;
            bool m_bRightRear : 1;
            bool m_bLeftRear : 1;
        } m_renderLights;
    };

    RwTexture* m_pCustomCarPlate;
    float m_fSteerInput; // AKA m_fSteeringLeftRight or fSteer

    eVehicleType m_nVehicleClass; // m_nVehicleType - base class
    eVehicleType m_nVehicleSubClass; // m_nVehicleSubType

    int16_t    m_nPreviousRemapTxd; // loaded paintjob
    int16_t    m_nRemapTxd;
    RwTexture* m_pRemapTexture;

    static float& WHEELSPIN_TARGET_RATE; // 1.0
    static float& WHEELSPIN_INAIR_TARGET_RATE; // 10.0
    static float& WHEELSPIN_RISE_RATE; // 0.95
    static float& WHEELSPIN_FALL_RATE; // 0.7
    static float& m_fAirResistanceMult; // 2.5
    static float& ms_fRailTrackResistance; // 0.003
    static float& ms_fRailTrackResistanceDefault; // 0.003
    static bool& bDisableRemoteDetonation;
    static bool& bDisableRemoteDetonationOnContact;
    static bool& m_bEnableMouseSteering;
    static bool& m_bEnableMouseFlying;
    static int& m_nLastControlInput;
    static CColModel** m_aSpecialColVehicle; // CColModel *CVehicle::m_aSpecialColVehicle[4]
    static bool& ms_forceVehicleLightsOff;
    static bool& s_bPlaneGunsEjectShellCasings;
    static CColModel* m_aSpecialColModel; // static CColModel m_aSpecialColModel[4]

    // functions
    static void* operator new(unsigned int size);
    static void operator delete(void* data);

    // originally vtable functions

    void ProcessControlCollisionCheck();
    void ProcessControlInputs(uint8_t playerNum);

    void GetComponentWorldPosition(int componentId, CVector& posnOut); // component index in m_apModelNodes array
    bool IsComponentPresent(int componentId); // component index in m_apModelNodes array

    void OpenDoor(CPed* ped, int componentId, eDoors door, float doorOpenRatio, bool playSound);
    void ProcessOpenDoor(CPed* ped, unsigned int doorComponentId, unsigned int arg2, unsigned int arg3, float arg4);
    float GetDooorAngleOpenRatio(unsigned int door);
    float GetDooorAngleOpenRatio(eDoors door);
    bool IsDoorReady(unsigned int door);
    bool IsDoorReady(eDoors door);
    bool IsDoorFullyOpen(unsigned int door);
    bool IsDoorFullyOpen(eDoors door);
    bool IsDoorClosed(unsigned int door);
    bool IsDoorClosed(eDoors door);
    bool IsDoorMissing(unsigned int door);
    bool IsDoorMissing(eDoors door);
    bool IsOpenTopCar(); // check if car has roof as extra

    void RemoveRefsToVehicle(CEntity* entity); // remove ref to this entity

    void BlowUpCar(CEntity* damager, bool bHideExplosion);
    void BlowUpCarCutSceneNoExtras(bool bNoCamShake, bool bNoSpawnFlyingComps, bool bDetachWheels, bool bExplosionSound);

    bool SetUpWheelColModel(CColModel* wheelCol);
    bool BurstTyre(uint8_t tyreComponentId, bool bPhysicalEffect); // false if it's not possible to burst tyre. bPhysicalEffect applies random moving force

    bool IsRoomForPedToLeaveCar(unsigned int arg0, CVector* arg1);
    void ProcessDrivingAnims(CPed* driver, uint8_t arg1);
    CRideAnimData* GetRideAnimData(); // get special ride anim data for bike or quad

    void SetupSuspensionLines();
    CVector AddMovingCollisionSpeed(CVector& arg0);
    void Fix();
    void SetupDamageAfterLoad();
    void DoBurstAndSoftGroundRatios();
    float GetHeightAboveRoad();
    void PlayCarHorn();
    int GetNumContactWheels();
    void VehicleDamage(float damageIntensity, uint16_t collisionComponent, CEntity* damager, CVector* vecCollisionCoors, CVector* vecCollisionDirection, eWeaponType weapon);
    bool CanPedStepOutCar(bool arg0);
    bool CanPedJumpOutCar(CPed* ped);

    bool GetTowHitchPos(CVector& posnOut, bool arg1, CVehicle* arg2);
    bool GetTowBarPos(CVector& posnOut, bool arg1, CVehicle* arg2);
    bool SetTowLink(CVehicle* arg0, bool arg1); // always return true
    bool BreakTowLink();

    float FindWheelWidth(bool bRear);

    bool Save(); // always return true
    bool Load(); // always return true

    static void Shutdown();
    
    int GetRemapIndex(); // -1 if no remap index
    void SetRemapTexDictionary(int txdId);
    void SetRemap(int remapIndex); // index for m_awRemapTxds[] array

    void SetCollisionLighting(uint8_t lighting);
    void UpdateLightingFromStoredPolys();
    void CalculateLightingFromCollision();

    void ResetAfterRender();
    eVehicleApperance GetVehicleAppearance();
    
    bool CustomCarPlate_TextureCreate(CVehicleModelInfo* model); // returns false if vehicle model has no car plate material
    void CustomCarPlate_TextureDestroy();

    bool CanBeDeleted();

    float ProcessWheelRotation(eWheelState wheelState, CVector const& arg1, CVector const& arg2, float arg3);
    bool CanVehicleBeDamaged(CEntity* damager, eWeaponType weapon, uint8_t* arg2);
    void ProcessDelayedExplosion();

    bool AddPassenger(CPed* passenger);
    bool AddPassenger(CPed* passenger, uint8_t seatNumber);
    void RemovePassenger(CPed* passenger);
    void SetDriver(CPed* driver);
    void RemoveDriver(bool arg0);
    CPed* SetUpDriver(int pedType, bool arg1, bool arg2);
    CPed* SetupPassenger(int seatNumber, int pedType, bool arg2, bool arg3);
    bool IsPassenger(CPed* ped);
    bool IsPassenger(int modelIndex);
    bool IsDriver(CPed* ped);
    bool IsDriver(int modelIndex);
    void KillPedsInVehicle();
    
    bool IsUpsideDown(); // return this->m_pCoords->matrix.at.z <= -0.9;
    bool IsOnItsSide(); // return this->m_pCoords->matrix.right.z >= 0.8 || this->m_pCoords->matrix.right.z <= -0.8;

    bool CanPedOpenLocks(CPed* ped);
    bool CanDoorsBeDamaged();
    bool CanPedEnterCar();
    void ProcessCarAlarm();

    bool IsVehicleNormal();
    void ChangeLawEnforcerState(uint8_t state);
    bool IsLawEnforcementVehicle();

    bool ShufflePassengersToMakeSpace();
    void ExtinguishCarFire();

    void ActivateBomb();
    void ActivateBombWhenEntered();

    bool CarHasRoof();
    float HeightAboveCeiling(float arg0, eFlightModel arg1);
    void SetComponentVisibility(RwFrame* component, unsigned int visibilityState);
    void ApplyBoatWaterResistance(tBoatHandlingData* boatHandling, float arg1);
    static void SetComponentAtomicAlpha(RpAtomic* atomic, int alpha);
    void UpdateClumpAlpha();

    void UpdatePassengerList();
    CPed* PickRandomPassenger();

    void AddDamagedVehicleParticles();
    void MakeDirty(CColPoint& colPoint);
    bool AddWheelDirtAndWater(CColPoint& colPoint, unsigned int arg1, uint8_t arg2, uint8_t arg3);

    void SetGettingInFlags(uint8_t doorId);
    void SetGettingOutFlags(uint8_t doorId);
    void ClearGettingInFlags(uint8_t doorId);
    void ClearGettingOutFlags(uint8_t doorId);
    void SetWindowOpenFlag(uint8_t doorId);
    void ClearWindowOpenFlag(uint8_t doorId);

    bool SetVehicleUpgradeFlags(int upgradeModelIndex, int componentIndex, int& resultModelIndex);
    bool ClearVehicleUpgradeFlags(int arg0, int componentIndex);
    RpAtomic* CreateUpgradeAtomic(CBaseModelInfo* model, UpgradePosnDesc const* upgradePosn, RwFrame* parentComponent, bool isDamaged);
    void RemoveUpgrade(int upgradeId);
    int GetUpgrade(int upgradeId); // return upgrade model id or -1 if not present
    RpAtomic* CreateReplacementAtomic(CBaseModelInfo* model, RwFrame* component, int arg2, bool bDamaged, bool bIsWheel);
    void AddReplacementUpgrade(int modelIndex, int nodeId);
    void RemoveReplacementUpgrade(int nodeId);
    int GetReplacementUpgrade(int nodeId); // return upgrade model id or -1 if not present
    void RemoveAllUpgrades();

    int GetSpareHasslePosId();
    void SetHasslePosId(int hasslePos, bool enable);

    void InitWinch(int arg0);
    void UpdateWinch();
    void RemoveWinch();
    void ReleasePickedUpEntityWithWinch();
    void PickUpEntityWithWinch(CEntity* arg0);
    CEntity* QueryPickedUpEntityWithWinch();

    float GetRopeHeightForHeli();
    void SetRopeHeightForHeli(float height);

    void RenderDriverAndPassengers();
    void PreRenderDriverAndPassengers();

    float GetPlaneGunsAutoAimAngle();
    int GetPlaneNumGuns();
    void SetFiringRateMultiplier(float multiplier);
    float GetFiringRateMultiplier();
    unsigned int GetPlaneGunsRateOfFire();
    CVector GetPlaneGunsPosition(int gunId);
    unsigned int GetPlaneOrdnanceRateOfFire(eOrdnanceType ordnanceType);
    CVector GetPlaneOrdnancePosition(eOrdnanceType ordnanceType);
    void SelectPlaneWeapon(bool bChange, eOrdnanceType ordnanceType);
    void DoPlaneGunFireFX(CWeapon* weapon, CVector& particlePos, CVector& gunshellPos, int particleIndex);
    void FirePlaneGuns();
    void FireUnguidedMissile(eOrdnanceType ordnanceType, bool bCheckTime);

    bool CanBeDriven();
    void ReactToVehicleDamage(CPed* ped);
    bool GetVehicleLightsStatus();
    bool CanPedLeanOut(CPed* ped);
    void SetVehicleCreatedBy(int createdBy);
    void SetupRender();
    void ProcessWheel(CVector& arg0, CVector& arg1, CVector& arg2, CVector& arg3, int arg4, float arg5, float arg6, float arg7, char arg8, float* arg9, eWheelState* arg10, uint16_t arg11);
    void ProcessBikeWheel(CVector& arg0, CVector& arg1, CVector& arg2, CVector& arg3, int arg4, float arg5, float arg6, float arg7, float arg8, char arg9, float* arg10, eWheelState* arg11, eBikeWheelSpecial arg12, uint16_t arg13);
    int FindTyreNearestPoint(float x, float y); // return nearest wheel?
    void InflictDamage(CEntity* damager, eWeaponType weapon, float intensity, CVector coords);
    void KillPedsGettingInVehicle();
    bool UsesSiren();
    bool IsSphereTouchingVehicle(float x, float y, float z, float radius);
    void FlyingControl(eFlightModel flightModel, float arg1, float arg2, float arg3, float arg4);
    void BladeColSectorList(CPtrList& ptrList, CColModel& colModel, CMatrix& matrix, short arg3, float arg4); // always return false?
    void SetComponentRotation(RwFrame* component, int axis, float angle, bool bResetPosition);
    void SetTransmissionRotation(RwFrame* component, float arg1, float arg2, CVector posn, bool isFront);
    void ProcessBoatControl(tBoatHandlingData* boatHandling, float& arg1, bool arg2, bool arg3);
    void DoBoatSplashes(float arg0);
    void DoSunGlare();
    void AddWaterSplashParticles();
    void AddExhaustParticles();
    bool AddSingleWheelParticles(eWheelState arg0, unsigned int arg1, float arg2, float arg3, CColPoint* arg4, CVector* arg5, float arg6, int arg7, unsigned int surfaceType, bool* bloodState, unsigned int arg10); // always return false?
    bool GetSpecialColModel();

    void RemoveVehicleUpgrade(int upgradeModelIndex);
    void AddUpgrade(int modelIndex, int upgradeIndex);

    void UpdateTrailerLink(bool arg0, bool arg1);
    void UpdateTractorLink(bool arg0, bool arg1);

    CEntity* ScanAndMarkTargetForHeatSeekingMissile(CEntity* entity);
    void FireHeatSeakingMissile(CEntity* targetEntity, eOrdnanceType ordnanceType, bool arg2);
    void PossiblyDropFreeFallBombForPlayer(eOrdnanceType ordnanceType, bool arg1);
    void ProcessSirenAndHorn(bool arg0);

    bool DoHeadLightEffect(int dummyId, CMatrix& vehicleMatrix, uint8_t lightId, uint8_t lightState);
    void DoHeadLightBeam(int arg0, CMatrix& matrix, uint8_t arg2);
    void DoHeadLightReflectionSingle(CMatrix& matrix, uint8_t lightId);
    void DoHeadLightReflectionTwin(CMatrix& matrix);
    void DoHeadLightReflection(CMatrix& matrix, unsigned int flags, uint8_t left, uint8_t right);
    bool DoTailLightEffect(int lightId, CMatrix& matrix, uint8_t arg2, uint8_t arg3, unsigned int arg4, uint8_t arg5);
    void DoVehicleLights(CMatrix& matrix, unsigned int flags); // see eVehicleLightsFlags

    void FillVehicleWithPeds(bool bSetClothesToAfro);
    void DoBladeCollision(CVector arg0, CMatrix& matrix, short arg2, float arg3, float arg4);
    void AddVehicleUpgrade(int modelId);
    void SetupUpgradesAfterLoad();

    void GetPlaneWeaponFiringStatus(bool& status, eOrdnanceType& ordnanceType);
    void ProcessWeapons();
};

VALIDATE_OFFSET(CVehicle, m_vehicleAudio, 0x138);
VALIDATE_OFFSET(CVehicle, m_pHandlingData, 0x384);
VALIDATE_OFFSET(CVehicle, m_pFlyingHandlingData, 0x388);
VALIDATE_OFFSET(CVehicle, m_nHandlingFlags, 0x38C);
VALIDATE_OFFSET(CVehicle, m_autoPilot, 0x390);
VALIDATE_OFFSET(CVehicle, m_nVehicleFlags, 0x428);
VALIDATE_OFFSET(CVehicle, m_nCreationTime, 0x430);
VALIDATE_OFFSET(CVehicle, m_nPrimaryColor, 0x434);
VALIDATE_OFFSET(CVehicle, m_nSecondaryColor, 0x435);
VALIDATE_OFFSET(CVehicle, m_nTertiaryColor, 0x436);
VALIDATE_OFFSET(CVehicle, m_nQuaternaryColor, 0x437);
VALIDATE_OFFSET(CVehicle, m_anExtras, 0x438);
VALIDATE_OFFSET(CVehicle, m_anUpgrades, 0x43A);
VALIDATE_OFFSET(CVehicle, m_fWheelScale, 0x458);
VALIDATE_OFFSET(CVehicle, m_nAlarmState, 0x45C);
VALIDATE_OFFSET(CVehicle, m_nRandomSeed, 0x45E);
VALIDATE_OFFSET(CVehicle, m_pDriver, 0x460);
VALIDATE_OFFSET(CVehicle, m_apPassengers, 0x464);
VALIDATE_OFFSET(CVehicle, m_nNumPassengers, 0x484);
VALIDATE_OFFSET(CVehicle, m_nNumGettingIn, 0x485);
VALIDATE_OFFSET(CVehicle, m_nGettingInFlags, 0x486);
VALIDATE_OFFSET(CVehicle, m_nGettingOutFlags, 0x487);
VALIDATE_OFFSET(CVehicle, m_nMaxPassengers, 0x488);
VALIDATE_OFFSET(CVehicle, m_nWindowsOpenFlags, 0x489);
VALIDATE_OFFSET(CVehicle, m_nNitroBoosts, 0x48A);
VALIDATE_OFFSET(CVehicle, m_nSpecialColModel, 0x48B);
VALIDATE_OFFSET(CVehicle, m_standingOnEntity, 0x48C);
VALIDATE_OFFSET(CVehicle, m_pFire, 0x490);
VALIDATE_OFFSET(CVehicle, m_fSteerAngle, 0x494);
VALIDATE_OFFSET(CVehicle, m_fSteerAngleB, 0x498);
VALIDATE_OFFSET(CVehicle, m_fGasPedal, 0x49C);
VALIDATE_OFFSET(CVehicle, m_fBreakPedal, 0x4A0);
VALIDATE_OFFSET(CVehicle, m_nCreatedBy, 0x4A4);
VALIDATE_OFFSET(CVehicle, m_nExtendedRemovalRange, 0x4A6);
VALIDATE_OFFSET(CVehicle, m_numPedsUseItAsCover, 0x4AA);
VALIDATE_OFFSET(CVehicle, m_nAmmoInClip, 0x4AB);
VALIDATE_OFFSET(CVehicle, m_nPacManPickupsCollected, 0x4AC);
VALIDATE_OFFSET(CVehicle, m_nRoadblockType, 0x4AD);
VALIDATE_OFFSET(CVehicle, m_nNumCopsForRoadBlock, 0x4AE);
VALIDATE_OFFSET(CVehicle, m_fDirtLevel, 0x4B0);
VALIDATE_OFFSET(CVehicle, m_nCurrentGear, 0x4B4);
VALIDATE_OFFSET(CVehicle, m_fChangeGearTime, 0x4B8);
VALIDATE_OFFSET(CVehicle, m_fWheelSpinForAudio, 0x4BC);
VALIDATE_OFFSET(CVehicle, m_fHealth, 0x4C0);
VALIDATE_OFFSET(CVehicle, m_pTractor, 0x4C4);
VALIDATE_OFFSET(CVehicle, m_pTrailer, 0x4C8);
VALIDATE_OFFSET(CVehicle, m_pBombRigger, 0x4CC);
VALIDATE_OFFSET(CVehicle, m_nTimeTillWeNeedThisCar, 0x4D0);
VALIDATE_OFFSET(CVehicle, m_nGunFiringTime, 0x4D4);
VALIDATE_OFFSET(CVehicle, m_nTimeOfDeath, 0x4D8);
VALIDATE_OFFSET(CVehicle, m_nTimeBlocked, 0x4DC);
VALIDATE_OFFSET(CVehicle, m_nBombTimer, 0x4DE);
VALIDATE_OFFSET(CVehicle, m_pBlowUpEntity, 0x4E0);
VALIDATE_OFFSET(CVehicle, m_fVehicleFrontGroundZ, 0x4E4);
VALIDATE_OFFSET(CVehicle, m_fVehicleRearGroundZ, 0x4E8);
VALIDATE_OFFSET(CVehicle, m_eDoorLock, 0x4F8);
VALIDATE_OFFSET(CVehicle, m_nProjectileWeaponFiringTime, 0x4FC);
VALIDATE_OFFSET(CVehicle, m_nAdditionalProjectileWeaponFiringTime, 0x500);
VALIDATE_OFFSET(CVehicle, m_nTimeForMinigunFiring, 0x504);
VALIDATE_OFFSET(CVehicle, m_nLastWeaponDamage, 0x508);
VALIDATE_OFFSET(CVehicle, pLastDamEntity, 0x50C);
VALIDATE_OFFSET(CVehicle, m_nRadioStation, 0x510);
VALIDATE_OFFSET(CVehicle, m_nRainAudioCounter, 0x511);
VALIDATE_OFFSET(CVehicle, m_nRainSamplesCounter, 0x512);
VALIDATE_OFFSET(CVehicle, m_nVehicleWeaponInUse, 0x513);
VALIDATE_OFFSET(CVehicle, m_nHornCounter, 0x514);
VALIDATE_OFFSET(CVehicle, m_nHornPattern, 0x518);
VALIDATE_OFFSET(CVehicle, m_nHornDelay, 0x519);
VALIDATE_OFFSET(CVehicle, m_comedyControlState, 0x51A);
VALIDATE_OFFSET(CVehicle, m_nHasslePosId, 0x51B);
VALIDATE_OFFSET(CVehicle, m_frontCollPoly, 0x51C);
VALIDATE_OFFSET(CVehicle, m_rearCollPoly, 0x548);
VALIDATE_OFFSET(CVehicle, m_aCollisionLighting, 0x574);
VALIDATE_OFFSET(CVehicle, m_pOverheatParticle, 0x578);
VALIDATE_OFFSET(CVehicle, m_pFireParticle, 0x57C);
VALIDATE_OFFSET(CVehicle, m_pDustParticle, 0x580);
VALIDATE_OFFSET(CVehicle, m_nRenderLightsFlags, 0x584);
VALIDATE_OFFSET(CVehicle, m_pCustomCarPlate, 0x588);
VALIDATE_OFFSET(CVehicle, m_fSteerInput, 0x58C);
VALIDATE_OFFSET(CVehicle, m_nVehicleClass, 0x590);
VALIDATE_OFFSET(CVehicle, m_nVehicleSubClass, 0x594);
VALIDATE_OFFSET(CVehicle, m_nPreviousRemapTxd, 0x598);
VALIDATE_OFFSET(CVehicle, m_nRemapTxd, 0x59A);
VALIDATE_OFFSET(CVehicle, m_pRemapTexture, 0x59C);
VALIDATE_SIZE(CVehicle, 0x5A0);

bool IsVehiclePointerValid(CVehicle* vehicle);
RpAtomic* RemoveUpgradeCB(RpAtomic* atomic, void* data);
RpAtomic* FindUpgradeCB(RpAtomic* atomic, void* data);
RwObject* RemoveObjectsCB(RwObject* object, void* data);
RwFrame* RemoveObjectsCB(RwFrame* component, void* data);
RwObject* CopyObjectsCB(RwObject* object, void* data);
RwObject* FindReplacementUpgradeCB(RwObject* object, void* data);
RpAtomic* RemoveAllUpgradesCB(RpAtomic* atomic, void* data);
RpMaterial* SetCompAlphaCB(RpMaterial* material, void* data);
RwObject* SetVehicleAtomicVisibilityCB(RwObject* object, void* data);
RwFrame* SetVehicleAtomicVisibilityCB(RwFrame* component, void* data);
void DestroyVehicleAndDriverAndPassengers(CVehicle* vehicle);

extern float &fBurstTyreMod; // 0.13
extern float &fBurstSpeedMax; // 0.3
extern float &CAR_NOS_EXTRA_SKID_LOSS; // 0.9
extern float &WS_TRAC_FRAC_LIMIT; // 0.3
extern float &WS_ALREADY_SPINNING_LOSS; // 0.2
extern float &fBurstBikeTyreMod; // 0.05
extern float &fBurstBikeSpeedMax; // 0.12
extern float &fTweakBikeWheelTurnForce; // 2.0
extern float &AUTOGYRO_ROTORSPIN_MULT; // 0.006
extern float &AUTOGYRO_ROTORSPIN_MULTLIMIT; // 0.25
extern float &AUTOGYRO_ROTORSPIN_DAMP; // 0.997
extern float &AUTOGYRO_ROTORLIFT_MULT; // 4.5
extern float &AUTOGYRO_ROTORLIFT_FALLOFF; // 0.75
extern float &AUTOGYRO_ROTORTILT_ANGLE; // 0.25
extern float &ROTOR_SEMI_THICKNESS; // 0.05
extern float *fSpeedMult; // float fSpeedMult[5] = {0.8, 0.75, 0.85, 0.9, 0.85, 0.85}
extern float &fDamagePosSpeedShift; // 0.4
extern unsigned int *aDriveAnimIdsLow; // unsigned int aDriveAnimIdsLow[4] = {61, 68, 69, 78}
extern unsigned int *aDriveAnimIdsBoat; // unsigned int aDriveAnimIdsBoat[4] = {81, 82, 83, 84}
extern unsigned int *aDriveAnimIdsBad; // unsigned int aDriveAnimIdsBad[4] = {62, 70, 71, 79}
extern unsigned int *aDriveAnimIdsBadSlow; // unsigned int aDriveAnimIdsBadSlow[4] = {62, 87, 88, 79}
extern unsigned int *aDriveAnimIdsStd; // unsigned int aDriveAnimIdsStd[4] = {60, 66, 67, 78}
extern unsigned int *aDriveAnimIdsStdSlow; // unsigned int aDriveAnimIdsStdSlow[4] = {60, 85, 86, 78}
extern unsigned int *aDriveAnimIdsPro; // unsigned int aDriveAnimIdsPro[4] = {63, 72, 73, 80}
extern unsigned int *aDriveAnimIdsProSlow; // unsigned int aDriveAnimIdsProSlow[4] = {63, 89, 90, 80}
extern unsigned int *aDriveAnimIdsTruck; // unsigned int aDriveAnimIdsTruck[4] = {91, 92, 93, 94}
extern unsigned int *aDriveAnimIdsKart; // unsigned int aDriveAnimIdsKart[4] = {95, 96, 97, 98}
extern float &DIFF_LIMIT; // 0.8
extern float &DIFF_SPRING_MULT_X; // 0.05
extern float &DIFF_SPRING_MULT_Y; // 0.05
extern float &DIFF_SPRING_MULT_Z; // 0.1
extern float &DIFF_SPRING_COMPRESS_MULT; // 2.0
extern CVector *VehicleGunOffset; // CVector VehicleGunOffset[12];
extern char *&HandlingFilename;
extern char(*VehicleNames)[14]; // char VehicleNames[100][14]; sorting is based on handling id
