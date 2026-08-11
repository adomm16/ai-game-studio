#pragma once

// Intentionally Unreal-free. This header can be compiled by any C++17 toolchain.
#include <algorithm>
#include <cstdint>
#include <vector>

namespace AshfallSim
{
enum class Job : std::uint8_t { Idle, Food, Timber, Metal };
enum class SoldierType : std::uint8_t { Spear, Ranged, Scout };

struct Resources
{
    std::int32_t Food = 0;
    std::int32_t Timber = 0;
    std::int32_t Metal = 0;
    std::int32_t Authority = 0;
};

struct Household
{
    std::int32_t Id = -1;
    Job AssignedJob = Job::Idle;
    bool Alive = true;
    bool Mustered = false;
};

struct Army
{
    std::int32_t Id = -1;
    SoldierType Type = SoldierType::Spear;
    std::int32_t ProvinceId = 0;
    std::int32_t DestinationProvinceId = -1;
    std::int32_t TravelTicksRemaining = 0;
    std::vector<std::int32_t> HouseholdIds;
};

inline Resources ProductionForTick(const std::vector<Household>& Households)
{
    Resources Result;
    for (const Household& Entry : Households)
    {
        if (!Entry.Alive || Entry.Mustered)
        {
            continue;
        }

        switch (Entry.AssignedJob)
        {
        case Job::Food: Result.Food += 2; break;
        case Job::Timber: Result.Timber += 2; break;
        case Job::Metal: Result.Metal += 1; break;
        default: break;
        }
    }
    return Result;
}

inline bool CanMuster(const std::vector<Household>& Households,
                      const std::vector<std::int32_t>& HouseholdIds)
{
    if (HouseholdIds.empty())
    {
        return false;
    }
    std::vector<std::int32_t> SeenIds;
    for (std::int32_t Id : HouseholdIds)
    {
        if (std::find(SeenIds.begin(), SeenIds.end(), Id) != SeenIds.end())
        {
            return false;
        }
        SeenIds.push_back(Id);
        const auto It = std::find_if(Households.begin(), Households.end(),
            [Id](const Household& Entry) { return Entry.Id == Id; });
        if (It == Households.end() || !It->Alive || It->Mustered)
        {
            return false;
        }
    }
    return true;
}

inline void SetMustered(std::vector<Household>& Households,
                        const std::vector<std::int32_t>& HouseholdIds)
{
    for (Household& Entry : Households)
    {
        if (std::find(HouseholdIds.begin(), HouseholdIds.end(), Entry.Id) != HouseholdIds.end())
        {
            Entry.Mustered = true;
        }
    }
}

inline void AdvanceArmy(Army& Company)
{
    if (Company.TravelTicksRemaining <= 0)
    {
        return;
    }
    --Company.TravelTicksRemaining;
    if (Company.TravelTicksRemaining == 0)
    {
        Company.ProvinceId = Company.DestinationProvinceId;
        Company.DestinationProvinceId = -1;
    }
}

inline std::vector<std::int32_t> ApplyCasualties(std::vector<Household>& Households,
                                                 Army& Company,
                                                 std::int32_t RequestedCasualties)
{
    const std::int32_t Count = std::clamp(RequestedCasualties, 0,
        static_cast<std::int32_t>(Company.HouseholdIds.size()));
    std::vector<std::int32_t> LostIds;
    for (std::int32_t Index = 0; Index < Count; ++Index)
    {
        const std::int32_t Id = Company.HouseholdIds.back();
        Company.HouseholdIds.pop_back();
        LostIds.push_back(Id);
        const auto It = std::find_if(Households.begin(), Households.end(),
            [Id](const Household& Entry) { return Entry.Id == Id; });
        if (It != Households.end())
        {
            It->Alive = false;
            It->Mustered = false;
        }
    }
    return LostIds;
}

inline std::uint32_t NextRandom(std::uint32_t& State)
{
    State = State * 1664525u + 1013904223u;
    return State;
}

inline std::int32_t ResolveCasualties(std::int32_t AttackerSize,
                                      std::int32_t DefenderSize,
                                      std::uint32_t& Seed)
{
    if (AttackerSize <= 0 || DefenderSize <= 0)
    {
        return 0;
    }
    const std::int32_t Pressure = std::max(1, DefenderSize - AttackerSize / 2);
    return 1 + static_cast<std::int32_t>(NextRandom(Seed) % static_cast<std::uint32_t>(Pressure));
}
}
