#ifndef ASTC_H
#define ASTC_H

#include <stdint.h>

void decode_astc(const uint8_t *data, const int w, const int h, const int bw, const int bh, uint32_t *image);

#endif /* end of include guard: ASTC_H */
