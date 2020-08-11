// TinyWatchdog | A simple external watchdog firmware for ATTiny10
// Copyright (C) 2020 Andre Eisenbach
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// See LICENSE for a copy of the GNU General Public License or see
// it online at <http://www.gnu.org/licenses/>.

#include <libavr--/asm.h>
#include <libavr--/io.h>

using namespace avr::io;

// PCINT0_vect
extern "C" __attribute__((signal, used, externally_visible)) void __vector_2() {
  // On pin change, we just reset the watchdog...
  // If no interrupt occurs within the watchdog timeout, we'll be reset and
  // start all over.
  avr::wdr();
}

// Silly delay function. The time unit of d is pretty arbitrary here.
inline void delay(volatile uint32_t d) {
  while (--d) avr::nop();
}

int main() {
  // Clear pending watchdog flags
  RSTFLR = 0;
  avr::wdr();

  // Disable some peripherals for power savings
  PRR = (1 << PRADC) | (1 << PRTIM0);

  // Enable watchdog; 4s timeout
  // Next two lines are a sequenced write operation
  CCP = CCP_UNLOCK_PROTECTED_REGISTERS;
  WDTCSR = (1 << WDE) | (1 << WDP3);

  // Set PB2 as output; enable pull-up on PB1
  DDRB = (1 << PB2);
  PUEB = (1 << PB1);

  // Disable power for some amount of time. This is
  // the part that would reset the dependent MCU.
  PORTB = (1 << PB2);
  delay(10'000);
  PORTB = 0;

  // Set sleep mode power down
  SMCR = (1 << SM1);

  // Enable pin change interrupt
  PCMSK = (1 << PCINT1);
  PCICR = (1 << PCIE0);

  // Enable interrupts and reset watchdog one last time.
  avr::wdr();
  avr::sei();

  // And now, we just sleep....
  // The pin change interrupt will cause the chip to wake up
  // and reset the watchdog. Afterwards we go right back to
  // sleep.
  while (1) {
    SMCR |= (1 << SE);  // Sleep enable
    avr::sleep();
    SMCR &= ~(1 << SE);  // Sleep disable
  }
}
