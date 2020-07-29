#pragma once

#include <random>
#include <cstdint>


class Dice
{
    public:
    Dice(uint32_t lowerBound, uint32_t upperBound);
    Dice(const Dice& other);

    uint32_t rollUnsignedInt();
    int32_t rollSignedInt();


    private:
    std::random_device randomDevice;
    std::default_random_engine randomEngine;
    std::uniform_int_distribution<uint32_t> unsignedUniformIntDistribution;
    std::uniform_int_distribution<int32_t> signedUniformIntDistribution;
};
