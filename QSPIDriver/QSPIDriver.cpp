#include <stm32f4xx_hal.h>
#include <stm32_hal_legacy.h>

#include "FLASHPluginConfig.h"
#include "FLASHPluginInterface.h"

#ifdef __cplusplus
extern "C"
#endif
void SysTick_Handler(void)
{
	HAL_IncTick();
	HAL_SYSTICK_IRQHandler();
}

//int main(void)
//{
//	HAL_Init();
//
//	__GPIOG_CLK_ENABLE();
//	GPIO_InitTypeDef GPIO_InitStructure;
//
//	GPIO_InitStructure.Pin = GPIO_PIN_6;
//
//	GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
//	GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_HIGH;
//	GPIO_InitStructure.Pull = GPIO_NOPULL;
//	HAL_GPIO_Init(GPIOG, &GPIO_InitStructure);
//
//	for (;;)
//	{
//		HAL_GPIO_WritePin(GPIOG, GPIO_PIN_6, GPIO_PIN_SET);
//		HAL_Delay(500);
//		HAL_GPIO_WritePin(GPIOG, GPIO_PIN_6, GPIO_PIN_RESET);
//		HAL_Delay(500);
//	}
//}



FLASHBankInfo FLASHPlugin_Probe(unsigned base, unsigned size, unsigned width1, unsigned width2)
{
	InterruptEnabler enabler;
    
	FLASHBankInfo result = {
		.BaseAddress = base, 
		.BlockCount = N25Q128A_FLASH_SIZE / N25Q128A_SUBSECTOR_SIZE, 
		.BlockSize = N25Q128A_SUBSECTOR_SIZE,
		.WriteBlockSize = MINIMUM_PROGRAMMED_BLOCK_SIZE
	};
	return result;
}

WorkAreaInfo FLASHPlugin_FindWorkArea(void *endOfStack)
{
	InterruptEnabler enabler;
    
	WorkAreaInfo info = { .Address = endOfStack, .Size = 4096 };
	return info;
}

int FLASHPlugin_EraseSectors(unsigned firstSector, unsigned sectorCount)
{
	InterruptEnabler enabler;
    int initialTick = HAL_GetTick();
    int timeout = 5000;
    
	for (unsigned i = 0; i < sectorCount; i++)
	{
    	if ((HAL_GetTick() - initialTick) > timeout)
        	return i;   //OpenOCD will continue the erase operation

    	uint8_t error = BSP_QSPI_Erase_Block((firstSector + i) * N25Q128A_SUBSECTOR_SIZE);
		if (error != QSPI_OK)
			return -1;
	}
	return sectorCount;
}

int FLASHPlugin_Unload()
{
	BSP_QSPI_DeInit();
	HAL_DeInit();
	SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;
    
	for (int i = 0; i < sizeof(NVIC->ICER) / sizeof(NVIC->ICER[0]); i++)
		NVIC->ICER[0] = -1;
    
	return 0;
}

int FLASHPlugin_DoProgramSync(unsigned startOffset, const void *pData, int bytesToWrite)
{
	uint8_t result = BSP_QSPI_Write((uint8_t *)pData, startOffset, bytesToWrite);
	if (result != QSPI_OK)
		return 0;
	return bytesToWrite;
}


int main(void)
{
	extern void *g_pfnVectors;
    
	SCB->VTOR = (uint32_t)&g_pfnVectors;
	HAL_Init();
	BSP_QSPI_Init();
	FLASHPlugin_InitDone();
    
#ifdef DEBUG
    TestFLASHProgramming(0x90000000, 0);
#endif

	for (;;)
		;
}
