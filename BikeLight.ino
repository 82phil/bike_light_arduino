// NeoPixel Ring simple sketch (c) 2013 Shae Erisson
// released under the GPLv3 license to match the rest of the AdaFruit NeoPixel library

#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif

// Which pin on the Arduino is connected to the NeoPixels?
// On a Trinket or Gemma we suggest changing this to 1
#define PINA  7
#define PINB  6

// When we setup the NeoPixel library, we tell it how many pixels, and which pin to use to send signals.
// Note that for older NeoPixel strips you might need to change the third parameter--see the strandtest
// example for more information on possible values.
Adafruit_NeoPixel StripA = Adafruit_NeoPixel(30, PINA, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel StripB = Adafruit_NeoPixel(8, PINB, NEO_GRB + NEO_KHZ800);

int delayval = 50; // delay for half a second

/*
  Firmata is a generic protocol for communicating with microcontrollers
  from software on a host computer. It is intended to work with
  any host computer software package.

  To download a host software package, please clink on the following link
  to open the list of Firmata client libraries your default browser.

  https://github.com/firmata/arduino#firmata-client-libraries

  Copyright (C) 2006-2008 Hans-Christoph Steiner.  All rights reserved.
  Copyright (C) 2010-2011 Paul Stoffregen.  All rights reserved.
  Copyright (C) 2009 Shigeru Kobayashi.  All rights reserved.
  Copyright (C) 2009-2016 Jeff Hoefs.  All rights reserved.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  See file LICENSE.txt for further informations on licensing terms.

  Last updated by Jeff Hoefs: January 10th, 2016
*/

#include <Wire.h>
#include <Firmata.h>

/*==============================================================================
 * GLOBAL VARIABLES
 *============================================================================*/

#ifdef FIRMATA_SERIAL_FEATURE
SerialFirmata serialFeature;
#endif

/* utility functions */
void wireWrite(byte data)
{
#if ARDUINO >= 100
  Wire.write((byte)data);
#else
  Wire.send(data);
#endif
}

byte wireRead(void)
{
#if ARDUINO >= 100
  return Wire.read();
#else
  return Wire.receive();
#endif
}

/*==============================================================================
 * SYSEX-BASED commands
 *============================================================================*/

void sysexCallback(byte command, byte argc, byte *argv)
{
  switch (command) {
    case 0x08:
      // Front Right Segment
      for(int i=0;i<4;i++){
        StripA.setPixelColor(i, StripA.Color(argv[0], argv[1], argv[2]));
      }
      StripA.show();
      break;
    case 0x09:
      // Front Side Right Segment
      for(int i=4;i<8;i++){
        StripA.setPixelColor(i, StripA.Color(argv[0], argv[1], argv[2]));
      }
      StripA.show();
      break;
    case 0x0A:
      // Front Left Segment
      for(int i=8;i<12;i++){
        StripA.setPixelColor(i, StripA.Color(argv[0], argv[1], argv[2]));
      }
      StripA.show();
      break;
    case 0x0B:
      // Front Side Left Segment
      for(int i=12;i<16;i++){
        StripA.setPixelColor(i, StripA.Color(argv[0], argv[1], argv[2]));
      }
      StripA.show();
      break;
    case 0x0C:
      // Front Side Left Segment
      for(int i=21;i<26;i++){
        StripA.setPixelColor(i, StripA.Color(argv[0], argv[1], argv[2]));
      }
      StripA.show();
      break;
    case 0x0D:
      // Side Right
      for(int i=16;i<21;i++){
        StripA.setPixelColor(i, StripA.Color(argv[0], argv[1], argv[2]));
      }
      StripA.show();
      break;
    case 0x0E:
      // Front Side Left Segment
      for(int i=0;i<4;i++){
        StripB.setPixelColor(i, StripB.Color(argv[0], argv[1], argv[2]));
      }
      StripB.show();
      break;
    case SERIAL_MESSAGE:
#ifdef FIRMATA_SERIAL_FEATURE
      serialFeature.handleSysex(command, argc, argv);
#endif
      break;
  }
}

/*==============================================================================
 * SETUP()
 *============================================================================*/

void systemResetCallback()
{

#ifdef FIRMATA_SERIAL_FEATURE
  serialFeature.reset();
#endif

}

void setup()
{
  ////////////////////////////////////////////////////////////////////////
  // NeoPixels Setup
  ////////////////////////////////////////////////////////////////////////
  
  // This is for Trinket 5V 16MHz, you can remove these three lines if you are not using a Trinket
#if defined (__AVR_ATtiny85__)
  if (F_CPU == 16000000) clock_prescale_set(clock_div_1);
#endif
  // End of trinket special code

  StripA.begin(); // This initializes the NeoPixel library.
  StripB.begin();

  

  ////////////////////////////////////////////////////////////////////////
  // Firmata Setup
  ////////////////////////////////////////////////////////////////////////
  
  Firmata.setFirmwareVersion(FIRMATA_FIRMWARE_MAJOR_VERSION, FIRMATA_FIRMWARE_MINOR_VERSION);
  
  Firmata.attach(START_SYSEX, sysexCallback);
  Firmata.attach(SYSTEM_RESET, systemResetCallback);

  // to use a port other than Serial, such as Serial1 on an Arduino Leonardo or Mega,
  // Call begin(baud) on the alternate serial port and pass it to Firmata to begin like this:
  // Serial1.begin(9600);
  // Firmata.begin(Serial1);
  // However do not do this if you are using SERIAL_MESSAGE

  Firmata.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for ATmega32u4-based boards and Arduino 101
  }

  systemResetCallback();  // reset to default config
}

/*==============================================================================
 * LOOP()
 *============================================================================*/
void loop()
{
  while (Firmata.available())
    Firmata.processInput();

#ifdef FIRMATA_SERIAL_FEATURE
  serialFeature.update();
#endif
}
