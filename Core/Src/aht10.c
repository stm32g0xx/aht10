#include "AHT10.h"

/* Variables for AHT10 */
uint8_t AHT10_RX_Data[6];
uint32_t AHT10_ADC_Raw;
float AHT10_Temperature;
float AHT10_Humidity;
uint8_t AHT10_TmpHum_Cmd[3] = {0xAC, 0x33, 0x00};

/* Additional vars */
uint8_t AHT10_Switcher = 255;

extern I2C_HandleTypeDef hi2c1;

bool AHT10_Process(void)
{
	if (AHT10_Switcher)
	{
		/* Send command (trigger measuremetns) + parameters */
		HAL_I2C_Master_Transmit_IT(&hi2c1, AHT10_ADRESS, (uint8_t*)AHT10_TmpHum_Cmd, 3);
	}
	else
	{
		/* Receive data: STATUS[1]:HIMIDITY[2.5]:TEMPERATURE[2.5] */
		HAL_I2C_Master_Receive_IT(&hi2c1, AHT10_ADRESS, (uint8_t*)AHT10_RX_Data, 6);
	}

	if (~AHT10_RX_Data[0] & 0x80)
	{
		/* Convert to Temperature in °C */
		AHT10_ADC_Raw = (((uint32_t)AHT10_RX_Data[3] & 15) << 16) | ((uint32_t)AHT10_RX_Data[4] << 8) | AHT10_RX_Data[5];
		AHT10_Temperature = (float)(AHT10_ADC_Raw * 200.00 / 1048576.00) - 50.00;

		/* Convert to Relative Humidity in % */
		AHT10_ADC_Raw = ((uint32_t)AHT10_RX_Data[1] << 12) | ((uint32_t)AHT10_RX_Data[2] << 4) | (AHT10_RX_Data[3] >> 4);
		AHT10_Humidity = (float)(AHT10_ADC_Raw*100.00/1048576.00);
	}

	AHT10_Switcher = ~AHT10_Switcher; /* Invert */

	return true;
}

bool AHT10_Get(AHT10_DATA *data)
{
	data->Temperatur = AHT10_Temperature;
	data->Humidity = AHT10_Humidity;

	return true;
}
