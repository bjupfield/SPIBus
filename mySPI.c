#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <linux/spi/spidev.h>
#include <sys/ioctl.h>
#include <fcntl.h>

int spi_fd;

typedef struct spi_ioc_transfer spi;
int openSpi()
{
	spi_fd = open("/dev/spidev0.0", O_RDWR);
	if(spi_fd < 0)
		return -1;
	return 1;
}

int readSpiMCP3008(int channel)
{
	if(channel > 7 || channel < 0)
	{
		printf("Channel Num Cannot Exceed 7\n");
		exit(-1);
	}
	
	spi mySPI = {0};
	
	uint8_t txb[4];
	txb[0] = 1;
	txb[1] = 8 + channel;
	txb[2] = 0;
	txb[3] = 0;
	//ill be honest, I dont really know what this does
	//I read the documentation for the MCP3008 and it seems to either be saying that the first byte needs
	//to be 0-15 to indicate what channels you want to target, or that the first byte needs to be 1 and
	//the second byte needs to be 0-15...
	//neither seem to work... but this gets me my number
	
	uint8_t rxb[4] = {0};
	
	mySPI.tx_buf = (unsigned long)txb;
	mySPI.rx_buf = (unsigned long)rxb;
	mySPI.len = 3;
	mySPI.bits_per_word = 8;
	mySPI.delay_usecs = 0;
	mySPI.speed_hz = 4000;
	
	ioctl(spi_fd, SPI_IOC_MESSAGE(1), &mySPI);
	return (rxb[1]<<8) +(rxb[2]);
}

int main(int argc, char* argv[])
{
	if(argc < 2)
	{
		printf("Enter Channel Num as Arg\n");
		return EXIT_FAILURE;
	}
	
	if(openSpi() < 0)
	{
		printf("File Failed to Open\n");
		return EXIT_FAILURE;
	}
	
	printf("Spi returned: %d \n", readSpiMCP3008(argv[1][0] - 48));
	
	printf("Code Completed with No Errors\n");
	
	return EXIT_SUCCESS;
}
