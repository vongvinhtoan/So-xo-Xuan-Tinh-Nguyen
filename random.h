#include <random>

struct Random : public std::mt19937_64
{
public:
    Random(unsigned long long seed) : std::mt19937_64(seed) {}

    unsigned long long rand(unsigned long long l, unsigned long long r)
    {
        return std::uniform_int_distribution<>(l, r)(*this);
    }
};
