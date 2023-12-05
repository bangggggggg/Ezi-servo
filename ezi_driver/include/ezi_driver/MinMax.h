#ifndef _MINMAX_H_
#define _MINMAX_H_
//===========================================================================
//	File Name	: MinMax.h
//	Description : Min/Max Functions
//===========================================================================

#ifndef min
#define min(x, y)	(x < y) ? x : y;
#endif

#ifndef max
#define max(x, y)	(x < y) ? y : x;
#endif

#endif	// _MINMAX_H_