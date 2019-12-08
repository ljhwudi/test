#ifndef __DRV_AT24CXX_H__
#define __DRV_AT24CXX_H__

void AT24CXX_ByteWrite(uint8_t addr, uint8_t data);
uint8_t AT24CXX_RandomRead(uint8_t addr);
void AT24CXX_PageWrite(uint8_t addr, uint8_t *data, uint8_t n);
void AT24CXX_SeqRead(uint8_t addr, uint8_t *data, uint8_t n);

#endif //__DRV_AT24CXX_H__

