#ifndef WAVEFORGE_STATUS_H
#define WAVEFORGE_STATUS_H
#ifdef __cplusplus
extern "C" {
#endif
typedef enum {
    WAVEFORGE_OK = 0,
    WAVEFORGE_ERR_ARG = -1,   /* null pointer or invalid parameter */
    WAVEFORGE_ERR_FULL = -2,  /* not enough free space */
    WAVEFORGE_ERR_EMPTY = -3  /* not enough data available */
} waveforge_status;
#ifdef __cplusplus
}
#endif
#endif
