/************************************************************************************************

 Author        : ADI - Apps            www.analog.com/

 Date          : June 2010

 File          : I2CTestSlave.c

 Hardware      : ADuC7124

 Description   : This Simple example shows how to initialize the part as a Slave for 
 				 I2C communication and use it to recieve data from The Master chip. 		
*************************************************************************************************/
// Bit Definitions
#define BIT2  0x04   //I2C Slave TX DATA STATUS BIT
#define BIT3  0x08   //I2C Slave RX DATA STATUS BIT
#define BIT8  0x100  //清除從機 FIFO BIT
#define BIT10 0x400  //I2C Stop STATUS BIT
#define BIT13 0x2000 //I2C Start STATUS BIT

#include<aduc7126.h>

unsigned char szTxData[16];	// Array to recieve from master //接收主機資料
unsigned char ucTxCount = 0;  // Array index variable for szTxData[] //接主機資料幾筆
unsigned char szRxData[16];	  // Array for reading Data from Slave //從副機讀取資料
unsigned char ucRxCount = 0;  // Array index variable for szRxData[]//接副機資料幾筆

unsigned char szStoreData[16];	// Array to store data from master
unsigned char ucStoreCount = 0;  // Array index variable for szStoreData[]

int main(void)
{
	POWKEY1 = 0x01;
	POWCON0 = 0x00;								// Set core at maximum speed//設定為41.78MHZ
	POWKEY2 = 0xF4;

	POWKEY3 = 0x76;
	POWCON1 = 0x124;							//powers up SPI & I2C (by default)//啟動SPI和I2CO,I2C1
	POWKEY4 = 0xB1;

	GP1CON = 0x2222;							//Set P1.0-3 as I2C pins
	
	I2C1SCON = 0x701;             //中斷啟動,SLECT SLAVE
	I2C1ID0 = 0x08;               //單板用ID

	// Begin Slave Transmit sequence
	I2C1FSTA = BIT8;						// Flush Slave Tx FIFO  //置1可清除從機發送FIFO
	I2C1FSTA &= ~BIT8;          

	//IRQCONE = 0x02;			// Rising edge of XIRQ0
	IRQEN = 0x08000; 		// Enable I2C0S + XIRQ0 interrupt + uart//設定中斷源

  	// Begin receive sequence
	ucTxCount = 0;
	I2C1STX = szTxData[ucTxCount++];
	ucRxCount = 0;

	while(1)
	{   
	}

}

void IRQ_Handler(void) __irq
{  
 unsigned long IRQSTATUS = 0;
 unsigned long I2C1SSTATUS = 0; 

	IRQSTATUS = IRQSTA;	                		// Read off IRQSTA register  ///中斷源是那一個

	if ((IRQSTATUS & BIT13) == BIT13)	//If I2C Slave interrupt source//BIT13 0x2000 I2C0從機中斷源
	{
	   I2C1SSTATUS = I2C1SSTA; //I2C0狀態暫存器
	   if ((I2C1SSTATUS & BIT2) == BIT2) // If I2C Slave Tx IRQ//BIT2  0x04//I2C從機發送請求位
	   {
			if (ucStoreCount < 6)		   // Have max 6 bytes been sent?
				I2C1STX = szStoreData[ucStoreCount++];	 // Load Tx buffer //要傳送
	   }
	   if ((I2C1SSTATUS & BIT3) == BIT3) // If I2C Slave Rx IRQ// BIT3  0x08
	   {
			if (ucStoreCount < 6)		   // Have max 6 bytes been received?
			{
				szStoreData[ucStoreCount] = I2C1SRX;  // Read Rx buffer//要接收
				ucStoreCount++;
			}
	   }
	   if ((I2C1SSTATUS & BIT10) == BIT10) // If I2C Stop after Start condition detected IRQ
	   {
			ucStoreCount = 0;	// Reset counter Array pointer.
			I2C1FSTA = BIT8;			// Flush Slave Tx FIFO
			I2C1FSTA &= ~BIT8;
	   }
	   if ((I2C1SSTATUS & BIT13) == BIT13) // If I2C Repeated Start detected IRQ
	   {
			ucStoreCount = 0;	// Reset counter Array pointer.
			I2C1FSTA = BIT8;			// Flush Slave Tx FIFO
			I2C1FSTA &= ~BIT8;
	   }
	}
}
