## Example Summary

The WSN Concentrator example illustrates how to create a simple Wireless Sensor
Network Concentrator device which listens for packets from other nodes. This
example is meant to be used together with the WSN Node example to form a one-
to-many network where the nodes send messages to the concentrator.

This examples showcases the use of several Tasks, Semaphores and Events to
receive packets, send acknowledgements and display the received data on the
LCD. For the radio layer, this example uses the EasyLink API which provides
an easy-to-use API for the most frequently used radio operations.

Refer to [EasyLink API](http://processors.wiki.ti.com/index.php/SimpleLink-EasyLink)
for more information on the EasyLink API.


## Peripherals Exercised
* `Board_LED0` - Toggled when data is received over the RF interface

## Resources & Jumper Settings

Please refer to the development board's specific "Settings and Resources"
section in the Getting Started Guide (http://www.ti.com/lit/SPRUHU7C).
For convenience, a short summary is also shown below.

    | Development Boards | Notes                                               |
    | ================== | =================================================== |
    | CC1310DK           | Board_LED0 is the "LED4" LED                        |
    | ------------------ | --------------------------------------------------- |

## Example Usage
Run the example. On another board (or several boards) run the WSN Node example.
The LCD will show the discovered node(s). When the collector receives data from
a new node, it is given a new row on the display and the received value is shown.
If more than 7 nodes are detected, the device list rolls over, overriding
the first. Whenever an updated value is received from a node, it is updated on
the LCD display.

## Application Design Details
This examples consists of two tasks, one application task and one radio
protocol task.

The ConcentratorRadioTask handles the radio protocol. This sets up the EasyLink
API and uses it to always wait for packets on a set frequency. When it receives
a valid packet, it sends an ACK and then forwards it to the ConcentratorTask.

The ConentratorTask receives packets from the ConcentratorRadioTask, displays
the data on the LCD and toggles Board_LED0.

The default frequency is 868.0 MHz, in order to change the frequency, please see
"RadioProtocol.h". This can also be used to change the PHY settings to be either
the default IEEE 802.15.4g 50kbit, Long Range Mode or custom settings. In the
case of custom settings, the "smartrf_settings.c" file is used. This can be
changed either by exporting from Smart RF Studio or directly in the
file.

Note for IAR users: When using the CC1310DK, the TI XDS110v3 USB Emulator must
be selected. For the CC1310_LAUNCHXL, select TI XDS110 Emulator. In both cases,
select the cJTAG interface.

## References
* For GNU and IAR users, please read the following website for details
  about enabling [semi-hosting](http://processors.wiki.ti.com/index.php/TI-RTOS_Examples_SemiHosting)
  in order to view console output.
