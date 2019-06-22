#ifndef AMMO_EFFECTS_H
#define AMMO_EFFECTS_H

namespace AmmoEffects
{
    class AmmoEffect
    {
    protected:
        AmmoEffect(float ammo_damage = 0.0f, float ammo_time = 0.0f, float ammo_speed_multiplier = 1.0f) : ammo_damage(ammo_damage), ammo_time(ammo_time), ammo_speed_multiplier(ammo_speed_multiplier) {};

    public:
        float ammo_damage = 0.0f;
        float ammo_time = 0.0f;
        float ammo_speed_multiplier = 1.0f;
        std::string ammo_type = "";

        virtual void update(float delta_time)
        {
            if (ammo_time >= 0.0f)
                ammo_time -= delta_time;
        }
    };

    class NoneEffect : public AmmoEffect
    {
    public:
        NoneEffect() : AmmoEffect(0.0f, 0.0f, 1.0f) { }
        std::string ammo_type = "none";
    };


    class AreaDamageEffect : public AmmoEffect
    {
    public:
        AreaDamageEffect(float ammo_damage = 7.5f, float ammo_time = 0.0f, float ammo_speed_multiplier = 1.0f) : AmmoEffect(ammo_damage, ammo_time, ammo_speed_multiplier) { };
        std::string ammo_type = "aoe";
    };

    class DamagePerSecondEffect : public AmmoEffect
    {
    public:
        DamagePerSecondEffect(float ammo_damage = 5.0f, float ammo_time = 2.0f, float ammo_speed_multiplier = 1.0f) : AmmoEffect(ammo_damage, ammo_time, ammo_speed_multiplier) { };
        std::string ammo_type = "dps";
    };

    class SlowEffect : public AmmoEffect
    {
    public:
        SlowEffect(float ammo_damage = 0.0f, float ammo_time = 2.0f, float ammo_speed_multiplier = 0.65f) : AmmoEffect(ammo_damage, ammo_time, ammo_speed_multiplier) { };
        std::string ammo_type = "slow";
    };

    class FreezingEffect : public AmmoEffect
    {
    public:
        FreezingEffect(float ammo_damage = 0.0f, float ammo_time = 0.75f, float ammo_speed_multiplier = 0.0f) : AmmoEffect(ammo_damage, ammo_time, ammo_speed_multiplier) { };
        std::string ammo_type = "slow";
    };
}

#endif // AMMO_EFFECTS_H
