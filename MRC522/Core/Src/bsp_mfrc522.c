/**
  ******************************************************************************

  BSP MFRC522 For STM32F446RE
  Author:   LVQ
  Updated:  12 August 2022

  ******************************************************************************
*/

#include "bsp_mfrc522.h"

#define bsp_mfrc522_check_hal(x) \
  if ((x) != HAL_OK)             \
  {                              \
    return bsp_mfrc522_error;    \
  }

#define bsp_mfrc522_check_status(x) \
  if ((x) != bsp_mfrc522_ok)        \
  {                                 \
    return bsp_mfrc522_error;       \
  }

bsp_mfrc522_status_t bsp_mfrc522_cs(uint8_t bit)
{
  HAL_GPIO_WritePin(BSP_MFRC522_CS_PORT, BSP_MFRC522_CS_PIN, (bit ? GPIO_PIN_SET : GPIO_PIN_RESET));
  return bsp_mfrc522_ok;
}

bsp_mfrc522_status_t bsp_mfrc522_write_reg(uint8_t reg, uint8_t data)
{
  uint8_t reg_value = ((reg << 1) & 0x7E);
  bsp_mfrc522_cs(0);
  bsp_mfrc522_check_hal(HAL_SPI_Transmit(BSP_MFRC522_SPI, &reg_value, 1, BSP_MFRC522_TIMEOUT));
  bsp_mfrc522_check_hal(HAL_SPI_Transmit(BSP_MFRC522_SPI, &data, 1, BSP_MFRC522_TIMEOUT));
  bsp_mfrc522_cs(1);
  return bsp_mfrc522_ok;
}

uint8_t bsp_mfrc522_read_reg(uint8_t reg)
{
  uint8_t reg_value = (((reg << 1) & 0x7E) | 0x80);
  uint8_t value     = 0;
  bsp_mfrc522_cs(0);
  HAL_SPI_Transmit(BSP_MFRC522_SPI, &reg_value, 1, BSP_MFRC522_TIMEOUT);
  HAL_SPI_Receive(BSP_MFRC522_SPI, &value, 1, BSP_MFRC522_TIMEOUT);
  bsp_mfrc522_cs(1);
  return value;
}

bsp_mfrc522_status_t bsp_mfrc522_set_bit_reg(uint8_t reg, uint8_t bit)
{
  return bsp_mfrc522_write_reg(reg, bsp_mfrc522_read_reg(reg) | bit);
}

bsp_mfrc522_status_t bsp_mfrc522_clear_bit_reg(uint8_t reg, uint8_t bit)
{
  return bsp_mfrc522_write_reg(reg, bsp_mfrc522_read_reg(reg) & (~bit));
}

bsp_mfrc522_status_t bsp_mfrc522_to_card(uint8_t command, uint8_t *send_data, uint8_t send_len, uint8_t *back_data, uint16_t *back_len)
{
  bsp_mfrc522_status_t status  = bsp_mfrc522_error;
  uint8_t              irqEn   = 0x00;
  uint8_t              waitIRq = 0x00;
  uint8_t              lastBits;
  uint8_t              n;
  uint16_t             i;

  switch (command)
  {
  case BSP_PCD_AUTHENT:
  {
    irqEn   = 0x12;
    waitIRq = 0x10;
    break;
  }
  case BSP_PCD_TRANSCEIVE:
  {
    irqEn   = 0x77;
    waitIRq = 0x30;
    break;
  }
  default: break;
  }

  /* enable interrupt */
  bsp_mfrc522_write_reg(BSP_MFRC522_REG_COMM_I_EN, irqEn | 0x80);
  /* clear interrupt */
  bsp_mfrc522_clear_bit_reg(BSP_MFRC522_REG_COMM_IRQ, 0x80);
  /* clear FIFO */
  bsp_mfrc522_set_bit_reg(BSP_MFRC522_REG_FIFO_LEVEL, 0x80);

  bsp_mfrc522_write_reg(BSP_MFRC522_REG_COMMAND, BSP_PCD_IDLE);

  // Writing data to the FIFO
  for (i = 0; i < send_len; i++)
  {
    bsp_mfrc522_write_reg(BSP_MFRC522_REG_FIFO_DATA, send_data[i]);
  }

  // Execute the command
  bsp_mfrc522_write_reg(BSP_MFRC522_REG_COMMAND, command);
  if (command == BSP_PCD_TRANSCEIVE)
  {
    bsp_mfrc522_set_bit_reg(BSP_MFRC522_REG_BIT_FRAMING, 0x80);
  }

  i = 2000;
  do
  {
    /* Set1 TxIRq RxIRq IdleIRq HiAlerIRq LoAlertIRq ErrIRq TimerIRq */
    n = bsp_mfrc522_read_reg(BSP_MFRC522_REG_COMM_IRQ);
    i--;
  } while ((i != 0) && !(n & 0x01) && !(n & waitIRq));

  bsp_mfrc522_clear_bit_reg(BSP_MFRC522_REG_BIT_FRAMING, 0x80);

  if (i != 0)
  {
    if (!(bsp_mfrc522_read_reg(BSP_MFRC522_REG_ERROR) & 0x1B))
    {
      status = bsp_mfrc522_ok;
      if (n & irqEn & 0x01)
      {
        status = bsp_mfrc522_error;
      }

      if (command == BSP_PCD_TRANSCEIVE)
      {
        n        = bsp_mfrc522_read_reg(BSP_MFRC522_REG_FIFO_LEVEL);
        lastBits = bsp_mfrc522_read_reg(BSP_MFRC522_REG_CONTROL) & 0x07;
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
        if (n > BSP_MFRC522_MAX_LEN)
        {
          n = BSP_MFRC522_MAX_LEN;
        }

        /* reading the received data in FIFO */
        for (i = 0; i < n; i++)
        {
          back_data[i] = bsp_mfrc522_read_reg(BSP_MFRC522_REG_FIFO_DATA);
        }
      }
    }
    else
    {
      status = bsp_mfrc522_error;
    }
  }
  return status;
}

bsp_mfrc522_status_t bsp_mfrc522_request(uint8_t request_mode, uint8_t *tag_type)
{
  bsp_mfrc522_status_t status;
  uint16_t             back_bits;

  bsp_mfrc522_write_reg(BSP_MFRC522_REG_BIT_FRAMING, 0x07);

  tag_type[0] = request_mode;
  status      = bsp_mfrc522_to_card(BSP_PCD_TRANSCEIVE, tag_type, 1, tag_type, &back_bits);

  if ((status != bsp_mfrc522_ok) || (back_bits != 0x10))
  {
    status = bsp_mfrc522_error;
  }

  return status;
}

bsp_mfrc522_status_t bsp_mfrc522_anticoll(uint8_t *data)
{
  bsp_mfrc522_status_t status;
  uint8_t              i;
  uint8_t              num_check = 0;
  uint16_t             len;

  bsp_mfrc522_write_reg(BSP_MFRC522_REG_BIT_FRAMING, 0x00);

  data[0] = BSP_PICC_ANTICOLL;
  data[1] = 0x20;
  status  = bsp_mfrc522_to_card(BSP_PCD_TRANSCEIVE, data, 2, data, &len);

  if (status == bsp_mfrc522_ok)
  {
    /* check card serial number */
    for (i = 0; i < 4; i++)
    {
      num_check ^= data[i];
    }
    if (num_check != data[i])
    {
      status = bsp_mfrc522_error;
    }
  }
  return status;
}

void bsp_mfrc522_calculate_crc(uint8_t *data_in, uint8_t len, uint8_t *data_out)
{
  uint8_t i, n;

  bsp_mfrc522_clear_bit_reg(BSP_MFRC522_REG_DIV_IRQ, 0x04);
  bsp_mfrc522_set_bit_reg(BSP_MFRC522_REG_FIFO_LEVEL, 0x80);

  /* writing data to the FIFO */
  for (i = 0; i < len; i++)
  {
    bsp_mfrc522_write_reg(BSP_MFRC522_REG_FIFO_DATA, *(data_in + i));
  }
  bsp_mfrc522_write_reg(BSP_MFRC522_REG_COMMAND, BSP_PCD_CALCCRC);

  i = 0xFF;
  do
  {
    n = bsp_mfrc522_read_reg(BSP_MFRC522_REG_DIV_IRQ);
    i--;
  } while ((i != 0) && !(n & 0x04));

  /* read CRC calculation result */
  data_out[0] = bsp_mfrc522_read_reg(BSP_MFRC522_REG_CRC_RESULT_L);
  data_out[1] = bsp_mfrc522_read_reg(BSP_MFRC522_REG_CRC_RESULT_M);
}

bsp_mfrc522_status_t bsp_mfrc522_haft(void)
{
  uint16_t len;
  uint8_t  buff[4];

  buff[0] = BSP_PICC_HALT;
  buff[1] = 0;
  bsp_mfrc522_calculate_crc(buff, 2, &buff[2]);

  return bsp_mfrc522_to_card(BSP_PCD_TRANSCEIVE, buff, 4, buff, &len);
}

bsp_mfrc522_status_t bsp_mfrc522_read_address(uint8_t block_address, uint8_t *receive_data)
{
  bsp_mfrc522_status_t status;
  uint16_t             len;
  receive_data[0] = BSP_PICC_READ;
  receive_data[1] = block_address;
  bsp_mfrc522_calculate_crc(receive_data, 2, &receive_data[2]);
  status = bsp_mfrc522_to_card(BSP_PCD_TRANSCEIVE, receive_data, 4, receive_data, &len);

  if ((status != bsp_mfrc522_ok) || (len != 0x90))
  {
    status = bsp_mfrc522_error;
  }
  return status;
}

bsp_mfrc522_status_t bsp_mfrc522_write_address(uint8_t block_address, uint8_t *send_data)
{
  bsp_mfrc522_status_t status;
  uint16_t             receive_bits;
  uint8_t              i;
  uint8_t              buff[18];

  buff[0] = BSP_PICC_WRITE;
  buff[1] = block_address;
  bsp_mfrc522_calculate_crc(buff, 2, &buff[2]);
  status = bsp_mfrc522_to_card(BSP_PCD_TRANSCEIVE, buff, 4, buff, &receive_bits);

  if ((status != bsp_mfrc522_ok) || (receive_bits != 4) || ((buff[0] & 0x0F) != 0x0A))
  {
    status = bsp_mfrc522_error;
  }
  if (status == bsp_mfrc522_ok)
  {
    for (i = 0; i < 16; i++)
    {
      buff[i] = *(send_data + i);
    }
    bsp_mfrc522_calculate_crc(buff, 16, &buff[16]);
    status = bsp_mfrc522_to_card(BSP_PCD_TRANSCEIVE, buff, 18, buff, &receive_bits);

    if ((status != bsp_mfrc522_ok) || (receive_bits != 4) || ((buff[0] & 0x0F) != 0x0A))
    {
      status = bsp_mfrc522_error;
    }
  }
  return status;
}

bsp_mfrc522_status_t bsp_mfrc522_antenna_on(void)
{
  uint8_t temp = bsp_mfrc522_read_reg(BSP_MFRC522_REG_TX_CONTROL);
  return bsp_mfrc522_write_reg(BSP_MFRC522_REG_TX_CONTROL, temp | 0x03);
}

bsp_mfrc522_status_t bsp_mfrc522_init(void)
{
  bsp_mfrc522_cs(1);

  bsp_mfrc522_write_reg(BSP_MFRC522_REG_COMMAND, BSP_PCD_RESETPHASE);

  HAL_Delay(1);

  bsp_mfrc522_write_reg(BSP_MFRC522_REG_T_MODE, 0x8D);
  bsp_mfrc522_write_reg(BSP_MFRC522_REG_T_PRESCALER, 0x3E);
  bsp_mfrc522_write_reg(BSP_MFRC522_REG_T_RELOAD_L, 30);
  bsp_mfrc522_write_reg(BSP_MFRC522_REG_T_RELOAD_H, 0);

  /* 48dB gain */
  bsp_mfrc522_write_reg(BSP_MFRC522_REG_RF_CFG, 0x70);
  bsp_mfrc522_write_reg(BSP_MFRC522_REG_TX_AUTO, 0x40);
  bsp_mfrc522_write_reg(BSP_MFRC522_REG_MODE, 0x3D);

  bsp_mfrc522_antenna_on(); /* open the antenna */

  return bsp_mfrc522_ok;
}

bsp_mfrc522_status_t bsp_mfrc522_read_all(bsp_mfrc522_info_t *mfrc522_info)
{
  /* Page: 0 */
  mfrc522_info->command_reg     = bsp_mfrc522_read_reg(BSP_MFRC522_REG_COMMAND);
  mfrc522_info->com_ien_reg     = bsp_mfrc522_read_reg(BSP_MFRC522_REG_COMM_I_EN);
  mfrc522_info->div_ien_reg     = bsp_mfrc522_read_reg(BSP_MFRC522_REG_DIV1_EN);
  mfrc522_info->com_irq_reg     = bsp_mfrc522_read_reg(BSP_MFRC522_REG_COMM_IRQ);
  mfrc522_info->div_irq_reg     = bsp_mfrc522_read_reg(BSP_MFRC522_REG_DIV_IRQ);
  mfrc522_info->error_reg       = bsp_mfrc522_read_reg(BSP_MFRC522_REG_ERROR);
  mfrc522_info->status1_reg     = bsp_mfrc522_read_reg(BSP_MFRC522_REG_STATUS1);
  mfrc522_info->status2_reg     = bsp_mfrc522_read_reg(BSP_MFRC522_REG_STATUS2);
  mfrc522_info->fifo_data_reg   = bsp_mfrc522_read_reg(BSP_MFRC522_REG_FIFO_DATA);
  mfrc522_info->fifo_level_reg  = bsp_mfrc522_read_reg(BSP_MFRC522_REG_FIFO_LEVEL);
  mfrc522_info->water_level_reg = bsp_mfrc522_read_reg(BSP_MFRC522_REG_WATER_LEVEL);
  mfrc522_info->control_reg     = bsp_mfrc522_read_reg(BSP_MFRC522_REG_CONTROL);
  mfrc522_info->bit_framing_reg = bsp_mfrc522_read_reg(BSP_MFRC522_REG_BIT_FRAMING);
  mfrc522_info->coll_reg        = bsp_mfrc522_read_reg(BSP_MFRC522_REG_COLL);
  /* Page: 1 */
  mfrc522_info->mode_reg         = bsp_mfrc522_read_reg(BSP_MFRC522_REG_MODE);
  mfrc522_info->tx_mode_reg      = bsp_mfrc522_read_reg(BSP_MFRC522_REG_TX_MODE);
  mfrc522_info->rx_mode_reg      = bsp_mfrc522_read_reg(BSP_MFRC522_REG_RX_MODE);
  mfrc522_info->tx_control_reg   = bsp_mfrc522_read_reg(BSP_MFRC522_REG_TX_CONTROL);
  mfrc522_info->tx_ask_reg       = bsp_mfrc522_read_reg(BSP_MFRC522_REG_TX_AUTO);
  mfrc522_info->tx_sel_reg       = bsp_mfrc522_read_reg(BSP_MFRC522_REG_TX_SELL);
  mfrc522_info->rx_sel_reg       = bsp_mfrc522_read_reg(BSP_MFRC522_REG_RX_SELL);
  mfrc522_info->rx_threshold_reg = bsp_mfrc522_read_reg(BSP_MFRC522_REG_RX_THRESHOLD);
  mfrc522_info->demod_reg        = bsp_mfrc522_read_reg(BSP_MFRC522_REG_DEMOD);
  mfrc522_info->mf_tx_reg        = bsp_mfrc522_read_reg(BSP_MFRC522_REG_MF_TX);
  mfrc522_info->mf_rx_reg        = bsp_mfrc522_read_reg(BSP_MFRC522_REG_MF_RX);
  mfrc522_info->serial_speed_reg = bsp_mfrc522_read_reg(BSP_MFRC522_REG_SERIALSPEED);
  /* Page:2 */
  mfrc522_info->crc_result_reg[0]     = bsp_mfrc522_read_reg(BSP_MFRC522_REG_CRC_RESULT_M);
  mfrc522_info->crc_result_reg[1]     = bsp_mfrc522_read_reg(BSP_MFRC522_REG_CRC_RESULT_L);
  mfrc522_info->mod_width_reg         = bsp_mfrc522_read_reg(BSP_MFRC522_REG_MOD_WIDTH);
  mfrc522_info->rf_cfg_reg            = bsp_mfrc522_read_reg(BSP_MFRC522_REG_RF_CFG);
  mfrc522_info->gsn_reg               = bsp_mfrc522_read_reg(BSP_MFRC522_REG_GS_N);
  mfrc522_info->cwgs_preg             = bsp_mfrc522_read_reg(BSP_MFRC522_REG_CWGS_PREG);
  mfrc522_info->modgs_preg            = bsp_mfrc522_read_reg(BSP_MFRC522_REG_MODGS_PREG);
  mfrc522_info->t_mode_reg            = bsp_mfrc522_read_reg(BSP_MFRC522_REG_T_MODE);
  mfrc522_info->t_prescaler_reg       = bsp_mfrc522_read_reg(BSP_MFRC522_REG_T_PRESCALER);
  mfrc522_info->t_reload_reg[0]       = bsp_mfrc522_read_reg(BSP_MFRC522_REG_T_RELOAD_H);
  mfrc522_info->t_reload_reg[1]       = bsp_mfrc522_read_reg(BSP_MFRC522_REG_T_RELOAD_L);
  mfrc522_info->t_countervalue_reg[0] = bsp_mfrc522_read_reg(BSP_MFRC522_REG_T_COUNTER_VALUE_H);
  mfrc522_info->t_countervalue_reg[1] = bsp_mfrc522_read_reg(BSP_MFRC522_REG_T_COUNTER_VALUE_L);
  return bsp_mfrc522_ok;
}
