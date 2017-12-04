#include<p18f452.h>
#include<delays.h>
#include <stdio.h>
#include <stdlib.h>
#include"xlcd.h"


#pragma config  OSC=HS
#pragma config  LVP=OFF
#pragma config  WDT=OFF

#define bitmask_data 1

#define DATA_0_TRIS TRISAbits.RA0
#define DATA_1_TRIS TRISAbits.RA1  
#define DATA_2_TRIS TRISAbits.RA2  
#define DATA_3_TRIS TRISAbits.RA3  
#define DATA_4_TRIS TRISDbits.RD3
#define DATA_5_TRIS TRISAbits.RA5  
#define DATA_6_TRIS TRISEbits.RE0
#define DATA_7_TRIS TRISEbits.RE1  

#define DATA_0 LATAbits.LATA0
#define DATA_1 LATAbits.LATA1  
#define DATA_2 LATAbits.LATA2  
#define DATA_3 LATAbits.LATA3  
#define DATA_4 LATDbits.LATD3
#define DATA_5 LATAbits.LATA5  
#define DATA_6 LATEbits.LATE0
#define DATA_7 LATEbits.LATE1 



#define WE_FLASH_TRIS TRISEbits.RE2
#define OE_FLASH_TRIS TRISAbits.RA6
#define CE_FLASH_TRIS TRISCbits.RC0

#define WE_FLASH LATEbits.LATE2
#define OE_FLASH LATAbits.LATA6
#define CE_FLASH LATCbits.LATC0



#define SER_SR_TRIS TRISCbits.RC3
#define RCK_SR_TRIS TRISBbits.RB2
#define SRCK_SR_TRIS TRISBbits.RB0

#define SER_SR LATCbits.LATC3
#define RCK_SR LATBbits.LATB2
#define SRCK_SR LATBbits.LATB0

void DelayFor18TCY( void )
{

    Delay1TCY();
    Delay1TCY();
    Delay1TCY();
    Delay1TCY();
    Delay10TCYx(1);
}

void DelayPORXLCD (void)
{
 Delay10KTCYx(6);
}

void DelayXLCD (void)
{
 Delay10KTCYx(2); 
}
/*
 ****************************************************************
 *                        init_LCD
 **************************************************************** 
 */
void init_LCD(void){
    OpenXLCD( FOUR_BIT & LINES_5X7 );
    while( BusyXLCD() );
    WriteCmdXLCD( FOUR_BIT & LINES_5X7 );
    while( BusyXLCD() );
    WriteCmdXLCD( BLINK_ON );
    while( BusyXLCD() );
    WriteCmdXLCD( SHIFT_DISP_LEFT );
}
void config_DATA_PINS(int state)
{
    //Set all data pins as outputs initially
    DATA_0_TRIS =state; 
    DATA_1_TRIS =state;  
    DATA_2_TRIS =state; 
    DATA_3_TRIS =state;  
    DATA_4_TRIS =state; 
    DATA_5_TRIS =state; 
    DATA_6_TRIS =state; 
    DATA_7_TRIS =state; 
}

void config_PINS(void)
{
    TRISA=1;
    TRISB=1;
    TRISC=1;
    TRISD=1;


    ADCON1 = 0x07;      //to ensure that all analog pins are set as normal I/O
    //Set all data pins as outputs initially
    DATA_0_TRIS =1; 
    DATA_1_TRIS =1;  
    DATA_2_TRIS =1; 
    DATA_3_TRIS =1;  
    DATA_4_TRIS =1; 
    DATA_5_TRIS =1; 
    DATA_6_TRIS =1; 
    DATA_7_TRIS =1; 
    
    
    //Set them as outputs 
    WE_FLASH_TRIS =0;
    OE_FLASH_TRIS =0;
    CE_FLASH_TRIS =0;
    
    SER_SR_TRIS =0;
    RCK_SR_TRIS =0;
    SRCK_SR_TRIS =0;
}
void initi_PINS(void)
{
    config_PINS();
    DATA_0 =0; 
    DATA_1 =0;  
    DATA_2 =0; 
    DATA_3 =0;  
    DATA_4 =0; 
    DATA_5 =0; 
    DATA_6 =0; 
    DATA_7 =0; 
    
    
    //Set them as Low
    WE_FLASH =0;
    OE_FLASH =0;
    CE_FLASH =0;
    
    SER_SR =0;
    RCK_SR =0;
    SRCK_SR =0;  
}

void write_data_byte(unsigned int data)
{
    int i;
    unsigned int tempData;
    config_DATA_PINS(0);        //configure the data pins as outputs
    for(i=0;i<8;i++)
    {
        tempData=data;
        switch(i)
        {
                case 0:
                    DATA_0=tempData&bitmask_data;
                    break;
                case 1:
                    tempData=tempData>>i;
                    DATA_1=tempData&bitmask_data;
                    break;
                case 2:
                    tempData=tempData>>i;
                    DATA_2=tempData&bitmask_data;
                    break;
                case 3:
                    tempData=tempData>>i;
                    DATA_3=tempData&bitmask_data;
                    break;
                case 4:
                    tempData=tempData>>i;
                    DATA_4=tempData&bitmask_data;
                    break;
                case 5:
                    tempData=tempData>>i;
                    DATA_5=tempData&bitmask_data;
                    break;
                case 6:
                    tempData=tempData>>i;
                    DATA_6=tempData&bitmask_data;
                    break;
                case 7:
                    tempData=tempData>>i;
                    DATA_7=tempData&bitmask_data;
                    break;    
        }
    }
}

unsigned int read_data_byte(void)
{
    int i;
    unsigned int Data=0;
    config_DATA_PINS(1);        //configure the data pins as inputs
    
    //Configure flash to read 
    WE_FLASH =1;
    OE_FLASH =0;
    CE_FLASH =0;
    Delay10TCYx(20);    
    for(i=0;i<8;i++)
    {
        switch(i)
        {
                case 0:
                    Data=Data|DATA_7;
                    break;
                case 1:
                    Data=Data<<1;
                    Data=Data|DATA_6;
                    break;
                case 2:
                    Data=Data<<1;
                    Data=Data|DATA_5;
                    break;
                case 3:
                    Data=Data<<1;
                    Data=Data|DATA_4;
                    break;
                case 4:
                    Data=Data<<1;
                    Data=Data|DATA_3;
                    break;
                case 5:
                    Data=Data<<1;
                    Data=Data|DATA_2;
                    break;
                case 6:
                    Data=Data<<1;
                    Data=Data|DATA_1;
                    break;
                case 7:
                    Data=Data<<1;
                    Data=Data|DATA_0;
                    break;    
        }
    }
    OE_FLASH =1;
    CE_FLASH =1;
    return Data;
}

void write_byte_SR(unsigned int data)
{
    int i;
    unsigned int data_temp;
    
    RCK_SR=0;           //to prevent storage to shift register memory 
    for(i=0;i<8;i++)
    {
        SRCK_SR=0;    
        data_temp=(data>>(7-i));
        data_temp=(data_temp&bitmask_data);
        SER_SR=data_temp;
        SRCK_SR=1;          //send data down SR
    }
    RCK_SR=1;          //Store byte to SR memory
    Delay10TCYx(1);
}


void write_ADDR_Flash(unsigned int addr)
{
    unsigned int addr_copy=addr;
    unsigned int addr_high=0;
    unsigned int addr_low=0;
    addr_high= (addr_copy >>8);
    addr_copy=addr;
    addr_low=(addr_copy&0x00FF);

    write_byte_SR(addr_high);
    write_byte_SR(addr_low);
}

void store_data_to_flash_command(void)
{
    WE_FLASH = 0;
    CE_FLASH= 0;

    WE_FLASH = 1;
    CE_FLASH= 1;
    
    WE_FLASH=1;
    CE_FLASH=0;
    
    WE_FLASH = 1;
    CE_FLASH =1;
}

void write_ADDR_BYTE_Flash(unsigned int addr,unsigned int data)
{
    write_ADDR_Flash(addr);
    write_data_byte(data);
    store_data_to_flash_command();
    Delay1TCY();
    config_DATA_PINS(1);        //this is done to ensure that both the flash
                                //and pic pins are not both outputting at the same
                                //time
    
}

void set_operation_modes_selection_pins(void)   
{
    OE_FLASH=1;
    WE_FLASH = 1;
    CE_FLASH =1;
}

void sector_erase_operation_modes_selection(void) 
{
    OE_FLASH=1;
    WE_FLASH = 0;
    CE_FLASH =0;
}

void read_operation_modes_selection(void)    
{
    OE_FLASH=1;
    WE_FLASH=1;
    CE_FLASH=1;
    
    Delay1TCY();
    
    OE_FLASH=0;
    CE_FLASH=0;
}

void unset_operation_modes_selection_pins(void)  
{
    WE_FLASH=0;
    CE_FLASH=1;
    OE_FLASH=1;
}


void ERASE_Sector(unsigned int address)
{
    sector_erase_operation_modes_selection();
    write_ADDR_BYTE_Flash(0x5555,0xAA);
    write_ADDR_BYTE_Flash(0x2AAA,0x55);
    write_ADDR_BYTE_Flash(0x5555,0x80);
    write_ADDR_BYTE_Flash(0x5555,0xAA);
    write_ADDR_BYTE_Flash(0x2AAA,0x55);
    write_ADDR_BYTE_Flash(address,0x30);
}

void WRITE_Byte(unsigned int address, unsigned int data)
{
    set_operation_modes_selection_pins();
    write_ADDR_BYTE_Flash(0x5555,0xAA);
    write_ADDR_BYTE_Flash(0x2AAA,0x55);
    write_ADDR_BYTE_Flash(0x5555,0xA0);
    write_ADDR_BYTE_Flash(address,data);
    unset_operation_modes_selection_pins();
}

unsigned int READ_Byte(unsigned int address)
{
    unsigned int Data=0;
    read_operation_modes_selection();
    write_ADDR_BYTE_Flash(address,0x00);
    Data=read_data_byte();
    return Data;
}

void display(unsigned int address,unsigned int data)
{
    char addr_char[4];
    char data_char[4];

    itoa(address,addr_char);
    itoa(data,data_char);

    while(BusyXLCD());                                      //HR
    WriteCmdXLCD(0b00000001);
    
    while(BusyXLCD());
    SetDDRamAddr( 0x00 );
    putrsXLCD( "Addr:");

    while(BusyXLCD());
    SetDDRamAddr( 0x08 );
    putsXLCD(addr_char);
    
    while(BusyXLCD());                                  //HRV
    SetDDRamAddr( 0x40 );
    
    putrsXLCD( "Data:");
    while(BusyXLCD());
    SetDDRamAddr( 0x47 );
    putsXLCD(data_char);
}


void main(void)
{

    initi_PINS();
    init_LCD();
    WRITE_Byte(0x0000, 0x05);
}




