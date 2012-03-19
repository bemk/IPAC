#ifndef X1205_INCLUDE
#define X1205_INCLUDE

/* prototypes */
void x1205Init(void);
int x1205Enable(void);
char x1205ReadByte(unsigned char addr);
int x1205WriteByte(unsigned char addr, unsigned char data);

#endif
