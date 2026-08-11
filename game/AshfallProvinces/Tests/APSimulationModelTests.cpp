#include "../Source/AshfallProvinces/Public/Simulation/APSimulationModel.h"
#include <cassert>

int main()
{
    using namespace AshfallSim;

    std::vector<Household> Households;
    for (std::int32_t Id = 0; Id < 24; ++Id)
    {
        const Job Assigned = Id < 8 ? Job::Food : (Id < 16 ? Job::Timber : Job::Metal);
        Households.push_back({Id, Assigned, true, false});
    }

    const Resources BeforeMuster = ProductionForTick(Households);
    assert(BeforeMuster.Food == 16);
    assert(BeforeMuster.Timber == 16);
    assert(BeforeMuster.Metal == 8);

    const std::vector<std::int32_t> Recruits = {0, 1, 8, 16};
    assert(CanMuster(Households, Recruits));
    assert(!CanMuster(Households, {0, 0}));
    SetMustered(Households, Recruits);
    const Resources AfterMuster = ProductionForTick(Households);
    assert(AfterMuster.Food == 12);
    assert(AfterMuster.Timber == 14);
    assert(AfterMuster.Metal == 7);

    Army Company{0, SoldierType::Spear, 0, 1, 2, Recruits};
    AdvanceArmy(Company);
    assert(Company.ProvinceId == 0 && Company.TravelTicksRemaining == 1);
    AdvanceArmy(Company);
    assert(Company.ProvinceId == 1 && Company.DestinationProvinceId == -1);

    const std::vector<std::int32_t> Lost = ApplyCasualties(Households, Company, 1);
    assert(Lost.size() == 1 && Lost.front() == 16);
    assert(!Households[16].Alive && !Households[16].Mustered);
    const Resources AfterCasualty = ProductionForTick(Households);
    assert(AfterCasualty.Metal == 7); // A dead household cannot return to production.

    std::uint32_t SeedA = 42;
    std::uint32_t SeedB = 42;
    assert(ResolveCasualties(4, 3, SeedA) == ResolveCasualties(4, 3, SeedB));
    assert(SeedA == SeedB);
    return 0;
}
