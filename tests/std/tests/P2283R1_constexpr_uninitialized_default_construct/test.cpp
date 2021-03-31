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

struct int_wrapper {
    int val = 10;

    constexpr int_wrapper() noexcept = default;
    constexpr int_wrapper(const int x) : val{x} {}

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
    static constexpr int expected_output[] = {0, 0, 0};

    template <ranges::forward_range W>
    static constexpr void call() {
        using ranges::uninitialized_default_construct, ranges::common_range, ranges::equal;

        { // Validate range overload
            holder<int_wrapper, 3> mem;
            W wrapped_output{mem.as_span()};

            const auto result = uninitialized_default_construct(wrapped_output);
            assert(result == wrapped_output.end());
        }

        { // Validate iterator overload
            holder<int_wrapper, 3> mem;
            W wrapped_output{mem.as_span()};

            const auto result = uninitialized_default_construct(wrapped_output.begin(), wrapped_output.end());
            assert(result == wrapped_output.end());
        }

        if constexpr (common_range<W>) {
            // Validate iterator overload for std
            holder<int_wrapper, 3> mem;
            W wrapped_output{mem.as_span()};

            const auto result = std::uninitialized_default_construct(wrapped_output.begin(), wrapped_output.end());
            assert(result == wrapped_output.end());
        }
    }
};

using test_range = test::range<test::fwd, int_wrapper, test::Sized::no, test::CanDifference::no, test::Common::no,
    test::CanCompare::yes, test::ProxyRef::no>;

int main() {
    // The algorithm is oblivious to non-required category, size, difference. It _is_ sensitive to proxyness in that it
    // requires non-proxy references for the input range.

    instantiator::call<test_range>();
#if 0 // TRANSITION, compiler support
    static_assert((instantiator::call<test_range>(), true));
#endif // compiler support
}
