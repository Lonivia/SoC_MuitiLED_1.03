#include <stdbool.h>
#include <stdint.h>
#include "nrf_delay.h"
#include "nrf_gpio.h"
#include "nrf.h"
#include "nrf_adc.h"
#include "MDZ_Board.h"


void nrf_adc_init(ADC_Res_t ADC_res, ADC_input_selection_t ADC_input_selection, ADC_interrupt_enabled_t ADC_interrupt_enabled)
{
    if(ADC_input_selection <= 7)
    {
			
			NRF_ADC->CONFIG = ADC_res << ADC_CONFIG_RES_Pos | ADC_CONFIG_INPSEL_AnalogInputOneThirdPrescaling << ADC_CONFIG_INPSEL_Pos |
                          ADC_CONFIG_REFSEL_VBG << ADC_CONFIG_REFSEL_Pos | (1 << ADC_input_selection) << ADC_CONFIG_PSEL_Pos;
 
			/*
        NRF_ADC->CONFIG = ADC_res << ADC_CONFIG_RES_Pos | ADC_CONFIG_INPSEL_SupplyOneThirdPrescaling << ADC_CONFIG_INPSEL_Pos |
                          ADC_CONFIG_REFSEL_SupplyOneHalfPrescaling << ADC_CONFIG_REFSEL_Pos | (1 << ADC_input_selection) << ADC_CONFIG_PSEL_Pos;
      */  
			
			NRF_ADC->ENABLE = 1;  
        if(ADC_interrupt_enabled)
        {
            NRF_ADC->INTENSET = ADC_INTENSET_END_Msk;
            NVIC_SetPriority(ADC_IRQn, 1);
            NVIC_EnableIRQ(ADC_IRQn);
        }
    }
}

void nrf_adc_init_vbat(void)
{
	
    NRF_ADC->CONFIG = ADC_CONFIG_RES_10bit << ADC_CONFIG_RES_Pos | ADC_CONFIG_INPSEL_SupplyOneThirdPrescaling << ADC_CONFIG_INPSEL_Pos |
                      ADC_CONFIG_REFSEL_VBG << ADC_CONFIG_REFSEL_Pos | ADC_CONFIG_PSEL_Disabled << ADC_CONFIG_PSEL_Pos;    
    
	//NRF_ADC->CONFIG = ADC_CONFIG_RES_10bit << ADC_CONFIG_RES_Pos | ADC_CONFIG_INPSEL_SupplyOneThirdPrescaling << ADC_CONFIG_INPSEL_Pos |
   //                   ADC_CONFIG_REFSEL_SupplyOneThirdPrescaling << ADC_CONFIG_REFSEL_Pos | ADC_CONFIG_PSEL_Disabled << ADC_CONFIG_PSEL_Pos;  
	NRF_ADC->ENABLE = 1;
}//useless

uint32_t nrf_adc_read(void)
{
  uint32_t adc_data;
  NRF_ADC->TASKS_START = 1;
  while(NRF_ADC->EVENTS_END == 0);
  NRF_ADC->EVENTS_END = 0;
  adc_data = NRF_ADC->RESULT;
  return adc_data;
}

float nrf_adc_read_vbat_f(void)
{
    return (float)nrf_adc_read() * 3.0f * 1.2f / 1024.0f;
}//useless

uint32_t nrf_adc_read_vbat_mv(void)
{
    // The non-optimized math is: (ADC / 1024) * 3 * 1.2 * 1000
    return nrf_adc_read() * (3 * 6 * 1000 / 5) / 1024;
}//useless

void ADC_Init(void){
	nrf_adc_init(ADC_RES_10bit, ADC_INPUT_AIN2_P01, ADC_INT_DISABLED);
	nrf_adc_init(ADC_RES_10bit, ADC_INPUT_AIN3_P02, ADC_INT_DISABLED);
	nrf_adc_init(ADC_RES_10bit, ADC_INPUT_AIN4_P03, ADC_INT_DISABLED);
}



uint16_t adc_read_bat(){
	nrf_adc_init(ADC_RES_10bit, ADC_INPUT_AIN2_P01, ADC_INT_DISABLED);
	return nrf_adc_read();
}

uint16_t adc_read_usb(){
	nrf_adc_init(ADC_RES_10bit, ADC_INPUT_AIN3_P02, ADC_INT_DISABLED);
	//return nrf_adc_read();
	return (uint16_t)nrf_adc_read() * 3.0f * 1.2f *1000/ 1024.0f;
}

uint16_t adc_read_LED(){
	nrf_adc_init(ADC_RES_10bit, ADC_INPUT_AIN4_P03, ADC_INT_DISABLED);
	//return nrf_adc_read();
	return (uint16_t)nrf_adc_read() * 3.0f * 1.2f *1000/ 1024.0f;
}



