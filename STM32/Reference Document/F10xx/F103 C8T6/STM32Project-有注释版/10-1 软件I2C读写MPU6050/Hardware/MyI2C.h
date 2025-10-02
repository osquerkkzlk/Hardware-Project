#ifndef __MYI2C_H
#define __MYI2C_H

void MYI2C_Init(void);
void MYI2C_W_SDA(uint8_t value);
uint16_t MYI2C_R_SDA(void);
void MYI2C_W_SCL(uint8_t value);
uint16_t MYI2C_R_SCL(void);

void MYI2C_Start(void);
void MYI2C_End(void);
void MYI2C_SendData(void);
uint16_t MYI2C_ReceiveData(void);
void MYI2C_Send_ACK(uint8_t ACK_bit);
uint8_t  MYI2C_Receive_ACK(void);
#endif
