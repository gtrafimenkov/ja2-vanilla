#include "Strategic/CampaignTypes.h"
#include "Strategic/TownMilitia.h"

int main() {
    if (SoldierClassToMilitiaRank(SOLDIER_CLASS_GREEN_MILITIA) != GREEN_MILITIA) {
        return 10;
    };
    return 0;
}
