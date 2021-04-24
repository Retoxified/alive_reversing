#pragma once

#include "FunctionFwd.hpp"
#include "Primitives.hpp"
#include "BaseGameObject.hpp"

class ScreenShake : public BaseGameObject
{
public:
    EXPORT ScreenShake* ctor_4ACF70(s16 enableShakeEvent, s16 softerShakes);
   
    virtual BaseGameObject* VDestructor(signed int flags) override;
    virtual void VUpdate() override;
    virtual void VRender(PrimHeader** ppOt) override;

private:
    EXPORT void dtor_4AD060();
    EXPORT void vUpdate_4AD0E0();
    EXPORT BaseGameObject* vdtor_4AD030(signed int flags);
    EXPORT void vRender_4AD120(PrimHeader** /*ppOt*/);

private:
    Prim_ScreenOffset field_20_screenOffset[2];
    s16 field_40_shakeNumber;
    s16 field_42_enableShakeEvent;
    s16 field_44_softerShakes;

    s16 field_46_padding;
    int field_48_padding;
};
ALIVE_ASSERT_SIZEOF(ScreenShake, 0x4C);
