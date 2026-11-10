#include "pch.h"
#include "tmatrix.h"

TEST(ClassMatrixInherited, can_access_elements_via_brackets) {
    TMatrix<int> m = {
        { 1, 2, 3 },
        { 4, 5, 6 }
    };
    EXPECT_EQ(1, m[0][0]);
    EXPECT_EQ(3, m[0][2]);
    EXPECT_EQ(5, m[1][1]);

    m[1][1] = 99;
    EXPECT_EQ(99, m[1][1]);
}

TEST(ClassMatrixInherited, equality_and_inequality) {
    TMatrix<int> m1 = { { 1, 2 }, { 3, 4 } };
    TMatrix<int> m2 = { { 1, 2 }, { 3, 4 } };
    TMatrix<int> m3 = { { 1, 2 }, { 3, 5 } };

    EXPECT_TRUE(m1 == m2);
    EXPECT_FALSE(m1 != m2);
    EXPECT_TRUE(m1 != m3);
}

TEST(ClassMatrixInherited, unary_minus) {
    TMatrix<int> m = {
        { 1, -2 },
        { -3, 4 }
    };
    TMatrix<int> neg = -m;
    EXPECT_EQ(-1, neg[0][0]);
    EXPECT_EQ(2, neg[0][1]);
    EXPECT_EQ(3, neg[1][0]);
    EXPECT_EQ(-4, neg[1][1]);
}

TEST(ClassMatrixInherited, matrix_addition) {
    TMatrix<int> m1 = {
        { 1, 2, 3 },
        { 4, 5, 6 }
    };
    TMatrix<int> m2 = {
        { 10, 20, 30 },
        { 40, 50, 60 }
    };
    TMatrix<int> res = m1 + m2;

    EXPECT_EQ(11, res[0][0]);
    EXPECT_EQ(22, res[0][1]);
    EXPECT_EQ(33, res[0][2]);
    EXPECT_EQ(44, res[1][0]);
    EXPECT_EQ(55, res[1][1]);
    EXPECT_EQ(66, res[1][2]);
}

TEST(ClassMatrixInherited, matrix_subtraction) {
    TMatrix<int> m1 = { { 5, 7 }, { 9, 11 } };
    TMatrix<int> m2 = { { 1, 2 }, { 3, 4 } };
    TMatrix<int> res = m1 - m2;

    EXPECT_EQ(4, res[0][0]);
    EXPECT_EQ(5, res[0][1]);
    EXPECT_EQ(6, res[1][0]);
    EXPECT_EQ(7, res[1][1]);
}

TEST(ClassMatrixInherited, throw_when_add_different_dimensions) {
    TMatrix<int> m1 = { { 1, 2 } };
    TMatrix<int> m2 = { { 1, 2 }, { 3, 4 } };
    EXPECT_THROW(m1 + m2, const char*);
}

TEST(ClassMatrixNonInherited, constructor_rows_cols) {
    TMatrix<double> m(2, 3, 1.5);
    EXPECT_EQ(2u, m.rows());
    EXPECT_EQ(3u, m.cols());
    for (size_t i = 0; i < m.rows(); ++i) {
        for (size_t j = 0; j < m.cols(); ++j) {
            EXPECT_DOUBLE_EQ(1.5, m[i][j]);
        }
    }
}

TEST(ClassMatrixNonInherited, copy_and_move_constructors) {
    TMatrix<int> m1 = { {1, 2}, {3, 4} };
    TMatrix<int> m2(m1);
    EXPECT_EQ(m1, m2);

    TMatrix<int> m3(std::move(m2));
    EXPECT_EQ(m1, m3);
    EXPECT_EQ(0u, m2.rows());
}

TEST(ClassMatrixNonInherited, multiplication_by_scalar) {
    TMatrix<int> m = {
        { 1, 2 },
        { 3, 4 }
    };
    TMatrix<int> res1 = m * 2;
    TMatrix<int> res2 = 2 * m;

    EXPECT_EQ(2, res1[0][0]);
    EXPECT_EQ(4, res1[0][1]);
    EXPECT_EQ(6, res1[1][0]);
    EXPECT_EQ(8, res1[1][1]);
    EXPECT_EQ(res1, res2);
}

TEST(ClassMatrixNonInherited, division_by_scalar) {
    TMatrix<double> m = {
        { 2.0, 4.0 },
        { 6.0, 8.0 }
    };
    TMatrix<double> res = m / 2.0;
    EXPECT_DOUBLE_EQ(1.0, res[0][0]);
    EXPECT_DOUBLE_EQ(4.0, res[1][1]);

    EXPECT_THROW(m / 0.0, const char*);
}

TEST(ClassMatrixNonInherited, matrix_vector_multiplication) {
    // [ 1 2 3 ]   [ 1 ]   [ 14 ]
    // [ 4 5 6 ] * [ 2 ] = [ 32 ]
    //             [ 3 ]
    TMatrix<int> m = {
        { 1, 2, 3 },
        { 4, 5, 6 }
    };
    TMathVector<int> v = { 1, 2, 3 };

    TMathVector<int> res = m * v;
    EXPECT_EQ(2u, res.size());
    EXPECT_EQ(14, res[0]);
    EXPECT_EQ(32, res[1]);
}

TEST(ClassMatrixNonInherited, throw_when_matrix_vector_dimension_mismatch) {
    TMatrix<int> m(2, 3, 1);
    TMathVector<int> v(2, 1); // needs 3
    EXPECT_THROW(m * v, const char*);
}

TEST(ClassMatrixNonInherited, matrix_matrix_multiplication_rectangular) {
    // A: 2x3, B: 3x2 -> C: 2x2
    TMatrix<int> A = {
        { 1, 2, 3 },
        { 4, 5, 6 }
    };
    TMatrix<int> B = {
        { 7,  8 },
        { 9,  1 },
        { 2,  3 }
    };
    // C[0][0] = 1*7 + 2*9 + 3*2 = 7 + 18 + 6 = 31
    // C[0][1] = 1*8 + 2*1 + 3*3 = 8 + 2 + 9 = 19
    // C[1][0] = 4*7 + 5*9 + 6*2 = 28 + 45 + 12 = 85
    // C[1][1] = 4*8 + 5*1 + 6*3 = 32 + 5 + 18 = 55
    TMatrix<int> C = A * B;

    EXPECT_EQ(2u, C.rows());
    EXPECT_EQ(2u, C.cols());
    EXPECT_EQ(31, C[0][0]);
    EXPECT_EQ(19, C[0][1]);
    EXPECT_EQ(85, C[1][0]);
    EXPECT_EQ(55, C[1][1]);
}

TEST(ClassMatrixNonInherited, throw_when_matrix_multiplication_dimension_mismatch) {
    TMatrix<int> A(2, 3, 1);
    TMatrix<int> B(2, 2, 1); // A cols=3, B rows=2 -> mismatch
    EXPECT_THROW(A * B, const char*);
}

TEST(ClassMatrixNonInherited, transpose) {
    // 2x3 -> 3x2
    TMatrix<int> m = {
        { 1, 2, 3 },
        { 4, 5, 6 }
    };
    TMatrix<int> t = m.transpose();

    EXPECT_EQ(3u, t.rows());
    EXPECT_EQ(2u, t.cols());

    EXPECT_EQ(1, t[0][0]);
    EXPECT_EQ(4, t[0][1]);
    EXPECT_EQ(2, t[1][0]);
    EXPECT_EQ(5, t[1][1]);
    EXPECT_EQ(3, t[2][0]);
    EXPECT_EQ(6, t[2][1]);
}

TEST(ClassMatrixNonInherited, stream_output) {
    TMatrix<int> m = {
        { 1, 2 },
        { 3, 4 }
    };
    std::stringstream ss;
    ss << m;
    EXPECT_EQ("[ 1 2 ]\n[ 3 4 ]", ss.str());
}