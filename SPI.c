#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <linux/spi/spidev.h>
#include <termios.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include "kbhit.c"

typedef struct spi_ioc_transfer spi;
int file;

//this section is used to allow a single key to close program
// <3

void start(struct termios *oldt, struct termios *newt)
{
	tcgetattr(STDIN_FILENO, oldt);
	newt->c_lflag &= ~(ICANON);
	tcsetattr( STDIN_FILENO, TCSANOW, newt);
}

void closed(struct termios *old)
{
	tcsetattr(STDIN_FILENO, TCSANOW, old);
}

//
// <3

//this section implements the spidev functions dev\fined by spidev
// <3

int open_Spi(spi* spy)
{
	
	spy->tx_buf = (unsigned long)malloc(sizeof(unsigned long));
	spy->rx_buf = (unsigned long)malloc(sizeof(unsigned long));
	spy->len = 4;
	//set to read
	spy->tx_buf = 64;
	spy->tx_buf << 2;
	spy->tx_buf += 19;
	spy->tx_buf << 4;

	
	file = open("/dev/spidev/0.0", O_RDWR);
	
	return -1;
}

int close_Spi(spi* spy)
{
	
	close(file);
	return 0;
}

int write_Spi(spi* spy)
{
	return -1;
}

unsigned long read_Spi(spi* spy)
{	
	
	ioctl(file, SPI_IOC_MESSAGE(1), &spy);
	if(spy->rx_buf != 0)
	{
		return spy->rx_buf;
	}
	return -1;
}

//
// <3

int main(int argc, char** argv)
{
	if(argc > 1)
	{
		printf("No Arguments Allowed\n");
		exit(1);
	}
	
	char* dispensery = malloc(sizeof(char) * 50);
	
	int status = 1;
	printf("Press Any Key To End\n");
	sleep(3);
	
	//spi communicator
	
	spi* spy = malloc(sizeof(spi));
	open_Spi(spy);
	
	//look at how spy works, on favorites tab on fire fox
	//basically send open(location, message, spi struct);
	//^ basic syntax i think
	
	
	static struct termios oldt, newt;
	start(&oldt, &newt);
	while(status == 1)
	{
		
		printf("Char is: %lu", read_Spi(spy));
		
		if(kbhit())
		{
			status = 0;
			getchar();
			continue;
		}
		
		
		usleep(10);
		
	}
	closed(&oldt);
	
	printf("\nKey Was Entered\n");
	
}
