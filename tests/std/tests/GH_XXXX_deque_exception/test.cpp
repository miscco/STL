// Copyright (c) Microsoft Corporation.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#include <assert.h>
#include <deque2>
#include <iostream>
#include <utility>

using namespace std;

int default_construction_count{0};
int implicit_construction_count{0};
int copy_construction_count{0};
int move_construction_count{0};
int copy_assignment_count{0};
int move_assignment_count{0};
int destruction_count{0};

[[nodiscard]] constexpr bool nothing_leaked() {
    const int num_constructions = exchange(default_construction_count, 0) //
                                + exchange(implicit_construction_count, 0) //
                                + exchange(copy_construction_count, 0) //
                                + exchange(move_construction_count, 0);
    const int num_destructions = exchange(destruction_count, 0);
    return num_constructions == num_destructions;
}

enum ThrowType {
    DefaultConstructed = 0,
    ImplictConstructed = 1,
    CopyConstructed    = 2,
    MoveConstructed    = 3,
    CopyAssigned       = 4,
    MoveAssigned       = 5,
};

template <int ThrowAtDefaultConstruction, int ThrowAtImplicitConstruction = 0, //
    int ThrowAtCopyConstruction = 0, int ThrowAtMoveConstruction = 0, //
    int ThrowAtCopyAssignment = 0, int ThrowAtMoveAssignment = 0>
struct Evil {
    Evil() {
        if constexpr (ThrowAtDefaultConstruction >= 0) {
            if (default_construction_count == ThrowAtDefaultConstruction) {
                // TRANSITION fix initialization
                // throw ThrowType::DefaultConstructed;
            }
        }
        ++default_construction_count;
    }
    Evil(const int) {
        if constexpr (ThrowAtImplicitConstruction >= 0) {
            if (implicit_construction_count == ThrowAtImplicitConstruction) {
                throw ThrowType::ImplictConstructed;
            }
        }
        ++implicit_construction_count;
    }
    Evil(const Evil&) {
        if constexpr (ThrowAtCopyConstruction >= 0) {
            if (copy_construction_count == ThrowAtCopyConstruction) {
                throw ThrowType::CopyConstructed;
            }
        }
        ++copy_construction_count;
    }
    Evil(Evil&&) {
        if constexpr (ThrowAtMoveConstruction >= 0) {
            if (move_construction_count == ThrowAtMoveConstruction) {
                throw ThrowType::MoveConstructed;
            }
        }
        ++move_construction_count;
    }
    Evil& operator=(const Evil&) {
        if constexpr (ThrowAtCopyAssignment >= 0) {
            if (copy_assignment_count == ThrowAtCopyAssignment) {
                throw ThrowType::CopyAssigned;
            }
        }
        ++copy_assignment_count;
        return *this;
    }
    Evil& operator=(Evil&&) {
        if constexpr (ThrowAtCopyAssignment >= 0) {
            if (move_assignment_count == ThrowAtMoveAssignment) {
                throw ThrowType::MoveAssigned;
            }
        }
        ++move_assignment_count;
        return *this;
    }
    ~Evil() {
        ++destruction_count;
    }
};

static constexpr int input[] = {0, 1, 2, 3, 4, 5};
// static constexpr int short_input[]      = {1, 2, 3, 4};
static constexpr size_t vector_capacity = 6;

template <int ThrowAtDefaultConstruction, int ThrowAtImplicitConstruction = 0, //
    int ThrowAtCopyConstruction = 0, int ThrowAtMoveConstruction = 0, //
    int ThrowAtCopyAssignment = 0, int ThrowAtMoveAssignment = 0>
using vec = deque2<Evil<ThrowAtDefaultConstruction, ThrowAtImplicitConstruction, ThrowAtCopyConstruction,
                       ThrowAtMoveConstruction, ThrowAtCopyAssignment, ThrowAtMoveAssignment>,
    vector_capacity>;

void test_exceptions() {
    { // constructors
        try {
            vec<0> default_construct{};
        } catch (...) {
            assert(false && "default construction failed");
        }
        assert(nothing_leaked() && "default construction leaked");

        try {
            vec<0, -1> construct_range(input);
            [[maybe_unused]] vec<0, -1> copy_construct(construct_range);
            assert(false && "copy construction no exception");
        } catch (ThrowType e) {
            assert(e == ThrowType::CopyConstructed && "copy construction wrong exception");
        } catch (...) {
            assert(false && "copy construction wrong exception type");
        }
        assert(nothing_leaked() && "copy construction leaked");

        try {
            vec<0, -1> construct_range(input);
            [[maybe_unused]] vec<0, -1> move_construct(move(construct_range));
            assert(false && "move construction no exception");
        } catch (ThrowType e) {
            assert(e == ThrowType::MoveConstructed && "move construction wrong exception");
        } catch (...) {
            assert(false && "move construction wrong exception type");
        }
        assert(nothing_leaked() && "move construction leaked");

        // other constructors
        try {
            [[maybe_unused]] vec<0> construct_size(5);
            // TRANSITION fix initialization
            assert(true && "size construction no exception");
        } catch (ThrowType e) {
            assert(e == ThrowType::DefaultConstructed && "size construction wrong exception");
        } catch (...) {
            assert(false && "size construction wrong exception type");
        }
        assert(nothing_leaked() && "size construction leaked");

        try {
            [[maybe_unused]] vec<0> construct_size_value(5, 7);
            assert(false && "size value construction no exception");
        } catch (ThrowType e) {
            assert(e == ThrowType::ImplictConstructed && "size value construction wrong exception");
        } catch (...) {
            assert(false && "size value construction wrong exception type");
        }
        assert(nothing_leaked() && "size value construction leaked");

        try {
            [[maybe_unused]] vec<0> construct_range(input);
            assert(false && "range construction no exception");
        } catch (ThrowType e) {
            assert(e == ThrowType::ImplictConstructed && "range construction wrong exception");
        } catch (...) {
            assert(false && "range construction wrong exception type");
        }
        assert(nothing_leaked() && "range construction leaked");

        try {
            [[maybe_unused]] vec<0> construct_iter_range(begin(input), end(input));
            assert(false && "iter range construction no exception");
        } catch (ThrowType e) {
            assert(e == ThrowType::ImplictConstructed && "iter range construction wrong exception");
        } catch (...) {
            assert(false && "iter range construction wrong exception type");
        }
        assert(nothing_leaked() && "iter range construction leaked");

        try {
            [[maybe_unused]] vec<0> contruct_initializer_list({0, 1, 2, 3, 4, 5});
            assert(false && "initializer construction no exception");
        } catch (ThrowType e) {
            assert(e == ThrowType::ImplictConstructed && "initializer construction wrong exception");
        } catch (...) {
            assert(false && "initializer construction no exception type");
        }
        assert(nothing_leaked() && "initializer construction leaked");

        // assignments
        try {
            vec<0, -1> construct_range(input);
            vec<0, -1> copy_assigned(input);
            copy_assigned = construct_range;
            assert(false && "copy assignment no exception");
        } catch (ThrowType e) {
            assert(e == ThrowType::CopyAssigned && "copy assignment wrong exception");
        } catch (...) {
            assert(false && "copy assignment no exception type");
        }
        assert(nothing_leaked() && "copy assignment leaked");

        try {
            vec<0, -1> construct_range(input);
            vec<0, -1> copy_assigned(input);
            copy_assigned = move(construct_range);
            assert(false && "move assignment no exception");
        } catch (ThrowType e) {
            assert(e == ThrowType::MoveAssigned && "move assignment wrong exception");
        } catch (...) {
            assert(false && "move assignment wrong exception type");
        }
        assert(nothing_leaked() && "move assignment leaked");

        try {
            vec<0, -1> assignment_initializer_list(input);
            assignment_initializer_list = {0, 1, 2, 3, 4, 5};
            assert(false && "initializer assignment no exception");
        } catch (ThrowType e) {
            assert(e == ThrowType::CopyAssigned && "initializer assignment wrong exception");
        } catch (...) {
            assert(false && "initializer assignment wrong exception type");
        }
        assert(nothing_leaked() && "initializer assignment leaked");
    }

    { // assign
        try {
            vec<-1, -1, 3> assign_count_val{3, 4};
            assign_count_val.assign(5, 1);
            assert(false && "assign count value no exception");
        } catch (ThrowType e) {
            assert(e == ThrowType::CopyAssigned && "assign count value wrong exception");
        } catch (...) {
            assert(false && "assign count value wrong exception type");
        }
        assert(nothing_leaked() && "assign count value leaked");

        try {
            vec<-1, -1, -1> assign_range{3, 4};
            vec<-1, -1, -1> assign_input{input};
            assign_range.assign(assign_input);
            assert(false && "assign range no exception");
        } catch (ThrowType e) {
            assert(e == ThrowType::CopyAssigned && "assign range wrong exception");
        } catch (...) {
            assert(false && "assign range wrong exception type");
        }
        assert(nothing_leaked() && "assign range leaked");

        try {
            vec<-1, -1, -1> assign_iter_range{3, 4};
            vec<-1, -1, -1> assign_input{input};
            assign_iter_range.assign(begin(assign_input), end(assign_input));
            assert(false && "assign iter range no exception");
        } catch (ThrowType e) {
            assert(e == ThrowType::CopyAssigned && "assign iter range wrong exception");
        } catch (...) {
            assert(false && "assign iter range wrong exception type");
        }
        assert(nothing_leaked() && "assign iter range leaked");

        try {
            vec<-1, -1, -1> assign_initializer_list{3, 4};
            assign_initializer_list.assign({0, 1, 2, 3, 4, 5});
            assert(false && "assign initializer no exception");
        } catch (ThrowType e) {
            assert(e == ThrowType::CopyAssigned && "assign initializer wrong exception");
        } catch (...) {
            assert(false && "assign initializer wrong exception type");
        }
        assert(nothing_leaked() && "assign initializer leaked");
    }

    { // insert
        {
            vec<-1, -1, 2> insert_value_strong_guarantee{3, 4};
            try {
                const Evil<-1, -1, 2> to_be_inserted{3};
                insert_value_strong_guarantee.insert(insert_value_strong_guarantee.end(), to_be_inserted);
                assert(false && "insert lvalue end no exception");
            } catch (ThrowType e) {
                assert(e == ThrowType::CopyConstructed && "insert lvalue end wrong exception");
            } catch (...) {
                assert(false && "insert lvalue end wrong exception type");
            }
        }
        assert(nothing_leaked() && "insert lvalue end leaked");

        try {
            const Evil<-1, -1, 2> to_be_inserted{3};
            vec<-1, -1, 2> insert_value_temporary_construction{3, 4};
            insert_value_temporary_construction.insert(insert_value_temporary_construction.begin() + 1, to_be_inserted);
            assert(false && "insert lvalue temporary no exception");
        } catch (ThrowType e) {
            assert(e == ThrowType::CopyConstructed && "insert lvalue temporary wrong exception");
        } catch (...) {
            assert(false && "insert lvalue temporary wrong exception type");
        }
        assert(nothing_leaked() && "insert lvalue temporary leaked");

        try {
            const Evil<-1, -1, -1> to_be_inserted{3};
            vec<-1, -1, -1> insert_value_move_construction{3, 4};
            insert_value_move_construction.insert(insert_value_move_construction.begin() + 1, to_be_inserted);
            assert(false && "insert lvalue move construction no exception");
        } catch (ThrowType e) {
            assert(e == ThrowType::MoveConstructed && "insert lvalue move construction wrong exception");
        } catch (...) {
            assert(false && "insert lvalue move construction wrong exception type");
        }
        assert(nothing_leaked() && "insert lvalue construction move leaked");

        try {
            const Evil<-1, -1, -1, -1> to_be_inserted{3};
            vec<-1, -1, -1, -1> insert_value_move_construction{3, 4};
            insert_value_move_construction.insert(insert_value_move_construction.begin() + 1, to_be_inserted);
            assert(false && "insert lvalue move construction no exception");
        } catch (ThrowType e) {
            assert(e == ThrowType::MoveAssigned && "insert lvalue move construction wrong exception");
        } catch (...) {
            assert(false && "insert lvalue move construction wrong exception type");
        }
        assert(nothing_leaked() && "insert lvalue move construction leaked");

        /*
            vec insert_lvalue_const = {0, 5};
            const same_as<vec::iterator> auto res_insert_lvalue_const =
                insert_lvalue_const.insert(insert_lvalue_const.cbegin() + 1, to_be_inserted);
            assert(equal(insert_lvalue_const, expected_insert));
            assert(res_insert_lvalue_const == insert_lvalue_const.cbegin() + 1);

            vec insert_rvalue                                   = {0, 5};
            const same_as<vec::iterator> auto res_insert_rvalue = insert_rvalue.insert(insert_rvalue.begin() + 1, 3);
            assert(equal(insert_rvalue, expected_insert));
            assert(res_insert_rvalue == insert_rvalue.begin() + 1);

            vec insert_rvalue_const = {0, 5};
            const same_as<vec::iterator> auto res_insert_rvalue_const =
                insert_rvalue_const.insert(insert_rvalue_const.cbegin() + 1, 3);
            assert(equal(insert_rvalue_const, expected_insert));
            assert(res_insert_rvalue_const == insert_rvalue_const.cbegin() + 1);

            vec insert_range                                   = {0, 5};
            const same_as<vec::iterator> auto res_insert_range = insert_range.insert(insert_range.begin() + 1,
        short_input); assert(equal(insert_range, input)); assert(res_insert_range == insert_range.begin() + 1);

            vec insert_range_const = {0, 5};
            const same_as<vec::iterator> auto res_insert_range_const =
                insert_range_const.insert(insert_range_const.cbegin() + 1, short_input);
            assert(equal(insert_range_const, input));
            assert(res_insert_range_const == insert_range_const.cbegin() + 1);

            vec insert_iter_range = {0, 5};
            const same_as<vec::iterator> auto res_insert_iter_range =
                insert_iter_range.insert(insert_iter_range.begin() + 1, begin(short_input), end(short_input));
            assert(equal(insert_iter_range, input));
            assert(res_insert_iter_range == insert_iter_range.begin() + 1);

            vec insert_iter_range_const = {0, 5};
            const same_as<vec::iterator> auto res_insert_iter_range_const =
                insert_iter_range_const.insert(insert_iter_range_const.cbegin() + 1, begin(short_input),
        end(short_input)); assert(equal(insert_iter_range_const, input)); assert(res_insert_iter_range_const ==
        insert_iter_range_const.cbegin() + 1);

            vec insert_initializer = {0, 5};
            const same_as<vec::iterator> auto res_insert_initializer =
                insert_initializer.insert(insert_initializer.begin() + 1, {1, 2, 3, 4});
            assert(equal(insert_initializer, input));
            assert(res_insert_initializer == insert_initializer.begin() + 1);

            vec insert_initializer_const = {0, 5};
            const same_as<vec::iterator> auto res_insert_initializer_const =
                insert_initializer_const.insert(insert_initializer_const.cbegin() + 1, {1, 2, 3, 4});
            assert(equal(insert_initializer_const, input));
            assert(res_insert_initializer_const == insert_initializer_const.cbegin() + 1);
        }

        { // emplace
            vec emplace                                    = {0, 1, 2, 4, 5};
            const same_as<vec::iterator> auto res_emplaced = emplace.emplace(emplace.begin() + 3, 3);
            assert(equal(emplace, input));
            assert(res_emplaced == emplace.begin() + 3);

            vec emplace_const                                   = {0, 1, 2, 4, 5};
            const same_as<vec::iterator> auto res_emplace_const = emplace_const.emplace(emplace_const.cbegin() + 3, 3);
            assert(equal(emplace_const, input));
            assert(res_emplace_const == emplace_const.cbegin() + 3);

            vec emplace_back                      = {0, 1, 2, 3, 4};
            same_as<int&> auto&& res_emplace_back = emplace_back.emplace_back(5);
            assert(equal(emplace_back, input));
            assert(res_emplace_back++ == 5);
            assert(res_emplace_back == 6);

            vec push_back_lvalue                      = {0, 1, 2, 3, 4};
            const int to_be_pushed                    = 5;
            same_as<int&> auto&& res_push_back_lvalue = push_back_lvalue.push_back(to_be_pushed);
            assert(equal(push_back_lvalue, input));
            assert(res_push_back_lvalue++ == 5);
            assert(res_push_back_lvalue == 6);

            vec push_back_rvalue                      = {0, 1, 2, 3, 4};
            same_as<int&> auto&& res_push_back_rvalue = push_back_rvalue.push_back(5);
            assert(equal(push_back_rvalue, input));
            assert(res_push_back_rvalue++ == 5);
            assert(res_push_back_rvalue == 6);
        }

        { // resize
            vec resize_shrink{input};
            resize_shrink.resize(1);
            assert(equal(resize_shrink, vec{0}));

            vec resize_value_shrink{input};
            resize_value_shrink.resize(1, 5);
            assert(equal(resize_value_shrink, vec{0}));

            vec resize_grow(3, 4);
            resize_grow.resize(5);
            assert(equal(resize_grow, vec{4, 4, 4, 0, 0}));

            vec resize_value_grow(3, 4);
            resize_value_grow.resize(5, 2);
            assert(equal(resize_value_grow, vec{4, 4, 4, 2, 2}));
        */
    }
}


int main() {
    test_exceptions();
}
