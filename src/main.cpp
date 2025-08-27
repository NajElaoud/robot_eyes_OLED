//***********************************************************************************************
//  This example shows how to use the basic methods of the FluxGarage Robo Eyes library.
//
//  Hardware: You'll need a breadboard, an arduino nano r3, an I2C oled display with 1306
//  or 1309 chip and some jumper wires.
//
//  Published in September 2024 by Dennis Hoelscher, FluxGarage
//  www.youtube.com/@FluxGarage
//  www.fluxgarage.com
//
//***********************************************************************************************
#include <Arduino.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <FluxGarage_RoboEyes.hpp>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET -1 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

roboEyes roboeyes; // create eyes

// EVENT TIMER
unsigned long eventTimer; // will save the timestamps
bool event1wasPlayed = 0; // flag variables
bool event2wasPlayed = 0;
bool event3wasPlayed = 0;

// Demo variables
unsigned long demoStartTime = 0;
int demoPhase = 0;
const int phaseDuration = 3000; // 3 seconds per phase
unsigned long lastModeChange = 0;
int currentMode = 0;
const int modeCount = 6;

// Animation
void runDemoPhase(int phase, unsigned long timeInPhase);

// Main drawing function - CALL THIS IN YOUR LOOP
void drawComplexEyes();
void phase0_HumanEyes();
void phase1_HappyEyes();
void phase2_AngryEyes();
void phase3_TiredEyes();
void phase4_RobotEyes();
void phase5_AlienEyes();
void phase6_LookAround();
void phase7_GazeTracking(unsigned long time);
void phase8_BlinkingDemo();
void phase9_ConfusedDemo();
void phase10_LaughDemo();
void phase11_SweatDemo();
void phase12_CuriousDemo();
void phase13_CyclopsDemo();
void phase14_SizeChangeDemo(unsigned long time);
void phase15_CombinationDemo(unsigned long time);

// Eye type control functions
void setHumanEyes();
void setHappyEyes();
void setAngryEyes();
void setTiredEyes();
void setRobotEyes();
void setAlienEyes();

// Gaze control functions
void lookLeft();
void lookRight();
void lookCenter();
void lookAtPosition(int x, int y);

// Animation control functions
void blinkEyes();
void setConfusedMode(bool enable);
void setLaughMode(bool enable);
void setSweatMode(bool enable);

// Utility functions
void changeEyeMode(int mode);
void drawEyeDetails(int x, int y, int width, int height, bool isLeftEye);
void drawHumanEye(int x, int y, int width, int height, int centerX, int centerY);
void drawCatEye(int x, int y, int width, int height, int centerX, int centerY);
void drawRobotEye(int x, int y, int width, int height, int centerX, int centerY);
void drawAlienEye(int x, int y, int width, int height, int centerX, int centerY);
void drawEyelidDetails(int x, int y, int width, int height);
void drawTiredDetails(int x, int y, int width, int height);
void drawSparkleEffects(int centerX, int centerY, int width);

void setup()
{
  // OLED Display
  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C))
  { // Address 0x3C or 0x3D
    Serial.println(F("SSD1306 allocation failed"));
    for (;;)
      ; // Don't proceed, loop forever
  }

  // Startup robo eyes
  roboeyes.begin(SCREEN_WIDTH, SCREEN_HEIGHT, 100); // screen-width, screen-height, max framerate - 60-100fps are good for smooth animations
  roboeyes.setDisplayColors(SSD1306_BLACK, SSD1306_WHITE);
  roboeyes.setAutoblinker(true, 2, 1);

  Serial.println("Full RoboEyes Demo Started");
  demoStartTime = millis();

} // end of setup

void loop()
{
  unsigned long currentTime = millis();
  unsigned long phaseTime = currentTime - demoStartTime;

  // Cycle through demo phases
  if (phaseTime > phaseDuration)
  {
    demoPhase = (demoPhase + 1) % 16; // 16 different phases
    demoStartTime = currentTime;
    phaseTime = 0;

    Serial.print("Starting phase: ");
    Serial.println(demoPhase);
  }

  // Execute current demo phase
  runDemoPhase(demoPhase, phaseTime);

  // Always draw the eyes
  drawComplexEyes();

  delay(10);
} // end of main loop

void runDemoPhase(int phase, unsigned long timeInPhase)
{
  switch (phase)
  {
  case 0:
    phase0_HumanEyes();
    break;
  case 1:
    phase1_HappyEyes();
    break;
  case 2:
    phase2_AngryEyes();
    break;
  case 3:
    phase3_TiredEyes();
    break;
  case 4:
    phase4_RobotEyes();
    break;
  case 5:
    phase5_AlienEyes();
    break;
  case 6:
    phase6_LookAround();
    break;
  case 7:
    phase7_GazeTracking(timeInPhase);
    break;
  case 8:
    phase8_BlinkingDemo();
    break;
  case 9:
    phase9_ConfusedDemo();
    break;
  case 10:
    phase10_LaughDemo();
    break;
  case 11:
    phase11_SweatDemo();
    break;
  case 12:
    phase12_CuriousDemo();
    break;
  case 13:
    phase13_CyclopsDemo();
    break;
  case 14:
    phase14_SizeChangeDemo(timeInPhase);
    break;
  case 15:
    phase15_CombinationDemo(timeInPhase);
    break;
  }
}

// === DEMO PHASES ===

void phase0_HumanEyes()
{
  setHumanEyes();
  lookCenter();
  Serial.println("Phase 0: Default Human Eyes");
}

void phase1_HappyEyes()
{
  setHappyEyes();
  lookCenter();
  Serial.println("Phase 1: Happy Eyes with Sparkles");
}

void phase2_AngryEyes()
{
  setAngryEyes();
  lookCenter();
  Serial.println("Phase 2: Angry Cat Eyes");
}

void phase3_TiredEyes()
{
  setTiredEyes();
  lookCenter();
  Serial.println("Phase 3: Tired Eyes with Veins");
}

void phase4_RobotEyes()
{
  setRobotEyes();
  lookCenter();
  Serial.println("Phase 4: Robot Eyes with Grid");
}

void phase5_AlienEyes()
{
  setAlienEyes();
  lookCenter();
  Serial.println("Phase 5: Alien Eyes with Multiple Pupils");
}

void phase6_LookAround()
{
  setHumanEyes();

  // Cycle through different gaze directions
  unsigned long subPhase = (millis() / 1000) % 4;
  switch (subPhase)
  {
  case 0:
    lookLeft();
    Serial.println("Looking Left");
    break;
  case 1:
    lookRight();
    Serial.println("Looking Right");
    break;
  case 2:
    lookCenter();
    Serial.println("Looking Center");
    break;
  case 3:
    lookAtPosition(30, 10);
    Serial.println("Looking Up-Left");
    break;
  }
}

void phase7_GazeTracking(unsigned long time)
{
  setHumanEyes();

  // Animate gaze in a circular pattern
  float angle = time * 0.002;
  int gazeX = 40 + cos(angle) * 30;
  int gazeY = 20 + sin(angle) * 20;
  lookAtPosition(gazeX, gazeY);

  Serial.println("Gaze Tracking - Circular Motion");
}

void phase8_BlinkingDemo()
{
  setHumanEyes();
  lookCenter();

  // Trigger blinks at different intervals
  if (millis() % 1500 < 20)
  {
    blinkEyes();
    Serial.println("Blink!");
  }
}

void phase9_ConfusedDemo()
{
  setHumanEyes();
  lookCenter();
  setConfusedMode(true);
  Serial.println("Confused Mode - Horizontal Shaking");
}

void phase10_LaughDemo()
{
  setHappyEyes();
  lookCenter();
  setLaughMode(true);
  Serial.println("Laugh Mode - Vertical Bouncing");
}

void phase11_SweatDemo()
{
  setTiredEyes();
  lookCenter();
  setSweatMode(true);
  Serial.println("Sweat Mode - Animated Sweat Drops");
}

void phase12_CuriousDemo()
{
  roboeyes.setCuriosity(true);
  setHumanEyes();

  // Look side to side to show curious effect
  unsigned long subTime = millis() % 2000;
  if (subTime < 1000)
  {
    lookLeft();
    Serial.println("Curious - Looking Left (Left eye enlarged)");
  }
  else
  {
    lookRight();
    Serial.println("Curious - Looking Right (Right eye enlarged)");
  }
}

void phase13_CyclopsDemo()
{
  setAlienEyes(); // Alien works well for cyclops
  roboeyes.setCyclops(true);
  lookCenter();

  // Make cyclops eye move slightly
  int wobble = sin(millis() * 0.005) * 3;
  lookAtPosition(40 + wobble, 20);
  Serial.println("Cyclops Mode - Single Moving Eye");
}

void phase14_SizeChangeDemo(unsigned long time)
{
  setHumanEyes();
  lookCenter();

  // Animate eye size
  float sizeFactor = 0.7 + 0.3 * sin(time * 0.002);
  roboeyes.setWidth(30 * sizeFactor, 30 * sizeFactor);
  roboeyes.setHeight(30 * sizeFactor, 30 * sizeFactor);

  Serial.println("Size Changing - Pupil Dilation Effect");
}

void phase15_CombinationDemo(unsigned long time)
{
  // Combine multiple effects
  roboeyes.setCuriosity(true);
  setHappyEyes();
  setSweatMode(true);

  // Animated gaze
  float angle = time * 0.0015;
  int gazeX = 40 + cos(angle) * 25;
  int gazeY = 20 + sin(angle * 1.5) * 15;
  lookAtPosition(gazeX, gazeY);

  // Occasional blinks
  if (millis() % 2000 < 20)
  {
    blinkEyes();
  }

  Serial.println("Combination Demo - Happy + Curious + Sweat + Moving");
}

// MAIN DRAWING FUNCTION - CALL THIS IN LOOP
void drawComplexEyes()
{
  roboeyes.update(); // Update basic eye animations

  // Add complex details on top
  drawEyeDetails(roboeyes.eyeLx, roboeyes.eyeLy, roboeyes.eyeLwidthCurrent,
                 roboeyes.eyeLheightCurrent, true);

  if (!roboeyes.cyclops)
  {
    drawEyeDetails(roboeyes.eyeRx, roboeyes.eyeRy, roboeyes.eyeRwidthCurrent,
                   roboeyes.eyeRheightCurrent, false);
  }
}

// EYE TYPE CONTROL FUNCTIONS
void setHumanEyes() { changeEyeMode(0); }
void setHappyEyes() { changeEyeMode(1); }
void setAngryEyes() { changeEyeMode(2); }
void setTiredEyes() { changeEyeMode(3); }
void setRobotEyes() { changeEyeMode(4); }
void setAlienEyes() { changeEyeMode(5); }

// GAZE CONTROL FUNCTIONS
void lookLeft()
{
  roboeyes.eyeLxNext = 10;
  roboeyes.eyeRxNext = roboeyes.eyeLxNext + roboeyes.eyeLwidthCurrent + roboeyes.spaceBetweenCurrent;
}

void lookRight()
{
  roboeyes.eyeLxNext = roboeyes.getScreenConstraint_X() - 10;
  roboeyes.eyeRxNext = roboeyes.eyeLxNext + roboeyes.eyeLwidthCurrent + roboeyes.spaceBetweenCurrent;
}

void lookCenter()
{
  roboeyes.eyeLxNext = roboeyes.getScreenConstraint_X() / 2;
  roboeyes.eyeRxNext = roboeyes.eyeLxNext + roboeyes.eyeLwidthCurrent + roboeyes.spaceBetweenCurrent;
}

void lookAtPosition(int x, int y)
{
  roboeyes.eyeLxNext = constrain(x, 0, roboeyes.getScreenConstraint_X());
  roboeyes.eyeLyNext = constrain(y, 0, roboeyes.getScreenConstraint_Y());
  roboeyes.eyeRxNext = roboeyes.eyeLxNext + roboeyes.eyeLwidthCurrent + roboeyes.spaceBetweenCurrent;
  roboeyes.eyeRyNext = roboeyes.eyeLyNext;
}

// ANIMATION CONTROL FUNCTIONS
void blinkEyes() { roboeyes.blink(); }
void setConfusedMode(bool enable)
{
  if (enable)
    roboeyes.anim_confused();
}
void setLaughMode(bool enable)
{
  if (enable)
    roboeyes.anim_laugh();
}
void setSweatMode(bool enable) { roboeyes.setSweat(enable); }

// UTILITY FUNCTIONS
void changeEyeMode(int mode)
{
  currentMode = mode;

  roboeyes.setMood(DEFAULT);
  roboeyes.setCuriosity(false);
  roboeyes.setSweat(false);
  roboeyes.setCyclops(false);

  switch (mode)
  {
  case 0:
    roboeyes.setMood(DEFAULT);
    break;
  case 1:
    roboeyes.setMood(HAPPY);
    break;
  case 2:
    roboeyes.setMood(ANGRY);
    break;
  case 3:
    roboeyes.setMood(TIRED);
    roboeyes.setSweat(true);
    break;
  case 4:
    roboeyes.setCuriosity(true);
    roboeyes.eyeLxNext = 10;
    break;
  case 5:
    roboeyes.setCyclops(true);
    break;
  }
}

void drawEyeDetails(int x, int y, int width, int height, bool isLeftEye)
{
  if (height <= 1)
    return;

  int centerX = x + width / 2;
  int centerY = y + height / 2;

  switch (currentMode)
  {
  case 0:
    drawHumanEye(x, y, width, height, centerX, centerY);
    break;
  case 1:
    drawHumanEye(x, y, width, height, centerX, centerY);
    drawSparkleEffects(centerX, centerY, width);
    break;
  case 2:
    drawCatEye(x, y, width, height, centerX, centerY);
    break;
  case 3:
    drawHumanEye(x, y, width, height, centerX, centerY);
    drawTiredDetails(x, y, width, height);
    break;
  case 4:
    drawRobotEye(x, y, width, height, centerX, centerY);
    break;
  case 5:
    drawAlienEye(x, y, width, height, centerX, centerY);
    break;
  }
}

void drawHumanEye(int x, int y, int width, int height, int centerX, int centerY)
{
  int irisSize = min(width, height) * 0.6;
  int pupilSize = irisSize * 0.4;

  display.fillCircle(centerX, centerY, irisSize / 2, SSD1306_WHITE);
  display.fillCircle(centerX, centerY, pupilSize / 2, SSD1306_BLACK);

  int highlightX = centerX + width * 0.15;
  int highlightY = centerY - height * 0.15;
  display.fillCircle(highlightX, highlightY, pupilSize * 0.4, SSD1306_WHITE);

  drawEyelidDetails(x, y, width, height);
}

void drawCatEye(int x, int y, int width, int height, int centerX, int centerY)
{
  int irisSize = min(width, height) * 0.7;
  display.fillCircle(centerX, centerY, irisSize / 2, SSD1306_WHITE);

  int pupilWidth = irisSize * 0.15;
  int pupilHeight = irisSize * 0.6;
  display.fillRoundRect(centerX - pupilWidth / 2, centerY - pupilHeight / 2,
                        pupilWidth, pupilHeight, pupilWidth / 2, SSD1306_BLACK);
}

void drawRobotEye(int x, int y, int width, int height, int centerX, int centerY)
{
  int eyeSize = min(width, height) * 0.8;
  display.fillCircle(centerX, centerY, eyeSize / 2, SSD1306_WHITE);
  display.fillCircle(centerX, centerY, eyeSize * 0.3, SSD1306_BLACK);

  for (int i = -1; i <= 1; i++)
  {
    for (int j = -1; j <= 1; j++)
    {
      if (i != 0 || j != 0)
      {
        display.drawPixel(centerX + i * 3, centerY + j * 3, SSD1306_WHITE);
      }
    }
  }
}

void drawAlienEye(int x, int y, int width, int height, int centerX, int centerY)
{
  int eyeSize = min(width, height) * 0.9;
  display.fillCircle(centerX, centerY, eyeSize / 2, SSD1306_WHITE);

  display.fillCircle(centerX - eyeSize * 0.2, centerY, eyeSize * 0.15, SSD1306_BLACK);
  display.fillCircle(centerX + eyeSize * 0.2, centerY, eyeSize * 0.15, SSD1306_BLACK);
  display.fillCircle(centerX, centerY - eyeSize * 0.2, eyeSize * 0.1, SSD1306_BLACK);
}

void drawEyelidDetails(int x, int y, int width, int height)
{
  display.drawLine(x + 3, y + 1, x + width - 3, y + 1, SSD1306_WHITE);
  if (height > 15)
  {
    display.drawLine(x + 3, y + height - 2, x + width - 3, y + height - 2, SSD1306_WHITE);
  }
}

void drawTiredDetails(int x, int y, int width, int height)
{
  for (int i = 0; i < 3; i++)
  {
    int veinY = y + height / 4 + i * height / 6;
    display.drawLine(x + 2, veinY, x + width - 2, veinY, SSD1306_WHITE);
  }
}

void drawSparkleEffects(int centerX, int centerY, int width)
{
  int sparkleSize = width * 0.1;
  for (int i = 0; i < 4; i++)
  {
    float angle = millis() / 1000.0 + i * PI / 2;
    int sparkleX = centerX + cos(angle) * width * 0.7;
    int sparkleY = centerY + sin(angle) * width * 0.7;
    display.fillCircle(sparkleX, sparkleY, sparkleSize, SSD1306_WHITE);
  }
}