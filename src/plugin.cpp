

#include "include\PerkEntryPointExtenderAPI.h"

struct Hooks {
    struct CommandedActorLimitHook {
        static void thunk(RE::PerkEntryPoint entry_point, RE::Actor* target, RE::MagicItem* a_spell, void* out) {
            float* floatPtr = static_cast<float*>(out);
            *floatPtr = 999.0f;  // If you need more than 999 summons, I think you've got a problem

        }
        static inline REL::Relocation<decltype(thunk)> func;
    };

    struct CommandedActorHook {
        static void thunk(RE::AIProcess* test, RE::ActiveEffectReferenceEffectController* target2, void* target3) {
            func(test, target2, target3);
            auto a_AE = target2->effect;
            std::vector<int> indexarray;
            std::vector<int> indexarrayuntyped;
            std::vector<int> indexarrayworking;
            std::vector<int> indexarrayworking2;
            std::vector<float> indexarrayworkingfloat;
            std::vector<int> indexarrayworkinguntyped;
            std::vector<int> indexarrayworking2untyped;
            std::vector<float> indexarrayworkingfloatuntyped;

            float perkfactor = 0.0f;
            int j = 0;
            int n = 0;
            int reanimated = 0;
            int accountedfor = 0;
            if (a_AE && test->middleHigh->perkData) {  

                auto akCastedMagic = a_AE->spell;
                RE::SummonCreatureEffect* summonedeffect;
                RE::ReanimateEffect* reanimatedeffect;
                RE::CommandEffect* commandedeffect;
                auto mid = test->middleHigh->commandedActors;
                auto summoner = test->GetUserData();
                RE::Actor* summonedactor;
                std::string_view a_editorID;

                if (a_AE->effect->baseEffect->HasArchetype(RE::EffectArchetypes::ArchetypeID::kSummonCreature)) {
                    summonedeffect = reinterpret_cast<RE::SummonCreatureEffect*>(a_AE);
                    if (summonedeffect) {
                        a_editorID = "MagicSummon";

                        summonedactor = summonedeffect->commandedActor.get().get();
                    }
                } else if (a_AE->effect->baseEffect->HasArchetype(RE::EffectArchetypes::ArchetypeID::kReanimate)) {
                    reanimatedeffect = reinterpret_cast<RE::ReanimateEffect*>(a_AE);
                    if (reanimatedeffect) {
                        a_editorID = "MagicSummon";
                        reanimated = 1;
                        summonedactor = reanimatedeffect->commandedActor.get().get();
                    }
                } else if (a_AE->effect->baseEffect->HasArchetype(
                               RE::EffectArchetypes::ArchetypeID::kCommandSummoned)) {
                    commandedeffect = reinterpret_cast<RE::CommandEffect*>(a_AE);
                    if (commandedeffect) {
                        a_editorID = "MagicCommand";

                        summonedactor = commandedeffect->commandedActor.get().get();
                    }
                }

                std::unordered_map<std::string, float> keywordmap;
                perkfactor = 1.0f;
                RE::BGSEntryPoint::HandleEntryPoint(RE::PerkEntryPoint::kModCommandedActorLimit, summoner,
                                                    akCastedMagic, &perkfactor);
                float perkfactorcapped = 0.0f;
                RE::HandleEntryPoint(RE::PerkEntryPoint::kModSpellCost, summoner, &perkfactorcapped,
                                     "VanillaCapped", 3,
                                     {akCastedMagic});
                float reanimatedbonus = 0.0f;
                float nonreanimatedbonus = 0.0f;
                RE::HandleEntryPoint(RE::PerkEntryPoint::kModSpellCost, summoner, &reanimatedbonus, "VanillaCapped", 4,
                                     {akCastedMagic});

                RE::HandleEntryPoint(RE::PerkEntryPoint::kModSpellCost, summoner, &nonreanimatedbonus, "VanillaCapped",
                                     5, {akCastedMagic});


                if (perkfactor >= 1.0f && (perkfactor + perkfactorcapped < 1.0f)) {
                
                    perkfactor = 1.0f;
                } else {
                    perkfactor += perkfactorcapped;
                }
                // char buffer[150];
                //sprintf_s(buffer, "perkfactor: %f", perkfactor);
                // RE::ConsoleLog::GetSingleton()->Print(buffer);
                keywordmap["untyped"] = perkfactor;
                for (auto& elements : mid) {
                    if (mid[j].activeEffect) {
                        indexarrayworking.push_back(j);
                        indexarrayworkingfloat.push_back(mid[j].activeEffect->elapsedSeconds+0.01f);
                        //char buffer1[150];
                        //sprintf_s(buffer1, "elapsed second: %f", mid[j].activeEffect->elapsedSeconds);
                        //RE::ConsoleLog::GetSingleton()->Print(buffer1);
                    }
                    j += 1;
                }
                for (std::uint32_t widx = 0; widx < indexarrayworking.size(); ++widx) {
                        auto maxtime = std::max_element(indexarrayworkingfloat.begin(), indexarrayworkingfloat.end());
                        float maxvalue = *maxtime;
                        auto iter = (std::find(indexarrayworkingfloat.begin(), indexarrayworkingfloat.end(), maxvalue));
                        auto index = std::distance(indexarrayworkingfloat.begin(), iter);
                        if (indexarrayworking2.empty()) {
                        indexarrayworking2.push_back(indexarrayworking[index]);
                        } else {
                        indexarrayworking2.insert(indexarrayworking2.begin(), indexarrayworking[index]);
                        }
                        indexarrayworkingfloat[index] = 0.0f;
                        //char buffer[150];
                        //sprintf_s(buffer, "widx first: %i", widx);
                        //RE::ConsoleLog::GetSingleton()->Print(buffer);
                }
                //float antiperkfactor = 0.0f;
                //float antiperktotal = 0.0f;
                for (std::uint32_t widx = 0; widx < indexarrayworking2.size(); ++widx) {
                        auto element = mid[indexarrayworking2[widx]];
                        //mid.size();
                        accountedfor = 0;
                        //char buffer[150];
                        //sprintf_s(buffer, "widx second: %i", widx);
                        //RE::ConsoleLog::GetSingleton()->Print(buffer);
                        //char buffer1[150];
                        //sprintf_s(buffer1, "indexarrayworking second: %i", indexarrayworking2[widx]);
                        //RE::ConsoleLog::GetSingleton()->Print(buffer1);
                        //antiperktotal = 0.0f;

                        for (std::uint32_t idx = 0; idx < element.activeEffect->effect->baseEffect->numKeywords;
                             ++idx) {
                        //char buffer[150];
                        //sprintf_s(buffer, "perkfactor2: %f", perkfactor);
                        //RE::ConsoleLog::GetSingleton()->Print(
                            //element.activeEffect->effect->baseEffect->keywords[idx]->formEditorID.c_str());

                            if (element.activeEffect->effect->baseEffect->keywords[idx]->formEditorID.contains(
                                    a_editorID)) {
                                auto testkey =
                                    element.activeEffect->effect->baseEffect->keywords[idx]->formEditorID.c_str();

                                //antiperkfactor = 0.0f;
                                //RE::HandleEntryPoint(
                                //    RE::PerkEntryPoint::kModSpellCost, summoner, &antiperkfactor,
                                //    element.activeEffect->effect->baseEffect->keywords[idx]->formEditorID.c_str(), 4,
                                //    {element.activeEffect->spell});
                                //antiperktotal += antiperkfactor;
                                if (!keywordmap.contains(testkey)) {
                                    if (element.activeEffect->effect->baseEffect->keywords[idx]->formEditorID.contains(
                                            "BaseOne")) {
                                        perkfactor = 1.0f;
                                    } else {
                                        perkfactor = 0.0f;
                                    }
                                        
                                    RE::HandleEntryPoint(RE::PerkEntryPoint::kModSpellCost, summoner, &perkfactor, element.activeEffect->effect->baseEffect->keywords[idx]->formEditorID.c_str(), 3, {element.activeEffect->spell});
                                        keywordmap[testkey] = perkfactor;
                                    //char buffer[150];
                                    //sprintf_s(buffer, "perkfactor2: %f", perkfactor);
                                    //RE::ConsoleLog::GetSingleton()->Print(element.activeEffect->effect->baseEffect->keywords[idx]->formEditorID.c_str());
                                }
                                if (keywordmap[testkey] >= 1.0f && accountedfor == 0) {

                                        keywordmap[testkey] -= 1.0f;
                                        accountedfor = 1;
                                }

                            }
                        }





                        if (!element.activeEffect->effect->baseEffect->HasKeywordString("MagicSpecialConjuration")) {
                            if (element.activeEffect->effect->baseEffect->HasArchetype(
                                    RE::EffectArchetypes::ArchetypeID::kReanimate)) {
                                if (reanimatedbonus > 0.0f && accountedfor == 0) {
                                        reanimatedbonus -= 1.0f;
                                        accountedfor = 1;
                                        // RE::ConsoleLog::GetSingleton()->Print(
                                        //     element.activeEffect->effect->baseEffect->keywords[0]->formEditorID.c_str());
                                }
                            }
                            if (element.activeEffect->effect->baseEffect->HasArchetype(
                                    RE::EffectArchetypes::ArchetypeID::kSummonCreature)) {
                                if (nonreanimatedbonus > 0.0f && accountedfor == 0) {
                                        nonreanimatedbonus -= 1.0f;
                                        accountedfor = 1;
                                        // RE::ConsoleLog::GetSingleton()->Print(
                                        //     element.activeEffect->effect->baseEffect->keywords[0]->formEditorID.c_str());
                                }
                            }
                            if (keywordmap["untyped"] > 0.0f && accountedfor == 0) {
                                if (keywordmap["untyped"]> 0.0f) {
                                        keywordmap["untyped"] -= 1.0f;
                                        //if (antiperktotal
                                        //    >= 1.0f) 
                                        //{ antiperktotal -= 1.0f; }
                                        accountedfor = 1;                                
                                
                                }

                                //RE::ConsoleLog::GetSingleton()->Print(
                                //    element.activeEffect->effect->baseEffect->keywords[0]->formEditorID.c_str());

                            }

                        }

                        if (accountedfor == 0) {
                            indexarray.push_back(indexarrayworking2[widx]);
                            if (widx == 0) n = 0;
                            //RE::ConsoleLog::GetSingleton()->Print(
                            //    element.activeEffect->effect->baseEffect->keywords[0]->formEditorID.c_str());

                        }

                }

                if (indexarray.size() > 0) {
                        for (std::uint32_t widx = 0; widx < indexarray.size(); ++widx) {

                            mid[indexarray[widx]].activeEffect->Dispel(true);
                        }
                }

                if (n == 0 && reanimated == 0) {
                    std::vector<RE::SpellItem*> reanimateSpells;

                    //RE::BGSEntryPoint::HandleEntryPoint(RE::PerkEntryPoint::kApplyReanimateSpell, summoner,
                    //                                    akCastedMagic, summonedactor, &reanimateSpells); //This would be to use vanilla apply reanimate spell entry point

                     RE::HandleEntryPoint(RE::PerkEntryPoint::kApplyReanimateSpell, summoner,
                                          &reanimateSpells, "SummonSpell", 3, {akCastedMagic, summonedactor});
                    for (auto* reanimateSpell : reanimateSpells) {
                        if (reanimateSpell->IsPermanent()) {
                            summonedactor->AddSpell(reanimateSpell);
                        } else {
                            summoner->GetMagicCaster(RE::MagicSystem::CastingSource::kInstant)
                                ->CastSpellImmediate(reanimateSpell, false, summonedactor, 1.0F, false, 0.0F, nullptr);
                        }
                    }
                    std::vector<RE::SpellItem*> selfreanimateSpells;
                    RE::HandleEntryPoint(RE::PerkEntryPoint::kApplyReanimateSpell, summoner, &reanimateSpells,
                                         "SummonSpell", 4, {akCastedMagic, summonedactor});
                    for (auto* reanimateSpell : selfreanimateSpells) {
                        if (reanimateSpell->IsPermanent()) {
                            summoner->AddSpell(reanimateSpell);
                        } else {
                            summoner->GetMagicCaster(RE::MagicSystem::CastingSource::kInstant)
                                ->CastSpellImmediate(reanimateSpell, false, summoner, 1.0F, false, 0.0F, nullptr);
                        }
                    }



                }
            }

        }
        static inline REL::Relocation<decltype(thunk)> func;
    };
    static void Install() {
        REL::Relocation<std::uintptr_t> functionCommandedActorLimitHook{RELOCATION_ID(38993, 40056),
                                                                        REL::Relocate(0xA1, 0xEC)};
        stl::write_thunk_call<CommandedActorLimitHook>(functionCommandedActorLimitHook.address());

        REL::Relocation<std::uintptr_t> functionCommandedActorHook{RELOCATION_ID(38904, 39950),
                                                                   REL::Relocate(0x14B, 0x12B)};
        stl::write_thunk_call<CommandedActorHook>(functionCommandedActorHook.address());
    };
};


SKSEPluginLoad(const SKSE::LoadInterface* skse) {
    SKSE::Init(skse);
    Hooks::Install();
    return true;
}

