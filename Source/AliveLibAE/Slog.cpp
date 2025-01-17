#include "stdafx.h"
#include "Slog.hpp"
#include "Function.hpp"
#include "stdlib.hpp"
#include "MusicController.hpp"
#include "Collisions.hpp"
#include "Events.hpp"
#include "Abe.hpp"
#include "DDCheat.hpp"
#include "Shadow.hpp"
#include "Map.hpp"
#include "Sound/Midi.hpp"
#include "ObjectIds.hpp"
#include "PlatformBase.hpp"
#include "Bone.hpp"
#include "Sfx.hpp"
#include "Blood.hpp"
#include "SnoozeParticle.hpp"
#include "Gibs.hpp"
#include "Bullet.hpp"
#include "Particle.hpp"
#include "GameSpeak.hpp"
#include "SwitchStates.hpp"
#include "PsxDisplay.hpp"
#include "Mudokon.hpp"
#include "Grid.hpp"

ALIVE_VAR(1, 0xBAF7F2, s16, sSlogCount_BAF7F2, 0);

const TSlogMotionFn sSlog_motion_table_560978[24] = {
    &Slog::M_Idle_0_4C5F90,
    &Slog::M_Walk_1_4C60C0,
    &Slog::M_Run_2_4C6340,
    &Slog::M_TurnAround_3_4C65C0,
    &Slog::M_Fall_4_4C6930,
    &Slog::M_MoveHeadUpwards_5_4C5F20,
    &Slog::M_StopRunning_6_4C66C0,
    &Slog::M_SlideTurn_7_4C6790,
    &Slog::M_StartWalking_8_4C62E0,
    &Slog::M_EndWalking_9_4C6310,
    &Slog::M_Land_10_4C7820,
    &Slog::M_Unused_11_4C7860,
    &Slog::M_StartFastBarking_12_4C7880,
    &Slog::M_EndFastBarking_13_4C78D0,
    &Slog::M_AngryBark_14_4C6CF0,
    &Slog::M_Sleeping_15_4C6D60,
    &Slog::M_MoveHeadDownwards_16_4C70D0,
    &Slog::M_WakeUp_17_4C7000,
    &Slog::M_JumpForwards_18_4C7210,
    &Slog::M_JumpUpwards_19_4C7470,
    &Slog::M_Eating_20_4C75F0,
    &Slog::M_Dying_21_4C77F0,
    &Slog::M_Scratch_22_4C7120,
    &Slog::M_Growl_23_4C7170};

enum eSlogMotions
{
    M_Idle_0_4C5F90,
    M_Walk_1_4C60C0,
    M_Run_2_4C6340,
    M_TurnAround_3_4C65C0,
    M_Fall_4_4C6930,
    M_MoveHeadUpwards_5_4C5F20,
    M_StopRunning_6_4C66C0,
    M_SlideTurn_7_4C6790,
    M_StartWalking_8_4C62E0,
    M_EndWalking_9_4C6310,
    M_Land_10_4C7820,
    M_Unused_11_4C7860,
    M_StartFastBarking_12_4C7880,
    M_EndFastBarking_13_4C78D0,
    M_AngryBark_14_4C6CF0,
    M_Sleeping_15_4C6D60,
    M_MoveHeadDownwards_16_4C70D0,
    M_WakeUp_17_4C7000,
    M_JumpForwards_18_4C7210,
    M_JumpUpwards_19_4C7470,
    M_Eating_20_4C75F0,
    M_Dying_21_4C77F0,
    M_Scratch_22_4C7120,
    M_Growl_23_4C7170
};

const TSlogBrainFn sSlog_fns_brain_560A38[4] = {
    &Slog::Brain_ListeningToSlig_0_4C3790,
    &Slog::Brain_Idle_1_4C2830,
    &Slog::Brain_ChasingAbe_2_4C0A00,
    &Slog::Brain_Death_3_4C3250};

const SfxDefinition sSlogSFXList_560B00[19] = {
    {0u, 12u, 38u, 30u, 0, 0},
    {0u, 12u, 39u, 30u, 0, 0},
    {0u, 12u, 40u, 100u, -256, 0},
    {0u, 12u, 41u, 60u, 0, 0},
    {0u, 12u, 42u, 50u, 0, 0},
    {0u, 12u, 40u, 100u, -256, 0},
    {0u, 12u, 44u, 90u, 0, 0},
    {0u, 12u, 60u, 100u, 0, 0},
    {0u, 12u, 45u, 100u, -256, 0},
    {0u, 12u, 46u, 127u, -127, 127},
    {0u, 12u, 47u, 70u, 0, 0},
    {0u, 12u, 48u, 70u, 0, 0},
    {0u, 12u, 49u, 30u, 0, 0},
    {0u, 12u, 40u, 120u, 300, 400},
    {0u, 12u, 61u, 45u, 0, 0},
    {0u, 12u, 62u, 45u, 0, 0},
    {0u, 3u, 59u, 67u, 0, 0},
    {0u, 12u, 33u, 45u, 0, 127},
    {0u, 12u, 33u, 40u, -127, 0},
};

const static BrainFunctionData<TSlogBrainFn> sSlogAiTable[4] = {
    {&Slog::Brain_ListeningToSlig_0_4C3790, 0x4C3790, "Brain_ListeningToSlig_0"}, // no stubs for any of these ??
    {&Slog::Brain_Idle_1_4C2830, 0x4C2830, "Brain_Idle_1"},
    {&Slog::Brain_ChasingAbe_2_4C0A00, 0x4C0A00, "Brain_ChasingAbe_2"},
    {&Slog::Brain_Death_3_4C3250, 0x4C3250, "Brain_Death_3"},
};

Slog* Slog::ctor_4C4540(FP xpos, FP ypos, FP scale, s16 bListenToSligs, s16 chaseDelay)
{
    ctor_408240(5);
    SetVTable(this, 0x547578);

    field_134_last_event_index = -1;

    field_BC_ypos = ypos;
    field_B8_xpos = xpos;

    field_CC_sprite_scale = scale;

    Init_4C46A0();

    field_160_flags.Clear(Flags_160::eBit5_CommandedToAttack);
    field_12C_tlvInfo = 0xFFFF;
    field_120_brain_state_idx = 2;
    field_122_brain_state_result = 0;

    BaseAliveGameObject* pTarget = FindTarget_4C33C0(0, 0);
    if (!pTarget)
    {
        pTarget = sControlledCharacter_5C1B8C;
    }
    field_118_target_id = pTarget->field_8_object_id;

    field_160_flags.Clear(Flags_160::eBit2_ListenToSligs);
    field_160_flags.Clear(Flags_160::eBit7_Asleep);
    field_160_flags.Clear(Flags_160::eBit9_MovedOffScreen);

    field_160_flags.Set(Flags_160::eBit2_ListenToSligs, bListenToSligs & 1);

    field_144_wake_up_anger = 0;
    field_158_chase_delay = chaseDelay;
    field_154_anger_switch_id = 0;
    field_106_current_motion = eSlogMotions::M_Idle_0_4C5F90;
    field_146_total_anger = 10;
    field_148_chase_anger = 20;
    field_156_bone_eating_time = 60;

    return this;
}

Slog* Slog::ctor_4C42E0(Path_Slog* pTlv, s32 tlvInfo)
{
    ctor_408240(5);
    field_134_last_event_index = -1;
    SetVTable(this, 0x547578);

    field_B8_xpos = FP_FromInteger(pTlv->field_8_top_left.field_0_x);
    field_BC_ypos = FP_FromInteger(pTlv->field_8_top_left.field_2_y);

    if (pTlv->field_10_scale != Scale_short::eFull_0)
    {
        field_CC_sprite_scale = FP_FromDouble(0.5);
    }
    else
    {
        field_CC_sprite_scale = FP_FromDouble(1);
    }

    field_C_objectId = tlvInfo;

    Init_4C46A0();

    field_160_flags.Clear(Flags_160::eBit9_MovedOffScreen);
    field_160_flags.Set(Flags_160::eBit2_ListenToSligs);
    field_160_flags.Set(Flags_160::eBit7_Asleep, pTlv->field_14_asleep == Choice_short::eYes_1);
    field_160_flags.Clear(Flags_160::eBit5_CommandedToAttack);

    field_6_flags.Set(BaseGameObject::eCanExplode_Bit7);

    field_20_animation.field_4_flags.Set(AnimFlags::eBit5_FlipX, pTlv->field_12_direction == XDirection_short::eLeft_0);

    field_12C_tlvInfo = tlvInfo;
    field_C_objectId = tlvInfo;
    field_120_brain_state_idx = 1;
    field_118_target_id = -1;
    field_144_wake_up_anger = pTlv->field_16_wake_up_anger;
    field_146_total_anger = pTlv->field_16_wake_up_anger + pTlv->field_18_bark_anger;
    field_148_chase_anger = field_146_total_anger + pTlv->field_1A_chase_anger;
    field_158_chase_delay = pTlv->field_1C_chase_delay;
    field_154_anger_switch_id = pTlv->field_20_anger_switch_id;
    field_156_bone_eating_time = pTlv->field_22_bone_eating_time;

    if (field_160_flags.Get(Flags_160::eBit7_Asleep))
    {
        field_106_current_motion = eSlogMotions::M_Sleeping_15_4C6D60;
        SetAnimFrame_4C42A0();
    }
    else
    {
        field_106_current_motion = eSlogMotions::M_Idle_0_4C5F90;
    }

    VUpdate();

    return this;
}

BaseGameObject* Slog::VDestructor(s32 flags)
{
    return vdtor_4C4510(flags);
}

void Slog::VUpdate()
{
    vUpdate_4C50D0();
}

void Slog::VOnTrapDoorOpen()
{
    vOn_TrapDoor_Open_4C5D50();
}

void Slog::VOn_TLV_Collision_4087F0(Path_TLV* pTlv)
{
    vOn_Tlv_Collision_4C5060(pTlv);
}

s16 Slog::VTakeDamage_408730(BaseGameObject* pFrom)
{
    return vTakeDamage_4C4B80(pFrom);
}

void Slog::VOnThrowableHit(BaseGameObject* pFrom)
{
    vOnThrowableHit_4C4B50(pFrom);
}

s32 Slog::VGetSaveState(u8* pSaveBuffer)
{
    return vGetSaveState_4C78F0(reinterpret_cast<Slog_State*>(pSaveBuffer));
}


const AnimId sSlogFrameOffsetTable_5609D8[24] = {
    AnimId::Slog_Idle,
    AnimId::Slog_Walk,
    AnimId::Slog_Run,
    AnimId::Slog_TurnAround,
    AnimId::Slog_Fall,
    AnimId::Slog_MoveHeadUpwards,
    AnimId::Slog_StopRunning,
    AnimId::Slog_SlideTurn,
    AnimId::Slog_StartWalking,
    AnimId::Slog_EndWalking,
    AnimId::Slog_Land,
    AnimId::Slog_Unused,
    AnimId::Slog_StartFastBarking,
    AnimId::Slog_EndFastBarking,
    AnimId::Slog_AngryBark,
    AnimId::Slog_Sleeping,
    AnimId::Slog_MoveHeadDownwards,
    AnimId::Slog_WakeUp,
    AnimId::Slog_JumpForwards,
    AnimId::Slog_JumpUpwards,
    AnimId::Slog_Eating,
    AnimId::Slog_Dying,
    AnimId::Slog_Scratch,
    AnimId::Slog_Growl};

ALIVE_VAR(1, 0xBAF7F0, u8, sSlogRandomIdx_BAF7F0, 0);

s32 CC Slog::CreateFromSaveState_4C54F0(const u8* pBuffer)
{
    auto pState = reinterpret_cast<const Slog_State*>(pBuffer);
    auto pTlv = static_cast<Path_Slog*>(sPath_dword_BB47C0->TLV_From_Offset_Lvl_Cam_4DB770(pState->field_40_tlvInfo));
    if (!ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, AEResourceID::kDogbasicResID, FALSE, FALSE))
    {
        ResourceManager::LoadResourceFile_49C170("SLOG.BND", nullptr);
    }

    if (!ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, AEResourceID::kDogknfdResID, FALSE, FALSE))
    {
        ResourceManager::LoadResourceFile_49C170("DOGKNFD.BAN", nullptr);
    }

    auto pSlog = ae_new<Slog>();

    if (pState->field_40_tlvInfo == 0xFFFF)
    {
        pSlog->ctor_4C4540(
            pState->field_8_xpos,
            pState->field_C_ypos,
            pState->field_1C_sprite_scale, pState->field_74_flags.Get(Slog_State::eBit10_ListenToSligs), pState->field_70_jump_delay);

        pSlog->field_C_objectId = pState->field_4_objectId;
    }
    else
    {
        pSlog->ctor_4C42E0(pTlv, pState->field_40_tlvInfo);
    }

    pSlog->field_FC_pPathTLV = nullptr;
    pSlog->field_100_pCollisionLine = nullptr;
    pSlog->field_110_id = pState->field_3C_id;
    pSlog->field_B8_xpos = pState->field_8_xpos;
    pSlog->field_BC_ypos = pState->field_C_ypos;
    pSlog->field_C4_velx = pState->field_10_velx;
    pSlog->field_C8_vely = pState->field_14_vely;
    pSlog->field_128_falling_velx_scale_factor = pState->field_50_falling_velx_scale_factor;
    pSlog->field_C0_path_number = pState->field_18_path_number;
    pSlog->field_C2_lvl_number = pState->field_1A_lvl_number;
    pSlog->field_CC_sprite_scale = pState->field_1C_sprite_scale;
    pSlog->field_D0_r = pState->field_20_r;
    pSlog->field_D2_g = pState->field_22_g;
    pSlog->field_D4_b = pState->field_24_b;

    pSlog->field_106_current_motion = pState->field_28_current_motion;
    u8** ppRes = pSlog->ResBlockForMotion_4C4A80(pState->field_28_current_motion);
    const AnimRecord& animRec = AnimRec(sSlogFrameOffsetTable_5609D8[pSlog->field_106_current_motion]);
    pSlog->field_20_animation.Set_Animation_Data_409C80(animRec.mFrameTableOffset, ppRes);

    pSlog->field_20_animation.field_92_current_frame = pState->field_2A_anim_cur_frame;
    pSlog->field_20_animation.field_E_frame_change_counter = pState->field_2C_frame_change_counter;

    pSlog->field_20_animation.field_4_flags.Set(AnimFlags::eBit5_FlipX, pState->field_26_bAnimFlipX & 1);
    pSlog->field_20_animation.field_4_flags.Set(AnimFlags::eBit3_Render, pState->field_2E_bRender & 1);

    pSlog->field_6_flags.Set(BaseGameObject::eDrawable_Bit4, pState->field_2F_bDrawable & 1);

    if (IsLastFrame(&pSlog->field_20_animation))
    {
        pSlog->field_20_animation.field_4_flags.Set(AnimFlags::eBit18_IsLastFrame);
    }

    pSlog->field_10C_health = pState->field_30_health;
    pSlog->field_106_current_motion = pState->field_34_current_motion;
    pSlog->field_108_next_motion = pState->field_36_next_motion;
    pSlog->field_F8_LastLineYPos = FP_FromInteger(pState->field_38_last_line_ypos);
    pSlog->field_114_flags.Set(Flags_114::e114_Bit9_RestoredFromQuickSave);
    pSlog->field_104_collision_line_type = pState->field_3A_line_type;
    pSlog->field_12C_tlvInfo = pState->field_40_tlvInfo;
    pSlog->field_118_target_id = pState->field_44_obj_id;
    pSlog->field_120_brain_state_idx = pState->field_48_state_idx;
    pSlog->field_122_brain_state_result = pState->field_4A_brain_state_result;
    pSlog->field_124_timer = pState->field_4C_timer;
    pSlog->field_128_falling_velx_scale_factor = pState->field_50_falling_velx_scale_factor;
    pSlog->field_12C_tlvInfo = pState->field_40_tlvInfo;
    pSlog->field_138_listening_to_slig_id = pState->field_54_obj_id;
    pSlog->field_132_has_woofed = pState->field_58_has_woofed;
    pSlog->field_13C_waiting_counter = pState->field_5A_waiting_counter;
    pSlog->field_13E_response_index = pState->field_5C_response_index;
    pSlog->field_140_response_part = pState->field_5E_response_part;
    pSlog->field_142_anger_level = pState->field_60_anger_level;
    pSlog->field_15A_jump_counter = pState->field_62_jump_counter;
    pSlog->field_14C_scratch_timer = pState->field_64_scratch_timer;
    pSlog->field_150_growl_timer = pState->field_68_growl_timer;
    pSlog->field_158_chase_delay = pState->field_70_jump_delay;
    pSlog->field_15C_bone_id = pState->field_6C_bone_id;
    sSlogRandomIdx_BAF7F0 = pState->field_72_slog_random_index;


    pSlog->field_11C_biting_target = pState->field_74_flags.Get(Slog_State::eBit1_BitingTarget);
    // bit2 never read
    pSlog->field_160_flags.Set(Flags_160::eBit8_Asleep, pState->field_74_flags.Get(Slog_State::eBit3_Asleep));
    pSlog->field_160_flags.Set(Flags_160::eBit9_MovedOffScreen, pState->field_74_flags.Get(Slog_State::eBit4_MovedOffScreen));
    pSlog->field_160_flags.Set(Flags_160::eBit1_StopRunning, pState->field_74_flags.Get(Slog_State::eBit5_StopRunning));
    pSlog->field_160_flags.Set(Flags_160::eBit3_Shot, pState->field_74_flags.Get(Slog_State::eBit6_Shot));
    pSlog->field_160_flags.Set(Flags_160::eBit4_Hungry, pState->field_74_flags.Get(Slog_State::eBit7_Hungry));
    pSlog->field_160_flags.Set(Flags_160::eBit5_CommandedToAttack, pState->field_74_flags.Get(Slog_State::eBit8_CommandedToAttack));
    pSlog->field_160_flags.Set(Flags_160::eBit6_HitByAbilityRing, pState->field_74_flags.Get(Slog_State::eBit9_HitByAbilityRing));
    pSlog->field_160_flags.Set(Flags_160::eBit2_ListenToSligs, pState->field_74_flags.Get(Slog_State::eBit10_ListenToSligs));

    if (pSlog->field_160_flags.Get(Flags_160::eBit3_Shot))
    {
        sSlogCount_BAF7F2--;
    }

    return sizeof(Slog_State);
}

s32 Slog::vGetSaveState_4C78F0(Slog_State* pState)
{
    if (field_114_flags.Get(Flags_114::e114_Bit7_Electrocuted))
    {
        return 0;
    }

    pState->field_0_type = AETypes::eSlog_126;

    pState->field_4_objectId = field_C_objectId;

    pState->field_8_xpos = field_B8_xpos;
    pState->field_C_ypos = field_BC_ypos;
    pState->field_10_velx = field_C4_velx;
    pState->field_14_vely = field_C8_vely;

    pState->field_50_falling_velx_scale_factor = field_128_falling_velx_scale_factor;

    pState->field_18_path_number = field_C0_path_number;
    pState->field_1A_lvl_number = field_C2_lvl_number;
    pState->field_1C_sprite_scale = field_CC_sprite_scale;

    pState->field_20_r = field_D0_r;
    pState->field_22_g = field_D2_g;
    pState->field_24_b = field_D4_b;

    pState->field_26_bAnimFlipX = field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX);
    pState->field_28_current_motion = field_106_current_motion;
    pState->field_2A_anim_cur_frame = field_20_animation.field_92_current_frame;
    pState->field_2C_frame_change_counter = field_20_animation.field_E_frame_change_counter;
    pState->field_2F_bDrawable = field_6_flags.Get(BaseGameObject::eDrawable_Bit4);
    pState->field_2E_bRender = field_20_animation.field_4_flags.Get(AnimFlags::eBit3_Render);
    pState->field_30_health = field_10C_health;
    pState->field_34_current_motion = field_106_current_motion;
    pState->field_36_next_motion = field_108_next_motion;
    pState->field_38_last_line_ypos = FP_GetExponent(field_F8_LastLineYPos);

    if (field_100_pCollisionLine)
    {
        pState->field_3A_line_type = field_100_pCollisionLine->field_8_type;
    }
    else
    {
        pState->field_3A_line_type = -1;
    }

    pState->field_3C_id = field_110_id;
    pState->field_74_flags.Set(Slog_State::eBit2_Possessed, sControlledCharacter_5C1B8C == this); // Lol can't be possessed anyway so ??
    pState->field_40_tlvInfo = field_12C_tlvInfo;
    pState->field_40_tlvInfo = field_12C_tlvInfo;
    pState->field_44_obj_id = -1;

    if (field_118_target_id != -1)
    {
        BaseGameObject* pObj = sObjectIds_5C1B70.Find_449CF0(field_118_target_id);
        if (pObj)
        {
            pState->field_44_obj_id = pObj->field_C_objectId;
        }
    }

    pState->field_48_state_idx = field_120_brain_state_idx;
    pState->field_4A_brain_state_result = field_122_brain_state_result;
    pState->field_4C_timer = field_124_timer;
    pState->field_50_falling_velx_scale_factor = field_128_falling_velx_scale_factor;
    pState->field_40_tlvInfo = field_12C_tlvInfo;
    pState->field_54_obj_id = -1;

    if (field_138_listening_to_slig_id != -1)
    {
        BaseGameObject* pObj = sObjectIds_5C1B70.Find_449CF0(field_138_listening_to_slig_id);
        if (pObj)
        {
            pState->field_54_obj_id = pObj->field_C_objectId;
        }
    }

    pState->field_58_has_woofed = field_132_has_woofed;
    pState->field_5A_waiting_counter = field_13C_waiting_counter;
    pState->field_5C_response_index = field_13E_response_index;
    pState->field_5E_response_part = field_140_response_part;
    pState->field_60_anger_level = field_142_anger_level;
    pState->field_62_jump_counter = field_15A_jump_counter;
    pState->field_64_scratch_timer = field_14C_scratch_timer;
    pState->field_68_growl_timer = field_150_growl_timer;
    pState->field_6C_bone_id = -1;

    if (field_15C_bone_id != -1)
    {
        BaseGameObject* pObj = sObjectIds_5C1B70.Find_449CF0(field_15C_bone_id);
        if (pObj)
        {
            pState->field_6C_bone_id = pObj->field_C_objectId;
        }
    }

    pState->field_70_jump_delay = field_158_chase_delay;
    pState->field_72_slog_random_index = sSlogRandomIdx_BAF7F0;

    pState->field_74_flags.Set(Slog_State::eBit1_BitingTarget, field_11C_biting_target & 1);
    pState->field_74_flags.Set(Slog_State::eBit2_Possessed, sControlledCharacter_5C1B8C == this); // Lol can't be possessed anyway so ??
    pState->field_74_flags.Set(Slog_State::eBit3_Asleep, field_160_flags.Get(Flags_160::eBit7_Asleep));
    pState->field_74_flags.Set(Slog_State::eBit4_MovedOffScreen, field_160_flags.Get(Flags_160::eBit8_Asleep));
    pState->field_74_flags.Set(Slog_State::eBit5_StopRunning, field_160_flags.Get(Flags_160::eBit1_StopRunning));
    pState->field_74_flags.Set(Slog_State::eBit6_Shot, field_160_flags.Get(Flags_160::eBit3_Shot));
    pState->field_74_flags.Set(Slog_State::eBit7_Hungry, field_160_flags.Get(Flags_160::eBit4_Hungry));
    pState->field_74_flags.Set(Slog_State::eBit8_CommandedToAttack, field_160_flags.Get(Flags_160::eBit5_CommandedToAttack));
    pState->field_74_flags.Set(Slog_State::eBit9_HitByAbilityRing, field_160_flags.Get(Flags_160::eBit6_HitByAbilityRing));
    pState->field_74_flags.Set(Slog_State::eBit10_ListenToSligs, field_160_flags.Get(Flags_160::eBit2_ListenToSligs));

    return sizeof(Slog_State);
}

void Slog::M_Idle_0_4C5F90()
{
    if (!ToNextMotion_4C5A30())
    {
        if (field_108_next_motion == eSlogMotions::M_JumpForwards_18_4C7210)
        {
            ToJump_4C5C60();
        }
        else if (field_108_next_motion == -1)
        {
            if (!field_106_current_motion)
            {
                if (gMap_5C3030.Is_Point_In_Current_Camera_4810D0(field_C2_lvl_number, field_C0_path_number, field_B8_xpos, field_BC_ypos, 0))
                {
                    SND_SEQ_PlaySeq_4CA960(SeqId::Empty_13, 1, 0);
                }

                if (gMap_5C3030.GetDirection_4811A0(field_C2_lvl_number, field_C0_path_number, field_B8_xpos, field_BC_ypos) >= CameraPos::eCamCurrent_0)
                {
                    if (MusicController::GetMusicType_47FDA0(0, 0, 0) == MusicController::MusicTypes::eSlogChaseTension_6)
                    {
                        MusicController::PlayMusic_47FD60(MusicController::MusicTypes::eSlogChaseTension_6, this, 0, 0);
                    }
                    else
                    {
                        MusicController::PlayMusic_47FD60(MusicController::MusicTypes::eSlogTension_5, this, 0, 0);
                    }
                }
            }
        }
        else
        {
            field_106_current_motion = field_108_next_motion;
            field_108_next_motion = -1;
        }
    }
}

const FP sSlogWalkVelXTable_5475EC[18] = {
    FP_FromDouble(1.3329315185546875),
    FP_FromDouble(2.4870452880859375),
    FP_FromDouble(2.537445068359375),
    FP_FromDouble(1.974761962890625),
    FP_FromDouble(3.015594482421875),
    FP_FromDouble(3.10400390625),
    FP_FromDouble(2.758575439453125),
    FP_FromDouble(3.1327056884765625),
    FP_FromDouble(3.646148681640625),
    FP_FromDouble(2.3465118408203125),
    FP_FromDouble(2.222015380859375),
    FP_FromDouble(2.1777801513671875),
    FP_FromDouble(2.2137908935546875),
    FP_FromDouble(2.0020904541015625),
    FP_FromDouble(2.413543701171875),
    FP_FromDouble(2.185516357421875),
    FP_FromDouble(4.1008453369140625),
    FP_FromDouble(4.35284423828125)};

void Slog::M_Walk_1_4C60C0()
{
    FP velX = {};
    if (field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX))
    {
        velX = -sSlogWalkVelXTable_5475EC[field_20_animation.field_92_current_frame];
    }
    else
    {
        velX = sSlogWalkVelXTable_5475EC[field_20_animation.field_92_current_frame];
    }

    field_C4_velx = (field_CC_sprite_scale * velX);

    if (CollisionCheck_4C5480(field_CC_sprite_scale * FP_FromInteger(20), field_C4_velx * FP_FromInteger(4)))
    {
        ToIdle_4C5C10();

        if (field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX))
        {
            field_B8_xpos += field_C4_velx + (ScaleToGridSize_4498B0(field_CC_sprite_scale) / FP_FromInteger(2));
        }
        else
        {
            field_B8_xpos -= (ScaleToGridSize_4498B0(field_CC_sprite_scale) / FP_FromInteger(2)) - field_C4_velx;
        }
    }
    else
    {
        MoveOnLine_4C5DA0();

        if (field_106_current_motion == eSlogMotions::M_Walk_1_4C60C0)
        {
            if (field_20_animation.field_92_current_frame == 2 || field_20_animation.field_92_current_frame == 11)
            {
                if (field_108_next_motion == eSlogMotions::M_Idle_0_4C5F90)
                {
                    field_106_current_motion = eSlogMotions::M_EndWalking_9_4C6310;
                    field_108_next_motion = -1;
                }
                if (field_108_next_motion == eSlogMotions::M_TurnAround_3_4C65C0)
                {
                    field_106_current_motion = eSlogMotions::M_EndWalking_9_4C6310;
                }
            }
            else if (field_20_animation.field_92_current_frame == 5 || field_20_animation.field_92_current_frame == 14)
            {
                Sfx_4C7D30(SlogSound::SlowStep_18);

                if (!field_160_flags.Get(Flags_160::eBit8_Asleep))
                {
                    field_160_flags.Set(Flags_160::eBit8_Asleep);
                    MapFollowMe_408D10(FALSE);
                }

                if (field_108_next_motion == eSlogMotions::M_Run_2_4C6340)
                {
                    field_106_current_motion = eSlogMotions::M_Run_2_4C6340;
                    field_108_next_motion = -1;
                }
            }
            else
            {
                field_160_flags.Clear(Flags_160::eBit8_Asleep);
            }
        }
    }
}

static u8 Slog_NextRandom()
{
    return sRandomBytes_546744[sSlogRandomIdx_BAF7F0++];
}


const FP sSlogRunVelXTable_547634[8] = {
    FP_FromDouble(8.625900268554688),
    FP_FromDouble(3.38677978515625),
    FP_FromDouble(3.5192413330078125),
    FP_FromDouble(5.1022491455078125),
    FP_FromDouble(6.7406005859375),
    FP_FromDouble(7.8218231201171875),
    FP_FromDouble(8.39404296875),
    FP_FromDouble(6.532867431640625)};

void Slog::M_Run_2_4C6340()
{
    if (gMap_5C3030.GetDirection_4811A0(field_C2_lvl_number, field_C0_path_number, field_B8_xpos, field_BC_ypos) >= CameraPos::eCamCurrent_0)
    {
        MusicController::PlayMusic_47FD60(MusicController::MusicTypes::eIntenseChase_7, this, 0, 0);
    }

    if (field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX))
    {
        field_C4_velx = (field_CC_sprite_scale * -sSlogRunVelXTable_547634[field_20_animation.field_92_current_frame]);
    }
    else
    {
        field_C4_velx = (field_CC_sprite_scale * sSlogRunVelXTable_547634[field_20_animation.field_92_current_frame]);
    }

    if (CollisionCheck_4C5480(field_CC_sprite_scale * FP_FromInteger(20), field_C4_velx * FP_FromInteger(4)))
    {
        ToIdle_4C5C10();
    }
    else
    {
        MoveOnLine_4C5DA0();

        if (field_106_current_motion == 2)
        {
            if (Slog_NextRandom() < 35u)
            {
                Sfx_4C7D30(SlogSound::CautiousWoof_5);
            }

            if (field_20_animation.field_92_current_frame == 4 || field_20_animation.field_92_current_frame == 7)
            {
                Sfx_4C7D30(SlogSound::FastStep_17);

                if (!field_160_flags.Get(Flags_160::eBit8_Asleep))
                {
                    field_160_flags.Set(Flags_160::eBit8_Asleep);
                    MapFollowMe_408D10(FALSE);
                }

                if (field_108_next_motion == eSlogMotions::M_Idle_0_4C5F90)
                {
                    field_106_current_motion = eSlogMotions::M_StopRunning_6_4C66C0;
                    field_108_next_motion = -1;
                }
                else if (field_108_next_motion == eSlogMotions::M_JumpForwards_18_4C7210)
                {
                    ToJump_4C5C60();
                    field_108_next_motion = -1;
                }
                else if (field_108_next_motion != -1)
                {
                    field_106_current_motion = field_108_next_motion;
                    field_108_next_motion = -1;
                }
            }
            else
            {
                field_160_flags.Clear(Flags_160::eBit8_Asleep);
            }
        }
    }
}

void Slog::M_TurnAround_3_4C65C0()
{
    if (field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        field_20_animation.field_4_flags.Toggle(AnimFlags::eBit5_FlipX);
        ToIdle_4C5C10();
    }
}

void Slog::M_Fall_4_4C6930()
{
    field_C8_vely += (field_CC_sprite_scale * FP_FromDouble(1.8));
    if (field_C8_vely > (field_CC_sprite_scale * FP_FromInteger(20)))
    {
        field_C8_vely = (field_CC_sprite_scale * FP_FromInteger(20));
    }

    if (field_C4_velx > FP_FromInteger(0))
    {
        if (field_C4_velx > (FP_FromInteger(8) * field_CC_sprite_scale))
        {
            field_C4_velx = (FP_FromInteger(8) * field_CC_sprite_scale);
        }
    }
    else if (field_C4_velx < FP_FromInteger(0))
    {
        if (field_C4_velx < -(FP_FromInteger(8) * field_CC_sprite_scale))
        {
            field_C4_velx = -(FP_FromInteger(8) * field_CC_sprite_scale);
        }
    }

    if (field_C4_velx > FP_FromInteger(0))
    {
        field_C4_velx -= (field_CC_sprite_scale * field_128_falling_velx_scale_factor);
        if (field_C4_velx < FP_FromInteger(0))
        {
            field_C4_velx = FP_FromInteger(0);
        }
    }
    else if (field_C4_velx < FP_FromInteger(0))
    {
        field_C4_velx += (field_CC_sprite_scale * field_128_falling_velx_scale_factor);
        if (field_C4_velx > FP_FromInteger(0))
        {
            field_C4_velx = FP_FromInteger(0);
        }
    }

    const FP xposBeforeChange = field_B8_xpos;
    const FP yposBeforeChange = field_BC_ypos;

    field_B8_xpos += field_C4_velx;
    field_BC_ypos += field_C8_vely;

    FP hitX = {};
    FP hitY = {};
    PathLine* pLine = nullptr;
    if (sCollisions_DArray_5C1128->Raycast_417A60(
            xposBeforeChange,
            yposBeforeChange - (field_CC_sprite_scale * FP_FromInteger(20)),
            field_B8_xpos,
            field_BC_ypos,
            &pLine, &hitX, &hitY, 15))
    {
        switch (pLine->field_8_type)
        {
            case eLineTypes::eFloor_0:
            case eLineTypes::eBackgroundFloor_4:
            case eLineTypes::eUnknown_32:
            case eLineTypes::eUnknown_36:
                field_100_pCollisionLine = pLine;
                field_BC_ypos = hitY;
                field_B8_xpos = hitX;
                MapFollowMe_408D10(FALSE);
                if (field_100_pCollisionLine->field_8_type == eLineTypes::eUnknown_32 || field_100_pCollisionLine->field_8_type == eLineTypes::eUnknown_36)
                {
                    PSX_RECT bRect = {};
                    vGetBoundingRect_424FD0(&bRect, 1);

                    const PSX_Point xy = {bRect.x, static_cast<s16>(bRect.y + 5)};
                    const PSX_Point wh = {bRect.w, static_cast<s16>(FP_GetExponent(field_BC_ypos) + 5)};
                    vOnCollisionWith_424EE0(
                        xy,
                        wh,
                        ObjList_5C1B78,
                        1,
                        (TCollisionCallBack) &BaseAliveGameObject::OnTrapDoorIntersection_408BA0);
                }
                field_106_current_motion = eSlogMotions::M_Land_10_4C7820;
                break;

            case eLineTypes::eWallLeft_1:
            case eLineTypes::eWallRight_2:
            case eLineTypes::eBackgroundWallLeft_5:
            case eLineTypes::eBackgroundWallRight_6:
                field_BC_ypos = hitY;
                field_B8_xpos = hitX - field_C4_velx;
                MapFollowMe_408D10(FALSE);
                field_C4_velx = FP_FromInteger(0);
                break;

            default:
                return;
        }
    }
}

void Slog::M_MoveHeadUpwards_5_4C5F20()
{
    if (field_20_animation.field_92_current_frame == 0)
    {
        Sfx_4C7D30(SlogSound::IdleWoof_2);
        field_132_has_woofed = 1;
    }

    if (field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        if (Math_RandomRange_496AB0(0, 100) < 30)
        {
            Sfx_4C7D30(SlogSound::IdleGrrAlt_4);
        }
        ToIdle_4C5C10();
    }
}

const FP sSlogStopRunningVelX_547658[10] = {
    FP_FromDouble(3.468994140625),
    FP_FromDouble(4.5489959716796875),
    FP_FromDouble(8.642990112304688),
    FP_FromDouble(9.31298828125),
    FP_FromDouble(6.4949951171875),
    FP_FromDouble(4.631988525390625),
    FP_FromDouble(3.9169921875),
    FP_FromDouble(4.334991455078125),
    FP_FromDouble(3.6609954833984375),
    FP_FromDouble(1.2819976806640625)};

void Slog::M_StopRunning_6_4C66C0()
{
    if (field_20_animation.field_92_current_frame == 0)
    {
        Sfx_4C7D30(SlogSound::Skid_14);
    }

    FP velX = {};
    if (field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX))
    {
        velX = -sSlogStopRunningVelX_547658[field_20_animation.field_92_current_frame];
    }
    else
    {
        velX = sSlogStopRunningVelX_547658[field_20_animation.field_92_current_frame];
    }

    field_C4_velx = (field_CC_sprite_scale * velX);

    if (!CollisionCheck_4C5480(field_CC_sprite_scale * FP_FromInteger(20), field_C4_velx * FP_FromInteger(4)))
    {
        MoveOnLine_4C5DA0();
        if (!field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
        {
            return;
        }
    }

    ToIdle_4C5C10();
}

const FP sSlogSlideTurnVelXTable_547684[25] = {
    FP_FromDouble(17.197998046875),
    FP_FromDouble(11.907989501953125),
    FP_FromDouble(8.52899169921875),
    FP_FromDouble(7.33599853515625),
    FP_FromDouble(4.168212890625),
    FP_FromDouble(5.3128204345703125),
    FP_FromDouble(3.81121826171875),
    FP_FromDouble(1.503692626953125),
    FP_FromDouble(0.5045166015625),
    FP_FromDouble(-0.2426605224609375),
    FP_FromDouble(-0.5961456298828125),
    FP_FromDouble(-1.2430877685546875),
    FP_FromDouble(-4.89166259765625)};


void Slog::M_SlideTurn_7_4C6790()
{
    FP velX = {};
    if (field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX))
    {
        velX = -sSlogSlideTurnVelXTable_547684[field_20_animation.field_92_current_frame];
    }
    else
    {
        velX = sSlogSlideTurnVelXTable_547684[field_20_animation.field_92_current_frame];
    }

    field_C4_velx = (field_CC_sprite_scale * velX);
    if (CollisionCheck_4C5480(field_CC_sprite_scale * FP_FromInteger(20), field_C4_velx * FP_FromInteger(4)))
    {
        ToIdle_4C5C10();
    }
    else
    {
        MoveOnLine_4C5DA0();

        if (field_106_current_motion == eSlogMotions::M_SlideTurn_7_4C6790)
        {
            if (field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
            {
                MapFollowMe_408D10(FALSE);

                if (field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX))
                {
                    field_20_animation.field_4_flags.Clear(AnimFlags::eBit5_FlipX);
                    field_C4_velx = (ScaleToGridSize_4498B0(field_CC_sprite_scale) / FP_FromInteger(3));
                }
                else
                {
                    field_20_animation.field_4_flags.Set(AnimFlags::eBit5_FlipX);
                    field_C4_velx = -(ScaleToGridSize_4498B0(field_CC_sprite_scale) / FP_FromInteger(3));
                }
                field_106_current_motion = eSlogMotions::M_Run_2_4C6340;
            }
        }
    }
}

void Slog::M_StartWalking_8_4C62E0()
{
    if (field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        field_106_current_motion = eSlogMotions::M_Walk_1_4C60C0;
    }

    MoveOnLine_4C5DA0();
}

void Slog::M_EndWalking_9_4C6310()
{
    MoveOnLine_4C5DA0();

    if (field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        ToIdle_4C5C10();
    }
}

void Slog::M_Land_10_4C7820()
{
    if (field_20_animation.field_92_current_frame == 0)
    {
        Sfx_4C7D30(SlogSound::Landing_16);
    }

    if (field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        ToIdle_4C5C10();
    }
}

void Slog::M_Unused_11_4C7860()
{
    if (field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        field_106_current_motion = eSlogMotions::M_StartFastBarking_12_4C7880;
    }
}

void Slog::M_StartFastBarking_12_4C7880()
{
    if (field_20_animation.field_92_current_frame == 0)
    {
        Sfx_4C7D30(SlogSound::IdleWoof_2);
        field_132_has_woofed = 1;
    }

    if (field_108_next_motion != -1)
    {
        if (field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
        {
            field_106_current_motion = eSlogMotions::M_EndFastBarking_13_4C78D0;
        }
    }
}

void Slog::M_EndFastBarking_13_4C78D0()
{
    if (field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        ToIdle_4C5C10();
    }
}

void Slog::M_AngryBark_14_4C6CF0()
{
    if (field_20_animation.field_92_current_frame == 0 || field_20_animation.field_92_current_frame == 6)
    {
        Sfx_4C7D30(SlogSound::CautiousWoof_5);
    }

    if (field_108_next_motion != -1)
    {
        if (field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
        {
            field_106_current_motion = field_108_next_motion;
            field_108_next_motion = -1;
        }
    }
}

void Slog::M_Sleeping_15_4C6D60()
{
    if (field_108_next_motion != -1 && field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        field_106_current_motion = field_108_next_motion;
        field_108_next_motion = -1;
        return;
    }

    bool createParticle = false;
    if (static_cast<s32>(sGnFrame_5C1B84) % 60)
    {
        if (!(static_cast<s32>((sGnFrame_5C1B84 - 20)) % 60))
        {
            Sfx_4C7D30(SlogSound::YawnEnd_11);
            createParticle = true;
        }
    }
    else
    {
        Sfx_4C7D30(SlogSound::YawnStart_10);
        createParticle = true;
    }

    if (createParticle)
    {
        if (gMap_5C3030.Is_Point_In_Current_Camera_4810D0(field_C2_lvl_number, field_C0_path_number, field_B8_xpos, field_BC_ypos, 0))
        {
            auto pSnoozeParticle = ae_new<SnoozeParticle>();
            if (pSnoozeParticle)
            {
                FP xOff = {};
                if (field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX))
                {
                    xOff = -(field_CC_sprite_scale * FP_FromInteger(18));
                }
                else
                {
                    xOff = (field_CC_sprite_scale * FP_FromInteger(18));
                }
                pSnoozeParticle->ctor_4B06F0(
                    xOff + field_B8_xpos,
                    (field_CC_sprite_scale * FP_FromInteger(-13)) + field_BC_ypos,
                    field_20_animation.field_C_render_layer, field_20_animation.field_14_scale);
            }
        }
    }
}

void Slog::M_MoveHeadDownwards_16_4C70D0()
{
    if (field_108_next_motion != -1)
    {
        if (field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
        {
            field_106_current_motion = field_108_next_motion;
            field_108_next_motion = -1;
        }
    }

    MusicController::PlayMusic_47FD60(MusicController::MusicTypes::eNone_0, this, 0, 0);
}

void Slog::M_WakeUp_17_4C7000()
{
    for (s32 i = 0; i < gBaseGameObject_list_BB47C4->Size(); i++)
    {
        auto pObj = gBaseGameObject_list_BB47C4->ItemAt(i);
        if (!pObj)
        {
            break;
        }

        if (pObj->Type() == AETypes::eSnoozeParticle_124)
        {
            static_cast<SnoozeParticle*>(pObj)->field_1E4_state = SnoozeParticle::SnoozeParticleState::eBlowingUp_2;
        }
    }

    if (field_108_next_motion != -1)
    {
        if (field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
        {
            field_106_current_motion = field_108_next_motion;
            field_108_next_motion = -1;
            Sfx_4C7D30(SlogSound::IdleGrrAlt_4);
        }
    }

    if (gMap_5C3030.GetDirection_4811A0(field_C2_lvl_number, field_C0_path_number, field_B8_xpos, field_BC_ypos) >= CameraPos::eCamCurrent_0)
    {
        MusicController::PlayMusic_47FD60(MusicController::MusicTypes::eSlogTension_5, this, 0, 0);
    }
}

void Slog::M_JumpForwards_18_4C7210()
{
    field_C8_vely += (field_CC_sprite_scale * FP_FromDouble(1.8));

    const FP k20Scaled = field_CC_sprite_scale * FP_FromInteger(20);
    if (field_C8_vely > k20Scaled)
    {
        field_C8_vely = k20Scaled;
    }

    const FP oldXPos = field_B8_xpos;
    const FP oldYPos = field_BC_ypos;

    field_B8_xpos += field_C4_velx;
    field_BC_ypos += field_C8_vely;

    PathLine* pLine = nullptr;
    FP hitX = {};
    FP hitY = {};
    if (sCollisions_DArray_5C1128->Raycast_417A60(oldXPos, oldYPos - k20Scaled, field_B8_xpos, field_BC_ypos, &pLine, &hitX, &hitY, 15) == 1)
    {
        switch (pLine->field_8_type)
        {
            case 0u:
            case 4u:
            case 32u:
            case 36u:
                if (field_C8_vely > FP_FromInteger(0))
                {
                    field_100_pCollisionLine = pLine;
                    field_108_next_motion = -1;
                    field_106_current_motion = eSlogMotions::M_Run_2_4C6340;
                    field_BC_ypos = hitY;
                    field_C8_vely = FP_FromInteger(0);
                }
                break;

            case 1u:
            case 5u:
                if (field_C4_velx < FP_FromInteger(0))
                {
                    field_C4_velx = (-field_C4_velx / FP_FromInteger(2));
                }
                break;

            case 2u:
            case 6u:
                if (field_C4_velx > FP_FromInteger(0))
                {
                    field_C4_velx = (-field_C4_velx / FP_FromInteger(2));
                }
                break;

            default:
                break;
        }
    }

    if (field_BC_ypos - field_F8_LastLineYPos > FP_FromInteger(2))
    {
        field_128_falling_velx_scale_factor = FP_FromDouble(0.3);
        field_F8_LastLineYPos = field_BC_ypos;
        field_106_current_motion = eSlogMotions::M_Fall_4_4C6930;
    }
}

void Slog::M_JumpUpwards_19_4C7470()
{
    PSX_RECT bRect = {};
    vGetBoundingRect_424FD0(&bRect, 1);

    PathLine* pLine = nullptr;
    FP hitX = {};
    FP hitY = {};
    if (sCollisions_DArray_5C1128->Raycast_417A60(
            FP_FromInteger(bRect.x),
            FP_FromInteger(bRect.y),
            FP_FromInteger(bRect.w),
            FP_FromInteger(bRect.h),
            &pLine, &hitX, &hitY, 8))
    {
        if (field_20_animation.field_92_current_frame < 12)
        {
            field_20_animation.SetFrame_409D50(24 - field_20_animation.field_92_current_frame);
        }
    }

    if (gMap_5C3030.GetDirection_4811A0(field_C2_lvl_number, field_C0_path_number, field_B8_xpos, field_BC_ypos) >= CameraPos::eCamCurrent_0)
    {
        MusicController::PlayMusic_47FD60(MusicController::MusicTypes::eSlogChaseTension_6, this, 0, 0);
    }

    if (field_20_animation.field_92_current_frame == 5)
    {
        if (field_160_flags.Get(Flags_160::eBit4_Hungry) && field_118_target_id == sActiveHero_5C1B68->field_8_object_id && sActiveHero_5C1B68->field_D6_scale == field_D6_scale && (sActiveHero_5C1B68->field_106_current_motion == eAbeMotions::Motion_104_RockThrowStandingHold_455DF0 || sActiveHero_5C1B68->field_106_current_motion == eAbeMotions::Motion_107_RockThrowCrouchingHold_454410))
        {
            Sfx_4C7D30(SlogSound::HungryYip_13);
        }
        else
        {
            Sfx_4C7D30(SlogSound::JumpBite_6);
        }
    }

    if (field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        field_106_current_motion = eSlogMotions::M_Idle_0_4C5F90;
        field_108_next_motion = -1;
    }
}

void Slog::M_Eating_20_4C75F0()
{
    SND_SEQ_Stop_4CAE60(SeqId::Empty_13);
    if (field_20_animation.field_92_current_frame == 0)
    {
        field_20_animation.field_4_flags.Clear(AnimFlags::eBit19_LoopBackwards);
        if (field_108_next_motion != -1 && field_108_next_motion != eSlogMotions::M_Eating_20_4C75F0)
        {
            field_106_current_motion = eSlogMotions::M_Idle_0_4C5F90;
            return;
        }
    }

    if (field_20_animation.field_92_current_frame == 3 && !field_20_animation.field_4_flags.Get(AnimFlags::eBit19_LoopBackwards))
    {
        SFX_Play_46FA90(static_cast<SoundEffect>(Math_RandomRange_496AB0(SoundEffect::Eating1_65, SoundEffect::Eating2_66)), 100);
        auto pBlood = ae_new<Blood>();
        if (pBlood)
        {
            pBlood->ctor_40F0B0(
                ((field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX)) != 0 ? FP_FromInteger(-25) : FP_FromInteger(25)) * field_CC_sprite_scale + field_B8_xpos,
                field_BC_ypos - (FP_FromInteger(4) * field_CC_sprite_scale),
                FP_FromInteger(0), FP_FromInteger(0),
                field_CC_sprite_scale, 12);
        }
    }

    if (field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        if (Math_RandomRange_496AB0(0, 100) < 85)
        {
            if (static_cast<s32>(sGnFrame_5C1B84) > field_150_growl_timer && Math_RandomRange_496AB0(0, 100) < 60)
            {
                field_150_growl_timer = sGnFrame_5C1B84 + 16;
                Sfx_4C7D30(SlogSound::IdleGrrr_3);
            }
            field_20_animation.field_4_flags.Set(AnimFlags::eBit19_LoopBackwards);
        }
    }

    if (field_20_animation.field_92_current_frame == 0)
    {
        field_20_animation.field_4_flags.Clear(AnimFlags::eBit19_LoopBackwards);
    }
}

void Slog::M_Dying_21_4C77F0()
{
    if (!field_100_pCollisionLine)
    {
        M_Fall_4_4C6930();
        field_106_current_motion = eSlogMotions::M_Dying_21_4C77F0;
    }
}

void Slog::M_Scratch_22_4C7120()
{
    if (field_20_animation.field_92_current_frame == 4)
    {
        SND_SEQ_PlaySeq_4CA960(SeqId::Scratching_12, 1, 1);
    }

    if (field_108_next_motion != -1)
    {
        if (field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
        {
            field_106_current_motion = field_108_next_motion;
            field_108_next_motion = -1;
        }
    }
}

void Slog::M_Growl_23_4C7170()
{
    if (field_20_animation.field_92_current_frame == 3)
    {
        if (field_132_has_woofed)
        {
            Sfx_4C7D30(SlogSound::IdleGrrr_3);
            field_20_animation.field_4_flags.Clear(AnimFlags::eBit2_Animate);
            field_132_has_woofed = 0;
            field_150_growl_timer = sGnFrame_5C1B84 + 12;
        }
    }

    if (static_cast<s32>(sGnFrame_5C1B84) > field_150_growl_timer)
    {
        field_20_animation.field_4_flags.Set(AnimFlags::eBit2_Animate);
    }

    if (field_108_next_motion != -1)
    {
        if (field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
        {
            field_106_current_motion = field_108_next_motion;
            field_108_next_motion = -1;
        }
    }
}

const s16 sSlogResponseMotion_560930[3][10] = {
    {eSlogMotions::M_TurnAround_3_4C65C0,
     eSlogMotions::M_Run_2_4C6340,
     eSlogMotions::M_SlideTurn_7_4C6790,
     -1,
     eSlogMotions::M_SlideTurn_7_4C6790,
     -1,
     eSlogMotions::M_StopRunning_6_4C66C0,
     -2,
     0,
     0},
    {eSlogMotions::M_StartFastBarking_12_4C7880,
     eSlogMotions::M_Run_2_4C6340,
     eSlogMotions::M_StopRunning_6_4C66C0,
     -2,
     0, 0, 0, 0, 0, 0},
    {eSlogMotions::M_StartFastBarking_12_4C7880,
     -2,
     0, 0, 0, 0, 0, 0, 0, 0}};

s16 Slog::Brain_ListeningToSlig_0_4C3790()
{
    auto pObj = static_cast<BaseAliveGameObject*>(sObjectIds_5C1B70.Find_449CF0(field_138_listening_to_slig_id));

    // TODO: OG bug - return never used?
    //sObjectIds_5C1B70.Find_449CF0(field_118);

    if (!pObj || pObj->field_6_flags.Get(BaseGameObject::eDead_Bit3))
    {
        field_142_anger_level = 0;
        field_138_listening_to_slig_id = -1;
        field_118_target_id = -1;
        field_120_brain_state_idx = 1;
        return 0;
    }

    FP gridSize1Directed = ScaleToGridSize_4498B0(field_CC_sprite_scale) * FP_FromInteger(1);
    if (pObj->field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX))
    {
        gridSize1Directed = -gridSize1Directed;
    }

    const FP xpos1GridAHead = gridSize1Directed + pObj->field_B8_xpos;
    switch (field_122_brain_state_result)
    {
        case 0:
            return Brain_ListeningToSlig_State_0_Init();
        case 1:
            return Brain_ListeningToSlig_State_1_Idle(xpos1GridAHead);
        case 2:
            return Brain_ListeningToSlig_State_2_Listening(xpos1GridAHead, pObj);
        case 3:
            return Brain_ListeningToSlig_State_3_Walking(xpos1GridAHead);
        case 4:
            return Brain_ListeningToSlig_State_4_Running(xpos1GridAHead);
        case 5:
            return Brain_ListeningToSlig_State_5_Waiting();
        case 6:
            return Brain_ListeningToSlig_State_6_Responding();
        default:
            return field_122_brain_state_result;
    }
}

s16 Slog::Brain_ListeningToSlig_State_6_Responding()
{
    if (static_cast<s32>(sGnFrame_5C1B84) <= field_124_timer)
    {
        return field_122_brain_state_result;
    }

    field_124_timer = sGnFrame_5C1B84 + 10;
    field_108_next_motion = sSlogResponseMotion_560930[field_13E_response_index][field_140_response_part++];

    if (field_108_next_motion == -2)
    {
        field_108_next_motion = eSlogMotions::M_Idle_0_4C5F90;
        return 2;
    }
    else
    {
        return field_122_brain_state_result;
    }
}

s16 Slog::Brain_ListeningToSlig_State_5_Waiting()
{
    if (static_cast<s32>(sGnFrame_5C1B84) <= field_124_timer)
    {
        return field_122_brain_state_result;
    }
    field_13C_waiting_counter--;
    field_108_next_motion = eSlogMotions::M_MoveHeadUpwards_5_4C5F20;
    return 2;
}

s16 Slog::Brain_ListeningToSlig_State_4_Running(const FP xpos1GridAHead)
{
    if (field_106_current_motion == eSlogMotions::M_Idle_0_4C5F90)
    {
        return 2;
    }

    if (field_106_current_motion != eSlogMotions::M_Run_2_4C6340)
    {
        return field_122_brain_state_result;
    }

    if (Facing_4C4020(xpos1GridAHead))
    {
        if (FP_Abs(xpos1GridAHead - field_B8_xpos) > (ScaleToGridSize_4498B0(field_CC_sprite_scale) * FP_FromInteger(3)))
        {
            return field_122_brain_state_result;
        }
        field_108_next_motion = eSlogMotions::M_Idle_0_4C5F90;
        return 2;
    }
    else
    {
        field_108_next_motion = eSlogMotions::M_SlideTurn_7_4C6790;
        return field_122_brain_state_result;
    }
}

s16 Slog::Brain_ListeningToSlig_State_3_Walking(const FP xpos1GridAHead)
{
    if (field_106_current_motion == eSlogMotions::M_Idle_0_4C5F90)
    {
        return 2;
    }

    if (field_106_current_motion != eSlogMotions::M_Walk_1_4C60C0)
    {
        return field_122_brain_state_result;
    }

    if (!Facing_4C4020(xpos1GridAHead))
    {
        field_108_next_motion = eSlogMotions::M_TurnAround_3_4C65C0;
        return 2;
    }

    if (FP_Abs(xpos1GridAHead - field_B8_xpos) > (ScaleToGridSize_4498B0(field_CC_sprite_scale) * FP_FromInteger(4)))
    {
        field_108_next_motion = eSlogMotions::M_Run_2_4C6340;
        return 4;
    }

    if (FP_Abs(xpos1GridAHead - field_B8_xpos) > (ScaleToGridSize_4498B0(field_CC_sprite_scale) * FP_FromInteger(3)))
    {
        return field_122_brain_state_result;
    }

    field_108_next_motion = eSlogMotions::M_Idle_0_4C5F90;
    return 2;
}

s16 Slog::Brain_ListeningToSlig_State_2_Listening(const FP xpos1GridAHead, BaseAliveGameObject* pObj)
{
    if (field_106_current_motion != eSlogMotions::M_Idle_0_4C5F90)
    {
        return field_122_brain_state_result;
    }

    GameSpeakEvents speakValue = GameSpeakEvents::eNone_m1;
    if (field_134_last_event_index == pEventSystem_5BC11C->field_28_last_event_index)
    {
        if (pEventSystem_5BC11C->field_20_last_event == GameSpeakEvents::eNone_m1)
        {
            speakValue = GameSpeakEvents::eNone_m1;
        }
        else
        {
            speakValue = GameSpeakEvents::eSameAsLast_m2;
        }
    }
    else
    {
        field_134_last_event_index = pEventSystem_5BC11C->field_28_last_event_index;
        speakValue = pEventSystem_5BC11C->field_20_last_event;
    }

    switch (speakValue)
    {
        case GameSpeakEvents::Slig_LookOut_6:
            DelayedResponse_4C3750(2);
            return 6;

        case GameSpeakEvents::Slig_Hi_27:
            field_13C_waiting_counter++;
            if (static_cast<s32>(sGnFrame_5C1B84) % 2)
            {
                field_13C_waiting_counter++;
            }
            [[fallthrough]];

        case GameSpeakEvents::Slig_HereBoy_28:
            field_124_timer = sGnFrame_5C1B84 - Math_NextRandom() % 8 + 15;
            field_13C_waiting_counter++;
            break;

        case GameSpeakEvents::Slig_GetEm_29:
        {
            auto pTarget = FindTarget_4C33C0(1, 0);
            if (pTarget)
            {
                field_138_listening_to_slig_id = -1;
                field_160_flags.Set(Flags_160::eBit5_CommandedToAttack);
                field_118_target_id = pTarget->field_8_object_id;
                field_120_brain_state_idx = 2;
                return 0;
            }

            FP gridSize = {};
            if (field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX))
            {
                gridSize = -ScaleToGridSize_4498B0(field_CC_sprite_scale);
            }
            else
            {
                gridSize = ScaleToGridSize_4498B0(field_CC_sprite_scale);
            }

            if (!CollisionCheck_4C5480(field_CC_sprite_scale * FP_FromInteger(20), gridSize * FP_FromInteger(2)))
            {
                DelayedResponse_4C3750(1);
                return 6;
            }
            break;
        }

        case GameSpeakEvents::eUnknown_33:
            DelayedResponse_4C3750(0);
            return 6;

        default:
            break;
    }

    if (!(Math_NextRandom() % 128))
    {
        field_13C_waiting_counter++;
    }

    if (field_13C_waiting_counter > 0)
    {
        field_124_timer += Math_NextRandom() % 16;
        return 5;
    }

    if (FP_Abs(xpos1GridAHead - field_B8_xpos) > (ScaleToGridSize_4498B0(field_CC_sprite_scale) * FP_FromInteger(4)))
    {
        if (!Facing_4C4020(xpos1GridAHead))
        {
            field_108_next_motion = eSlogMotions::M_TurnAround_3_4C65C0;
            return field_122_brain_state_result;
        }

        FP gridSize2 = {};
        if (field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX))
        {
            gridSize2 = -ScaleToGridSize_4498B0(field_CC_sprite_scale);
        }
        else
        {
            gridSize2 = ScaleToGridSize_4498B0(field_CC_sprite_scale);
        }

        if (!CollisionCheck_4C5480(field_CC_sprite_scale * FP_FromInteger(20), gridSize2 * FP_FromInteger(2)))
        {
            field_108_next_motion = eSlogMotions::M_Run_2_4C6340;
            return 4;
        }
    }

    if (FP_Abs(xpos1GridAHead - field_B8_xpos) > (ScaleToGridSize_4498B0(field_CC_sprite_scale) * FP_FromInteger(1)))
    {
        if (!Facing_4C4020(xpos1GridAHead))
        {
            field_108_next_motion = eSlogMotions::M_TurnAround_3_4C65C0;
            return field_122_brain_state_result;
        }

        FP gridSize3 = {};
        if (field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX))
        {
            gridSize3 = -ScaleToGridSize_4498B0(field_CC_sprite_scale);
        }
        else
        {
            gridSize3 = ScaleToGridSize_4498B0(field_CC_sprite_scale);
        }

        if (!CollisionCheck_4C5480(field_CC_sprite_scale * FP_FromInteger(20), gridSize3 * FP_FromInteger(2)))
        {
            field_108_next_motion = eSlogMotions::M_Walk_1_4C60C0;
            return 3;
        }
    }

    if (pObj->field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX) != field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX))
    {
        field_108_next_motion = eSlogMotions::M_TurnAround_3_4C65C0;
    }

    return field_122_brain_state_result;
}

s16 Slog::Brain_ListeningToSlig_State_1_Idle(const FP xpos1GridAHead)
{
    if (field_106_current_motion != eSlogMotions::M_Idle_0_4C5F90)
    {
        return field_122_brain_state_result;
    }

    if (!Facing_4C4020(xpos1GridAHead))
    {
        field_108_next_motion = eSlogMotions::M_TurnAround_3_4C65C0;
        return field_122_brain_state_result;
    }

    if (static_cast<s32>(sGnFrame_5C1B84) <= field_124_timer)
    {
        return field_122_brain_state_result;
    }
    field_108_next_motion = eSlogMotions::M_MoveHeadUpwards_5_4C5F20;
    return 2;
}

s16 Slog::Brain_ListeningToSlig_State_0_Init()
{
    field_108_next_motion = eSlogMotions::M_Idle_0_4C5F90;
    field_13C_waiting_counter = 0;
    field_124_timer = sGnFrame_5C1B84 + 15;
    return 1;
}

s16 Slog::Brain_Idle_1_4C2830()
{
    BaseGameObject* pTarget = sObjectIds_5C1B70.Find_449CF0(field_118_target_id);

    // OG dead code - return never used
    //sObjectIds_5C1B70.Find_449CF0(field_138);

    if (pTarget && pTarget->field_6_flags.Get(BaseGameObject::eDead_Bit3))
    {
        field_118_target_id = -1;
    }

    if (field_134_last_event_index != pEventSystem_5BC11C->field_28_last_event_index)
    {
        field_134_last_event_index = pEventSystem_5BC11C->field_28_last_event_index;
        if (pEventSystem_5BC11C->field_20_last_event == GameSpeakEvents::Slig_HereBoy_28 && sControlledCharacter_5C1B8C->Type() == AETypes::eSlig_125)
        {
            field_120_brain_state_idx = 0;
            field_118_target_id = -1;
            field_138_listening_to_slig_id = sControlledCharacter_5C1B8C->field_8_object_id;
            return 0;
        }
    }

    if (SwitchStates_Get_466020(field_154_anger_switch_id))
    {
        field_160_flags.Clear(Flags_160::eBit5_CommandedToAttack);
        field_120_brain_state_idx = 2;
        return 0;
    }

    switch (field_122_brain_state_result)
    {
        case 0:
            if (field_106_current_motion != eSlogMotions::M_Sleeping_15_4C6D60 && field_106_current_motion != eSlogMotions::M_Idle_0_4C5F90)
            {
                field_108_next_motion = eSlogMotions::M_Idle_0_4C5F90;
                return field_122_brain_state_result;
            }

            if (field_160_flags.Get(Flags_160::eBit7_Asleep))
            {
                field_142_anger_level = 0;
                return 1;
            }
            else
            {
                field_142_anger_level = field_144_wake_up_anger;
                return 4;
            }
            break;

        case 1:
            if (Event_Is_Event_In_Range_422C30(kEventSuspiciousNoise, field_B8_xpos, field_BC_ypos, 1))
            {
                field_142_anger_level++;
            }

            if (Event_Is_Event_In_Range_422C30(kEventSpeaking, field_B8_xpos, field_BC_ypos, 1))
            {
                field_142_anger_level += Slog_NextRandom() % 8 + 15;
            }

            if (!(static_cast<s32>(sGnFrame_5C1B84) % 16))
            {
                // Calm down a bit
                if (field_142_anger_level > 0)
                {
                    field_142_anger_level--;
                }
            }

            if (field_142_anger_level <= field_144_wake_up_anger)
            {
                return field_122_brain_state_result;
            }

            field_108_next_motion = eSlogMotions::M_WakeUp_17_4C7000;
            return 2;

        case 2:
            if (field_106_current_motion != eSlogMotions::M_WakeUp_17_4C7000)
            {
                return field_122_brain_state_result;
            }

            field_108_next_motion = eSlogMotions::M_MoveHeadUpwards_5_4C5F20;
            field_14C_scratch_timer = Math_NextRandom() % 32 + sGnFrame_5C1B84 + 120;
            field_150_growl_timer = Math_NextRandom() % 32 + sGnFrame_5C1B84 + 60;
            return 4;

        case 3:
            if (field_106_current_motion != eSlogMotions::M_MoveHeadDownwards_16_4C70D0)
            {
                if (field_108_next_motion != eSlogMotions::M_MoveHeadDownwards_16_4C70D0)
                {
                    field_108_next_motion = eSlogMotions::M_MoveHeadDownwards_16_4C70D0;
                }

                return field_122_brain_state_result;
            }

            field_108_next_motion = eSlogMotions::M_Sleeping_15_4C6D60;
            return 1;

        case 4:
            if (Event_Is_Event_In_Range_422C30(kEventSuspiciousNoise, field_B8_xpos, field_BC_ypos, 1))
            {
                field_142_anger_level++;
            }

            if (Event_Is_Event_In_Range_422C30(kEventSpeaking, field_B8_xpos, field_BC_ypos, 1))
            {
                field_142_anger_level += Slog_NextRandom() % 8 + 15;
            }

            if (!(static_cast<s32>(sGnFrame_5C1B84) % 32))
            {
                if (field_142_anger_level)
                {
                    if (field_160_flags.Get(Flags_160::eBit7_Asleep))
                    {
                        field_142_anger_level--;
                    }
                }
            }

            if (PlayerOrNakedSligNear_4C26A0())
            {
                field_142_anger_level += 2;
            }

            if (!(Slog_NextRandom() % 64) && field_106_current_motion == eSlogMotions::M_Idle_0_4C5F90)
            {
                field_106_current_motion = eSlogMotions::M_MoveHeadUpwards_5_4C5F20;
                return field_122_brain_state_result;
            }

            if (static_cast<s32>(sGnFrame_5C1B84) > field_150_growl_timer && field_106_current_motion == eSlogMotions::M_Idle_0_4C5F90)
            {
                field_150_growl_timer = Math_NextRandom() % 32 + sGnFrame_5C1B84 + 60;
                field_106_current_motion = eSlogMotions::M_Growl_23_4C7170;
                field_108_next_motion = eSlogMotions::M_Idle_0_4C5F90;
                sGnFrame_5C1B84 = sGnFrame_5C1B84;
            }

            if (static_cast<s32>(sGnFrame_5C1B84) > field_14C_scratch_timer && field_106_current_motion == eSlogMotions::M_Idle_0_4C5F90)
            {
                field_14C_scratch_timer = Math_NextRandom() % 32 + sGnFrame_5C1B84 + 120;
                field_106_current_motion = eSlogMotions::M_Scratch_22_4C7120;
                field_108_next_motion = eSlogMotions::M_Idle_0_4C5F90;
            }

            if (field_142_anger_level > field_146_total_anger)
            {
                field_108_next_motion = eSlogMotions::M_AngryBark_14_4C6CF0;
                field_142_anger_level = field_142_anger_level + Slog_NextRandom() % 8;
                return 5;
            }

            if (field_142_anger_level >= field_144_wake_up_anger)
            {
                return field_122_brain_state_result;
            }

            field_108_next_motion = eSlogMotions::M_MoveHeadDownwards_16_4C70D0;
            return 3;

        case 5:
            if (Event_Is_Event_In_Range_422C30(kEventSuspiciousNoise, field_B8_xpos, field_BC_ypos, 1))
            {
                field_142_anger_level++;
            }

            if (Event_Is_Event_In_Range_422C30(kEventSpeaking, field_B8_xpos, field_BC_ypos, 1))
            {
                field_142_anger_level += Math_NextRandom() % 8 + 15;
            }

            if (!(static_cast<s32>(sGnFrame_5C1B84) % 2))
            {
                if (field_142_anger_level > 0)
                {
                    field_142_anger_level--;
                }
            }

            if (PlayerOrNakedSligNear_4C26A0())
            {
                field_142_anger_level += 2;
            }

            if (field_142_anger_level >= field_146_total_anger)
            {
                if (field_142_anger_level <= field_148_chase_anger)
                {
                    return field_122_brain_state_result;
                }
                else
                {
                    field_160_flags.Clear(Flags_160::eBit5_CommandedToAttack);
                    field_120_brain_state_idx = 2;
                    return 0;
                }
            }
            else
            {
                field_106_current_motion = eSlogMotions::M_Idle_0_4C5F90;
                field_14C_scratch_timer = Math_NextRandom() % 32 + sGnFrame_5C1B84 + 120;
                field_150_growl_timer = Math_NextRandom() % 32 + sGnFrame_5C1B84 + 60;
                return 4;
            }
            break;

        default:
            return field_122_brain_state_result;
    }
}

s16 Slog::Brain_ChasingAbe_2_4C0A00()
{
    auto pTarget = static_cast<BaseAliveGameObject*>(sObjectIds_5C1B70.Find_449CF0(field_118_target_id));
    if (field_160_flags.Get(Flags_160::eBit2_ListenToSligs))
    {
        if (field_134_last_event_index != pEventSystem_5BC11C->field_28_last_event_index)
        {
            field_134_last_event_index = pEventSystem_5BC11C->field_28_last_event_index;
            if (pEventSystem_5BC11C->field_20_last_event == GameSpeakEvents::Slig_HereBoy_28 && sControlledCharacter_5C1B8C->Type() == AETypes::eSlig_125)
            {
                field_120_brain_state_idx = 0;
                field_118_target_id = -1;
                field_138_listening_to_slig_id = sControlledCharacter_5C1B8C->field_8_object_id;
                return 0;
            }
        }
    }

    bool updateTarget = false;
    if (!pTarget)
    {
        if (field_118_target_id != -1)
        {
            field_118_target_id = -1;
            field_142_anger_level = 0;
            field_120_brain_state_idx = 1;
            field_108_next_motion = eSlogMotions::M_Idle_0_4C5F90;
            return 0;
        }
        updateTarget = true;
    }

    if (updateTarget || !field_160_flags.Get(Flags_160::eBit5_CommandedToAttack) || pTarget->field_CC_sprite_scale == FP_FromDouble(0.5))
    {
        if (!field_11C_biting_target)
        {
            pTarget = FindTarget_4C33C0(0, 0);
            if (!pTarget)
            {
                pTarget = FindTarget_4C33C0(0, 1);
                if (!pTarget)
                {
                    pTarget = sControlledCharacter_5C1B8C;
                    if (sControlledCharacter_5C1B8C->field_CC_sprite_scale == FP_FromDouble(0.5))
                    {
                        field_118_target_id = -1;
                        field_142_anger_level = 0;
                        field_120_brain_state_idx = 1;
                        field_108_next_motion = eSlogMotions::M_Idle_0_4C5F90;
                        return 0;
                    }
                }
            }
            field_118_target_id = pTarget->field_8_object_id;
        }
    }

    switch (field_122_brain_state_result)
    {
        case 0:
            return Brain_ChasingAbe_State_0_Init();
        case 1:
            return Brain_ChasingAbe_State_1_Waiting();
        case 2:
            return Brain_ChasingAbe_State_2_Thinking(pTarget);
        case 3:
            return Brain_ChasingAbe_State_3_GrowlOrScratch(pTarget);
        case 4:
            return Brain_ChasingAbe_State_4_LungingAtTarget(pTarget);
        case 7:
            return Brain_ChasingAbe_State_7_EatingTarget(pTarget);
        case 8:
            return Brain_ChasingAbe_State_8_ToIdle();
        case 9:
            return Brain_ChasingAbe_State_9_Falling();
        case 10:
            return Brain_ChasingAbe_State_10_HungryForBone();
        case 11:
            return Brain_ChasingAbe_State_11_ChasingAfterBone();
        case 12:
            return Brain_ChasingAbe_State_12_WalkingToBone();
        case 13:
            return Brain_ChasingAbe_State_13_EatingBone();
        case 14:
            return Brain_ChasingAbe_State_14_CheckingIfBoneNearby();
        case 15:
            return Brain_ChasingAbe_State_15_ChasingAfterTarget(pTarget);
        case 16:
            return Brain_ChasingAbe_State_16_JumpingUpwards();
        case 17:
            return Brain_ChasingAbe_State_17_WaitingToChase(pTarget);
        case 18:
            return Brain_ChasingAbe_State_18_WaitingToJump(pTarget);
        case 19:
            return Brain_ChasingAbe_State_19_AboutToCollide(pTarget);
        case 20:
            return Brain_ChasingAbe_State_20_Collided(pTarget);
        default:
            return field_122_brain_state_result;
    }
}

s16 Slog::Brain_ChasingAbe_State_19_AboutToCollide(BaseAliveGameObject* pTarget)
{
    FP gridSize = {};

    if (field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX))
    {
        gridSize = -ScaleToGridSize_4498B0(field_CC_sprite_scale);
    }
    else
    {
        gridSize = ScaleToGridSize_4498B0(field_CC_sprite_scale);
    }

    if (!CollisionCheck_4C5480(field_CC_sprite_scale * FP_FromInteger(20), FP_FromInteger(2) * gridSize))
    {
        return 2;
    }

    if (pTarget)
    {
        if (vIsObjNearby_4253B0(ScaleToGridSize_4498B0(field_CC_sprite_scale), pTarget))
        {
            if (pTarget->field_10C_health > FP_FromInteger(0) && vOnSameYLevel_425520(pTarget))
            {
                field_108_next_motion = eSlogMotions::M_JumpForwards_18_4C7210;
                return 4;
            }
        }
    }

    if (!(Slog_NextRandom() % 64))
    {
        field_106_current_motion = eSlogMotions::M_MoveHeadUpwards_5_4C5F20;
        return field_122_brain_state_result;
    }
    return Brain_ChasingAbe_State_20_Collided(pTarget);
}

s16 Slog::Brain_ChasingAbe_State_18_WaitingToJump(BaseAliveGameObject* pTarget)
{
    if (field_106_current_motion != eSlogMotions::M_Idle_0_4C5F90)
    {
        return field_122_brain_state_result;
    }

    field_15A_jump_counter -= 20;

    if (vIsObjNearby_4253B0(ScaleToGridSize_4498B0(field_CC_sprite_scale) * FP_FromInteger(3), pTarget))
    {
        return 15;
    }
    field_108_next_motion = eSlogMotions::M_Run_2_4C6340;
    return 2;
}

s16 Slog::Brain_ChasingAbe_State_17_WaitingToChase(BaseAliveGameObject* pTarget)
{
    if (field_106_current_motion == eSlogMotions::M_Idle_0_4C5F90)
    {
        if (!vIsFacingMe_4254A0(pTarget))
        {
            field_108_next_motion = eSlogMotions::M_TurnAround_3_4C65C0;
            return field_122_brain_state_result;
        }
        else
        {
            field_108_next_motion = eSlogMotions::M_MoveHeadUpwards_5_4C5F20;
        }
    }

    if (static_cast<s32>(sGnFrame_5C1B84) <= field_124_timer)
    {
        return field_122_brain_state_result;
    }

    if (!vIsObjNearby_4253B0(ScaleToGridSize_4498B0(field_CC_sprite_scale) * FP_FromInteger(3), pTarget))
    {
        field_108_next_motion = eSlogMotions::M_Run_2_4C6340;
    }
    return 2;
}

s16 Slog::Brain_ChasingAbe_State_16_JumpingUpwards()
{
    if (field_106_current_motion != eSlogMotions::M_Idle_0_4C5F90)
    {
        return field_122_brain_state_result;
    }
    field_15A_jump_counter += Slog_NextRandom() % 64;
    return 15;
}

s16 Slog::Brain_ChasingAbe_State_15_ChasingAfterTarget(BaseAliveGameObject* pTarget)
{
    if (field_C4_velx > FP_FromInteger(0) && HandleEnemyStopper_4C5340())
    {
        field_108_next_motion = eSlogMotions::M_StopRunning_6_4C66C0;
        field_160_flags.Set(Flags_160::eBit1_StopRunning, field_C4_velx < FP_FromInteger(0));
        field_14C_scratch_timer = Math_NextRandom() % 32 + sGnFrame_5C1B84 + 120;
        field_150_growl_timer = Math_NextRandom() % 32 + sGnFrame_5C1B84 + 60;
        return 20;
    }

    if (!vIsFacingMe_4254A0(pTarget) && field_106_current_motion == eSlogMotions::M_Run_2_4C6340)
    {
        field_108_next_motion = eSlogMotions::M_SlideTurn_7_4C6790;
    }

    if (vIsObjNearby_4253B0(ScaleToGridSize_4498B0(field_CC_sprite_scale) * FP_FromInteger(3), pTarget))
    {
        if (pTarget->field_CC_sprite_scale == FP_FromInteger(1) && field_106_current_motion == eSlogMotions::M_Run_2_4C6340)
        {
            if (vIsFacingMe_4254A0(pTarget))
            {
                field_108_next_motion = eSlogMotions::M_StopRunning_6_4C66C0;
            }
        }
    }

    if (field_106_current_motion == eSlogMotions::M_SlideTurn_7_4C6790)
    {
        field_108_next_motion = eSlogMotions::M_Run_2_4C6340;
    }

    if (field_BC_ypos >= pTarget->field_BC_ypos + FP_FromInteger(50))
    {
        auto pBone = FindBone_4C25B0();
        if (pBone)
        {
            field_15C_bone_id = pBone->field_8_object_id;
            return 11;
        }

        if (field_106_current_motion == eSlogMotions::M_Fall_4_4C6930)
        {
            return 9;
        }

        if (field_106_current_motion != eSlogMotions::M_Idle_0_4C5F90)
        {
            return field_122_brain_state_result;
        }

        if (vIsFacingMe_4254A0(pTarget))
        {
            if (field_15A_jump_counter < 100 && vIsObjNearby_4253B0(ScaleToGridSize_4498B0(field_CC_sprite_scale) * FP_FromInteger(3), pTarget))
            {
                field_108_next_motion = eSlogMotions::M_JumpUpwards_19_4C7470;
                return 16;
            }

            if (Math_RandomRange_496AB0(0, 100) < 20)
            {
                field_106_current_motion = eSlogMotions::M_Growl_23_4C7170;
            }

            field_108_next_motion = eSlogMotions::M_MoveHeadUpwards_5_4C5F20;
            return 18;
        }

        field_108_next_motion = eSlogMotions::M_TurnAround_3_4C65C0;
        return field_122_brain_state_result;
    }

    field_124_timer = sGnFrame_5C1B84 + field_158_chase_delay;
    return 17;
}

s16 Slog::Brain_ChasingAbe_State_14_CheckingIfBoneNearby()
{
    auto pBone = static_cast<Bone*>(sObjectIds_5C1B70.Find_449CF0(field_15C_bone_id));
    if (!pBone)
    {
        return 0;
    }

    if (FP_Abs(field_BC_ypos - pBone->field_BC_ypos) <= FP_FromInteger(50) || pBone->VCanBeEaten_411560())
    {
        if (pBone->VIsFalling_49E330())
        {
            field_15C_bone_id = -1;
            field_108_next_motion = eSlogMotions::M_Idle_0_4C5F90;
            return 2;
        }

        if (field_106_current_motion == eSlogMotions::M_Idle_0_4C5F90)
        {
            FP gridSize = {};
            if (field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX))
            {
                gridSize = -ScaleToGridSize_4498B0(field_CC_sprite_scale);
            }
            else
            {
                gridSize = ScaleToGridSize_4498B0(field_CC_sprite_scale);
            }

            if (CollisionCheck_4C5480(field_CC_sprite_scale * FP_FromInteger(20), gridSize * FP_FromInteger(2)))
            {
                field_108_next_motion = eSlogMotions::M_TurnAround_3_4C65C0;
            }
            else
            {
                field_108_next_motion = eSlogMotions::M_Walk_1_4C60C0;
            }
        }

        if (vIsObjNearby_4253B0(ScaleToGridSize_4498B0(field_CC_sprite_scale) * FP_FromInteger(1), pBone))
        {
            return field_122_brain_state_result;
        }
        return 12;
    }

    field_15C_bone_id = -1;
    return 2;
}

s16 Slog::Brain_ChasingAbe_State_13_EatingBone()
{
    auto pBone = static_cast<Bone*>(sObjectIds_5C1B70.Find_449CF0(field_15C_bone_id));
    if (!pBone || pBone->VIsFalling_49E330())
    {
        field_15C_bone_id = -1;
        field_108_next_motion = eSlogMotions::M_Idle_0_4C5F90;
        return 2;
    }

    if (FP_Abs(field_BC_ypos - pBone->field_BC_ypos) <= FP_FromInteger(50) || pBone->VCanBeEaten_411560())
    {
        if (vIsFacingMe_4254A0(pBone))
        {
            if (field_106_current_motion == eSlogMotions::M_Idle_0_4C5F90)
            {
                field_108_next_motion = eSlogMotions::M_Eating_20_4C75F0;
            }

            if (field_124_timer > static_cast<s32>(sGnFrame_5C1B84))
            {
                return field_122_brain_state_result;
            }

            pBone->field_6_flags.Set(BaseGameObject::eDead_Bit3);
            field_108_next_motion = eSlogMotions::M_Idle_0_4C5F90;
            field_15C_bone_id = -1;
            return 2;
        }

        field_108_next_motion = eSlogMotions::M_TurnAround_3_4C65C0;
        return 12;
    }

    field_15C_bone_id = -1;
    return 2;
}

s16 Slog::Brain_ChasingAbe_State_12_WalkingToBone()
{
    if (field_106_current_motion != eSlogMotions::M_Idle_0_4C5F90 && field_106_current_motion != eSlogMotions::M_Walk_1_4C60C0)
    {
        return field_122_brain_state_result;
    }

    auto pBone = static_cast<Bone*>(sObjectIds_5C1B70.Find_449CF0(field_15C_bone_id));
    if (!pBone || pBone->VIsFalling_49E330())
    {
        field_15C_bone_id = -1;
        field_108_next_motion = eSlogMotions::M_Idle_0_4C5F90;
        return 2;
    }

    if (field_106_current_motion == eSlogMotions::M_Idle_0_4C5F90)
    {
        field_108_next_motion = eSlogMotions::M_Walk_1_4C60C0;
    }

    if (FP_Abs(field_BC_ypos - pBone->field_BC_ypos) <= FP_FromInteger(50) || pBone->VCanBeEaten_411560())
    {
        if (vIsFacingMe_4254A0(pBone))
        {
            if (!vIsObjNearby_4253B0(ScaleToGridSize_4498B0(field_CC_sprite_scale) * FP_FromDouble(1.5), pBone))
            {
                field_108_next_motion = eSlogMotions::M_Walk_1_4C60C0;
            }

            if (!vIsObjNearby_4253B0(ScaleToGridSize_4498B0(field_CC_sprite_scale) * FP_FromDouble(1.5), pBone) || pBone->field_C4_velx > FP_FromInteger(0))
            {
                return field_122_brain_state_result;
            }

            field_108_next_motion = eSlogMotions::M_Idle_0_4C5F90;
            field_124_timer = sGnFrame_5C1B84 + field_156_bone_eating_time;
            return 13;
        }

        field_108_next_motion = eSlogMotions::M_TurnAround_3_4C65C0;
        return field_122_brain_state_result;
    }

    field_15C_bone_id = -1;
    return 2;
}

s16 Slog::Brain_ChasingAbe_State_11_ChasingAfterBone()
{
    auto pBone = static_cast<Bone*>(sObjectIds_5C1B70.Find_449CF0(field_15C_bone_id));
    if (!pBone || pBone->VIsFalling_49E330())
    {
        field_15C_bone_id = -1;
        field_108_next_motion = eSlogMotions::M_Idle_0_4C5F90;
        return 2;
    }

    if (FP_Abs(field_BC_ypos - pBone->field_BC_ypos) <= FP_FromInteger(50) || pBone->VCanBeEaten_411560())
    {
        if (!vIsFacingMe_4254A0(pBone) && field_106_current_motion == eSlogMotions::M_Run_2_4C6340)
        {
            field_108_next_motion = eSlogMotions::M_SlideTurn_7_4C6790;
        }

        if (vIsObjNearby_4253B0(ScaleToGridSize_4498B0(field_CC_sprite_scale) * FP_FromInteger(4), pBone))
        {
            if (FP_Abs(field_BC_ypos - pBone->field_BC_ypos) < FP_FromInteger(50) && field_106_current_motion == eSlogMotions::M_Run_2_4C6340)
            {
                field_108_next_motion = eSlogMotions::M_StopRunning_6_4C66C0;
                return 12;
            }
        }

        if (field_106_current_motion == eSlogMotions::M_SlideTurn_7_4C6790)
        {
            field_108_next_motion = eSlogMotions::M_Run_2_4C6340;
        }

        if (field_106_current_motion == eSlogMotions::M_Walk_1_4C60C0)
        {
            field_108_next_motion = eSlogMotions::M_Run_2_4C6340;
        }

        if (field_106_current_motion == eSlogMotions::M_Fall_4_4C6930)
        {
            field_15C_bone_id = -1;
            return 9;
        }

        if (field_106_current_motion != eSlogMotions::M_Idle_0_4C5F90)
        {
            return field_122_brain_state_result;
        }

        if (vIsFacingMe_4254A0(pBone))
        {
            FP gridSize = {};
            if (field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX))
            {
                gridSize = -ScaleToGridSize_4498B0(field_CC_sprite_scale);
            }
            else
            {
                gridSize = ScaleToGridSize_4498B0(field_CC_sprite_scale);
            }

            // TODO: Same check twice ??
            if (CollisionCheck_4C5480(field_CC_sprite_scale * FP_FromInteger(20), FP_FromInteger(4) * gridSize))
            {
                FP gridSize2 = {};
                if (field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX))
                {
                    gridSize2 = -ScaleToGridSize_4498B0(field_CC_sprite_scale);
                }
                else
                {
                    gridSize2 = ScaleToGridSize_4498B0(field_CC_sprite_scale);
                }

                if (CollisionCheck_4C5480(field_CC_sprite_scale * FP_FromInteger(20), FP_FromInteger(1) * gridSize2))
                {
                    if (Slog_NextRandom() % 32)
                    {
                        return field_122_brain_state_result;
                    }
                    field_106_current_motion = eSlogMotions::M_MoveHeadUpwards_5_4C5F20;
                    return field_122_brain_state_result;
                }

                field_108_next_motion = eSlogMotions::M_Walk_1_4C60C0;
                return 12;
            }

            field_108_next_motion = eSlogMotions::M_Run_2_4C6340;
            return field_122_brain_state_result;
        }

        field_108_next_motion = eSlogMotions::M_TurnAround_3_4C65C0;
        return field_122_brain_state_result;
    }

    field_15C_bone_id = -1;
    return 2;
}

s16 Slog::Brain_ChasingAbe_State_20_Collided(BaseAliveGameObject* pTarget)
{
    auto pBone = FindBone_4C25B0();
    if (pBone)
    {
        field_15C_bone_id = pBone->field_8_object_id;
        return 11;
    }

    if (field_106_current_motion != eSlogMotions::M_Idle_0_4C5F90)
    {
        return field_122_brain_state_result;
    }

    if (field_160_flags.Get(Flags_160::eBit1_StopRunning))
    {
        if (pTarget->field_B8_xpos > field_B8_xpos)
        {
            return 2;
        }
    }
    else
    {
        if (pTarget->field_B8_xpos < field_B8_xpos)
        {
            return 2;
        }
    }

    if (static_cast<s32>(sGnFrame_5C1B84) > field_150_growl_timer)
    {
        field_150_growl_timer = Math_NextRandom() % 32 + sGnFrame_5C1B84 + 60;
        field_106_current_motion = eSlogMotions::M_Growl_23_4C7170;
        field_108_next_motion = eSlogMotions::M_Idle_0_4C5F90;
    }

    if (static_cast<s32>(sGnFrame_5C1B84) <= field_14C_scratch_timer)
    {
        return field_122_brain_state_result;
    }

    field_14C_scratch_timer = Math_NextRandom() % 32 + sGnFrame_5C1B84 + 120;
    field_106_current_motion = eSlogMotions::M_Scratch_22_4C7120;
    field_108_next_motion = eSlogMotions::M_Idle_0_4C5F90;
    return field_122_brain_state_result;
}

s16 Slog::Brain_ChasingAbe_State_10_HungryForBone()
{
    if (field_106_current_motion == eSlogMotions::M_Idle_0_4C5F90)
    {
        field_108_next_motion = eSlogMotions::M_JumpUpwards_19_4C7470;
    }

    if (static_cast<s32>(sGnFrame_5C1B84) <= field_124_timer)
    {
        auto pBone = FindBone_4C25B0();
        if (pBone)
        {
            field_108_next_motion = eSlogMotions::M_Run_2_4C6340;
            field_15C_bone_id = pBone->field_8_object_id;
            return 11;
        }

        if (field_106_current_motion == eSlogMotions::M_StopRunning_6_4C66C0)
        {
            return field_122_brain_state_result;
        }

        if (field_106_current_motion != eSlogMotions::M_Idle_0_4C5F90)
        {
            return field_122_brain_state_result;
        }

        if (Slog_NextRandom() % 16)
        {
            return field_122_brain_state_result;
        }

        field_108_next_motion = eSlogMotions::M_MoveHeadUpwards_5_4C5F20;
        return field_122_brain_state_result;
    }

    field_160_flags.Clear(Flags_160::eBit4_Hungry);
    field_108_next_motion = eSlogMotions::M_Run_2_4C6340;
    return 2;
}

s16 Slog::Brain_ChasingAbe_State_9_Falling()
{
    if (field_106_current_motion != eSlogMotions::M_Idle_0_4C5F90)
    {
        return field_122_brain_state_result;
    }
    field_106_current_motion = eSlogMotions::M_Run_2_4C6340;
    return 2;
}

s16 Slog::Brain_ChasingAbe_State_8_ToIdle()
{
    if (field_106_current_motion != eSlogMotions::M_Idle_0_4C5F90)
    {
        return field_122_brain_state_result;
    }
    field_142_anger_level = 0;
    field_120_brain_state_idx = 1;
    field_11C_biting_target = 0;
    return 0;
}

s16 Slog::Brain_ChasingAbe_State_7_EatingTarget(BaseAliveGameObject* pTarget)
{
    if (static_cast<s32>(sGnFrame_5C1B84) <= field_124_timer && pTarget->field_20_animation.field_4_flags.Get(AnimFlags::eBit3_Render))
    {
        if (field_106_current_motion != eSlogMotions::M_Idle_0_4C5F90)
        {
            return field_122_brain_state_result;
        }

        field_106_current_motion = eSlogMotions::M_Eating_20_4C75F0;
        return field_122_brain_state_result;
    }

    field_108_next_motion = eSlogMotions::M_Idle_0_4C5F90;
    return 8;
}

s16 Slog::Brain_ChasingAbe_State_4_LungingAtTarget(BaseAliveGameObject* pTarget)
{
    if (field_106_current_motion == eSlogMotions::M_Run_2_4C6340)
    {
        if (pTarget->field_10C_health > FP_FromInteger(0))
        {
            field_124_timer = Math_RandomRange_496AB0(1, 3) + sGnFrame_5C1B84 + field_158_chase_delay;
            return 1;
        }

        if (FP_Abs(pTarget->field_B8_xpos - field_B8_xpos) > ScaleToGridSize_4498B0(field_CC_sprite_scale) * FP_FromInteger(2))
        {
            field_124_timer = Math_RandomRange_496AB0(1, 3) + sGnFrame_5C1B84 + field_158_chase_delay;
            return 1;
        }

        if (FP_Abs(pTarget->field_BC_ypos - field_BC_ypos) > ScaleToGridSize_4498B0(field_CC_sprite_scale) * FP_FromInteger(2))
        {
            field_124_timer = Math_RandomRange_496AB0(1, 3) + sGnFrame_5C1B84 + field_158_chase_delay;
            return 1;
        }

        if (vIsFacingMe_4254A0(pTarget))
        {
            field_106_current_motion = eSlogMotions::M_Eating_20_4C75F0;
            field_108_next_motion = -1;
        }
        else
        {
            field_106_current_motion = eSlogMotions::M_TurnAround_3_4C65C0;
            field_108_next_motion = eSlogMotions::M_Eating_20_4C75F0;
        }

        field_124_timer = sGnFrame_5C1B84 + 90;
        return 7;
    }

    if (field_106_current_motion != eSlogMotions::M_Fall_4_4C6930)
    {
        return field_122_brain_state_result;
    }

    return 9;
}

s16 Slog::Brain_ChasingAbe_State_3_GrowlOrScratch(BaseAliveGameObject* pTarget)
{
    if (field_106_current_motion != eSlogMotions::M_Idle_0_4C5F90)
    {
        field_108_next_motion = eSlogMotions::M_Idle_0_4C5F90;
        return field_122_brain_state_result;
    }

    if (Slog_NextRandom() % 64)
    {
        if (static_cast<s32>(sGnFrame_5C1B84) > field_150_growl_timer)
        {
            field_150_growl_timer = Math_NextRandom() % 32 + sGnFrame_5C1B84 + 60;
            field_106_current_motion = eSlogMotions::M_Growl_23_4C7170;
            field_108_next_motion = eSlogMotions::M_Idle_0_4C5F90;
        }

        if (static_cast<s32>(sGnFrame_5C1B84) > field_14C_scratch_timer)
        {
            field_14C_scratch_timer = Math_NextRandom() % 32 + sGnFrame_5C1B84 + 120;
            field_106_current_motion = eSlogMotions::M_Scratch_22_4C7120;
            field_108_next_motion = eSlogMotions::M_Idle_0_4C5F90;
        }

        if (pTarget->field_CC_sprite_scale != FP_FromInteger(1))
        {
            return field_122_brain_state_result;
        }
        return 2;
    }

    field_106_current_motion = eSlogMotions::M_MoveHeadUpwards_5_4C5F20;
    return field_122_brain_state_result;
}

s16 Slog::Brain_ChasingAbe_State_2_Thinking(BaseAliveGameObject* pTarget)
{
    if (field_C4_velx > FP_FromInteger(0) && HandleEnemyStopper_4C5340())
    {
        field_108_next_motion = eSlogMotions::M_StopRunning_6_4C66C0;
        field_160_flags.Set(Flags_160::eBit1_StopRunning, field_C4_velx < FP_FromInteger(0));
        field_14C_scratch_timer = Math_NextRandom() % 32 + sGnFrame_5C1B84 + 120;
        field_150_growl_timer = Math_NextRandom() % 32 + sGnFrame_5C1B84 + 60;
        return 20;
    }

    if (CollisionCheck_4C5480(field_CC_sprite_scale * FP_FromInteger(20), field_C4_velx * FP_FromInteger(4)))
    {
        field_108_next_motion = eSlogMotions::M_StopRunning_6_4C66C0;
        field_160_flags.Set(Flags_160::eBit1_StopRunning, field_C4_velx < FP_FromInteger(0));
        field_14C_scratch_timer = Math_NextRandom() % 32 + sGnFrame_5C1B84 + 120;
        field_150_growl_timer = Math_NextRandom() % 32 + sGnFrame_5C1B84 + 60;
        return 19;
    }

    if (!vIsFacingMe_4254A0(pTarget) && field_106_current_motion == eSlogMotions::M_Run_2_4C6340)
    {
        field_108_next_motion = eSlogMotions::M_SlideTurn_7_4C6790;
    }

    if (vIsObjNearby_4253B0(ScaleToGridSize_4498B0(field_CC_sprite_scale) * FP_FromInteger(3), pTarget))
    {
        if (vOnSameYLevel_425520(pTarget))
        {
            if (vIsFacingMe_4254A0(pTarget))
            {
                if (!CollisionCheck_4C5480(field_CC_sprite_scale * FP_FromInteger(20), pTarget->field_B8_xpos - field_B8_xpos) && !field_160_flags.Get(Flags_160::eBit9_MovedOffScreen))
                {
                    if (pTarget->field_10C_health <= FP_FromInteger(0))
                    {
                        field_108_next_motion = eSlogMotions::M_Idle_0_4C5F90;
                    }
                    else
                    {
                        field_108_next_motion = eSlogMotions::M_JumpForwards_18_4C7210;
                    }
                }
            }
        }
    }

    if (field_106_current_motion == eSlogMotions::M_SlideTurn_7_4C6790)
    {
        field_108_next_motion = eSlogMotions::M_Run_2_4C6340;
    }

    auto pBone = FindBone_4C25B0();
    if (pBone)
    {
        field_15C_bone_id = pBone->field_8_object_id;
        return 11;
    }

    if (field_106_current_motion == eSlogMotions::M_Idle_0_4C5F90)
    {
        if (vIsFacingMe_4254A0(pTarget))
        {
            FP gridSizeDirected = {};
            if (field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX))
            {
                gridSizeDirected = -ScaleToGridSize_4498B0(field_CC_sprite_scale);
            }
            else
            {
                gridSizeDirected = ScaleToGridSize_4498B0(field_CC_sprite_scale);
            }

            if (!CollisionCheck_4C5480(field_CC_sprite_scale * FP_FromInteger(20), FP_FromInteger(2) * gridSizeDirected))
            {
                if (pTarget->field_10C_health > FP_FromInteger(0))
                {
                    field_124_timer = Math_RandomRange_496AB0(1, 3) + sGnFrame_5C1B84 + field_158_chase_delay;
                    return 1;
                }

                if (FP_Abs(pTarget->field_B8_xpos - field_B8_xpos) > (ScaleToGridSize_4498B0(field_CC_sprite_scale) * FP_FromInteger(2)))
                {
                    field_124_timer = Math_RandomRange_496AB0(1, 3) + sGnFrame_5C1B84 + field_158_chase_delay;
                    return 1;
                }

                if (FP_Abs(pTarget->field_BC_ypos - field_BC_ypos) > (ScaleToGridSize_4498B0(field_CC_sprite_scale) * FP_FromInteger(2)))
                {
                    field_124_timer = Math_RandomRange_496AB0(1, 3) + sGnFrame_5C1B84 + field_158_chase_delay;
                    return 1;
                }
                field_124_timer = sGnFrame_5C1B84 + 90;
                return 7;
            }

            field_160_flags.Set(Flags_160::eBit1_StopRunning, field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX));

            field_14C_scratch_timer = Math_NextRandom() % 32 + sGnFrame_5C1B84 + 120;
            field_150_growl_timer = Math_NextRandom() % 32 + sGnFrame_5C1B84 + 60;
            return 19;
        }
        field_106_current_motion = eSlogMotions::M_TurnAround_3_4C65C0;
    }

    if (field_160_flags.Get(Flags_160::eBit4_Hungry) && pTarget == sActiveHero_5C1B68 && pTarget->field_D6_scale == field_D6_scale && (sActiveHero_5C1B68->field_106_current_motion == eAbeMotions::Motion_104_RockThrowStandingHold_455DF0 || sActiveHero_5C1B68->field_106_current_motion == eAbeMotions::Motion_107_RockThrowCrouchingHold_454410))
    {
        field_108_next_motion = eSlogMotions::M_StopRunning_6_4C66C0;
        field_124_timer = sGnFrame_5C1B84 + 90;
        return 10;
    }

    if (field_106_current_motion == eSlogMotions::M_Fall_4_4C6930)
    {
        return 9;
    }

    if (field_106_current_motion == eSlogMotions::M_JumpForwards_18_4C7210)
    {
        return 4;
    }

    if (field_BC_ypos <= pTarget->field_BC_ypos + FP_FromInteger(50))
    {
        if (pTarget->field_CC_sprite_scale != FP_FromDouble(0.5))
        {
            return field_122_brain_state_result;
        }
        field_150_growl_timer = Math_NextRandom() % 32 + sGnFrame_5C1B84 + 60;
        field_14C_scratch_timer = Math_NextRandom() % 32 + sGnFrame_5C1B84 + 120;
        return 3;
    }

    return 15;
}

s16 Slog::Brain_ChasingAbe_State_1_Waiting()
{
    if (field_124_timer > static_cast<s32>(sGnFrame_5C1B84))
    {
        return field_122_brain_state_result;
    }
    field_108_next_motion = eSlogMotions::M_Run_2_4C6340;
    return 2;
}

s16 Slog::Brain_ChasingAbe_State_0_Init()
{
    field_11C_biting_target = 0;
    field_15A_jump_counter = 0;
    field_15C_bone_id = -1;
    field_124_timer = Math_RandomRange_496AB0(1, 3) + sGnFrame_5C1B84 + field_158_chase_delay;
    Sfx_4C7D30(SlogSound::AttackGrowl_8);
    return 1;
}

s16 Slog::Brain_Death_3_4C3250()
{
    field_138_listening_to_slig_id = -1;
    field_118_target_id = -1;

    if (field_124_timer < static_cast<s32>(sGnFrame_5C1B84 + 80))
    {
        field_CC_sprite_scale -= FP_FromDouble(0.023);
        field_D0_r -= 2;
        field_D2_g -= 2;
        field_D4_b -= 2;
    }

    if (static_cast<s32>(sGnFrame_5C1B84) < field_124_timer - 24)
    {
        DeathSmokeEffect(true);
    }

    if (field_CC_sprite_scale < FP_FromInteger(0))
    {
        field_6_flags.Set(BaseGameObject::eDead_Bit3);
    }

    return 100;
}

u8** Slog::ResBlockForMotion_4C4A80(s16 motion)
{
    if (motion < eSlogMotions::M_AngryBark_14_4C6CF0)
    {
        field_130_motion_resource_block_index = 0;
        return field_10_resources_array.ItemAt(field_130_motion_resource_block_index);
    }

    if (motion < eSlogMotions::M_JumpForwards_18_4C7210)
    {
        field_130_motion_resource_block_index = 1;
        return field_10_resources_array.ItemAt(field_130_motion_resource_block_index);
    }

    if (motion < eSlogMotions::M_Dying_21_4C77F0)
    {
        field_130_motion_resource_block_index = 2;
        return field_10_resources_array.ItemAt(field_130_motion_resource_block_index);
    }

    if (motion < eSlogMotions::M_Scratch_22_4C7120)
    {
        field_130_motion_resource_block_index = 3;
        return field_10_resources_array.ItemAt(field_130_motion_resource_block_index);
    }

    if (motion < 24) // last + 1
    {
        field_130_motion_resource_block_index = 4;
        return field_10_resources_array.ItemAt(field_130_motion_resource_block_index);
    }
    else
    {
        field_130_motion_resource_block_index = 0;
        return field_10_resources_array.ItemAt(field_130_motion_resource_block_index);
    }
}

void Slog::SetAnimFrame_4C42A0()
{
    const AnimRecord& animRec = AnimRec(sSlogFrameOffsetTable_5609D8[field_106_current_motion]);
    u8** ppRes = ResBlockForMotion_4C4A80(field_106_current_motion);
    field_20_animation.Set_Animation_Data_409C80(animRec.mFrameTableOffset, ppRes);
}

const TintEntry sSlogTints_560A48[] = {
    {LevelIds_s8::eMines_1, 127u, 127u, 127u},
    {LevelIds_s8::eNecrum_2, 127u, 127u, 127u},
    {LevelIds_s8::eMudomoVault_3, 127u, 127u, 127u},
    {LevelIds_s8::eMudancheeVault_4, 127u, 127u, 127u},
    {LevelIds_s8::eFeeCoDepot_5, 127u, 127u, 127u},
    {LevelIds_s8::eBarracks_6, 127u, 127u, 127u},
    {LevelIds_s8::eMudancheeVault_Ender_7, 127u, 127u, 127u},
    {LevelIds_s8::eBonewerkz_8, 127u, 127u, 127u},
    {LevelIds_s8::eBrewery_9, 127u, 127u, 127u},
    {LevelIds_s8::eBrewery_Ender_10, 127u, 127u, 127u},
    {LevelIds_s8::eMudomoVault_Ender_11, 127u, 127u, 127u},
    {LevelIds_s8::eFeeCoDepot_Ender_12, 127u, 127u, 127u},
    {LevelIds_s8::eBarracks_Ender_13, 127u, 127u, 127u},
    {LevelIds_s8::eBonewerkz_Ender_14, 127u, 127u, 127u},
    {LevelIds_s8::eNone, 127u, 127u, 127u}};

void Slog::Init_4C46A0()
{
    SetType(AETypes::eSlog_126);
    const AnimRecord& rec = AnimRec(AnimId::Slog_Idle);
    field_10_resources_array.SetAt(0, ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, rec.mResourceId, 1, 0));
    field_10_resources_array.SetAt(1, ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, AEResourceID::kDogrstnResID, 1, 0));
    field_10_resources_array.SetAt(2, ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, AEResourceID::kDogattkResID, 1, 0));
    field_10_resources_array.SetAt(3, ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, AEResourceID::kDogknfdResID, 1, 0));
    field_10_resources_array.SetAt(4, ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, AEResourceID::kDogidleResID, 1, 0));
    Animation_Init_424E10(rec.mFrameTableOffset, rec.mMaxW, rec.mMaxH, field_10_resources_array.ItemAt(0), 1, 1);

    field_114_flags.Set(Flags_114::e114_Bit6_SetOffExplosives);

    field_160_flags.Clear(Flags_160::eBit3_Shot);
    field_160_flags.Clear(Flags_160::eBit6_HitByAbilityRing);
    field_160_flags.Set(Flags_160::eBit4_Hungry);

    field_DC_bApplyShadows |= 2u;
    field_20_animation.field_1C_fn_ptr_array = kSlog_Anim_Frame_Fns_55EFBC;
    field_124_timer = 0;
    field_122_brain_state_result = 0;
    field_108_next_motion = -1;
    field_130_motion_resource_block_index = 0;
    field_110_id = -1;
    field_138_listening_to_slig_id = -1;
    field_118_target_id = -1;
    field_15C_bone_id = -1;
    SetTint_425600(&sSlogTints_560A48[0], gMap_5C3030.field_0_current_level);
    field_20_animation.field_C_render_layer = Layer::eLayer_SlogFleech_34;

    if (field_CC_sprite_scale == FP_FromInteger(1))
    {
        field_D8_yOffset = 1;
    }
    else
    {
        field_D8_yOffset = 2;
    }

    field_D6_scale = 1;

    FP hitX = {};
    FP hitY = {};
    if (sCollisions_DArray_5C1128->Raycast_417A60(
            field_B8_xpos, field_BC_ypos,
            field_B8_xpos, field_BC_ypos + FP_FromInteger(24),
            &field_100_pCollisionLine, &hitX, &hitY, 1)
        == 1)
    {
        field_BC_ypos = hitY;
        if (field_100_pCollisionLine->field_8_type == eLineTypes::eUnknown_32)
        {
            PSX_RECT bRect = {};
            vGetBoundingRect_424FD0(&bRect, 1);
            const PSX_Point xy = {bRect.x, static_cast<s16>(bRect.y + 5)};
            const PSX_Point wh = {bRect.w, static_cast<s16>(bRect.h + 5)};
            vOnCollisionWith_424EE0(xy, wh, ObjList_5C1B78, 1, (TCollisionCallBack) &BaseAliveGameObject::OnTrapDoorIntersection_408BA0);
        }
    }

    MapFollowMe_408D10(FALSE);
    field_E0_pShadow = ae_new<Shadow>();
    if (field_E0_pShadow)
    {
        field_E0_pShadow->ctor_4AC990();
    }

    sSlogCount_BAF7F2++;

    vStackOnObjectsOfType_425840(AETypes::eSlog_126);
}

void Slog::vUpdate_4C50D0()
{
    if (field_114_flags.Get(Flags_114::e114_Bit9_RestoredFromQuickSave))
    {
        field_114_flags.Clear(Flags_114::e114_Bit9_RestoredFromQuickSave);
        if (field_104_collision_line_type == -1)
        {
            field_100_pCollisionLine = nullptr;
        }
        else
        {
            sCollisions_DArray_5C1128->Raycast_417A60(
                field_B8_xpos,
                field_BC_ypos - FP_FromInteger(20),
                field_B8_xpos,
                field_BC_ypos + FP_FromInteger(20),
                &field_100_pCollisionLine,
                &field_B8_xpos,
                &field_BC_ypos,
                1 << field_104_collision_line_type);
        }
        field_104_collision_line_type = 0;
        field_118_target_id = BaseGameObject::Find_Flags_4DC170(field_118_target_id);
        field_138_listening_to_slig_id = BaseGameObject::Find_Flags_4DC170(field_138_listening_to_slig_id);
        field_15C_bone_id = BaseGameObject::Find_Flags_4DC170(field_15C_bone_id);
    }

    if (Event_Get_422C00(kEventDeathReset))
    {
        field_6_flags.Set(BaseGameObject::eDead_Bit3);
    }

    if (FP_Abs(field_B8_xpos - sControlledCharacter_5C1B8C->field_B8_xpos) > FP_FromInteger(750) || FP_Abs(field_BC_ypos - sControlledCharacter_5C1B8C->field_BC_ypos) > FP_FromInteger(390))
    {
        field_20_animation.field_4_flags.Clear(AnimFlags::eBit2_Animate);
        field_20_animation.field_4_flags.Clear(AnimFlags::eBit3_Render);
    }
    else
    {
        if (field_10C_health > FP_FromInteger(0))
        {
            field_20_animation.field_4_flags.Set(AnimFlags::eBit2_Animate);
            field_20_animation.field_4_flags.Set(AnimFlags::eBit3_Render);
        }

        const auto oldMotion = field_106_current_motion;
        const auto oldBrain = sSlog_fns_brain_560A38[field_120_brain_state_idx];
        field_122_brain_state_result = (this->*sSlog_fns_brain_560A38[field_120_brain_state_idx])();
        if (sDDCheat_ShowAI_Info_5C1BD8)
        {
            DDCheat::DebugStr_4F5560("Slog:  Motion=%d  BrainState=%d\n", field_106_current_motion, field_122_brain_state_result);
        }

        const FP oldXPos = field_B8_xpos;
        const FP oldYPos = field_BC_ypos;

        (this->*sSlog_motion_table_560978[field_106_current_motion])();

        if (oldXPos != field_B8_xpos || oldYPos != field_BC_ypos)
        {
            field_FC_pPathTLV = sPath_dword_BB47C0->TLV_Get_At_4DB290(
                nullptr,
                field_B8_xpos,
                field_BC_ypos,
                field_B8_xpos,
                field_BC_ypos);
            VOn_TLV_Collision_4087F0(field_FC_pPathTLV);
        }

        // TODO: This is extra debug logging to figure out the motion names
        if (oldBrain != sSlog_fns_brain_560A38[field_120_brain_state_idx])
        {
            LOG_INFO("Slog: Old brain = " << GetOriginalFn(oldBrain, sSlogAiTable).fnName << " new brain = " << GetOriginalFn(sSlog_fns_brain_560A38[field_120_brain_state_idx], sSlogAiTable).fnName);

            //LOG_INFO("Slog: Old motion = " << oldMotion << " new motion = " << field_106_current_motion);
        }

        if (oldMotion != field_106_current_motion)
        {
            SetAnimFrame_4C42A0();
        }
    }
}

void Slog::dtor_4C49A0()
{
    SetVTable(this, 0x547578);

    field_118_target_id = -1;
    field_138_listening_to_slig_id = -1;
    field_15C_bone_id = -1;

    if (field_12C_tlvInfo != 0xFFFF)
    {
        Path::TLV_Reset_4DB8E0(field_12C_tlvInfo, -1, 0, field_10C_health <= FP_FromInteger(0));
    }

    MusicController::PlayMusic_47FD60(MusicController::MusicTypes::eNone_0, this, 0, 0);

    if (!field_160_flags.Get(Flags_160::eBit3_Shot))
    {
        sSlogCount_BAF7F2--;
    }

    dtor_4080B0();
}

Slog* Slog::vdtor_4C4510(s32 flags)
{
    dtor_4C49A0();
    if (flags & 1)
    {
        ae_delete_free_495540(this);
    }
    return this;
}

void Slog::ToIdle_4C5C10()
{
    MapFollowMe_408D10(FALSE);
    field_128_falling_velx_scale_factor = FP_FromInteger(0);
    field_C4_velx = FP_FromInteger(0);
    field_C8_vely = FP_FromInteger(0);
    field_106_current_motion = 0;
    field_108_next_motion = -1;
}

const SfxDefinition getSfxDef(SlogSound effectId)
{
    return sSlogSFXList_560B00[static_cast<s32>(effectId)];
}

void Slog::Sfx_4C7D30(SlogSound effectId)
{
    s16 volumeLeft = 0;
    s16 volumeRight = 0;
    const SfxDefinition effectDef = getSfxDef(effectId);


    const CameraPos direction = gMap_5C3030.GetDirection_4811A0(
        field_C2_lvl_number,
        field_C0_path_number,
        field_B8_xpos,
        field_BC_ypos);
    PSX_RECT pRect = {};
    gMap_5C3030.Get_Camera_World_Rect_481410(direction, &pRect);

    const s16 defaultSndIdxVol = effectDef.field_3_default_volume;
    volumeRight = defaultSndIdxVol;
    switch (direction)
    {
        case CameraPos::eCamCurrent_0:
            volumeLeft = volumeRight;
            break;
        case CameraPos::eCamTop_1:
        case CameraPos::eCamBottom_2:
        {
            volumeLeft = FP_GetExponent(FP_FromInteger(defaultSndIdxVol * 2) / FP_FromInteger(3));
            volumeRight = volumeLeft;
        }
        break;
        case CameraPos::eCamLeft_3:
        {
            const FP percentHowFar = (FP_FromInteger(pRect.w) - field_B8_xpos) / FP_FromInteger(368);
            volumeLeft = volumeRight - FP_GetExponent(percentHowFar * FP_FromInteger(volumeRight - (volumeRight / 3)));
            volumeRight = volumeRight - FP_GetExponent(percentHowFar * FP_FromInteger(volumeRight));
        }
        break;
        case CameraPos::eCamRight_4:
        {
            const FP percentHowFar = (field_B8_xpos - FP_FromInteger(pRect.x)) / FP_FromInteger(368);
            volumeLeft = volumeRight - FP_GetExponent(percentHowFar * FP_FromInteger(volumeRight));
            volumeRight = volumeRight - FP_GetExponent(percentHowFar * FP_FromInteger(volumeRight - (volumeRight / 3)));
        }
        break;
        default:
            return;
    }

    if (field_CC_sprite_scale == FP_FromDouble(0.5))
    {
        SFX_SfxDefinition_Play_4CA700(
            &effectDef,
            volumeLeft,
            volumeRight,
            effectDef.field_4_pitch_min + 1524,
            effectDef.field_6_pitch_max + 1524);
    }
    else
    {
        SFX_SfxDefinition_Play_4CA700(
            &effectDef,
            volumeLeft,
            volumeRight,
            effectDef.field_4_pitch_min,
            effectDef.field_6_pitch_max);
    }
}

void Slog::ToJump_4C5C60()
{
    field_C4_velx = (field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX) ? FP_FromDouble(-10.18) : FP_FromDouble(10.18)) * field_CC_sprite_scale;
    field_C8_vely = FP_FromInteger(-8) * field_CC_sprite_scale;

    field_F8_LastLineYPos = field_BC_ypos;

    VOnTrapDoorOpen();

    field_106_current_motion = eSlogMotions::M_JumpForwards_18_4C7210;
    field_100_pCollisionLine = nullptr;

    Sfx_4C7D30(SlogSound::AttackGrowl_8);

    if (gMap_5C3030.GetDirection_4811A0(field_C2_lvl_number, field_C0_path_number, field_B8_xpos, field_BC_ypos) >= CameraPos::eCamCurrent_0)
    {
        MusicController::PlayMusic_47FD60(MusicController::MusicTypes::eIntenseChase_7, this, 0, 0);
    }
}

s16 Slog::ToNextMotion_4C5A30()
{
    switch (field_108_next_motion)
    {
        case eSlogMotions::M_TurnAround_3_4C65C0:
            field_106_current_motion = eSlogMotions::M_TurnAround_3_4C65C0;
            field_108_next_motion = -1;
            return 1;

        case eSlogMotions::M_Walk_1_4C60C0:
            if (field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX))
            {
                field_C4_velx = -(ScaleToGridSize_4498B0(field_CC_sprite_scale) / FP_FromInteger(9));
            }
            else
            {
                field_C4_velx = (ScaleToGridSize_4498B0(field_CC_sprite_scale) / FP_FromInteger(9));
            }

            if (!CollisionCheck_4C5480(field_CC_sprite_scale * FP_FromInteger(20), (field_C4_velx * FP_FromInteger(9)) * FP_FromInteger(2)))
            {
                field_106_current_motion = eSlogMotions::M_StartWalking_8_4C62E0;
                field_108_next_motion = -1;
                return 1;
            }
            ToIdle_4C5C10();
            return 0;

        case eSlogMotions::M_Run_2_4C6340:
            if (field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX))
            {
                field_C4_velx = -(ScaleToGridSize_4498B0(field_CC_sprite_scale) / FP_FromInteger(3));
            }
            else
            {
                field_C4_velx = (ScaleToGridSize_4498B0(field_CC_sprite_scale) / FP_FromInteger(3));
            }

            if (!CollisionCheck_4C5480(field_CC_sprite_scale * FP_FromInteger(20), field_C4_velx * FP_FromInteger(4)))
            {
                field_106_current_motion = eSlogMotions::M_Run_2_4C6340;
                field_108_next_motion = -1;
                return 1;
            }
            ToIdle_4C5C10();
            return 0;
    }
    return 0;
}

Bool32 Slog::CollisionCheck_4C5480(FP hitY, FP hitX)
{
    PathLine* pLine = nullptr;
    return sCollisions_DArray_5C1128->Raycast_417A60(field_B8_xpos, field_BC_ypos - hitY, field_B8_xpos + hitX, field_BC_ypos - hitY, &pLine, &hitX, &hitY, 6) != 0;
}

void Slog::MoveOnLine_4C5DA0()
{
    auto pPlatform = static_cast<PlatformBase*>(sObjectIds_5C1B70.Find_449CF0(field_110_id));
    const FP oldXPos = field_B8_xpos;

    if (field_100_pCollisionLine)
    {
        field_100_pCollisionLine = field_100_pCollisionLine->MoveOnLine_418260(&field_B8_xpos, &field_BC_ypos, field_C4_velx);
        if (field_100_pCollisionLine)
        {
            if (pPlatform)
            {
                if (field_100_pCollisionLine->field_8_type != 32 && field_100_pCollisionLine->field_8_type != 36)
                {
                    pPlatform->VRemove(this);
                    field_110_id = -1;
                }
            }
            else if (field_100_pCollisionLine->field_8_type == eLineTypes::eUnknown_32 || field_100_pCollisionLine->field_8_type == eLineTypes::eUnknown_36)
            {
                PSX_RECT bRect = {};
                vGetBoundingRect_424FD0(&bRect, 1);
                const PSX_Point xy = {bRect.x, static_cast<s16>(bRect.y + 5)};
                const PSX_Point wh = {bRect.w, static_cast<s16>(bRect.h + 5)};
                vOnCollisionWith_424EE0(xy, wh, ObjList_5C1B78, 1, (TCollisionCallBack) &BaseAliveGameObject::OnTrapDoorIntersection_408BA0);
            }
        }
        else
        {
            VOnTrapDoorOpen();
            field_128_falling_velx_scale_factor = FP_FromDouble(0.3);
            field_F8_LastLineYPos = field_BC_ypos;
            field_B8_xpos = oldXPos + field_C4_velx;
        }
    }
    else
    {
        field_128_falling_velx_scale_factor = FP_FromDouble(0.3);
        field_F8_LastLineYPos = field_BC_ypos;
        field_106_current_motion = eSlogMotions::M_Fall_4_4C6930;
    }
}

Bone* Slog::FindBone_4C25B0()
{
    for (s32 i = 0; i < gBaseGameObject_list_BB47C4->Size(); i++)
    {
        auto pObj = gBaseGameObject_list_BB47C4->ItemAt(i);
        if (!pObj)
        {
            break;
        }

        if (pObj->Type() == AETypes::eBone_11)
        {
            auto pBone = static_cast<Bone*>(pObj);
            if (pBone->VCanThrow_49E350())
            {
                if (gMap_5C3030.Is_Point_In_Current_Camera_4810D0(pBone->field_C2_lvl_number, pBone->field_C0_path_number, pBone->field_B8_xpos, pBone->field_BC_ypos, 0) && pBone->field_D6_scale == field_D6_scale)
                {
                    if (FP_Abs(field_BC_ypos - pBone->field_BC_ypos) <= FP_FromInteger(50) || pBone->VCanBeEaten_411560())
                    {
                        return pBone;
                    }
                }
            }
        }
    }
    return nullptr;
}

BaseAliveGameObject* Slog::FindTarget_4C33C0(s16 bKillSligs, s16 bLookingUp)
{
    PSX_RECT bRect = {};
    vGetBoundingRect_424FD0(&bRect, 1);

    if (field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX))
    {
        bRect.x -= 368;
    }
    else
    {
        bRect.w += 368;
    }

    if (bLookingUp)
    {
        bRect.x -= 368;
        bRect.w += 368;
        bRect.y -= gPsxDisplay_5C1130.field_2_height;
        bRect.h += gPsxDisplay_5C1130.field_2_height;
    }

    FP distanceToLastFoundObj = FP_FromInteger(gPsxDisplay_5C1130.field_0_width);
    auto pSligBeingListendTo = static_cast<BaseAliveGameObject*>(sObjectIds_5C1B70.Find_449CF0(field_138_listening_to_slig_id));

    BaseAliveGameObject* pBestObj = nullptr;
    BaseAliveGameObject* pLastFoundObj = nullptr;

    s32 array_idx = 0;
    s32 local_array[10] = {};

    for (s32 i = 0; i < gBaseAliveGameObjects_5C1B7C->Size(); i++)
    {
        BaseAliveGameObject* pObj = gBaseAliveGameObjects_5C1B7C->ItemAt(i);
        if (!pObj)
        {
            break;
        }

        if (pObj->Type() == AETypes::eSlog_126)
        {
            auto pSlog = static_cast<Slog*>(pObj);
            if (pSlog->field_118_target_id != -1 && array_idx < ALIVE_COUNTOF(local_array))
            {
                local_array[array_idx++] = pSlog->field_118_target_id;
            }
        }

        if (pObj != pSligBeingListendTo && pObj != this && pObj->field_D6_scale == field_D6_scale)
        {
            switch (pObj->Type())
            {
                case AETypes::eCrawlingSlig_26:
                case AETypes::eFlyingSlig_54:
                case AETypes::eGlukkon_67:
                case AETypes::eAbe_69:
                case AETypes::eMudokon_110:
                case AETypes::eSlig_125:
                    if (bKillSligs || (!bKillSligs && (pObj->Type() == AETypes::eAbe_69 || pObj->Type() == AETypes::eCrawlingSlig_26 || pObj->Type() == AETypes::eFlyingSlig_54 || pObj->Type() == AETypes::eGlukkon_67 || (pObj->Type() == AETypes::eMudokon_110 && static_cast<Mudokon*>(pObj)->field_18E_brain_state == Mud_Brain_State::Brain_4_ListeningToAbe_477B40))))
                    {
                        PSX_RECT objRect = {};
                        pObj->vGetBoundingRect_424FD0(&objRect, 1);

                        if (objRect.x <= bRect.w && objRect.w >= bRect.x && objRect.h >= bRect.y && objRect.y <= bRect.h)
                        {
                            pLastFoundObj = pObj;

                            const FP xDelta = FP_Abs(field_B8_xpos - pObj->field_B8_xpos);
                            if (xDelta < distanceToLastFoundObj)
                            {
                                s32 array_idx_iter = 0;
                                if (array_idx)
                                {
                                    // Something to do with finding who is the last attacker
                                    while (local_array[array_idx_iter] != pObj->field_8_object_id)
                                    {
                                        if (++array_idx_iter >= array_idx)
                                        {
                                            distanceToLastFoundObj = xDelta;
                                            pBestObj = pObj;
                                            break;
                                        }
                                    }
                                }
                                else
                                {
                                    distanceToLastFoundObj = xDelta;
                                    pBestObj = pObj;
                                }
                            }
                        }
                    }
                    break;

                default:
                    break;
            }
        }
    }

    if (pBestObj)
    {
        return pBestObj;
    }

    if (pLastFoundObj)
    {
        return pLastFoundObj;
    }

    return nullptr;
}

void Slog::vOn_TrapDoor_Open_4C5D50()
{
    auto pPlatform = static_cast<PlatformBase*>(sObjectIds_5C1B70.Find_449CF0(field_110_id));
    if (pPlatform)
    {
        pPlatform->VRemove(this);
        field_110_id = -1;
        field_106_current_motion = eSlogMotions::M_Fall_4_4C6930;
    }
}

void Slog::vOn_Tlv_Collision_4C5060(Path_TLV* pTlv)
{
    while (pTlv)
    {
        if (pTlv->field_4_type == TlvTypes::DeathDrop_4)
        {
            field_10C_health = FP_FromInteger(0);
            field_6_flags.Set(BaseGameObject::eDead_Bit3);
        }
        pTlv = sPath_dword_BB47C0->TLV_Get_At_4DB290(pTlv, field_B8_xpos, field_BC_ypos, field_B8_xpos, field_BC_ypos);
    }
}

s16 Slog::vTakeDamage_4C4B80(BaseGameObject* pFrom)
{
    if (field_10C_health <= FP_FromInteger(0))
    {
        return 0;
    }

    switch (pFrom->Type())
    {
        case AETypes::eBullet_15:
        {
            auto pBullet = static_cast<Bullet*>(pFrom);
            switch (pBullet->field_20_type)
            {
                case BulletType::eSligPossessedOrUnderGlukkonCommand_0:
                case BulletType::eNormalBullet_2:
                    if (pBullet->field_30_x_distance <= FP_FromInteger(0))
                    {
                        auto pBlood = ae_new<Blood>();
                        if (pBlood)
                        {
                            pBlood->ctor_40F0B0(
                                field_B8_xpos,
                                pBullet->field_2C_ypos,
                                FP_FromInteger(-24),
                                FP_FromInteger(0),
                                field_CC_sprite_scale, 50);
                        }
                    }
                    else
                    {
                        auto pBlood = ae_new<Blood>();
                        if (pBlood)
                        {
                            pBlood->ctor_40F0B0(
                                field_B8_xpos,
                                pBullet->field_2C_ypos,
                                FP_FromInteger(24),
                                FP_FromInteger(0),
                                field_CC_sprite_scale, 50);
                        }
                    }
                    break;

                case BulletType::ePossessedSligZBullet_1:
                case BulletType::eZBullet_3:
                {
                    auto pBlood = ae_new<Blood>();
                    if (pBlood)
                    {
                        pBlood->ctor_40F0B0(
                            field_B8_xpos,
                            field_BC_ypos - (FP_FromInteger(20) * field_CC_sprite_scale),
                            FP_FromInteger(0),
                            FP_FromInteger(0),
                            field_CC_sprite_scale, 50);
                    }
                    break;
                }

                default:
                    break;
            }

            Sfx_4C7D30(SlogSound::DeathWhine_9);
            field_10C_health = FP_FromInteger(0);
            field_120_brain_state_idx = 3;
            field_106_current_motion = eSlogMotions::M_Dying_21_4C77F0;
            field_124_timer = sGnFrame_5C1B84 + 90;
            SetAnimFrame_4C42A0();
            field_20_animation.field_4_flags.Set(AnimFlags::eBit2_Animate);
            field_160_flags.Set(Flags_160::eBit3_Shot);
            sSlogCount_BAF7F2--;
            break;
        }

        case AETypes::eDrill_30:
        case AETypes::eBaseBomb_46:
        case AETypes::eExplosion_109:
        {
            Sfx_4C7D30(SlogSound::DeathWhine_9);
            field_10C_health = FP_FromInteger(0);
            auto pGibs = ae_new<Gibs>();
            if (pGibs)
            {
                pGibs->ctor_40FB40(GibType::Slog_2, field_B8_xpos, field_BC_ypos, field_C4_velx, field_C8_vely, field_CC_sprite_scale, 0);
            }

            PSX_RECT bRect = {};
            vGetBoundingRect_424FD0(&bRect, 1);
            auto pBlood = ae_new<Blood>();
            if (pBlood)
            {
                pBlood->ctor_40F0B0(
                    FP_FromInteger((bRect.x + bRect.w) / 2),
                    FP_FromInteger((bRect.y + bRect.h) / 2),
                    FP_FromInteger(0),
                    FP_FromInteger(0),
                    field_CC_sprite_scale, 50);
            }

            field_6_flags.Set(BaseGameObject::eDead_Bit3);
            break;
        }

        case AETypes::eElectricWall_39:
            Sfx_4C7D30(SlogSound::DeathWhine_9);
            field_160_flags.Set(Flags_160::eBit13_Unused);
            break;

        case AETypes::eRockSpawner_48:
        case AETypes::eMineCar_89:
            Sfx_4C7D30(SlogSound::DeathWhine_9);
            field_10C_health = FP_FromInteger(0);
            field_120_brain_state_idx = 3;
            field_106_current_motion = eSlogMotions::M_Dying_21_4C77F0;
            field_124_timer = sGnFrame_5C1B84 + 90;
            SetAnimFrame_4C42A0();
            field_20_animation.field_4_flags.Set(AnimFlags::eBit2_Animate);
            break;

        case AETypes::eAbilityRing_104:
            if (!field_160_flags.Get(Flags_160::eBit6_HitByAbilityRing))
            {
                field_160_flags.Set(Flags_160::eBit6_HitByAbilityRing);
                Sfx_4C7D30(SlogSound::DeathWhine_9);
            }
            break;

        case AETypes::eElectrocute_150:
            field_10C_health = FP_FromInteger(0);
            field_120_brain_state_idx = 3;
            field_6_flags.Set(BaseGameObject::eDead_Bit3);
            break;

        default:
            return 1;
    }
    return 1;
}

void Slog::vOnThrowableHit_4C4B50(BaseGameObject* /*pFrom*/)
{
    field_142_anger_level += field_148_chase_anger; // on throwable hit?
}

s16 Slog::PlayerOrNakedSligNear_4C26A0()
{
    const FP kMinXDist = FP_FromInteger(100) * field_CC_sprite_scale;
    const FP kMinYDist = FP_FromInteger(25) * field_CC_sprite_scale;

    // Is the player near?
    if (FP_Abs(sActiveHero_5C1B68->field_B8_xpos - field_B8_xpos) < kMinXDist && FP_Abs(sActiveHero_5C1B68->field_BC_ypos - field_BC_ypos) < kMinYDist && sActiveHero_5C1B68->field_CC_sprite_scale == FP_FromInteger(1))
    {
        return 1;
    }

    for (s32 i = 0; i < gBaseAliveGameObjects_5C1B7C->Size(); i++)
    {
        BaseAliveGameObject* pObj = gBaseAliveGameObjects_5C1B7C->ItemAt(i);
        if (!pObj)
        {
            break;
        }

        if (pObj->Type() == AETypes::eCrawlingSlig_26)
        {
            // Is this naked slig near?
            if (FP_Abs(pObj->field_B8_xpos - field_B8_xpos) < kMinXDist && FP_Abs(pObj->field_BC_ypos - field_BC_ypos) < kMinYDist && pObj->field_CC_sprite_scale == field_CC_sprite_scale)
            {
                return 1;
            }
        }
    }
    return 0;
}

void Slog::DelayedResponse_4C3750(s16 responseIdx)
{
    field_140_response_part = 0;
    field_13E_response_index = responseIdx;
    field_124_timer = sGnFrame_5C1B84 + 10;
}

s16 Slog::HandleEnemyStopper_4C5340()
{
    FP xPos = ScaleToGridSize_4498B0(field_CC_sprite_scale) * FP_FromInteger(2);
    if (field_C4_velx >= FP_FromInteger(0))
    {
        xPos += field_B8_xpos;
    }
    FP width = field_B8_xpos;
    if (field_B8_xpos <= xPos)
    {
        width = xPos;
    }
    FP xToUse = xPos;
    if (field_B8_xpos <= xPos)
    {
        xToUse = field_B8_xpos;
    }

    const auto stopperPath = static_cast<Path_EnemyStopper*>(
        sPath_dword_BB47C0->TLV_Get_At_4DB4B0(
            FP_GetExponent(xToUse), FP_GetExponent(field_BC_ypos),
            FP_GetExponent(width), FP_GetExponent(field_BC_ypos), TlvTypes::EnemyStopper_47));

    return stopperPath != nullptr && stopperPath->field_10_stop_direction == (field_C4_velx > FP_FromInteger(0) ? Path_EnemyStopper::StopDirection::Right_1 : Path_EnemyStopper::StopDirection::Left_0) && SwitchStates_Get_466020(stopperPath->field_12_switch_id) > 0;
}

s16 Slog::Facing_4C4020(FP xpos)
{
    if (field_B8_xpos < xpos && !field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX))
    {
        return TRUE;
    }

    if (xpos < field_B8_xpos && field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX))
    {
        return TRUE;
    }

    return FALSE;
}
