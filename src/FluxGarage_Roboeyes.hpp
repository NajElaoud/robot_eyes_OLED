/*
 * FluxGarage RoboEyes for OLED Displays V 1.1.0
 * Draws smoothly animated robot eyes on OLED displays, based on the Adafruit GFX
 * library's graphics primitives, such as rounded rectangles and triangles.
 *
 * Copyright (C) 2024-2025 Dennis Hoelscher
 * www.fluxgarage.com
 * www.youtube.com/@FluxGarage
 *
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <https://www.gnu.org/licenses/>.
 */

#include <Arduino.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#ifndef _FLUXGARAGE_ROBOEYES_H
#define _FLUXGARAGE_ROBOEYES_H

// Display colors
extern uint8_t BGCOLOR;   // background and overlays
extern uint8_t MAINCOLOR; // drawings

// For mood type switch
#define DEFAULT 0
#define TIRED 1
#define ANGRY 2
#define HAPPY 3
#define DASAI_MOSHI 4

// For turning things on or off
#define ON 1
#define OFF 0

// For switch "predefined positions"
#define N 1  // north, top center
#define NE 2 // north-east, top right
#define E 3  // east, middle right
#define SE 4 // south-east, bottom right
#define S 5  // south, bottom center
#define SW 6 // south-west, bottom left
#define W 7  // west, middle left
#define NW 8 // north-west, top left
// for middle center set "DEFAULT"

// Forward declaration of display object (should be defined in main sketch)
extern Adafruit_SSD1306 display;

class roboEyes
{
private:
  // Yes, everything is currently still accessible. Be responsible and don't mess things up :)

public:
  // For general setup - screen size and max. frame rate
  int screenWidth = 128;      // OLED display width, in pixels
  int screenHeight = 64;      // OLED display height, in pixels
  int frameInterval = 20;     // default value for 50 frames per second (1000/50 = 20 milliseconds)
  unsigned long fpsTimer = 0; // for timing the frames per second

  // For controlling mood types and expressions
  bool tired = 0;
  bool angry = 0;
  bool happy = 0;
  bool curious = 0;   // if true, draw the outer eye larger when looking left or right
  bool cyclops = 0;   // if true, draw only one eye
  bool eyeL_open = 0; // left eye opened or closed?
  bool eyeR_open = 0; // right eye opened or closed?

  //*********************************************************************************************
  //  Eyes Geometry
  //*********************************************************************************************

  // EYE LEFT - size and border radius
  int eyeLwidthDefault = 36;
  int eyeLheightDefault = 36;
  int eyeLwidthCurrent = eyeLwidthDefault;
  int eyeLheightCurrent = 1; // start with closed eye, otherwise set to eyeLheightDefault
  int eyeLwidthNext = eyeLwidthDefault;
  int eyeLheightNext = eyeLheightDefault;
  int eyeLheightOffset = 0;
  // Border Radius
  byte eyeLborderRadiusDefault = 8;
  byte eyeLborderRadiusCurrent = eyeLborderRadiusDefault;
  byte eyeLborderRadiusNext = eyeLborderRadiusDefault;

  // EYE RIGHT - size and border radius
  int eyeRwidthDefault = eyeLwidthDefault;
  int eyeRheightDefault = eyeLheightDefault;
  int eyeRwidthCurrent = eyeRwidthDefault;
  int eyeRheightCurrent = 1; // start with closed eye, otherwise set to eyeRheightDefault
  int eyeRwidthNext = eyeRwidthDefault;
  int eyeRheightNext = eyeRheightDefault;
  int eyeRheightOffset = 0;
  // Border Radius
  byte eyeRborderRadiusDefault = 8;
  byte eyeRborderRadiusCurrent = eyeRborderRadiusDefault;
  byte eyeRborderRadiusNext = eyeRborderRadiusDefault;

  // EYE LEFT - Coordinates
  int eyeLxDefault;
  int eyeLyDefault;
  int eyeLx;
  int eyeLy;
  int eyeLxNext;
  int eyeLyNext;

  // EYE RIGHT - Coordinates
  int eyeRxDefault;
  int eyeRyDefault;
  int eyeRx;
  int eyeRy;
  int eyeRxNext;
  int eyeRyNext;

  // BOTH EYES
  // Eyelid top size
  byte eyelidsHeightMax = eyeLheightDefault / 2; // top eyelids max height
  byte eyelidsTiredHeight = 0;
  byte eyelidsTiredHeightNext = eyelidsTiredHeight;
  byte eyelidsAngryHeight = 0;
  byte eyelidsAngryHeightNext = eyelidsAngryHeight;
  // Bottom happy eyelids offset
  byte eyelidsHappyBottomOffsetMax = (eyeLheightDefault / 2) + 3;
  byte eyelidsHappyBottomOffset = 0;
  byte eyelidsHappyBottomOffsetNext = 0;
  // Space between eyes
  int spaceBetweenDefault = 10;
  int spaceBetweenCurrent = spaceBetweenDefault;
  int spaceBetweenNext = 10;

  //*********************************************************************************************
  //  Macro Animations
  //*********************************************************************************************

  // Animation - horizontal flicker/shiver
  bool hFlicker = 0;
  bool hFlickerAlternate = 0;
  byte hFlickerAmplitude = 2;

  // Animation - vertical flicker/shiver
  bool vFlicker = 0;
  bool vFlickerAlternate = 0;
  byte vFlickerAmplitude = 10;

  // Animation - auto blinking
  bool autoblinker = 0;           // activate auto blink animation
  int blinkInterval = 1;          // basic interval between each blink in full seconds
  int blinkIntervalVariation = 4; // interval variaton range in full seconds, random number inside of given range will be add to the basic blinkInterval, set to 0 for no variation
  unsigned long blinktimer = 0;   // for organising eyeblink timing

  // Animation - idle mode: eyes looking in random directions
  bool idle = 0;
  int idleInterval = 1;                 // basic interval between each eye repositioning in full seconds
  int idleIntervalVariation = 3;        // interval variaton range in full seconds, random number inside of given range will be add to the basic idleInterval, set to 0 for no variation
  unsigned long idleAnimationTimer = 0; // for organising eyeblink timing

  // Animation - eyes confused: eyes shaking left and right
  bool confused = 0;
  unsigned long confusedAnimationTimer = 0;
  int confusedAnimationDuration = 500;
  bool confusedToggle = 1;

  // Animation - eyes laughing: eyes shaking up and down
  bool laugh = 0;
  unsigned long laughAnimationTimer = 0;
  int laughAnimationDuration = 500;
  bool laughToggle = 1;

  // Animation - sweat on the forehead
  bool sweat = 0;
  byte sweatBorderradius = 3;

  // Sweat drop 1
  int sweat1XPosInitial = 2;
  int sweat1XPos;
  float sweat1YPos = 2;
  int sweat1YPosMax;
  float sweat1Height = 2;
  float sweat1Width = 1;

  // Sweat drop 2
  int sweat2XPosInitial = 2;
  int sweat2XPos;
  float sweat2YPos = 2;
  int sweat2YPosMax;
  float sweat2Height = 2;
  float sweat2Width = 1;

  // Sweat drop 3
  int sweat3XPosInitial = 2;
  int sweat3XPos;
  float sweat3YPos = 2;
  int sweat3YPosMax;
  float sweat3Height = 2;
  float sweat3Width = 1;

  //*********************************************************************************************
  //  GENERAL METHODS
  //*********************************************************************************************

  // Startup RoboEyes with defined screen-width, screen-height and max. frames per second
  void begin(int width, int height, byte frameRate);

  void update();

  //*********************************************************************************************
  //  SETTERS METHODS
  //*********************************************************************************************

  // Calculate frame interval based on defined frameRate
  void setFramerate(byte fps);

  // Set color values
  void setDisplayColors(uint8_t background, uint8_t main);

  void setWidth(byte leftEye, byte rightEye);

  void setHeight(byte leftEye, byte rightEye);

  // Set border radius for left and right eye
  void setBorderradius(byte leftEye, byte rightEye);

  // Set space between the eyes, can also be negative
  void setSpacebetween(int space);

  // Set mood expression
  void setMood(unsigned char mood);

  // Set predefined position
  void setPosition(unsigned char position);

  // Set automated eye blinking, minimal blink interval in full seconds and blink interval variation range in full seconds
  void setAutoblinker(bool active, int interval, int variation);
  void setAutoblinker(bool active);

  // Set idle mode - automated eye repositioning, minimal time interval in full seconds and time interval variation range in full seconds
  void setIdleMode(bool active, int interval, int variation);
  void setIdleMode(bool active);

  // Set curious mode - the respectively outer eye gets larger when looking left or right
  void setCuriosity(bool curiousBit);

  // Set cyclops mode - show only one eye
  void setCyclops(bool cyclopsBit);

  // Set horizontal flickering (displacing eyes left/right)
  void setHFlicker(bool flickerBit, byte Amplitude);
  void setHFlicker(bool flickerBit);

  // Set vertical flickering (displacing eyes up/down)
  void setVFlicker(bool flickerBit, byte Amplitude);
  void setVFlicker(bool flickerBit);

  void setSweat(bool sweatBit);

  //*********************************************************************************************
  //  GETTERS METHODS
  //*********************************************************************************************

  // Returns the max x position for left eye
  int getScreenConstraint_X();

  // Returns the max y position for left eye
  int getScreenConstraint_Y();

  //*********************************************************************************************
  //  BASIC ANIMATION METHODS
  //*********************************************************************************************

  // BLINKING FOR BOTH EYES AT ONCE
  // Close both eyes
  void close();

  // Open both eyes
  void open();

  // Trigger eyeblink animation
  void blink();

  // BLINKING FOR SINGLE EYES, CONTROL EACH EYE SEPARATELY
  // Close eye(s)
  void close(bool left, bool right);

  // Open eye(s)
  void open(bool left, bool right);

  // Trigger eyeblink(s) animation
  void blink(bool left, bool right);

  //*********************************************************************************************
  //  MACRO ANIMATION METHODS
  //*********************************************************************************************

  // Play confused animation - one shot animation of eyes shaking left and right
  void anim_confused();

  // Play laugh animation - one shot animation of eyes shaking up and down
  void anim_laugh();

  //*********************************************************************************************
  //  PRE-CALCULATIONS AND ACTUAL DRAWINGS
  //*********************************************************************************************

  void drawEyes();

}; // end of class roboEyes

#endif