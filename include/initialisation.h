/*
Defines:
#define NPN // For inverted input ports (NPN transistor)

Tested with:
  - PIC16F877A
*/
#ifndef INITIALISATION_H
#define INITIALISATION_H

// #[declarations:init_gpio]
#ifdef NPN
  typedef enum { ON = 0, OFF = 1 } PinStates;
#else
  typedef enum { OFF = 0, ON = 1 } PinStates;
#endif

typedef enum { OUTPUT = 0,  INPUT = 1 }  PinDirection;
typedef enum { DIGITAL = 0, ANALOG = 1 } PinMode;

typedef struct {
  volatile uint8_t* tris;
  volatile uint8_t* lat;
  volatile uint8_t* ansel;
  PinDirection direction;
  PinMode mode;
} PinConfig;

// #[declarations:init_osc]
typedef enum { CONFIG_FOSC = 0,  INTERNAL_FOSC = 1 } OscStartupTime;
typedef enum { CONFIG_CLK = 0,   INTERNAL_CLK = 1 }  SysClockSelect;
typedef enum { HTS_UNSTABLE = 0, HTS_STABLE = 1 }    HFINTOSCStatus;
typedef enum { LTS_UNSTABLE = 0, LTS_STABLE = 1 }    LFINTOSCStable;
typedef enum {
  KHZ032 = 0b000, // LFINTOSC
  KHZ125 = 0b001,
  KHZ250 = 0b010,
  KHZ500 = 0b011, // Default
  MHZ001 = 0b100,
  MHZ002 = 0b101,
  MHZ004 = 0b110,
  MHZ008 = 0b111
} InternalFrequency;

typedef struct {
  SysClockSelect SCS;
  InternalFrequency IRCF;
  OscStartupTime OSTS;
} OscillatorConfig;

// #[declarations:interupt]
typedef enum { GIE_DISABLED = 0,   GIE_ENABLED = 1 }   GlobalInterrupt;
typedef enum { EINT_DISABLED = 0,  EINT_ENABLED = 1 }  ExternalInterrupt;
typedef enum { PEIE_DISABLED = 0,  PEIE_ENABLED = 1 }  PeripheralInterrupt;
typedef enum { T0INT_DISABLED = 0, T0INT_ENABLED = 1 } Timer0OverflowInterrupt;
typedef enum { RBINT_DISABLED = 0, RBINT_ENABLED = 1 } PortBInterrupt;
typedef enum { FALLING_EDGE = 0,   RISING_EDGE = 1 }   InterruptEdge;
typedef enum { INT_AWAITING = 0,   INT_OCCURED = 1 }   InterruptFlag;

typedef struct {
  GlobalInterrupt GIE;
  ExternalInterrupt EINT;
  PeripheralInterrupt PEIE;
  Timer0OverflowInterrupt T0IE;
  PortBInterrupt RBIE;
  InterruptEdge INTEDG;
} InterruptConfig;

// #[declarations:adc]
typedef enum { ADC_OFF = 0,      ADC_ON = 1 }          ADCEnable;
typedef enum { VREF_MINPIN = 0,  VREF_VSS = 1 }        ADCVoltageReference1;
typedef enum { VREF_PLUSPIN = 0, VREF_VDD = 1 }        ADCVoltageReference0;
typedef enum { ADC_LEFT = 0,     ADC_RIGHT = 1 }       ADCResultFormat;
typedef enum { ADC_DONE = 0,     ADC_IN_PROGRESS = 1 } ADCConversionStatus;
typedef enum {
  ADC_AN00  = 0b0000,
  ADC_AN01  = 0b0001,
  ADC_AN02  = 0b0010,
  ADC_AN03  = 0b0011,
  ADC_AN04  = 0b0100,
  ADC_AN05  = 0b0101,
  ADC_AN06  = 0b0110,
  ADC_AN07  = 0b0111,
  ADC_AN08  = 0b1000,
  ADC_AN09  = 0b1001,
  ADC_AN10  = 0b1010,
  ADC_AN11  = 0b1011,
  ADC_AN12  = 0b1100,
  ADC_AN13  = 0b1101,
  ADC_CVREF = 0b1110, // CVREF
  ADC_FXREF = 0b1111  // Fixed Ref (0.6V fixed voltage reference)
} ADCChannelSelect;

typedef enum {
  ADC_FOSC02 = 0b00,
  ADC_FOSC08 = 0b01,
  ADC_FOSC32 = 0b10,
  ADC_INTOSC = 0b11 // (FRC) from a dedicated internal oscillator = 500 kHz max
} ADCConversionClock;

typedef struct {
  ADCEnable ADON;
  ADCResultFormat ADFM;
  ADCChannelSelect CHS;
  ADCConversionClock ADCS;
  ADCVoltageReference0 VCFG0;
  ADCVoltageReference1 VCFG1;
  ADCConversionStatus GO_nDONE;
} ADCConfig;

// #[declarations:functions]

void init_osc(OscillatorConfig config);
void init_int(InterruptConfig config);
void init_adc(ADCConfig config);
void init_gpio2(PinConfig* ports);

#endif
