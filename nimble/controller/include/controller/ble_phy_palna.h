#ifndef __BLE_PHY_PALNA_H
#define __BLE_PHY_PALNA_H

#define BLE_PHY_PALNA_ENABLED

#define BLE_PHY_PALNA_PPI_CHANNEL_RADIO_READY       0
#define BLE_PHY_PALNA_PPI_CHANNEL_RADIO_DISABLED    1
#define BLE_PHY_PALNA_GPIOTE_CHANNEL                0

// p1.02
#define BLE_PHY_PALNA_ANTENNA_SELECT_PIN        34
#define BLE_PHY_PALNA_ANTENNA_SELECT_INTERNAL   1
#define BLE_PHY_PALNA_ANTENNA_SELECT_EXTERNAL   0

// p1.04
#define BLE_PHY_PALNA_MODE_PIN                  36
#define BLE_PHY_PALNA_MODE_ENABLE               0

// p1.05
#define BLE_PHY_PALNA_PA_ENABLE_PIN             37 
#define BLE_PHY_PALNA_PA_ENABLE_ACTIVE          1

// p1.06
#define BLE_PHY_PALNA_LNA_ENABLE_PIN            38
#define BLE_PHY_PALNA_LNA_ENABLE_ACTIVE         1

void ble_phy_palna_init(void);
void ble_phy_palna_tx_prepare(void);
void ble_phy_palna_rx_prepare(void);
void ble_phy_palna_idle(void);

#endif