/**
  ******************************************************************************
  * @file    bmp280_config.h
  * @author  AW
  * @version V1.0
  * @date    15-Nov-2020
  * @brief   Configuration file for BMP280 sensor driver library: 
  *          SPI routines implementation.
  *
  ******************************************************************************
  */
#ifndef INC_BMP280_CONFIG_H_
#define INC_BMP280_CONFIG_H_

/* Config --------------------------------------------------------------------*/

/* Includes ------------------------------------------------------------------*/
#include "Components/bmp280.h"
#include "Components/bmp280_defs.h"

/* Typedef -------------------------------------------------------------------*/
#define BMP280_CS_PortType GPIO_TypeDef*
#define BMP280_CS_PinType uint16_t

typedef enum {
  BMP280_CS1, BMP280_CS2
} BMP280_CS_EnumType;

/* Define --------------------------------------------------------------------*/
#define BMP280_SPI (&hspi4)

#define BMP280_SPI_BUFFER_LEN  28  //! @see BMP280 technical note p. 24
#define BMP280_DATA_INDEX       1  //! @see BMP280 technical note pp. 31-32
#define BMP280_REG_ADDR_INDEX   0  //! @see BMP280 technical note pp. 31-32
#define BMP280_REG_ADDR_LEN     1  //! @see BMP280 technical note pp. 31-32

#define BMP280_TIMEOUT          5
#define BMP280_NUM_OF_SENSORS   2

/* Macro ---------------------------------------------------------------------*/

/* Public variables ----------------------------------------------------------*/
extern struct bmp280_dev bmp280_1;
extern struct bmp280_dev bmp280_2;

/* Public function prototypes ------------------------------------------------*/

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
int8_t BMP280_Init(struct bmp280_dev* bmp);

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
int8_t bmp280_spi_reg_write(uint8_t cs, uint8_t reg_addr, uint8_t *reg_data, uint16_t length);

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
int8_t bmp280_spi_reg_read(uint8_t cs, uint8_t reg_addr, uint8_t *reg_data, uint16_t length);

#endif /* INC_BMP280_CONFIG_H_ */
