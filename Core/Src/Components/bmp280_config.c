/**
  ******************************************************************************
  * @file    bmp280_config.c
  * @author  AW
  * @version V1.0
  * @date    15-Nov-2020
  * @brief   Configuration file for BMP280 sensor driver library:
  *          SPI routines implementation.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "Components/bmp280.h"
#include "Components/bmp280_defs.h"
#include "Components/bmp280_config.h"
#include "main.h"
#include "spi.h"

/* Typedef -------------------------------------------------------------------*/

/* Define --------------------------------------------------------------------*/

/* Macro ---------------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/
BMP280_CS_PortType BMP280_CS_Ports[BMP280_NUM_OF_SENSORS] = {
  BMP280_CS1_GPIO_Port, BMP280_CS2_GPIO_Port
};

BMP280_CS_PinType BMP280_CS_Pins[BMP280_NUM_OF_SENSORS] = {
  BMP280_CS1_Pin,       BMP280_CS2_Pin
};

/* Public variables ----------------------------------------------------------*/
struct bmp280_dev bmp280_1 = {
  .dev_id = (uint8_t)BMP280_CS1,
  .intf = BMP280_SPI_INTF,
  .read = bmp280_spi_reg_read, .write = bmp280_spi_reg_write,
  .delay_ms = HAL_Delay
};

struct bmp280_dev bmp280_2 = {
  .dev_id = (uint8_t)BMP280_CS2,
  .intf = BMP280_SPI_INTF,
  .read = bmp280_spi_reg_read, .write = bmp280_spi_reg_write,
  .delay_ms = HAL_Delay
};

/* Private function prototypes -----------------------------------------------*/

/* Private function ----------------------------------------------------------*/

/* Public function -----------------------------------------------------------*/

/*!
 *  @brief BMP280 initialization function.
 *  @note Enables both pressure and temperature measurement with no oversampling.
 *        Disables internal digital filters. Sets measurement frequency to 4 Hz.
 *        Uses blocking mode SPI transmitting and receiving routine.
 *  @param[in] bmp BMP280 device structure
 *  @return Status of execution
 *  @retval 0  -> Success
 *  @retval >0 -> Failure Info
 *
 */
int8_t BMP280_Init(struct bmp280_dev* bmp)
{
  int8_t rslt;
  struct bmp280_config conf;

  rslt = bmp280_init(bmp);

  /* Always read the current settings before writing, especially when all the configuration is not modified  */
  rslt = bmp280_get_config(&conf, bmp);

  /* configuring the temperature oversampling, filter coefficient and output data rate */
  /* Overwrite the desired settings */
  conf.filter = BMP280_FILTER_OFF;

  /* Temperature oversampling set at 1x */
  conf.os_temp = BMP280_OS_1X;

  /* Temperature oversampling set at 1x */
  conf.os_pres = BMP280_OS_1X;

  /* Setting the output data rate as 4 Hz (250 ms) */
  conf.odr = BMP280_ODR_250_MS;

  rslt = bmp280_set_config(&conf, bmp);

  /* Always set the power mode after setting the configuration */
  rslt = bmp280_set_power_mode(BMP280_NORMAL_MODE, bmp);

  return rslt;
}

/*!
 *  @brief Function for writing the sensor's registers through SPI bus.
 *  @note Uses blocking mode SPI transmitting routine.
 *  @param[in] cs       Chip select to enable the sensor.
 *  @param[in] reg_addr Register address.
 *  @param[in] reg_data Pointer to the data buffer whose data has to be written.
 *  @param[in] length   No of bytes to write.
 *  @return Status of execution
 *  @retval 0 -> Success
 *  @retval >0 -> Failure Info
 *
 */
int8_t bmp280_spi_reg_write(uint8_t cs, uint8_t reg_addr, uint8_t *reg_data, uint16_t length)
{
  /* Implement the SPI write routine according to the target machine. */
  HAL_StatusTypeDef status = HAL_OK;
  int32_t iError = BMP280_OK;
  uint8_t txarray[BMP280_SPI_BUFFER_LEN];

  txarray[BMP280_REG_ADDR_INDEX] = reg_addr;
  for(uint8_t i = 0; i < length; i++)
  {
    txarray[i+BMP280_DATA_INDEX] = reg_data[i];
  }

  /* Software slave selection procedure */
  HAL_GPIO_WritePin( BMP280_CS_Ports[cs], BMP280_CS_Pins[cs], GPIO_PIN_RESET );

  /* Data exchange */
  status = HAL_SPI_Transmit(BMP280_SPI, (uint8_t*)(&txarray), length+BMP280_REG_ADDR_LEN, BMP280_TIMEOUT);
  //while( BMP280_SPI->State == HAL_SPI_STATE_BUSY ) {};

  /* Disable all slaves */
  for(uint8_t i = 0; i < BMP280_NUM_OF_SENSORS; i++)
  {
    HAL_GPIO_WritePin( BMP280_CS_Ports[i], BMP280_CS_Pins[i], GPIO_PIN_SET );
  }

  if (status != HAL_OK)
  {
  	// The BMP280 API calls for 0 return value as a success, and -1 returned as failure
  	iError = (-1);
  }

  return (int8_t)iError;
}

/*!
 *  @brief Function for reading the sensor's registers through SPI bus.
 *  @note Uses blocking mode SPI receiving routine.
 *  @param[in]  cs       Chip select to enable the sensor
 *  @param[in]  reg_addr Register address
 *  @param[out] reg_data Pointer to the data buffer to store the read data
 *  @param[in]  length   No of bytes to read
 *  @return Status of execution
 *  @retval 0  -> Success
 *  @retval >0 -> Failure Info
 *
 */
int8_t bmp280_spi_reg_read(uint8_t cs, uint8_t reg_addr, uint8_t *reg_data, uint16_t length)
{
  /* Implement the SPI read routine according to the target machine. */
  HAL_StatusTypeDef status = HAL_OK;
  int32_t iError = BMP280_OK;
  uint8_t txarray[BMP280_SPI_BUFFER_LEN] = {0,};
  uint8_t rxarray[BMP280_SPI_BUFFER_LEN] = {0,};

  txarray[BMP280_REG_ADDR_INDEX] = reg_addr;

  /* Software slave selection procedure */
  HAL_GPIO_WritePin( BMP280_CS_Ports[cs], BMP280_CS_Pins[cs], GPIO_PIN_RESET );

  /* Data exchange */
  status = HAL_SPI_TransmitReceive(BMP280_SPI, (uint8_t*)(&txarray), (uint8_t*)(&rxarray), length+BMP280_REG_ADDR_LEN, BMP280_TIMEOUT);
  //while( BMP280_SPI->State == HAL_SPI_STATE_BUSY ) {};

  /* Disable all slaves */
  for(uint8_t i = 0; i < BMP280_NUM_OF_SENSORS; i++)
  {
    HAL_GPIO_WritePin( BMP280_CS_Ports[i], BMP280_CS_Pins[i], GPIO_PIN_SET );
  }

  for (uint8_t i = 0; i < length; i++) {
    reg_data[i] = rxarray[i+BMP280_DATA_INDEX];
  }

  if (status != HAL_OK)
  {
  	// The BME280 API calls for 0 return value as a success, and -1 returned as failure
  	iError = (-1);
  }

  return (int8_t)iError;
}
