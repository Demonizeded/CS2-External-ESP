#include "readData.hpp"

namespace core {
    bool readData::readMemory(sys::Process* prcObj, sys::NtWrap* ntObj, dataMagazine* dataMag) {
        std::vector<math::Player> localPlayers;
        float localVm[4][4];
        uintptr_t entityList, entry;

        if (!prcObj->rpm(&localVm, prcObj->getModBase() + offset::dwViewMatrix, ntObj))
            return false;
        if (!prcObj->rpm(&entityList, prcObj->getModBase() + offset::dwEntityList, ntObj))
            return false;
        if (!prcObj->rpm(&entry, entityList + 0x10, ntObj))
            return false;

        for (int i = 1; i < 64; i++) {
            uintptr_t pawn, mPawn, ctrl, pe;
            if (!prcObj->rpm(&ctrl, entry + (i * 1) * 0x70, ntObj)) 
                continue;
            if (!prcObj->rpm(&mPawn, ctrl + offset::m_hPawn, ntObj)) 
                continue;
            if (!prcObj->rpm(&pe, entityList + 0x10 + 0x8 * ((mPawn & 0x7FFF) >> 9), ntObj)) continue;
            if (!prcObj->rpm(&pawn, pe + 0x70 * (mPawn & 0x1FF), ntObj)) continue;

            bool isLocal;
            if (!prcObj->rpm(&isLocal, ctrl + offset::m_bIsLocalPlayerController, ntObj)) continue;

            math::Player p;
            if (!prcObj->rpm(&p.health, pawn + offset::m_iHealth, ntObj)) continue;
            if (!prcObj->rpm(&p.teamNum, pawn + offset::m_iTeamNum, ntObj)) continue;
            if (!prcObj->rpm(&p.pos, pawn + offset::m_vOldOrigin, ntObj)) continue;

            if (isLocal) {
                m_readyData.teamNum = p.teamNum;    
                continue;
            }
            if (p.health > 100 || p.health <= 0) {
                continue;
            }

            uintptr_t gameSceneNode = 0;
            if (!prcObj->rpm(&gameSceneNode, pawn + offset::m_pGameSceneNode, ntObj)) continue;
            uintptr_t boneArray = 0;
            if (!prcObj->rpm(&boneArray, gameSceneNode + offset::m_modelState + offset::m_boneArray, ntObj)) continue;
            math::BoneData boneD[32];
            p.bonePos.resize(32);
            if (!prcObj->rpm(&boneD, boneArray, ntObj)) continue;
            for (int i = 0; i < 32; i++) {
                p.bonePos[i] = boneD[i].pos;
            }
            localPlayers.push_back(p);

        }
        m_readyData.players = localPlayers;
        memcpy(m_readyData.vm, localVm, sizeof(m_readyData.vm));
        dataMag->saveGameData(m_readyData);
        return true;
    }
}