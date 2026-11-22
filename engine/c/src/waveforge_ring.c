#include "waveforge_ring.h"

size_t waveforge_ring_sizeof(void) { return sizeof(waveforge_ring); }
size_t waveforge_ring_alignof(void) { return _Alignof(waveforge_ring); }

static int is_pow2(size_t v) { return v >= 2U && (v & (v - 1U)) == 0U; }

waveforge_status waveforge_ring_init(waveforge_ring *r, float *storage, size_t capacity) {
    if (r == NULL || storage == NULL || !is_pow2(capacity)) return WAVEFORGE_ERR_ARG;
    r->data = storage;
    r->capacity = capacity;
    r->mask = capacity - 1U;
    atomic_init(&r->head, 0U);
    atomic_init(&r->tail, 0U);
    return WAVEFORGE_OK;
}

size_t waveforge_ring_available(const waveforge_ring *r) {
    if (r == NULL) return 0U;
    size_t h = atomic_load_explicit(&r->head, memory_order_acquire);
    size_t t = atomic_load_explicit(&r->tail, memory_order_acquire);
    return h - t; /* unsigned wrap-around is well defined */
}

size_t waveforge_ring_space(const waveforge_ring *r) {
    if (r == NULL) return 0U;
    return (r->capacity - 1U) - waveforge_ring_available(r);
}

waveforge_status waveforge_ring_write(waveforge_ring *r, const float *src, size_t n) {
    if (r == NULL || (src == NULL && n > 0U)) return WAVEFORGE_ERR_ARG;
    size_t h = atomic_load_explicit(&r->head, memory_order_relaxed);
    size_t t = atomic_load_explicit(&r->tail, memory_order_acquire);
    if (n > (r->capacity - 1U) - (h - t)) return WAVEFORGE_ERR_FULL;
    for (size_t i = 0; i < n; ++i) r->data[(h + i) & r->mask] = src[i];
    atomic_store_explicit(&r->head, h + n, memory_order_release);
    return WAVEFORGE_OK;
}

waveforge_status waveforge_ring_read(waveforge_ring *r, float *dst, size_t n) {
    if (r == NULL || (dst == NULL && n > 0U)) return WAVEFORGE_ERR_ARG;
    size_t t = atomic_load_explicit(&r->tail, memory_order_relaxed);
    size_t h = atomic_load_explicit(&r->head, memory_order_acquire);
    if (n > h - t) return WAVEFORGE_ERR_EMPTY;
    for (size_t i = 0; i < n; ++i) dst[i] = r->data[(t + i) & r->mask];
    atomic_store_explicit(&r->tail, t + n, memory_order_release);
    return WAVEFORGE_OK;
}
