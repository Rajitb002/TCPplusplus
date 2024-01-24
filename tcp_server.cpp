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
#include <arpa/inet.h>
#include <sys/select.h>
#include <sys/time.h>
#include <errno.h>
#include <stdarg.h>

using namespace std;

/* buffer for reading from tun/tap interface, must be >= 1500 */
#define BUFSIZE 2000
char buffer[BUFSIZE];

/**
 *
 * @brief This main method implements a TCP connection as specified by IETF's RCF 793, RCF XXX, ...to a bare minimum. This is heavily
 * inspired by Jon Gjenset's series in implementing TCP in Rust
 *
 * References:
 * Pcapplusplus: https://pcapplusplus.github.io/
 * Jon Gjenset's series: https://www.youtube.com/watch?v=bzja9fQWzdA&t=17809s
 *
 */

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
		cout << "What was not supposed to happen with tun_alloc_old happened"
			 << "\n";
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

	cout << "Reading from tun0"
		 << "\n";

	int nread;
	tun_fd = tun_alloc(); /* tun interface */

	if (tun_fd < 0)
	{
		perror("Could not connect to existing  interface");
		exit(1);
	}

	/* Now read data coming from the kernel */
	while (1)
	{
		/* Note that "buffer" should be at least the MTU size of the interface, eg 1500 bytes */
		nread = read(tun_fd, buffer, sizeof(buffer));
		if (nread < 0)
		{
			cout << "Could not read what tun0 is sending"
				 << "\n";

			close(tun_fd);
			exit(1);
		}
		cout << "tun0 sending out : "
			 << nread << "bytes \n";
	}
	return 0;
}
