// Copyright (c) Microsoft Corporation.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#include <assert.h>
#include <initializer_list>
#include <optional>
#include <utility>
#include <variant>

using namespace std;

struct With_trivial_destrutor {
    int _val = 0;
    constexpr With_trivial_destrutor(const int val) noexcept : _val(val) {}
    constexpr With_trivial_destrutor(initializer_list<int> vals) noexcept : _val(*vals.begin()) {}

    constexpr bool operator==(const int right) const noexcept {
        return _val == right;
    }
};

struct With_nontrivial_destrutor {
    int _val = 0;
    constexpr With_nontrivial_destrutor(const int val) noexcept : _val(val) {}
    constexpr With_nontrivial_destrutor(initializer_list<int> vals) noexcept : _val(*vals.begin()) {}
    constexpr ~With_nontrivial_destrutor() {}

    constexpr bool operator==(const int right) const noexcept {
        return _val == right;
    }
};

template <class T>
_CONSTEXPR20_DYNALLOC bool test_optional() {
    { // empty construction
        optional<T> defaut_constructed;
        assert(!defaut_constructed.has_value());

        optional<T> nullopt_constructed{nullopt};
        assert(!nullopt_constructed.has_value());
    }

    { // construction from underlying type
        const T input{42};
        optional<T> copy_constructed{input};
        assert(copy_constructed.has_value());
        assert(*copy_constructed == 42);

        optional<T> move_constructed{T{42}};
        assert(move_constructed.has_value());
        assert(*move_constructed == 42);

        optional<T> copy_assigned;
        assert(!copy_assigned.has_value());
        copy_assigned = input;
        assert(copy_assigned.has_value());
        assert(*copy_assigned == 42);

        optional<T> move_assigned;
        assert(!move_assigned.has_value());
        move_assigned = T{42};
        assert(move_assigned.has_value());
        assert(*move_assigned == 42);
    }

    { // construction from convertible type
        const int input{42};
        optional<T> copy_constructed{input};
        assert(copy_constructed.has_value());
        assert(*copy_constructed == 42);

        optional<T> move_constructed{42};
        assert(move_constructed.has_value());
        assert(*move_constructed == 42);

        optional<T> copy_assigned;
        assert(!copy_assigned.has_value());
        copy_assigned = input;
        assert(copy_assigned.has_value());
        assert(*copy_assigned == 42);

        optional<T> move_assigned;
        assert(!move_assigned.has_value());
        move_assigned = 42;
        assert(move_assigned.has_value());
        assert(*move_assigned == 42);
    }

    { // construction from optional with same type
        optional<T> constructed{42};
        assert(constructed.has_value());
        assert(*constructed == 42);

        optional<T> copy_constructed{constructed};
        assert(copy_constructed.has_value());
        assert(*copy_constructed == 42);

        optional<T> move_constructed{move(constructed)};
        assert(move_constructed.has_value());
        assert(*move_constructed == 42);

        optional<T> copy_assigned;
        assert(!copy_assigned.has_value());
        copy_assigned = constructed;
        assert(copy_assigned.has_value());
        assert(*copy_assigned == 42);

        optional<T> move_assigned;
        assert(!move_assigned.has_value());
        move_assigned = move(constructed);
        assert(move_assigned.has_value());
        assert(*move_assigned == 42);
    }

    { // construction from optional with convertible types
        optional<T> constructed{42};
        optional<int> input{42};

        constructed = input;
        assert(constructed.has_value());
        assert(*constructed == 42);

        constructed = optional<int>{3};
        assert(constructed.has_value());
        assert(*constructed == 3);
    }

    { // emplace
        T input{42};
        optional<T> emplace_copy;
        assert(!emplace_copy.has_value());
        emplace_copy.emplace(input);
        assert(emplace_copy.has_value());
        assert(*emplace_copy == 42);

        optional<T> emplace_move;
        assert(!emplace_move.has_value());
        emplace_move.emplace(T{42});
        assert(emplace_move.has_value());
        assert(*emplace_move == 42);

        optional<T> emplace_conversion;
        assert(!emplace_conversion.has_value());
        emplace_conversion.emplace(42);
        assert(emplace_conversion.has_value());
        assert(*emplace_conversion == 42);

        optional<T> emplace_initializer_list;
        assert(!emplace_initializer_list.has_value());
        emplace_initializer_list.emplace({42, 43});
        assert(emplace_initializer_list.has_value());
        assert(*emplace_initializer_list == 42);
    }

    { // reset
        optional<T> resetted{42};
        resetted.reset();
        assert(!resetted.has_value());
    }

    { // swap
        optional<T> left{42};
        optional<T> right{3};
        assert(*left == 42);
        assert(*right == 3);

        left.swap(right);
        assert(*left == 3);
        assert(*right == 42);

        swap(left, right);
        assert(*left == 42);
        assert(*right == 3);
    }
    return true;
}

int main() {
    test_optional<With_trivial_destrutor>();
    test_optional<With_nontrivial_destrutor>();
#ifdef __cpp_lib_constexpr_dynamic_alloc
    static_assert(test_optional<With_trivial_destrutor>());
    static_assert(test_optional<With_nontrivial_destrutor>());
#endif // __cpp_lib_constexpr_dynamic_alloc
}
