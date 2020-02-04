#include "MathUtils.hpp"

#include <cstdlib>

#include "Vector.hpp"

using namespace Pht;

float Pht::NormalizedRand() {
    return static_cast<float>(std::rand()) / static_cast<float>(RAND_MAX);
}

int Pht::Sign(float value) {
    return (0.0f < value) - (value < 0.0f);
}

float Pht::ToDegrees(float rad) {
    return rad * 180.0f / Pi;
}

float Pht::ToRadians(float degrees) {
    return degrees * Pi / 180.0f;
}
