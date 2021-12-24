/*
 * Copyright (C) 2016+ AzerothCore <www.azerothcore.org>
 * Released under GNU AGPL v3 
 * License: https://github.com/azerothcore/azerothcore-wotlk/blob/master/LICENSE-AGPL3
 */

#ifndef _BOT_DREADLORD_H_
#define _BOT_DREADLORD_H_

#include "BotCommon.h"
#include "Creature.h"
#include "Object.h"
#include "ScriptedCreature.h"
#include "ScriptMgr.h"
#include "Spell.h"
#include "BotAI.h"

enum DreadlordBaseSpells
{
    CARRION_SWARM_1         = SPELL_CARRION_SWARM,
    SLEEP_1                 = SPELL_SLEEP,
    INFERNO_1               = SPELL_INFERNO_METEOR_VISUAL
};

enum DreadlordPassives
{
    VAMPIRIC_AURA           = SPELL_VAMPIRIC_AURA,
};

enum DreadlordSpecial
{
    MH_ATTACK_ANIM          = SPELL_ATTACK_MELEE_1H,

    CARRION_COST            = 110 * 5,
    SLEEP_COST              = 50 * 5,
    INFERNAL_COST           = 175 * 5,

    DAMAGE_CD_REDUCTION     = 250,  //ms
    INFERNO_SPAWN_DELAY     = 650,  //ms

    IMMOLATION              = 39007
};

static const uint32 dreadlord_spells_damage_arr[] =     { CARRION_SWARM_1, INFERNO_1 };
static const uint32 dreadlord_spells_cc_arr[] =         { SLEEP_1 };
static const uint32 dreadlord_spells_support_arr[] =    { INFERNO_1 };

static const std::vector<uint32> dreadlord_spells_damage(FROM_ARRAY(dreadlord_spells_damage_arr));
static const std::vector<uint32> dreadlord_spells_cc(FROM_ARRAY(dreadlord_spells_cc_arr));
static const std::vector<uint32> dreadlord_spells_support(FROM_ARRAY(dreadlord_spells_support_arr));

class BotDreadlordAI : public BotAI
{
private:
    class DelayedPetUnSpawnEvent : public BasicEvent
    {
    public:
        DelayedPetUnSpawnEvent(Creature const* bot, Position const* pos) : m_bot(bot) { }

    protected:
        bool Execute(uint64 /*e_time*/, uint32 /*p_time*/)
        {
            ((BotDreadlordAI*)m_bot->AI())->UnSummonBotPet();

            return true;
        }

    private:
        DelayedPetUnSpawnEvent(DelayedPetUnSpawnEvent const&);

    private:
        Creature const* m_bot;
    };

public:
    BotDreadlordAI(Creature* creature);

public:
    void UnSummonBotPet() override;
    void OnClassSpellGo(SpellInfo const* spellInfo) override;

protected:
    void UpdateBotAI(uint32 uiDiff) override;
    void CheckAura(uint32 uiDiff);
    void RefreshAura(uint32 spellId, int8 count = 1, Unit* target = nullptr) const;
    void ReduceCooldown(uint32 uiDiff) override;
    
    void InitCustomeSpells() override;

private:
    bool DoSummonInfernoIfReady(uint32 uiDiff);

private:
    uint32 m_checkAuraTimer;
    Position m_infernoPos;
};

class BotDreadlord : public CreatureScript
{
    friend class BotDreadlordAI;

public:
    BotDreadlord() : CreatureScript("bot_dreadlord") { }

public:
    CreatureAI* GetAI(Creature* creature) const
    {
        return new BotDreadlordAI(creature);
    }
};

#endif // _BOT_DREADLORD_H_
