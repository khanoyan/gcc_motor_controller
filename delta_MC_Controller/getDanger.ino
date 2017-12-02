
//Globals
//***********************
bool dangerFront = false;    
bool dangerBack = false;

int currentM1 = 0;
int currentM2 = 0;
int currentM3 = 0;
int currentM4 = 0;
int currentM5 = 0;
int currentM6 = 0;


bool dangerMotor1 = false;
bool dangerMotor2 = false;
bool dangerMotor3 = false;
bool dangerMotor4 = false;
bool dangerMotor5 = false;
bool dangerMotor6 = false;   

bool FrontRight = false;
bool FrontLeft = false;
bool BackRight = false;
bool BackLeft = false;
//***********************

int M1_thresh;
int M2_thresh;
int M3_thresh;
int M4_thresh;
int M5_thresh;
int M6_thresh;                  // undertermined values; requires testing

int BUTTON_PIN_FRONT_R = 43;
int BUTTON_PIN_FRONT_L = 41;
int BUTTON_PIN_BACK_R = 47;
int BUTTON_PIN_BACK_L = 45;     // arbitrary pin locations

int buttonStateFront_R = 0;
int buttonStateFront_L = 0;
int buttonStateBack_R = 0;
int buttonStateBack_L = 0;

void setup() 
{
  pinMode(BUTTON_PIN_FRONT_R, INPUT);
  pinMode(BUTTON_PIN_FRONT_L, INPUT);
  pinMode(BUTTON_PIN_BACK_R, INPUT);
  pinMode(BUTTON_PIN_BACK_L, INPUT);
}

void loop() 
{
  if( mode = 0)
      {
        buttonStateFront_R = digitalRead(BUTTON_PIN_FRONT_R);
        buttonStateBack_R = digitalRead(BUTTON_PIN_BACK_R);
        buttonStateFront_L = digitalRead(BUTTON_PIN_FRONT_L);
        buttonStateBack_L = digitalRead(BUTTON_PIN_BACK_L);         

        if (buttonStateFront_R || buttonStateFront_L == HIGH)
        {
            dangerFront = true;
        }
        else 
        {
            dangerFront = false;
        }
  
        if (buttonStateBack_R || buttonStateBack_L == HIGH)
        {
            dangerBack = true;
        }
        else
        {
            dangerBack = false;
        }
      }

  if( mode = 1) //arm
    {
      if (
    
    }
  
}
