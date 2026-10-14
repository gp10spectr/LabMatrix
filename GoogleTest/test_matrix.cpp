#include "pch.h"
#include "tmatrix.h"

TEST(ClassMatrix, can_create_from_2d_initializer) {
    TMatrix<int> m({{1, 2, 3},
        {4, 5, 6}});
    EXPECT_EQ(2u, m.rows());
    EXPECT_EQ(3u, m.cols());
    EXPECT_EQ(1, m[0][0]);
    EXPECT_EQ(6, m[1][2]);
}

TEST(ClassMatrix, throws_on_nonuniform_2d_initializer) {
    EXPECT_THROW(
        TMatrix<int>({{1, 2, 3},
            {4, 5}}),
        const char*
    );
}

TEST(ClassMatrix, can_create_sized_matrix_with_value) {
    TMatrix<double> m(3, 4, 2.5);
    EXPECT_EQ(3u, m.rows());
    EXPECT_EQ(4u, m.cols());
    for (size_t i = 0; i < 3; i++)
        for (size_t j = 0; j < 4; j++)
            EXPECT_DOUBLE_EQ(2.5, m[i][j]);
}

TEST(ClassMatrix, can_transpose) {
    TMatrix<int> m({{1, 2, 3},
        {4, 5, 6}});
    auto t = m.transpose();
    EXPECT_EQ(3u, t.rows());
    EXPECT_EQ(2u, t.cols());
    EXPECT_EQ(1, t[0][0]);
    EXPECT_EQ(4, t[0][1]);
    EXPECT_EQ(3, t[2][0]);
    EXPECT_EQ(6, t[2][1]);
}

TEST(ClassMatrix, transpose_twice_returns_original) {
    TMatrix<int> m({{1, 2, 3},
        {4, 5, 6}});
    auto tt = m.transpose().transpose();
    for (size_t i = 0; i < m.rows(); i++)
        for (size_t j = 0; j < m.cols(); j++)
            EXPECT_EQ(m[i][j], tt[i][j]);
}

TEST(ClassMatrix, can_trace) {
    TMatrix<int> m({{1, 2, 3},
        {4, 5, 6},
        {7, 8, 9}});
    EXPECT_EQ(15, m.trace());   // 1 + 5 + 9
}

TEST(ClassMatrix, trace_throws_on_non_square) {
    TMatrix<int> m({{1, 2, 3},
        {4, 5, 6}});
    EXPECT_THROW(m.trace(), const char*);
}

TEST(ClassMatrix, can_determinant_1x1) {
    TMatrix<int> m({ { 7 } });
    EXPECT_EQ(7, m.determinant());
}

TEST(ClassMatrix, can_determinant_2x2) {
    TMatrix<int> m({{1, 2},
        {3, 4}});
    EXPECT_EQ(-2, m.determinant());
}

TEST(ClassMatrix, can_determinant_3x3) {
    TMatrix<int> m({{1, 2, 3},
        {4, 5, 6},
        {7, 8, 10}});
    // det = -3
    EXPECT_EQ(-3, m.determinant());
}

TEST(ClassMatrix, determinant_throws_on_non_square) {
    TMatrix<int> m({{1, 2, 3},
        {4, 5, 6}});
    EXPECT_THROW(m.determinant(), const char*);
}

TEST(ClassMatrix, can_multiply_matrices) {
    TMatrix<int> a({{1, 2},
        {3, 4}});
    TMatrix<int> b({{5, 6},
        {7, 8}});
    auto c = a * b;
    EXPECT_EQ(19, c[0][0]);  // 1*5 + 2*7
    EXPECT_EQ(22, c[0][1]);  // 1*6 + 2*8
    EXPECT_EQ(43, c[1][0]);  // 3*5 + 4*7
    EXPECT_EQ(50, c[1][1]);  // 3*6 + 4*8
}

TEST(ClassMatrix, multiply_throws_on_dimension_mismatch) {
    TMatrix<int> a(2, 3);
    TMatrix<int> b(2, 2);
    EXPECT_THROW(a * b, const char*);
}

TEST(ClassMatrix, can_multiply_assign_matrices) {
    TMatrix<int> a({{1, 2},
        {3, 4}});
    TMatrix<int> b({{2, 0},
        {0, 2}});
    a *= b;
    EXPECT_EQ(2, a[0][0]);
    EXPECT_EQ(4, a[0][1]);
    EXPECT_EQ(6, a[1][0]);
    EXPECT_EQ(8, a[1][1]);
}

TEST(ClassMatrix, can_output) {
    TMatrix<int> m({{1, 2},
        {3, 4}});
    std::stringstream ss;
    ss << m;
    EXPECT_EQ("[ 1 2 ]\n[ 3 4 ]", ss.str());
}