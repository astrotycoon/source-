#ifndef _TYPES_H_
#define _TYPES_H_

/* little/big endian stuff */

typedef	signed int	S32;
typedef unsigned int	U32;
typedef unsigned short 	U16;
typedef unsigned char	U8;

#define LE_16_TO_NATIVE(x)	(x)

#define GET32(ptr) (((U16)(((U8*)(ptr))[0]))+(((U16)(((U8*)(ptr))[1]))<<8)+(((U16)(((U8*)(ptr))[2]))<<16)+(((U16)(((U8*)(ptr))[3]))<<24))

#endif  /* _TYPES_H_ */
