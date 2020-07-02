/*
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements.  See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership.  The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License.  You may obtain a copy of the License at
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 * KIND, either express or implied.  See the License for the
 * specific language governing permissions and limitations
 * under the License.
 */

#include <assert.h>
#include "os/mynewt.h"
#include "hal/hal_gpio.h"
#include "controller/ble_ll.h"

static void bmd345_init(void) {
    //set the mac
    #define BMD345_UICR_MAC_ADDR 0x10001080
    ble_ll_set_public_addr((void*)BMD345_UICR_MAC_ADDR);

    // enable the pa/lna
    // p1.02
    #define BLE_PHY_PALNA_ANTENNA_SELECT_PIN        34
    #define BLE_PHY_PALNA_ANTENNA_SELECT_INTERNAL   1
    #define BLE_PHY_PALNA_ANTENNA_SELECT_EXTERNAL   0

    // p1.04
    #define BLE_PHY_PALNA_MODE_PIN                  36
    #define BLE_PHY_PALNA_MODE_ENABLE               0

    // enable palna
    hal_gpio_init_out(BLE_PHY_PALNA_MODE_ENABLE, BLE_PHY_PALNA_MODE_ENABLE);

    // internal antenna
    hal_gpio_init_out(BLE_PHY_PALNA_ANTENNA_SELECT_PIN, BLE_PHY_PALNA_ANTENNA_SELECT_INTERNAL);
}

int
main(void)
{
    bmd345_init();

    /* Initialize OS */
    sysinit();

    while (1) {
        os_eventq_run(os_eventq_dflt_get());
    }
    return 0;
}


