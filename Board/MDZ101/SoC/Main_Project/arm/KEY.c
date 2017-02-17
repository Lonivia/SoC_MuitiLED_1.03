#include <stdbool.h>
#include <stdint.h>
#include "nrf_delay.h"
#include "nrf_gpio.h"
#include "nrf.h"
#include "nrf_adc.h"
#include "s110\nrf_soc.h"
#include "MDZ_Board.h"

#include "key.h"

#include "ADS1248.h"

void KEY_Init(void)
{	
  nrf_gpio_cfg_input(KEY_1,NRF_GPIO_PIN_PULLUP);
	
	nrf_gpio_cfg_sense_input(KEY_1, NRF_GPIO_PIN_PULLUP, NRF_GPIO_PIN_SENSE_LOW); //DETECT signal!!!!
	
	nrf_gpio_cfg_input(2,NRF_GPIO_PIN_PULLUP);

}
	
/*void Delay(uint32_t temp)
{
  for(; temp!= 0; temp--);
} */

void KEY1_Manger(void)		// 1ms
{			
	static unsigned int KeyCnt = 0;
	if( nrf_gpio_pin_read(KEY_1)== 0 ) 
	{	   
		KeyCnt++;
		if(KeyCnt>=2000)
		{
			LED_ON(LED_RED);
//			LED_ON(LED_GREEN);
			nrf_gpio_cfg_sense_input(WAKEUP_BUTTON_PIN,
															 BUTTON_PULL,
															 NRF_GPIO_PIN_SENSE_LOW);      

			AD_Sleep(ADS1248_SPI);
			nrf_delay_us(1000000);				
//			LED_OFF(LED_GREEN);
			LED_OFF(LED_RED);
			LED_OFF(ADGCE1);
			LED_OFF(ADGCE2);
			
			AO_Output(1,0);
			AO_Output(2,0);
			AO_Output(3,0);
			AO_Output(4,0);
			AO_Output(5,0);
			AO_Output(6,0);
			AO_Output(7,0);
			AO_Output(8,0);
			
			//nrf_delay_us(1000000);
			NVIC_DisableIRQ(TIMER1_IRQn);
			nrf_delay_us(1000000);
			uint32_t err_code = sd_power_system_off(); 
			APP_ERROR_CHECK(err_code);			
		}
	}
	else
	{
		KeyCnt = 0;
	}
}

