#ifndef _MEMSET_H_
#define _MEMSET_H_
//===========================================================================
//	File Name	: MemSet.h
//	Description : Memory Copy/Reset Functions.
//===========================================================================

#ifndef memset

static void *memset(void *dest, int c, unsigned int count);
static void *memcpy(void *dest, const void *src, unsigned int count);

void *memset(void *dest, int c, unsigned int count)
{
	unsigned int i;

	if ((dest == 0) || (count <= 0))
		return 0;

	for (i=0; i<count; i++)
		*((unsigned char*)dest + i) = c;

	return dest;
}

void *memcpy(void *dest, const void *src, unsigned int count)
{
	unsigned int i;

	if ((dest == 0) || (count <= 0))
		return 0;

	for (i=0; i<count; i++)
		*((unsigned char*)dest + i) = (unsigned char)*((unsigned char*)src + i);

	return dest;
}

#endif	// memset

#endif	// _MEMSET_H_
