// Copyright (c) Microsoft Corporation.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#include <algorithm>
#include <cassert>
#include <concepts>
#include <cstdlib>
#include <memory>
#include <ranges>
#include <span>
#include <utility>

#include <range_algorithm_support.hpp>

using namespace std;

#if __cpp_lib_raw_memory_algorithms >= 202106L

struct int_wrapper {
    int val = 10;

    constexpr int_wrapper() noexcept = default;
    constexpr int_wrapper(const int x) : val{x} {}
    constexpr int_wrapper(const int_wrapper&) = default;
    constexpr int_wrapper(int_wrapper&& that) {
        val = exchange(that.val, -1);
    }

    constexpr auto operator==(const int_wrapper& right) const {
        return val == right.val;
    }
    constexpr auto operator<=>(const int_wrapper&) const = default;
};

template <class T, size_t N>
struct holder {
    static_assert(N < ~size_t{0} / sizeof(T));
    T* _data = new T[N];

    constexpr holder() = default;
    constexpr ~holder() {
        delete[] _data;
    }

    constexpr auto as_span() noexcept {
        return span<T, N>{_data, N};
    }
};

struct instantiator {
    static constexpr int expected_output[]      = {13, 55, 12345};
    static constexpr int expected_output_long[] = {13, 55, 12345, -1};
    static constexpr int expected_input[]       = {-1, -1, -1};
    static constexpr int expected_input_long[]  = {-1, -1, -1, 42};

    template <ranges::input_range R, ranges::forward_range W>
    static constexpr void call() {
        using ranges::uninitialized_move_n, ranges::common_range, ranges::equal, ranges::range_value_t,
            ranges::range_reference_t;

        { // Validate matching range
            int_wrapper input[3] = {13, 55, 12345};
            R wrapped_input{input};
            holder<int_wrapper, 3> mem;
            W wrapped_output{mem.as_span()};

            const auto result =
                uninitialized_move_n(wrapped_input.begin(), 3, wrapped_output.begin(), wrapped_output.end());
            assert(result.in == wrapped_input.end());
            assert(result.out == wrapped_output.end());
            assert(equal(wrapped_output, expected_output));
            assert(equal(input, expected_input));
        }

        if constexpr (common_range<R> && is_same_v<range_value_t<R>, remove_reference_t<range_reference_t<R>>>) {
            // Validate matching range for std
            int_wrapper input[3] = {13, 55, 12345};
            R wrapped_input{input};
            holder<int_wrapper, 3> mem;
            W wrapped_output{mem.as_span()};

            const auto result = std::uninitialized_move_n(wrapped_input.begin(), 3, wrapped_output.begin());
            assert(result == wrapped_output.end());
            assert(equal(wrapped_output, expected_output));
            assert(equal(input, expected_input));
        }

        { // Validate shorter output
            int_wrapper input[4] = {13, 55, 12345, 42};
            R wrapped_input{input};
            holder<int_wrapper, 3> mem;
            W wrapped_output{mem.as_span()};

            auto result = uninitialized_move_n(wrapped_input.begin(), 3, wrapped_output.begin(), wrapped_output.end());
            assert(++result.in == wrapped_input.end());
            assert(result.out == wrapped_output.end());
            assert(equal(wrapped_output, expected_output));
            assert(equal(input, expected_input_long));
        }

        { // Validate shorter input
            int_wrapper input[3] = {13, 55, 12345};
            R wrapped_input{input};
            holder<int_wrapper, 4> mem;
            W wrapped_output{mem.as_span()};

            auto result = uninitialized_move_n(wrapped_input.begin(), 3, wrapped_output.begin(), wrapped_output.end());
            assert(result.in == wrapped_input.end());
            construct_at(addressof(*result.out), -1); // Need to construct non written element for comparison
            assert(++result.out == wrapped_output.end());
            assert(equal(wrapped_output, expected_output_long));
            assert(equal(input, expected_input));
        }

        if constexpr (common_range<R> && is_same_v<range_value_t<R>, remove_reference_t<range_reference_t<R>>>) {
            // Validate shorter input for std
            int_wrapper input[3] = {13, 55, 12345};
            R wrapped_input{input};
            holder<int_wrapper, 4> mem;
            W wrapped_output{mem.as_span()};

            const auto result = std::uninitialized_move_n(wrapped_input.begin(), 3, wrapped_output.begin());
            construct_at(addressof(*result), -1); // Need to construct non written element for comparison
            assert(++result == wrapped_output.end());
            assert(equal(wrapped_output, expected_output));
            assert(equal(input, expected_input));
        }
    }
};

struct memcpy_test {
    static constexpr int expected_output[]      = {13, 55, 12345};
    static constexpr int expected_output_long[] = {13, 55, 12345, -1};
    static constexpr int expected_input[]       = {13, 55, 12345};
    static constexpr int expected_input_long[]  = {13, 55, 12345, 42};

    static constexpr void call() {
        { // Validate matching range
            int input[] = {13, 55, 12345};
            int output[3];

            const auto result = ranges::uninitialized_move_n(begin(input), 3, begin(output), end(output));
            assert(result.in == end(input));
            assert(result.out == end(output));
            assert(ranges::equal(input, expected_input));
            assert(ranges::equal(output, expected_output));
        }

        { // Validate matching range std algorithm
            int input[] = {13, 55, 12345};
            int output[3];

            const auto result = std::uninitialized_move_n(begin(input), 3, begin(output));
            assert(result.first == end(input));
            assert(result.second == end(output));
            assert(ranges::equal(input, expected_input));
            assert(ranges::equal(output, expected_output));
        }

        { // Validate output shorter
            int input[] = {13, 55, 12345, 42};
            int output[3];

            auto result = ranges::uninitialized_move_n(begin(input), 4, begin(output), end(output));
            assert(++result.in == end(input));
            assert(result.out == end(output));
            assert(ranges::equal(input, expected_input_long));
            assert(ranges::equal(output, expected_output));
        }

        { // Validate input shorter
            int input[] = {13, 55, 12345};
            int output[4];

            auto result = ranges::uninitialized_move_n(begin(input), 3, begin(output), end(output));
            construct_at(addressof(*result.out), -1); // Need to construct non written element for comparison
            assert(result.in == end(input));
            assert(++result.out == end(output));
            assert(ranges::equal(input, expected_input));
            assert(ranges::equal(output, expected_output_long));
        }

        { // Validate input shorter std algorithm
            int input[] = {13, 55, 12345};
            int output[4];

            auto result = std::uninitialized_move_n(begin(input), 3, begin(output));
            construct_at(addressof(*result.second), -1); // Need to construct non written element for comparison
            assert(result.first == end(input));
            assert(++result.second == end(output));
            assert(ranges::equal(input, expected_input));
            assert(ranges::equal(output, expected_output_long));
        }
    }
};

template <test::ProxyRef IsProxy>
using test_input  = test::range<test::input, int_wrapper, test::Sized::no, test::CanDifference::no, test::Common::no,
    test::CanCompare::yes, IsProxy>;
using test_output = test::range<test::fwd, int_wrapper, test::Sized::no, test::CanDifference::no, test::Common::no,
    test::CanCompare::yes, test::ProxyRef::no>;

int main() {
    // The algorithm is oblivious to non-required category, size, difference, and "proxyness" of the input range. It
    // _is_ sensitive to proxyness in that it requires non-proxy references for the output range.

    instantiator::call<test_input<test::ProxyRef::no>, test_output>();
    instantiator::call<test_input<test::ProxyRef::yes>, test_output>();
    memcpy_test::call();

    static_assert((instantiator::call<test_input<test::ProxyRef::no>, test_output>(), true));
    static_assert((instantiator::call<test_input<test::ProxyRef::yes>, test_output>(), true));
    static_assert((memcpy_test::call(), true));
}
#else // ^^^ constexpr algorithms ^^^ / vvv No constexpr algorthms vvv
int main() {}
#endif // No constexpr algrithms
