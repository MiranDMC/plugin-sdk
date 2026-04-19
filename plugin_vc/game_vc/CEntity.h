/*
    Plugin-SDK (Grand Theft Auto Vice City) header file
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
#include "eEntityStatus.h"
#include "eEntityType.h"
#include "RenderWare.h"

class CEntity : public CPlaceable {
public:
    union {
        RwObject* m_pRwObject;
        RpAtomic* m_pRwAtomic;
        RpClump* m_pRwClump;
    };

    eEntityType   m_nType : 3;
    eEntityStatus m_nStatus : 5;

    bool bUsesCollision : 1;
    bool bEntUFlag02 : 1;
    bool bIsStatic : 1;
    bool bEntUFlag04 : 1;
    bool bEntUFlag05 : 1;
    bool bEntUFlag06 : 1;
    bool bEntUFlag07 : 1;
    bool bRecordCollisions : 1;

    bool bEntUFlag09 : 1;
    bool bExplosionProof : 1;
    bool bIsVisible : 1;
    bool bHasCollided : 1;
    bool bRenderScorched : 1;
    bool bHasBlip : 1;
    bool bUseLevelSectors : 1;
    bool bIsBigBuilding : 1;

    bool bEntUFlag17 : 1;
    bool bBulletProof : 1;
    bool bFireProof : 1;
    bool bCollisionProof : 1;
    bool bMeleeProof : 1;
    bool bImmuneToNonPlayerDamage : 1;
    bool bEntUFlag23 : 1;
    bool bRemoveFromWorld : 1;

    bool bEntUFlag25 : 1;
    bool bImBeingRendered : 1;
    bool bIsTouchingWater : 1;
    bool bEntUFlag28 : 1;
    bool bEntUFlag29 : 1;
    bool bEntUFlag30 : 1;
    bool bEntUFlag31 : 1;
    bool bEntUFlag32 : 1;

    bool bEntUFlag33 : 1;
    bool bDontCastShadowsOn : 1;
    bool bEntUFlag35 : 1;
    bool bIsStaticWaitingForCollision : 1;
    bool bEntUFlag37 : 1;
    bool bEntUFlag38 : 1;
    bool bEntUFlag39 : 1;
    bool bEntUFlag40 : 1;

private:
    char _pad56[2];
public:
    short m_nScanCode;
    short m_nRandomSeed;
    short m_nModelIndex;
    char m_nLevel;
    unsigned char m_nAreaCode;
    class CReference *m_pFirstRef;

protected:
    virtual ~CEntity() {};
    CEntity(plugin::dummy_func_t) {}

public:
    //vtable

    void Add();
    void Remove();
    void SetModelIndex(unsigned int modelIndex);
    void SetModelIndexNoCreate(unsigned int modelIndex);
    void CreateRwObject();
    void DeleteRwObject();
    CRect GetBoundRect();
    void ProcessControl();
    void ProcessCollision();
    void ProcessShift();
    void Teleport(CVector posn);
    void PreRender();
    void Render();
    bool SetupLighting();
    void RemoveLighting(bool resetWorldColors);
    void FlagToDestroyWhenNextProcessed();

    void SetRwObjectAlpha(int alpha);
    void ModifyMatrixForTreeInWind();
    void SetupBigBuilding();
    float GetDistanceFromCentreOfMassToBaseOfModel();
    bool GetIsOnScreenComplex();
    bool GetIsOnScreen();
    bool IsVisible();
    bool GetIsTouching(CVector const& posn, float radius);
    bool HasPreRenderEffects();
    void UpdateRpHAnim();
    void UpdateRwFrame();
    void GetBoundCentre(CVector& out);
    CVector GetBoundCentre();
    void DetachFromRwObject();
    void AttachToRwObject(RwObject* rwObject);
    void PruneReferences();
    void ResolveReferences();
    void CleanUpOldReference(CEntity** pEntity);
    void RegisterReference(CEntity** pEntity);
    void ProcessLightsForEntity();
    bool IsEntityOccluded();

public:
    inline CVector &GetPosition() {
        return pos;
    }

    inline float GetHeading() { return GetForward().Heading(); }

    inline void SetPosition(float x, float y, float z) {
        this->pos.x = x;
        this->pos.y = y;
        this->pos.z = z;
    }

    inline void SetPosition(CVector &pos) {
        this->pos = pos;
    }

    inline CVector TransformFromObjectSpace(CVector const& offset) {
        return *this * offset;
    }

    inline CColModel* GetColModel() {
        auto mi = CModelInfo::GetModelInfo(m_nModelIndex);
        return mi ? mi->m_pColModel : nullptr;
    }

    inline float GetBoundRadius() {
        auto col = GetColModel();
        return col ? col->m_boundSphere.m_fRadius : 0.0f;
    }

    CEntity() = delete;
    CEntity(const CEntity &) = delete;
    CEntity &operator=(const CEntity &) = delete;
};

VALIDATE_SIZE(CEntity, 0x64);

RpAtomic *AtomicRemoveAnimFromSkinCB(RpAtomic* atomic, void* callbackData);