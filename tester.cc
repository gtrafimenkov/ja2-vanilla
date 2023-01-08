#include "gtest/gtest.h"

extern "C"
{
#include "Strategic/CampaignTypes.h"
#include "Strategic/TownMilitia.h"
}

TEST(TownMilitia, SoldierClassToMilitiaRank)
{
    EXPECT_EQ(SoldierClassToMilitiaRank(SOLDIER_CLASS_GREEN_MILITIA), GREEN_MILITIA);
    EXPECT_EQ(SoldierClassToMilitiaRank(SOLDIER_CLASS_REG_MILITIA), REGULAR_MILITIA);
    EXPECT_EQ(SoldierClassToMilitiaRank(SOLDIER_CLASS_ELITE_MILITIA), ELITE_MILITIA);
    EXPECT_EQ(SoldierClassToMilitiaRank(200), -1);
}

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
