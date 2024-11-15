#include "global.h"
#include "test/battle.h"

DOUBLE_BATTLE_TEST("Spread Moves: Ability and Item effects activate correctly after a multi target move")
{
    // TODO: Might be a bug, verify on showdown
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
        MESSAGE("The opposing Wobbuffet is switched out with the Eject Button!");
        MESSAGE("2 sent out Pikachu!");
        ABILITY_POPUP(opponentLeft, ABILITY_EMERGENCY_EXIT);
        MESSAGE("2 sent out Wynaut!");
    }
}


DOUBLE_BATTLE_TEST("Spread Moves: t5") // Fixed issue #1961
{
    GIVEN {
        PLAYER(SPECIES_ABRA);
        PLAYER(SPECIES_JOLTEON) { Ability(ABILITY_VOLT_ABSORB); HP(1); MaxHP(100); }
        OPPONENT(SPECIES_GRAVELER_ALOLA) { Ability(ABILITY_GALVANIZE); }
        OPPONENT(SPECIES_WYNAUT);
    } WHEN {
        TURN { MOVE(opponentLeft, MOVE_EXPLOSION); }
    } SCENE {

    }
}

DOUBLE_BATTLE_TEST("Spread Moves: t6") // Fixed issue #1961
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET);
        PLAYER(SPECIES_ABRA);
        OPPONENT(SPECIES_GRAVELER_ALOLA) { Ability(ABILITY_GALVANIZE); }
        OPPONENT(SPECIES_WYNAUT);
    } WHEN {
        TURN { MOVE(opponentLeft, MOVE_EXPLOSION); }
    } SCENE {

    }
}

DOUBLE_BATTLE_TEST("Spread Moves: No damage will be dealt to a mon with an invulnerable position")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET);
        PLAYER(SPECIES_WYNAUT);
        OPPONENT(SPECIES_ZAPDOS);
        OPPONENT(SPECIES_WOBBUFFET);
    } WHEN {
        TURN { MOVE(opponentLeft, MOVE_FLY, target: playerLeft); MOVE(playerLeft, MOVE_HYPER_VOICE); }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_HYPER_VOICE, playerLeft);
        NOT HP_BAR(opponentLeft);
        HP_BAR(opponentRight);
    }
}

DOUBLE_BATTLE_TEST("[1.1] Spread Moves: No damage will be dealt to a mon with an invulnerable position")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET);
        PLAYER(SPECIES_WYNAUT);
        OPPONENT(SPECIES_ZAPDOS);
        OPPONENT(SPECIES_WOBBUFFET);
    } WHEN {
        TURN { MOVE(playerLeft, MOVE_BUBBLE_BEAM, target: opponentLeft); MOVE(opponentLeft, MOVE_SURF); }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_SURF, opponentLeft);
    }
}

DOUBLE_BATTLE_TEST("Spread Moves: A spread move attack will activate both resist berries")
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
        MESSAGE("The Chilan Berry weakened the damage to the opposing Raichu!");
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_HELD_ITEM_EFFECT, opponentRight);
        MESSAGE("The Chilan Berry weakened the damage to the opposing Sandslash!");

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

DOUBLE_BATTLE_TEST("Spread Moves: If a spread move attack will activate a resist berries on one pokemon, only the damage for that mon will be reduced")
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
        MESSAGE("The Chilan Berry weakened the damage to the opposing Sandslash!");

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

DOUBLE_BATTLE_TEST("Spread Moves: A spread move attack will be weakened by strong winds on both targets")
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
        EXPECT_MUL_EQ(opponentLeftDmg[0], Q_4_12(0.5), opponentLeftDmg[1]);
        EXPECT_MUL_EQ(opponentRightDmg[0], Q_4_12(0.5), opponentRightDmg[1]);
    }
}

DOUBLE_BATTLE_TEST("Spread Moves: A spread move attack will be weakened by strong winds on one of the targets")
{
    s16 opponentLeftDmg[2];
    s16 opponentRightDmg[2];

    GIVEN {
        PLAYER(SPECIES_GARDEVOIR);
        PLAYER(SPECIES_RAYQUAZA) { Ability(ABILITY_AIR_LOCK); }
        PLAYER(SPECIES_RALTS);
        OPPONENT(SPECIES_DONPHAN)
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

DOUBLE_BATTLE_TEST("Spread Moves: AOE move vs Disguise, Volt Absorb and Lightning Rod")
{
    KNOWN_FAILING;
    GIVEN {
        ASSUME(gMovesInfo[MOVE_DISCHARGE].target == MOVE_TARGET_FOES_AND_ALLY);
        ASSUME(gMovesInfo[MOVE_DISCHARGE].type == TYPE_ELECTRIC);
        PLAYER(SPECIES_WOBBUFFET) { Speed(40); }
        PLAYER(SPECIES_MIMIKYU) { Speed(30); }
        OPPONENT(SPECIES_JOLTEON) { Speed(20); Ability(ABILITY_LIGHTNING_ROD); }
        OPPONENT(SPECIES_LANTURN) { Speed(10); Ability(ABILITY_VOLT_ABSORB); HP(1); }
    } WHEN {
        TURN { MOVE(playerLeft, MOVE_DISCHARGE); }
    } SCENE {
        ABILITY_POPUP(opponentLeft, ABILITY_LIGHTNING_ROD);
        ABILITY_POPUP(opponentRight, ABILITY_VOLT_ABSORB);
        ANIMATION(ANIM_TYPE_MOVE, MOVE_DISCHARGE, playerLeft);
        ABILITY_POPUP(playerRight, ABILITY_DISGUISE);
    }
}

DOUBLE_BATTLE_TEST("Spread Moves: AOE move vs Eiscue and Mimikyu (Based on vanilla games)")
{
    KNOWN_FAILING; // Damage not based on speed but battler ID.
    GIVEN {
        ASSUME(gMovesInfo[MOVE_EARTHQUAKE].target == MOVE_TARGET_FOES_AND_ALLY);
        ASSUME(gMovesInfo[MOVE_EARTHQUAKE].category == DAMAGE_CATEGORY_PHYSICAL);
        PLAYER(SPECIES_WOBBUFFET) { Speed(40); }
        PLAYER(SPECIES_EISCUE) { Speed(30); }
        OPPONENT(SPECIES_MIMIKYU) { Speed(20); }
        OPPONENT(SPECIES_EISCUE) { Speed(10); }
    } WHEN {
        TURN { MOVE(playerLeft, MOVE_EARTHQUAKE); }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_EARTHQUAKE, playerLeft);
        ABILITY_POPUP(playerRight, ABILITY_ICE_FACE);
        ABILITY_POPUP(opponentLeft, ABILITY_DISGUISE);
        ABILITY_POPUP(opponentRight, ABILITY_ICE_FACE);
    }
}

// Can be removed once the above test passes
DOUBLE_BATTLE_TEST("Spread Moves: AOE move vs Eiscue and Mimikyu (Based on battler id)")
{
    GIVEN {
        ASSUME(gMovesInfo[MOVE_EARTHQUAKE].target == MOVE_TARGET_FOES_AND_ALLY);
        ASSUME(gMovesInfo[MOVE_EARTHQUAKE].category == DAMAGE_CATEGORY_PHYSICAL);
        PLAYER(SPECIES_WOBBUFFET);
        PLAYER(SPECIES_EISCUE);
        OPPONENT(SPECIES_MIMIKYU);
        OPPONENT(SPECIES_EISCUE);
    } WHEN {
        TURN { MOVE(playerLeft, MOVE_EARTHQUAKE); }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_EARTHQUAKE, playerLeft);
        ABILITY_POPUP(opponentLeft, ABILITY_DISGUISE);
        ABILITY_POPUP(playerRight, ABILITY_ICE_FACE);
        ABILITY_POPUP(opponentRight, ABILITY_ICE_FACE);
    }
}

DOUBLE_BATTLE_TEST("Spread Moves: Spread move, Gem Boosted, vs Resist Berries")
{
    GIVEN {
        ASSUME(gMovesInfo[MOVE_HYPER_VOICE].target == MOVE_TARGET_BOTH);
        PLAYER(SPECIES_WOBBUFFET) { Speed(40); Item(ITEM_NORMAL_GEM); }
        PLAYER(SPECIES_WYNAUT) { Speed(30); }
        OPPONENT(SPECIES_WOBBUFFET) { Speed(20); Item(ITEM_CHILAN_BERRY); }
        OPPONENT(SPECIES_WYNAUT) { Speed(10); Item(ITEM_CHILAN_BERRY); }
    } WHEN {
        TURN { MOVE(playerLeft, MOVE_HYPER_VOICE); }
    } SCENE {
        MESSAGE("The Normal Gem strengthened Wobbuffet's power!");
        MESSAGE("The Chilan Berry weakened the damage to the opposing Wobbuffet!");
        MESSAGE("The Chilan Berry weakened the damage to the opposing Wynaut!");
        ANIMATION(ANIM_TYPE_MOVE, MOVE_HYPER_VOICE, playerLeft);
        HP_BAR(opponentLeft);
        HP_BAR(opponentRight);
    }
}

DOUBLE_BATTLE_TEST("Spread Moves: Explosion, Gem Boosted, vs Resist Berries")
{
    GIVEN {
        ASSUME(gMovesInfo[MOVE_EXPLOSION].target == MOVE_TARGET_FOES_AND_ALLY);
        PLAYER(SPECIES_WOBBUFFET) { Speed(40); Item(ITEM_NORMAL_GEM); }
        PLAYER(SPECIES_MISDREAVUS) { Speed(30); }
        OPPONENT(SPECIES_WOBBUFFET) { Speed(20); Item(ITEM_CHILAN_BERRY); }
        OPPONENT(SPECIES_WYNAUT) { Speed(10); Item(ITEM_CHILAN_BERRY); }
    } WHEN {
        TURN { MOVE(playerLeft, MOVE_EXPLOSION); }
    } SCENE {
        MESSAGE("The Normal Gem strengthened Wobbuffet's power!");
        MESSAGE("The Chilan Berry weakened the damage to the opposing Wobbuffet!");
        MESSAGE("The Chilan Berry weakened the damage to the opposing Wynaut!");
        ANIMATION(ANIM_TYPE_MOVE, MOVE_EXPLOSION, playerLeft);
        HP_BAR(opponentLeft);
        HP_BAR(opponentRight);
        MESSAGE("It doesn't affect Misdreavus…");
    }
}

DOUBLE_BATTLE_TEST("Spread Moves: Spread move vs Eiscue and Mimikyu with 1 Eject Button")
{
    GIVEN {
        ASSUME(gMovesInfo[MOVE_RAZOR_LEAF].target == MOVE_TARGET_BOTH);
        ASSUME(gMovesInfo[MOVE_RAZOR_LEAF].category == DAMAGE_CATEGORY_PHYSICAL);
        PLAYER(SPECIES_WOBBUFFET) { Speed(40); }
        PLAYER(SPECIES_WYNAUT) { Speed(30); }
        OPPONENT(SPECIES_MIMIKYU) { Speed(20); Item(ITEM_EJECT_BUTTON); }
        OPPONENT(SPECIES_EISCUE) { Speed(10); }
        OPPONENT(SPECIES_WOBBUFFET) { Speed(100); }
    } WHEN {
        TURN { MOVE(playerLeft, MOVE_RAZOR_LEAF); SEND_OUT(opponentLeft, 2); }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_RAZOR_LEAF, playerLeft);
        ABILITY_POPUP(opponentLeft, ABILITY_DISGUISE);
        ABILITY_POPUP(opponentRight, ABILITY_ICE_FACE);
        MESSAGE("The opposing Mimikyu is switched out with the Eject Button!");
    }
}

DOUBLE_BATTLE_TEST("Spread Moves: Spread move vs Wide Guard")
{
    KNOWN_FAILING; // Message missing
    GIVEN {
        ASSUME(gMovesInfo[MOVE_HYPER_VOICE].target == MOVE_TARGET_BOTH);
        PLAYER(SPECIES_WOBBUFFET) { Speed(40); }
        PLAYER(SPECIES_WYNAUT) { Speed(20); }
        OPPONENT(SPECIES_WOBBUFFET) { Speed(30); }
        OPPONENT(SPECIES_WYNAUT) { Speed(10); }
    } WHEN {
        TURN { MOVE(playerLeft, MOVE_WIDE_GUARD); MOVE(opponentLeft, MOVE_HYPER_VOICE); }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_WIDE_GUARD, playerLeft);
        MESSAGE("Wide Guard protected your team!");
        MESSAGE("The opposing Wobbuffet used Hyper Voice");
        MESSAGE("Wide Guard protected Wobbuffet!");
        MESSAGE("Wide Guard protected Wynaut!");
        NOT { ANIMATION(ANIM_TYPE_MOVE, MOVE_HYPER_VOICE, playerLeft); }
    }
}

DOUBLE_BATTLE_TEST("Spread Moves: Spread move vs one protecting mon")
{
    KNOWN_FAILING; // Wrong message
    GIVEN {
        ASSUME(gMovesInfo[MOVE_HYPER_VOICE].target == MOVE_TARGET_BOTH);
        PLAYER(SPECIES_WOBBUFFET) { Speed(40); }
        PLAYER(SPECIES_WYNAUT) { Speed(30); }
        OPPONENT(SPECIES_WOBBUFFET) { Speed(20); }
        OPPONENT(SPECIES_WYNAUT) { Speed(10); }
    } WHEN {
        TURN { MOVE(opponentLeft, MOVE_PROTECT); MOVE(playerLeft, MOVE_HYPER_VOICE); }
    } SCENE {
        MESSAGE("Foe Wobbuffet used Protect!");
        MESSAGE("Wobbuffet used Hyper Voice!");
        MESSAGE("Foe Wobbuffet protected itself!");
        ANIMATION(ANIM_TYPE_MOVE, MOVE_WIDE_GUARD, playerLeft);
        HP_BAR(opponentRight);
    }
}