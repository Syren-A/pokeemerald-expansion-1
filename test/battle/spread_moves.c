#include "global.h"
#include "test/battle.h"

DOUBLE_BATTLE_TEST("Ability and Item effects activate correctly after a multi target move")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { Item(ITEM_LUM_BERRY); }
        PLAYER(SPECIES_WOBBUFFET) { Item(ITEM_COVERT_CLOAK); }
        OPPONENT(SPECIES_GOLISOPOD) { Ability(ABILITY_EMERGENCY_EXIT); MaxHP(260); HP(131); };
        OPPONENT(SPECIES_WOBBUFFET) { Item(ITEM_EJECT_BUTTON); }
        OPPONENT(SPECIES_WYNAUT);
        OPPONENT(SPECIES_PIKACHU);
    } WHEN {
        TURN {
            MOVE(opponentRight, MOVE_HEAT_WAVE);
            MOVE(playerLeft, MOVE_HYPER_VOICE);
            SEND_OUT(opponentRight, 3);
            SEND_OUT(opponentLeft, 2);
        }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_HYPER_VOICE, playerLeft);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_HELD_ITEM_EFFECT, opponentRight);
        MESSAGE("Foe Wobbuffet is switched out with the Eject Button!");
        MESSAGE("2 sent out Pikachu!");
        ABILITY_POPUP(opponentLeft, ABILITY_EMERGENCY_EXIT);
        MESSAGE("2 sent out Wynaut!");
    }
}

DOUBLE_BATTLE_TEST("t3")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET);
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_WOBBUFFET);
    } WHEN {
        TURN { MOVE(opponentLeft, MOVE_PROTECT); MOVE(playerLeft, MOVE_HYPER_VOICE); }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_PROTECT, opponentLeft);
        ANIMATION(ANIM_TYPE_MOVE, MOVE_HYPER_VOICE, playerLeft);
    }
}

DOUBLE_BATTLE_TEST("t4")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET);
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_WOBBUFFET);
    } WHEN {
        TURN { MOVE(opponentRight, MOVE_PROTECT); MOVE(playerLeft, MOVE_HYPER_VOICE); }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_PROTECT, opponentRight);
        ANIMATION(ANIM_TYPE_MOVE, MOVE_HYPER_VOICE, playerLeft);
    }
}

DOUBLE_BATTLE_TEST("t5") // Fixed issue #1961
{
    GIVEN {
        PLAYER(SPECIES_ABRA);
        PLAYER(SPECIES_JOLTEON) { Ability(ABILITY_VOLT_ABSORB); HP(1); MaxHP(100); }
        OPPONENT(SPECIES_GRAVELER_ALOLAN) { Ability(ABILITY_GALVANIZE); }
        OPPONENT(SPECIES_WYNAUT);
    } WHEN {
        TURN { MOVE(opponentLeft, MOVE_EXPLOSION); }
    } SCENE {

    }
}

DOUBLE_BATTLE_TEST("t6") // Fixed issue #1961
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET);
        PLAYER(SPECIES_ABRA);
        OPPONENT(SPECIES_GRAVELER_ALOLAN) { Ability(ABILITY_GALVANIZE); }
        OPPONENT(SPECIES_WYNAUT);
    } WHEN {
        TURN { MOVE(opponentLeft, MOVE_EXPLOSION); }
    } SCENE {

    }
}

DOUBLE_BATTLE_TEST("A spread move attack will activate both resist berries")
{
    s16 opponentLeftDmg[2];
    s16 opponentRightDmg[2];

    GIVEN {
        PLAYER(SPECIES_GARDEVOIR);
        PLAYER(SPECIES_RALTS);
        OPPONENT(SPECIES_RAICHU) { Item(ITEM_CHILAN_BERRY); }
        OPPONENT(SPECIES_SANDSLASH) { Item(ITEM_CHILAN_BERRY); }
    } WHEN {
        TURN { MOVE(playerLeft, MOVE_HYPER_VOICE); }
        TURN { MOVE(playerLeft, MOVE_HYPER_VOICE); }
    } SCENE {
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_HELD_ITEM_EFFECT, opponentLeft);
        MESSAGE("Chilan Berry weakened the damage to Foe Raichu!");
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_HELD_ITEM_EFFECT, opponentRight);
        MESSAGE("Chilan Berry weakened the damage to Foe Sandslash!");

        ANIMATION(ANIM_TYPE_MOVE, MOVE_HYPER_VOICE, playerLeft);
        HP_BAR(opponentLeft, captureDamage: &opponentLeftDmg[0]);
        HP_BAR(opponentRight, captureDamage: &opponentRightDmg[0]);

        ANIMATION(ANIM_TYPE_MOVE, MOVE_HYPER_VOICE, playerLeft);
        HP_BAR(opponentLeft, captureDamage: &opponentLeftDmg[1]);
        HP_BAR(opponentRight, captureDamage: &opponentRightDmg[1]);
    } THEN {
        EXPECT_MUL_EQ(opponentLeftDmg[1], Q_4_12(0.5), opponentLeftDmg[0]);
        EXPECT_MUL_EQ(opponentRightDmg[1], Q_4_12(0.5), opponentRightDmg[0]);
    }
}

DOUBLE_BATTLE_TEST("If a spread move attack will activate a resist berries on one pokemon, only the damage for that mon will be reduced")
{
    s16 opponentLeftDmg[2];
    s16 opponentRightDmg[2];

    GIVEN {
        PLAYER(SPECIES_GARDEVOIR);
        PLAYER(SPECIES_RALTS);
        OPPONENT(SPECIES_RAICHU)
        OPPONENT(SPECIES_SANDSLASH) { Item(ITEM_CHILAN_BERRY); }
    } WHEN {
        TURN { MOVE(playerLeft, MOVE_HYPER_VOICE); }
        TURN { MOVE(playerLeft, MOVE_HYPER_VOICE); }
    } SCENE {
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_HELD_ITEM_EFFECT, opponentRight);
        MESSAGE("Chilan Berry weakened the damage to Foe Sandslash!");

        ANIMATION(ANIM_TYPE_MOVE, MOVE_HYPER_VOICE, playerLeft);
        HP_BAR(opponentLeft, captureDamage: &opponentLeftDmg[0]);
        HP_BAR(opponentRight, captureDamage: &opponentRightDmg[0]);

        ANIMATION(ANIM_TYPE_MOVE, MOVE_HYPER_VOICE, playerLeft);
        HP_BAR(opponentLeft, captureDamage: &opponentLeftDmg[1]);
        HP_BAR(opponentRight, captureDamage: &opponentRightDmg[1]);
    } THEN {
        EXPECT_EQ(opponentLeftDmg[1], opponentLeftDmg[0]);
        EXPECT_MUL_EQ(opponentRightDmg[1], Q_4_12(0.5), opponentRightDmg[0]);
    }
}

DOUBLE_BATTLE_TEST("A spread move attack will be weakened by strong winds")
{
    s16 opponentLeftDmg[2];
    s16 opponentRightDmg[2];

    GIVEN {
        PLAYER(SPECIES_GARDEVOIR);
        PLAYER(SPECIES_RAYQUAZA) { Ability(ABILITY_AIR_LOCK); }
        PLAYER(SPECIES_RALTS);
        OPPONENT(SPECIES_ZAPDOS)
        OPPONENT(SPECIES_RAYQUAZA) { Moves(MOVE_DRAGON_ASCENT, MOVE_CELEBRATE); }
    } WHEN {
        TURN { MOVE(opponentRight, MOVE_CELEBRATE, gimmick: GIMMICK_MEGA); MOVE(playerLeft, MOVE_ROCK_SLIDE); }
        TURN { SWITCH(playerRight, 2); MOVE(opponentRight, MOVE_CELEBRATE); MOVE(playerLeft, MOVE_ROCK_SLIDE); }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_ROCK_SLIDE, playerLeft);
        HP_BAR(opponentLeft, captureDamage: &opponentLeftDmg[0]);
        HP_BAR(opponentRight, captureDamage: &opponentRightDmg[0]);

        ANIMATION(ANIM_TYPE_MOVE, MOVE_ROCK_SLIDE, playerLeft);
        HP_BAR(opponentLeft, captureDamage: &opponentLeftDmg[1]);
        HP_BAR(opponentRight, captureDamage: &opponentRightDmg[1]);
    } THEN {
        EXPECT_EQ(opponentLeftDmg[1], opponentLeftDmg[0]);
        EXPECT_MUL_EQ(opponentRightDmg[0], Q_4_12(0.5), opponentRightDmg[1]);
    }
}

