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
    int status;
    bool ok,fpga_loaded = false;
    //int (*f)(struct bladerf *, uint8_t, uint8_t) = NULL;
    unsigned int value;

    /* Exit cleanly if no device is attached */
    if (state->dev == NULL) {
        printf("cmd_ss_trsh:  No device attached.\n");
        return 0;
    }

    status = bladerf_is_fpga_configured(state->dev);
    if (status < 0) {
        state->last_lib_error = status;
        return CLI_RET_LIBBLADERF;
    } else if (status != 0) {
        fpga_loaded = true;
        printf("FPGA loaded: %i\n", fpga_loaded);
        status = 0;
        if (status < 0) {
            state->last_lib_error = status;
            return CLI_RET_LIBBLADERF;
        }
    } 

    /* 
    Write value to register
    */
    if( argc == 2 ) {
        printf("cmd_ss_trsh:  One argument: %s.\n",argv[1]);        

        /* Parse the value */
        value = str2uint( argv[1], 0, SS_MAX_TRSH, &ok );
        printf("Arg: %u\n", value);
        if( !ok ) {
            cli_err(state, argv[0],
                    "Invalid value provided (%s)", argv[1]);
            return CLI_RET_INVPARAM;
        }

        status = bladerf_write_register(state->dev, SS_REG_00, (uint32_t) value);
        if (status < 0) {
            state->last_lib_error = status;
            rv = CLI_RET_LIBBLADERF;
        }

        /* Write the value to the address */
        /*
        if( rv == CLI_RET_OK && f ) {
            status = f( state->dev, (uint8_t)SS_REG_00, (uint8_t)value );
            if (status < 0) {
                state->last_lib_error = status;
                rv = CLI_RET_LIBBLADERF;
            } else {
                printf( "\n  0x%2.2x: 0x%2.2x\n", address, value );
                if (f == bladerf_lms_write) {
                    uint8_t readback;
                    int status = bladerf_lms_read(state->dev,
                                                  (uint8_t)address,
                                                  &readback);
                    if (status == 0) {
                        lms_reg_info(address, readback);
                        putchar('\n'); // To be consistent with peek output 
                    }
                }
            }
        }
        */
        return rv;

    }
    /*
    Read from register
    */
    if (argc == 1) {
        uint32_t data=0;
        status = bladerf_read_register(state->dev, SS_REG_00, &data);
        if (status==0){
            printf("cmd_ss_trsh:  %02x\n",(unsigned int)data);  
        }
        return rv;      
    }
    else {
        cli_err(state, argv[0], "Invalid number of arguments (%d)\n", argc);
        rv = CLI_RET_INVPARAM;
    }
    return rv;
}

