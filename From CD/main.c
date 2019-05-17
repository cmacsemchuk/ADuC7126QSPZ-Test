/************************************************************************************************

 Author        : ADI - Apps            www.analog.com/

 Date          : June 2010

 File          : I2CTestMaster.c

 Hardware      : ADuC7124

 Description   : This Simple example shows how to initialize the part as the Master Chip for 
 				 I2C communication and use it to transmit data to a Slave chip.		
*************************************************************************************************/
// Bit Definitions
#define BIT0  0x01
#define BIT1  0x02
#define BIT2  0x04
#define BIT3  0x08
#define BIT4  0x10
#define BIT5  0x20
#define BIT6  0x40
#define BIT7  0x80
#define BIT8  0x100
#define BIT9  0x200
#define BIT10 0x400
#define BIT11 0x800
#define BIT12 0x1000
#define BIT13 0x2000
#define BIT14 0x4000
#define BIT15 0x8000

#include<aduc7126.h>

unsigned char szTxData[] = {0x41, 0x42, 0x43, 0x44, 0xd, 0x0a};	// Array to send to Slave
unsigned char ucTxCount = 0;  // Array index variable for szTxData[]
unsigned char szRxData[16];	  // Array for reading Data from Slave
unsigned char ucRxCount = 0;  // Array index variable for szRxData[]


int main(void)
{
	POWKEY1 = 0x01;
	POWCON0 = 0x00;								// Set core at maximum speed
	POWKEY2 = 0xF4;

	POWKEY3 = 0x76;
	POWCON1 = 0x124;							//powers up SPI & I2C
	POWKEY4 = 0xB1;

	GP1CON = 0x2222;							//Set P1.0-3 as I2C pins

	I2C0MCON = 0x131;				// Enable I2C Master + Enable Rx interrupt
	 								// Enable Tx interrupt + Enable transmission complete interrupt
									// Enable Nack received IRQ

	I2C0ADR0 = 0x45;				// Set slave address to transmit data to.
	I2C0DIV  = 0xC0C0;				// Select  clock rate

	// Begin Master Transmit sequence
	I2C0FSTA = BIT9;						// Flush Master Tx FIFO
	I2C0FSTA &= ~BIT9;
	IRQCONE = 0x02;			// Rising edge of XIRQ0
	IRQEN = 0x21200; 		// Enable I2C0M + XIRQ0 interrupt + uart

	ucTxCount = 0;
	I2C0MTX = szTxData[ucTxCount++];
	ucRxCount = 0;

	while(1)
	{  
	}
}
/*
void IRQ_Handler(void) __irq
{ 
 unsigned long IRQSTATUS = 0;
 unsigned long I2C0MSTATUS = 0; 

	IRQSTATUS = IRQSTA;	                		// Read off IRQSTA register

	if ((IRQSTATUS & BIT12) == BIT12)	//If I2C0 Master interrupt source
	{
	   I2C0MSTATUS = I2C0MSTA;

	   if ((I2C0MSTATUS & BIT2) == BIT2) // If I2C Master Tx IRQ
	   {
			if (ucTxCount < 6)		   // Have max 6 bytes been sent?
				I2C0MTX = szTxData[ucTxCount++];	 // Load Tx buffer
	   }
	   if ((I2C0MSTATUS & BIT3) == BIT3) // If I2C Master Rx IRQ
	   {
			if (ucRxCount < 6)		   // Have max 6 bytes been received?
			{
				szRxData[ucRxCount] = I2C0MRX;  // Read Rx buffer
				ucRxCount++;
			}
	   }
	}
}
*/