#include "nrf_delay.h"
#include "nrf_gpio.h"
#include "MDZ_Board.h"

#define  SetDAC_CE1()		nrf_gpio_pin_set(DAC_CE1)
#define  SetDAC_CE2()		nrf_gpio_pin_set(DAC_CE2)
#define  SetDAC_CE3()		nrf_gpio_pin_set(DAC_CE3)
#define  SetDAC_CE4()		nrf_gpio_pin_set(DAC_CE4)
#define  SetDAC_CE5()		nrf_gpio_pin_set(DAC_CE5)
#define  SetDAC_CE6()		nrf_gpio_pin_set(DAC_CE6)
#define  SetDAC_CE7()		nrf_gpio_pin_set(DAC_CE7)
#define  SetDAC_CE8()		nrf_gpio_pin_set(DAC_CE8)

#define  ResetDAC_CE1()		nrf_gpio_pin_clear(DAC_CE1)
#define  ResetDAC_CE2()		nrf_gpio_pin_clear(DAC_CE2)
#define  ResetDAC_CE3()		nrf_gpio_pin_clear(DAC_CE3)
#define  ResetDAC_CE4()		nrf_gpio_pin_clear(DAC_CE4)
#define  ResetDAC_CE5()		nrf_gpio_pin_clear(DAC_CE5)
#define  ResetDAC_CE6()		nrf_gpio_pin_clear(DAC_CE6)
#define  ResetDAC_CE7()		nrf_gpio_pin_clear(DAC_CE7)
#define  ResetDAC_CE8()		nrf_gpio_pin_clear(DAC_CE8)


//********************************************************
#define  SetDAC_SCLK()		nrf_gpio_pin_set(DAC_SCLK)
#define  ResetDAC_SCLK()	nrf_gpio_pin_clear(DAC_SCLK)

#define  SetDAC_Din()		nrf_gpio_pin_set(DAC_Din)
#define  ResetDAC_Din()		nrf_gpio_pin_clear(DAC_Din)


void DAC_init(){
	nrf_gpio_cfg_output(DAC_CE1);
	nrf_gpio_cfg_output(DAC_CE2);
	nrf_gpio_cfg_output(DAC_CE3);
	nrf_gpio_cfg_output(DAC_CE4);
	nrf_gpio_cfg_output(DAC_CE5);
	nrf_gpio_cfg_output(DAC_CE6);
	nrf_gpio_cfg_output(DAC_CE7);
	nrf_gpio_cfg_output(DAC_CE8);
	
	nrf_gpio_cfg_output(DAC_SCLK);
	nrf_gpio_cfg_output(DAC_Din);
	
	SetDAC_CE1();
	SetDAC_CE2();
	SetDAC_CE3();
	SetDAC_CE4();
	SetDAC_CE5();
	SetDAC_CE6();
	SetDAC_CE7();
	SetDAC_CE8();
	
	SetDAC_SCLK();
	SetDAC_Din();
	
}


void AO_Output( unsigned char ch, unsigned short value )  	// ch:1~4; value:0~4095
{
	unsigned char i;
	unsigned int curr;
	
	if(value>=4094)
		value=4094;

	ResetDAC_SCLK();
	SetDAC_CE1();
	SetDAC_CE2();
	SetDAC_CE3();
	SetDAC_CE4();
	SetDAC_CE5();
	SetDAC_CE6();
	SetDAC_CE7();
	SetDAC_CE8();

	if( ch == 1 )
	{
		ResetDAC_CE1();
	}
	else if( ch == 2 )
	{
		ResetDAC_CE2();
	}
	else if( ch == 3 )
	{
		ResetDAC_CE3();
	}
	else if( ch == 4 )
	{
		ResetDAC_CE4();
	}
	else if( ch == 5 )
	{
		ResetDAC_CE5();
	}
	else if( ch == 6 )
	{
		ResetDAC_CE6();
	}
	else if( ch == 7 )
	{
		ResetDAC_CE7();
	}
	else if( ch == 8 )
	{
		ResetDAC_CE8();
	}
	else
	{
		return;
	}
	nrf_delay_us(5);

	for( i=0; i<16; i++ )
	{
		SetDAC_SCLK();
		nrf_delay_us(5);
		if( (value&(0x8000>>i))>>(15-i) )
		{
			SetDAC_Din();
		}
		else
		{
			ResetDAC_Din();
		}
		nrf_delay_us(5);
		ResetDAC_SCLK();	// ?????
		nrf_delay_us(5);
	}
	SetDAC_CE1();
	SetDAC_CE2();
	SetDAC_CE3();
	SetDAC_CE4();
	SetDAC_CE5();
	SetDAC_CE6();
	SetDAC_CE7();
	SetDAC_CE8();

	curr = value;
	curr = curr*5000/4096;


}



