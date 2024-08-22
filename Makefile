build:
	g++ -o main main.cpp
	./main

tun:	
	sudo ip tuntap add mode tun tun0
	sudo ip addr add 192.168.0.1/24 dev tun0
	sudo ip link set up dev tun0
show:
	ip link show

ping_to:
	ping 192.168.0.1

ping_from:
	ping -I tun0 192.168.1.1
