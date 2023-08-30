#include <gtest/gtest.h>
#include <navigation/nav.hpp>

// Demonstrate some basic assertions.
TEST(nav, dummy) {
  // Expect two strings not to be equal.
  EXPECT_STRNE("hello", "world");
  // Expect equality.
  EXPECT_EQ(7 * 6, 42);
}

// TEST(squaternion, create) {
//   Quaternion q;
//   EXPECT_TRUE(q.w == 1.0);
//   EXPECT_TRUE(q.x + q.y + q.z == 0.0);

//   // this will never (?) be arduino?
//   // #ifdef ARDUINO
//   // EXPECT_TRUE(sizeof(q.w) == sizeof(float));
//   // #else
//   EXPECT_TRUE(sizeof(q.w) == sizeof(double));
//   // #endif

//   Quaternion qq(1, 2, 3, 4);
//   EXPECT_EQ(qq.w, 1.0);
//   EXPECT_EQ(qq.x, 2.0);
//   EXPECT_EQ(qq.y, 3.0);
//   EXPECT_EQ(qq.z, 4.0);
// }

// TEST(squaternion, math) {
//   Quaternion q;
//   q = q * 2.0;
//   EXPECT_EQ(q.w, 2.0);

//   Quaternion qq(1, 2, 3, 4);
//   qq.normalize();
//   q = 2.0 * qq / 2.0;
//   ASSERT_DOUBLE_EQ(q.magnitude(), 1.0);
// }
