// ****************************************************************************
// IronManSuit application
//
// Read buttons, toggle LEDs, play sounds, move servos, measure battery, sleep if possible
// ****************************************************************************
#ifndef SUITCONTROL_H
#define SUITCONTROL_H



// ****************************************************************************
// Suit settings
// ****************************************************************************
// Period to check button for release, after it was pressed.
#define SUIT_BUTTON_CHECK_MS        100UL 

// If button is pressed longer than this time - that means long click. Release moment does not matter.
#define SUIT_BUTTON_LONG_MS         1500UL


// PWM for servo
#define SUIT_SERVO_MIN              1000UL
#define SUIT_SERVO_MAX              2000UL





// ****************************************************************************
// Event processing
// ****************************************************************************

// Read buttons state and process
void checkPressed();
void checkReleased();
void processButtonEvent();

// Change LEDs state
void processLeds();

// Servo control with PWM
void processServos();

// Change effects state
void processEffects();

// Battery voltage measurement
void processBatteryControl();

// Check state and go to the sleep mode
void processSleep();




#endif // SUITCONTROL_H