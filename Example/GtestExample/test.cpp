#include <gtest/gtest.h>
#include "person.h"

TEST(PersonTest, GetName) {
    Person person("Alice", 30);
    EXPECT_EQ(person.getName(), "Alice");
}

TEST(PersonTest, GetAge) {
    Person person("Bob", 25);
    EXPECT_EQ(person.getAge(), 25);
}


int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
