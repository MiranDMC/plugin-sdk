/*
    Plugin-SDK (Grand Theft Auto 3) header file
    Authors: GTA Community. See more here
    https://github.com/DK22Pac/plugin-sdk
    Do not delete this comment block. Respect others' work!
*/
#pragma once
#include "PluginBase.h"
#include "CColModel.h"
#include "CModelInfo.h"
#include "CPlaceable.h"
#include "CRect.h"
#include "CReference.h"
#include "CVector.h"
#include "eEntityStatus.h"
#include "eEntityType.h"
#include "RenderWare.h"

class PLUGIN_API CEntity : public CPlaceable {
public:
    union {
        RwObject* m_pRwObject;
        RpAtomic* m_pRwAtomic;
        RpClump*  m_pRwClump;
    };
    eEntityType   m_nType : 3;
    eEntityStatus m_nStatus : 5;

    bool bUsesCollision : 1; // does entity use collision
    bool bCollisionProcessed : 1; // has object been processed by a ProcessEntityCollision function
    bool bIsStatic : 1; // is entity static
    bool bHasContacted : 1; // has entity processed some contact forces
    bool bPedPhysics : 1;
    bool bIsStuck : 1; // is entity stuck
    bool bIsInSafePosition : 1; // is entity in a collision free safe position
    bool bUseCollisionRecords : 1;

    bool bWasPostponed : 1; // was entity control processing postponed
    bool bExplosionProof : 1;
    bool bIsVisible : 1; // is the entity visible
    bool bHasCollided : 1;
    bool bRenderScorched : 1;
    bool bHasBlip : 1;
    bool bIsBIGBuilding : 1; // set if this entity is a big building
    bool bRenderDamaged : 1; // use damaged LOD models for objects with applicable damage

    bool bBulletProof : 1;
    bool bFireProof : 1;
    bool bCollisionProof : 1;
    bool bMeleeProof : 1;
    bool bOnlyDamagedByPlayer : 1;
    bool bStreamingDontDelete : 1; // dont let the streaming remove this
    bool bZoneCulled : 1;
    bool bZoneCulled2 : 1; // only treadables+10m

    bool bRemoveFromWorld : 1; // remove this entity next time it should be processed
    bool bHasHitWall : 1; // has collided with a building (changes subsequent collisions)
    bool bImBeingRendered : 1; // don't delete me because I'm being rendered
    bool bTouchingWater : 1; // used by cBuoyancy::ProcessBuoyancy
    bool bIsSubway : 1; // set when subway, but maybe different meaning?
    bool bDrawLast : 1; // draw object last
    bool bNoBrightHeadLights : 1;
    bool bDoNotRender : 1;

    bool bDistanceFade : 1; // fade entity because it is far away
    bool bFlag34 : 1;

private:
    char _pad56[2];
public:
    unsigned short m_nScanCode;
    unsigned short m_nRandomSeed;
    short m_nModelIndex;
    short m_nLevel; // -1 - ignore level transitions
    CReference *m_pFirstRef;

    // functions
    PLUGIN_NO_DEFAULT_CONSTRUCTION(CEntity)
    // virtual function #0 (destructor)

    SUPPORTED_10EN_11EN_STEAM void Add();
    SUPPORTED_10EN_11EN_STEAM void Remove();
    SUPPORTED_10EN_11EN_STEAM void SetModelIndex(unsigned int modelIndex);
    SUPPORTED_10EN_11EN_STEAM void SetModelIndexNoCreate(unsigned int modelIndex);
    SUPPORTED_10EN_11EN_STEAM void CreateRwObject();
    SUPPORTED_10EN_11EN_STEAM void DeleteRwObject();
    SUPPORTED_10EN_11EN_STEAM CRect GetBoundRect();
    SUPPORTED_10EN_11EN_STEAM void ProcessControl();
    SUPPORTED_10EN_11EN_STEAM void ProcessCollision();
    SUPPORTED_10EN_11EN_STEAM void ProcessShift();
    SUPPORTED_10EN_11EN_STEAM void Teleport(CVector point);
    SUPPORTED_10EN_11EN_STEAM void PreRender();
    SUPPORTED_10EN_11EN_STEAM void Render();
    SUPPORTED_10EN_11EN_STEAM bool SetupLighting();
    SUPPORTED_10EN_11EN_STEAM void RemoveLighting(bool resetWorldColors);
    SUPPORTED_10EN_11EN_STEAM void FlagToDestroyWhenNextProcessed();

    SUPPORTED_10EN_11EN_STEAM void AddSteamsFromGround(CVector *unused);
    SUPPORTED_10EN_11EN_STEAM void AttachToRwObject(RwObject *rwObject);
    SUPPORTED_10EN_11EN_STEAM void DetachFromRwObject();
    SUPPORTED_10EN_11EN_STEAM CVector *GetBoundCentre();
    SUPPORTED_10EN_11EN_STEAM void GetBoundCentre(CVector &out);
    SUPPORTED_10EN_11EN_STEAM float GetBoundRadius();
    SUPPORTED_10EN_11EN_STEAM float GetDistanceFromCentreOfMassToBaseOfModel();
    SUPPORTED_10EN_11EN_STEAM bool GetIsOnScreen();
    SUPPORTED_10EN_11EN_STEAM bool GetIsOnScreenComplex();
    SUPPORTED_10EN_11EN_STEAM bool GetIsTouching(CVector const &posn, float radius);
    SUPPORTED_10EN_11EN_STEAM bool IsVisible();
    SUPPORTED_10EN_11EN_STEAM void ModifyMatrixForBannerInWind();
    SUPPORTED_10EN_11EN_STEAM void ModifyMatrixForTreeInWind();
    SUPPORTED_10EN_11EN_STEAM void PreRenderForGlassWindow();
    SUPPORTED_10EN_11EN_STEAM void ProcessLightsForEntity();
    SUPPORTED_10EN_11EN_STEAM void PruneReferences();
    SUPPORTED_10EN_11EN_STEAM void RegisterReference(CEntity **entity);
    SUPPORTED_10EN_11EN_STEAM void ResolveReferences();
    SUPPORTED_10EN_11EN_STEAM void SetState(unsigned char state);
    SUPPORTED_10EN_11EN_STEAM void SetupBigBuilding();
    SUPPORTED_10EN_11EN_STEAM void UpdateRwFrame();
	
	inline CVector TransformFromObjectSpace(CVector const& offset) {
        return this->m_matrix * offset;
    }

    inline CColModel* GetColModel() {
       auto mi = CModelInfo::GetModelInfo(m_nModelIndex);
       return mi ? mi->m_pColModel : nullptr;
    }
};

VTABLE_DESC(CEntity, 0x5F11A8, 17);
VALIDATE_SIZE(CEntity, 0x64);

#include "meta/meta.CEntity.h"
