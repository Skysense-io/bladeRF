/*
 * This file is part of the bladeRF project
 *
 * Copyright (C) 2013-2014 Nuand LLC
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */
#include <stdio.h>
#include <string.h>
#include <libbladeRF.h>

#include "cmd.h"
#include "ss_cmd.h"

int cmd_ss_numsamples(struct cli_state *state, int argc, char **argv)
{
    /* Valid commands:
        ss_trsh [value]

        value 0xff00ff00
    */
    int rv = CLI_RET_OK;
    return rv;
}

int cmd_ss_numpretrig(struct cli_state *state, int argc, char **argv)
{
    /* Valid commands:
        ss_trsh [value]

        value 0xff00ff00
    */
    int rv = CLI_RET_OK;
    return rv;
}

int cmd_ss_trsh(struct cli_state *state, int argc, char **argv)
{
    /* Valid commands:
        ss_trsh [value]

        value 0xff00ff00
    */
    int rv = CLI_RET_OK;
    int status,reg;
    bool ok;
    //bool fpga_loaded = false;
    //int (*f)(struct bladerf *, uint8_t, uint8_t) = NULL;
    unsigned int value;

    /* Exit cleanly if no device is attached */
    if (state->dev == NULL) {
        printf("  cmd_ss_trsh:  No device attached.\n");
        return 0;
    }

    status = bladerf_is_fpga_configured(state->dev);
    if (status < 0) {
        state->last_lib_error = status;
        return CLI_RET_LIBBLADERF;
    } else if (status != 0) {
        //fpga_loaded = true;
        //printf("FPGA loaded: %i\n", fpga_loaded);
        status = 0;
    } 

    /* 
    Write value to register
    */
    if( argc == 3 ) {
        /* Parse the value */
        reg = str2uint( argv[1],SS_REG_00,SS_REG_02,&ok);

        if( !ok ) {
            cli_err(state, argv[0],
                    "Invalid register provided (%s)", argv[1]);
            return CLI_RET_INVPARAM;
        }
        value = str2uint( argv[2], 0, SS_MAX_VAL, &ok );
        if( !ok ) {
            cli_err(state, argv[0],
                    "Invalid value provided (%s)", argv[2]);
            return CLI_RET_INVPARAM;
        }

        status = bladerf_write_register(state->dev, reg, (uint32_t) value);
        if (status < 0) {
            state->last_lib_error = status;
            rv = CLI_RET_LIBBLADERF;
        }
        printf("  Wrote 0x%08x to register 0x%02x \n", value, reg);


        return rv;

    }
    /*
    Read from register
    */
    if (argc == 2) {
        uint32_t data=0;
        reg = str2uint( argv[1],SS_REG_00,SS_REG_02,&ok);

        if( !ok ) {
            cli_err(state, argv[0],
                    "Invalid register provided (%s)", argv[1]);
            return CLI_RET_INVPARAM;
        }

        status = bladerf_read_register(state->dev, reg, &data);

        if (status<0){
            state->last_lib_error = status;
            rv = CLI_RET_LIBBLADERF;            
        }
        printf("  0x%02x:  0x%08x\n",reg,(unsigned int)data);  
        return rv;      
    }
    else {
        cli_err(state, argv[0], "Invalid number of arguments (%d)\n", argc);
        rv = CLI_RET_INVPARAM;
    }
    return rv;
}

