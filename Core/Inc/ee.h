#ifndef __EEPROM_H
#define __EEPROM_H

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdbool.h>
#include "main.h"

    uint32_t Flash_Write_Data(uint32_t StartPageAddress, uint32_t *Data, uint16_t numberofwords);
    void Flash_Read_Data(uint32_t StartPageAddress,__IO uint32_t *RxBuf, uint16_t numberofwords);
		void read_flash_memory(uint32_t memory_address, uint8_t *data, uint16_t data_length);

#ifdef __cplusplus
}
#endif

#endif
