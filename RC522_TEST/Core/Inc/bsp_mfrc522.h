/**
  ******************************************************************************

  BSP MFRC522 For STM32F446RE
  Author:   LVQ
  Updated:  12 August 2022

  ******************************************************************************
*/

#ifndef BSP_MFRC522_H
#define BSP_MFRC522_H

#include "main.h"

extern SPI_HandleTypeDef hspi1;

#define BSP_MFRC522_SPI                   &hspi1
#define BSP_MFRC522_TIMEOUT               500

#define BSP_MFRC522_CS_PORT               GPIOA
#define BSP_MFRC522_CS_PIN                GPIO_PIN_0

/* MFRC522 Commands */
#define BSP_PCD_IDLE                      (0x00)  // NO action; Cancel the current command
#define BSP_PCD_AUTHENT                   (0x0E)  // Authentication Key
#define BSP_PCD_RECEIVE                   (0x08)  // Receive Data
#define BSP_PCD_TRANSMIT                  (0x04)  // Transmit data
#define BSP_PCD_TRANSCEIVE                (0x0C)  // Transmit and receive data,
#define BSP_PCD_RESETPHASE                (0x0F)  // Reset
#define BSP_PCD_CALCCRC                   (0x03)  // CRC Calculate

/* Mifare_One card command word */
#define BSP_PICC_REQIDL                   (0x26)  // find the antenna area does not enter hibernation
#define BSP_PICC_REQALL                   (0x52)  // find all the cards antenna area
#define BSP_PICC_ANTICOLL                 (0x93)  // anti-collision
#define BSP_PICC_SElECTTAG                (0x93)  // election card
#define BSP_PICC_AUTHENT1A                (0x60)  // authentication key A
#define BSP_PICC_AUTHENT1B                (0x61)  // authentication key B
#define BSP_PICC_READ                     (0x30)  // Read Block
#define BSP_PICC_WRITE                    (0xA0)  // write block
#define BSP_PICC_DECREMENT                (0xC0)  // debit  
#define BSP_PICC_INCREMENT                (0xC1)  // recharge
#define BSP_PICC_RESTORE                  (0xC2)  // transfer block data to the buffer
#define BSP_PICC_TRANSFER                 (0xB0)  // save the data in the buffer
#define BSP_PICC_HALT                     (0x50)  // Sleep

/* MFRC522 Registers */
// Page 0: Command and Status
#define BSP_MFRC522_REG_RESERVED00        (0x00)
#define BSP_MFRC522_REG_COMMAND           (0x01)
#define BSP_MFRC522_REG_COMM_I_EN         (0x02)
#define BSP_MFRC522_REG_DIV1_EN           (0x03)
#define BSP_MFRC522_REG_COMM_IRQ          (0x04)
#define BSP_MFRC522_REG_DIV_IRQ           (0x05)
#define BSP_MFRC522_REG_ERROR             (0x06)
#define BSP_MFRC522_REG_STATUS1           (0x07)
#define BSP_MFRC522_REG_STATUS2           (0x08)
#define BSP_MFRC522_REG_FIFO_DATA         (0x09)
#define BSP_MFRC522_REG_FIFO_LEVEL        (0x0A)
#define BSP_MFRC522_REG_WATER_LEVEL       (0x0B)
#define BSP_MFRC522_REG_CONTROL           (0x0C)
#define BSP_MFRC522_REG_BIT_FRAMING       (0x0D)
#define BSP_MFRC522_REG_COLL              (0x0E)
#define BSP_MFRC522_REG_RESERVED01        (0x0F)
// Page 1: Command
#define BSP_MFRC522_REG_RESERVED10        (0x10)
#define BSP_MFRC522_REG_MODE              (0x11)
#define BSP_MFRC522_REG_TX_MODE           (0x12)
#define BSP_MFRC522_REG_RX_MODE           (0x13)
#define BSP_MFRC522_REG_TX_CONTROL        (0x14)
#define BSP_MFRC522_REG_TX_AUTO           (0x15)
#define BSP_MFRC522_REG_TX_SELL           (0x16)
#define BSP_MFRC522_REG_RX_SELL           (0x17)
#define BSP_MFRC522_REG_RX_THRESHOLD      (0x18)
#define BSP_MFRC522_REG_DEMOD             (0x19)
#define BSP_MFRC522_REG_RESERVED11        (0x1A)
#define BSP_MFRC522_REG_RESERVED12        (0x1B)
#define BSP_MFRC522_REG_MF_TX             (0x1C)
#define BSP_MFRC522_REG_MF_RX             (0x1D)
#define BSP_MFRC522_REG_RESERVED13        (0x1E)
#define BSP_MFRC522_REG_SERIALSPEED       (0x1F)
// Page 2: CFG
#define BSP_MFRC522_REG_RESERVED20        (0x20)
#define BSP_MFRC522_REG_CRC_RESULT_M      (0x21)
#define BSP_MFRC522_REG_CRC_RESULT_L      (0x22)
#define BSP_MFRC522_REG_RESERVED21        (0x23)
#define BSP_MFRC522_REG_MOD_WIDTH         (0x24)
#define BSP_MFRC522_REG_RESERVED22        (0x25)
#define BSP_MFRC522_REG_RF_CFG            (0x26)
#define BSP_MFRC522_REG_GS_N              (0x27)
#define BSP_MFRC522_REG_CWGS_PREG         (0x28)
#define BSP_MFRC522_REG_MODGS_PREG        (0x29)
#define BSP_MFRC522_REG_T_MODE            (0x2A)
#define BSP_MFRC522_REG_T_PRESCALER       (0x2B)
#define BSP_MFRC522_REG_T_RELOAD_H        (0x2C)
#define BSP_MFRC522_REG_T_RELOAD_L        (0x2D)
#define BSP_MFRC522_REG_T_COUNTER_VALUE_H (0x2E)
#define BSP_MFRC522_REG_T_COUNTER_VALUE_L (0x2F)
// Page 3:TestRegister
#define BSP_MFRC522_REG_RESERVED30        (0x30)
#define BSP_MFRC522_REG_TEST_SEL1         (0x31)
#define BSP_MFRC522_REG_TEST_SEL2         (0x32)
#define BSP_MFRC522_REG_TEST_PIN_EN       (0x33)
#define BSP_MFRC522_REG_TEST_PIN_VALUE    (0x34)
#define BSP_MFRC522_REG_TEST_BUS          (0x35)
#define BSP_MFRC522_REG_AUTO_TEST         (0x36)
#define BSP_MFRC522_REG_VERSION           (0x37)
#define BSP_MFRC522_REG_ANALOG_TEST       (0x38)
#define BSP_MFRC522_REG_TEST_ADC1         (0x39)
#define BSP_MFRC522_REG_TEST_ADC2         (0x3A)
#define BSP_MFRC522_REG_TEST_ADC0         (0x3B)
#define BSP_MFRC522_REG_RESERVED31        (0x3C)
#define BSP_MFRC522_REG_RESERVED32        (0x3D)
#define BSP_MFRC522_REG_RESERVED33        (0x3E)
#define BSP_MFRC522_REG_RESERVED34        (0x3F)
// Dummy byte
#define BSP_MFRC522_DUMMY                 (0x00)

#define BSP_MFRC522_MAX_LEN               (16)

typedef enum
{
  bsp_mfrc522_error = 0,
  bsp_mfrc522_ok,
}
bsp_mfrc522_status_t;

bsp_mfrc522_status_t bsp_mfrc522_init(void);

bsp_mfrc522_status_t bsp_mfrc522_check_card_type(uint8_t *card_type);

bsp_mfrc522_status_t bsp_mfrc522_read_id(uint8_t *card_type);

bsp_mfrc522_status_t bsp_mfrc522_check(uint8_t *id);

bsp_mfrc522_status_t bsp_mfrc522_anticoll(uint8_t *data);

bsp_mfrc522_status_t bsp_mfrc522_request(uint8_t request_mode, uint8_t *tag_type);

bsp_mfrc522_status_t bsp_mfrc522_haft(void);

bsp_mfrc522_status_t bsp_mfrc522_write_address(uint8_t block_address, uint8_t *send_data);

#endif
