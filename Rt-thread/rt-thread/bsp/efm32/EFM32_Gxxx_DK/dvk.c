/**************************************************************************//**
 * @file
 * @brief DVK board support package, initialization
 * @author Energy Micro AS
 * @version 1.7.3
 ******************************************************************************
 * @section License
 * <b>(C) Copyright 2010 Energy Micro AS, http://www.energymicro.com</b>
 ******************************************************************************
 *
 * This source code is the property of Energy Micro AS. The source and compiled
 * code may only be used on Energy Micro "EFM32" microcontrollers.
 *
 * This copyright notice may not be removed from the source code nor changed.
 *
 * DISCLAIMER OF WARRANTY/LIMITATION OF REMEDIES: Energy Micro AS has no
 * obligation to support this Software. Energy Micro AS is providing the
 * Software "AS IS", with no express or implied warranties of any kind,
 * including, but not limited to, any implied warranties of merchantability
 * or fitness for any particular purpose or warranties against infringement
 * of any proprietary rights of a third party.
 *
 * Energy Micro AS will not be liable for any consequential, incidental, or
 * special damages, or any other relief, or for any claim by any third party,
 * arising from your use of this Software.
 *
 *****************************************************************************/

#include "efm32.h"
#include "dvk.h"

/***************************************************************************//**
 * @addtogroup BSP
 * @{
 ******************************************************************************/

/**************************************************************************//**
 * @brief  Initializes DVK, configures board control access
 *****************************************************************************/
bool DVK_init(void)
{
  bool ret;
#ifdef DVK_EBI_CONTROL
  ret = DVK_EBI_init();
#endif
#ifdef DVK_SPI_CONTROL
  ret = DVK_SPI_init();
#endif  
  if ( ret == false )
  {
    /* Board is configured in wrong mode, please restart KIT! */
    while(1);
  }
  /* Inform AEM application that we are in Energy Mode 0 by default */
  DVK_setEnergyMode(0);
  return ret;
}

/**************************************************************************//**
 * @brief  Disables DVK, free up resources
 *****************************************************************************/
void DVK_disable(void)
{
#ifdef DVK_EBI_CONTROL
  /* Handover bus control */
  DVK_disableBus();
  /* Disable EBI interface */
  DVK_EBI_disable();
#endif

#ifdef DVK_SPI_CONTROL
  DVK_SPI_disable();
#endif
}

/** @} (end group BSP) */
