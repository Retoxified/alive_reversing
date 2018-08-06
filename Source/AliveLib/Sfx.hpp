#pragma once

#include "FunctionFwd.hpp"

struct SfxDefinition
{
    BYTE field_0_block_idx;
    BYTE field_1_program;
    BYTE field_2_note;
    BYTE field_3_default_volume;
    BYTE field_4;
    BYTE field_5;
    __int16 field_6_pitch_variation;
};


EXPORT int CC SFX_Play_46FBA0(unsigned __int8 sfxIdx, __int16 volume, int a3, int scale);
EXPORT int CC SFX_Play_46FA90(unsigned __int8 sfxIdx, __int16 volume, int scale);