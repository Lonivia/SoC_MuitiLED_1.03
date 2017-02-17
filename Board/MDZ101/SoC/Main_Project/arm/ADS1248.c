#include "ADS1248.h"
#include "nrf_gpio.h"
#include "common.h"
#include "key.h"
#include "spi_master.h"
#include "nrf_delay.h"
#include "MDZ_Board.h"
#include "spi_master_config.h" 

/***********向AD中写指令***************/

void ADS1248WCMD(uint32_t *spi_base_address,uint8_t cmd){

	 uint8_t tx_cmd[]={cmd};
	
   uint8_t temp[]={0};
	
   spi_master_tx_rx(spi_base_address,1,tx_cmd,temp);

}


/************向AD中的寄存器中写入配置值************************/

bool ADS1248WREG(uint32_t *spi_base_address,unsigned char regaddr,uint8_t databyte)
{
	  uint8_t temp[]={0};
		
	  uint8_t reg[1]={ADS1248_CMD_WREG+(regaddr & 0x0F)};
    //??????????
		//while(nrf_gpio_pin_read(AD_CS)==0);
    spi_master_tx_rx(spi_base_address,1,reg,temp);
		// nrf_delay_us(5);
		 reg[0]=0x00;
     spi_master_tx_rx(spi_base_address,1,reg,temp);
		// nrf_delay_us(5);
		 reg[0]=databyte;
		// nrf_delay_us(5);
   return spi_master_tx_rx(spi_base_address,1,reg,temp);
		//  nrf_delay_us(4);
}

/******************读取AD中寄存器的值***********************/

uint8_t ADS1248RREG(uint32_t *spi_base_address,unsigned char regaddr)
{
	   uint8_t rx1[1]={0};
		 uint8_t rx2[1]={0};
		 uint8_t tx[1]={ADS1248_CMD_RREG+(regaddr & 0x0F)};
    
		 //ADS1248WCMD(spi_base_address,(ADS1248_CMD_RREG+(regaddr&0x0F)));
		// while(nrf_gpio_pin_read(AD_CS));
		 spi_master_tx_rx(spi_base_address,1,tx,rx1);
		 //nrf_delay_us(5);
		 tx[0]=0x00;
		 
		 spi_master_tx_rx(spi_base_address,1,tx,rx1);
		 //nrf_delay_us(5);
		 tx[0]=ADS1248_CMD_NOP;
		 spi_master_tx_rx(spi_base_address,1,tx,rx2);
		 //nrf_delay_us(5);
	    return rx2[0];//????
}

/************在AD的SPI总线上读取数据************************/

unsigned short ADS1248RRDATA(uint32_t *spi_base_address)
{
	nrf_gpio_pin_clear(AD_CS);
	nrf_delay_us(1);
	
	  uint8_t tx_data1[1]={ADS1248_CMD_RDATA};
		uint8_t tx_data2[3]={ADS1248_CMD_NOP,ADS1248_CMD_NOP,ADS1248_CMD_NOP};
		uint8_t rx_data1[3];		// ???
		uint8_t rx_data2[3];
		unsigned short result;
		
			ADS_Start1;
			nrf_delay_us(1); //20
			ADS_Start0;
		
		while(nrf_gpio_pin_read(AD_DRDY));
		
		
		//nrf_delay_us(200000); //!!!!!!!!!
		
	  spi_master_tx_rx(spi_base_address,1,tx_data1,rx_data1);	

		
		spi_master_tx_rx(spi_base_address,3,tx_data2,rx_data2);	

	  result=(rx_data2[1]) | (rx_data2[0])<<8;
		 
		spi_master_tx_rx(spi_base_address,1,tx_data1,rx_data1);	  //????DRDY
		
	  nrf_gpio_pin_set(AD_CS);
		
	  return result;
		
		
		
}
/*****************初始化ADS1248*********************/

bool InitADS1248(uint32_t *spi_base_address)
{
  //uint32_t *spi_base_address=spi_master_init(SPI0, SPI_MODE0, 1);
	

	
	nrf_gpio_cfg_output(AD_CS);
	nrf_gpio_cfg_output(AD_START);
	nrf_gpio_cfg_output(AD_RST);
	
	
	
		nrf_gpio_cfg_input(AD_DRDY, NRF_GPIO_PIN_PULLDOWN);
    nrf_gpio_cfg_output(AD_START);   //Start
    nrf_gpio_cfg_input(AD_DRDY, NRF_GPIO_PIN_PULLDOWN);
    nrf_gpio_cfg_output(AD_RST);    //RESET
  
    ADS_Start0;
	  nrf_gpio_pin_set(SPI_PSELSS0);
    ADS_RESET0;
    nrf_delay_us(100);
    ADS_RESET1;
    nrf_delay_us(1000);
		
		
/*
    ADS1248WREG(spi_base_address,ADS1248_REG_MUX0,0x1A); //00aaabbb +\-

    ADS1248WREG(spi_base_address,ADS1248_REG_MUX1,0x38); //
	
    ADS1248WREG(spi_base_address,ADS1248_REG_VBIAS,0x00); //00000000,Bit7-0:????????(??)

    ADS1248WREG(spi_base_address,ADS1248_REG_IDAC1,0xFF);//!!!!!!!!!!!!!!!!!!!! 

    ADS1248WREG(spi_base_address,ADS1248_REG_IDAC0,0x07); //Imax

    return ADS1248WREG(spi_base_address,ADS1248_REG_SYS0,0x07); // 0-7 for 128-1,0-8 spi speed
		*/
		
		ADS1248WREG(spi_base_address,ADS1248_REG_MUX0,0x1A); //00000001,A0???,A1????Bit7-6:???????????,Bit5-3:????AIN0,Bit2-0:????AIN1

    ADS1248WREG(spi_base_address,ADS1248_REG_MUX1,0x38); //00110000,Bit7:??????,Bit6-5:????????,Bit4-3:?????????,Bit2-0:????
	
    ADS1248WREG(spi_base_address,ADS1248_REG_VBIAS,0x00); //00000000,Bit7-0:????????(??)

    ADS1248WREG(spi_base_address,ADS1248_REG_IDAC1,0x23);//11111111,Bit7-4:????????????,Bit3-0:????????????  0X8F?????????

    ADS1248WREG(spi_base_address,ADS1248_REG_IDAC0,0x07); //00001000,Bit7-4:??,Bit3:DRDY/DOUT????Dout? ,Bit2-0:???????

    ADS1248WREG(spi_base_address,ADS1248_REG_SYS0,0x07); //00000000,Bit7:?????0,Bit6-41:??1???,Bit3-0:??2000SPS????
		
		ADS1248WREG(spi_base_address,ADS1248_REG_MUX0,0x01);  //AIN0??,AIN1??
	return ADS1248WREG(spi_base_address,ADS1248_REG_IDAC1,0xFF);
}


void LightGND(uint32_t *spi_base_address,unsigned short *ReadData){

	unsigned short Data[1];
	
  ADS_Start1;
	//ADS1248WREG(spi_base_address,ADS1248_REG_MUX0,0x01);  //AIN0??,AIN1??
	ADS1248WREG(spi_base_address,ADS1248_REG_MUX0,0x13); //ANNP TO GND!!
	//ADS1248WREG(spi_base_address,ADS1248_REG_IDAC1,0x23);  
  ADS_Start0;
	//nrf_delay_us(2);
	Data[0]=ADS1248RRDATA(spi_base_address);
	
	/*
	Data[1]=ADS1248RRDATA(spi_base_address);
	Data[2]=ADS1248RRDATA(spi_base_address);
	Data[3]=ADS1248RRDATA(spi_base_address);
	Data[4]=ADS1248RRDATA(spi_base_address);
	Data[5]=ADS1248RRDATA(spi_base_address);
	Data[6]=ADS1248RRDATA(spi_base_address);
	Data[7]=ADS1248RRDATA(spi_base_address);
	Data[8]=ADS1248RRDATA(spi_base_address);
	Data[9]=ADS1248RRDATA(spi_base_address);
	Data[10]=ADS1248RRDATA(spi_base_address);
	Data[11]=ADS1248RRDATA(spi_base_address);
	Data[12]=ADS1248RRDATA(spi_base_address);
	Data[13]=ADS1248RRDATA(spi_base_address);
	Data[14]=ADS1248RRDATA(spi_base_address);
	Data[15]=ADS1248RRDATA(spi_base_address);
	Data[16]=ADS1248RRDATA(spi_base_address);
	Data[17]=ADS1248RRDATA(spi_base_address);
	Data[18]=ADS1248RRDATA(spi_base_address);
	Data[19]=ADS1248RRDATA(spi_base_address);
	
	
	ReadData[0]=Data[0]/20+Data[1]/20+Data[2]/20+Data[3]/20+Data[4]/20+
							Data[5]/20+Data[6]/20+Data[7]/20+Data[8]/20+Data[9]/20+
							Data[10]/20+Data[11]/20+Data[12]/20+Data[13]/20+Data[14]/20+
							Data[15]/20+Data[16]/20+Data[17]/20+Data[18]/20+Data[19]/20;
							
						*/
						ReadData[0]=Data[0];
}


void Light(uint32_t *spi_base_address,unsigned short *ReadData){

	unsigned short Data[1];
	
  ADS_Start1;
	ADS1248WREG(spi_base_address,ADS1248_REG_MUX0,0x01);  //AIN0??,AIN1??
	//ADS1248WREG(spi_base_address,ADS1248_REG_MUX0,0x13); //ANNP TO GND!!
	//ADS1248WREG(spi_base_address,ADS1248_REG_IDAC1,0x23);  
  ADS_Start0;
	//nrf_delay_us(2);
	Data[0]=ADS1248RRDATA(spi_base_address);
	
	/*
	Data[1]=ADS1248RRDATA(spi_base_address);
	Data[2]=ADS1248RRDATA(spi_base_address);
	Data[3]=ADS1248RRDATA(spi_base_address);
	Data[4]=ADS1248RRDATA(spi_base_address);
	Data[5]=ADS1248RRDATA(spi_base_address);
	Data[6]=ADS1248RRDATA(spi_base_address);
	Data[7]=ADS1248RRDATA(spi_base_address);
	Data[8]=ADS1248RRDATA(spi_base_address);
	Data[9]=ADS1248RRDATA(spi_base_address);
	Data[10]=ADS1248RRDATA(spi_base_address);
	Data[11]=ADS1248RRDATA(spi_base_address);
	Data[12]=ADS1248RRDATA(spi_base_address);
	Data[13]=ADS1248RRDATA(spi_base_address);
	Data[14]=ADS1248RRDATA(spi_base_address);
	Data[15]=ADS1248RRDATA(spi_base_address);
	Data[16]=ADS1248RRDATA(spi_base_address);
	Data[17]=ADS1248RRDATA(spi_base_address);
	Data[18]=ADS1248RRDATA(spi_base_address);
	Data[19]=ADS1248RRDATA(spi_base_address);
	
	
	ReadData[0]=Data[0]/20+Data[1]/20+Data[2]/20+Data[3]/20+Data[4]/20+
							Data[5]/20+Data[6]/20+Data[7]/20+Data[8]/20+Data[9]/20+
							Data[10]/20+Data[11]/20+Data[12]/20+Data[13]/20+Data[14]/20+
							Data[15]/20+Data[16]/20+Data[17]/20+Data[18]/20+Data[19]/20;
							
						*/
						ReadData[0]=Data[0];
}





/*****************读取ADS1248转换后的值，取24位中的高两个字节*************************/
/*
void ADS(uint32_t *spi_base_address,unsigned short *ReadData)
{

  unsigned short Data[5];
	
  ADS_Start1;
	ADS1248WREG(spi_base_address,ADS1248_REG_MUX0,0x01);  //AIN0??,AIN1??
	ADS1248WREG(spi_base_address,ADS1248_REG_IDAC1,0x01);
  ADS_Start0;
	//nrf_delay_us(2);
	Data[0]=ADS1248RRDATA(spi_base_address)+0x8000;
	//Data[0]=((uint32_t)ADS1248RRDATA(spi_base_address)+(uint32_t)ADS1248RRDATA(spi_base_address)+(uint32_t)ADS1248RRDATA(spi_base_address))/3;
	//nrf_delay_us(2);

			//???
	
	
	ADS_Start1;
	ADS1248WREG(spi_base_address,ADS1248_REG_MUX0,0x13);  //AIN2??,AIN3??
	ADS1248WREG(spi_base_address,ADS1248_REG_IDAC1,0x23);
	ADS_Start0;
	//nrf_delay_us(5);
	Data[1]=ADS1248RRDATA(spi_base_address)+0x8000;
	

	ADS_Start1;
	ADS1248WREG(spi_base_address,ADS1248_REG_MUX0,0x25);  //AIN4??,AIN5??
	ADS1248WREG(spi_base_address,ADS1248_REG_IDAC1,0x45);
  ADS_Start0;
	//nrf_delay_us(5);
	Data[2]=ADS1248RRDATA(spi_base_address)+0x8000;
	
	
	
	
	ADS_Start1;
	ADS1248WREG(spi_base_address,ADS1248_REG_MUX0,0x37);  //AIN6??,AIN7??
	ADS1248WREG(spi_base_address,ADS1248_REG_IDAC1,0x67);
	ADS_Start0;
	nrf_gpio_pin_write(17, 1);  //NO1,NO2??
	//nrf_delay_us(5);
	Data[3]=ADS1248RRDATA(spi_base_address)+0x8000;
	
	
	//ADS1248WREG(ADS1248_REG_MUX0,0x37);  //AIN6??,AIN7??
	nrf_gpio_pin_write(17, 0);  //NC1,NC2??
	//nrf_delay_us(5);
  Data[4]=ADS1248RRDATA(spi_base_address)+0x8000;
	//nrf_delay_us(5);
	
	
	ADS1248WREG(spi_base_address,ADS1248_REG_IDAC1,0x11);

	
	
	
	
//	ReadData=Data;

  for(int i=0;i<5;i++){
	
		ReadData[i]=Data[i];
	
	}


		
}

*/

void AD_Sleep(uint32_t *spi_base_address){

	ADS1248WCMD(spi_base_address,0x02);

}

void AD_Wakeup(uint32_t *spi_base_address){

	ADS1248WCMD(spi_base_address,0x01);
	
}



	         
