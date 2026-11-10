#include "pch.h"
#include "tmathvector.h"

TEST(ClassMathVector, can_create_default) {
    TMathVector<double> v;
    EXPECT_EQ(0u, v.size());
}

TEST(ClassMathVector, can_create_with_size_and_default_val) {
    TMathVector<double> v(5, 0, 3.14);
    EXPECT_EQ(5u, v.size());
    for (size_t i = 0; i < v.size(); ++i) {
        EXPECT_DOUBLE_EQ(3.14, v[i]);
    }
}

TEST(ClassMathVector, can_create_with_initializer_list) {
    TMathVector<double> v({ 1.0, 2.0, 3.0 });
    EXPECT_EQ(3u, v.size());
    EXPECT_DOUBLE_EQ(1.0, v[0]);
    EXPECT_DOUBLE_EQ(2.0, v[1]);
    EXPECT_DOUBLE_EQ(3.0, v[2]);
}

TEST(ClassMathVector, can_create_from_raw_array) {
    double arr[3] = { 4.0, 5.0, 6.0 };
    TMathVector<double> v(arr, 3);
    EXPECT_EQ(3u, v.size());
    EXPECT_DOUBLE_EQ(4.0, v[0]);
    EXPECT_DOUBLE_EQ(6.0, v[2]);
}

TEST(ClassMathVector, can_create_copy) {
    TMathVector<double> v1({ 1.0, 2.0, 3.0 });
    TMathVector<double> v2(v1);
    EXPECT_EQ(v1, v2);
}

TEST(ClassMathVector, can_create_move) {
    TMathVector<double> v1({ 1.0, 2.0, 3.0 });
    TMathVector<double> v2(std::move(v1));
    EXPECT_EQ(3u, v2.size());
    EXPECT_DOUBLE_EQ(1.0, v2[0]);
    EXPECT_EQ(0u, v1.size());
}

TEST(ClassMathVector, equality_and_inequality) {
    TMathVector<double> v1({ 1.0, 2.0, 3.0 });
    TMathVector<double> v2({ 1.0, 2.0, 3.0 });
    TMathVector<double> v3({ 1.0, 2.0, 4.0 });
    TMathVector<double> v4({ 1.0, 2.0 });

    EXPECT_TRUE(v1 == v2);
    EXPECT_FALSE(v1 != v2);
    EXPECT_TRUE(v1 != v3);
    EXPECT_TRUE(v1 != v4);
}

TEST(ClassMathVector, unary_minus) {
    TMathVector<double> v({ 1.0, -2.5, 3.0 });
    TMathVector<double> neg = -v;
    EXPECT_DOUBLE_EQ(-1.0, neg[0]);
    EXPECT_DOUBLE_EQ(2.5, neg[1]);
    EXPECT_DOUBLE_EQ(-3.0, neg[2]);
}

TEST(ClassMathVector, addition_operator) {
    TMathVector<double> v1({ 1.0, 2.0, 3.0 });
    TMathVector<double> v2({ 4.0, 5.0, 6.0 });
    TMathVector<double> sum = v1 + v2;

    EXPECT_DOUBLE_EQ(5.0, sum[0]);
    EXPECT_DOUBLE_EQ(7.0, sum[1]);
    EXPECT_DOUBLE_EQ(9.0, sum[2]);
}

TEST(ClassMathVector, throw_when_add_different_sizes) {
    TMathVector<double> v1({ 1.0, 2.0 });
    TMathVector<double> v2({ 1.0, 2.0, 3.0 });
    EXPECT_THROW(v1 + v2, const char*);
}

TEST(ClassMathVector, subtraction_operator) {
    TMathVector<double> v1({ 5.0, 7.0, 9.0 });
    TMathVector<double> v2({ 1.0, 2.0, 3.0 });
    TMathVector<double> diff = v1 - v2;

    EXPECT_DOUBLE_EQ(4.0, diff[0]);
    EXPECT_DOUBLE_EQ(5.0, diff[1]);
    EXPECT_DOUBLE_EQ(6.0, diff[2]);
}

TEST(ClassMathVector, throw_when_subtract_different_sizes) {
    TMathVector<double> v1({ 1.0, 2.0 });
    TMathVector<double> v2({ 1.0, 2.0, 3.0 });
    EXPECT_THROW(v1 - v2, const char*);
}

TEST(ClassMathVector, multiplication_by_scalar) {
    TMathVector<double> v({ 1.0, -2.0, 3.0 });
    TMathVector<double> res1 = v * 2.0;
    TMathVector<double> res2 = 2.0 * v;

    EXPECT_DOUBLE_EQ(2.0, res1[0]);
    EXPECT_DOUBLE_EQ(-4.0, res1[1]);
    EXPECT_DOUBLE_EQ(6.0, res1[2]);
    EXPECT_EQ(res1, res2);
}

TEST(ClassMathVector, division_by_scalar) {
    TMathVector<double> v({ 2.0, -4.0, 6.0 });
    TMathVector<double> res = v / 2.0;

    EXPECT_DOUBLE_EQ(1.0, res[0]);
    EXPECT_DOUBLE_EQ(-2.0, res[1]);
    EXPECT_DOUBLE_EQ(3.0, res[2]);
}

TEST(ClassMathVector, throw_when_division_by_zero) {
    TMathVector<double> v({ 1.0, 2.0 });
    EXPECT_THROW(v / 0.0, const char*);
}

TEST(ClassMathVector, dot_product) {
    TMathVector<double> v1({ 1.0, 2.0, 3.0 });
    TMathVector<double> v2({ 4.0, 5.0, 6.0 });
    // 1*4 + 2*5 + 3*6 = 4 + 10 + 18 = 32
    double dot = v1 * v2;
    EXPECT_DOUBLE_EQ(32.0, dot);
}

TEST(ClassMathVector, throw_when_dot_product_different_sizes) {
    TMathVector<double> v1({ 1.0, 2.0 });
    TMathVector<double> v2({ 1.0, 2.0, 3.0 });
    EXPECT_THROW(v1 * v2, const char*);
}

TEST(ClassMathVector, vector_length) {
    TMathVector<double> v({ 3.0, 4.0 });
    EXPECT_DOUBLE_EQ(5.0, v.length());
}

TEST(ClassMathVector, stream_output) {
    TMathVector<int> v({ 1, 2, 3 });
    std::stringstream ss;
    ss << v;
    EXPECT_EQ("(1, 2, 3)", ss.str());
}

TEST(ClassMathVector, stream_input) {
    TMathVector<int> v(3);
    std::stringstream ss("10 20 30");
    ss >> v;
    EXPECT_EQ(10, v[0]);
    EXPECT_EQ(20, v[1]);
    EXPECT_EQ(30, v[2]);
}

TEST(ClassMathVector, can_create_with_start_index) {
    TMathVector<int> v(3, 2, 7); // size 3 start_index = 2 values 7
    EXPECT_EQ(3u, v.size());
    EXPECT_EQ(2u, v.start_index());

    // before start_index returns 0
    EXPECT_EQ(0, v[0]);
    EXPECT_EQ(0, v[1]);

    // from start_index there are vector elements
    EXPECT_EQ(7, v[2]);
    EXPECT_EQ(7, v[3]);
    EXPECT_EQ(7, v[4]);

    // out of range
    EXPECT_THROW(v[5], const char*);
}

TEST(ClassMathVector, equality_checks_start_index) {
    TMathVector<int> v1({ 1, 2, 3 }, 0);
    TMathVector<int> v2({ 1, 2, 3 }, 1); // same elements but different shift
    EXPECT_FALSE(v1 == v2);
    EXPECT_TRUE(v1 != v2);
}

TEST(ClassMathVector, throw_when_add_with_different_start_index) {
    TMathVector<int> v1(3, 0, 1);
    TMathVector<int> v2(3, 1, 1);
    EXPECT_THROW(v1 + v2, const char*);
}

TEST(ClassMathVector, throw_when_dot_product_different_start_index) {
    TMathVector<int> v1(3, 0, 1);
    TMathVector<int> v2(3, 2, 1);
    EXPECT_THROW(v1 * v2, const char*);
}