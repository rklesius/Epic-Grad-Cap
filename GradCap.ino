
#include <MD_Parola.h>
#include <MD_MAX72xx.h>
#include <SPI.h>
#include <Servo.h>

//These pins are the only pins that will work for the LED display
#define HARDWARE_TYPE MD_MAX72XX::FC16_HW
#define MAX_DEVICES 4
#define CLK_PIN   13
#define DATA_PIN  11
#define CS_PIN    10

//Button inputs
#define TEXT_BUTTON 2
#define TASSLE_BUTTON 3

//servo
#define SERVO 4

MD_Parola P = MD_Parola(HARDWARE_TYPE, DATA_PIN, CLK_PIN, CS_PIN, MAX_DEVICES);
Servo tassleMover; 

//Parola display messages
char graduatingMessage[] = {"Hi mom, I'm graduating!"};
char staticMessage[] = {"USF!"};
char graduatedMessage[] = {"Yay!  I graduated!"};  
char *currentScrollingMessage = graduatingMessage;

//Button variables
int textButtonStatus;  //start off with an unpressed button
int prevTextStatus = 0;
int tassleButtonStatus;
int prevTassleStatus = 0;

//States the Arduino can be in
bool scrollState = true;
bool graduationState = false;

void setup(void)
{
  P.begin();
  P.displayText(graduatingMessage, PA_CENTER, 50, 20, PA_SCROLL_LEFT, PA_SCROLL_LEFT);
  pinMode(TEXT_BUTTON, INPUT);
  pinMode(TASSLE_BUTTON, INPUT);
  tassleMover.attach(SERVO);
  tassleMover.write(45);  //move servo to 45 degrees
}

void loop(void)
{
  if (P.displayAnimate())
  {
    P.displayReset();
  }

  //Change text
  textButtonStatus = digitalRead(TEXT_BUTTON);

  //look for a change from LOW to HIGH instead of a button press of HIGH
  if (textButtonStatus == 1 && prevTextStatus == 0)
  {
    if(scrollState)  //if in the scroll state, enter the static state
    {
      P.displayClear();
      P.displayText(staticMessage, PA_CENTER, 0, 0, PA_PRINT, PA_NO_EFFECT);
      scrollState = false;
    }
    else
    {
      P.displayClear();
      P.displayText(currentScrollingMessage, PA_CENTER, 50, 20, PA_SCROLL_LEFT, PA_SCROLL_LEFT);
      scrollState = true;
    }
  }
  tassleButtonStatus = digitalRead(TASSLE_BUTTON);

  //Change Tassle
  if (tassleButtonStatus == 1 && prevTassleStatus == 0)
  {
    if(tassleMover.read() == 45)  //servo on the right, pre-graduation
    {
      tassleMover.write(135);  //graduate
      currentScrollingMessage = graduatedMessage;  //set the scrolling message to graduated
    }

    else 
    {
      tassleMover.write(45);
      currentScrollingMessage = graduatingMessage;  //revert the scrolling message
    }

    //if scrolling, write the message that was changed here
    if(scrollState)
    {
      P.displayClear();
      P.displayText(currentScrollingMessage, PA_CENTER, 50, 20, PA_SCROLL_LEFT, PA_SCROLL_LEFT);
    }
  }

  //set the previous state buttons
  prevTassleStatus = tassleButtonStatus;
  prevTextStatus = textButtonStatus;
}

