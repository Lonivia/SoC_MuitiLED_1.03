#include <stdint.h>
#include <string.h>
#include "nordic_common.h"
#include "nrf.h"
#include "nrf51_bitfields.h"
#include "ble_hci.h"
#include "ble_advdata.h"
#include "ble_conn_params.h"
#include "softdevice_handler.h"
#include "app_timer.h"
#include "app_button.h"
#include "ble_nus.h"

#include "ble_error_log.h"
#include "ble_debug_assert_handler.h"
#include "MDZ_Board.h"

#include "spi_master.h"
#include "ADS1248.h"
#include "nrf_delay.h"


uint32_t *ADS1248_SPI;



int main(void)
{
		nrf_delay_us(1000000);

    // Initialize
    leds_init();
    timers_init();
    buttons_init();
    ble_stack_init();
    gap_params_init();
    services_init();
    advertising_init();
    conn_params_init();
    sec_params_init();
		KEY_Init();
		//ADC_Init();
	  timer1_init();
		TIMER1_Enable();
	                  
		ADS1248_SPI=spi_master_init(SPI0, SPI_MODE1, 0);                 
		if(InitADS1248(ADS1248_SPI)){
			LED_OFF(LED_RED);
		}else{
			LED_OFF(LED_RED);
			while(1);
		}
		AD_Wakeup(ADS1248_SPI);
//		LED_ON(LED_GREEN);
		LED_ON(LED_RED);

		
		//LED_OFF(LED_GREEN);		
		//nrf_delay_us(1000000);
		
		

		nrf_gpio_cfg_output(A0);
		nrf_gpio_cfg_output(A1);
		nrf_gpio_cfg_output(A2);
		nrf_gpio_cfg_output(A3);
		nrf_gpio_cfg_output(A4);
		nrf_gpio_cfg_output(A5);
		nrf_gpio_pin_clear(A0);
		nrf_gpio_pin_clear(A1);
		nrf_gpio_pin_clear(A2);
		nrf_gpio_pin_clear(A3);
		nrf_gpio_pin_clear(A4);
		nrf_gpio_pin_clear(A5);
		nrf_gpio_cfg_output(ADGCE1);
		nrf_gpio_cfg_output(ADGCE2);
		nrf_gpio_pin_clear(ADGCE1);
		nrf_gpio_pin_clear(ADGCE2);  //both disable
		nrf_gpio_pin_set(ADGCE1);
		nrf_gpio_pin_set(ADGCE2);   //both enable
		
		DAC_init();
		clear();

timer1_init();
		TIMER1_Enable();
		

 
 
    for (;;)
    {
			MainWork();
    }
}

/** 
 * @}
 */
