build:
	g++ -o tcp_server tcp_server.cpp
	./tcp_server

tun:	
	sudo ip tuntap add mode tun tun0
	sudo ip addr add 192.168.0.1/24 dev tun0
	sudo ip link set up dev tun0

