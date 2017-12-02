<<<<<<< HEAD
//Globals
//***********************
bool dangerFront = false;
bool dangerBack = false;

uint16_t currentM1 = 0;
uint16_t currentM2 = 0;
uint16_t currentM3 = 0;
uint16_t currentM4 = 0;
uint16_t currentM5 = 0;
uint16_t currentM6 = 0;

bool dangerOverride = false;

bool dangerM1 = false;
bool dangerM2 = false;
bool dangerM3 = false;
bool dangerM4 = false;
bool dangerM5 = false;
bool dangerM6 = false;   
=======

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
>>>>>>> eb9166e364bc91493cbb9364c7658d9073abf511

bool FrontRight = false;
bool FrontLeft = false;
bool BackRight = false;
bool BackLeft = false;
//***********************

<<<<<<< HEAD
int M1_thresh = 1;
int M2_thresh = 2;
int M3_thresh = 3;
int M4_thresh = 4;
int M5_thresh = 5;
int M6_thresh = 6;              // undertermined values; requires testing
=======
int M1_thresh;
int M2_thresh;
int M3_thresh;
int M4_thresh;
int M5_thresh;
int M6_thresh;                  // undertermined values; requires testing
>>>>>>> eb9166e364bc91493cbb9364c7658d9073abf511

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

<<<<<<< HEAD
void loop() {

  if( dangerOverride = true){
    bool dangerM1 = false;
    bool dangerM2 = false;
    bool dangerM3 = false;
    bool dangerM4 = false;
    bool dangerM5 = false;
    bool dangerM6 = false;   

    bool dangerFront = false;
    bool dangerBack = false;
  }

  else {

  if( mode = MODE_ROVER){

=======
void loop() 
{
  if( mode = 0)
      {
>>>>>>> eb9166e364bc91493cbb9364c7658d9073abf511
        buttonStateFront_R = digitalRead(BUTTON_PIN_FRONT_R);
        buttonStateBack_R = digitalRead(BUTTON_PIN_BACK_R);
        buttonStateFront_L = digitalRead(BUTTON_PIN_FRONT_L);
        buttonStateBack_L = digitalRead(BUTTON_PIN_BACK_L);         

<<<<<<< HEAD
        if (buttonStateFront_R || buttonStateFront_L == HIGH){
          dangerFront = true;
        }
        else {
          dangerFront = false;
        }
  
        if (buttonStateBack_R || buttonStateBack_L == HIGH){
          dangerBack = true;
        }

        else{
          dangerBack = false;
        }
  }
  else if( mode = MODE_ARM){

      for(int i; i<50; i++){

        if(currentM1 > M1_thresh){
          dangerM1 = true;
        }

        else if(currentM2 > M2_thresh){
          dangerM2 = true;
        }

        else if(currentM3 > M3_thresh){
          dangerM3 = true;
        }

        else if(currentM4 > M4_thresh){
          dangerM4 = true;
        }

        else if(currentM5 > M5_thresh){
          dangerM5 = true;
        }

        else if(currentM6 > M6_thresh){
          dangerM6 = true;
        }

        if(i = 50){
          roboclaw1.ReadCurrents(address, &currentM1, &currentM2);
          roboclaw2.ReadCurrents(address, &currentM3, &currentM4);
          roboclaw3.ReadCurrents(address, &currentM5, &currentM6);
          i = 0;
        }
      }
    }
  }
}
=======
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
>>>>>>> eb9166e364bc91493cbb9364c7658d9073abf511
