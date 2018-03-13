
#ifndef BLADERF_SS_REGISTER_H_
#define BLADERF_SS_REGISTER_H_

#include "bladerf_priv.h"

/**
 * Read trigger control register
 *
 * @param   dev             Device handle
 * @param   signal          Trigger signal control register to read from
 * @param   val             Pointer to variable that register is read into
 *                          See the BLADERF_TRIGGER_REG_* macros for the meaning
 *                          of each bit.
 *
 * @return 0 on success, BLADERF_ERR_* value on failure
 */
int ss_read_user_register(struct bladerf *dev, uint8_t reg, uint32_t *val);
int ss_write_user_register(struct bladerf *dev, uint8_t reg, uint32_t val);

#endif