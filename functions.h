#ifndef functions_H
#define functions_H

void Display_config(void);
void SPI_config(void);
void Display_clear(void);
void SPI1_write_to_buffer(uint8_t byte);
uint8_t Flip_byte(uint8_t data);
uint8_t Conversion(uint8_t data);
void SPI1_send_data(uint8_t byte_1, uint8_t byte_2);
void Print_at_X_Y(uint8_t x, uint8_t y, uint8_t symbol);
void Send_data_to_display(void);

#endif