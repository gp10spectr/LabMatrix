#include "pch.h"
#include "ttrianglematrix.h"
#include <sstream>

TEST(ClassTriangleMatrix, can_create_size_and_value) {
    TTriangleMatrix<double> m(3, 1.5);
    EXPECT_EQ(3u, m.size());
    for (size_t i = 0; i < 3; i++)
        for (size_t j = 0; j < 3; j++) {
            if (j >= i) EXPECT_DOUBLE_EQ(1.5, m[i][j]);
            else        EXPECT_DOUBLE_EQ(0.0, m[i][j]);
        }
}

TEST(ClassTriangleMatrix, can_create_from_2d_list) {
    TTriangleMatrix<int> m({{ 3, 4, 3, 0, 0 },
        { 0, 4, 3, 5, 1 },
        { 0, 0, 3, 5, 1 },
        { 0, 0, 0, 5, 1 },
        { 0, 0, 0, 0, 1 }});
    EXPECT_EQ(5u, m.size());
    EXPECT_EQ(3, m[0][0]);
    EXPECT_EQ(4, m[0][1]);
    EXPECT_EQ(0, m[0][4]);
    EXPECT_EQ(0, m[4][1]);
    EXPECT_EQ(1, m[4][4]);
}

TEST(ClassTriangleMatrix, throws_on_non_square) {
    EXPECT_THROW(
        TTriangleMatrix<int>({{ 1, 2, 3 },
            { 0, 1, 2 }}),
        const char*
    );
}

TEST(ClassTriangleMatrix, throws_on_nonzero_below_diagonal) {
    EXPECT_THROW(
        TTriangleMatrix<int>({{ 1, 2, 3 },
            { 5, 1, 2 },
            { 0, 0, 1 }}),
        const char*
    );
}

TEST(ClassTriangleMatrix, read_below_diagonal_returns_zero) {
    TTriangleMatrix<int> m({{ 1, 2, 3 },
        { 0, 4, 5 },
        { 0, 0, 6 }});
    EXPECT_EQ(0, m[2][0]);
    EXPECT_EQ(0, m[1][0]);
    EXPECT_EQ(0, m[2][1]);
}

TEST(ClassTriangleMatrix, write_above_diagonal_changes_element) {
    TTriangleMatrix<int> m({{ 1, 2, 3 },
        { 0, 4, 5 },
        { 0, 0, 6 }});
    m[1][2] = -6;
    EXPECT_EQ(-6, m[1][2]);
}

TEST(ClassTriangleMatrix, write_below_diagonal_is_ignored_or_throws) {
    TTriangleMatrix<int> m({{ 1, 2, 3 },
        { 0, 4, 5 },
        { 0, 0, 6 }});
    m[2][0] = 999;
    EXPECT_EQ(0, m[2][0]);
    EXPECT_EQ(1, m[0][0]);
    EXPECT_EQ(6, m[2][2]);
}

TEST(ClassTriangleMatrix, can_output) {
    TTriangleMatrix<int> m({{ 1, 2, 3 },
        { 0, 4, 5 },
        { 0, 0, 6 }});
    std::stringstream ss;
    ss << m;
    EXPECT_EQ("[ 1 2 3 ]\n[ 0 4 5 ]\n[ 0 0 6 ]", ss.str());
}

TEST(ClassTriangleMatrix, output_matches_example_from_task) {
    TTriangleMatrix<int> A({
        { 3, 4, 3, 0, 0 },
        { 0, 4, 3, 5, 1 },
        { 0, 0, 3, 5, 1 },
        { 0, 0, 0, 5, 1 },
        { 0, 0, 0, 0, 1 }
        });

    std::stringstream ss;
    ss << A;
    EXPECT_EQ(
        "[ 3 4 3 0 0 ]\n"
        "[ 0 4 3 5 1 ]\n"
        "[ 0 0 3 5 1 ]\n"
        "[ 0 0 0 5 1 ]\n"
        "[ 0 0 0 0 1 ]",
        ss.str());
    A[2][3] = -6;
    ss.str("");
    ss << A;
    EXPECT_EQ(
        "[ 3 4 3 0 0 ]\n"
        "[ 0 4 3 5 1 ]\n"
        "[ 0 0 3 -6 1 ]\n"
        "[ 0 0 0 5 1 ]\n"
        "[ 0 0 0 0 1 ]",
        ss.str());
    EXPECT_EQ(0, A[4][1]);
}

TEST(ClassTriangleMatrix, storage_size_is_triangular) {
    //n*(n+1)/2 elements contains
    TTriangleMatrix<int> m(4);
    size_t total = 0;
    for (size_t i = 0; i < m.size(); ++i)
        total += m.row_length(i);
    EXPECT_EQ(10u, total);           // 4+3+2+1 = 10
}
