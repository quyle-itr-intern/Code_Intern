/**
 * @file       drv_mfrc522.c
 * @copyright  Copyright (C) 2023 QuyLe Co., Ltd. All rights reserved.
 * @license    This project is released under the QuyLe License.
 * @version    V1.0.0
 * @date       2023-08-19
 * @author     quyle-itr-intern
 *
 * @brief      driver mfrc522
 *
 * @note
 */

/* Includes ----------------------------------------------------------- */
#include "drv_mfrc522.h"

/* Private variables -------------------------------------------------- */
static uint8_t drv_mfrc522_initialize = 0;
/* Variable must be initialize to usu #define DRV_DS1307_CHECK_STATUS(x) */
static uint8_t _status = 0;

/* Private defines ---------------------------------------------------- */

#define DRV_DS1307_CHECK_ID(x)               \
  if ((x < 0) || (x > 7))                    \
    return drv_mfrc522_invalid_id;           \
  if (((1 << (x)) & drv_mfrc522_initialize)) \
    return drv_mfrc522_id_exist;

#define DRV_MFRC522_CHECK_INIT(x)             \
  if (!((1 << (x)) & drv_mfrc522_initialize)) \
    return drv_mfrc522_no_init;

#define DRV_DS1307_CHECK_SPI(x)               \
  if ((drv_mfrc522->drv_mfrc522_spi) == NULL) \
    return drv_mfrc522_invalid_parameter;

#define DRV_MFRC522_CHECK_FAULT(x)          \
  {                                         \
    switch (x)                              \
    {                                       \
    case bsp_spi_not_initialize:            \
    {                                       \
      return drv_mfrc522_spi_no_init;       \
      break;                                \
    }                                       \
    case bsp_spi_error:                     \
    {                                       \
      return drv_mfrc522_error;             \
      break;                                \
    }                                       \
    case bsp_spi_invalid_parameter:         \
    {                                       \
      return drv_mfrc522_invalid_parameter; \
      break;                                \
    }                                       \
    default: break;                         \
    }                                       \
  }

#define DRV_MFRC522_CHECK_STATUS(x)     \
  _status = x;                          \
  if ((_status) != drv_mfrc522_success) \
    return _status;

/* Private enumerate/structure ---------------------------------------- */

/* Private macros ----------------------------------------------------- */

/* Public variables --------------------------------------------------- */

/* Private function prototypes ---------------------------------------- */

static drv_mfrc522_status_t drv_mfrc522_write_reg(drv_mfrc522_config_t drv_mfrc522, uint8_t reg_write, uint8_t data_write);
static drv_mfrc522_status_t drv_mfrc522_read_reg(drv_mfrc522_config_t drv_mfrc522, uint8_t reg_read, uint8_t *data_read);
static drv_mfrc522_status_t drv_mfrc522_set_bit_reg(drv_mfrc522_config_t drv_mfrc522, uint8_t reg, uint8_t bit);
static drv_mfrc522_status_t drv_mfrc522_clear_bit_reg(drv_mfrc522_config_t drv_mfrc522, uint8_t reg, uint8_t bit);
static drv_mfrc522_status_t drv_mfrc522_request(drv_mfrc522_config_t drv_mfrc522, uint8_t request_mode, uint8_t *tag_type);
static drv_mfrc522_status_t drv_mfrc522_anticoll(drv_mfrc522_config_t drv_mfrc522, uint8_t *data);
static drv_mfrc522_status_t drv_mfrc522_calculate_crc(drv_mfrc522_config_t drv_mfrc522, uint8_t *data_in, uint8_t len, uint8_t *data_out);
static drv_mfrc522_status_t drv_mfrc522_haft(drv_mfrc522_config_t drv_mfrc522);
static drv_mfrc522_status_t drv_mfrc522_antenna_on(drv_mfrc522_config_t drv_mfrc522);
static uint8_t              drv_mfrc522_read(drv_mfrc522_config_t drv_mfrc522, uint8_t reg_read);
static drv_mfrc522_status_t
drv_mfrc522_to_card(drv_mfrc522_config_t drv_mfrc522, uint8_t command, uint8_t *send_data, uint8_t send_len, uint8_t *back_data, uint16_t *back_len);

/* Function definitions ----------------------------------------------- */

static drv_mfrc522_status_t drv_mfrc522_write_reg(drv_mfrc522_config_t drv_mfrc522, uint8_t reg_write, uint8_t data_write)
{
  /* Check device init */
  DRV_MFRC522_CHECK_INIT(drv_mfrc522.drv_mfrc522_id);

  uint8_t reg_value = ((reg_write << 1) & 0x7E);
  bsp_gpio_reset_pin(drv_mfrc522.drv_mfrc522_cs_port, drv_mfrc522.drv_mfrc522_cs_pin);

  // if (drv_mfrc522.drv_mfrc522_spi->Instance == SPI1)
  //   DRV_MFRC522_CHECK_FAULT(bsp_spi1_write(reg_value, data_write))
  // else if (drv_mfrc522.drv_mfrc522_spi->Instance == SPI2)
  //   DRV_MFRC522_CHECK_FAULT(bsp_spi2_write(reg_value, data_write))
  // else if (drv_mfrc522.drv_mfrc522_spi->Instance == SPI3)
  //   DRV_MFRC522_CHECK_FAULT(bsp_spi3_write(reg_value, data_write))

  HAL_SPI_Transmit(drv_mfrc522.drv_mfrc522_spi, &reg_value, 1, 500);
  HAL_SPI_Transmit(drv_mfrc522.drv_mfrc522_spi, &data_write, 1, 500);

  bsp_gpio_set_pin(drv_mfrc522.drv_mfrc522_cs_port, drv_mfrc522.drv_mfrc522_cs_pin);
  return drv_mfrc522_success;
}

static drv_mfrc522_status_t drv_mfrc522_read_reg(drv_mfrc522_config_t drv_mfrc522, uint8_t reg_read, uint8_t *data_read)
{
  /* Check device init */
  DRV_MFRC522_CHECK_INIT(drv_mfrc522.drv_mfrc522_id);

  uint8_t reg_value = (((reg_read << 1) & 0x7E) | 0x80);
  bsp_gpio_reset_pin(drv_mfrc522.drv_mfrc522_cs_port, drv_mfrc522.drv_mfrc522_cs_pin);

  // if (drv_mfrc522.drv_mfrc522_spi->Instance == SPI1)
  //   DRV_MFRC522_CHECK_FAULT(bsp_spi1_read(reg_value, data_read))
  // else if (drv_mfrc522.drv_mfrc522_spi->Instance == SPI2)
  //   DRV_MFRC522_CHECK_FAULT(bsp_spi2_read(reg_value, data_read))
  // else if (drv_mfrc522.drv_mfrc522_spi->Instance == SPI3)
  //   DRV_MFRC522_CHECK_FAULT(bsp_spi3_read(reg_value, data_read))

  HAL_SPI_Transmit(drv_mfrc522.drv_mfrc522_spi, &reg_value, 1, 500);
  HAL_SPI_Receive(drv_mfrc522.drv_mfrc522_spi, data_read, 1, 500);

  bsp_gpio_set_pin(drv_mfrc522.drv_mfrc522_cs_port, drv_mfrc522.drv_mfrc522_cs_pin);
  return drv_mfrc522_success;
}

static uint8_t drv_mfrc522_read(drv_mfrc522_config_t drv_mfrc522, uint8_t reg_read)
{
  /* Check device init */
  DRV_MFRC522_CHECK_INIT(drv_mfrc522.drv_mfrc522_id);

  uint8_t data_read = 0;
  uint8_t reg_value = (((reg_read << 1) & 0x7E) | 0x80);

  bsp_gpio_reset_pin(drv_mfrc522.drv_mfrc522_cs_port, drv_mfrc522.drv_mfrc522_cs_pin);

  if (drv_mfrc522.drv_mfrc522_spi->Instance == SPI1)
    DRV_MFRC522_CHECK_FAULT(bsp_spi1_read(reg_value, &data_read))
  else if (drv_mfrc522.drv_mfrc522_spi->Instance == SPI2)
    DRV_MFRC522_CHECK_FAULT(bsp_spi2_read(reg_value, &data_read))
  else if (drv_mfrc522.drv_mfrc522_spi->Instance == SPI3)
    DRV_MFRC522_CHECK_FAULT(bsp_spi3_read(reg_value, &data_read))

  bsp_gpio_set_pin(drv_mfrc522.drv_mfrc522_cs_port, drv_mfrc522.drv_mfrc522_cs_pin);
  return data_read;
}

static drv_mfrc522_status_t drv_mfrc522_set_bit_reg(drv_mfrc522_config_t drv_mfrc522, uint8_t reg, uint8_t bit)
{
  /* Check device init */
  DRV_MFRC522_CHECK_INIT(drv_mfrc522.drv_mfrc522_id);
  uint8_t value = 0;
  drv_mfrc522_read_reg(drv_mfrc522, reg, &value);
  return drv_mfrc522_write_reg(drv_mfrc522, reg, value | bit);
}

static drv_mfrc522_status_t drv_mfrc522_clear_bit_reg(drv_mfrc522_config_t drv_mfrc522, uint8_t reg, uint8_t bit)
{
  /* Check device init */
  DRV_MFRC522_CHECK_INIT(drv_mfrc522.drv_mfrc522_id);
  uint8_t value = 0;
  drv_mfrc522_read_reg(drv_mfrc522, reg, &value);
  return drv_mfrc522_write_reg(drv_mfrc522, reg, value & (~bit));
}

static drv_mfrc522_status_t
drv_mfrc522_to_card(drv_mfrc522_config_t drv_mfrc522, uint8_t command, uint8_t *send_data, uint8_t send_len, uint8_t *back_data, uint16_t *back_len)
{
  /* Check device init */
  DRV_MFRC522_CHECK_INIT(drv_mfrc522.drv_mfrc522_id);

  drv_mfrc522_status_t status  = drv_mfrc522_error;
  uint8_t              irqEn   = 0x00;
  uint8_t              waitIRq = 0x00;
  uint8_t              lastBits;
  uint8_t              n;
  uint16_t             i;
  uint8_t              check;

  switch (command)
  {
  case DRV_PCD_AUTHENT:
  {
    irqEn   = 0x12;
    waitIRq = 0x10;
    break;
  }
  case DRV_PCD_TRANSCEIVE:
  {
    irqEn   = 0x77;
    waitIRq = 0x30;
    break;
  }
  default: break;
  }

  /* enable interrupt */
  drv_mfrc522_write_reg(drv_mfrc522, DRV_MFRC522_REG_COMM_I_EN, irqEn | 0x80);
  /* clear interrupt */
  drv_mfrc522_clear_bit_reg(drv_mfrc522, DRV_MFRC522_REG_COMM_IRQ, 0x80);
  /* clear FIFO */
  drv_mfrc522_set_bit_reg(drv_mfrc522, DRV_MFRC522_REG_FIFO_LEVEL, 0x80);

  drv_mfrc522_write_reg(drv_mfrc522, DRV_MFRC522_REG_COMMAND, DRV_PCD_IDLE);
  // Writing data to the FIFO
  for (i = 0; i < send_len; i++)
  {
    drv_mfrc522_write_reg(drv_mfrc522, DRV_MFRC522_REG_FIFO_DATA, send_data[i]);
  }
  // Execute the command
  drv_mfrc522_write_reg(drv_mfrc522, DRV_MFRC522_REG_COMMAND, command);
  if (command == DRV_PCD_TRANSCEIVE)
  {
    drv_mfrc522_set_bit_reg(drv_mfrc522, DRV_MFRC522_REG_BIT_FRAMING, 0x80);
  }
  i = 2000;
  do
  {
    /* Set1 TxIRq RxIRq IdleIRq HiAlerIRq LoAlertIRq ErrIRq TimerIRq */
    drv_mfrc522_read_reg(drv_mfrc522, DRV_MFRC522_REG_COMM_IRQ, &n);
    i--;
  } while ((i != 0) && !(n & 0x01) && !(n & waitIRq));

  drv_mfrc522_clear_bit_reg(drv_mfrc522, DRV_MFRC522_REG_BIT_FRAMING, 0x80);

  if (i != 0)
  {
    drv_mfrc522_read_reg(drv_mfrc522, DRV_MFRC522_REG_ERROR, &check);
    if (!(check & 0x1B))
    {
      status = drv_mfrc522_success;
      if (n & irqEn & 0x01)
      {
        status = drv_mfrc522_error;
      }

      if (command == DRV_PCD_TRANSCEIVE)
      {
        drv_mfrc522_read_reg(drv_mfrc522, DRV_MFRC522_REG_FIFO_LEVEL, &n);
        drv_mfrc522_read_reg(drv_mfrc522, DRV_MFRC522_REG_CONTROL, &lastBits);
        lastBits = lastBits & 0x07;
        if (lastBits)
        {
          *back_len = (n - 1) * 8 + lastBits;
        }
        else
        {
          *back_len = n * 8;
        }

        if (n == 0)
        {
          n = 1;
        }
        if (n > DRV_MFRC522_MAX_LEN)
        {
          n = DRV_MFRC522_MAX_LEN;
        }

        /* reading the received data in FIFO */
        for (i = 0; i < n; i++)
        {
          drv_mfrc522_read_reg(drv_mfrc522, DRV_MFRC522_REG_FIFO_DATA, &back_data[i]);
        }
      }
    }
    else
    {
      status = drv_mfrc522_error;
    }
  }
  return status;
}

static drv_mfrc522_status_t drv_mfrc522_request(drv_mfrc522_config_t drv_mfrc522, uint8_t request_mode, uint8_t *tag_type)
{
  /* Check device init */
  DRV_MFRC522_CHECK_INIT(drv_mfrc522.drv_mfrc522_id);

  drv_mfrc522_status_t status;
  uint16_t             back_bits;

  DRV_MFRC522_CHECK_STATUS(drv_mfrc522_write_reg(drv_mfrc522, DRV_MFRC522_REG_BIT_FRAMING, 0x07))

  tag_type[0] = request_mode;
  status      = drv_mfrc522_to_card(drv_mfrc522, DRV_PCD_TRANSCEIVE, tag_type, 1, tag_type, &back_bits);

  if ((status != drv_mfrc522_success) || (back_bits != 0x10))
  {
    status = drv_mfrc522_error;
  }
  return status;
}

static drv_mfrc522_status_t drv_mfrc522_anticoll(drv_mfrc522_config_t drv_mfrc522, uint8_t *data)
{
  /* Check device init */
  DRV_MFRC522_CHECK_INIT(drv_mfrc522.drv_mfrc522_id);

  drv_mfrc522_status_t status;
  uint8_t              i;
  uint8_t              num_check = 0;
  uint16_t             len;

  DRV_MFRC522_CHECK_STATUS(drv_mfrc522_write_reg(drv_mfrc522, DRV_MFRC522_REG_BIT_FRAMING, 0x00));

  data[0] = DRV_PICC_ANTICOLL;
  data[1] = 0x20;
  status  = drv_mfrc522_to_card(drv_mfrc522, DRV_PCD_TRANSCEIVE, data, 2, data, &len);

  if (status == drv_mfrc522_success)
  {
    /* check card serial number */
    for (i = 0; i < 4; i++)
    {
      num_check ^= data[i];
    }
    if (num_check != data[i])
    {
      status = drv_mfrc522_error;
    }
  }
  return status;
}

static drv_mfrc522_status_t drv_mfrc522_calculate_crc(drv_mfrc522_config_t drv_mfrc522, uint8_t *data_in, uint8_t len, uint8_t *data_out)
{
  /* Check device init */
  DRV_MFRC522_CHECK_INIT(drv_mfrc522.drv_mfrc522_id);

  uint8_t i, n;

  DRV_MFRC522_CHECK_STATUS(drv_mfrc522_clear_bit_reg(drv_mfrc522, DRV_MFRC522_REG_DIV_IRQ, 0x04))
  DRV_MFRC522_CHECK_STATUS(drv_mfrc522_set_bit_reg(drv_mfrc522, DRV_MFRC522_REG_FIFO_LEVEL, 0x80))

  /* writing data to the FIFO */
  for (i = 0; i < len; i++)
  {
    DRV_MFRC522_CHECK_STATUS(drv_mfrc522_write_reg(drv_mfrc522, DRV_MFRC522_REG_FIFO_DATA, *(data_in + i)));
  }
  DRV_MFRC522_CHECK_STATUS(drv_mfrc522_write_reg(drv_mfrc522, DRV_MFRC522_REG_COMMAND, DRV_PCD_CALCCRC));

  i = 0xFF;
  do
  {
    DRV_MFRC522_CHECK_STATUS(drv_mfrc522_read_reg(drv_mfrc522, DRV_MFRC522_REG_DIV_IRQ, &n))
    i--;
  } while ((i != 0) && !(n & 0x04));

  /* read CRC calculation result */
  DRV_MFRC522_CHECK_STATUS(drv_mfrc522_read_reg(drv_mfrc522, DRV_MFRC522_REG_CRC_RESULT_L, &data_out[0]))
  DRV_MFRC522_CHECK_STATUS(drv_mfrc522_read_reg(drv_mfrc522, DRV_MFRC522_REG_CRC_RESULT_M, &data_out[1]))
  return drv_mfrc522_success;
}

static drv_mfrc522_status_t drv_mfrc522_haft(drv_mfrc522_config_t drv_mfrc522)
{
  /* Check device init */
  DRV_MFRC522_CHECK_INIT(drv_mfrc522.drv_mfrc522_id);

  uint16_t len;
  uint8_t  buff[4];

  buff[0] = DRV_PICC_HALT;
  buff[1] = 0;
  DRV_MFRC522_CHECK_STATUS(drv_mfrc522_calculate_crc(drv_mfrc522, buff, 2, &buff[2]))
  return drv_mfrc522_to_card(drv_mfrc522, DRV_PCD_TRANSCEIVE, buff, 4, buff, &len);
}

drv_mfrc522_status_t drv_mfrc522_read_address(drv_mfrc522_config_t drv_mfrc522, uint8_t block_address, uint8_t *receive_data)
{
  /* Check device init */
  DRV_MFRC522_CHECK_INIT(drv_mfrc522.drv_mfrc522_id);

  drv_mfrc522_status_t status;
  uint16_t             len;
  receive_data[0] = DRV_PICC_READ;
  receive_data[1] = block_address;
  DRV_MFRC522_CHECK_STATUS(drv_mfrc522_calculate_crc(drv_mfrc522, receive_data, 2, &receive_data[2]));
  status = drv_mfrc522_to_card(drv_mfrc522, DRV_PCD_TRANSCEIVE, receive_data, 4, receive_data, &len);

  if ((status != drv_mfrc522_success) || (len != 0x90))
  {
    status = drv_mfrc522_error;
  }
  return status;
}

drv_mfrc522_status_t drv_mfrc522_write_address(drv_mfrc522_config_t drv_mfrc522, uint8_t block_address, uint8_t *send_data)
{
  /* Check device init */
  DRV_MFRC522_CHECK_INIT(drv_mfrc522.drv_mfrc522_id);

  drv_mfrc522_status_t status = drv_mfrc522_error;
  uint16_t             receive_bits;
  uint8_t              i;
  uint8_t              buff[18];

  buff[0] = DRV_PICC_WRITE;
  buff[1] = block_address;
  DRV_MFRC522_CHECK_STATUS(drv_mfrc522_calculate_crc(drv_mfrc522, buff, 2, &buff[2]))
  status = drv_mfrc522_to_card(drv_mfrc522, DRV_PCD_TRANSCEIVE, buff, 4, buff, &receive_bits);

  if ((status != drv_mfrc522_success))
  {
    status = drv_mfrc522_error;
  }
  if (status == drv_mfrc522_success)
  {
    for (i = 0; i < 16; i++)
    {
      buff[i] = *(send_data + i);
    }
    DRV_MFRC522_CHECK_STATUS(drv_mfrc522_calculate_crc(drv_mfrc522, buff, 16, &buff[16]))
    status = drv_mfrc522_to_card(drv_mfrc522, DRV_PCD_TRANSCEIVE, buff, 18, buff, &receive_bits);

    if ((status != drv_mfrc522_success))
    {
      status = drv_mfrc522_error;
    }
  }
  return status;
}

static drv_mfrc522_status_t drv_mfrc522_antenna_on(drv_mfrc522_config_t drv_mfrc522)
{
  /* Check device init */
  DRV_MFRC522_CHECK_INIT(drv_mfrc522.drv_mfrc522_id);

  uint8_t temp;
  DRV_MFRC522_CHECK_STATUS(drv_mfrc522_read_reg(drv_mfrc522, DRV_MFRC522_REG_TX_CONTROL, &temp))
  return drv_mfrc522_write_reg(drv_mfrc522, DRV_MFRC522_REG_TX_CONTROL, temp | 0x03);
}

drv_mfrc522_status_t drv_mfrc522_init(drv_mfrc522_config_t *drv_mfrc522)
{
  DRV_DS1307_CHECK_ID(drv_mfrc522->drv_mfrc522_id);
  DRV_DS1307_CHECK_SPI(drv_mfrc522->drv_mfrc522_spi);

  if (drv_mfrc522->drv_mfrc522_spi->Instance == SPI1)
    DRV_MFRC522_CHECK_FAULT(bsp_spi1_init(drv_mfrc522->drv_mfrc522_spi))
  else if (drv_mfrc522->drv_mfrc522_spi->Instance == SPI2)
    DRV_MFRC522_CHECK_FAULT(bsp_spi2_init(drv_mfrc522->drv_mfrc522_spi))
  else if (drv_mfrc522->drv_mfrc522_spi->Instance == SPI3)
    DRV_MFRC522_CHECK_FAULT(bsp_spi3_init(drv_mfrc522->drv_mfrc522_spi))

  /* Add device to private variable */
  drv_mfrc522_initialize |= (1 << drv_mfrc522->drv_mfrc522_id);
  bsp_gpio_set_pin(drv_mfrc522->drv_mfrc522_cs_port, drv_mfrc522->drv_mfrc522_cs_pin);

  /* Reset the Mifare Classic and NTAG devices */
  DRV_MFRC522_CHECK_STATUS(drv_mfrc522_write_reg(*drv_mfrc522, DRV_MFRC522_REG_COMMAND, DRV_PCD_RESETPHASE))

  DRV_MFRC522_CHECK_STATUS(drv_mfrc522_write_reg(*drv_mfrc522, DRV_MFRC522_REG_T_MODE, 0x8D))
  DRV_MFRC522_CHECK_STATUS(drv_mfrc522_write_reg(*drv_mfrc522, DRV_MFRC522_REG_T_PRESCALER, 0x3E))
  DRV_MFRC522_CHECK_STATUS(drv_mfrc522_write_reg(*drv_mfrc522, DRV_MFRC522_REG_T_RELOAD_L, 30))
  DRV_MFRC522_CHECK_STATUS(drv_mfrc522_write_reg(*drv_mfrc522, DRV_MFRC522_REG_T_RELOAD_H, 0))
  /* 48dB gain */
  DRV_MFRC522_CHECK_STATUS(drv_mfrc522_write_reg(*drv_mfrc522, DRV_MFRC522_REG_RF_CFG, 0x70))
  DRV_MFRC522_CHECK_STATUS(drv_mfrc522_write_reg(*drv_mfrc522, DRV_MFRC522_REG_TX_AUTO, 0x40))
  DRV_MFRC522_CHECK_STATUS(drv_mfrc522_write_reg(*drv_mfrc522, DRV_MFRC522_REG_MODE, 0x3D))
  DRV_MFRC522_CHECK_STATUS(drv_mfrc522_antenna_on(*drv_mfrc522))
  return drv_mfrc522_success;
}

drv_mfrc522_status_t drv_mfrc522_read_id_card(drv_mfrc522_config_t drv_mfrc522, uint8_t *card_id)
{
  /* Check device init */
  DRV_MFRC522_CHECK_INIT(drv_mfrc522.drv_mfrc522_id);
  uint8_t data[5];

  drv_mfrc522_status_t status;
  /* Find cards, return card type */
  status = drv_mfrc522_request(drv_mfrc522, DRV_PICC_REQIDL, data);
  if (status == drv_mfrc522_success)
  {
    /* Card detected */
    /* Anti-collision, return card serial number 4 bytes */
    status = drv_mfrc522_anticoll(drv_mfrc522, card_id);
  }
  drv_mfrc522_haft(drv_mfrc522); /* Command card into hibernation */
  return status;
}

drv_mfrc522_status_t drv_mfrc522_read_all(drv_mfrc522_config_t drv_mfrc522, drv_mfrc522_info_t *mfrc522_info)
{
  /* Check device init */
  DRV_MFRC522_CHECK_INIT(drv_mfrc522.drv_mfrc522_id);

  /* Page: 0 */
  mfrc522_info->command_reg     = drv_mfrc522_read(drv_mfrc522, DRV_MFRC522_REG_COMMAND);
  mfrc522_info->com_ien_reg     = drv_mfrc522_read(drv_mfrc522, DRV_MFRC522_REG_COMM_I_EN);
  mfrc522_info->div_ien_reg     = drv_mfrc522_read(drv_mfrc522, DRV_MFRC522_REG_DIV1_EN);
  mfrc522_info->com_irq_reg     = drv_mfrc522_read(drv_mfrc522, DRV_MFRC522_REG_COMM_IRQ);
  mfrc522_info->div_irq_reg     = drv_mfrc522_read(drv_mfrc522, DRV_MFRC522_REG_DIV_IRQ);
  mfrc522_info->error_reg       = drv_mfrc522_read(drv_mfrc522, DRV_MFRC522_REG_ERROR);
  mfrc522_info->status1_reg     = drv_mfrc522_read(drv_mfrc522, DRV_MFRC522_REG_STATUS1);
  mfrc522_info->status2_reg     = drv_mfrc522_read(drv_mfrc522, DRV_MFRC522_REG_STATUS2);
  mfrc522_info->fifo_data_reg   = drv_mfrc522_read(drv_mfrc522, DRV_MFRC522_REG_FIFO_DATA);
  mfrc522_info->fifo_level_reg  = drv_mfrc522_read(drv_mfrc522, DRV_MFRC522_REG_FIFO_LEVEL);
  mfrc522_info->water_level_reg = drv_mfrc522_read(drv_mfrc522, DRV_MFRC522_REG_WATER_LEVEL);
  mfrc522_info->control_reg     = drv_mfrc522_read(drv_mfrc522, DRV_MFRC522_REG_CONTROL);
  mfrc522_info->bit_framing_reg = drv_mfrc522_read(drv_mfrc522, DRV_MFRC522_REG_BIT_FRAMING);
  mfrc522_info->coll_reg        = drv_mfrc522_read(drv_mfrc522, DRV_MFRC522_REG_COLL);
  /* Page: 1 */
  mfrc522_info->mode_reg         = drv_mfrc522_read(drv_mfrc522, DRV_MFRC522_REG_MODE);
  mfrc522_info->tx_mode_reg      = drv_mfrc522_read(drv_mfrc522, DRV_MFRC522_REG_TX_MODE);
  mfrc522_info->rx_mode_reg      = drv_mfrc522_read(drv_mfrc522, DRV_MFRC522_REG_RX_MODE);
  mfrc522_info->tx_control_reg   = drv_mfrc522_read(drv_mfrc522, DRV_MFRC522_REG_TX_CONTROL);
  mfrc522_info->tx_ask_reg       = drv_mfrc522_read(drv_mfrc522, DRV_MFRC522_REG_TX_AUTO);
  mfrc522_info->tx_sel_reg       = drv_mfrc522_read(drv_mfrc522, DRV_MFRC522_REG_TX_SELL);
  mfrc522_info->rx_sel_reg       = drv_mfrc522_read(drv_mfrc522, DRV_MFRC522_REG_RX_SELL);
  mfrc522_info->rx_threshold_reg = drv_mfrc522_read(drv_mfrc522, DRV_MFRC522_REG_RX_THRESHOLD);
  mfrc522_info->demod_reg        = drv_mfrc522_read(drv_mfrc522, DRV_MFRC522_REG_DEMOD);
  mfrc522_info->mf_tx_reg        = drv_mfrc522_read(drv_mfrc522, DRV_MFRC522_REG_MF_TX);
  mfrc522_info->mf_rx_reg        = drv_mfrc522_read(drv_mfrc522, DRV_MFRC522_REG_MF_RX);
  mfrc522_info->serial_speed_reg = drv_mfrc522_read(drv_mfrc522, DRV_MFRC522_REG_SERIALSPEED);
  /* Page:2 */
  mfrc522_info->crc_result_reg[0]     = drv_mfrc522_read(drv_mfrc522, DRV_MFRC522_REG_CRC_RESULT_M);
  mfrc522_info->crc_result_reg[1]     = drv_mfrc522_read(drv_mfrc522, DRV_MFRC522_REG_CRC_RESULT_L);
  mfrc522_info->mod_width_reg         = drv_mfrc522_read(drv_mfrc522, DRV_MFRC522_REG_MOD_WIDTH);
  mfrc522_info->rf_cfg_reg            = drv_mfrc522_read(drv_mfrc522, DRV_MFRC522_REG_RF_CFG);
  mfrc522_info->gsn_reg               = drv_mfrc522_read(drv_mfrc522, DRV_MFRC522_REG_GS_N);
  mfrc522_info->cwgs_preg             = drv_mfrc522_read(drv_mfrc522, DRV_MFRC522_REG_CWGS_PREG);
  mfrc522_info->modgs_preg            = drv_mfrc522_read(drv_mfrc522, DRV_MFRC522_REG_MODGS_PREG);
  mfrc522_info->t_mode_reg            = drv_mfrc522_read(drv_mfrc522, DRV_MFRC522_REG_T_MODE);
  mfrc522_info->t_prescaler_reg       = drv_mfrc522_read(drv_mfrc522, DRV_MFRC522_REG_T_PRESCALER);
  mfrc522_info->t_reload_reg[0]       = drv_mfrc522_read(drv_mfrc522, DRV_MFRC522_REG_T_RELOAD_H);
  mfrc522_info->t_reload_reg[1]       = drv_mfrc522_read(drv_mfrc522, DRV_MFRC522_REG_T_RELOAD_L);
  mfrc522_info->t_countervalue_reg[0] = drv_mfrc522_read(drv_mfrc522, DRV_MFRC522_REG_T_COUNTER_VALUE_H);
  mfrc522_info->t_countervalue_reg[1] = drv_mfrc522_read(drv_mfrc522, DRV_MFRC522_REG_T_COUNTER_VALUE_L);
  return drv_mfrc522_success;
}

/* End of file -------------------------------------------------------- */
