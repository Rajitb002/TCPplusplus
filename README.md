# TCPplusplus
A simple C++ userspace implementation of the TCP stack . To be more specific, an implementation of core features of TCP that allow us to transrecieve a packet with another device.

RFC 793 is the main one. 2525 for extra stuff/testing. All under 7414.

Directly receiving/sending packets from the kernel will interfere with the actual stack, so we use Tun-Tap/virtual network interface that is present in the userspace and takes care of sending/receiving packets.

Run the makefile first to create the tun device if you don't have one. The tun device will not be in state UP until you attach the program to the interface by actually running the program, meaning, you need the interface to be UP to be able to receive and transmit packets.
