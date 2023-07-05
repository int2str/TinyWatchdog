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

#include "device/attiny10.h"

using namespace avr::io;

// PCINT0_vect
extern "C" __attribute__((signal, used, externally_visible)) void __vector_2() {
  // On pin change, we just reset the watchdog...
  // If no interrupt occurs within the watchdog timeout, we'll be reset and
  // start all over.
  avr::wdr();
}

int main() {
  // Clear pending watchdog flags
  RSTFLR = 0;
  avr::wdr();

  // Disable some peripherals for power savings
  PRR = (1 << PRADC);

  // Enable watchdog; 4s timeout
  // Next two lines are a sequenced write operation
  CCP = CCP_UNLOCK_PROTECTED_REGISTERS;
  WDTCSR = (1 << WDE) | (1 << WDP3);

  DDRB = (1 << PB0);
  PORTB = (1 << PB0);

  TCCR0A = (1 << WGM02) | (1 << WGM00) | (1 << COM0A1);
  TCCR0B = (1 << CS00);

  OCR0AH = 0;
  OCR0AL = 0x48;

  CCP = CCP_UNLOCK_PROTECTED_REGISTERS;
  CLKPSR = (1 << CLKPS1);

  avr::sei();

  while (true) {
    avr::wdr();
    avr::nop();
  }
}
