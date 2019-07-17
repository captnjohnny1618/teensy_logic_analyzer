// Initialize our "bus" definition
// Here, pins 0-7 on the Teensy are our

int bus_width=8;
int bus_pins[8];

bool bus_vals[8];

void blink();

void setup(){

    // Define PINS
    for (int i=0; i<bus_width; i++){
        bus_pins[i]=i;
    }

    // Define pin MODES
    for (int i=0; i<bus_width;i++){
        pinMode(bus_pins[i],INPUT);
    }

    // Indicator LED
    pinMode(13,OUTPUT);
    
    // Startup SERIAL
    //Serial.begin(9600);
    Serial.begin(115200);
}

void loop(){

    for (int i=0; i<bus_width; i++){
        Serial.print(digitalRead(bus_pins[i]));
        Serial.print(" ");
    }
    Serial.println();

    blink();
    delay(10);
}

void blink(){
    digitalWrite(13,HIGH);
    delay(10);
    digitalWrite(13,LOW);
}
