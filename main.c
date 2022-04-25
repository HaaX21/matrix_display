

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
    //PBCLK = SYSCLOCK / 2

    ANSELA = 0; //Odłączenie wszystkich portów od przetwornika ADC
    ANSELB = 0;

    RPB13Rbits.RPB13R = 0b0011; //RPB13 jako SDO1
    TRISBbits.TRISB13 = 0; //RB13 jak wyjście SDO1
    TRISBbits.TRISB14 = 0; //RB14 jako wyjście SCK1
    TRISBbits.TRISB15 = 0; //RB15 jak wyjście OE1

    //konfiguracja T32
    T2CON = 0;
    T3CON = 0;
    T2CONbits.TCKPS = 0b0111; //Częstotliwość licznika wynosi PBCLK / 256
    T2CONbits.T32 = 0b0001; //Liczniki T3 oraz T2 pracują razem jako licznik 32 bitowy 
    PR2 = 0xFFFF; //Przepełnienie następuje przy wartości 0xFFFFFFFFFF
    PR3 = 0xFFFF;
    T2CONbits.ON = 1; //Licznik zostaje uruchomiony

    //Poczekaj aż wartość licznika osiągnie liczbę 78125. Program czeka w tym miejscu przez 1000ms
    while ((TMR3 << 16 | TMR2) <= 78125) {
    }

    T2CONbits.ON = 0; //Licznik zostaje wyłączony
    TMR2 = 0; //Wyzerowanie wartości licznika
    TMR3 = 0; 

    SPI_config(); //Konfiguracja interfejsu SPI1
    Display_config(); //Konfiguracja sterowników MAX7219
    Display_clear(); //Wyczyszczenie wyświetlacza 
	
 //Wyświetlenie przykładowego tekstu na wyświetlaczu.
    Print_at_X_Y(0, 4, '1'); 
    Print_at_X_Y(8, 4, '4');
    Print_at_X_Y(16, 4, ':');
    Print_at_X_Y(24, 4, '2');
    Print_at_X_Y(32, 4, '2');
    Print_at_X_Y(40, 4, ':');
    Print_at_X_Y(48, 4, '3');
    Print_at_X_Y(56, 4, '6');
 //Na wyświetlaczy pojawi się napis "14:22:36"
    Send_data_to_display(); //Wysłanie danych do wyświetlacza 
	
    
    while (1) {
    }
}


