#include <iostream>
#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "decoder.hpp"
#include "instruction.hpp"

TEST(DecoderTest, HandlesZeroInput) {
  std::cout << "AAAAAAAAAAAAAAAAAAAAA TEST\n";
  EXPECT_EQ(1, 1);
}


int main(int argc, char **argv)
{
  ::testing::InitGoogleTest(&argc, argv);

  return RUN_ALL_TESTS();
}