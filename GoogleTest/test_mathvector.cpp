#include "pch.h"
#include "tmathvector.h"

TEST(ClassMathVector, can_create_empty) {
    TMathVector<double> v;
    EXPECT_EQ(0u, v.size());
}

TEST(ClassMathVector, can_create_with_size) {
    TMathVector<double> v(5);
    EXPECT_EQ(5u, v.size());
    for (size_t i = 0; i < v.size(); ++i)
        EXPECT_DOUBLE_EQ(0.0, v[i]);
}

TEST(ClassMathVector, can_create_with_initializer_list) {
    TMathVector<double> v({ 1.0, 2.0, 3.0 });
    EXPECT_EQ(3u, v.size());
    EXPECT_DOUBLE_EQ(1.0, v[0]);
    EXPECT_DOUBLE_EQ(3.0, v[2]);
}

TEST(ClassMathVector, can_create_from_tvector) {
    TVector<double> v({ 1.0, 2.0, 3.0 });
    TMathVector<double> m(v);
    EXPECT_EQ(3u, m.size());
    EXPECT_DOUBLE_EQ(1.0, m[0]);
    EXPECT_DOUBLE_EQ(3.0, m[2]);
}

TEST(ClassMathVector, can_add_assign) {
    TMathVector<double> a({ 1, 2, 3 });
    TMathVector<double> b({ 4, 5, 6 });
    a += b;
    EXPECT_DOUBLE_EQ(5.0, a[0]);
    EXPECT_DOUBLE_EQ(7.0, a[1]);
    EXPECT_DOUBLE_EQ(9.0, a[2]);
}

TEST(ClassMathVector, throws_on_add_size_mismatch) {
    TMathVector<double> a({ 1, 2, 3 });
    TMathVector<double> b({ 1, 2 });
    EXPECT_THROW(a += b, const char*);
}

TEST(ClassMathVector, can_subtract_assign) {
    TMathVector<double> a({ 5, 6, 7 });
    TMathVector<double> b({ 1, 2, 3 });
    a -= b;
    EXPECT_DOUBLE_EQ(4.0, a[0]);
    EXPECT_DOUBLE_EQ(4.0, a[2]);
}

TEST(ClassMathVector, can_add) {
    TMathVector<double> a({ 1, 2, 3 });
    TMathVector<double> b({ 4, 5, 6 });
    auto c = a + b;
    EXPECT_EQ(3u, c.size());
    EXPECT_DOUBLE_EQ(5.0, c[0]);
    EXPECT_DOUBLE_EQ(9.0, c[2]);
    EXPECT_DOUBLE_EQ(1.0, a[0]);   // a не изменился
    EXPECT_DOUBLE_EQ(4.0, b[0]);   // b не изменился
}

TEST(ClassMathVector, can_subtract) {
    TMathVector<double> a({ 5, 6, 7 });
    TMathVector<double> b({ 1, 2, 3 });
    auto c = a - b;
    EXPECT_DOUBLE_EQ(4.0, c[0]);
    EXPECT_DOUBLE_EQ(4.0, c[1]);
    EXPECT_DOUBLE_EQ(4.0, c[2]);
}

TEST(ClassMathVector, can_multiply_by_scalar) {
    TMathVector<double> a({ 1, 2, 3 });
    auto b = a * 2.0;
    auto c = 3.0 * a;
    EXPECT_DOUBLE_EQ(2.0, b[0]);
    EXPECT_DOUBLE_EQ(6.0, b[2]);
    EXPECT_DOUBLE_EQ(3.0, c[0]);
    EXPECT_DOUBLE_EQ(9.0, c[2]);
    EXPECT_DOUBLE_EQ(1.0, a[0]);   // a не изменился
}

TEST(ClassMathVector, can_divide_by_scalar) {
    TMathVector<double> a({ 2, 4, 6 });
    auto b = a / 2.0;
    EXPECT_DOUBLE_EQ(1.0, b[0]);
    EXPECT_DOUBLE_EQ(3.0, b[2]);
}

TEST(ClassMathVector, can_dot_product) {
    TMathVector<double> a({ 1, 2, 3 });
    TMathVector<double> b({ 4, 5, 6 });
    EXPECT_DOUBLE_EQ(32.0, a.dot(b));   // 4 + 10 + 18
}

TEST(ClassMathVector, dot_throws_on_size_mismatch) {
    TMathVector<double> a({ 1, 2 });
    TMathVector<double> b({ 1, 2, 3 });
    EXPECT_THROW(a.dot(b), const char*);
}

TEST(ClassMathVector, can_compute_norm) {
    TMathVector<double> a({ 3, 4 });
    EXPECT_DOUBLE_EQ(5.0, a.norm());

    TMathVector<double> b({ 1, 0, 0 });
    EXPECT_DOUBLE_EQ(1.0, b.norm());

    TMathVector<double> c({ 0, 0, 0 });
    EXPECT_DOUBLE_EQ(0.0, c.norm());
}

// Проверяем, что TMathVector работает как основа для матриц:
// T = TMathVector<int> (вектор векторов)
TEST(ClassMathVector, works_as_matrix_base) {
    using Row = TMathVector<int>;
    TMathVector<Row> m;
    m.push_back(Row({ 1, 2, 3 }));
    m.push_back(Row({ 4, 5, 6 }));

    EXPECT_EQ(2u, m.size());
    EXPECT_EQ(3u, m[0].size());
    EXPECT_EQ(1, m[0][0]);
    EXPECT_EQ(6, m[1][2]);

    TMathVector<Row> m2;
    m2.push_back(Row({ 1, 1, 1 }));
    m2.push_back(Row({ 1, 1, 1 }));

    m += m2;
    EXPECT_EQ(2, m[0][0]);
    EXPECT_EQ(7, m[1][2]);

    m *= 2;
    EXPECT_EQ(4, m[0][0]);
    EXPECT_EQ(14, m[1][2]);
}
