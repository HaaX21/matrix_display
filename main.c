

// PIC32MX270F256B Configuration Bit Settings

// 'C' source line config statements

// DEVCFG3
#pragma config USERID = 0xFFFF          // Enter Hexadecimal value (Enter Hexadecimal value)
#pragma config PMDL1WAY = ON            // Peripheral Module Disable Configuration (Allow only one reconfiguration)
#pragma config IOL1WAY = ON             // Peripheral Pin Select Configuration (Allow only one reconfiguration)
#pragma config FUSBIDIO = ON            // USB USID Selection (Controlled by the USB Module)
#pragma config FVBUSONIO = ON           // USB VBUS ON Selection (Controlled by USB Module)

// DEVCFG2
#pragma config FPLLIDIV = DIV_2         // PLL Input Divider (2x Divider)
#pragma config FPLLMUL = MUL_20         // PLL Multiplier (20x Multiplier)
#pragma config UPLLIDIV = DIV_12        // USB PLL Input Divider (12x Divider)
#pragma config UPLLEN = OFF             // USB PLL Enable (Disabled and Bypassed)
#pragma config FPLLODIV = DIV_2         // System PLL Output Clock Divider (PLL Divide by 2)

// DEVCFG1
#pragma config FNOSC = FRCPLL           // Oscillator Selection Bits (Fast RC Osc with PLL)
#pragma config FSOSCEN = ON             // Secondary Oscillator Enable (Enabled)
#pragma config IESO = OFF               // Internal/External Switch Over (Disable)
#pragma config POSCMOD = OFF            // Primary Oscillator Configuration (Primary osc disabled)
#pragma config OSCIOFNC = OFF           // CLKO Output Signal Active on the OSCO Pin (Disabled)
#pragma config FPBDIV = DIV_2           // Peripheral Clock Divisor (Pb_Clk is Sys_Clk/2)
#pragma config FCKSM = CSDCMD           // Clock Switching and Monitor Selection (Clock Switch Disable, FSCM Disabled)
#pragma config WDTPS = PS1048576        // Watchdog Timer Postscaler (1:1048576)
#pragma config WINDIS = OFF             // Watchdog Timer Window Enable (Watchdog Timer is in Non-Window Mode)
#pragma config FWDTEN = OFF             // Watchdog Timer Enable (WDT Disabled (SWDTEN Bit Controls))
#pragma config FWDTWINSZ = WINSZ_25     // Watchdog Timer Window Size (Window Size is 25%)

// DEVCFG0
#pragma config JTAGEN = OFF              // JTAG Enable (JTAG Port Enabled)
#pragma config ICESEL = ICS_PGx1        // ICE/ICD Comm Channel Select (Communicate on PGEC1/PGED1)
#pragma config PWP = OFF                // Program Flash Write Protect (Disable)
#pragma config BWP = OFF                // Boot Flash Write Protect bit (Protection Disabled)
#pragma config CP = OFF                 // Code Protect (Protection Disabled)

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.

#include <xc.h>
#include <sys/attribs.h>
#include "./p32mx270f256b.h"
#include "functions.h"

uint8_t update = 0;

void main(void) {
    //SYSCLOCK = 40MHz
    //PBxCLK = SYSCLOCK / 2

    ANSELA = 0;
    ANSELB = 0;

    RPB13Rbits.RPB13R = 0b0011; //RPB13 as SDO1
    TRISBbits.TRISB13 = 0; //SDO1
    TRISBbits.TRISB14 = 0; //SCK1
    TRISBbits.TRISB15 = 0; //OE1

    //T23 configuration
    T2CON = 0;
    T3CON = 0;
    T2CONbits.TCKPS = 0b0111;
    T2CONbits.T32 = 0b0001;
    PR2 = 0xFFFF;
    PR3 = 0xFFFF;
    T2CONbits.ON = 1;

    //Wait 1000ms
    while ((TMR3 << 16 | TMR2) <= 78125) {
    }

    T2CONbits.ON = 0;
    TMR2 = 0;
    TMR3 = 0;

    SPI_config();
    Display_config();
    Display_clear();
    asm volatile("di");
    RTCC_Init();
    RTCC_Update();
    asm volatile("ei");

    while (1) {
        if (update != RTCTIMEbits.SEC01) {
            update = RTCTIMEbits.SEC01;
            Print_at_X_Y(0, 4, (RTCTIMEbits.HR10 + 48));
            Print_at_X_Y(8, 4, (RTCTIMEbits.HR01 + 48));
            Print_at_X_Y(16, 4, ':');
            Print_at_X_Y(24, 4, (RTCTIMEbits.MIN10 + 48));
            Print_at_X_Y(32, 4, (RTCTIMEbits.MIN01 + 48));
            Print_at_X_Y(40, 4, ':');
            Print_at_X_Y(48, 4, (RTCTIMEbits.SEC10 + 48));
            Print_at_X_Y(56, 4, (RTCTIMEbits.SEC01 + 48));
            Send_data_to_display();
        }

    }
}

