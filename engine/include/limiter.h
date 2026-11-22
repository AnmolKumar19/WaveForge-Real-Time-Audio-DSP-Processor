#pragma once
#include "effect.h"
#include "waveforge_limiter.h"

namespace waveforge {

// Peak limiter: C++ Effect wrapper around the C11 waveforge_limiter.
class LimiterEffect : public Effect {
public:
    LimiterEffect(float threshold, double attackMs, double releaseMs, double sampleRateHz);
    void process(const float* in, float* out, int numSamples) override;
    void reset() override;
    const char* name() const override { return "peak_limiter"; }
private:
    waveforge_limiter lim_;
};

} // namespace waveforge
