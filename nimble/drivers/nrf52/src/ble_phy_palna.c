#include <stdbool.h>
#include "hal/hal_gpio.h"
#include "os/os_cputime.h"
#include "controller/ble_phy_palna.h"

static void pa_enable(bool enable);
static void lna_enable(bool enable);

void 
ble_phy_palna_init(void){
    // setup gpios
    hal_gpio_init_out(BLE_PHY_PALNA_PA_ENABLE_PIN,!BLE_PHY_PALNA_PA_ENABLE_ACTIVE_STATE);
    hal_gpio_init_out(BLE_PHY_PALNA_LNA_ENABLE_PIN,!BLE_PHY_PALNA_LNA_ENABLE_ACTIVE_STATE);

    // todo: use gpiote after we see this work...
}

void 
ble_phy_palna_tx_prepare(void)
{
    lna_enable(false);
    pa_enable(true);
    os_cputime_delay_usecs(BLE_PHY_PALNA_PA_ENABLE_DELAY_US);
}

void
ble_phy_palna_tx_complete(void)
{
    pa_enable(false);
}

void 
ble_phy_palna_rx_prepare(void)
{
    pa_enable(false);
    lna_enable(true);
    os_cputime_delay_usecs(BLE_PHY_PALNA_LNA_ENABLE_DELAY_US);
}

void 
ble_phy_palna_rx_complete(void){
    lna_enable(false);
}

static void 
pa_enable(bool enable)
{
    int val = BLE_PHY_PALNA_PA_ENABLE_ACTIVE_STATE;
    if (!enable)
    {
        val = !val;
    }

    hal_gpio_write(BLE_PHY_PALNA_PA_ENABLE_PIN, val);
};

static void 
lna_enable(bool enable)
{
    int val = BLE_PHY_PALNA_LNA_ENABLE_ACTIVE_STATE;
    if (!enable)
    {
        val = !val;
    }

    hal_gpio_write(BLE_PHY_PALNA_LNA_ENABLE_PIN, val);
}
