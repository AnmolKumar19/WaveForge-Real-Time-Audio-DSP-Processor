#import "WFLimiter.h"
#include <stdlib.h>
#include "waveforge_limiter.h"

@implementation WFLimiter

- (instancetype)initWithThreshold:(float)threshold
                         attackMs:(float)attackMs
                        releaseMs:(float)releaseMs
                       sampleRate:(float)sampleRate {
    self = [super init];
    if (self == nil) return nil;
    _lim = (struct waveforge_limiter *)calloc(1, sizeof(struct waveforge_limiter));
    if (_lim == NULL || waveforge_limiter_init(_lim, threshold, attackMs, releaseMs, sampleRate) != WAVEFORGE_OK) {
        [self release];
        return nil;
    }
    return self;
}

- (void)dealloc {
    free(_lim);
    [super dealloc];
}

- (float)threshold { return _lim->threshold; }
- (float)currentGain { return _lim->gain; }
- (void)reset { waveforge_limiter_reset(_lim); }

- (NSData *)processSamples:(NSData *)samples {
    if (samples == nil || ([samples length] % sizeof(float)) != 0) return nil;
    NSUInteger n = [samples length] / sizeof(float);
    NSMutableData *out = [NSMutableData dataWithLength:[samples length]];
    if (n > 0 && waveforge_limiter_process(_lim, (const float *)[samples bytes], (float *)[out mutableBytes], n) != WAVEFORGE_OK)
        return nil;
    return out;
}

@end
