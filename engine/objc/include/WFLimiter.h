// WFLimiter: Objective-C wrapper around the C11 waveforge_limiter peak limiter.
#import <Foundation/Foundation.h>

struct waveforge_limiter;

@interface WFLimiter : NSObject {
    struct waveforge_limiter *_lim;  // heap-allocated C11 limiter (kept opaque here)
}

/// Returns nil if any parameter is invalid (threshold must be in (0, 1], times and rate positive and finite).
- (instancetype)initWithThreshold:(float)threshold
                         attackMs:(float)attackMs
                        releaseMs:(float)releaseMs
                       sampleRate:(float)sampleRate;

@property(nonatomic, readonly) float threshold;
@property(nonatomic, readonly) float currentGain;

- (void)reset;

/// Processes an NSData of 32-bit floats and returns a new NSData of the same length. Output never exceeds the threshold.
- (NSData *)processSamples:(NSData *)samples;

@end
