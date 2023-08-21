
/**
  ******************************************************************************

  DRIVER MFRC522 For STM32F446RE
  Author:   LVQ
  Updated:  14 August 2022

  ******************************************************************************
*/

#ifndef DRIVER_MFRC522_H
#define DRIVER_MFRC522_H

#include "main.h"
#include "bsp_mfrc522.h"

typedef enum
{
    drv_mfrc522_error,
    drv_mfrc522_ok,
}
drv_mfrc522_status_t;

drv_mfrc522_status_t drv_mfrc522_init(void);

drv_mfrc522_status_t drv_mfrc522_read_id_card(uint8_t *card_id);

#endif
/* DRIVER_MFRC522_H */