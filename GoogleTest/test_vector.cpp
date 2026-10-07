#pragma once
#include "pch.h"

#define MEMDATA_TESTS
//#define VECTOR_TESTS

#ifdef MEMDATA_TESTS
#include "memdata.h"

TEST(FunctionsForMemData, calculate_capacity) {
    EXPECT_EQ(0, calculate_capacity(0));
    EXPECT_EQ(0, calculate_capacity(-5));
    EXPECT_EQ(15, calculate_capacity(1));
    EXPECT_EQ(15, calculate_capacity(15));
    EXPECT_EQ(30, calculate_capacity(16));
    EXPECT_EQ(30, calculate_capacity(30));
    EXPECT_EQ(45, calculate_capacity(31));
}

TEST(ClassMemData, can_create_with_default_constructor) {
    MemData<double> md;
    EXPECT_EQ(0u, md.size());
    EXPECT_EQ(0u, md.capacity());
    EXPECT_TRUE(md.is_empty());
    EXPECT_TRUE(md.is_full());
}

TEST(ClassMemData, can_create_with_constructor_by_size) {
    MemData<double> md(7);
    EXPECT_EQ(7u, md.size());
    EXPECT_EQ(15u, md.capacity());
    EXPECT_FALSE(md.is_empty());
    EXPECT_FALSE(md.is_full());
    for (size_t i = 0; i < md.size(); ++i)
        EXPECT_DOUBLE_EQ(0.0, md.data()[i]);
}

TEST(ClassMemData, can_create_with_constructor_by_initializer_list) {
    MemData<double> md({ 1.5, 2.5, 3.5 });
    EXPECT_EQ(3u, md.size());
    EXPECT_EQ(15u, md.capacity());
    EXPECT_DOUBLE_EQ(1.5, md.data()[0]);
    EXPECT_DOUBLE_EQ(2.5, md.data()[1]);
    EXPECT_DOUBLE_EQ(3.5, md.data()[2]);
}

TEST(ClassMemData, can_create_with_init_constructor) {
    double arr[3] = { 4.0, 5.0, 6.0 };
    MemData<double> md(arr, 3);
    EXPECT_EQ(3u, md.size());
    EXPECT_EQ(15u, md.capacity());
    EXPECT_DOUBLE_EQ(4.0, md.data()[0]);
    EXPECT_DOUBLE_EQ(6.0, md.data()[2]);
}

TEST(ClassMemData, can_create_with_copy_constructor) {
    MemData<double> src({ 1.0, 2.0, 3.0 });
    MemData<double> md(src);
    EXPECT_EQ(3u, md.size());
    EXPECT_EQ(15u, md.capacity());
    EXPECT_DOUBLE_EQ(1.0, md.data()[0]);
    EXPECT_DOUBLE_EQ(3.0, md.data()[2]);
    EXPECT_NE(src.data(), md.data());
}

TEST(ClassMemData, can_create_with_move_constructor) {
    MemData<double> src({ 1.0, 2.0, 3.0 });
    const double* old_ptr = src.data();
    MemData<double> md(std::move(src));

    EXPECT_EQ(3u, md.size());
    EXPECT_EQ(15u, md.capacity());
    EXPECT_EQ(old_ptr, md.data());

    EXPECT_EQ(0u, src.size());
    EXPECT_EQ(0u, src.capacity());
    EXPECT_EQ(nullptr, src.data());
}

TEST(ClassMemData, can_is_empty) {
    MemData<double> a;
    MemData<double> b(5);
    EXPECT_TRUE(a.is_empty());
    EXPECT_FALSE(b.is_empty());
}

TEST(ClassMemData, can_is_full) {
    MemData<double> a(15);
    MemData<double> b(10);
    EXPECT_TRUE(a.is_full());
    EXPECT_FALSE(b.is_full());
}

TEST(ClassMemData, can_set_memory_for_empty) {
    MemData<double> md;
    md.set_memory(30);
    EXPECT_EQ(0u, md.size());
    EXPECT_EQ(30u, md.capacity());
    EXPECT_NE(nullptr, md.data());
    md.clear_memory();
    EXPECT_EQ(0u, md.capacity());
}

TEST(ClassMemData, can_set_memory_for_not_empty) {
    MemData<double> md({ 1.0, 2.0, 3.0 });
    md.set_memory(30);
    EXPECT_EQ(3u, md.size());
    EXPECT_EQ(30u, md.capacity());
}

TEST(ClassMemData, can_set_memory_without_reallocation) {
    MemData<double> md({ 1.0, 2.0, 3.0 });
    const double* ptr_before = md.data();
    md.set_memory(15);
    EXPECT_EQ(ptr_before, md.data());
}

TEST(ClassMemData, can_reset_memory_for_empty) {
    MemData<double> md;
    md.reset_memory(5);
    EXPECT_EQ(5u, md.size());
    EXPECT_EQ(15u, md.capacity());
    for (size_t i = 0; i < 5; ++i)
        EXPECT_DOUBLE_EQ(0.0, md.data()[i]);
}

TEST(ClassMemData, can_reset_memory_for_not_empty_increase) {
    MemData<double> md({ 1.0, 2.0, 3.0 });
    md.reset_memory(16);
    EXPECT_EQ(16u, md.size());
    EXPECT_EQ(30u, md.capacity());
    EXPECT_DOUBLE_EQ(1.0, md.data()[0]);
    EXPECT_DOUBLE_EQ(2.0, md.data()[1]);
    EXPECT_DOUBLE_EQ(3.0, md.data()[2]);
}

TEST(ClassMemData, can_reset_memory_for_not_empty_decrease) {
    MemData<double> md({ 1.0, 2.0, 3.0 });
    md.reset_memory(2);
    EXPECT_EQ(2u, md.size());
    EXPECT_EQ(15u, md.capacity());
    EXPECT_DOUBLE_EQ(1.0, md.data()[0]);
    EXPECT_DOUBLE_EQ(2.0, md.data()[1]);
}

TEST(ClassMemData, can_reset_memory_without_reallocation) {
    MemData<double> md({ 1.0, 2.0, 3.0 });
    const double* ptr_before = md.data();
    md.reset_memory(10);
    EXPECT_EQ(ptr_before, md.data());
    EXPECT_EQ(10u, md.size());
    EXPECT_EQ(15u, md.capacity());
    EXPECT_DOUBLE_EQ(1.0, md.data()[0]);
    EXPECT_DOUBLE_EQ(3.0, md.data()[2]);
}

TEST(ClassMemData, can_reset_memory_with_shift) {
    MemData<double> md({ 1.0, 2.0, 3.0 });
    md.reset_memory(5, 2);
    EXPECT_EQ(5u, md.size());
    EXPECT_EQ(15u, md.capacity());
    EXPECT_DOUBLE_EQ(0.0, md.data()[0]);
    EXPECT_DOUBLE_EQ(0.0, md.data()[1]);
    EXPECT_DOUBLE_EQ(1.0, md.data()[2]);
    EXPECT_DOUBLE_EQ(2.0, md.data()[3]);
    EXPECT_DOUBLE_EQ(3.0, md.data()[4]);
}

TEST(ClassMemData, can_clear_memory_for_empty) {
    MemData<double> md;
    md.clear_memory();
    EXPECT_EQ(0u, md.size());
    EXPECT_EQ(0u, md.capacity());
    EXPECT_EQ(nullptr, md.data());
}

TEST(ClassMemData, can_clear_memory_for_not_empty) {
    MemData<double> md({ 1.0, 2.0, 3.0 });
    md.clear_memory();
    EXPECT_EQ(0u, md.size());
    EXPECT_EQ(0u, md.capacity());
    EXPECT_EQ(nullptr, md.data());
}

TEST(ClassMemData, can_assigment) {
    MemData<double> a({ 1.0, 2.0, 3.0 });
    MemData<double> b;
    b = a;
    EXPECT_EQ(3u, b.size());
    EXPECT_EQ(15u, b.capacity());
    EXPECT_DOUBLE_EQ(1.0, b.data()[0]);
    EXPECT_NE(a.data(), b.data());
}

TEST(ClassMemData, can_move_assigment) {
    MemData<double> a({ 1.0, 2.0, 3.0 });
    const double* old_ptr = a.data();
    MemData<double> b;
    b = std::move(a);
    EXPECT_EQ(3u, b.size());
    EXPECT_EQ(old_ptr, b.data());
    EXPECT_EQ(0u, a.size());
    EXPECT_EQ(0u, a.capacity());
}

#endif

#define VECTOR_TESTS
#ifdef VECTOR_TESTS
#include "vector.h"

TEST(ClassVector, can_create_with_default_constructor) {
    TVector<double> v;
    EXPECT_EQ(0u, v.size());
    EXPECT_EQ(0u, v.capacity());
    EXPECT_TRUE(v.is_empty());
}

TEST(ClassVector, can_create_with_constructor_by_size) {
    TVector<double> v(5);
    EXPECT_EQ(5u, v.size());
    EXPECT_EQ(15u, v.capacity());
    EXPECT_FALSE(v.is_empty());
    for (size_t i = 0; i < v.size(); ++i)
        EXPECT_DOUBLE_EQ(0.0, v[i]);
}

TEST(ClassVector, can_create_with_constructor_by_initializer_list) {
    TVector<double> v({ 1.0, 2.0, 3.0 });
    EXPECT_EQ(3u, v.size());
    EXPECT_EQ(15u, v.capacity());
    EXPECT_DOUBLE_EQ(1.0, v[0]);
    EXPECT_DOUBLE_EQ(3.0, v[2]);
}

TEST(ClassVector, can_create_with_init_constructor) {
    double arr[3] = { 4.0, 5.0, 6.0 };
    TVector<double> v(arr, 3);
    EXPECT_EQ(3u, v.size());
    EXPECT_DOUBLE_EQ(4.0, v[0]);
    EXPECT_DOUBLE_EQ(6.0, v[2]);
}

TEST(ClassVector, can_create_with_copy_constructor) {
    TVector<double> a({ 1.0, 2.0, 3.0 });
    TVector<double> b(a);
    EXPECT_EQ(3u, b.size());
    EXPECT_DOUBLE_EQ(1.0, b[0]);
    EXPECT_DOUBLE_EQ(3.0, b[2]);
}

TEST(ClassVector, can_create_with_move_constructor) {
    TVector<double> a({ 1.0, 2.0, 3.0 });
    TVector<double> b(std::move(a));
    EXPECT_EQ(3u, b.size());
    EXPECT_EQ(0u, a.size());
    EXPECT_EQ(0u, a.capacity());
    EXPECT_DOUBLE_EQ(1.0, b[0]);
    EXPECT_DOUBLE_EQ(3.0, b[2]);
}

TEST(ClassVector, can_is_empty) {
    TVector<double> a;
    TVector<double> b({ 1.0 });
    EXPECT_TRUE(a.is_empty());
    EXPECT_FALSE(b.is_empty());
}

TEST(ClassVector, can_is_full) {
    TVector<double> a(15);
    TVector<double> b(5);
    EXPECT_TRUE(a.is_full());
    EXPECT_FALSE(b.is_full());
}

TEST(ClassVector, can_get_front) {
    TVector<double> v({ 10.0, 20.0, 30.0 });
    EXPECT_DOUBLE_EQ(10.0, v.front());
}

TEST(ClassVector, can_get_back) {
    TVector<double> v({ 10.0, 20.0, 30.0 });
    EXPECT_DOUBLE_EQ(30.0, v.back());
}

TEST(ClassVector, can_set_front) {
    TVector<double> v({ 10.0, 20.0, 30.0 });
    v.front() = 99.0;
    EXPECT_DOUBLE_EQ(99.0, v[0]);
    EXPECT_DOUBLE_EQ(99.0, v.front());
}

TEST(ClassVector, can_set_back) {
    TVector<double> v({ 10.0, 20.0, 30.0 });
    v.back() = 77.0;
    EXPECT_DOUBLE_EQ(77.0, v[2]);
    EXPECT_DOUBLE_EQ(77.0, v.back());
}

TEST(ClassVector, throw_when_try_get_front_in_empty_vector) {
    TVector<double> v;
    EXPECT_THROW(v.front(), const char*);
}

TEST(ClassVector, throw_when_try_get_back_in_empty_vector) {
    TVector<double> v;
    EXPECT_THROW(v.back(), const char*);
}

TEST(ClassVector, throw_when_try_set_front_in_empty_vector) {
    TVector<double> v;
    EXPECT_THROW(v.front() = 1.0, const char*);
}

TEST(ClassVector, throw_when_try_set_back_in_empty_vector) {
    TVector<double> v;
    EXPECT_THROW(v.back() = 1.0, const char*);
}

TEST(ClassVector, can_output_with_operator_cout) {
    TVector<double> vec({ 1, 2, 3, 4, 5, 6, 7, 8, 9 });
    std::stringstream out;
    out << vec;
    EXPECT_EQ("{ 1, 2, 3, 4, 5, 6, 7, 8, 9 }", out.str());
}

TEST(ClassVector, can_input_with_operator_cin) {
    TVector<double> vec({ 1, 2, 3, 4, 5, 6, 7, 8, 9 });
    std::stringstream in("9 1 2 3 4 5 6 7 8 9");
    in >> vec;

    EXPECT_EQ(9, vec.size());
    EXPECT_EQ(15, vec.capacity());

    for (size_t i = 0; i < vec.size(); i++) {
        EXPECT_DOUBLE_EQ(vec[i], i + 1);
    }
}

TEST(ClassVector, can_push_front) {
    TVector<double> v({ 2.0, 3.0 });
    v.push_front(1.0);
    EXPECT_EQ(3u, v.size());
    EXPECT_EQ(15u, v.capacity());
    EXPECT_DOUBLE_EQ(1.0, v[0]);
    EXPECT_DOUBLE_EQ(2.0, v[1]);
    EXPECT_DOUBLE_EQ(3.0, v[2]);
}

TEST(ClassVector, can_push_front_in_empty_vector) {
    TVector<double> v;
    v.push_front(5.0);
    EXPECT_EQ(1u, v.size());
    EXPECT_EQ(15u, v.capacity());
    EXPECT_DOUBLE_EQ(5.0, v[0]);
    EXPECT_DOUBLE_EQ(5.0, v.front());
    EXPECT_DOUBLE_EQ(5.0, v.back());
}

TEST(ClassVector, can_push_front_with_reallocation) {
    TVector<double> v;
    for (int i = 0; i < 16; ++i) v.push_front(static_cast<double>(i));
    EXPECT_EQ(16u, v.size());
    EXPECT_EQ(30u, v.capacity());
    for (size_t i = 0; i < v.size(); ++i)
        EXPECT_DOUBLE_EQ(static_cast<double>(15 - i), v[i]);
}

TEST(ClassVector, can_push_back) {
    TVector<double> v({ 1.0, 2.0 });
    v.push_back(3.0);
    EXPECT_EQ(3u, v.size());
    EXPECT_DOUBLE_EQ(3.0, v.back());
}

TEST(ClassVector, can_push_back_in_empty_vector) {
    TVector<double> v;
    v.push_back(5.0);
    EXPECT_EQ(1u, v.size());
    EXPECT_EQ(15u, v.capacity());
    EXPECT_DOUBLE_EQ(5.0, v.front());
    EXPECT_DOUBLE_EQ(5.0, v.back());
}

TEST(ClassVector, can_push_back_with_reallocation) {
    TVector<double> v;
    for (int i = 0; i < 16; ++i) v.push_back(static_cast<double>(i));
    EXPECT_EQ(16u, v.size());
    EXPECT_EQ(30u, v.capacity());
    for (size_t i = 0; i < v.size(); ++i)
        EXPECT_DOUBLE_EQ(static_cast<double>(i), v[i]);
}

TEST(ClassVector, can_insert) {
    TVector<double> v({ 1.0, 3.0, 4.0 });
    v.insert(2.0, 1);
    EXPECT_EQ(4u, v.size());
    EXPECT_DOUBLE_EQ(1.0, v[0]);
    EXPECT_DOUBLE_EQ(2.0, v[1]);
    EXPECT_DOUBLE_EQ(3.0, v[2]);
    EXPECT_DOUBLE_EQ(4.0, v[3]);
}

TEST(ClassVector, can_insert_with_reallocation) {
    TVector<double> v;
    for (int i = 0; i < 15; ++i) v.push_back(static_cast<double>(i + 1));
    v.insert(0.5, 5);
    EXPECT_EQ(16u, v.size());
    EXPECT_EQ(30u, v.capacity());
    EXPECT_DOUBLE_EQ(0.5, v[5]);
    EXPECT_DOUBLE_EQ(5.0, v[4]);
    EXPECT_DOUBLE_EQ(6.0, v[6]);
}

TEST(ClassVector, can_insert_to_front) {
    TVector<double> v({ 2.0, 3.0 });
    v.insert(1.0, 0);
    EXPECT_EQ(3u, v.size());
    EXPECT_DOUBLE_EQ(1.0, v.front());
}

TEST(ClassVector, throw_when_try_insert_with_wrong_position) {
    TVector<double> v({ 1.0, 2.0 });
    EXPECT_THROW(v.insert(0.0, 5), const char*);
}

TEST(ClassVector, can_pop_front) {
    TVector<double> v({ 1.0, 2.0, 3.0 });
    v.pop_front();
    EXPECT_EQ(2u, v.size());
    EXPECT_DOUBLE_EQ(2.0, v.front());
    EXPECT_DOUBLE_EQ(3.0, v.back());
}

TEST(ClassVector, can_pop_front_with_reallocation) {
    TVector<double> v;
    for (int i = 0; i < 30; ++i) v.push_back(static_cast<double>(i + 1));
    EXPECT_EQ(30u, v.capacity());

    for (int i = 0; i < 15; ++i) v.pop_front();

    EXPECT_EQ(15u, v.size());
    EXPECT_EQ(15u, v.capacity());
    for (size_t i = 0; i < v.size(); ++i)
        EXPECT_DOUBLE_EQ(static_cast<double>(i + 16), v[i]);
}

TEST(ClassVector, throw_when_try_pop_front_from_empty_vector) {
    TVector<double> v;
    EXPECT_THROW(v.pop_front(), const char*);
}

TEST(ClassVector, can_pop_back) {
    TVector<double> v({ 1.0, 2.0, 3.0 });
    v.pop_back();
    EXPECT_EQ(2u, v.size());
    EXPECT_DOUBLE_EQ(1.0, v.front());
    EXPECT_DOUBLE_EQ(2.0, v.back());
}

TEST(ClassVector, can_pop_back_with_reallocation) {
    TVector<double> v;
    for (int i = 0; i < 30; ++i) v.push_back(static_cast<double>(i + 1));
    EXPECT_EQ(30u, v.capacity());

    for (int i = 0; i < 15; ++i) v.pop_back();

    EXPECT_EQ(15u, v.size());
    EXPECT_EQ(15u, v.capacity());
    for (size_t i = 0; i < v.size(); ++i)
        EXPECT_DOUBLE_EQ(static_cast<double>(i + 1), v[i]);
}

TEST(ClassVector, throw_when_try_pop_back_from_empty_vector) {
    TVector<double> v;
    EXPECT_THROW(v.pop_back(), const char*);
}

TEST(ClassVector, can_correctly_recalc_back_in_area_of_zero) {
    TVector<double> vec;

    for (size_t i = 0; i < 15; i++) {
        vec.push_back(i + 1);
    }

    vec.pop_front();
    vec.push_back(16);

    EXPECT_EQ(15, vec.size());
    EXPECT_EQ(15, vec.capacity());
    EXPECT_DOUBLE_EQ(16.0, vec.back());

    for (size_t i = 0; i < vec.size(); i++) {
        EXPECT_DOUBLE_EQ(vec[i], i + 2);
    }

    vec.pop_back();

    EXPECT_EQ(14, vec.size());
    EXPECT_EQ(15, vec.capacity());
    EXPECT_DOUBLE_EQ(15.0, vec.back());

    for (size_t i = 0; i < vec.size(); i++) {
        EXPECT_DOUBLE_EQ(vec[i], i + 2);
    }
}

TEST(ClassVector, can_correctly_recalc_front_in_area_of_zero) {
    TVector<double> vec;

    for (size_t i = 0; i < 15; i++) {
        vec.push_front(i + 1);
    }

    vec.pop_back();
    vec.push_front(16);

    EXPECT_EQ(15u, vec.size());
    EXPECT_EQ(15u, vec.capacity());
    EXPECT_DOUBLE_EQ(16.0, vec.front());
    for (size_t i = 0; i < vec.size(); i++) {
        EXPECT_DOUBLE_EQ(vec[i], 16.0 - i);
    }

    vec.pop_front();

    EXPECT_EQ(14u, vec.size());
    EXPECT_EQ(15u, vec.capacity());
    EXPECT_DOUBLE_EQ(15.0, vec.front());
    for (size_t i = 0; i < vec.size(); i++) {
        EXPECT_DOUBLE_EQ(vec[i], 15.0 - i);
    }
}

TEST(ClassVector, can_erase) {
    TVector<double> v({ 1.0, 2.0, 3.0, 4.0 });
    v.erase(1);
    EXPECT_EQ(3u, v.size());
    EXPECT_DOUBLE_EQ(1.0, v[0]);
    EXPECT_DOUBLE_EQ(3.0, v[1]);
    EXPECT_DOUBLE_EQ(4.0, v[2]);
}

TEST(ClassVector, can_erase_front) {
    TVector<double> v({ 1.0, 2.0, 3.0 });
    v.erase(0);
    EXPECT_EQ(2u, v.size());
    EXPECT_DOUBLE_EQ(2.0, v.front());
}

TEST(ClassVector, can_erase_back) {
    TVector<double> v({ 1.0, 2.0, 3.0 });
    v.erase(2);
    EXPECT_EQ(2u, v.size());
    EXPECT_DOUBLE_EQ(2.0, v.back());
}

TEST(ClassVector, can_erase_with_reallocation) {
    TVector<double> v;
    for (int i = 0; i < 30; ++i) v.push_back(static_cast<double>(i + 1));
    EXPECT_EQ(30u, v.capacity());

    for (int i = 0; i < 15; ++i) v.erase(0);

    EXPECT_EQ(15u, v.size());
    EXPECT_EQ(15u, v.capacity());
    for (size_t i = 0; i < v.size(); ++i)
        EXPECT_DOUBLE_EQ(static_cast<double>(i + 16), v[i]);
}

TEST(ClassVector, throw_when_try_erase_from_empty_vector) {
    TVector<double> v;
    EXPECT_THROW(v.erase(0), const char*);
}

TEST(ClassVector, throw_when_try_erase_with_wrong_position) {
    TVector<double> v({ 1.0, 2.0 });
    EXPECT_THROW(v.erase(5), const char*);
}

TEST(ClassVector, combination_push_pop_insert_erase) {
    TVector<double> vec({ 3, 44, 5, 7, 8 });

    std::stringstream out;
    out << vec;
    EXPECT_EQ("{ 3, 44, 5, 7, 8 }", out.str());
    out.str("");

    vec.pop_front();
    out << vec;
    EXPECT_EQ("{ 44, 5, 7, 8 }", out.str());
    out.str("");

    for (size_t i = 0; i < 4; i++) {
        vec.push_front(3 - i);
    }
    out << vec;
    EXPECT_EQ("{ 0, 1, 2, 3, 44, 5, 7, 8 }", out.str());
    out.str("");

    vec.pop_back();
    out << vec;
    EXPECT_EQ("{ 0, 1, 2, 3, 44, 5, 7 }", out.str());
    out.str("");

    for (size_t i = 0; i < 4; i++) {
        vec.push_back(8 + i);
    }
    out << vec;
    EXPECT_EQ("{ 0, 1, 2, 3, 44, 5, 7, 8, 9, 10, 11 }", out.str());
    out.str("");

    vec.erase(0);
    out << vec;
    EXPECT_EQ("{ 1, 2, 3, 44, 5, 7, 8, 9, 10, 11 }", out.str());
    out.str("");

    vec.erase(3);
    out << vec;
    EXPECT_EQ("{ 1, 2, 3, 5, 7, 8, 9, 10, 11 }", out.str());
    out.str("");

    vec.insert(6, 4);
    out << vec;
    EXPECT_EQ("{ 1, 2, 3, 5, 6, 7, 8, 9, 10, 11 }", out.str());
    out.str("");

    for (size_t i = 0; i < 5; i++) {
        vec.push_back(12 + i);
    }
    out << vec;
    EXPECT_EQ("{ 1, 2, 3, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16 }", out.str());
    out.str("");

    vec.insert(4, 3);
    out << vec;
    EXPECT_EQ("{ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16 }", out.str());
    out.str("");

    EXPECT_EQ(16, vec.size());
    EXPECT_EQ(30, vec.capacity());

    for (size_t i = 0; i < vec.size(); i++) {
        EXPECT_DOUBLE_EQ(vec[i], i + 1);
    }
}

TEST(ClassVector, can_assigment) {
    TVector<double> a({ 1.0, 2.0, 3.0 });
    TVector<double> b;
    b = a;
    EXPECT_EQ(3u, b.size());
    EXPECT_DOUBLE_EQ(1.0, b[0]);
    EXPECT_DOUBLE_EQ(3.0, b[2]);
}

TEST(ClassVector, can_move_assigment) {
    TVector<double> vec_1;
    TVector<double> vec_2;

    for (size_t i = 0; i < 4; i++) {
        vec_1.push_back(5 + i);
    }

    for (size_t i = 0; i < 4; i++) {
        vec_1.push_front(4 - i);
    }

    vec_2 = std::move(vec_1);

    EXPECT_EQ(0, vec_1.size());
    EXPECT_EQ(0, vec_1.capacity());

    EXPECT_EQ(8, vec_2.size());
    EXPECT_EQ(15, vec_2.capacity());

    for (size_t i = 0; i < vec_2.size(); i++) {
        EXPECT_EQ(vec_2[i], i + 1);
    }
}

TEST(ClassVector, can_push_back_multiple) {
    TVector<double> v({ 1.0, 2.0 });
    v.push_back(3u, 9.0);
    EXPECT_EQ(5u, v.size());
    EXPECT_DOUBLE_EQ(1.0, v[0]);
    EXPECT_DOUBLE_EQ(2.0, v[1]);
    EXPECT_DOUBLE_EQ(9.0, v[2]);
    EXPECT_DOUBLE_EQ(9.0, v[3]);
    EXPECT_DOUBLE_EQ(9.0, v[4]);
}

TEST(ClassVector, can_push_back_multiple_with_reallocation) {
    TVector<double> v({ 1.0, 2.0, 3.0 });
    v.push_back(20u, 7.0);
    EXPECT_EQ(23u, v.size());
    EXPECT_EQ(30u, v.capacity());
    for (size_t i = 0; i < 23; ++i) {
        if (i < 3) EXPECT_DOUBLE_EQ(double(i + 1), v[i]);
        else EXPECT_DOUBLE_EQ(7.0, v[i]);
    }
}

TEST(ClassVector, can_push_front_multiple) {
    TVector<double> v({ 3.0, 4.0 });
    v.push_front(2u, 9.0);
    EXPECT_EQ(4u, v.size());
    EXPECT_DOUBLE_EQ(9.0, v[0]);
    EXPECT_DOUBLE_EQ(9.0, v[1]);
    EXPECT_DOUBLE_EQ(3.0, v[2]);
    EXPECT_DOUBLE_EQ(4.0, v[3]);
}

TEST(ClassVector, can_push_front_multiple_with_reallocation) {
    TVector<double> v({ 1.0, 2.0 });
    v.push_front(20u, 5.0);
    EXPECT_EQ(22u, v.size());
    EXPECT_EQ(30u, v.capacity());
    for (size_t i = 0; i < 20; ++i) EXPECT_DOUBLE_EQ(5.0, v[i]);
    EXPECT_DOUBLE_EQ(1.0, v[20]);
    EXPECT_DOUBLE_EQ(2.0, v[21]);
}

TEST(ClassVector, can_insert_multiple) {
    TVector<double> v({ 1.0, 2.0, 3.0 });
    v.insert(7.0, 1, 3);
    EXPECT_EQ(6u, v.size());
    EXPECT_DOUBLE_EQ(1.0, v[0]);
    EXPECT_DOUBLE_EQ(7.0, v[1]);
    EXPECT_DOUBLE_EQ(7.0, v[2]);
    EXPECT_DOUBLE_EQ(7.0, v[3]);
    EXPECT_DOUBLE_EQ(2.0, v[4]);
    EXPECT_DOUBLE_EQ(3.0, v[5]);
}

TEST(ClassVector, can_insert_multiple_with_reallocation) {
    TVector<double> v;
    for (int i = 0; i < 15; ++i) v.push_back(double(i + 1));
    v.insert(0.0, 7, 3);
    EXPECT_EQ(18u, v.size());
    EXPECT_EQ(30u, v.capacity());
    for (size_t i = 0; i < 7; ++i) EXPECT_DOUBLE_EQ(double(i + 1), v[i]);
    for (size_t i = 7; i < 10; ++i) EXPECT_DOUBLE_EQ(0.0, v[i]);
    for (size_t i = 10; i < 18; ++i) EXPECT_DOUBLE_EQ(double(i - 2), v[i]);
}

TEST(ClassVector, can_pop_front_multiple) {
    TVector<double> v({ 1.0, 2.0, 3.0, 4.0, 5.0 });
    v.pop_front(2);
    EXPECT_EQ(3u, v.size());
    EXPECT_DOUBLE_EQ(3.0, v[0]);
    EXPECT_DOUBLE_EQ(5.0, v[2]);
}

TEST(ClassVector, can_pop_front_multiple_to_empty) {
    TVector<double> v({ 1.0, 2.0, 3.0 });
    v.pop_front(3);
    EXPECT_TRUE(v.is_empty());
    EXPECT_EQ(0u, v.capacity());
}

TEST(ClassVector, can_pop_back_multiple) {
    TVector<double> v({ 1.0, 2.0, 3.0, 4.0, 5.0 });
    v.pop_back(2);
    EXPECT_EQ(3u, v.size());
    EXPECT_DOUBLE_EQ(3.0, v.back());
}

TEST(ClassVector, can_pop_back_multiple_to_empty) {
    TVector<double> v({ 1.0, 2.0, 3.0 });
    v.pop_back(3);
    EXPECT_TRUE(v.is_empty());
    EXPECT_EQ(0u, v.capacity());
}

TEST(ClassVector, can_erase_multiple) {
    TVector<double> v({ 1.0, 2.0, 3.0, 4.0, 5.0, 6.0 });
    v.erase(1, 3);
    EXPECT_EQ(3u, v.size());
    EXPECT_DOUBLE_EQ(1.0, v[0]);
    EXPECT_DOUBLE_EQ(5.0, v[1]);
    EXPECT_DOUBLE_EQ(6.0, v[2]);
}

TEST(ClassVector, throw_when_try_erase_multiple_with_wrong_position) {
    TVector<double> v({ 1.0, 2.0, 3.0 });
    EXPECT_THROW(v.erase(2, 5), const char*);
}

TEST(ClassVector, throw_when_try_pop_front_multiple_too_many) {
    TVector<double> v({ 1.0, 2.0 });
    EXPECT_THROW(v.pop_front(5), const char*);
}

TEST(ClassVector, throw_when_try_pop_back_multiple_too_many) {
    TVector<double> v({ 1.0, 2.0 });
    EXPECT_THROW(v.pop_back(5), const char*);
}

#endif

TEST(ClassVector, iterator_basic_walk) {
    TVector<double> v({ 1, 2, 3, 4, 5 });
    double expected = 1;
    for (TVector<double>::iterator it = v.begin(); it != v.end(); ++it) {
        EXPECT_DOUBLE_EQ(expected, *it);
        expected += 1;
    }
}

TEST(ClassVector, iterator_write_through) {
    TVector<int> v(5);
    int val = 10;
    for (TVector<int>::iterator it = v.begin(); it != v.end(); ++it) {
        *it = val++;
    }
    for (size_t i = 0; i < v.size(); ++i)
        EXPECT_EQ(static_cast<int>(10 + i), v[i]);
}

TEST(ClassVector, const_iterator_read) {
    const TVector<double> v({ 1.5, 2.5, 3.5 });
    double sum = 0;
    for (TVector<double>::const_iterator it = v.begin(); it != v.end(); ++it) {
        sum += *it;
    }
    EXPECT_DOUBLE_EQ(7.5, sum);
}

TEST(ClassVector, iterator_postfix_and_prefix) {
    TVector<int> v({ 1, 2, 3 });
    TVector<int>::iterator it = v.begin();
    EXPECT_EQ(1, *it);
    EXPECT_EQ(1, *(it++));   // постфикс — возвращает старое
    EXPECT_EQ(2, *it);
    EXPECT_EQ(3, *(++it));   // префикс — возвращает новое
}

TEST(ClassVector, iterator_arithmetic) {
    TVector<int> v({ 1, 2, 3, 4, 5 });
    TVector<int>::iterator it = v.begin();
    EXPECT_EQ(3, *(it + 2));
    EXPECT_EQ(5, *(it + 4));
    EXPECT_EQ(1, *(it - 0));
    it += 3;
    EXPECT_EQ(4, *it);
    it -= 2;
    EXPECT_EQ(2, *it);
}

TEST(ClassVector, iterator_equality) {
    TVector<int> v({ 1, 2, 3 });
    TVector<int>::iterator a = v.begin();
    TVector<int>::iterator b = v.begin();
    EXPECT_TRUE(a == b);
    ++a;
    EXPECT_TRUE(a != b);
    --a;
    EXPECT_TRUE(a == b);
}

TEST(ClassVector, range_based_for) {
    TVector<double> v({ 1, 2, 3, 4 });
    double sum = 0;
    for (const auto& x : v) sum += x;
    EXPECT_DOUBLE_EQ(10.0, sum);

    for (auto& x : v) x *= 2;
    EXPECT_DOUBLE_EQ(2.0, v[0]);
    EXPECT_DOUBLE_EQ(8.0, v[3]);
}

TEST(ClassVector, iterator_with_ring_buffer) {
    TVector<int> v;
    for (int i = 1; i <= 15; ++i) v.push_back(i);
    v.pop_front();
    v.push_back(16);   // front=1, back=0 -> кольцо

    int expected = 2;
    for (TVector<int>::iterator it = v.begin(); it != v.end(); ++it) {
        EXPECT_EQ(expected++, *it);
    }
    EXPECT_EQ(17, expected);
}