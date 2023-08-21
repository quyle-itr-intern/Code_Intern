/**
  ******************************************************************************

  DRIVER MFRC522 For STM32F446RE
  Author:   LVQ
  Updated:  14 August 2022

  ******************************************************************************
*/
#include "driver_mfrc522.h"

drv_mfrc522_status_t drv_mfrc522_init(void)
{
  return bsp_mfrc522_init();
}

drv_mfrc522_status_t drv_mfrc522_read_id_card(uint8_t *card_id)
{
  bsp_mfrc522_status_t status;
  /* Find cards, return card type */
  status = bsp_mfrc522_request(BSP_PICC_REQIDL, card_id);
  if (status == bsp_mfrc522_ok)
  {
    /* Card detected */
    /* Anti-collision, return card serial number 4 bytes */
    status = bsp_mfrc522_anticoll(card_id);
  }
  bsp_mfrc522_haft(); /* Command card into hibernation */
  return status;
}
