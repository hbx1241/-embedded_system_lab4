#ifndef __BLE_MAGNETO_SERVICE_H__
#define __BLE_MAGNETO_SERVICE_H__

#include <stdio.h>
#include "mbed.h"
#include "stm32l475e_iot01_magneto.h"
#include "ble/BLE.h"
#include <cstdint>

class MagnetoService {
public:
    const static uint16_t MAGNETO_SERVICE_UUID        = 0xA002;
    const static uint16_t MAGNETOX_CHARACTERISTIC_UUID = 0xA003;
    const static uint16_t MAGNETOY_CHARACTERISTIC_UUID = 0xA004;
    const static uint16_t MAGNETOZ_CHARACTERISTIC_UUID = 0xA005;

    MagnetoService(BLE &_ble) :
        ble(_ble), 
        magnetoXCharacteristic( MAGNETOX_CHARACTERISTIC_UUID, (uint8_t *) magnetoX, 
                                (uint16_t) sizeof(magnetoX), (uint16_t) sizeof(magnetoX), 
                                GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_NOTIFY),
        magnetoYCharacteristic( MAGNETOY_CHARACTERISTIC_UUID, (uint8_t *) magnetoY, 
                                (uint16_t) sizeof(magnetoY), (uint16_t) sizeof(magnetoY),
                                GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_NOTIFY),
        magnetoZCharacteristic( MAGNETOZ_CHARACTERISTIC_UUID, (uint8_t *) magnetoZ, 
                                (uint16_t) sizeof(magnetoZ), (uint16_t) sizeof(magnetoZ),
                                GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_NOTIFY)
    {
        static bool serviceAdded = false; /* We should only ever need to add the information service once. */
        if (serviceAdded) {
            return;
        }
        GattCharacteristic *charTable[] = { &magnetoXCharacteristic, 
                                            &magnetoYCharacteristic, 
                                            &magnetoZCharacteristic };
        GattService         magnetoService(MagnetoService::MAGNETO_SERVICE_UUID, charTable, sizeof(charTable) / sizeof(GattCharacteristic *));
        ble.gattServer().addService(magnetoService);

        serviceAdded = true;
    }

    void updateMagnetoX(int16_t newMagneto) {
        sprintf(magnetoX, "%d unit", newMagneto);
        ble.gattServer().write(magnetoXCharacteristic.getValueHandle(), (uint8_t *) magnetoX, sizeof(magnetoX));
    }

    void updateMagnetoY(int16_t newMagneto) {
        sprintf(magnetoY, "%d unit", newMagneto);
        ble.gattServer().write(magnetoYCharacteristic.getValueHandle(), (uint8_t *) magnetoY, sizeof(magnetoY));
    }

    void updateMagnetoZ(int16_t newMagneto) {
        sprintf(magnetoZ, "%d unit", newMagneto);
        ble.gattServer().write(magnetoZCharacteristic.getValueHandle(), (uint8_t *) magnetoZ, sizeof(magnetoZ));
    }

private:
    BLE &ble;
    char magnetoX[10];
    char magnetoY[10];
    char magnetoZ[10];
    GattCharacteristic    magnetoXCharacteristic;
    GattCharacteristic    magnetoYCharacteristic;
    GattCharacteristic    magnetoZCharacteristic;
};

#endif 