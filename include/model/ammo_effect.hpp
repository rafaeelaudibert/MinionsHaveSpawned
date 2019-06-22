#ifndef AMMO_EFFECTS_H
#define AMMO_EFFECTS_H

namespace AmmoEffects
{
    class AmmoEffect
    {
    protected:
        AmmoEffect(float ammo_power = 0.0f, float ammo_time = 0.0f) : ammo_power(ammo_power), ammo_time(ammo_time) {};

    public:
        float ammo_power = 0.0f;
        float ammo_time = 0.0f;

        virtual void update(float delta_time)
        {
            if (ammo_time >= 0.0f)
                ammo_time -= delta_time;
        }
    };

    class NoneEffect : public AmmoEffect
    {
    public:
        NoneEffect() : AmmoEffect(0.0f, 0.0f) { }
    };


    class AreaDamageEffect : public AmmoEffect
    {
    public:
        AreaDamageEffect(float ammo_power = 7.5f, float ammo_time = 0.0f) : AmmoEffect(ammo_power, ammo_time) { };
    };

    class DamagePerSecondEffect : public AmmoEffect
    {
    public:
        DamagePerSecondEffect(float ammo_power = 5.0f, float ammo_time = 2.0f) : AmmoEffect(ammo_power, ammo_time) { };
    };

    class SlowEffect : public AmmoEffect
    {
    public:
        SlowEffect(float ammo_power = 0.5f, float ammo_time = 2.0f) : AmmoEffect(ammo_power, ammo_time) { };
    };

    class FreezingEffect : public AmmoEffect
    {
    public:
        FreezingEffect(float ammo_power = 1.0f, float ammo_time = 1.0f) : AmmoEffect(ammo_power, ammo_time) { };
    };
}

#endif // AMMO_EFFECTS_H
