// Allocate the array that we'll use to hold data
#define ONE_CAPTURE 512
#define N_CAPTURES 110
#define CAPTURE_SIZE ONE_CAPTURE*N_CAPTURES

char usb_buffer[CAPTURE_SIZE];

void blink();
void blink_angry();
void blink_once();
    
void setup(){

    // Enable the cycle counter
    ARM_DEMCR |= ARM_DEMCR_TRCENA;
    ARM_DWT_CTRL |= ARM_DWT_CTRL_CYCCNTENA;
    
    // Set bus pins
    // Sets pins 0-7 to be output.  NOTE: Does not take advantage
    // of port reading on the Teensy the way that it would on an
    // Atmega 328.
    // DDRD=0;

    // Set the D port pins for the Teensy
    pinMode(2 ,INPUT); // D0
    pinMode(14,INPUT); // D1
    pinMode(7 ,INPUT); // D2
    pinMode(8 ,INPUT); // D3
    pinMode(6 ,INPUT); // D4
    pinMode(20,INPUT); // D5
    pinMode(21,INPUT); // D6
    pinMode(5 ,INPUT); // D7

    pinMode(13,OUTPUT);
    Serial.begin(115200);
}


void loop(){

    // Set an initial state for everything
    uint32_t cycles= ARM_DWT_CYCCNT;    
    do_capture();    
    cycles= ARM_DWT_CYCCNT-cycles;

    // Buffer to hold command strings for parsing
    while (1){
        if (Serial.available()>0){

            char cmd=Serial.read();

            // Dump data to serial (i.e. for final readout)
            if (cmd=='d'){
                transfer_large_data();
            }
            // Read # cycles for entire capture out
            else if (cmd=='c'){
                Serial.write((char*)&cycles,4); // 32-bit integer
            }
            // Blink LED (nice sanity check for serial communication)
            else if (cmd=='b'){
                blink();
            }
            // Redo capture
            else if (cmd=='r'){
                cycles=ARM_DWT_CYCCNT;
                do_capture();
                cycles=ARM_DWT_CYCCNT-cycles;
            }
            // Invalid command
            else{
                blink_angry();
            }
        }
    }
}


void blink(){
    digitalWrite(13,HIGH);
    delay(25);
    digitalWrite(13,LOW);
    delay(350);

    digitalWrite(13,HIGH);
    delay(25);
    digitalWrite(13,LOW);
    delay(350);

    digitalWrite(13,HIGH);
    delay(25);
    digitalWrite(13,LOW);
    delay(350);

    digitalWrite(13,HIGH);
    delay(25);
    digitalWrite(13,LOW);
    delay(350);        
}

void blink_angry(){
    digitalWrite(13,HIGH);
    delay(25);
    digitalWrite(13,LOW);
    delay(100);

    digitalWrite(13,HIGH);
    delay(25);
    digitalWrite(13,LOW);
    delay(100);

    digitalWrite(13,HIGH);
    delay(25);
    digitalWrite(13,LOW);
    delay(100);

    digitalWrite(13,HIGH);
    delay(25);
    digitalWrite(13,LOW);
    delay(100);    
}

void do_capture(){
    int loop_count=0;    
    while (loop_count<CAPTURE_SIZE){

        // Use pins 0-7
        //usb_buffer[loop_count]=(uint8_t)PIND;
         
        // Use PORTD of the Teensy (should also work though for arduino architectures)
        usb_buffer[loop_count]=GPIOD_PDIR;        
        loop_count+=1;
    }
}

void transfer_large_data(){

    char ack_nack;
    
    // Tell the host we're read
    Serial.write('<');

    // Check if host acknowledges received byte
    while (1){
        if (Serial.available()>0){
            ack_nack=Serial.read();
            if (ack_nack=='y'){break;}
            else {continue;}
        }
    }
    blink_once();


    // Main transmit loop
    for (int i=0;i<N_CAPTURES;i++){

        // Send 512 bytes
        Serial.write(&usb_buffer[i*ONE_CAPTURE],ONE_CAPTURE);

        // Wait for the host to acknowledge receipt of 512 bytes
        while (1){
            if (Serial.available()>0){
                ack_nack=Serial.read();
                if (ack_nack=='y'){break;}
                else {continue;}
            }
        }
        blink_once();        
    }
    
    Serial.write('>');
    while (1){
        if (Serial.available()>0){
            ack_nack=Serial.read();
            if (ack_nack=='y'){break;}
            else {continue;}
        }
    }
    blink_once();    
}

void blink_once(){
    digitalWrite(13,HIGH);
    delay(25);
    digitalWrite(13,LOW);
}
