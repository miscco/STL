// Copyright (c) Microsoft Corporation.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#define _CONTAINER_DEBUG_LEVEL 1

#include <algorithm>
#include <cassert>
#include <deque2>

#include <test_death.hpp>
using namespace std;

using SV = deque2<int>;
using IT = typename SV::iterator;

SV empty_vector;
SV full_vector = {1, 2, 3, 4, 5};

void test_case_operator_dereference_value_initialized_iterator() {
    IT it; // note: for IDL to work correctly, default-init and value-init are equivalent
    (void) *it; // cannot dereference value-initialized deque2 iterator
}

void test_case_operator_dereference_end_iterator() {
    SV sv(full_vector);
    IT it = sv.end();
    (void) *it; // cannot dereference end deque2 iterator
}

void test_case_operator_arrow_value_initialized_iterator() {
    IT it;
    (void) it.operator->(); // cannot dereference value-initialized deque2 iterator
}

void test_case_operator_arrow_end_iterator() {
    SV sv(full_vector);
    IT it = sv.end();
    (void) it.operator->(); // cannot dereference end deque2 iterator
}

void test_case_operator_preincrement_value_initialized_iterator() {
    IT it;
    ++it; // cannot increment value-initialized deque2 iterator
}

void test_case_operator_preincrement_after_end() {
    SV sv(full_vector);
    IT it = sv.end();
    ++it; // cannot increment deque2 iterator past end
}

void test_case_operator_predecrement_value_initialized_iterator() {
    IT it;
    --it; // cannot decrement value-initialized deque2 iterator
}

void test_case_operator_predecrement_before_begin() {
    SV sv(full_vector);
    IT it = sv.begin();
    --it; // cannot decrement deque2 iterator before begin
}

void test_case_operator_advance_value_initialized_iterator() {
    IT it;
    it += 1; // cannot seek value-initialized deque2 iterator
}

void test_case_operator_advance_value_initialized_iterator_zero() {
    IT it;
    it += 0; // OK
}

void test_case_operator_advance_before_begin() {
    SV sv(full_vector);
    IT it = sv.begin();
    it += -1; // cannot seek deque2 iterator before begin
}

void test_case_operator_advance_after_end() {
    SV sv(full_vector);
    IT it = sv.end();
    it += 1; // cannot seek deque2 iterator after end
}

void test_case_operator_retreat_value_initialized_iterator() {
    IT it;
    it -= 1; // cannot seek value-initialized deque2 iterator
}

void test_case_operator_retreat_value_initialized_iterator_zero() {
    IT it;
    it -= 0; // OK
}

void test_case_operator_retreat_before_begin() {
    SV sv(full_vector);
    IT it = sv.begin();
    it -= 1; // cannot seek deque2 iterator before begin
}

void test_case_operator_retreat_after_end() {
    SV sv(full_vector);
    IT it = sv.end();
    it -= -1; // cannot seek deque2 iterator after end
}

void test_case_operator_subtract_incompatible_different_data() {
    SV sp1(full_vector);
    SV sp2(empty_vector);
    (void) (sp1.begin() - sp2.begin()); // cannot subtract incompatible deque2 iterators
}

void test_case_operator_subtract_incompatible_value_initialized() {
    SV sv(full_vector);
    (void) (sv.begin() - IT{}); // cannot subtract incompatible deque2 iterators
}

void test_case_operator_equal_incompatible_different_data() {
    SV sp1(full_vector);
    SV sp2(empty_vector);
    (void) (sp1.begin() == sp2.begin()); // cannot compare incompatible deque2 iterators for equality
}

void test_case_operator_equal_incompatible_value_initialized() {
    SV sv(full_vector);
    (void) (sv.begin() == IT{}); // cannot compare incompatible deque2 iterators for equality
}

void test_case_operator_less_incompatible_different_data() {
    SV sp1(full_vector);
    SV sp2(empty_vector);
    (void) (sp1.begin() < sp2.begin()); // cannot compare incompatible deque2 iterators
}

void test_case_operator_less_incompatible_value_initialized() {
    SV sv(full_vector);
    (void) (sv.begin() < IT{}); // cannot compare incompatible deque2 iterators
}

void test_case_algorithm_incompatible_different_data() {
    SV sp1(full_vector);
    SV sp2(empty_vector);
    (void) find(sp1.begin(), sp2.begin(), -1); // deque2 iterators from different views do not form a range
}

void test_case_algorithm_incompatible_value_initialized() {
    SV sv(full_vector);
    (void) find(sv.begin(), IT{}, -1); // deque2 iterators from different views do not form a range
}

void test_case_algorithm_incompatible_transposed() {
    SV sv(full_vector);
    (void) find(sv.end(), sv.begin(), -1); // deque2 iterator range transposed
}

void test_case_operator_subscript_out_of_range() {
    SV sv(empty_vector);
    (void) sv[1]; // deque2 index out of range
}

void test_case_front_empty() {
    SV sv{};
    (void) sv.front(); // front of empty deque2
}

void test_case_front_const_empty() {
    const SV sv{};
    (void) sv.front(); // front of empty deque2
}

void test_case_back_empty() {
    SV sv{};
    (void) sv.back(); // back of empty deque2
}

void test_case_back_const_empty() {
    const SV sv{};
    (void) sv.back(); // back of empty deque2
}

void test_case_insert_invalid_iterator_lvalue() {
    SV sv1;
    SV sv2;
    const int val = 5;
    (void) sv1.insert(sv2.begin(), val); // insert iterator outside range
}

void test_case_insert_invalid_iterator_rvalue() {
    SV sv1;
    SV sv2;
    (void) sv1.insert(sv2.begin(), 5); // insert iterator outside range
}

void test_case_insert_invalid_iterator_count_value() {
    SV sv1;
    SV sv2;
    (void) sv1.insert(sv2.begin(), 5, 4); // insert iterator outside range
}

void test_case_insert_invalid_iterator_iterator_range() {
    SV sv1;
    SV sv2;
    const int values[] = {1, 2, 3};
    (void) sv1.insert(sv2.begin(), values, values + 3); // insert iterator outside range
}

void test_case_insert_invalid_iterator_initalizer() {
    SV sv1;
    SV sv2;
    (void) sv1.insert(sv2.begin(), {1, 2, 3}); // insert iterator outside range
}

void test_case_emplace_invalid_iterator() {
    SV sv1;
    SV sv2;
    (void) sv1.emplace(sv2.begin(), 5); // emplace iterator outside range
}

void test_case_pop_back_empty() {
    SV sv;
    sv.pop_back(); // deque2 pop_back of empty range
}

void test_case_pop_back_dereference_invalidated() {
    SV sv   = {1, 2};
    auto it = sv.begin() + 1;
    sv.pop_back();
    (void) *it; // dereference of invalidated iterator
}

void test_case_pop_front_empty() {
    SV sv;
    sv.pop_front(); // deque2 pop_front of empty range
}

void test_case_pop_front_dereference_invalidated() {
    SV sv   = {1, 2};
    auto it = sv.begin();
    sv.pop_front();
    (void) *it; // dereference of invalidated iterator
}

void test_case_erase_empty() {
    SV sv;
    sv.erase(sv.begin()); // deque2 erase of empty range
}

void test_case_erase_invalid_iterator() {
    SV sv1 = {1, 2, 3};
    SV sv2;
    (void) sv1.erase(sv2.begin()); // erase iterator outside range
}

void test_case_erase_invalid_iterator_range1() {
    SV sv1 = {1, 2, 3};
    SV sv2;
    (void) sv1.erase(sv2.begin(), sv1.begin() + 2); // erase iterator outside range
}

void test_case_erase_invalid_iterator_range2() {
    SV sv1 = {1, 2, 3};
    SV sv2;
    (void) sv1.erase(sv1.begin(), sv2.begin()); // erase iterator outside range
}

int main(int argc, char* argv[]) {
    std_testing::death_test_executive exec([] {
        test_case_operator_advance_value_initialized_iterator_zero();
        test_case_operator_retreat_value_initialized_iterator_zero();
    });

#if _ITERATOR_DEBUG_LEVEL != 0
    exec.add_death_tests({
        test_case_operator_dereference_value_initialized_iterator,
        test_case_operator_dereference_end_iterator,
        test_case_operator_arrow_value_initialized_iterator,
        test_case_operator_arrow_end_iterator,
        test_case_operator_preincrement_value_initialized_iterator,
        test_case_operator_preincrement_after_end,
        test_case_operator_predecrement_value_initialized_iterator,
        test_case_operator_predecrement_before_begin,
        test_case_operator_advance_value_initialized_iterator,
        test_case_operator_advance_before_begin,
        test_case_operator_advance_after_end,
        test_case_operator_retreat_value_initialized_iterator,
        test_case_operator_retreat_before_begin,
        test_case_operator_retreat_after_end,
        test_case_operator_subtract_incompatible_different_data,
        test_case_operator_subtract_incompatible_value_initialized,
        test_case_operator_equal_incompatible_different_data,
        test_case_operator_equal_incompatible_value_initialized,
        test_case_operator_less_incompatible_different_data,
        test_case_operator_less_incompatible_value_initialized,
        test_case_algorithm_incompatible_different_data,
        test_case_algorithm_incompatible_value_initialized,
        test_case_algorithm_incompatible_transposed,
    });
#endif // _ITERATOR_DEBUG_LEVEL != 0

#if _ITERATOR_DEBUG_LEVEL == 2
    exec.add_death_tests({
        test_case_insert_invalid_iterator_lvalue,
        test_case_insert_invalid_iterator_rvalue,
        test_case_insert_invalid_iterator_count_value,
        test_case_insert_invalid_iterator_iterator_range,
        test_case_insert_invalid_iterator_initalizer,
        test_case_emplace_invalid_iterator,
        test_case_erase_invalid_iterator,
        test_case_erase_invalid_iterator_range1,
        test_case_erase_invalid_iterator_range2,
        test_case_pop_back_empty,
        test_case_pop_back_dereference_invalidated,
        test_case_pop_front_empty,
        test_case_pop_front_dereference_invalidated,
    });
#endif // _ITERATOR_DEBUG_LEVEL == 2

    // _CONTAINER_DEBUG_LEVEL tests
    exec.add_death_tests({
        test_case_operator_subscript_out_of_range,
        test_case_front_empty,
        test_case_front_const_empty,
        test_case_back_empty,
        test_case_back_const_empty,
        test_case_erase_empty,
    });

    return exec.run(argc, argv);
}
