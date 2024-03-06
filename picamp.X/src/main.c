/* main.c */
// Include all global config variables
#include "../include/config.h"

// Include library header files
#include "../include/initialisation.h"
#include "../include/rotary_encoder.h"
#include "../include/potentiometer.h"
#include "../include/HCMS2913.h"

// Rotary encoder position
char count = 0;

// Function declarations
void pic_init(void);
void pic_loop(void);

void main(void) {
    // Do all initialisation here
    pic_init();
    // Start indefinite program loop
    pic_loop();
}

void pic_loop(void) {
    int stepArray[LED_AMOUNT] = {0};
    populate_step_array(stepArray, LED_AMOUNT, POT_LIMIT);
    short calcVolume;

    while (1) {
        HCMS_load_word(selLoadString, selectBuffer, &PORTD, CENABLE_PINNUM);
        HCMS_load_word(volLoadString, volumeBuffer, &PORTC, CENABLE_PINNUM);
        //        if (count == 0) {
        // Parse result from potentiometer
        int result = read_pot();

        // Loop through all `stepArray`, check if the output of the potmeter
        // (on `ADRES`) falls inbetween the steps - `DEADZONE`, if so execute
        // An OR/AND bitshift operation in order to turn on the corresponding
        // LED on `PORTA`.
        //            for (int i = 0; i < LED_AMOUNT; i++) {
        //                if (result > (stepArray[i] - DEADZONE)) {
        //                    PORTA = (unsigned char) (PORTA & ~(1 << i));
        //                } else if (result <= (stepArray[i] - DEADZONE)) {
        //                    PORTA = (unsigned char) (PORTA | (1 << i));
        //                }
        //            }
        //        }
        calcVolume = result / POT_STEP_AMOUNT;

        if (calcVolume > 100) {
            calcVolume = 100;
        }
        if (calcVolume < 0) {
            calcVolume = 0;
        }

        selLoadString[6] = '0' + count / 10;
        selLoadString[7] = '0' + (count % 10);

        volLoadString[5] = '0' + (char) (calcVolume / 100);
        volLoadString[6] = '0' + (char) (calcVolume / 10 > 9 ? 0 : calcVolume / 10);
        volLoadString[7] = '0' + (char) (calcVolume % 10);
    }
}

void pic_init(void) {
    OscillatorConfig osc_config = {INTERNAL_CLK, MHZ008, INTERNAL_FOSC};
    init_osc(osc_config);

    PinConfig pin_config[] = {
        { &TRISA, &PORTA, &ANSEL, OUTPUT, DIGITAL}
    };
    init_gpio(pin_config, 1);

    // Moved to rotary_encoder.c
    // InterruptConfig int_config  = { GIE_ENABLED, EINT_ENABLED, PEIE_ENABLED,
    //                               T0INT_ENABLED, RBINT_ENABLED, FALLING_EDGE };
    // init_int(int_config);

    init_rotary(IOCRB4, IOCRB5, TRISBbits.TRISB4, TRISBbits.TRISB5);
    init_pot(ADC_AN07, TRISEbits.TRISE2);
    HCMS_spi_init(MST_OSC_DIV_04, SMP1_SDI_END, CKP0_CPOL0_IDLE_LOW, CKE1_CPHA0_TRSMIT_ACT2IDL);
    HCMS_init(&PORTC, RESET_PINNUM, CENABLE_PINNUM, RSELECT_PINNUM);
    HCMS_init(&PORTD, RESET_PINNUM, CENABLE_PINNUM, RSELECT_PINNUM);
}

// #[routine:interrupt_service]

void __interrupt() isr(void) {
    if (INTCONbits.RBIF == INT_OCCURED) {
        // Parse rotary changes into `count`, shift it into `PORTA`
        parse_rotary(REA, REB, &count, LED_AMOUNT, 0);

        //PORTA = (unsigned char) (~(1 << count));

        // Clear the Port B change interrupt flag
        INTCONbits.RBIF = INT_AWAITING;
        INTCONbits.INTF = INT_AWAITING;
    }
}
