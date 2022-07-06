#ifndef _AHT10_H_
#define _AHT10_H_

#include <stdbool.h>
#include "main.h"

#define AHT10_ADRESS (0x38 << 1) // 0b1110000; Adress[7-bit]Wite/Read[1-bit]

typedef struct {
	float Temperatur;
	float Humidity;
} AHT10_DATA;


bool AHT10_Process(void);
bool AHT10_Get(AHT10_DATA *data);
 
#endif
