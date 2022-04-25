#include <xc.h>

extern uint8_t Font[][8];
uint64_t ROWS[16];

void SPI1_write_to_buffer(uint8_t byte) {
    while (SPI1STATbits.SPITBF);
    SPI1BUF = byte;
}

void SPI1_send_data(uint8_t byte_1, uint8_t byte_2) {

    SPI1_write_to_buffer(byte_1);
    while (SPI1STATbits.SPIBUSY) {
    }
    SPI1_write_to_buffer(byte_2);
    while (SPI1STATbits.SPIBUSY) {
    }
}


void Display_config(void) {
    int i = 0;
    while (i < 16) {
        SPI1_send_data(0x0F, 0x00);
        i++;
    }
    LATBbits.LATB15 = 0;
    LATBbits.LATB15 = 1;
    i = 0;


    while (i < 16) {
        SPI1_send_data(0x0C, 0x01);
        i++;
    }
    LATBbits.LATB15 = 0;
    LATBbits.LATB15 = 1;
    i = 0;


    while (i < 16) {
        SPI1_send_data(0x0B, 0x07);
        i++;
    }

    LATBbits.LATB15 = 0;
    LATBbits.LATB15 = 1;
    i = 0;

    while (i < 16) {
        SPI1_send_data(0x0A, 0x08);
        i++;
    }

    LATBbits.LATB15 = 0;
    LATBbits.LATB15 = 1;
}

void SPI_config(void) {
    SPI1CON = 0;
    SPI1CON2 = 0;
    SPI1CONbits.MSTEN = 1;
    SPI1CONbits.CKP = 0;
    SPI1CONbits.CKE = 1;
    SPI1CONbits.DISSDI = 1;
    SPI1CONbits.ENHBUF = 0;
    SPI1BRG = 2;
    SPI1CONbits.ON = 1;

    LATBbits.LATB15 = 1; //OE default state is HIGH
}

void Display_clear(void) {
    int i = 1;
    int j = 0;
    while (i < 9) {
        while (j < 16) {
            SPI1_send_data(i, 0x00);
            j++;
        }
        j = 0;
        LATBbits.LATB15 = 0;
        LATBbits.LATB15 = 1;
        i++;
    }
}

uint8_t Flip_byte(uint8_t data) {
    uint8_t array_temp[8];
    uint8_t i = 0;
    while (i < 8) {
        array_temp[i] = (data >> i) & 0x01;
        i++;
    }
    return 128 * array_temp[0] + 64 * array_temp[1] + 32 * array_temp[2] + 16 * array_temp[3] + 8 * array_temp[4] + 4 * array_temp[5] + 2 * array_temp[6] + 1 * array_temp[7];
}

uint8_t Conversion(uint8_t data) {
    uint8_t temp;
    temp = data;
    data = (data >> 1) | (temp << 7);
    return data;
}

void Print_at_X_Y(uint8_t x, uint8_t y, uint8_t symbol) {
    uint8_t symbol_temp = symbol - 33;
    uint8_t i = 0;
    uint64_t temp = 0;
    while (i < 8) {
        temp = Flip_byte(Font[symbol_temp] [i]);
        ROWS[i + y] = ROWS[i + y] | (temp << x);
        i++;
    }
}

void Send_data_to_display(void) {
    uint8_t i = 1;
    uint8_t j = 0;
    while (i < 9) {
        while (j < 8) {
            SPI1_send_data(i, Conversion(Flip_byte((ROWS[8 + (i - 1)] >> 8 * j) & 0xFF)));
            j++;
        }
        j = 0;
        while (j < 8) {
            SPI1_send_data(i, Conversion(Flip_byte((ROWS[i - 1] >> 8 * (7 - j)) & 0xFF)));
            j++;
        }
        j = 0;
        i++;
        LATBbits.LATB15 = 0;
        LATBbits.LATB15 = 1;
    }
    i= 0;
    while (i < 16) {
        ROWS[i] = 0;
        i++;
    }
}