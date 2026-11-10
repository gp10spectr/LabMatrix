#include "pch.h"
#include "ttrianglematrix.h"

TEST(ClassTriangleMatrix, memory_optimization_check) {
    //checks if rows are really different sizes
    TTriangleMatrix<int> A(5);
    EXPECT_EQ(5u, A.raw_row_size(0));
    EXPECT_EQ(4u, A.raw_row_size(1));
    EXPECT_EQ(3u, A.raw_row_size(2));
    EXPECT_EQ(2u, A.raw_row_size(3));
    EXPECT_EQ(1u, A.raw_row_size(4));
}

TEST(ClassTriangleMatrix, sample_from_assignment_read) {
    TTriangleMatrix<int> A({
        { 3, 4, 3, 0, 0 },
        { 0, 4, 3, 5, 1 },
        { 0, 0, 3, 5, 1 },
        { 0, 0, 0, 5, 1 },
        { 0, 0, 0, 0, 1 }
        });

    EXPECT_EQ(3, A[0][0]);
    EXPECT_EQ(4, A[0][1]);
    EXPECT_EQ(5, A[1][3]);
    EXPECT_EQ(1, A[4][4]);

    //under diagonal returns 0
    EXPECT_EQ(0, A[4][1]);
    EXPECT_EQ(0, A[3][0]);
    EXPECT_EQ(0, A[2][1]);
}

TEST(ClassTriangleMatrix, sample_from_assignment_write) {
    TTriangleMatrix<int> A({
        { 3, 4, 3, 0, 0 },
        { 0, 4, 3, 5, 1 },
        { 0, 0, 3, 5, 1 },
        { 0, 0, 0, 5, 1 },
        { 0, 0, 0, 0, 1 }
        });

    // above diagonal
    A[2][3] = -6;
    EXPECT_EQ(-6, A[2][3]);

    // under diagonal
    A[4][1] = 5;
    EXPECT_EQ(0, A[4][1]);

    A[3][0] = 10;
    EXPECT_EQ(0, A[3][0]);
}

TEST(ClassTriangleMatrix, equality_and_inequality) {
    TTriangleMatrix<int> A({ {1, 2}, {0, 3} });
    TTriangleMatrix<int> B({ {1, 2}, {0, 3} });
    TTriangleMatrix<int> C({ {1, 9}, {0, 3} });

    EXPECT_TRUE(A == B);
    EXPECT_FALSE(A != B);
    EXPECT_TRUE(A != C);
}

TEST(ClassTriangleMatrix, throw_when_operations_dimension_mismatch) {
    TTriangleMatrix<int> A(2);
    TTriangleMatrix<int> B(3);
    TMathVector<int> v(3);

    EXPECT_THROW(A + B, const char*);
    EXPECT_THROW(A - B, const char*);
    EXPECT_THROW(A * B, const char*);
    EXPECT_THROW(A * v, const char*); // vector size 3 matrix size 2
}

TEST(ClassTriangleMatrix, throw_when_initializer_list_row_size_mismatch) {
    // wrong size vectors
    EXPECT_THROW(TTriangleMatrix<int>({ {1, 2, 3}, {0, 4} }), const char*);
}

TEST(ClassTriangleMatrix, addition) {
    TTriangleMatrix<int> A({
        { 1, 2 },
        { 0, 3 }
        });
    TTriangleMatrix<int> B({
        { 4, 5 },
        { 0, 6 }
        });

    TTriangleMatrix<int> C = A + B;
    EXPECT_EQ(5, C[0][0]);
    EXPECT_EQ(7, C[0][1]);
    EXPECT_EQ(0, C[1][0]);
    EXPECT_EQ(9, C[1][1]);
}

TEST(ClassTriangleMatrix, subtraction) {
    TTriangleMatrix<int> A({
        { 5, 7 },
        { 0, 9 }
        });
    TTriangleMatrix<int> B({
        { 1, 2 },
        { 0, 3 }
        });

    TTriangleMatrix<int> C = A - B;
    EXPECT_EQ(4, C[0][0]);
    EXPECT_EQ(5, C[0][1]);
    EXPECT_EQ(0, C[1][0]);
    EXPECT_EQ(6, C[1][1]);
}

TEST(ClassTriangleMatrix, multiplication_by_scalar) {
    TTriangleMatrix<int> A({
        { 1, 2 },
        { 0, 3 }
        });
    TTriangleMatrix<int> C = A * 3;
    EXPECT_EQ(3, C[0][0]);
    EXPECT_EQ(6, C[0][1]);
    EXPECT_EQ(0, C[1][0]);
    EXPECT_EQ(9, C[1][1]);
}

TEST(ClassTriangleMatrix, matrix_vector_multiplication) {
    TTriangleMatrix<int> A({
        { 1, 2, 3 },
        { 0, 4, 5 },
        { 0, 0, 6 }
        });
    TMathVector<int> v({ 1, 2, 3 });

    TMathVector<int> res = A * v;
    EXPECT_EQ(14, res[0]);
    EXPECT_EQ(23, res[1]);
    EXPECT_EQ(18, res[2]);
}

TEST(ClassTriangleMatrix, matrix_matrix_multiplication) {
    TTriangleMatrix<int> A({
        { 1, 2 },
        { 0, 3 }
        });
    TTriangleMatrix<int> B({
        { 4, 5 },
        { 0, 6 }
        });

    TTriangleMatrix<int> C = A * B;
    EXPECT_EQ(4, C[0][0]);
    EXPECT_EQ(17, C[0][1]);
    EXPECT_EQ(0, C[1][0]);
    EXPECT_EQ(18, C[1][1]);
}

TEST(ClassTriangleMatrix, stream_output_exact_format) {
    TTriangleMatrix<int> A({
        { 3, 4 },
        { 0, 5 }
        });
    std::stringstream ss;
    ss << A;
    EXPECT_EQ("[ 3 4 ]\n[ 0 5 ]", ss.str());
}