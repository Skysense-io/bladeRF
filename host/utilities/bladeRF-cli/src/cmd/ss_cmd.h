#ifndef SS_CMD_H__
#define SS_CMD_H__

#include "common.h"

#define  SS_REG_00 (0x00)
#define  SS_REG_01 (0x01)
#define  SS_REG_02 (0x02)

#define  SS_REG_MIN_VAL (0x00)
#define  SS_REG_MAX_VAL (0xffffffff)

#define SS_MIN_TRSH (0x000000)
#define SS_MAX_TRSH (0xffffff)

#define SS_MIN_NUMSAMPLES  (0x00)
#define SS_MAX_NUMSAMPLES  (0x0007ffff)
#define SS_MASK_NUMSAMPLES (0x0007ffff)

#define SS_MIN_PRETRIG  (0x00)
#define SS_MAX_PRETRIG  (0x00001fff)
#define SS_MASK_PRETRIG (0xfff80000)


#define SS_SLOPE        (1<<0)
#define SS_ENABLE       (1<<1)
#define SS_MASTER_SLAVE (1<<2)
#define SS_DAP          (1<<3)


#endif /* SS_CMD_H__ */