# TinyWatchdog
ATTiny10 based external watchdog firmware.
Uses the libavr-- library.

This is a simple external watchdog chip. It should be placed in-line to the power supply and the micro-controller that it governs:
*Power -> TinyWatchdog VCC -> TinyWatchdog VOUT -> Other microcontroller VCC*

## Normal operation

Thw "W" or "WDR" (watchdog reset) pin on the TinyWatchdog is pulled up to VCC. The external micro-controller that is being goverened by the external watchdog circuit must pull thw WDR signal to GND within aproximately 4s. If no low-going edge is detected witin the timeout period (4s), VOUT is disconnected briefly.

### Pinout
           ______
      GND -|1  6|- N/C
     WRST -|2  5|- N/C
      VCC -|3  4|- VOUT
           ‾‾‾‾‾‾

| Pin | Top | Bottom | Description |
|-|-|-|-|
| 1 | G | GND | Ground |
| 2 | W | WRST | Watchdog reset signal |
| 3 | > | VCC | Voltage in (3.3V - 6V) |
| 4 | < | VOUT | Voltage output (max. 2A) |
| 5 | | | Do not connect |
| 6 | | | Do not connect |

## Using TinyWatchdog as a break-out board
TinyWatchdog can also be used as a generic ATTiny10 break-out board. It can be flashed using the standard TPI protocol. To enable this, the TPIDATA and RESET pins are available as the unlabelled pins 5 and 6 on the TinyWatchdog board.

Note: PB2 is connected through a P-channel MOSFET to pin 4 of the TinyWatchdog board. This means it can be used as an inverted output only. PB2 cannot be used as an input.

### Pinout - ATTiny10 pin mapping
                   ______
              GND -|1  6|- PB3 / RESET
     TPICLK / PB1 -|2  5|- PB0 / TPIDATA
              VCC -|3  4|- !PB2
                   ‾‾‾‾‾‾