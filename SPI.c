#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <stdint.h>
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

int open_Spi()
{
	

	printf("hello");
	file = open("/dev/spidev/0.0", O_RDWR);
	
	return -1;
}

int close_Spi()
{
	
	close(file);
	return 0;
}

int write_Spi()
{
	return -1;
}

int read_Spi()
{	
	spi spy = {0};
	uint8_t rx[4] = {0};
	
	spy.tx_buf = (unsigned long)malloc(sizeof(unsigned long));
	spy.rx_buf = (unsigned long)rx;
	spy.len = 3;
	spy.delay_usecs = 0;
	spy.speed_hz = 5000;
	spy.bits_per_word = 8;
	
	//set to read
	spy.tx_buf = 1;
	spy.tx_buf << 6;
	
	ioctl(file, SPI_IOC_MESSAGE(1), spy);
	
	for(int i = 0; i < 4; i++)
	{
		printf("%d, ", rx[i]);
	}
	printf("\n");
	
	if(spy.rx_buf != 0)
	{
		return rx[3];
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
	
	//char* dispensery = malloc(sizeof(char) * 50);
	
	int status = 1;
	printf("Press Any Key To End\n");
	sleep(3);
	
	//spi communicator
	
	open_Spi();
	
	//look at how spy works, on favorites tab on fire fox
	//basically send open(location, message, spi struct);
	//^ basic syntax i think
	
	
	//static struct termios oldt, newt;
	//start(&oldt, &newt);
	while(status == 1)
	{
		
		printf("Char is: %d\n", read_Spi());
		
		if(kbhit())
		{
			status = 0;
			getchar();
			continue;
		}
		
		
		usleep(100000);
		
	}
	//closed(&oldt);
	
	printf("\nKey Was Entered\n");
	
}
