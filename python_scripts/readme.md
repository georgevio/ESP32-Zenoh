There is no zenoh server avaliable currently, so to run a local zenoh server (router) in Windows, download the 
https://www.eclipse.org/downloads/download.php?file=/zenoh/zenoh/latest/zenoh-1.5.1-x86_64-pc-windows-msvc-standalone.zip

## BE CAREFUL 
the msvc compiled version seems to work in Windows (gnu version did not connect!)

## IMPORTANT NOTE: 
OPEN THE FIREWALL PORTS both for UDP/TCP 7447 both inbound/outbound and add /path/zenohd.exe to the exceptions. The easier way to test, is to TEMPORARILY disable the firewall!

Run the zenohd.exe file with the following parameters for both UDP & TCP for all IP addresses on the host:

```.\zenohd.exe -l tcp/0.0.0.0:7447 -l udp/0.0.0.0:7447```

## ESP32 Devs seem to connect fine to udp

## BE CAREFUL: 
if you run the .\zenohd.exe without parameters, it seems only possible to connect to the `localhost` but not to any IP adddress!

You can either run the pub and sub files or run the same zenoh_node.py twice with different parameters below

### Scenario 1: Server-Only Communication (client mode)
In this mode, if the defined server is unreachable, they will not communicate.

Terminal 1 (Subscriber):
```python zenoh_node.py --role sub --mode client```

Terminal 2 (Publisher):
```python zenoh_node.py --role pub --mode client```


### Scenario 2: P2P-Allowed Communication (peer mode)
They can talk directly if they are on the same local network.

Terminal 1 (Subscriber):
```python zenoh_node.py --role sub --mode peer```
(python zenoh_node.py --role sub # peer is the default)

Terminal 2 (Publisher):
```python zenoh_node.py --role pub --mode peer```