#include <libbladeRF.h>
#include "trigger.h"
#include "log.h"



int ss_read_user_register(struct bladerf *dev, uint8_t reg, uint32_t *val)
{
    return dev->fn->read_user_register(dev, reg , val);
}

int ss_write_user_register(struct bladerf *dev, uint8_t reg, uint32_t val)
{
    return dev->fn->write_user_register(dev, reg , val);
}