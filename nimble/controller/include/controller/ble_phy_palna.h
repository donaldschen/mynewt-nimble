#ifndef __BLE_PHY_PALNA_H
#define __BLE_PHY_PALNA_H

#define BLE_PHY_PALNA_PA_ENABLE_PIN               37 
#define BLE_PHY_PALNA_PA_ENABLE_ACTIVE_STATE      1
#define BLE_PHY_PALNA_PA_ENABLE_DELAY_US          1

#define BLE_PHY_PALNA_LNA_ENABLE_PIN              38
#define BLE_PHY_PALNA_LNA_ENABLE_ACTIVE_STATE     1
#define BLE_PHY_PALNA_LNA_ENABLE_DELAY_US         1

void ble_phy_palna_init(void);
void ble_phy_palna_tx_prepare(void);
void ble_phy_palna_tx_complete(void);
void ble_phy_palna_rx_prepare(void);
void ble_phy_palna_rx_complete(void);

#endif