struct Random
{
private:
    unsigned long long seed;

public:
    Random(unsigned long long seed = 0) : seed(seed) {}

    unsigned long long rand()
    {
        seed ^= seed << 13;
        seed ^= seed >> 7;
        seed ^= seed << 17;
        return seed;
    }

    unsigned long long rand(unsigned long long l, unsigned long long r)
    {
        return l + rand() % (r-l+1);
    }
};
