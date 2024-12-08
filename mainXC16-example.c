#include <string.h> /* memset */
#include <unistd.h> /* close */

#include <xc.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#define FCY 1000000UL
#include <libpic30.h>
#include <stdbool.h>

// FSEC
#pragma config BWRP = OFF    //Boot Segment Write-Protect bit->Boot Segment may be written
#pragma config BSS = DISABLED    //Boot Segment Code-Protect Level bits->No Protection (other than BWRP)
#pragma config BSEN = OFF    //Boot Segment Control bit->No Boot Segment
#pragma config GWRP = OFF    //General Segment Write-Protect bit->General Segment may be written
#pragma config GSS = DISABLED    //General Segment Code-Protect Level bits->No Protection (other than GWRP)
#pragma config CWRP = OFF    //Configuration Segment Write-Protect bit->Configuration Segment may be written
#pragma config CSS = DISABLED    //Configuration Segment Code-Protect Level bits->No Protection (other than CWRP)
#pragma config AIVTDIS = OFF    //Alternate Interrupt Vector Table bit->Disabled AIVT

// FBSLIM
#pragma config BSLIM = 8191    //Boot Segment Flash Page Address Limit bits->8191

// FOSCSEL
#pragma config FNOSC = FRCDIVN    //Oscillator Source Selection->Internal Fast RC (FRC) Oscillator with postscaler
#pragma config IESO = OFF    //Two-speed Oscillator Start-up Enable bit->Start up with user-selected oscillator source

// FOSC
#pragma config POSCMD = NONE    //Primary Oscillator Mode Select bits->Primary Oscillator disabled
#pragma config OSCIOFNC = OFF    //OSC2 Pin Function bit->OSC2 is clock output
#pragma config FCKSM = CSDCMD    //Clock Switching Mode bits->Both Clock switching and Fail-safe Clock Monitor are disabled
#pragma config PLLKEN = ON    //PLL Lock Enable->PLL clock output will be disabled if LOCK is lost
#pragma config XTCFG = G3    //XT Config->24-32 MHz crystals
#pragma config XTBST = ENABLE    //XT Boost->Boost the kick-start

// FWDT
#pragma config RWDTPS = PS2147483648    //Run Mode Watchdog Timer Post Scaler select bits->1:2147483648
#pragma config RCLKSEL = LPRC    //Watchdog Timer Clock Select bits->Always use LPRC
#pragma config WINDIS = OFF    //Watchdog Timer Window Enable bit->Watchdog Timer in Window mode
#pragma config WDTWIN = WIN25    //Watchdog Timer Window Select bits->WDT Window is 25% of WDT period
#pragma config SWDTPS = PS2147483648    //Sleep Mode Watchdog Timer Post Scaler select bits->1:2147483648
#pragma config FWDTEN = ON_SW    //Watchdog Timer Enable bit->WDT controlled via SW, use WDTCON.ON bit

// FPOR
#pragma config BISTDIS = DISABLED    //Memory BIST Feature Disable->mBIST on reset feature disabled

// FICD
#pragma config ICS = PGD1    //ICD Communication Channel Select bits->Communicate on PGC1 and PGD1
#pragma config JTAGEN = OFF    //JTAG Enable bit->JTAG is disabled

// FDMTIVTL
#pragma config DMTIVTL = 0    //Dead Man Timer Interval low word->0

// FDMTIVTH
#pragma config DMTIVTH = 0    //Dead Man Timer Interval high word->0

// FDMTCNTL
#pragma config DMTCNTL = 0    //Lower 16 bits of 32 bit DMT instruction count time-out value (0-0xFFFF)->0

// FDMTCNTH
#pragma config DMTCNTH = 0    //Upper 16 bits of 32 bit DMT instruction count time-out value (0-0xFFFF)->0

// FDMT
#pragma config DMTDIS = OFF    //Dead Man Timer Disable bit->Dead Man Timer is Disabled and can be enabled by software

// FDEVOPT
#pragma config ALTI2C1 = OFF    //Alternate I2C1 Pin bit->I2C1 mapped to SDA1/SCL1 pins
#pragma config SMB3EN = SMBUS3    //SM Bus Enable->SMBus 3.0 input levels
#pragma config SPI2PIN = PPS    //SPI2 Pin Select bit->SPI2 uses I/O remap (PPS) pins

// FALTREG
#pragma config CTXT1 = OFF    //Specifies Interrupt Priority Level (IPL) Associated to Alternate Working Register 1 bits->Not Assigned
#pragma config CTXT2 = OFF    //Specifies Interrupt Priority Level (IPL) Associated to Alternate Working Register 2 bits->Not Assigned
#pragma config CTXT3 = OFF    //Specifies Interrupt Priority Level (IPL) Associated to Alternate Working Register 3 bits->Not Assigned
#pragma config CTXT4 = OFF    //Specifies Interrupt Priority Level (IPL) Associated to Alternate Working Register 4 bits->Not Assigned


void print_debug(int value){
        char str[20]; 
        sprintf(str, "%d", value); 
        UartSendString(str);
        UartSendString("\r\n");
}

void PwmInit(void){

    // PLL settings to convert the 8MHz FRC to 500MHZ(=F_PGx_clk)
    // FPLLO = FPLLI (M/N1*N2*N3)
    // 500MHz = 8MHz*125/2
    // PGxPER = F_PGx_clk/F_PWM - 1 = (500MHz/20khz)-1 = 24999
    
	CLKDIVbits.PLLPRE = 2; 			// N1=2
	PLLFBDbits.PLLFBDIV = 125; 		// M=125
	PLLDIVbits.POST1DIV = 1; 		// N2=1
	PLLDIVbits.POST2DIV = 1;		// N3=1  
    PCLKCONbits.MCLKSEL = 2;        // Use PLL as clock input 

    // PG1CONL
    PG1CONLbits.CLKSEL = 1;         // Use MCLKSEL as clock source reg
    PG1CONLbits.MODSEL = 0b010;     // dual output
    PG1IOCONHbits.PENH = 1;         // PWM Generator controls the PWMxH output pin
    PG1IOCONHbits.PENL = 1;         // PWM Generator controls the PWMxL output pin
    PG1IOCONHbits.PMOD = 1;         // PWM Generator outputs operate in Independent mode
    PG1EVTLbits.PGTRGSEL = 0;
    PG1PER = PGxPER_VALUE;
    // PG2CONL
    PG2CONLbits.CLKSEL = 1;         // Use MCLKSEL as clock source reg
    PG2CONLbits.MODSEL = 0b010;     // dual output
    PG2IOCONHbits.PENH = 1;         // PWM Generator controls the PWMxH output pin
    PG2IOCONHbits.PENL = 1;         // PWM Generator controls the PWMxL output pin
    PG2IOCONHbits.PMOD = 1;         // PWM Generator outputs operate in Independent mode
    PG2CONHbits.SOCS = 1;
    PG2PER = PGxPER_VALUE;
    // PG3CONL
    PG3CONLbits.CLKSEL = 1;         // Use MCLKSEL as clock source reg
    PG3CONLbits.MODSEL = 0b010;     // dual output
    PG3IOCONHbits.PENH = 1;         // PWM Generator controls the PWMxH output pin
    PG3IOCONHbits.PENL = 1;         // PWM Generator controls the PWMxL output pin
    PG3IOCONHbits.PMOD = 1;         // PWM Generator outputs operate in Independent mode
    PG3CONHbits.SOCS = 1;
    PG3PER = PGxPER_VALUE;
    // PG4CONL
    PG4CONLbits.CLKSEL = 1;         // Use MCLKSEL as clock source reg
    PG4CONLbits.MODSEL = 0b010;     // dual output
    PG4IOCONHbits.PENH = 1;         // PWM Generator controls the PWMxH output pin
    PG4IOCONHbits.PENL = 1;         // PWM Generator controls the PWMxL output pin
    PG4IOCONHbits.PMOD = 1;         // PWM Generator outputs operate in Independent mode
    PG4CONHbits.SOCS = 1;
    PG4PER = PGxPER_VALUE;
    RPOR15bits.RP65R = 34;          // map PWM4L to RD8 
    RPOR15bits.RP72R = 35;          // map PWM4H to RD1
    
    // CurrentShunt-A
    // PWMS_B -> PWM4H nor PWM4L = PWM2L -> (not PWM4H) and (not PWM4L) = PWM3L
    LOGCONBbits.PWMS1B = 6;         // Input1 is PWM4H
    LOGCONBbits.PWMS2B = 7;         // Input2 is PWM4L
    LOGCONBbits.S1BPOL = 1;         // Input1 is inv
    LOGCONBbits.S2BPOL = 1;         // Input2 is inv 
    LOGCONBbits.PWMLFB = 1;         // PWMS_B function is and (and with inverted inputs = nor)
    LOGCONBbits.PWMLFBD = 2;        // PWMS_B output is PWM3L (RB11)
    
    // EN-A
    // PWMS_A -> PWM4H or PWM4L = PWM3H
    LOGCONAbits.PWMS1A = 6;         // Input1 is PWM4H
    LOGCONAbits.PWMS2A = 7;         // Input2 is PWM4L
    LOGCONAbits.S1APOL = 0;         // Input1 is positive logic
    LOGCONAbits.S2APOL = 0;         // Input2 is positive logic 
    LOGCONAbits.PWMLFA = 0;         // PWMS_B function is or
    LOGCONAbits.PWMLFAD = 2;        // PWMS_B output is PWM3H (RB10)
}

void ClcInit(void)
{
    // Set Inputs 
    RPINR45bits.CLCINAR = 53;       // Assign CLC Input A to RC5
    RPINR46bits.CLCINBR = 61;       // Assign CLC Input B to RC13
    RPINR46bits.CLCINCR = 60;       // Assign CLC Input C to RC12

    // Set Outputs
    RPOR0bits.RP32R = 40;           // map CLC1OUT to RB0
    RPOR1bits.RP34R = 41;           // map CLC2OUT to RB2

   
// EN-B with CLC1     
    // Set Data MUX 1,2,3 to I/O input
    CLC1SELbits.DS1 = 0;            // CLCINA I/O pin
    CLC1SELbits.DS2 = 0;            // CLCINB I/O pin
    CLC1SELbits.DS3 = 0;            // CLCINC I/O pin    
    
    // All 3 Data MUX inputs go into Data GATE 1
    CLC1GLSLbits.G1D1T = 1;
    CLC1GLSLbits.G1D2T = 1;
    CLC1GLSLbits.G1D3T = 1;
    
    CLC1CONHbits.G1POL = 0;         // Data GATE 1 not inverted
    CLC1CONLbits.MODE = 1;          // Combinatorial Option: OR - XOR
    CLC1CONLbits.LCOUT = 1;         // The output of the module is not inverted
    CLC1CONLbits.LCPOL = 0;         // CLC1 output high
    CLC1CONLbits.LCOE = 1;          // CLC1 port pin output is enabled
    CLC1CONLbits.INTN = 0;          // interrupt will not be generated
    CLC1CONLbits.INTP = 0;          // interrupt will not be generated
    CLC1CONLbits.LCEN = 1;          // CLC1 is enabled

// Parasitic Shunt-B with CLC2 
    // Set Data MUX 1,2,3 to I/O input
    CLC2SELbits.DS1 = 0;            // CLCINA I/O pin
    CLC2SELbits.DS2 = 0;            // CLCINB I/O pin
    CLC2SELbits.DS3 = 0;            // CLCINC I/O pin
    
    // All 3 Data MUX inputs go into Data GATE 1
    CLC2GLSLbits.G2D1T = 1;
    CLC2GLSLbits.G2D2T = 1;
    CLC2GLSLbits.G2D3T = 1;
    
    CLC2CONHbits.G2POL = 0;         // Data GATE 1 not inverted
    CLC2CONLbits.MODE = 1;          // Combinatorial Option: OR ? XOR
    CLC2CONLbits.LCOUT = 1;         // The output of the module is not inverted
    CLC2CONLbits.LCPOL = 1;         // CLC2 output high
    CLC2CONLbits.LCOE = 1;          // CLC2 port pin output is enabled
    CLC2CONLbits.INTN = 0;          // interrupt will not be generated
    CLC2CONLbits.INTP = 0;          // interrupt will not be generated
    CLC2CONLbits.LCEN = 1;          // CLC2 is enabled
}


void PwmSet(uint16_t value1, uint16_t value2, uint16_t value3, uint16_t value4, uint16_t value5)
{
    
    PG1CONLbits.ON = 0;
    PG2CONLbits.ON = 0;
    PG3CONLbits.ON = 0;
    PG4CONLbits.ON = 0;

    //PWM1L, RB15
    PG1TRIGA = 0;
    PG1TRIGB = value1;

    //PWM1H, RB14
    PG1PHASE = 0;
    PG1DC = value2;

    //PWM2H, RB12
    PG2PHASE = 0;
    PG2DC = value3;

    //PWM4L, RD8
    PG4TRIGA = 0;
    PG4TRIGB = value4;

    //PWM4H, RD1
    PG4PHASE = 0;
    PG4DC = value5;
    
    PG1CONLbits.ON = 1;
    PG2CONLbits.ON = 1;
    PG3CONLbits.ON = 1;
    PG4CONLbits.ON = 1;
}

void UartInit(void)
{
    U1MODE = 0;
    U1MODEH = 0;    
    U1STA = 0;
    U1STAH = 0;
    U1BRG = 25;  // 9600 @ Fosc 200MHz

    U1STAHbits.URXISEL = 6;
    U1MODEHbits.BCLKSEL = 0;
    U1MODEbits.UTXEN = 1;
    U1MODEbits.URXEN = 1;
    U1MODEbits.UARTEN = 1;
    
    //DEBUGGER USB RX/TX TEST - FOR OOB DEMO EFFORT
    RPOR13bits.RP58R = 1;
    RPINR18bits.U1RXR = 59;
}

uint8_t UART1_Read(void)
{
    while((U1STAHbits.URXBE == 1))
    {
        
    }

    if ((U1STAbits.OERR == 1))
    {
        U1STAbits.OERR = 0;
    }
    
    return U1RXREG;
}

void UartSendByte(char data)
{
    while (U1STAbits.TRMT == 0)
    {
    } // wait for room in TX buffer

    U1TXREG = data;
} 

void UartSendString(char *pData)
{
    unsigned int x = 0;

    while(pData[x] != 0)
    {
        UartSendByte(pData[x]);
        x = x + 1;
    }
}

int main(void)
{ 
    char data[] = "\n\r Waiting for data in the format: value1,value2,value3,value4,value5; \n\r The SUM of all values has to be <=25599\n\r";

    _ANSELD13 = 0; //make RD13 a digital input
    _CNPUD13 = 1; //turn on internal pull up for RD13 
    _TRISD13 = 1; //RD13 an input 
    _TRISD10 = 0; // make trisd10 an output

    UartInit();
    PwmInit();
    ClcInit();
    PwmSet(0,0,0,0,0);

    char recv[50] = "";       // Initializing with an empty string
    memset(recv, 0, sizeof recv);
    uint16_t value1 = 0;
    uint16_t value2 = 0;
    uint16_t value3 = 0;
    uint16_t value4 = 0;
    uint16_t value5 = 0;
    PwmSet(1000,0,0,1000,0); 
    while(1)
    {s
        //UartSendString(data);
        int i = 0;
        while(recv[i] = UART1_Read(), recv[i] != ';'){
            i=i+1;
        }
        sscanf(recv, "%d,%d,%d,%d,%d,%d;", &value1, &value2, &value3, &value4, &value5);
        PwmSet(value1,value2,value3,value4,value5);
        UartSendString(recv);
        UartSendString(" ok\n\r");
        memset(recv, 0, sizeof recv);
        //SpiRecv();
  }
}