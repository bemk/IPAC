#ifndef X1205_INCLUDE
#define X1205_INCLUDE

/* prototypes */
void x1205Init(void);
int x1205Enable(void);
char x1205ReadByte(unsigned char addr);
int x1205WriteByte(unsigned char addr, unsigned char data);
int x1205ReadNByte(u_char addr, u_char *buf, u_char cnt );
int x1205WriteNBytes(unsigned char addr, unsigned char *data, u_char cnt );

#endif
