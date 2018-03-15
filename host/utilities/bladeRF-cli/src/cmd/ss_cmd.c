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

int getreg(struct cli_state *state, int reg, uint32_t *val){
    int status,rv = CLI_RET_OK;

    status = bladerf_read_register(state->dev, reg, val);

    if (status<0){
        state->last_lib_error = status;
        rv = CLI_RET_LIBBLADERF;            
    }
    return rv;
}

int setreg(struct cli_state *state, int reg, uint32_t val){
    int status,rv = CLI_RET_OK;

    status = bladerf_write_register(state->dev, reg, val);

    if (status<0){
        state->last_lib_error = status;
        rv = CLI_RET_LIBBLADERF;            
    }
    return rv;
}


int cmd_ss(struct cli_state *state, int argc, char **argv)
{
    /* Valid commands:
        ss [<r0 | r1 | r2> [value]]
    */
    int rv = CLI_RET_OK;
    int status,reg;
    bool ok=true;
    //bool fpga_loaded = false;
    //int (*f)(struct bladerf *, uint8_t, uint8_t) = NULL;
    unsigned int value;

    /* Exit cleanly if no device is attached */
    if (state->dev == NULL) {
        printf("\n  cmd_ss:  No device attached.\n");
        return 0;
    }

    status = bladerf_is_fpga_configured(state->dev);
    if (status < 0) {
        state->last_lib_error = status;
        return CLI_RET_LIBBLADERF;
    } 
    /* 
    Write value to register
    */
    if( argc == 3 ) {
        uint32_t data = 0;
        /* Parse the value */

        if ( strcmp(argv[1],"r0") == 0){
            reg = SS_REG_00;
        }
        else if ( strcmp(argv[1],"r1") == 0){
            reg = SS_REG_01;
        }
        else if ( strcmp(argv[1],"r2") == 0){
            reg = SS_REG_02;
        }


        else if ( strcmp(argv[1],"trsh") == 0){
            value = str2uint( argv[2], SS_MIN_TRSH, SS_MAX_TRSH, &ok );
            if( !ok ) {
                cli_err(state, argv[0],
                    "Value out of range: %s [0x%08x 0x%08x]", argv[2],SS_MIN_TRSH,SS_MAX_TRSH);
                return CLI_RET_INVPARAM;
            }            
            rv = setreg(state,SS_REG_00,value);
            return rv;
        }
        else if ( strcmp(argv[1],"num") == 0){
            value = str2uint( argv[2], SS_MIN_NUMSAMPLES, SS_MAX_NUMSAMPLES, &ok );
            if( !ok ) {
                cli_err(state, argv[0],
                     "Value out of range: %s [0x%08x 0x%08x]", argv[2],SS_MIN_NUMSAMPLES,SS_MAX_NUMSAMPLES);
                return CLI_RET_INVPARAM;
            }
            // w ^= (-f ^ w) & m;

            rv = getreg(state,SS_REG_01,&data);
            data ^= value & SS_MASK_NUMSAMPLES;
            rv = setreg(state,SS_REG_01,data);
            return rv;
        }              
        else if ( strcmp(argv[1],"pre") == 0){
            value = str2uint( argv[2], SS_MIN_PRETRIG, SS_MAX_PRETRIG, &ok );
            if( !ok ) {
                cli_err(state, argv[0],
                        "Value out of range: %s [0x%08x 0x%08x]", argv[2],SS_MIN_PRETRIG,SS_MAX_PRETRIG);
                return CLI_RET_INVPARAM;
            }
            rv = getreg(state,SS_REG_01,&data);
            data ^=  (value<<19) & SS_MASK_PRETRIG;
            rv = setreg(state,SS_REG_01,data);                    
            return rv;
        }
        else if ( strcmp(argv[1],"master") == 0){
            value = str2uint( argv[2], 0, 1, &ok );
            if( !ok ) {
                cli_err(state, argv[0],
                        "Value out of range: %s [%1i %1i]", argv[2],0,1);
                return CLI_RET_INVPARAM;
            }
            rv = getreg(state,SS_REG_02,&data);
            data ^= (value<<31) & SS_MASTER_SLAVE;
            rv = setreg(state,SS_REG_02,data);                  
            return rv;
        }
        else{
            ok = false;
        }

        if( !ok ) {
            cli_err(state, argv[0],
                    "Invalid parameter provided %s", argv[1]);
            return CLI_RET_INVPARAM;
        }

        value = str2uint( argv[2], SS_REG_MIN_VAL, SS_REG_MAX_VAL, &ok );
        if( !ok ) {
            cli_err(state, argv[0],
                    "Value out of range: %s [0x%08x 0x%08x]", argv[2],SS_REG_MIN_VAL,SS_REG_MAX_VAL);
            return CLI_RET_INVPARAM;
        }

        status = bladerf_write_register(state->dev, reg, (uint32_t) value);
        if (status < 0) {
            state->last_lib_error = status;
            rv = CLI_RET_LIBBLADERF;
        }
        //printf("  Wrote 0x%08x to register 0x%02x \n", value, reg);


        return rv;

    }
    /*
    Read from register
    */
    if (argc == 2) {

        uint32_t data=0;

        if ( strcmp(argv[1],"r0") == 0){
            reg = SS_REG_00;
        }
        else if ( strcmp(argv[1],"r1") == 0){
            reg = SS_REG_01;
        }
        else if ( strcmp(argv[1],"r2") == 0){
            reg = SS_REG_02;
        }
        else if ( strcmp(argv[1],"trsh") == 0){
            rv = getreg(state,SS_REG_00,&data);
            if (!rv) { printf("\n  Current threshold: 0x%08x %u\n\n",data,data); }
            return rv;
        }
        else if ( strcmp(argv[1],"num") == 0){
            rv = getreg(state,SS_REG_01,&data);            
            if (!rv) {
                data &= UINT32_C(0x7ffff);
                printf("\n  Current number of samples: 0x%08x %u\n\n",data,data);
            }
            return rv;
        }              
        else if ( strcmp(argv[1],"pre") == 0){
            rv = getreg(state,SS_REG_01,&data);            
            if (!rv) {                
                data = data >> 19;
                printf("\n  Current pretrig: 0x%08x %u\n\n",data,data);
            }
            return rv;
        }
        else if ( strcmp(argv[1],"master") == 0){
            rv = getreg(state,SS_REG_02,&data);            
            if (!rv) {                
                if(data & SS_MASTER_SLAVE){
                    printf("\n  Master mode\n\n");
                }
                else{
                    printf("\n  Slave mode\n\n");
                }
            }
            return rv;
        }         
        else{
            ok = false;
        }        

        if( !ok ) {
            cli_err(state, argv[0],
                    "Invalid register provided: %s", argv[1]);
            return CLI_RET_INVPARAM;
        }

        status = bladerf_read_register(state->dev, reg, &data);

        if (status<0){
            state->last_lib_error = status;
            rv = CLI_RET_LIBBLADERF;            
        }
        printf("\n  0x%02x:  0x%08x\n\n",reg,(unsigned int)data);  
        return rv;      
    }
    /*
    Got no arguments, read all registers and print
    */
    if (argc==1){

        uint32_t data=0;

        for(int reg=SS_REG_00;reg<=SS_REG_02;reg++){

            status = bladerf_read_register(state->dev, reg, &data);

            if (status<0){
                state->last_lib_error = status;
                rv = CLI_RET_LIBBLADERF;            
            }
            printf("\n  0x%02x:  0x%08x\n",reg,(unsigned int)data);  
        }
        printf("\n");
        return rv;     

    }
    else {
        cli_err(state, argv[0], "Invalid number of arguments: %d\n", argc);
        rv = CLI_RET_INVPARAM;
    }
    return rv;
}

