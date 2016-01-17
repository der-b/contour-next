This is an implementation to read the messurements from Bayer Contour Next USB Device.

At the moment it only read the whole data from the device and print it to standard output. The data is fomated as ASTM 1394-91 packets. Each line contain one packet. Fortunately ASTM 1394-91 is composed only with printable characters, BUT this is not checked by the program. Therefore be carefully using the program.

For this implementation I had no complete protocol specification. The most knowledge I got by reverse engineering the protocol. To be more exact I implementet three protocols. 
The first one I call "simple protocol" (.src/simple\_protocol/). I'm not sure whether this protocoll follows some kind of standard or if it is a properitary protocol. It is a very simple protocol.
The second one looks like ASTM 1381-95. For this protocol I had or complet specification, since one have to pay to get the specification (http://www.astm.org/Standards/E1394.htm). But i got some information from other sources (http://www.inovadx.com/portal/Technical%20Support/QUANTA%20Link/QL_ASTM_protocol.pdf).
The third one looks like ASTM 1394-91 (http://www.astm.org/DATABASE.CART/HISTORICAL/E1381-95.htm). I got some information from the same soruces as for ASTM 1381-95.


compiling:

./configure
make

to access the device one needs either root privileges or you can add a new udev rule. For example you can create a new group:

groupadd --system glucometer

create a new udev rule in "/etc/udev/rules.d/30-glucometer.rules" containing following line:

ACTION=="add", KERNEL=="hiddev\*", ATTRS{idVendor}=="1a79", ATTRS{idProduct}=="7410", GROUP="glucometer", MODE="0660"

as long as the user is member of the gorup glucometer you can run the tool with:

./src/contour-next-usb/contour-next


TODOs:

- The datatransmission from the computer to the device is a quick hack (see: main() in ./src/contour-next-usb/contour.c)
- Implement protocol to change device settings
- ASTM 1394-91 implementation is not complete, only basics
- Dangerous assumption in ./src/astm/astm-1381-95.c in \_asmt1381\_95\_textframe()

