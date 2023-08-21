/**
 * @file       drv_mfrc522.h
 * @copyright  Copyright (C) 2023 QuyLe Co., Ltd. All rights reserved.
 * @license    This project is released under the QuyLe License.
 * @version    v1.0.0
 * @date       2023-08-19
 * @author     quyle-itr-intern
 *
 * @brief      driver mfrc522
 *
 * @note
 */

/* Define to prevent recursive inclusion ------------------------------ */
#ifndef __DRV_MFRC522_H
#define __DRV_MFRC522_H

/* Includes ----------------------------------------------------------- */
#include "bsp_gpio.h"
#include "bsp_spi.h"
#include "main.h"

#include <string.h>

/* Public defines ----------------------------------------------------- */

/* MFRC522 Commands */
#define DRV_PCD_IDLE                      (0x00)  // NO action; Cancel the current command
#define DRV_PCD_AUTHENT                   (0x0E)  // Authentication Key
#define DRV_PCD_RECEIVE                   (0x08)  // Receive Data
#define DRV_PCD_TRANSMIT                  (0x04)  // Transmit data
#define DRV_PCD_TRANSCEIVE                (0x0C)  // Transmit and receive data,
#define DRV_PCD_RESETPHASE                (0x0F)  // Reset
#define DRV_PCD_CALCCRC                   (0x03)  // CRC Calculate

/* Mifare_One card command word */
#define DRV_PICC_REQIDL                   (0x26)  // find the antenna area does not enter hibernation
#define DRV_PICC_REQALL                   (0x52)  // find all the cards antenna area
#define DRV_PICC_ANTICOLL                 (0x93)  // anti-collision
#define DRV_PICC_SElECTTAG                (0x93)  // election card
#define DRV_PICC_AUTHENT1A                (0x60)  // authentication key A
#define DRV_PICC_AUTHENT1B                (0x61)  // authentication key B
#define DRV_PICC_READ                     (0x30)  // Read Block
#define DRV_PICC_WRITE                    (0xA0)  // write block
#define DRV_PICC_DECREMENT                (0xC0)  // debit
#define DRV_PICC_INCREMENT                (0xC1)  // recharge
#define DRV_PICC_RESTORE                  (0xC2)  // transfer block data to the buffer
#define DRV_PICC_TRANSFER                 (0xB0)  // save the data in the buffer
#define DRV_PICC_HALT                     (0x50)  // Sleep

/* MFRC522 Registers */
// Page 0: Command and Status
#define DRV_MFRC522_REG_RESERVED00        (0x00)
#define DRV_MFRC522_REG_COMMAND           (0x01)
#define DRV_MFRC522_REG_COMM_I_EN         (0x02)
#define DRV_MFRC522_REG_DIV1_EN           (0x03)
#define DRV_MFRC522_REG_COMM_IRQ          (0x04)
#define DRV_MFRC522_REG_DIV_IRQ           (0x05)
#define DRV_MFRC522_REG_ERROR             (0x06)
#define DRV_MFRC522_REG_STATUS1           (0x07)
#define DRV_MFRC522_REG_STATUS2           (0x08)
#define DRV_MFRC522_REG_FIFO_DATA         (0x09)
#define DRV_MFRC522_REG_FIFO_LEVEL        (0x0A)
#define DRV_MFRC522_REG_WATER_LEVEL       (0x0B)
#define DRV_MFRC522_REG_CONTROL           (0x0C)
#define DRV_MFRC522_REG_BIT_FRAMING       (0x0D)
#define DRV_MFRC522_REG_COLL              (0x0E)
#define DRV_MFRC522_REG_RESERVED01        (0x0F)
// Page 1: Command
#define DRV_MFRC522_REG_RESERVED10        (0x10)
#define DRV_MFRC522_REG_MODE              (0x11)
#define DRV_MFRC522_REG_TX_MODE           (0x12)
#define DRV_MFRC522_REG_RX_MODE           (0x13)
#define DRV_MFRC522_REG_TX_CONTROL        (0x14)
#define DRV_MFRC522_REG_TX_AUTO           (0x15)
#define DRV_MFRC522_REG_TX_SELL           (0x16)
#define DRV_MFRC522_REG_RX_SELL           (0x17)
#define DRV_MFRC522_REG_RX_THRESHOLD      (0x18)
#define DRV_MFRC522_REG_DEMOD             (0x19)
#define DRV_MFRC522_REG_RESERVED11        (0x1A)
#define DRV_MFRC522_REG_RESERVED12        (0x1B)
#define DRV_MFRC522_REG_MF_TX             (0x1C)
#define DRV_MFRC522_REG_MF_RX             (0x1D)
#define DRV_MFRC522_REG_RESERVED13        (0x1E)
#define DRV_MFRC522_REG_SERIALSPEED       (0x1F)
// Page 2: CFG
#define DRV_MFRC522_REG_RESERVED20        (0x20)
#define DRV_MFRC522_REG_CRC_RESULT_M      (0x21)
#define DRV_MFRC522_REG_CRC_RESULT_L      (0x22)
#define DRV_MFRC522_REG_RESERVED21        (0x23)
#define DRV_MFRC522_REG_MOD_WIDTH         (0x24)
#define DRV_MFRC522_REG_RESERVED22        (0x25)
#define DRV_MFRC522_REG_RF_CFG            (0x26)
#define DRV_MFRC522_REG_GS_N              (0x27)
#define DRV_MFRC522_REG_CWGS_PREG         (0x28)
#define DRV_MFRC522_REG_MODGS_PREG        (0x29)
#define DRV_MFRC522_REG_T_MODE            (0x2A)
#define DRV_MFRC522_REG_T_PRESCALER       (0x2B)
#define DRV_MFRC522_REG_T_RELOAD_H        (0x2C)
#define DRV_MFRC522_REG_T_RELOAD_L        (0x2D)
#define DRV_MFRC522_REG_T_COUNTER_VALUE_H (0x2E)
#define DRV_MFRC522_REG_T_COUNTER_VALUE_L (0x2F)
// Page 3:TestRegister
#define DRV_MFRC522_REG_RESERVED30        (0x30)
#define DRV_MFRC522_REG_TEST_SEL1         (0x31)
#define DRV_MFRC522_REG_TEST_SEL2         (0x32)
#define DRV_MFRC522_REG_TEST_PIN_EN       (0x33)
#define DRV_MFRC522_REG_TEST_PIN_VALUE    (0x34)
#define DRV_MFRC522_REG_TEST_BUS          (0x35)
#define DRV_MFRC522_REG_AUTO_TEST         (0x36)
#define DRV_MFRC522_REG_VERSION           (0x37)
#define DRV_MFRC522_REG_ANALOG_TEST       (0x38)
#define DRV_MFRC522_REG_TEST_ADC1         (0x39)
#define DRV_MFRC522_REG_TEST_ADC2         (0x3A)
#define DRV_MFRC522_REG_TEST_ADC0         (0x3B)
#define DRV_MFRC522_REG_RESERVED31        (0x3C)
#define DRV_MFRC522_REG_RESERVED32        (0x3D)
#define DRV_MFRC522_REG_RESERVED33        (0x3E)
#define DRV_MFRC522_REG_RESERVED34        (0x3F)
// Dummy byte
#define DRV_MFRC522_DUMMY                 (0x00)

#define DRV_MFRC522_MAX_LEN               (16)

/* Public enumerate/structure ----------------------------------------- */

typedef enum
{
  drv_mfrc522_error = 0,
  drv_mfrc522_success,
  drv_mfrc522_invalid_id,
  drv_mfrc522_id_exist,
  drv_mfrc522_spi_no_init,
  drv_mfrc522_no_init,
  drv_mfrc522_invalid_parameter,
} drv_mfrc522_status_t;

typedef struct
{
  /* Page: 0 */
  uint8_t command_reg;
  uint8_t com_ien_reg;
  uint8_t div_ien_reg;
  uint8_t com_irq_reg;
  uint8_t div_irq_reg;
  uint8_t error_reg;
  uint8_t status1_reg;
  uint8_t status2_reg;
  uint8_t fifo_data_reg;
  uint8_t fifo_level_reg;
  uint8_t water_level_reg;
  uint8_t control_reg;
  uint8_t bit_framing_reg;
  uint8_t coll_reg;
  /* Page: 1 */
  uint8_t mode_reg;
  uint8_t tx_mode_reg;
  uint8_t rx_mode_reg;
  uint8_t tx_control_reg;
  uint8_t tx_ask_reg;
  uint8_t tx_sel_reg;
  uint8_t rx_sel_reg;
  uint8_t rx_threshold_reg;
  uint8_t demod_reg;
  uint8_t mf_tx_reg;
  uint8_t mf_rx_reg;
  uint8_t serial_speed_reg;
  /* Page:2 */
  uint8_t crc_result_reg[2];
  uint8_t mod_width_reg;
  uint8_t rf_cfg_reg;
  uint8_t gsn_reg;
  uint8_t cwgs_preg;
  uint8_t modgs_preg;
  uint8_t t_mode_reg;
  uint8_t t_prescaler_reg;
  uint8_t t_reload_reg[2];
  uint8_t t_countervalue_reg[2];
} drv_mfrc522_info_t;

typedef struct
{
  SPI_HandleTypeDef *drv_mfrc522_spi;
  GPIO_TypeDef      *drv_mfrc522_cs_port;
  uint32_t           drv_mfrc522_cs_pin;
  uint8_t            drv_mfrc522_id;
} drv_mfrc522_config_t;

/* Public macros ------------------------------------------------------ */

/* Public variables --------------------------------------------------- */

/* Public function prototypes ----------------------------------------- */

drv_mfrc522_status_t drv_mfrc522_init(drv_mfrc522_config_t *drv_mfrc522);

drv_mfrc522_status_t drv_mfrc522_write_address(drv_mfrc522_config_t drv_mfrc522, uint8_t block_address, uint8_t *send_data);

drv_mfrc522_status_t drv_mfrc522_read_address(drv_mfrc522_config_t drv_mfrc522, uint8_t block_address, uint8_t *receive_data);

drv_mfrc522_status_t drv_mfrc522_read_id_card(drv_mfrc522_config_t drv_mfrc522, uint8_t *card_id);

#endif  // __DRV_DS1307_H

/* End of file -------------------------------------------------------- */
