#include "./config.h"

// Function declarations
void pic_loop(void);
void pic_init(void);
void init_osc(void);
void init_gpio(void);
void init_HCMS(void);
void init_IR(void);

// Rotary encoder position
int selectcount = 0;
unsigned char brightnessSetting = 20;

void main(void) {
    // Do all initialisation here
    pic_init();
    // Start indefinite program loop
    pic_loop();
}

void pic_loop(void) {
    // Initialize potentiometer step array
    int stepArray[LED_AMOUNT] = {0};
    populate_step_array(stepArray, LED_AMOUNT, POT_LIMIT);
    short calcVolume;

    while (1) {
        HCMS_load_word(volLoadString, &screenSettings_1);
        HCMS_load_word(selLoadString, &screenSettings_2);
        // Parse result from potentiometer
        int result = read_potentiometer();
        // Read the data from the potmeter pin (`E2`) registers, and combine
        // them into a single integer
        // Loop through all `stepArray`, check if the output of the potmeter
        // (on `ADRES`) falls inbetween the steps - `DEADZONE`, if so execute
        // An OR/AND bitshift operation in order to turn on the corresponding
        // LED on `PORTA`.

        // Do not display the potmeter LEDs if input selector is > minimum
        if (selectcount == 1) {
            for (int i = 0; i < LED_AMOUNT; i++) {
                if (result > (stepArray[i] - DEADZONE)) {
                    PORTA = (unsigned char) (PORTA & ~(1 << i));
                } else if (result <= (stepArray[i] - DEADZONE)) {
                    PORTA = (unsigned char) (PORTA | (1 << i));
                }
            }
        };
        // int step = adc_to_step(*stepArray, LED_AMOUNT, DEADZONE);
        // PORTA = (unsigned char)(PORTA | (1 << step));

        calcVolume = result / 10;

        if (IRBF) {
            PORTA = 0x00;
            if (data == 0b010010010000) { //input 1
                selectcount = 1;
                LED1 = 0;
            }
            if (data == 0b010100001000) { //input 2
                selectcount = 2;
                LED2 = 0;
            }
            if (data == 0b010010100000) { //input 3
                selectcount = 3;
                LED3 = 0;
            }
            if (data == 0b010010001000) { //input 4
                selectcount = 4;
                LED4 = 0;
            }
            if (data == 0b010100100000) { //volume up
                calcVolume++;
                brightnessSetting += 5;
                MOTOR_P = 1;
                MOTOR_M = 0;
                __delay_ms(500);
                MOTOR_M = 0;
                MOTOR_P = 0;
                selectcount = 2;
                LED1 = 0;
                LED2 = 0;
            }
            if (data == 0b010100010000) { //volume down
                calcVolume--;
                if (brightnessSetting > 4) {
                    brightnessSetting -= 5;
                }

                MOTOR_P = 0;
                MOTOR_M = 1;
                __delay_ms(500);
                MOTOR_M = 0;
                MOTOR_P = 0;
                selectcount = 3;
                LED3 = 0;
                LED4 = 0;
            }

            if (brightnessSetting > 59) {
                brightnessSetting = 59;
            }
            HCMS_change_brightness(brightnessSetting, &screenSettings_1); //change volume screen brightness
            __delay_ms(250);
            IRBF = 0;
        }

        if (calcVolume > 100) {
            calcVolume = 100;
        }
        if (calcVolume < 0) {
            calcVolume = 0;
        }

        selLoadString[6] = '0' + (char) (selectcount / 10);
        selLoadString[7] = '0' + (char) (selectcount % 10);

        volLoadString[5] = '0' + (char) (calcVolume / 100);
        volLoadString[6] = '0' + (char) (calcVolume / 10 > 9 ? 0 : calcVolume / 10);
        volLoadString[7] = '0' + (char) (calcVolume % 10);
    }
}

void pic_init(void) {
    init_osc();
    init_gpio();
    init_HCMS();
    init_IR();
    init_pot(PIC16F887A, P887A_AN07);
    init_rot(PIC16F887A);
}

// #[initialisation:gpio]

void init_gpio(void) {
    //Ready Ports C and D
    TRISC = 0;
    TRISD = 0;
    PORTC = 0;
    PORTD = 0;
    //Analog selection
    ANSEL = 0;
    ANSELH = 0;
    ANSELbits.ANS7 = 1;

    // LED => output, digital (transistors)
    TRISA = 0x00;
    PORTA = 0xFF;

    //Read motor
    MOTOR_Mtris = 0;
    MOTOR_Ptris = 0;
    MOTOR_M = 0;
    MOTOR_P = 0;
}

// #[initialisation:HCMS]

void init_HCMS() {
    HCMS_spi_init(MST_OSC_DIV_04, SMP1_SDI_END, CKP0_CPOL0_IDLE_LOW, CKE1_CPHA0_TRSMIT_ACT2IDL, &screenSettings_1);
    HCMS_init(&screenSettings_1);
    HCMS_init(&screenSettings_2);
    HCMS_change_brightness(brightnessSetting, &screenSettings_1); //set volume to brightsetting for demo
}

// #[initialisation:IR]

void init_IR() {
    IRBind887();
    IRInit(falling, _MTPR, _MTPrSc, _MTPoSc, _TTPrSc);
}

// #[initialisation:osc]

void init_osc(void) {
    // System Clock Select (SCS)
    OSCCONbits.SCS = 0b1; // Internal Clock selected
    // Internal Resistor-Capacitor Frequency select (IRCF)
    OSCCONbits.IRCF = 0b111; // 8 MHz clock speed
    OSCCONbits.OSTS = 0b1; // OSC startup time

    // Halt boot until clock is stable
    while (OSCCONbits.HTS != 0b1);
}

// #[routine:interrupt_service]

void __interrupt() isr(void) {
    if (INTF) {
        IntHandler(); //Interrupt Handling
    }//INTF

    if (TMR2IF) {
        MTHandler(); //Measure Timer Handling
    }//TMR2IF

    if (TMR1IF) { //timeout / reset
        TTHandler(); //Timeout Timer Handling
    } //TMR1IF

    if (count > 0) return; //if "count" from the IR Library is not 0, that means it is busy reading, so return.

    if (INTCONbits.RBIF == 1) {
        // Parse rotary changes into `count`, shift it into `PORTA`
        selectcount = read_rotary();
        PORTA = (unsigned char) (~(1 << selectcount));

        // Clear the Port B change interrupt flag
        INTCONbits.RBIF = 0;
    }
}
