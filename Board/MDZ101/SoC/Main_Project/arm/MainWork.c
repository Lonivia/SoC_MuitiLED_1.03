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
#include "BLEhandle.h"
#include "nrf_delay.h"
#include "ADS1248.h"
#include "MainWork.h"
#include "key.h"


int g_uiWdtCnt=0;

int noBat=0;
int noBattimeout=0;
uint16_t memo[8]={4050,4050,4050,4050,4050,4050,4050,4050};

void adg1(uint8_t adgp){
	if(adgp==1){
		LED_OFF(A0);
		LED_OFF(A1);
		LED_OFF(A2);
	}
	if(adgp==2){
		LED_ON(A0);
		LED_OFF(A1);
		LED_OFF(A2);
	}
	if(adgp==3){
		LED_OFF(A0);
		LED_ON(A1);
		LED_OFF(A2);
	}
	if(adgp==4){
		LED_ON(A0);
		LED_ON(A1);
		LED_OFF(A2);
	}
	if(adgp==5){
		LED_OFF(A0);
		LED_OFF(A1);
		LED_ON(A2);
	}
	if(adgp==6){
		LED_ON(A0);
		LED_OFF(A1);
		LED_ON(A2);
	}
	if(adgp==7){
		LED_OFF(A0);
		LED_ON(A1);
		LED_ON(A2);
	}
	if(adgp==8){
		LED_ON(A0);
		LED_ON(A1);
		LED_ON(A2);
	}
}
void adg2(uint8_t adgp){
	if(adgp==1){
		LED_OFF(A3);
		LED_OFF(A4);
		LED_OFF(A5);
	}
	if(adgp==2){
		LED_ON(A3);
		LED_OFF(A4);
		LED_OFF(A5);
	}
	if(adgp==3){
		LED_OFF(A3);
		LED_ON(A4);
		LED_OFF(A5);
	}
	if(adgp==4){
		LED_ON(A3);
		LED_ON(A4);
		LED_OFF(A5);
	}
	if(adgp==5){
		LED_OFF(A3);
		LED_OFF(A4);
		LED_ON(A5);
	}
	if(adgp==6){
		LED_ON(A3);
		LED_OFF(A4);
		LED_ON(A5);
	}
	if(adgp==7){
		LED_OFF(A3);
		LED_ON(A4);
		LED_ON(A5);
	}
	if(adgp==8){
		LED_ON(A3);
		LED_ON(A4);
		LED_ON(A5);
	}
}




void adg1off(){
	nrf_gpio_pin_clear(ADGCE1);
}

void adg2off(){
	nrf_gpio_pin_clear(ADGCE2);
}

void read1(uint8_t pin1){
	clear();
	AO_Output(pin1,memo[pin1-1]);
	adg1(pin1);
	adg2off();
	nrf_delay_us(1000000);
	LightGND(ADS1248_SPI,RxData);
	//uint32_t adc_data=adc_read_light();
	uint8_t bts_light[]={0x01,0x96,0x00,0x00,RxData[0]>>8&0xFF,RxData[0]&0xFF};
	while(ble_nus_send_string(&m_nus, bts_light, 6)){}
	clear();
}
void read2(uint8_t pin1,uint8_t pin2){
			clear();
	AO_Output(pin1,memo[pin1-1]+100);
	nrf_delay_us(100000);
	AO_Output(pin2,memo[pin2-1]+100);
	adg1(pin1);
	adg2(pin2);
			nrf_delay_us(1000000);
			Light(ADS1248_SPI,RxData);
			//uint32_t adc_data=adc_read_light();
			uint8_t bts_light[]={0x01,0x96,0x00,0x00,RxData[0]>>8&0xFF,RxData[0]&0xFF};
			while(ble_nus_send_string(&m_nus, bts_light, 6)){}
			clear();
}
void read1no(uint8_t pin1){
	clear();
			//nrf_gpio_pin_set(pin1);
	adg1(pin1);
	adg2off();
			nrf_delay_us(1000000);
			LightGND(ADS1248_SPI,RxData);
			//uint32_t adc_data=adc_read_light();
			uint8_t bts_light[]={0x01,0x96,0x00,0x00,RxData[0]>>8&0xFF,RxData[0]&0xFF};
			while(ble_nus_send_string(&m_nus, bts_light, 6)){}
			clear();
}
void read2no(uint8_t pin1,uint8_t pin2){
			clear();
	adg1(pin1);
	adg2(pin2);
			nrf_delay_us(1000000);
			Light(ADS1248_SPI,RxData);
			//uint32_t adc_data=adc_read_light();
			uint8_t bts_light[]={0x01,0x96,0x00,0x00,RxData[0]>>8&0xFF,RxData[0]&0xFF};
			while(ble_nus_send_string(&m_nus, bts_light, 6)){}
			clear();
}
void read2com(uint8_t pin1,uint8_t pin2){
			clear();
	if(pin1<pin2){
	adg1(pin1);
	adg2(pin2);}
	if(pin1>pin2){
	adg1(pin2);
	adg2(pin1);}
	AO_Output(pin1,memo[pin1-1]);
			nrf_delay_us(1000000);
			Light(ADS1248_SPI,RxData);
			//uint32_t adc_data=adc_read_light();
			uint8_t bts_light[]={0x01,0x96,0x00,0x00,RxData[0]>>8&0xFF,RxData[0]&0xFF};
			while(ble_nus_send_string(&m_nus, bts_light, 6)){}
			clear();
}
void read1d(uint8_t pin1,int8_t d){
	clear();
	AO_Output(pin1,memo[pin1-1]+d);
	adg1(pin1);
	adg2off();
	nrf_delay_us(1000000);
	LightGND(ADS1248_SPI,RxData);
	//uint32_t adc_data=adc_read_light();
	uint8_t bts_light[]={0x01,0x96,0x00,0x00,RxData[0]>>8&0xFF,RxData[0]&0xFF};
	while(ble_nus_send_string(&m_nus, bts_light, 6)){}
	clear();
}
void read2comd(uint8_t pin1,uint8_t pin2,int8_t d){
			clear();
	if(pin1<pin2){
	adg1(pin1);
	adg2(pin2);}
	if(pin1>pin2){
	adg1(pin2);
	adg2(pin1);}
	AO_Output(pin1,memo[pin1-1]+d);
			nrf_delay_us(1000000);
			Light(ADS1248_SPI,RxData);
			//uint32_t adc_data=adc_read_light();
			uint8_t bts_light[]={0x01,0x96,0x00,0x00,RxData[0]>>8&0xFF,RxData[0]&0xFF};
			while(ble_nus_send_string(&m_nus, bts_light, 6)){}
			clear();
}



void ClearWdt( void )
{
	g_uiWdtCnt = 0;
}

void MainWork(){		// 1ms
	static unsigned int uiLast1msTick = 0;
	if( timer_1ms_tick == uiLast1msTick )
	{
		return;
	}
	timer_1ms_tick=0;	
	ClearWdt();
	
	KEY1_Manger();	
	
	if(enableBLE){
		Key_Count=2;
	}
	
		if(startBLE==0 && (noBat>0 || (adc_read_usb()<=1729 && adc_read_usb()<=1729) )){
				Key_Count=0;
				enableBLE=0;
		if(noBat<=200){
//				LED_ON(LED_GREEN);
				LED_ON(LED_RED);
		}else{
//				LED_OFF(LED_GREEN);
				LED_OFF(LED_RED);
		}
		
		noBat++;
		noBattimeout++;
		if(noBat>=400){
			noBat=1;
		}
		if(noBattimeout>=2000){
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
				
	}else{
				Key_Count=2;
	}
	
	
	if(Key_Count==2 && enableBLE){
		Key_Count=0;
//		LED_OFF(LED_GREEN);
		
		
		//=======================================
				light=99;
		for(int k=1;k<=8;k++){
			for(int i=1000;i<=4050;i=i+5){
				AO_Output(k,i);
				nrf_delay_us(10000);
				if(adc_read_LED()>=900){
					memo[k-1]=i;
					break;
				}
			}
			clear();
			
	 }
		
		
		//=======================================
		light=0;
		
		
		advertising_start();
		power_manage();
		enableBLE=0;
		startBLE=1;
	}
	
	
	

	
	
	if(startBLE){
		
		

		if(light==1){
			for(int i=1;i<=8;i++){
				read1(i);
			}
			for(int i=1;i<=8;i++){
				for(int j=i+1;j<=8;j++){
					read2(i,j);
				}
			}
			light=0;
		}
		

		if(light==2){
			for(int i=1;i<=8;i++){
				read1no(i);
			}
			for(int i=1;i<=8;i++){
				for(int j=i+1;j<=8;j++){
					read2no(i,j);
				}
			}
			light=0;
		}
		
		
		if(light==3){
			/*
			for(int i=1;i<=8;i++){
				read1(i);
			}
			for(int i=1;i<=8;i++){
				for(int j=i+1;j<=8;j++){
					read2com(i,j);
					read2com(j,i);
				}
			}
			*/
			
			for(int8_t d=-100;d<=100;d=d+10){
				for(int i=1;i<=8;i++){
					read1d(i,d);
				}
				for(int i=1;i<=8;i++){
					for(int j=i+1;j<=8;j++){
						read2comd(i,j,d);
						read2comd(j,i,d);
					}
				}
			}
			light=0;
		}
		
		
		
		
		
		
	}
	
}

