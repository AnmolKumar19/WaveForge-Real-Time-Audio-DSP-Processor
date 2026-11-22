#pragma once
#include <cstddef>
#include <memory>
#include <new>
#include <stdexcept>
#include <vector>
#include "waveforge_ring.h"

namespace waveforge {

// C++ owner of a C11 waveforge_ring plus its float storage. Single producer thread
// calls write(), single consumer thread calls read().
class SpscRing {
public:
    explicit SpscRing(size_t capacityPow2)
        : storage_(capacityPow2),
          raw_(::operator new(waveforge_ring_sizeof(), std::align_val_t(waveforge_ring_alignof()))),
          align_(waveforge_ring_alignof()) {
        ring_ = static_cast<waveforge_ring*>(raw_);
        if (waveforge_ring_init(ring_, storage_.data(), capacityPow2) != WAVEFORGE_OK) {
            release();
            throw std::runtime_error("SpscRing: capacity must be a power of two >= 2");
        }
    }
    ~SpscRing() { release(); }
    SpscRing(const SpscRing&) = delete;
    SpscRing& operator=(const SpscRing&) = delete;

    bool write(const float* src, size_t n) { return waveforge_ring_write(ring_, src, n) == WAVEFORGE_OK; }
    bool read(float* dst, size_t n) { return waveforge_ring_read(ring_, dst, n) == WAVEFORGE_OK; }
    size_t available() const { return waveforge_ring_available(ring_); }

private:
    void release() { if (raw_) { ::operator delete(raw_, std::align_val_t(align_)); raw_ = nullptr; } }
    std::vector<float> storage_;
    void* raw_;
    size_t align_;
    waveforge_ring* ring_ = nullptr;
};

} // namespace waveforge
