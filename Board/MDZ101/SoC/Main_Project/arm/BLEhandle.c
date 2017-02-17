#include <stdint.h>
#include <string.h>
#include "nrf_delay.h"
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
#include "nrf_adc.h"
#include "MDZ_Board.h"
#include "BLEhandle.h"
#include "spi_master.h"
#include "ADS1248.h"


int isUSB=0;
int isFULL=0;
ble_nus_t                        m_nus; 
uint8_t light=0;

uint8_t bts_test[]={0x12,0x34,0x56,0x78};

void clear(){
		AO_Output(1,0);
	AO_Output(2,0);
	AO_Output(3,0);
	AO_Output(4,0);
	AO_Output(5,0);
	AO_Output(6,0);
	AO_Output(7,0);
	AO_Output(8,0);

}

void BLE_data_handler(ble_nus_t * p_nus, uint8_t * p_data, uint16_t length){
	
if(p_data[0]==FLAG_CMD && p_data[1]==0x99)
	{
		clear();
		
		if(p_data[2]==0x01){
		AO_Output(6,1500);
			AO_Output(1,3000);
		}
		
		if(p_data[2]==0x02)
		AO_Output(3,3100);
		
		if(p_data[2]==0x03)
		AO_Output(3,3400);
		
		if(p_data[2]==0x04)
		AO_Output(3,3700);
		
		if(p_data[2]==0x05)
		AO_Output(3,4000);
	
		
		//nrf_delay_us(1000000);
		//uint32_t adc_data=adc_read_light();
		uint16_t aaa=adc_read_LED();
		
		uint8_t bts_light[]={0x01,0x98,0x00,0x00,(int)aaa/ 1000,(int ) aaa/ 100 % 10,(int)aaa/ 10 % 10 , (int) aaa % 10};
		
//uint8_t bts_light[]={0x01,0x98,0x00,0x00,aaa>>8&0xFF,aaa&0xFF};
while(ble_nus_send_string(&m_nus, bts_light, 8)){}
	
	uint8_t bts_memo[]={0x01,0x98,0x00,0x00,(int)memo[0]/ 1000,(int ) memo[0]/ 100 % 10,(int)memo[0]/ 10 % 10 , (int) memo[0] % 10};
	while(ble_nus_send_string(&m_nus, bts_memo, 8)){}
		
		uint8_t bts_memo1[]={0x01,0x98,0x00,0x00,(int)memo[1]/ 1000,(int ) memo[1]/ 100 % 10,(int)memo[1]/ 10 % 10 , (int) memo[1] % 10};
	while(ble_nus_send_string(&m_nus, bts_memo1, 8)){}
		
		uint8_t bts_memo2[]={0x01,0x98,0x00,0x00,(int)memo[2]/ 1000,(int ) memo[2]/ 100 % 10,(int)memo[2]/ 10 % 10 , (int) memo[2] % 10};
	while(ble_nus_send_string(&m_nus, bts_memo2, 8)){}
		
		uint8_t bts_memo3[]={0x01,0x98,0x00,0x00,(int)memo[3]/ 1000,(int ) memo[3]/ 100 % 10,(int)memo[3]/ 10 % 10 , (int) memo[3] % 10};
	while(ble_nus_send_string(&m_nus, bts_memo3, 8)){}
		
		uint8_t bts_memo4[]={0x01,0x98,0x00,0x00,(int)memo[4]/ 1000,(int ) memo[4]/ 100 % 10,(int)memo[4]/ 10 % 10 , (int) memo[4] % 10};
	while(ble_nus_send_string(&m_nus, bts_memo4, 8)){}
		
		uint8_t bts_memo5[]={0x01,0x98,0x00,0x00,(int)memo[5]/ 1000,(int ) memo[5]/ 100 % 10,(int)memo[5]/ 10 % 10 , (int) memo[5] % 10};
	while(ble_nus_send_string(&m_nus, bts_memo5, 8)){}
		
		uint8_t bts_memo6[]={0x01,0x98,0x00,0x00,(int)memo[6]/ 1000,(int ) memo[6]/ 100 % 10,(int)memo[6]/ 10 % 10 , (int) memo[6] % 10};
	while(ble_nus_send_string(&m_nus, bts_memo6, 8)){}
		
		uint8_t bts_memo7[]={0x01,0x98,0x00,0x00,(int)memo[7]/ 1000,(int ) memo[7]/ 100 % 10,(int)memo[7]/ 10 % 10 , (int) memo[7] % 10};
	while(ble_nus_send_string(&m_nus, bts_memo7, 8)){}
		
		
		

	}
	
	if(p_data[0]==FLAG_CMD && p_data[1]==0x97){
		clear();
		light=1;
			
	}
	if(p_data[0]==FLAG_CMD && p_data[1]==0x57){
		clear();
		light=2;
			
	}
	if(p_data[0]==FLAG_CMD && p_data[1]==0x58){
		clear();
		light=3;
			
	}


		
	


if(p_data[0]==FLAG_CMD && p_data[1]==0x87){
	LED_OFF(A0);
	LED_OFF(A1);
	LED_OFF(A2);
	LED_ON(A3);
	LED_OFF(A4);
	LED_OFF(A5);
	
	uint8_t bts_lightx[]={0x01,0x86,0x00,0x01};
			while(ble_nus_send_string(&m_nus, bts_lightx, 4)){}
	
		unsigned short RxData[5]={0x1234,0x5678,0x9000,0xABCD,0xCDEF};
		Light(ADS1248_SPI,RxData);
			//uint32_t adc_data=adc_read_light();
			uint8_t bts_light[]={0x01,0x86,0x00,0x00,RxData[0]>>8&0xFF,RxData[0]&0xFF};
			while(ble_nus_send_string(&m_nus, bts_light, 6)){}
				
				uint8_t bts_lighty[]={0x01,0x86,0x00,0x02};
			while(ble_nus_send_string(&m_nus, bts_lighty, 4)){}
}
		



		//Read bat and usb
		if(p_data[0]==FLAG_CMD && p_data[1]==CMD_READ_BAT){  //0x51 and resp 0x52
			
			/*
			uint32_t adc_data=adc_read_bat();
			uint8_t BAT=0;
			if((uint8_t)adc_data>=0xC8){
				BAT=100;
			}else if((uint8_t)adc_data>0xC6 && (uint8_t)adc_data<0xC8){
				BAT=90;
			}else if((uint8_t)adc_data>0xC4 && (uint8_t)adc_data<=0xC6){
				BAT=80;
			}else if((uint8_t)adc_data>0xC2 && (uint8_t)adc_data<=0xC4){
				BAT=70;
			}else if((uint8_t)adc_data>0xC0 && (uint8_t)adc_data<=0xC2){
				BAT=60;
			}else if((uint8_t)adc_data>0xBB && (uint8_t)adc_data<=0xC0){
				BAT=50;
			}else if((uint8_t)adc_data>0xB6 && (uint8_t)adc_data<=0xBB){
				BAT=40;
			}else if((uint8_t)adc_data>0xB0 && (uint8_t)adc_data<=0xB6){
				BAT=30;
			}else if((uint8_t)adc_data>0xB6 && (uint8_t)adc_data<=0xB4){
				BAT=20;
			}else if((uint8_t)adc_data>0xB4 && (uint8_t)adc_data<=0xAF){
				BAT=10;
			}else if((uint8_t)adc_data<=0xAF){
				BAT=0;
			}
			uint32_t adc_data2=adc_read_usb();
			//if((char)adc_data>0xCC)
			
			if( (uint8_t)adc_data2>=0xDD || (uint8_t)adc_data2<=0x04) 
	    {	
				isUSB=1;
	  	}else{
				isUSB=0;
			}
			if(isUSB==1 && BAT==100 && nrf_gpio_pin_read(2)== 1){
				isFULL=1;
			}else{
				isFULL=0;
			}
			uint8_t bts_bat[]={0x01,CMD_READ_BAT_RESPOND,(uint8_t)adc_data2,(uint8_t)adc_data,BAT,isUSB,isFULL};
			ble_nus_send_string(&m_nus, bts_bat, 7);
				
			//adc_data=adc_read_usb();
			//uint8_t bts_usb[]={0x01,CMD_READ_BAT_RESPOND,(char)(adc_data>>24),(char)(adc_data>>16),(char)(adc_data>>8),(char)adc_data};
			//while(ble_nus_send_string(&m_nus, bts_usb, 6)){}
		*/
		}
		
		//Read SN and Ver
		if(p_data[0]==FLAG_CMD && p_data[1]==CMD_SN_VER){
			uint8_t bts_sn[]={0x01,0x34,0x00,0x00,0x96,0x00};
			while(ble_nus_send_string(&m_nus, bts_sn, 6)){}
			uint8_t bts_ver[]={0x00,0x34,0x00,0x00,0x00,0x21};
			while(ble_nus_send_string(&m_nus, bts_ver, 6)){}
		}
		
		//Handshake
		if(p_data[0]==FLAG_CMD && p_data[1]==CMD_HANDSHAKE){
			uint8_t bts_handshake[]={0x01,CMD_HANDSHAKE_RESPOND,0x00,0x00,p_data[4],p_data[5]};
			while(ble_nus_send_string(&m_nus, bts_handshake, 6)){}
		}

}



