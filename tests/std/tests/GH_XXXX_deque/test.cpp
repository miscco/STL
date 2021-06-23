// Copyright (c) Microsoft Corporation.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#define MSVC_INTERNAL_TESTING 0

#include <algorithm>
#include <assert.h>
#include <deque2>
#include <memory>
#include <numeric>
#include <utility>

#include <input_iterator.hpp>

using namespace std;

static constexpr int input[]       = {0, 1, 2, 3, 4, 5};
static constexpr int short_input[] = {1, 2, 3, 4};
using deq                          = deque2<int>;

template <class Range1, class Range2>
_CONSTEXPR23_CONTAINER bool equal(const Range1& range1, const Range2& range2) {
    return std::equal(begin(range1), end(range1), begin(range2), end(range2));
}

_CONSTEXPR23_CONTAINER void test_constructors() {
    { // special member functions
        deq construct_iter_range(begin(input), end(input));
        assert(equal(construct_iter_range, input));
        deq default_construct;
        assert(default_construct.empty());

        deq copy_construct(construct_iter_range);
        assert(equal(copy_construct, input));

        deq move_construct(move(copy_construct));
        assert(equal(move_construct, input));
        assert(copy_construct.empty());

        deq copy_assignment_empty{3, 4};
        copy_assignment_empty = default_construct;
        assert(copy_assignment_empty.empty());

        deq copy_assignment_grow{3, 4};
        copy_assignment_grow = construct_iter_range;
        assert(equal(copy_assignment_grow, input));

        const deq short_range{begin(short_input), end(short_input)};
        deq copy_assignment_shrink{2 + _Deque_block_size + 5, 4};
        copy_assignment_shrink = short_range;
        assert(equal(copy_assignment_shrink, short_input));

        deq move_assignment_empty{3, 4};
        move_assignment_empty = move(default_construct);
        assert(move_assignment_empty.empty());

        deq move_assignment_grow{3, 4};
        move_assignment_grow = move(copy_assignment_grow);
        assert(equal(move_assignment_grow, input));
        assert(copy_assignment_grow.empty());

        deq move_assignment_shrink{2 + _Deque_block_size + 5, 4};
        move_assignment_shrink = move(copy_assignment_shrink);
        assert(equal(move_assignment_shrink, short_input));
        assert(copy_assignment_shrink.empty());
    }

    // other constructors
    {
        deq construct_size(5);
        assert(construct_size.size() == 5);
        assert(all_of(begin(construct_size), end(construct_size), [](const int val) { return val == 0; }));
    }
    {
        deq construct_size_value(5, 7);
        assert(construct_size_value.size() == 5);
        assert(all_of(begin(construct_size_value), end(construct_size_value), [](const int val) { return val == 7; }));
    }
    {
        deq construct_iter_range(begin(input), end(input));
        assert(equal(construct_iter_range, input));
    }
    {
        const std_testing::input_iterator_container<int> input_range{begin(input), end(input)};
        deq construct_input_range(begin(input_range), end(input_range));
        assert(equal(construct_input_range, input));
    }
    {
        deq contruct_initializer_list_empty({});
        assert(contruct_initializer_list_empty.empty());
    }
    {
        deq contruct_initializer_list({0, 1, 2, 3, 4, 5});
        assert(equal(contruct_initializer_list, input));
    }
    {
        deq assignment_initializer_list_empty{3, 4};
        assignment_initializer_list_empty = {};
        assert(assignment_initializer_list_empty.empty());
    }
    {
        deq assignment_initializer_list_grow{3, 4};
        assignment_initializer_list_grow = {0, 1, 2, 3, 4, 5};
        assert(equal(assignment_initializer_list_grow, input));
    }
    {
        deq assignment_initializer_list_shrink{2 + _Deque_block_size + 5, 4};
        assignment_initializer_list_shrink = {1, 2, 3, 4};
        assert(equal(assignment_initializer_list_shrink, short_input));
    }

    { // allocator based constructors
        std::allocator<int> al{};
        deq default_al_construct(al);
        assert(default_al_construct.empty());
    }
}

_CONSTEXPR23_CONTAINER void test_assign() {
    {
        deq assign_count_val_empty;
        assign_count_val_empty.assign(5, 1);
        assert(equal(assign_count_val_empty, deq{1, 1, 1, 1, 1}));
    }
    {
        deq assign_count_val_zero;
        assign_count_val_zero.assign(0, 1);
        assert(equal(assign_count_val_zero, deq{}));
    }
    {
        deq assign_count_val_grow{3, 4};
        assign_count_val_grow.assign(5, 1);
        assert(equal(assign_count_val_grow, deq{1, 1, 1, 1, 1}));
    }
    {
        deq assign_count_val_shrink{3, 4};
        assign_count_val_shrink.assign(2, 1);
        assert(equal(assign_count_val_shrink, deq{1, 1}));
    }
    {
        deq assign_iter_range_empty;
        assign_iter_range_empty.assign(begin(input), end(input));
        assert(equal(assign_iter_range_empty, input));
    }
    {
        deq assign_iter_range_empty_range;
        assign_iter_range_empty_range.assign(begin(input), begin(input));
        assert(equal(assign_iter_range_empty_range, deq{}));
    }
    {
        deq assign_iter_range_grow{3, 4};
        assign_iter_range_grow.assign(begin(input), end(input));
        assert(equal(assign_iter_range_grow, input));
    }
    {
        deq assign_iter_range_shrink{2 + _Deque_block_size + 5, 4};
        assign_iter_range_shrink.assign(begin(short_input), end(short_input));
        assert(equal(assign_iter_range_shrink, short_input));
    }
    {
        const std_testing::input_iterator_container<int> input_range{begin(input), end(input)};
        deq assign_input_range_empty;
        assign_input_range_empty.assign(begin(input_range), end(input_range));
        assert(equal(assign_input_range_empty, input));

        deq assign_input_range_empty_range;
        assign_input_range_empty_range.assign(begin(input_range), begin(input_range));
        assert(equal(assign_input_range_empty_range, deq{}));

        deq assign_input_range_grow{3, 4};
        assign_input_range_grow.assign(begin(input_range), end(input_range));
        assert(equal(assign_input_range_grow, input));

        const std_testing::input_iterator_container<int> short_input_range{begin(short_input), end(short_input)};
        deq assign_input_range_shrink{2 + _Deque_block_size + 5, 4};
        assign_input_range_shrink.assign(begin(short_input_range), end(short_input_range));
        assert(equal(assign_input_range_shrink, short_input));
    }
    {
        deq assign_initializer_list_empty;
        assign_initializer_list_empty.assign({0, 1, 2, 3, 4, 5});
        assert(equal(assign_initializer_list_empty, input));
    }
    {
        deq assign_initializer_list_empty_range{3, 4};
        assign_initializer_list_empty_range.assign({});
        assert(assign_initializer_list_empty_range.empty());
    }
    {
        deq assign_initializer_list_grow{3, 4};
        assign_initializer_list_grow.assign({0, 1, 2, 3, 4, 5});
        assert(equal(assign_initializer_list_grow, input));
    }
    {
        deq assign_initializer_list_shrink{2 + _Deque_block_size + 5, 4};
        assign_initializer_list_shrink.assign({1, 2, 3, 4});
        assert(equal(assign_initializer_list_shrink, short_input));
    }
}

_CONSTEXPR23_CONTAINER void test_iterators() {
    deq range(begin(input), end(input));
    const deq const_range(begin(input), end(input));
    {
        auto b = range.begin();
        static_assert(is_same_v<decltype(b), deq::iterator>, "");
        assert(*b == 0);
    }
    {
        auto cb = range.cbegin();
        static_assert(is_same_v<decltype(cb), deq::const_iterator>, "");
        assert(*cb == 0);
    }
    {
        auto cb2 = const_range.begin();
        static_assert(is_same_v<decltype(cb2), deq::const_iterator>, "");
        assert(*cb2 == 0);
    }
    {
        auto e = range.end();
        static_assert(is_same_v<decltype(e), deq::iterator>, "");
        assert(*prev(e) == 5);

        auto ce = range.cend();
        static_assert(is_same_v<decltype(ce), deq::const_iterator>, "");
        assert(*prev(ce) == 5);
    }
    {
        auto ce2 = const_range.end();
        static_assert(is_same_v<decltype(ce2), deq::const_iterator>, "");
        assert(*prev(ce2) == 5);
    }
    {
        auto rb = range.rbegin();
        static_assert(is_same_v<decltype(rb), deq::reverse_iterator>, "");
        assert(*rb == 5);
    }
    {
        auto crb = range.crbegin();
        static_assert(is_same_v<decltype(crb), deq::const_reverse_iterator>, "");
        assert(*crb == 5);
    }
    {
        auto crb2 = const_range.rbegin();
        static_assert(is_same_v<decltype(crb2), deq::const_reverse_iterator>, "");
        assert(*crb2 == 5);
    }
    {
        auto re = range.rend();
        static_assert(is_same_v<decltype(re), deq::reverse_iterator>, "");
        assert(*prev(re) == 0);
    }
    {
        auto cre = range.crend();
        static_assert(is_same_v<decltype(cre), deq::const_reverse_iterator>, "");
        assert(*prev(cre) == 0);
    }
    {
        auto cre2 = const_range.rend();
        static_assert(is_same_v<decltype(cre2), deq::const_reverse_iterator>, "");
        assert(*prev(cre2) == 0);
    }
}

_CONSTEXPR23_CONTAINER void test_access() {
    deq range(begin(input), end(input));
    const deq const_range(begin(input), end(input));
    {
        auto&& op = range[3];
        static_assert(is_same_v<decltype(op), int&>, "");
        assert(op == 3);
    }
    {
        range[3]   = 4;
        auto&& op2 = range[3];
        static_assert(is_same_v<decltype(op2), int&>, "");
        assert(op2 == 4);
    }
    {
        auto&& cop = const_range[3];
        static_assert(is_same_v<decltype(cop), const int&>, "");
        assert(cop == 3);
    }
    {
        auto&& f = range.front();
        static_assert(is_same_v<decltype(f), int&>, "");
        assert(f == 0);
    }
    {
        auto&& cf = const_range.front();
        static_assert(is_same_v<decltype(cf), const int&>, "");
        assert(cf == 0);
    }
    {
        auto&& b = range.back();
        static_assert(is_same_v<decltype(b), int&>, "");
        assert(b == 5);
    }
    {
        auto&& cb = const_range.back();
        static_assert(is_same_v<decltype(cb), const int&>, "");
        assert(cb == 5);
    }
}

_CONSTEXPR23_CONTAINER void test_capacity() {
    deq range(begin(input), end(input));
    const deq const_range(begin(input), end(input));

    {
        auto e = range.empty();
        static_assert(is_same_v<decltype(e), bool>, "");
        assert(!e);
    }
    {
        auto ec = const_range.empty();
        static_assert(is_same_v<decltype(ec), bool>, "");
        assert(!ec);
    }
    {
        auto s = range.size();
        static_assert(is_same_v<decltype(s), size_t>, "");
        assert(s == size(input));
    }
    {
        auto sc = const_range.size();
        static_assert(is_same_v<decltype(sc), size_t>, "");
        assert(sc == size(input));
    }
    {
        auto ms = range.max_size();
        static_assert(is_same_v<decltype(ms), size_t>, "");
        assert(ms == size_t(-1) / sizeof(int));
    }
    {
        auto msc = const_range.max_size();
        static_assert(is_same_v<decltype(msc), size_t>, "");
        assert(msc == size_t(-1) / sizeof(int));
    }
    {
        deq resize_from_empty;
        resize_from_empty.resize(3);
        assert(equal(resize_from_empty, deq{0, 0, 0}));
    }
    {
        deq resize_value_from_empty;
        resize_value_from_empty.resize(3, 2);
        assert(equal(resize_value_from_empty, deq{2, 2, 2}));
    }
    {
        deq resize_shrink(begin(input), end(input));
        resize_shrink.resize(1);
        assert(equal(resize_shrink, deq{0}));
    }
    {
        deq resize_value_shrink(begin(input), end(input));
        resize_value_shrink.resize(1, 5);
        assert(equal(resize_value_shrink, deq{0}));
    }
    {
        deq resize_grow(3, 4);
        resize_grow.resize(5);
        assert(equal(resize_grow, deq{4, 4, 4, 0, 0}));
    }
    {
        deq resize_value_grow(3, 4);
        resize_value_grow.resize(5, 2);
        assert(equal(resize_value_grow, deq{4, 4, 4, 2, 2}));
    }
    {
        deq resize_equal(3, 4);
        resize_equal.resize(3);
        assert(equal(resize_equal, deq{4, 4, 4}));
    }
    {
        deq resize_value_equal(3, 4);
        resize_value_equal.resize(3, 2);
        assert(equal(resize_value_equal, deq{4, 4, 4}));
    }
}

_CONSTEXPR23_CONTAINER void test_emplace() {
    {
        deq emplace_back = {0, 1, 2, 3, 4};
#if _HAS_CXX17
        auto&& res = emplace_back.emplace_back(5);
        static_assert(is_same_v<decltype(res), int&>, "");
        assert(res == 5);
#else // ^^^ _HAS_CXX17 ^^^ / vvv !_HAS_CXX17 vvv
        emplace_back.emplace_back(5);
#endif // !_HAS_CXX17
        assert(equal(emplace_back, input));
    }
    {
        deq emplace_back_empty;
#if _HAS_CXX17
        auto&& res = emplace_back_empty.emplace_back(42);
        static_assert(is_same_v<decltype(res), int&>, "");
        assert(res == 42);
#else // ^^^ _HAS_CXX17 ^^^ / vvv !_HAS_CXX17 vvv
        emplace_back_empty.emplace_back(42);
#endif // !_HAS_CXX17
        assert(equal(emplace_back_empty, deq{42}));
    }
    {
        deq push_back_lvalue        = {0, 1, 2, 3, 4};
        const int to_be_pushed_back = 5;
        push_back_lvalue.push_back(to_be_pushed_back);
        assert(equal(push_back_lvalue, input));
    }
    {
        deq push_back_rvalue = {0, 1, 2, 3, 4};
        push_back_rvalue.push_back(5);
        assert(equal(push_back_rvalue, input));
    }
    {
        deq emplace_front = {1, 2, 3, 4, 5};
#if _HAS_CXX17
        auto&& res = emplace_front.emplace_front(0);
        static_assert(is_same_v<decltype(res), int&>, "");
        assert(res == 0);
#else // ^^^ _HAS_CXX17 ^^^ / vvv !_HAS_CXX17 vvv
        emplace_front.emplace_front(0);
#endif // !_HAS_CXX17
        assert(equal(emplace_front, input));
    }
    {
        deq emplace_front_empty;
#if _HAS_CXX17
        auto&& res = emplace_front_empty.emplace_front(42);
        static_assert(is_same_v<decltype(res), int&>, "");
        assert(res == 42);
#else // ^^^ _HAS_CXX17 ^^^ / vvv !_HAS_CXX17 vvv
        emplace_front_empty.emplace_front(42);
#endif // !_HAS_CXX17
        assert(equal(emplace_front_empty, deq{42}));
    }
    {
        deq push_front_lvalue        = {1, 2, 3, 4, 5};
        const int to_be_pushed_front = 0;
        push_front_lvalue.push_front(to_be_pushed_front);
        assert(equal(push_front_lvalue, input));
    }
    {
        deq push_front_rvalue = {1, 2, 3, 4, 5};
        push_front_rvalue.push_front(0);
        assert(equal(push_front_rvalue, input));
    }
    {
        deq emplace_begin = {1, 2, 3, 4, 5};
        auto res          = emplace_begin.emplace(emplace_begin.begin(), 0);
        static_assert(is_same_v<decltype(res), deq::iterator>, "");
        assert(equal(emplace_begin, input));
        assert(res == emplace_begin.begin());
    }
    {
        deq emplace_begin_empty;
        auto res = emplace_begin_empty.emplace(emplace_begin_empty.begin(), 42);
        static_assert(is_same_v<decltype(res), deq::iterator>, "");
        assert(equal(emplace_begin_empty, deq{42}));
        assert(res == emplace_begin_empty.begin());
    }
    {
        deq emplace_end = {0, 1, 2, 3, 4};
        auto res        = emplace_end.emplace(emplace_end.end(), 5);
        static_assert(is_same_v<decltype(res), deq::iterator>, "");
        assert(equal(emplace_end, input));
        assert(res == emplace_end.end() - 1);
    }
    {
        deq emplace_end_empty;
        auto res = emplace_end_empty.emplace(emplace_end_empty.end(), 42);
        static_assert(is_same_v<decltype(res), deq::iterator>, "");
        assert(equal(emplace_end_empty, deq{42}));
        assert(res == emplace_end_empty.end() - 1);
    }
#if 0
    {
        deq emplace_middle       = {0, 1, 2, 4, 5};
        auto res           = emplace_middle.emplace(emplace.begin() + 3, 3);
        static_assert(is_same_v<decltype(res), deq::iterator>, "");
        assert(equal(emplace_middle, input));
        assert(res == emplace_middle.begin() + 3);
    }
    {
        deq emplace_middle_const      = {0, 1, 2, 4, 5};
        auto res   = emplace_middle_const.emplace(emplace_const.cbegin() + 3, 3);
        static_assert(is_same_v<decltype(res), deq::iterator>, "");
        assert(equal(emplace_middle_const, input));
        assert(res == emplace_middle_const.cbegin() + 3);
    }
#endif // 0
}

_CONSTEXPR23_CONTAINER void test_insert() {
    constexpr int expected_insert[] = {0, 3, 5};
    constexpr int to_be_inserted    = 3;

    {
        deq insert_lvalue = {0, 5};
        const auto res    = insert_lvalue.insert(insert_lvalue.begin() + 1, to_be_inserted);
        static_assert(is_same_v<decltype(res), deq::iterator>, "");
        assert(equal(insert_lvalue, res));
        assert(res == insert_lvalue.begin() + 1);
    }
    {
        deq insert_lvalue_const      = {0, 5};
        auto res_insert_lvalue_const = insert_lvalue_const.insert(insert_lvalue_const.cbegin() + 1, to_be_inserted);
        static_assert(is_same_v<decltype(res_insert_lvalue_const), deq::iterator>, "");
        assert(equal(insert_lvalue_const, expected_insert));
        assert(res_insert_lvalue_const == insert_lvalue_const.cbegin() + 1);
    }
    {
        deq insert_rvalue      = {0, 5};
        auto res_insert_rvalue = insert_rvalue.insert(insert_rvalue.begin() + 1, 3);
        static_assert(is_same_v<decltype(res_insert_rvalue), deq::iterator>, "");
        assert(equal(insert_rvalue, expected_insert));
        assert(res_insert_rvalue == insert_rvalue.begin() + 1);
    }
    {
        deq insert_rvalue_const      = {0, 5};
        auto res_insert_rvalue_const = insert_rvalue_const.insert(insert_rvalue_const.cbegin() + 1, 3);
        static_assert(is_same_v<decltype(res_insert_rvalue_const), deq::iterator>, "");
        assert(equal(insert_rvalue_const, expected_insert));
        assert(res_insert_rvalue_const == insert_rvalue_const.cbegin() + 1);
    }
    {
        deq insert_iter_range = {0, 5};
        auto res_insert_iter_range =
            insert_iter_range.insert(insert_iter_range.begin() + 1, begin(short_input), end(short_input));
        static_assert(is_same_v<decltype(res_insert_iter_range), deq::iterator>, "");
        assert(equal(insert_iter_range, input));
        assert(res_insert_iter_range == insert_iter_range.begin() + 1);
    }
    {
        deq insert_iter_range_const = {0, 5};
        auto res_insert_iter_range_const =
            insert_iter_range_const.insert(insert_iter_range_const.cbegin() + 1, begin(short_input), end(short_input));
        static_assert(is_same_v<decltype(res_insert_iter_range_const), deq::iterator>, "");
        assert(equal(insert_iter_range_const, input));
        assert(res_insert_iter_range_const == insert_iter_range_const.cbegin() + 1);
    }
    {
        deq insert_initializer      = {0, 5};
        auto res_insert_initializer = insert_initializer.insert(insert_initializer.begin() + 1, {1, 2, 3, 4});
        static_assert(is_same_v<decltype(res_insert_initializer), deq::iterator>, "");
        assert(equal(insert_initializer, input));
        assert(res_insert_initializer == insert_initializer.begin() + 1);
    }
    {
        deq insert_initializer_const = {0, 5};
        auto res_insert_initializer_const =
            insert_initializer_const.insert(insert_initializer_const.cbegin() + 1, {1, 2, 3, 4});
        static_assert(is_same_v<decltype(res_insert_initializer_const), deq::iterator>, "");
        assert(equal(insert_initializer_const, input));
        assert(res_insert_initializer_const == insert_initializer_const.cbegin() + 1);
    }
}

_CONSTEXPR23_CONTAINER void test_modifiers() {
    {
        deq clear(begin(input), end(input));
        clear.clear();
        assert(clear.empty());
    }
    {
        deq pop_back(begin(input), end(input));
        pop_back.pop_back();
        assert(equal(pop_back, deq{0, 1, 2, 3, 4}));
    }
    {
        deq pop_back_all{42};
        pop_back_all.pop_back();
        assert(pop_back_all.empty());
    }
    {
        deq pop_front(begin(input), end(input));
        pop_front.pop_front();
        assert(equal(pop_front, deq{1, 2, 3, 4, 5}));
    }
    {
        deq pop_front_all{42};
        pop_front_all.pop_front();
        assert(pop_front_all.empty());
    }
    {
        deq swap_first(begin(input), end(input));
        deq swap_second(begin(short_input), end(short_input));

        swap_first.swap(swap_second);
        assert(equal(swap_first, short_input));
        assert(equal(swap_second, input));

        swap(swap_first, swap_second);
        assert(equal(swap_first, input));
        assert(equal(swap_second, short_input));
    }
}

_CONSTEXPR23_CONTAINER void test_erase() {
    int iota_range[6 * _Deque_block_size];
    std::iota(begin(iota_range), end(iota_range), 0);

    // erase single element
    {
        deq erase_iter_single_block(begin(input), end(input));
        const auto res = erase_iter_single_block.erase(erase_iter_single_block.begin() + 2);
        assert(equal(erase_iter_single_block, deq{0, 1, 3, 4, 5}));
        assert(res - erase_iter_single_block.begin() == 2);
        assert(*res == 3);
    }
    {
        deq erase_iter_double_block_shift_left(begin(iota_range), begin(iota_range) + _Deque_block_size + 5);
        const auto res = erase_iter_double_block_shift_left.erase(erase_iter_double_block_shift_left.begin() + 5);
        assert(res - erase_iter_double_block_shift_left.begin() == 5);
    }
    {
        deq erase_iter_multi_block_shift_left(begin(iota_range), begin(iota_range) + 3 * _Deque_block_size + 5);
        const auto res =
            erase_iter_multi_block_shift_left.erase(erase_iter_multi_block_shift_left.begin() + _Deque_block_size + 5);
        assert(res - erase_iter_multi_block_shift_left.begin() == _Deque_block_size + 5);
    }
    {
        deq erase_iter_double_block_shift_right(begin(iota_range), begin(iota_range) + _Deque_block_size + 5);
        const auto res = erase_iter_double_block_shift_right.erase(
            erase_iter_double_block_shift_right.begin() + _Deque_block_size + 2);
        assert(res - erase_iter_double_block_shift_right.begin() == _Deque_block_size + 2);
    }
    {
        deq erase_iter_multi_block_shift_right(begin(iota_range), begin(iota_range) + 3 * _Deque_block_size + 5);
        const auto res = erase_iter_multi_block_shift_right.erase(
            erase_iter_multi_block_shift_right.begin() + 2 * _Deque_block_size + 5);
        assert(res - erase_iter_multi_block_shift_right.begin() == 2 * _Deque_block_size + 5);
    }
    {
        deq erase_const_iter(begin(input), end(input));
        const auto res = erase_const_iter.erase(erase_const_iter.cbegin() + 2);
        assert(equal(erase_const_iter, deq{0, 1, 3, 4, 5}));
        assert(res - erase_const_iter.cbegin() == 2);
        assert(*res == 3);
    }
    {
        deq erase_begin(begin(input), end(input));
        const auto res = erase_begin.erase(erase_begin.begin());
        assert(equal(erase_begin, deq{1, 2, 3, 4, 5}));
        assert(res == erase_begin.begin());
        assert(*res == 1);
    }
    {
        deq erase_end(begin(input), end(input));
        const auto res = erase_end.erase(erase_end.end());
        assert(equal(erase_end, deq{0, 1, 2, 3, 4, 5}));
        assert(res == erase_end.end());
    }
    {
        deq erase_empty;
        const auto res = erase_empty.erase(erase_empty.end());
        assert(erase_empty.empty());
        assert(res == erase_empty.end());
    }
    {
        deq erase_all{42};
        const auto res = erase_all.erase(erase_all.begin());
        assert(erase_all.empty());
        assert(res == erase_all.end());
    }

    // erase range
    {
        deq erase_range(begin(input), end(input));
        const auto res = erase_range.erase(erase_range.begin() + 2, erase_range.begin() + 4);
        assert(equal(erase_range, deq{0, 1, 4, 5}));
        assert(res == erase_range.begin() + 2);
        assert(*res == 4);
    }
    {
        deq erase_all(begin(input), end(input));
        const auto res = erase_all.erase(erase_all.begin(), erase_all.end());
        assert(equal(erase_all, deq{}));
        assert(res == erase_all.end());
    }
    {
        deq erase_range_till_end(begin(input), end(input));
        const auto res = erase_range_till_end.erase(erase_range_till_end.begin() + 2, erase_range_till_end.end());
        assert(equal(erase_range_till_end, deq{0, 1}));
        assert(res == erase_range_till_end.end());
    }
    {
        deq erase_range_from_begin(begin(input), end(input));
        const auto res =
            erase_range_from_begin.erase(erase_range_from_begin.begin(), erase_range_from_begin.begin() + 4);
        assert(equal(erase_range_from_begin, deq{4, 5}));
        assert(res == erase_range_from_begin.begin());
        assert(*res == 4);
    }
    {
        deq erase_range_empty(begin(input), end(input));
        const auto res = erase_range_empty.erase(erase_range_empty.begin() + 2, erase_range_empty.begin() + 2);
        assert(equal(erase_range_empty, deq{0, 1, 2, 3, 4, 5}));
        assert(res == erase_range_empty.begin() + 2);
        assert(*res == 2);
    }
    {
        deq erase_range_empty_at_end(begin(input), end(input));
        const auto res = erase_range_empty_at_end.erase(erase_range_empty_at_end.end(), erase_range_empty_at_end.end());
        assert(equal(erase_range_empty_at_end, deq{0, 1, 2, 3, 4, 5}));
        assert(res == erase_range_empty_at_end.end());
    }
    {
        deq erase_range_matching_single_block(begin(iota_range), begin(iota_range) + 2 * _Deque_block_size);
        const auto res =
            erase_range_matching_single_block.erase(erase_range_matching_single_block.begin() + _Deque_block_size / 2,
                erase_range_matching_single_block.begin() + 3 * _Deque_block_size / 2);
        assert(erase_range_matching_single_block.size() == _Deque_block_size);
        assert(res == erase_range_matching_single_block.begin() + _Deque_block_size / 2);
    }
    {
        deq erase_range_matching_single_block_partial(begin(iota_range), begin(iota_range) + 2 * _Deque_block_size - 1);
        const auto res = erase_range_matching_single_block_partial.erase(
            erase_range_matching_single_block_partial.begin() + _Deque_block_size / 2,
            erase_range_matching_single_block_partial.begin() + 3 * _Deque_block_size / 2);
        assert(erase_range_matching_single_block_partial.size() == _Deque_block_size - 1);
        assert(res == erase_range_matching_single_block_partial.begin() + _Deque_block_size / 2);
    }
    {
        deq erase_range_matching_multi_block_shift_left(begin(iota_range), begin(iota_range) + 5 * _Deque_block_size);
        const auto res = erase_range_matching_multi_block_shift_left.erase(
            erase_range_matching_multi_block_shift_left.begin() + 5 * _Deque_block_size / 2,
            erase_range_matching_multi_block_shift_left.begin() + 7 * _Deque_block_size / 2);
        assert(erase_range_matching_multi_block_shift_left.size() == 4 * _Deque_block_size);
        assert(res == erase_range_matching_multi_block_shift_left.begin() + 5 * _Deque_block_size / 2);
    }
    {
        deq erase_range_matching_multi_block_shift_left_partial(
            begin(iota_range), begin(iota_range) + 5 * _Deque_block_size - 1);
        const auto res = erase_range_matching_multi_block_shift_left_partial.erase(
            erase_range_matching_multi_block_shift_left_partial.begin() + 7 * _Deque_block_size / 2,
            erase_range_matching_multi_block_shift_left_partial.begin() + 9 * _Deque_block_size / 2);
        assert(erase_range_matching_multi_block_shift_left_partial.size() == 4 * _Deque_block_size - 1);
        assert(res == erase_range_matching_multi_block_shift_left_partial.begin() + 7 * _Deque_block_size / 2);
    }
    {
        deq erase_range_matching_multi_block_shift_right(begin(iota_range), begin(iota_range) + 5 * _Deque_block_size);
        const auto res = erase_range_matching_multi_block_shift_right.erase(
            erase_range_matching_multi_block_shift_right.begin() + _Deque_block_size / 2,
            erase_range_matching_multi_block_shift_right.begin() + 3 * _Deque_block_size / 2);
        assert(erase_range_matching_multi_block_shift_right.size() == 4 * _Deque_block_size);
        assert(res == erase_range_matching_multi_block_shift_right.begin() + _Deque_block_size / 2);
    }
    {
        deq erase_range_matching_multi_block_shift_right_partial(
            begin(iota_range), begin(iota_range) + 5 * _Deque_block_size - 1);
        const auto res = erase_range_matching_multi_block_shift_right_partial.erase(
            erase_range_matching_multi_block_shift_right_partial.begin() + _Deque_block_size / 2,
            erase_range_matching_multi_block_shift_right_partial.begin() + 3 * _Deque_block_size / 2);
        assert(erase_range_matching_multi_block_shift_right_partial.size() == 4 * _Deque_block_size - 1);
        assert(res == erase_range_matching_multi_block_shift_right_partial.begin() + _Deque_block_size / 2);
    }
}

_CONSTEXPR23_CONTAINER void test_comparison() {
    deq first(begin(input), end(input));
    deq second(begin(input), end(input));
    deq third(begin(short_input), end(short_input));

    auto eq = first == second;
    static_assert(is_same_v<decltype(eq), bool>, "");
    assert(eq);

    auto ne = first != third;
    static_assert(is_same_v<decltype(ne), bool>, "");
    assert(ne);
}

_CONSTEXPR23_CONTAINER bool test_container() {
    test_constructors();
    test_assign();
    test_iterators();
    test_access();
    test_capacity();
    test_emplace();
    test_insert();
    test_modifiers();
    test_erase();
    test_comparison();
    return true;
}

_CONSTEXPR23_CONTAINER bool test_container_iterator() {
#if _HAS_CXX20
    static_assert(random_access_iterator<deq::iterator>);
    static_assert(random_access_iterator<deq::const_iterator>);
#endif // _HAS_CXX20

    deq range(begin(input), end(input));

    { // increment
        auto it = range.begin();
        assert(*++it == 1);
        assert(*it++ == 1);
        assert(*it == 2);

        auto cit = range.cbegin();
        assert(*++cit == 1);
        assert(*cit++ == 1);
        assert(*cit == 2);
    }

    { // advance
        auto it = range.begin() + 2;
        assert(*it == 2);
        it += 2;
        assert(*it == 4);

        auto cit = range.cbegin() + 2;
        assert(*cit == 2);
        cit += 2;
        assert(*cit == 4);
    }

    { // decrement
        auto it = range.end();
        assert(*--it == 5);
        assert(*it-- == 5);
        assert(*it == 4);

        auto cit = range.cend();
        assert(*--cit == 5);
        assert(*cit-- == 5);
        assert(*cit == 4);
    }

    { // advance back
        auto it = range.end() - 2;
        assert(*it == 4);
        it -= 2;
        assert(*it == 2);

        auto cit = range.cend() - 2;
        assert(*cit == 4);
        cit -= 2;
        assert(*cit == 2);
    }

    { // difference
        auto it1 = range.begin();
        auto it2 = range.end();
        assert(static_cast<size_t>(it2 - it1) == size(input));

        auto cit1 = range.cbegin();
        auto cit2 = range.cend();
        assert(static_cast<size_t>(cit2 - cit1) == size(input));

        assert(static_cast<size_t>(it2 - cit1) == size(input));
        assert(static_cast<size_t>(cit2 - it1) == size(input));
    }

    { // comparison
        auto it1 = range.begin();
        auto it2 = range.begin();
        auto it3 = range.end();

        assert(it1 == it2);
        assert(it1 != it3);
        assert(it1 < it3);
        assert(it1 <= it3);
        assert(it3 > it1);
        assert(it3 >= it1);
    }

    { // access
        auto it = range.begin() + 2;
        it[2]   = 3;
        assert(range[4] == 3);

        auto cit = range.cbegin() + 2;
        assert(cit[2] == 3);

        deque2<pair<int, int>> deque2 = {{1, 2}, {2, 3}};
        auto it2                      = deque2.begin();
        assert(it2->second == 2);

        auto cit2 = deque2.cbegin();
        assert(cit2->first == 1);
    }
    return true;
}

int main() {
    test_container();
    test_container_iterator();
#if defined(__cpp_lib_constexpr_dynamic_alloc) && !defined(__clang__) && _HAS_CXX23 // TRANSITION, LLVM-48606
    static_assert(test_container());
    static_assert(test_container_iterator());
#endif // _CONSTEXPR23_CONTAINER
}
