#ifndef GUARD_RANDOMIZER_H
#define GUARD_RANDOMIZER_H

#include "config/randomizer.h"
#if RANDOMIZER_AVAILABLE == TRUE

#define RANDOMIZER_MAX_MON  (SPECIES_EGG-1)
#define RANDOMIZER_SPECIES_COUNT (RANDOMIZER_MAX_MON+1)

#include "global.h"
#include "random.h"
#include "script.h"
#include "wild_encounter.h"
//#include "constants/moves.h"

#define RANDOMIZER_STREAM 17

enum RandomizerFeature
{
    RANDOMIZE_WILD_MON,    // Wild encounter randomization.
    RANDOMIZE_TRAINER_MON, // Trainer party randomization.
    RANDOMIZE_FIELD_ITEMS, // Item ball and hidden item randomization.
    RANDOMIZE_BASE_STATS,  // Randomization of species base stats. Not yet implemented.
    RANDOMIZE_MON_TYPES,   // Randomization of species types. Not yet implemented.
    RANDOMIZE_LEARNSET,    // Randomization of species learnset. Not yet implemented.
    RANDOMIZE_FIXED_MON,   // Randomization of fixed encounters.
    RANDOMIZE_STARTERS,    // Randomization of starter Pokémon species.
};

enum RandomizerReason
{
    RANDOMIZER_REASON_WILD_ENCOUNTER,
    RANDOMIZER_REASON_FIXED_ENCOUNTER,
    RANDOMIZER_REASON_TRAINER_PARTY,
    RANDOMIZER_REASON_BASE_STATS,
    RANDOMIZER_REASON_SPECIES_TYPE,
    RANDOMIZER_REASON_LEARNSET,
    RANDOMIZER_REASON_FIELD_ITEM,
    RANDOMIZER_REASON_STARTER,
};

enum RandomizerOption {
    RANDOMIZER_OPTION_SPECIES_MODE, // Controls how a species is randomized.
};

enum RandomizerSpeciesMode {
    // Any valid species can be randomized to any other valid species.
    MON_RANDOM,
    MON_RANDOM_LEGEND_AWARE,
    MON_RANDOM_BST,
    MON_EVOLUTION,
    // Other (custom) modes here.

    // A dummy mode to end the list.
    MON_MODE_MAX_COUNT
};

// This object can be passed to IsRandomizationPossible to speed up queries.
struct RandomizerGroupSet {
    u16 species;
    u16 minGroup;
    u16 maxGroup;
};


u32 GetRandomizerSeed(void);
bool32 RandomizerFeatureEnabled(enum RandomizerFeature feature);

struct Sfc32State RandomizerRandSeed(enum RandomizerReason reason, u32 data1, u32 data2);

// Returns the next number from a randomizer seed.
static inline u16 RandomizerNext(struct Sfc32State* state)
{
    return _SFC32_Next_Stream(state, RANDOMIZER_STREAM) >> 16;
}
u32 RandomizerNextRange(struct Sfc32State* state, u32 range);

u32 RandomizerRand(enum RandomizerReason reason, u32 data1, u32 data2);
u16 RandomizerRandRange(enum RandomizerReason reason, u32 data1, u32 data2, u32 range);

static inline u32 RandomizeMonType(u32 species, u32 typeNum)
{
    return (u8)RandomizerRandRange(RANDOMIZER_REASON_SPECIES_TYPE, species, typeNum, NUMBER_OF_MON_TYPES);
}

u32 RandomizeFoundItem(u32 itemId, u32 mapNum, u32 mapGroup, u32 localId);
void FindItemRandomize_NativeCall(struct ScriptContext *ctx);
void FindHiddenItemRandomize_NativeCall(struct ScriptContext *ctx);

u32 RandomizeMon(enum RandomizerReason reason, enum RandomizerSpeciesMode mode, u32 seed, u32 species);
u32 RandomizeMonBaseForm(enum RandomizerReason reason, enum RandomizerSpeciesMode mode, u32 seed, u32 species);

u32 RandomizeWildEncounter(u32 species, u32 mapNum, u32 mapGroup, enum WildArea area, u32 slot);

// Returns TRUE if it is possible for the species tableSpecies to randomize into the species matchSpecies.
// This does not mean that it actually did, though.
bool32 IsRandomizationPossible(u32 tableSpecies, u32 matchSpecies);

u32 RandomizeTrainerMon(u32 trainerId, u32 slot, u32 totalMons, u32 species);

u32 RandomizeFixedEncounterMon(u32 species, u32 mapNum, u32 mapGroup, u32 localId);

// Given a starter slot and the list of original starters, returns the starter in that slot.
u32 RandomizeStarter(u32 starterSlot, const u16* originalStarters);

static inline bool32 GroupSetsIntersect(struct RandomizerGroupSet* originalCache, struct RandomizerGroupSet* targetCache)
{
    return originalCache->maxGroup >= targetCache->minGroup
        && originalCache->minGroup <= targetCache->maxGroup;
}

#if RANDOMIZER_DYNAMIC_SPECIES == TRUE
// Once the randomizer is set up, in RAM mode, you can call this to preload the tables.
// This avoids a short hitch the first time the randomizer is used.
// It's also a good idea to call this function if the randomizer mode changes.
void PreloadRandomizationTables(void);
#endif

#endif // RANDOMIZER_AVAILABLE

#endif // GUARD_RANDOMIZER_H
