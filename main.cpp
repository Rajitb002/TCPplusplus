#include <iostream>
#include <stdlib.h>
#include <cstring>
#include <unistd.h>
#include <net/if.h>
#include <linux/if_tun.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <stdarg.h>

using namespace std;
typedef struct{

}TCP;

typedef struct{


}Quad;

/* buffer for reading from tun/tap interface, must be >= 1500 */
#define BUFSIZE 2000
char buffer[BUFSIZE];

/**
 *
 * @brief This main method implements a TCP connection as specified by IETF's RCF 793, RCF 7414, ...to a bare minimum. This is heavily
 * inspired by Jon Gjenset's series in implementing TCP in Rust
/

/**
 * @brief
 * Universal TUN/TAP device driver.
   Copyright (C) 1999-2000 Maxim Krasnyansky <max_mk@yahoo.com>
   Linux, Solaris drivers
   Copyright (C) 1999-2000 Maxim Krasnyansky <max_mk@yahoo.com>

   FreeBSD TAP driver
   Copyright (c) 1999-2000 Maksim Yevmenkin <m_evmenkin@yahoo.com>

   Revision of this document 2002 by Florian Thiel <florian.thiel@gmx.net>
 *
 * @param dev
 * @return int
 */
int tun_alloc()
{
	struct ifreq ifr;
	int fd, err;

	if ((fd = open("/dev/net/tun", O_RDWR)) < 0)
	{
		cout << "What was not supposed to happen with tun_alloc_old happened" << "\n";
		return fd; // This never happens apparently unless u run multiple threads of the interface
	}

	memset(&ifr, 0, sizeof(ifr));

	/* Flags: IFF_TUN   - TUN device (no Ethernet headers)
	 *        IFF_TAP   - TAP device
	 *
	 *        IFF_NO_PI - Do not provide packet information
	 */
	ifr.ifr_flags = IFF_TUN;
	strncpy(ifr.ifr_name, "tun0", IFNAMSIZ);
	if ((err = ioctl(fd, TUNSETIFF, (void *)&ifr)) < 0)
	{
		close(fd);
		cout << "Error in attaching interface"
			 << "\n";
		return err;
	}
	return fd;
}

int main()
{
	int tun_fd = 0;

	cout << "Reading from tun0" << "\n";

	int nread;
	tun_fd = tun_alloc(); /* tun interface */

	if (tun_fd < 0)
	{
		cerr << "Could not connect to existing  interface" << "\n";
		exit(EXIT_FAILURE);
	}

	/* Now read data coming from the kernel */
	while (1)
	{
		/* Note that "buffer" should be at least the MTU size of the interface, eg 1500 bytes */
		// This captures packets being sent 0.0 to 0.255 (192/24)
		nread = read(tun_fd, buffer, sizeof(buffer));
		uint16_t flags = buffer[0] << 4 | buffer[1];
		uint16_t protocol = buffer[2] << 8  | buffer[3];
		if (protocol != 0x800){
			continue;
		}
		cout << "tun0 sending out : "<< nread << " bytes \n";
		cout << "flags:" << hex << static_cast<int>(flags)  << "\n";
		cout << "proto:" << hex <<static_cast<int>(protocol) << "\n";
		
		// implement ether parsing
		//create structs for quad and tcp
		
	}


	close(tun_fd);
	return 0;
}
