#include <stdbool.h>
#include "hal/hal_gpio.h"
#include "nrfx/nrfx.h"
#include "nrfx/hal/nrf_gpiote.h"
#include "controller/ble_phy_palna.h"

#ifdef BLE_PHY_PALNA_ENABLED

typedef enum
{
    PA_SHUTDOWN,
    PA_TX,
    PA_RX,
    PA_BYPASS
} pa_modes_t;

#define PALNA_PPI_CHANNEL_MASK ((1 << BLE_PHY_PALNA_PPI_CHANNEL_RADIO_READY) | (1 << BLE_PHY_PALNA_PPI_CHANNEL_RADIO_DISABLED))

static void set_mode(pa_modes_t mode);

void ble_phy_palna_init(void)
{
    // disable pa, lna
    hal_gpio_init_out(BLE_PHY_PALNA_PA_ENABLE_PIN, !BLE_PHY_PALNA_PA_ENABLE_ACTIVE);
    hal_gpio_init_out(BLE_PHY_PALNA_LNA_ENABLE_PIN, !BLE_PHY_PALNA_LNA_ENABLE_ACTIVE);

    // enable palna
    hal_gpio_init_out(BLE_PHY_PALNA_MODE_ENABLE, BLE_PHY_PALNA_MODE_ENABLE);

    // internal antenna
    hal_gpio_init_out(BLE_PHY_PALNA_ANTENNA_SELECT_PIN, BLE_PHY_PALNA_ANTENNA_SELECT_INTERNAL);

    //Setup a PPI Channel for Radio Ready Event to enable PA-LNA
    NRF_PPI->CH[BLE_PHY_PALNA_PPI_CHANNEL_RADIO_READY].EEP = (uint32_t)&NRF_RADIO->EVENTS_READY;
    NRF_PPI->CH[BLE_PHY_PALNA_PPI_CHANNEL_RADIO_READY].TEP = (uint32_t)&NRF_GPIOTE->TASKS_SET[0];

    //Setup PPI channel for Radio Disabled Event to disable PA-LNA
    NRF_PPI->CH[BLE_PHY_PALNA_PPI_CHANNEL_RADIO_DISABLED].EEP = (uint32_t)&NRF_RADIO->EVENTS_DISABLED;
    NRF_PPI->CH[BLE_PHY_PALNA_PPI_CHANNEL_RADIO_DISABLED].TEP = (uint32_t)&NRF_GPIOTE->TASKS_CLR[0];

    //init gpiote
    nrf_gpiote_te_default(BLE_PHY_PALNA_GPIOTE_CHANNEL);

    // off
    ble_phy_palna_idle();

    //Note: FCC/IC max power setting: -12dBm || CE (RED) max power setting: -20dBm
}

void ble_phy_palna_tx_prepare(void)
{
    set_mode(PA_TX);
}

void ble_phy_palna_rx_prepare(void)
{
    set_mode(PA_RX);
}

void ble_phy_palna_idle(void)
{
    set_mode(PA_SHUTDOWN);
}

static void
set_mode(pa_modes_t mode)
{
    //todo: calculate the GPIOTE config by pin number/polarity

    NRF_PPI->CHENCLR = PALNA_PPI_CHANNEL_MASK;               //disable PPI channels
    NRF_GPIOTE->TASKS_CLR[BLE_PHY_PALNA_GPIOTE_CHANNEL] = 1; //ensure current GPIOTE pin is low

    if (mode == PA_SHUTDOWN)
    {
        nrf_gpiote_te_default(BLE_PHY_PALNA_GPIOTE_CHANNEL);
        //NRF_GPIOTE->CONFIG[BLE_PHY_PALNA_GPIOTE_CHANNEL] = 0; //reset GPIOTE config
    }
    else if (mode == PA_TX)
    {
        nrf_gpiote_task_configure(
            BLE_PHY_PALNA_GPIOTE_CHANNEL,
            BLE_PHY_PALNA_PA_ENABLE_PIN,
            NRF_GPIOTE_POLARITY_TOGGLE,
            (BLE_PHY_PALNA_PA_ENABLE_ACTIVE ? NRF_GPIOTE_INITIAL_VALUE_LOW:NRF_GPIOTE_INITIAL_VALUE_HIGH)
            );

        nrf_gpiote_task_enable(BLE_PHY_PALNA_GPIOTE_CHANNEL);

        //NRF_GPIOTE->CONFIG[BLE_PHY_PALNA_GPIOTE_CHANNEL] = 0x32503; //TX - pin p1.05, task, toggle, init low
        NRF_PPI->CHENSET = PALNA_PPI_CHANNEL_MASK;                  //re-enable PPI channels 1 and 2
    }
    else if (mode == PA_RX)
    {
        nrf_gpiote_task_configure(
            BLE_PHY_PALNA_GPIOTE_CHANNEL,
            BLE_PHY_PALNA_LNA_ENABLE_PIN,
            NRF_GPIOTE_POLARITY_TOGGLE,
            (BLE_PHY_PALNA_LNA_ENABLE_ACTIVE ? NRF_GPIOTE_INITIAL_VALUE_LOW:NRF_GPIOTE_INITIAL_VALUE_HIGH)
            );
        nrf_gpiote_task_enable(BLE_PHY_PALNA_GPIOTE_CHANNEL);

        //NRF_GPIOTE->CONFIG[BLE_PHY_PALNA_GPIOTE_CHANNEL] = 0x32603; //RX - pin p1.06, task, toggle, init low
        NRF_PPI->CHENSET = PALNA_PPI_CHANNEL_MASK;                  //re-enable PPI channels 1 and 2
    }
}

#endif