/* 
VCC = 1.8V to 5.5V
									256Page
128K (16,384 x 8)	64-byte Page Mode Block Write Protection

Supports SPI Modes 0 (0,0) and 3 (1,1):Data Sheet Describes Mode 0 Operation

20MHz Clock Rate (5V)

Self-timed Write Cycle (5ms max)

8-bit instruction registe;	MSB first and start with a high-to-low CS transition

Both Hardware and	Software Data Protection		16bits Address
*/

#ifndef SPI_EEPROM_AT25128_H_INCLUDED
#define SPI_EEPROM_AT25128_H_INCLUDED

#include <rtthread.h>
#include <drivers/spi.h>

struct spi_eeprom_at25128
{
    struct rt_device                eeprom_device;
    struct rt_device_blk_geometry   geometry;
    struct rt_spi_device *          rt_spi_device;
};

extern rt_err_t at25128_init(const char * feeprom_device_name, const char * spi_device_name);


#endif // SPI_EEPROM_AT25128_H_INCLUDED
