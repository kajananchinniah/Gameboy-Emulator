#ifndef TESTS_TEST_BASE_HPP_
#define TESTS_TEST_BASE_HPP_

#include <random>

namespace GB
{
template <typename T> T generateRandomTestNumber(T min, T max)
{
    std::random_device rd;
    std::mt19937 mt(rd());
    std::uniform_int_distribution<T> dist(min, max);
    return dist(mt);
}

} // namespace GB

#endif // TESTS_TEST_BASE_HPP_
