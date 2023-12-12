#include <IRremote.h>// IR remote library
/*
   ***********Left Motor***********
*/

// IN 1
#define LM_IN1    2
// IN 2
#define LM_IN2    3
/*
   ************Right Motor************
*/
// IN 3
#define RM_IN3    4
// IN 4
#define RM_IN4    5

#define W1 6
#define W2 7
/*
   ********************IR Receiver**********************
*/
# define RECV_PIN 10// Pin to which IR reciever is connected

IRrecv irrecv(RECV_PIN);// Pass the pin number to the function

decode_results results;// variable results stores the decoded hex values

/*
 ************HEX code of all the buttons used in the project**************
 */
#define FWD       0x1FED827 // go forward(2)
#define LFT       0x1FE30CF // go left(4)
#define RGT       0x1FE708F // go right(6)
#define BWD       0x1FEF00F // go backward(8)
#define STOP      0x1FEB04F // stop(0)
#define RPEAT     0x1FE10EF // repeat the stored sequence of movement from the temporary memory(automatically stores) (REPT)
#define DEL       0x1FEE01F // delete the stored sequence of movement from temporary memory(EQ)
#define RESET     0x1FE48B7 // Resets the Arduino Board(RED)
#define UP        0x1FE00FF
#define DWN       0x1FE9867

                  


/*
 ************Global Variables and Arrays**************
 */
 
unsigned long int value = 0; // stores the incoming hex value
byte seq = 0; //stores the current number of executed sequences
byte seq_Array[50];// array to store the movement sequence in terms of integers(1 for FWD, 2 for LEFT and so on..)

//counter for counting the number of times program pass through a movement function(fwd, lft etc.)
int fwd_Counter = -1;
int lft_Counter = -1;
int rgt_Counter = -1;
int bwd_Counter = -1;
int stp_Counter = -1;
int Up_Counter = -1;
int Dwn_Counter = -1;

//global "current time" variables for different movement functions(fwd, lft etc.)
unsigned long int current_Time0 = 0;// for FWD movement
unsigned long int current_Time1 = 0;// for LEFT movement
unsigned long int current_Time2 = 0;// for RIGHT movement
unsigned long int current_Time3 = 0;// for BWD movement
unsigned long int current_Time4 = 0;// for STOP
unsigned long int current_Time5 = 0;// for UP
unsigned long int current_Time6 = 0;// for DWN

//total time spend by the pgm in executing the movement(fwd, lft etc.) for a particular movement counter
unsigned long int total_Fwd_Time[10];
unsigned long int total_Lft_Time[10];
unsigned long int total_Rgt_Time[10];
unsigned long int total_Bwd_Time[10];
unsigned long int total_Stp_Time[10];
unsigned long int total_Up_Time[10];
unsigned long int total_Dwn_Time[10];

/*
 ************Arduino Reset Pin**************
 */
#define RESET_PIN A0

void setup() {

  // set mode of the pins as output
  for (int i = 2; i <= 7; i++) {
    pinMode(i, OUTPUT);
  }

  // start serial communication
  Serial.begin(9600);
  // In case the interrupt driver crashes on setup, give a clue
  // to the user what's going on.
  Serial.println("Enabling IRin");
  irrecv.enableIRIn(); // Start the receiver
  Serial.println("Enabled IRin");

}

void loop() {
  if (irrecv.decode(&results)) {
    value = results.value;
    irrecv.resume(); // Receive the next value
    delay(200);
  }

  delay(100);
  check_Inst(value);
  value=0;

}
void check_Inst(long int value) {

  switch (value) {
    case FWD:
      go_Forward();
      delay(10);
      break;
    case LFT:
      go_Left();
      delay(10);
      break;
    case RGT:
      go_Right();
      delay(10);
      break;
    case BWD:
      go_Backward();
      delay(10);
      break;
    case STOP:
      go_Stop();
      delay(10);
      break;
      case UP:
      go_Up();
      delay(10);
      break;
      case DWN:
      go_Dwn();
      delay(10);
      break;
    case RPEAT:
      go_In_Seq();
      delay(10);
      break;
    case DEL:
      del_From_Local_Mem();
      delay(10);
      break;  
    case RESET:
      pinMode(RESET_PIN, OUTPUT);
      digitalWrite(RESET_PIN,HIGH);   
      break;
                
     default:
      value = 0;
  }
}
void go_Forward() {
  movement_Inst_Fwd();

  current_Time0 = millis();
  int i = seq_Array[(seq - 1)];
  switch (i) {
    case 2:
      // total time elaspsed since Left button is pressed including rest time 
      total_Lft_Time[lft_Counter + 1] = (current_Time0 - current_Time1);
      lft_Counter++;
      break;

    case 3:
      // total time elaspsed since Right button is pressed including rest time 
      total_Rgt_Time[rgt_Counter + 1] = (current_Time0 - current_Time2);
      rgt_Counter++;
      break;

    case 4:
      total_Bwd_Time[bwd_Counter + 1] = (current_Time0 - current_Time3);
      bwd_Counter++;
      break;

    case 5:
      total_Stp_Time[stp_Counter + 1] = (current_Time0 - current_Time4);
      stp_Counter++;
      break;
    case 6:
      total_Up_Time[Up_Counter + 1] = (current_Time0 - current_Time5);
      Up_Counter++;
      break;
    case 7:
      total_Dwn_Time[Dwn_Counter + 1] = (current_Time0 - current_Time6);
      Dwn_Counter++;
      break;
  }

  seq_Array[seq] = 1;
  seq++;
}

void go_Left() {
  movement_Inst_Lft();

  current_Time1 = millis();
  int i = seq_Array[(seq - 1)];
  switch (i) {
    case 1:
      total_Fwd_Time[fwd_Counter + 1] = (current_Time1 - current_Time0);
      fwd_Counter++;
      break;

    case 3:
      total_Rgt_Time[rgt_Counter + 1] = (current_Time1 - current_Time2);
      rgt_Counter++;
      break;

    case 4:
      total_Bwd_Time[bwd_Counter + 1] = (current_Time1 - current_Time3);
      bwd_Counter++;
      break;

    case 5:
      total_Stp_Time[stp_Counter + 1] = (current_Time1 - current_Time4);
      stp_Counter++;
      break;
       case 6:
      total_Up_Time[Up_Counter + 1] = (current_Time1 - current_Time5);
      Up_Counter++;
      break;
      case 7:
      total_Dwn_Time[Dwn_Counter + 1] = (current_Time1 - current_Time6);
      Dwn_Counter++;
      break;
  }

  seq_Array[seq] = 2;
  seq++;
}

void go_Right() {
  movement_Inst_Rgt();

  current_Time2 = millis();
  int i = seq_Array[(seq - 1)];
  switch (i) {
    case 1:
      total_Fwd_Time[fwd_Counter + 1] = (current_Time2 - current_Time0);
      fwd_Counter++;
      break;

    case 2:
      total_Lft_Time[lft_Counter + 1] = (current_Time2 - current_Time1);
      lft_Counter++;
      break;

    case 4:
      total_Bwd_Time[bwd_Counter + 1] = (current_Time2 - current_Time3);
      bwd_Counter++;
      break;

    case 5:
      total_Stp_Time[stp_Counter + 1] = (current_Time2 - current_Time4);
      stp_Counter++;
      break;
       case 6:
      total_Up_Time[Up_Counter + 1] = (current_Time2 - current_Time5);
      Up_Counter++;
      break;
      case 7:
      total_Dwn_Time[Dwn_Counter + 1] = (current_Time2 - current_Time6);
      Dwn_Counter++;
      break;
  }

  seq_Array[seq] = 3;
  seq++;
}

void go_Backward() {
  movement_Inst_Bwd();

  current_Time3 = millis();
  int i = seq_Array[(seq - 1)];
  switch (i) {
    case 1:
      total_Fwd_Time[fwd_Counter + 1] = (current_Time3 - current_Time0);
      fwd_Counter++;
      break;

    case 2:
      total_Lft_Time[lft_Counter + 1] = (current_Time3 - current_Time1);
      lft_Counter++;
      break;

    case 3:
      total_Rgt_Time[rgt_Counter + 1] = (current_Time3 - current_Time2);
      rgt_Counter++;
      break;

    case 5:
      total_Stp_Time[stp_Counter + 1] = (current_Time3 - current_Time4);
      stp_Counter++;
      break;
       case 6:
      total_Up_Time[Up_Counter + 1] = (current_Time3 - current_Time5);
      Up_Counter++;
      break;
      case 7:
      total_Dwn_Time[Dwn_Counter + 1] = (current_Time3 - current_Time6);
      Dwn_Counter++;
      break;
  }

  seq_Array[seq] = 4;
  seq++;
}

void go_Stop() {
  movement_Inst_Stp();

  current_Time4 = millis();
  int i = seq_Array[(seq - 1)];
  switch (i) {
    case 1:
      total_Fwd_Time[fwd_Counter + 1] = (current_Time4 - current_Time0);
      fwd_Counter++;
      break;

    case 2:
      total_Lft_Time[lft_Counter + 1] = (current_Time4 - current_Time1);
      lft_Counter++;
      break;

    case 3:
      total_Rgt_Time[rgt_Counter + 1] = (current_Time4 - current_Time2);
      rgt_Counter++;
      break;

    case 4:
      total_Bwd_Time[bwd_Counter + 1] = (current_Time4 - current_Time3);
      bwd_Counter++;
      break;
       case 6:
      total_Up_Time[Up_Counter + 1] = (current_Time4 - current_Time5);
      Up_Counter++;
      break;
      case 7:
      total_Dwn_Time[Dwn_Counter + 1] = (current_Time4 - current_Time6);
      Dwn_Counter++;
      break;
  }

  seq_Array[seq] = 5;
  seq++;
}


void go_Up() {
  movement_Inst_Up();

  current_Time5 = millis();
  int i = seq_Array[(seq - 1)];
  switch (i) {
    case 1:
      total_Fwd_Time[fwd_Counter + 1] = (current_Time5 - current_Time0);
      fwd_Counter++;
      break;

    case 2:
      total_Lft_Time[lft_Counter + 1] = (current_Time5 - current_Time1);
      lft_Counter++;
      break;

    case 3:
      total_Rgt_Time[rgt_Counter + 1] = (current_Time5 - current_Time2);
      rgt_Counter++;
      break;

    case 4:
      total_Bwd_Time[bwd_Counter + 1] = (current_Time5 - current_Time3);
      bwd_Counter++;
      break;
    case 5:
      total_Stp_Time[stp_Counter + 1] = (current_Time5 - current_Time4);
      stp_Counter++;
      break;
       case 6:
      total_Up_Time[Up_Counter + 1] = (current_Time5 - current_Time5);
      Up_Counter++;
      break;
    case 7:
      total_Dwn_Time[Dwn_Counter + 1] = (current_Time5 - current_Time6);
      Dwn_Counter++;
      break;
  }

  seq_Array[seq] = 6;
  seq++;
}
void go_Dwn() {
  movement_Inst_Dwn();

  current_Time6 = millis();
  int i = seq_Array[(seq - 1)];
  switch (i) {
    case 1:
      total_Fwd_Time[fwd_Counter + 1] = (current_Time6 - current_Time0);
      fwd_Counter++;
      break;

    case 2:
      total_Lft_Time[lft_Counter + 1] = (current_Time6 - current_Time1);
      lft_Counter++;
      break;

    case 3:
      total_Rgt_Time[rgt_Counter + 1] = (current_Time6 - current_Time2);
      rgt_Counter++;
      break;

    case 4:
      total_Bwd_Time[bwd_Counter + 1] = (current_Time6 - current_Time3);
      bwd_Counter++;
      break;
    case 5:
      total_Stp_Time[stp_Counter + 1] = (current_Time6 - current_Time4);
      stp_Counter++;
      break;
    case 6:
      total_Up_Time[Up_Counter + 1] = (current_Time6 - current_Time5);
      Up_Counter++;
      break;
  }

  seq_Array[seq] = 7;
  seq++;
}
void go_In_Seq(void) {
  value = 0;
  for (int i = 0; i < (seq + 1); i++) {
    int value1 = 0;
    value1 = seq_Array[i];
    switch (value1) {
      case 1:
        static int j = 0;
        go_Forward_Seq(j);
        j++;
        break;
      case 2:
        static int k = 0;
        go_Left_Seq(k);
        k++;
        break;
      case 3:
        static int l = 0;
        go_Right_Seq(l);
        l++;
        break;
      case 4:
        static int m = 0;
        go_Backward_Seq(m);
        m++;
        break;
      case 5:
        static int n = 0;
        go_Stop_Seq(n);
        n++;
        break;
        case 6:
        static int o = 0;
        go_Up_Seq(o);
        o++;
        break;
        case 7:
        static int p = 0;
        go_Dwn_Seq(p);
        p++;
        break;
      default:
        j = 0; k = 0; l = 0; m = 0; n = 0;o = 0;p = 0;
    }
  }
}

void del_From_Local_Mem() {
  //set the movement counters to their default values
  fwd_Counter = -1;
  lft_Counter = -1;
  rgt_Counter = -1;
  bwd_Counter = - 1;
  stp_Counter = - 1;
  Up_Counter = - 1;
  Dwn_Counter = - 1;

  //set the total movement time to its default value
  for (int i = 0; i < 10; i++) {
    total_Fwd_Time[i] = 0;
    total_Lft_Time[i] = 0;
    total_Rgt_Time[i] = 0;
    total_Bwd_Time[i] = 0;
    total_Stp_Time[i] = 0;
    total_Up_Time[i] = 0;
    total_Dwn_Time[i] = 0;
  }

  // Reset the sequence array(stored movement instructions)
  for (int i = 0; i < 50; i++) {
    seq_Array[i] = 0;
  }

  seq = 0;
  
}

/**********************************************************************************************************
     These function moves the car in a direction for the time specified/stored in the total_x_time array
************************************************************************************************************/
void go_Forward_Seq(int j) {
  //go in forward direction sequence
  movement_Inst_Fwd();
  delay(total_Fwd_Time[j]);
}

void go_Left_Seq(int k) {
  //go in Left direction sequence
  movement_Inst_Lft();
  delay(total_Lft_Time[k]);
}

void go_Right_Seq(int l) {
  //go in right direction sequence
  movement_Inst_Rgt();
  delay(total_Rgt_Time[l]);
}

void go_Backward_Seq(int m) {
  //go in backward direction sequence
  movement_Inst_Bwd();
  delay(total_Bwd_Time[m]);
}

void go_Stop_Seq(int n) {
  //go in Stop sequence
  movement_Inst_Stp();
  delay(total_Stp_Time[n]);
}
void go_Up_Seq(int o) {
  //go in Stop sequence
  movement_Inst_Up();
  delay(total_Up_Time[o]);
}
void go_Dwn_Seq(int p) {
  //go in Stop sequence
  movement_Inst_Dwn();
  delay(total_Dwn_Time[p]);
}

/*********************************************************************************************
          These movement instruction are repeated(required) several times in the code
**********************************************************************************************/
void movement_Inst_Fwd(void) {
  // forward movement instructions
  Serial.println("Going_Forward");
  digitalWrite(LM_IN1, HIGH);
  digitalWrite(LM_IN2, LOW);
  digitalWrite(RM_IN3, HIGH);
  digitalWrite(RM_IN4, LOW);
  

  
}

void movement_Inst_Lft(void) {
  // Left movement instructions
  Serial.println("Going_Left");
  digitalWrite(LM_IN1, LOW);
  digitalWrite(LM_IN2, HIGH);
  digitalWrite(RM_IN3, HIGH);
  digitalWrite(RM_IN4, LOW);
 

  // NOTE: The minimum delay for RIGHT/LEFT movement is 1S(inluding .5s ON time & .5s OFF time). Hence subtract 1s before repeating this movement
}

void movement_Inst_Rgt(void) {
  // Rgt movement instructions
  Serial.println("Going_Right"); 
  digitalWrite(LM_IN1, HIGH);
  digitalWrite(LM_IN2, LOW);
  digitalWrite(RM_IN3, LOW);
  digitalWrite(RM_IN4, HIGH);

  // NOTE: The minimum delay for RIGHT/LEFT movement is 1S(inluding .5s ON time & .5s OFF time). Hence subtract 1s before repeating this movement 

}

void movement_Inst_Bwd(void) {
  // Bwd movement instructions
  Serial.println("Going_Backward"); 
  digitalWrite(LM_IN1, LOW);
  digitalWrite(LM_IN2, HIGH);
  digitalWrite(RM_IN3, LOW);
  digitalWrite(RM_IN4, HIGH);
    
}

void movement_Inst_Stp(void) {
  // Stp movement instructions
  Serial.println("Stopping");
  digitalWrite(LM_IN1, LOW);
  digitalWrite(LM_IN2, LOW);
  digitalWrite(RM_IN3, LOW);
  digitalWrite(RM_IN4, LOW);
  digitalWrite(W1, LOW);
  digitalWrite(W2, LOW);   
      
}
    void movement_Inst_Up(void) {
  // Stp movement instructions
  Serial.println("Up");
  digitalWrite(W1, LOW);
  digitalWrite(W2, HIGH);   
}
    void movement_Inst_Dwn(void) {
  // Stp movement instructions
  Serial.println("Down");
  digitalWrite(W1, HIGH);
  digitalWrite(W2, LOW);   
}
