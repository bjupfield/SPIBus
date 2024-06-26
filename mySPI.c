#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
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

int readSpiMCP3008(int channel, int mode)
{
	if(channel > 7 || channel < 0)
	{
		printf("Channel Num Cannot Exceed 7\n");
		exit(-1);
	}
	
	spi mySPI = {0};
	
	uint8_t txb[3];
	txb[0] = mode;
	txb[1] = (channel << 4) + 128;
	txb[2] = 0;
	
	//finally got the channel select working and actually understand what the txb is doing
	//refer to notes for more information
	
	uint8_t rxb[3] = {0};
	
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
	int mode = 1; 
	if(argc < 2)
	{
		printf("Enter Channel Num as Arg\n");
		return EXIT_FAILURE;
	}
	if(argc >= 3)
	{
		if(argv[2][0] != '0' && argv[2][0] != '1')
		{
			printf("Arg three must be 0 or 1 to indicate mode\n");
			return EXIT_FAILURE;
		}
		mode = argv[2][0] - 48;
	}
	
	if(openSpi() < 0)
	{
		printf("File Failed to Open\n");
		return EXIT_FAILURE;
	}
	
	while(1)
	{
		printf("Spi returned: %d \n", readSpiMCP3008(argv[1][0] - 48, mode));
		sleep(1);
	}
	printf("Code Completed with No Errors\n");
	
	return EXIT_SUCCESS;
}
