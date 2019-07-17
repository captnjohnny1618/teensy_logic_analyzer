// Allocate the array that we'll use to hold data
char usb_buffer[512];

void setup(){

    // Enable the cycle counter
    //ARM_DEMCR |= ARM_DEMCR_TRCENA;
    //ARM_DWT_CTRL |= ARM_DWT_CTRL_CYCCNTENA;
    
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

    Serial.begin(115200);

}


void loop(){

    ///Loop code    
    int loop_count=0;
    //int cycles= ARM_DWT_CYCCNT;    

    while (loop_count<512){

        //cycles= ARM_DWT_CYCCNT;

        // Use pins 0-7
        //usb_buffer[loop_count]=(uint8_t)PIND;
         
        // Use PORTD of the Teensy (should also work though for arduino architectures)
        usb_buffer[loop_count]=GPIOD_PDIR;
        
        //cycles=ARM_DWT_CYCCNT-cycles;
        //Serial.print("Cycles to read pins: ");
        //Serial.println(cycles);
        
        loop_count+=1;
    }

    //cycles=ARM_DWT_CYCCNT;
    Serial.write(usb_buffer,512);
    //cycles=ARM_DWT_CYCCNT-cycles;

    // DEBUG
    //Serial.print("Cycles to dump usb: ");
    //Serial.println(cycles);
    ///

    

}
