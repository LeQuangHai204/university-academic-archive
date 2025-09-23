## Token

- Gitlab: glpat-fFJbmCezbPYry7jaMuuy


## Default Setup

If the input or output policy for the firewall is set to DROP or REJECT, the following is still allowed for all Proxmox VE hosts in the cluster:

- loopback interface
- already established connections
- IGMP protocol
- TCP from management hosts to port 8006
- TCP from management hosts to port range 5900 to 5999 for VNC web console
- TCP from management hosts to port 3128 for SPICE proxy
- TCP from management hosts to port 22
- UDP in the cluster network to ports 5405-5412 for corosync
- UDP multicast in the cluster network
- ICMP traffic type 3 (Destination Unreachable), 4 (congestion control) or 11 (Time Exceeded)

The following traffic is dropped, but not logged even with logging enabled:

- TCP with invalid state
- Broadcast, multicast and anycast not related to corosync, i.e., not coming through ports 5405-5412
- TCP to port 43
- UDP to ports 135 and 445
- UDP to the port range 137 to 139
- UDP form source port 137 to port range 1024 to 65535
- UDP to port 1900
- TCP to port 135, 139 and 445
- UDP traffic originating from source port 53

The rest of the traffic is dropped or rejected, respectively, and also logged. This may vary depending on the additional options enabled in Firewall → Options, such as NDP, SMURFS and TCP flag filtering.

--------------------------------------------------------------------------------------------

Web interface: 8006 (TCP, HTTP/1.1 over TLS)

VNC Web console: 5900-5999 (TCP, WebSocket)

SPICE proxy: 3128 (TCP)

sshd (used for cluster actions): 22 (TCP)

rpcbind: 111 (UDP)

sendmail: 25 (TCP, outgoing)

corosync cluster traffic: 5405-5412 UDP

live migration (VM memory and local-disk data): 60000-60050 (TCP)