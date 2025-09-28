#ifndef BSP_RNG_H
#define BSP_RNG_H
#include <stdint.h>

extern uint32_t RNG_get_random_num(void);
extern int32_t RNG_get_random_rangle(int min, int max);
#endif
