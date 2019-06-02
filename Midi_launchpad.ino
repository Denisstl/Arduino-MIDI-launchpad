#include <MIDI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_PCD8544.h>

//CLK --- Pin 2
//DIN --- Pin 4
//D/C --- Pin 5
//CS  --- Pin 6
//RST --- Pin 7

Adafruit_PCD8544 display = Adafruit_PCD8544(2, 4, 5, 6, 7);

#if defined(USBCON)
#include <midi_UsbTransport.h>

static const unsigned sUsbTransportBufferSize = 16;
typedef midi::UsbTransport<sUsbTransportBufferSize> UsbTransport;

UsbTransport sUsbTransport;

MIDI_CREATE_INSTANCE(UsbTransport, sUsbTransport, MIDI);

#else // No USB available, fallback to Serial
MIDI_CREATE_DEFAULT_INSTANCE();
#endif

// --

#define BUTTON_M 12
#define BUTTON_D 11
#define BUTTON_U 10
#define BUTTON_L 9
#define BUTTON_R 8

int LIGHT_PIN = 3;
int aftertouchtime = 15;
int afttime = aftertouchtime;
int blinktime = 30;
int blinktime_ = 10;
int blinkTime = blinktime;
int blinkTime_ = blinktime_;
int buttontime = 25;
int buttonTime = buttontime;
int buttontime_LAUNCHPAD = buttonTime;
int lightmode = 0;
int lightvalue;
int brightness;
int brighpercent = 100;
int dispblinkingtime = 14;
int dispblinkingTime = dispblinkingtime;
int timedontallowcontrolmidi = 30;
int timeDontallowcontrolmidi = timedontallowcontrolmidi;
byte mode = 1;
byte menu = 0;
byte inchannel;
byte innumber;
byte invelocity;
byte invalue;
byte Pressure;
byte aftertouch = 0;
boolean pause = false;
boolean interrupts_M = false;
boolean interrupts_D = false;
boolean interrupts_U = false;
boolean interrupts_L = false;
boolean interrupts_R = false;
boolean interrupts_LH = false;
boolean interrupts_RH = false;
boolean interrupts_M_LAUNCHPAD = false;
boolean interrupts_D_LAUNCHPAD = false;
boolean interrupts_U_LAUNCHPAD = false;
boolean interrupts_L_LAUNCHPAD = false;
boolean interrupts_R_LAUNCHPAD = false;
boolean interrupts_LH_LAUNCHPAD = false;
boolean interrupts_RH_LAUNCHPAD = false;
boolean off = true; 
boolean light = false; //  false - ON    true - OFF
boolean lightmode34 = true;
boolean launchpad_pro_control = false;
boolean dontallowcontrolmidi = false;

// BUTTON
void button(){
    int knock_M = digitalRead(BUTTON_M);
    int knock_D = digitalRead(BUTTON_D);
    int knock_U = digitalRead(BUTTON_U);
    int knock_L = digitalRead(BUTTON_L);
    int knock_R = digitalRead(BUTTON_R);
    
    // MIDDLE
    if(knock_M == 0){
        interrupts_M = true;
    }
    else if(interrupts_M == true){
        if(menu == 0){
            pause = !pause;
        }
        else if(menu == 1){
            light = !light;
        }
        else if(menu == 3){
            if(lightmode < 4){
                lightmode++;
            }
            else{
                lightmode = 0;
            }
        }
        else if(menu == 4){
            launchpad_pro_control = !launchpad_pro_control;
        }
        interrupts_M = false;
    }

    // DOWN
    if(knock_D == 0){
        interrupts_D = true;
    }
    else if(interrupts_D == true){
        if(menu < 4){
            menu++;
        }
        else{
            menu = 0;
        }
        interrupts_D = false;
    }
    
    // UP
    if(knock_U == 0){
        interrupts_U = true;
    }
    else if(interrupts_U == true){
        if(menu > 0){
            menu--;
        }
        else{
            menu = 4;
        }
        interrupts_U = false;
    }

    // LEFT PRESSING
    if(knock_L == 0){
        interrupts_L = true;
    }
    else if(interrupts_L == true){
        if(menu == 0){
            pause = !pause;
        }
        else if(menu == 1){
            light = !light;
        }
        else if(menu == 2 && brighpercent > 0){
            brighpercent--;
        }
        else if(menu == 3){
            if(lightmode > 0){
                lightmode--;
            }
            else{
                lightmode = 4;
            }
        }
        else if(menu == 4){
            launchpad_pro_control = !launchpad_pro_control;
        }
        interrupts_L = false;
    }
    
    // RIGHT PRESSING
    if(knock_R == 0){
        interrupts_R = true;
    }
    else if(interrupts_R == true){
        if(menu == 0){
            pause = !pause;
        }
        else if(menu == 1){
            light = !light;
        }
        else if(menu == 2 && brighpercent < 100){
            brighpercent++;
        }
        else if(menu == 3){
            if(lightmode < 4){
                lightmode++;
            }
            else{
                lightmode = 0;
            }
        }
        else if(menu == 4){
            launchpad_pro_control = !launchpad_pro_control;
        }
        interrupts_R = false;
    }

    // LEFT HOLDING
    if(knock_L == 0 && interrupts_LH == false){
        if(buttontime > 0){
            buttontime--;
        }
        else{
            interrupts_LH = true;
        }
    }
    else if(knock_L == 0 && interrupts_LH == true){
        if(menu == 2 && brighpercent > 0){
            brighpercent--;
        }
    }
    else if(knock_L != 0 && interrupts_LH == true){
        interrupts_LH = false;
        interrupts_L = false;
        buttontime = buttonTime;
    }

    // RIGHT HOLDING
    if(knock_R == 0 && interrupts_RH == false){
        if(buttontime > 0){
            buttontime--;
        }
        else {
            interrupts_RH = true;
        }
    }
    else if(knock_R == 0 && interrupts_RH == true){
        if(menu == 2 && brighpercent < 100){
            brighpercent++;
        }
    }
    else if(knock_R != 0 && interrupts_RH == true){
        interrupts_RH = false;
        interrupts_R = false;
        buttontime = buttonTime;
    }
}

// DISPLAY LIGHT
void displight(){
    brightness = 255 - (255 * brighpercent / 100);
    if(light){
        lightvalue = 255;
    }

    
    else if(lightmode == 0){
        lightvalue = brightness;
    }

    
    else if(lightmode == 1){
        if(mode == 2){
            lightvalue = 2 * (127 - invalue) * brighpercent / 100 + brightness + 1;
        }
        else{
            lightvalue = 2 * (127 - Pressure) * brighpercent / 100 + brightness + 1;
        }
        if(aftertouch == 0 && mode != 2 && !off){
            lightvalue = brightness;
        }
        if (lightvalue < brightness){
            lightvalue = brightness;
        }
        else if (lightvalue > 255){
            lightvalue = 255;
        }
    }

    
    else if(lightmode == 2){
        if(mode == 2){
            lightvalue = 2 * invalue * brighpercent / 100 + brightness + 1;
        }
        else{
            lightvalue = 2 * Pressure * brighpercent / 100 + brightness + 1;
        }
        if(aftertouch == 0 && mode != 2 && !off){
            lightvalue = 255;
        }
        if (lightvalue < brightness){
            lightvalue = brightness;
        }
        else if (lightvalue > 255){
            lightvalue = 255;
        }
    }

    else if(lightmode == 3){
        lightvalue = 255;
        if(dispblinkingtime > 0 && lightmode34) {
            if((mode == 2 && invalue != 0) || (mode != 2 && !off)){
                lightvalue = brightness;
            }
            dispblinkingtime--;
        }
        else if(dispblinkingtime == 0 && lightmode34){
            lightmode34 = false;
            dispblinkingtime = dispblinkingTime;
        }
        else if(dispblinkingtime > 0 && !lightmode34){
            dispblinkingtime = dispblinkingtime - (dispblinkingTime / 2);
        }
        else if(dispblinkingtime == 0 && !lightmode34){
            lightmode34 = true;
            dispblinkingtime = dispblinkingTime;
        }
    }
    
    else if(lightmode == 4){
        lightvalue = brightness;
        if(dispblinkingtime > 0 && lightmode34) {
            dispblinkingtime--;
        }
        else if(dispblinkingtime == 0 && lightmode34){
            lightmode34 = false;
            dispblinkingtime = dispblinkingTime;
        }
        else if(dispblinkingtime > 0 && !lightmode34){
            if((mode == 2 && invalue != 0) || (mode != 2 && !off)){
                lightvalue = 255;
            }
            dispblinkingtime = dispblinkingtime - (dispblinkingTime / 2);
        }
        else if(dispblinkingtime == 0 && !lightmode34){
            lightmode34 = true;
            dispblinkingtime = dispblinkingTime;
        }
    }
    analogWrite(LIGHT_PIN, lightvalue);
}

//DISPLAY
void disp(){
    display.clearDisplay();
    display.setCursor(0, 0);
    display.setTextSize(1);
    display.print("Channel:   ");
    display.println(inchannel);
    /////
    if(mode == 0 || mode == 1){
        display.print("Note:      ");
    }
    else{
        display.print("Control.:  ");
    }
    display.println(innumber);
    /////
    display.print("Status:    ");
    if(mode == 0 || mode == 1){
        if(off){
            display.println("OFF");
        }
        else{
            display.println("ON");
        }
        display.print("Velocity:  ");
        display.println(invelocity);
    }
    else if(mode == 2){
        if(invalue == 0){
            display.println("OFF");
        }
        else{
            display.println("ON");
        }
        
        display.print("Value:     ");
        display.println(invalue);
    }
    /////
    if(!off && Pressure == 0){
        aftertouchoff();
    }
    if(aftertouch == 0 && mode != 2){
        display.println("ATouch:    OFF");
    }
    else if(aftertouch == 1 && mode != 2){
        display.print("ATouchCh:  ");
        display.println(Pressure);
    }
    else if(aftertouch == 2 && mode != 2){
        display.print("ATouchPo:  ");
        display.println(Pressure);
    }
    else if(mode == 2){
        display.println();
    }
    /////
    symbBlink();
    if(menu == 0){
        display.print(" Pause: ");
        if(pause){
            display.println(" true");
        }
        else{
            display.println("false");
        }
    }
    else if(menu == 1){
        display.print(" Light:   ");
        if(!light){
            display.println("ON");
        }
        else{
            display.println("OFF");
        }
    }
    else if(menu == 2){
        display.print(" Bright.: ");
        display.println(brighpercent);
    }
    else if(menu == 3){
        display.print(" LightM.: ");
        if(lightmode == 0){
            display.println("OFF");
        }
        else{
            display.println(lightmode);
        }
    }
    else if(menu == 4){
        display.print(" LppCon.: ");
        if(launchpad_pro_control == 0){
            display.println("OFF");
        }
        else{
            display.println("ON");
        }
    }
}

void aftertouchoff(){
    if(afttime == 0){
        aftertouch = 0;
        afttime = aftertouchtime;
    }
    else{
        afttime--;
    }
}

void symbBlink(){
    if(!dontallowcontrolmidi){
        if(blinktime_ == 0 && blinktime == 0){
            blinktime = blinkTime;
            blinktime_ = blinkTime_;
        }
        if(blinktime_ > 0){
            display.print(" ");
            blinktime_--;
        }
        else if(blinktime > 0){
            display.print("*");
            blinktime--;
        }
    }
    else{
        if(timedontallowcontrolmidi > 0){
            timedontallowcontrolmidi--;
            display.print("x");
        }
        else{
            timedontallowcontrolmidi = timeDontallowcontrolmidi;
            dontallowcontrolmidi = false;
            blinktime = blinkTime;
            blinktime_ = blinkTime_;
        }
    }
}


// CONTROL THROUGH MIDI
void midicontrol(){
    if(launchpad_pro_control){
        
        // MIDI UP
        if(innumber == 91 && invalue != 0){
            interrupts_U_LAUNCHPAD = true;
        }
        else if(interrupts_U_LAUNCHPAD == true){
            if(menu > 0){
                menu--;
            }
            else{
                menu = 4;
            }
        interrupts_U_LAUNCHPAD = false;
        }

        // MIDI DOWN
        if(innumber == 92 && invalue != 0){
            interrupts_D_LAUNCHPAD = true;
        }
        else if(interrupts_D_LAUNCHPAD == true){
            if(menu < 4){
                menu++;
            }
            else{
                menu = 0;
            }
            interrupts_D_LAUNCHPAD = false;
        }

        // LEFT PRESSING
        if(innumber == 93 && invalue != 0){
            interrupts_L_LAUNCHPAD = true;
        }
        else if(interrupts_L_LAUNCHPAD == true){
            if(menu == 0){
                pause = !pause;
            }
            else if(menu == 1){
                light = !light;
            }
            else if(menu == 2 && brighpercent > 0){
                brighpercent--;
            }
            else if(menu == 3){
                if(lightmode > 0){
                    lightmode--;
                }
                else{
                    lightmode = 4;
                }
            }
            else if(menu == 4){
                dontallowcontrolmidi = true;
            }
            interrupts_L_LAUNCHPAD = false;
        }
        
        // MIDI LEFT HOLDING
        if(innumber == 93 && invalue != 0 && interrupts_LH_LAUNCHPAD == false){
            if(buttontime_LAUNCHPAD > 0){
                buttontime_LAUNCHPAD--;
            }
            else{
                interrupts_LH_LAUNCHPAD = true;
            }
        }
        else if(innumber == 93 && invalue != 0 && interrupts_LH_LAUNCHPAD == true){
            if(menu == 2 && brighpercent > 0){
                brighpercent--;
            }
        }
        else if(innumber == 93 && invalue == 0 && interrupts_LH_LAUNCHPAD == true){
            interrupts_LH_LAUNCHPAD = false;
            interrupts_L_LAUNCHPAD = false;
            buttontime_LAUNCHPAD = buttonTime;
        }

        // MIDI RIGHT PRESSING
        if(innumber == 94 && invalue != 0){
            interrupts_R_LAUNCHPAD = true;
        }
        else if(interrupts_R_LAUNCHPAD == true){
            if(menu == 0){
                pause = !pause;
            }
            else if(menu == 1){
                light = !light;
            }
            else if(menu == 2 && brighpercent < 100){
                brighpercent++;
            }
            else if(menu == 3){
                if(lightmode < 4){
                    lightmode++;
                }
                else{
                    lightmode = 0;
                }
            }
            else if(menu == 4){
                dontallowcontrolmidi = true;
            }
            interrupts_R_LAUNCHPAD = false;
        }

        // MIDI RIGHT HOLDING
        if(innumber == 94 && invalue != 0 && interrupts_RH_LAUNCHPAD == false){
            if(buttontime_LAUNCHPAD > 0){
                buttontime_LAUNCHPAD--;
            }
            else {
                interrupts_RH_LAUNCHPAD = true;
            }
        }
        else if(innumber == 94 && invalue != 0 && interrupts_RH_LAUNCHPAD == true){
            if(menu == 2 && brighpercent < 100){
                brighpercent++;
            }
        }
        else if(innumber == 94 && invalue == 0 && interrupts_RH_LAUNCHPAD == true){
            interrupts_RH_LAUNCHPAD = false;
            interrupts_R_LAUNCHPAD = false;
            buttontime_LAUNCHPAD = buttonTime;
        }

        // MIDI MIDDLE
        if((innumber == 70  || innumber == 80) && invalue != 0){
            interrupts_M_LAUNCHPAD = true;
        }
        else if(interrupts_M_LAUNCHPAD == true){
            if(menu == 0){
            pause = !pause;
            }
            else if(menu == 1){
                light = !light;
            }
            else if(menu == 3){
                if(lightmode < 4){
                    lightmode++;
                }
                else{
                    lightmode = 0;
                }
            }
            else if(menu == 4){
                  dontallowcontrolmidi = true;
            }
            interrupts_M_LAUNCHPAD = false;
        }
    }
}

// MIDI SETTING
void setInputChannel(const byte Channel){}

void handleNoteOn(byte inChannel, byte inNumber, byte inVelocity){
    mode = 0;
    inchannel = inChannel;
    innumber = inNumber;
    invelocity = inVelocity;
    off = false;
}

void handleNoteOff(byte inChannel, byte inNumber, byte inVelocity){
    if(!pause) {
        mode = 1;
        inchannel = inChannel;
        innumber = inNumber;
        invelocity = inVelocity;
    }
    off = true;
}

void HandleControlChange(byte inChannel, byte inNumber, byte inValue){
    mode = 2;
    inchannel = inChannel;
    innumber = inNumber;
    invalue = inValue;
}

void HandleAfterTouchChannel(byte channel, byte pressure){
    inchannel = channel;
    Pressure = pressure;
    aftertouch = 1;
}

void HandleAfterTouchPoly(byte channel, byte note, byte pressure){
    inchannel = channel;
    innumber = note;
    Pressure = pressure;
    aftertouch = 2;
}

/////
void setup(){
    MIDI.begin();
    Serial.begin(115200);
    while (!Serial);
    MIDI.setInputChannel(MIDI_CHANNEL_OMNI);
    MIDI.setHandleNoteOn(handleNoteOn);
    MIDI.setHandleNoteOff(handleNoteOff);
    MIDI.setHandleControlChange(HandleControlChange);
    MIDI.setHandleAfterTouchChannel(HandleAfterTouchChannel);
    MIDI.setHandleAfterTouchPoly(HandleAfterTouchPoly);

    display.begin();
    display.setContrast(55);
    display.clearDisplay();
    display.display();
    
    pinMode(LIGHT_PIN, OUTPUT);
    digitalWrite(LIGHT_PIN, LOW);
    
    pinMode(BUTTON_D, INPUT_PULLUP);
    pinMode(BUTTON_U, INPUT_PULLUP);
    pinMode(BUTTON_L, INPUT_PULLUP);
    pinMode(BUTTON_R, INPUT_PULLUP);
    pinMode(BUTTON_M, INPUT_PULLUP);
}

void loop(){
    MIDI.read();
    button();
    disp();
    displight();
    midicontrol();
    display.display();
}
