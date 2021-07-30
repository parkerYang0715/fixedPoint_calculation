#include<stdio.h>
#include<stdlib.h>
#define MAX_32 (int)0x7fffffffL
#define MIN_32 (int)0x80000000L
#define MAX_16 (short)0x7fff
#define MIN_16 (short)0x8000
#define x_len (3)

#define L_shl_pos(x, y) (L_shl((x), (y)))
#define L_shr_pos(x, y) (L_shr((x), (y)))

int L_add (int L_var1, int L_var2);
int L_sub (int L_var1, int L_var2);
short norm_s (short var1);
short norm_l (int L_var1);
int L_shl (int L_var1, short var2);
int L_shl_sat (int L_var1, short var2);
short shl (short var1, short var2);
int L_shr (int L_var1, short var2);
short shr (short var1, short var2);
short lshr(short var1, short var2);
short lshl(short var1, short var2);
short shr_sat (short var1, short var2);
int L_mac (int L_var3, short var1, short var2);
int L_mac0(int L_var3, short var1, short var2);
short mac_r(int L_var3, short var1, short var2);
int L_msu0 (int L_var3, short var1, short var2);
short mult(short var1, short var2);
int L_mult (short var1, short var2);
int L_mult0 (short var1,short var2);
short extract_l (int L_var1);
short extract_h (int L_var1);
short getScaleFactor32( const int *x,  const short  len_x);
short getScaleFactor32_0( const int *x,  const short  len_x);
static inline int L_max( int L_var1, int L_var2);
static inline int L_min( int L_var1, int L_var2);
static inline short s_min( short var1, short var2);
static inline short s_and( short var1, short var2);
short round_fx(int L_var1);

void main(){

	int *ptr;
	short s;
    ptr = (int*) malloc(x_len * sizeof(int));
	printf("\n ========== test 1 =============\n");
	*ptr = 883;
	*(ptr+1) =399197;
	*(ptr+2) =-20;
	for(int ii=0;ii<x_len;ii++){
		printf("ptr[%d]=%d\n",ii,ptr[ii]);
	}
	s=getScaleFactor32(ptr,x_len);
	printf("s=getScaleFactor32(ptr,x_len) %d\n",s);
	s=getScaleFactor32_0(ptr,x_len);
	printf("s=getScaleFactor32_0(ptr,x_len) %d\n",s);
	printf("\n ========== test 2 =============\n");
	*ptr = 103;
	*(ptr+1) =107;
	*(ptr+2) =-65520;
	for(int ii=0;ii<x_len;ii++){
		printf("ptr[%d]=%d\n",ii,ptr[ii]);
	}
	s=getScaleFactor32(ptr,x_len);
	printf("s=getScaleFactor32(ptr,x_len) %d\n",s);
	s=getScaleFactor32_0(ptr,x_len);
	printf("s=getScaleFactor32_0(ptr,x_len) %d\n",s);
	printf("\n ========== test 3 =============\n");
	int *p;
	p=ptr;
	for(int ii=0;ii<x_len;ii++){
		*p++ = 0;
		printf("ptr[%d]=%d\n",ii,ptr[ii]);
	}
	s=getScaleFactor32(ptr,x_len);
	printf("s=getScaleFactor32(ptr,x_len) %d\n",s);
	s=getScaleFactor32_0(ptr,x_len);
	printf("s=getScaleFactor32_0(ptr,x_len) %d\n",s);
	free(ptr);
}
short getScaleFactor32(                    /* o: measured headroom in range [0..31], 0 if all x[i] == 0 */
                        const int *x,    /* i: array containing 32-bit data */
                        const short  len_x) /* i: length of the array to scan  */
{
    int i;
    short  i_min, i_max;
    int  x_min, x_max;

    x_max = L_add(0, 0);
    x_min = L_add(0, 0);
    for (i = 0; i < len_x; i++)
    {
        if (x[i] >= 0)
            x_max = L_max(x_max, x[i]);
        if (x[i] < 0)
            x_min = L_min(x_min, x[i]);
    }

    i_max = 0x20;  // = 32 = 0b 0000 0000 0010 0000
    i_min = 0x20;

    if (x_max != 0)
        i_max = norm_l(x_max);   // if x_max == 0   =>   norm_l(x_max) = 0

    if (x_min != 0)
        i_min = norm_l(x_min);

    i = s_and(s_min(i_max, i_min), 0x1F); //0x1F = 31 = 0b 0001 1111
	
	// if x_max=0&& x_min = 0  =>  i = 0 & 0x1F =0

    return i;
}

short getScaleFactor32_0(                    /* o: measured headroom in range [0..31], 32 if all x[i] == 0 */
                          const int *x,    /* i: array containing 32-bit data */
                          const short  len_x) /* i: length of the array to scan  */
{
    int i;
    short  i_min, i_max;
    int  x_min, x_max;

    x_max = L_add(0, 0);
    x_min = L_add(0, 0);
    for (i = 0; i < len_x; i++)
    {/*
        if (x[i] >= 0)
            x_max = L_max(x_max, x[i]);
        if (x[i] < 0)
            x_min = L_min(x_min, x[i]);*/
        /*way2*/
        if (x[i] > x_max)
            x_max = x[i];
        if (x[i] < x_min)
            x_min = x[i];
    }

    i_max = 0x20;  // = 32 = 0b 0000 0000 0010 0000
    i_min = 0x20; 

    if (x_max != 0)
        i_max = norm_l(x_max);     // if x_max == 0   =>   norm_l(x_max) = 0

    if (x_min != 0)
        i_min = norm_l(x_min);

    i = s_min(i_max, i_min);
	// if x_max=0&& x_min = 0  =>  i = 0 & 0x1F =0  i = 0
    return i;
}

short mult(short var1, short var2)
{
    short var_out;
    int L_product;

    L_product = (int)var1 * (int)var2;

    L_product = (L_product & (int)0xffff8000L) >> 15;

    if (L_product & (int)0x00010000L)
        L_product = L_product | (int)0xffff0000L;

    return (var_out);
}
int L_mult (short var1, short var2)
{
    int L_var_out;
    int Overflow = 0;

    L_var_out = (int) var1 *(int) var2;

    if (L_var_out != (int) 0x40000000L)
    {
        L_var_out *= 2;
    }
    else
    {
        Overflow = 1;
        L_var_out = MAX_32;
    }

    return (L_var_out);
}

int L_mult0 (short var1,short var2)
{
  int L_var_out;

  L_var_out = (int)var1 * (int)var2;

  return(L_var_out);
}

int L_add (int L_var1, int L_var2)
{
    int L_var_out;
    int Overflow = 0;

    L_var_out = L_var1 + L_var2;

    if (((L_var1 ^ L_var2) & MIN_32) == 0)
    {
        if ((L_var_out ^ L_var1) & MIN_32)
        {
            L_var_out = (L_var1 < 0) ? MIN_32 : MAX_32;
            Overflow = 1;
        }
    }

    return (L_var_out);
}

int L_sub (int L_var1, int L_var2)
{
    int L_var_out;
    int Overflow = 0;

    L_var_out = L_var1 - L_var2;

    if (((L_var1 ^ L_var2) & MIN_32) != 0)
    {
        if ((L_var_out ^ L_var1) & MIN_32)
        {
            L_var_out = (L_var1 < 0L) ? MIN_32 : MAX_32;
            Overflow = 1;
        }
    }

    return (L_var_out);
}

int L_msu0 (int L_var3, short var1, short var2) // L_var3 - var1*var2
{
  int L_var_out;
  int L_product;

  L_product = L_mult0(var1,var2);
  L_var_out = L_sub(L_var3,L_product);

  return(L_var_out);
}
int L_mac (int L_var3, short var1, short var2)  // L_var3 + 2*var1*var2
{
    int L_var_out;
    int L_product;

    L_product = L_mult (var1, var2);
	printf("L_mult(%d,%d)=%d\n",var1, var2, L_product);
    L_var_out = L_add (L_var3, L_product);

    return (L_var_out);
}
int L_mac0(int L_var3, short var1, short var2)
{
    int L_var_out;
    int L_product;

    L_product = L_mult0(var1, var2);
	printf("L_mult0(%d,%d)=%d\n",var1, var2, L_product);
    L_var_out = L_add(L_var3, L_product);
    return(L_var_out);
}
int L_shr (int L_var1, short var2)
{
    int L_var_out;
    int Overflow2 = 0;

    if (var2 < 0)
    {
        if (var2 < -32)
            var2 = -32;
        var2 = -var2;
        L_var_out = L_shl(L_var1, var2);

    }
    else
    {
        if (var2 >= 31)
        {
            L_var_out = (L_var1 < 0L) ? -1 : 0;
            if (var2 > 31) {
              Overflow2 = 1;
            }
        }
        else
        {
            if (L_var1 < 0)
            {
                L_var_out = ~((~L_var1) >> var2);
            }
            else
            {
                L_var_out = L_var1 >> var2;
            }
        }
    }

    return (L_var_out);
}
short shl (short var1, short var2)
{
    short var_out;
    int result;
    int Overflow = 0;

    if (var2 < 0)
    {
        if (var2 < -16)
            var2 = -16;
        var2 = -var2;
        var_out = shr (var1, var2);
    }
    else
    {
        result = (int) var1 *((int) 1 << var2);

        if ((var2 > 15 && var1 != 0) || (result != (int) ((short) result)))
        {
            Overflow = 1;
            var_out = (var1 > 0) ? MAX_16 : MIN_16;
        }
        else
        {
            var_out = extract_l (result);

        }
    }


    return (var_out);
}
short shr (short var1, short var2)
{
    short var_out;
    int Overflow2 = 0;

    if (var2 < 0)
    {
        if (var2 < -16)
            var2 = -16;
        var2 = -var2;
        var_out = shl (var1, var2);


    }
    else
    {
        if (var2 >= 15)
        {
            var_out = (var1 < 0) ? -1 : 0;
            if (var2 > 15) {
              Overflow2 = 1;
            }
        }
        else
        {
            if (var1 < 0)
            {
                var_out = ~((~var1) >> var2);
            }
            else
            {
                var_out = var1 >> var2;
            }
        }
    }



    return (var_out);
}
short shr_sat (short var1, short var2)
{
    int ret;
    //BASOP_SATURATE_WARNING_OFF
    ret = shr(var1, var2);
    //BASOP_SATURATE_WARNING_ON
    return ret;
}
int L_shl (int L_var1, short var2)
{

    int L_var_out = 0L;
    int Overflow = 0;

    if (var2 <= 0)
    {
        if (var2 < -32)
            var2 = -32;
        var2 = -var2;
        L_var_out = L_shr(L_var1, var2);
    }
    else
    {
        for (; var2 > 0; var2--)
        {
            if (L_var1 > (int) 0X3fffffffL)
            {
                Overflow = 1;
                L_var_out = MAX_32;
                break;
            }
            else
            {
                if (L_var1 < (int) 0xc0000000L)
                {
                    Overflow = 1;
                    L_var_out = MIN_32;
                    break;
                }
            }
            L_var1 *= 2;
            L_var_out = L_var1;
        }
    }
    return (L_var_out);
}

int L_shl_sat (int L_var1, short var2)
{
    int ret;
    //BASOP_SATURATE_WARNING_OFF
    ret = L_shl(L_var1, var2);
    //BASOP_SATURATE_WARNING_ON
    return ret;
}
short norm_s (short var1)
{
    short var_out;

    if (var1 == 0)
    {
        var_out = 0;
    }
    else
    {
        if (var1 == (short) 0xffff)
        {
            var_out = 15;
        }
        else
        {
            if (var1 < 0)
            {
                var1 = ~var1;
            }
            for (var_out = 0; var1 < 0x4000; var_out++)
            {
                var1 <<= 1;
            }
        }
    }

    return (var_out);
}
short norm_l (int L_var1)
{
    short var_out;

    if (L_var1 == 0)
    {
        var_out = 0;
    }
    else
    {
        if (L_var1 == (int) 0xffffffffL)
        {
            var_out = 31;
        }
        else
        {
            if (L_var1 < 0)
            {
                L_var1 = ~L_var1;
            }
            for (var_out = 0; L_var1 < (int) 0x40000000L; var_out++)
				// 0x40000000 = 100 0000 0000 0000 0000 0000 0000 0000 = 2^30
            {
                L_var1 <<= 1;
            }
        }
    }

    return (var_out);
}
short extract_l (int L_var1)
{
    short var_out;

    var_out = (short) L_var1;

    return (var_out);
}
short extract_h (int L_var1)
{
    short var_out;

    var_out = (short) (L_var1 >> 16);

    return (var_out);
}

static inline int L_max( int L_var1, int L_var2) {
   int L_var_out;

   if( L_var1 >= L_var2)
      L_var_out = L_var1;
   else
      L_var_out = L_var2;

   return( L_var_out);
}

static inline int L_min( int L_var1, int L_var2) {
   int L_var_out;

   if( L_var1 <= L_var2)
      L_var_out = L_var1;
   else
      L_var_out = L_var2;

   return( L_var_out);
}
static inline short s_and( short var1, short var2) {
   short var_out;

   var_out = var1 & var2;

   return( var_out);
}
static inline short s_min( short var1, short var2) {
   short var_out;

   if( var1 <= var2)
      var_out = var1;
   else
      var_out = var2;

   return( var_out);
}


short round_fx(int L_var1)
{
    short var_out;
    int L_rounded;

    L_rounded = L_add(L_var1, (int)0x00008000L);
    var_out = extract_h(L_rounded);

    return (var_out);
}

short mac_r(int L_var3, short var1, short var2)
{
    short var_out;

    L_var3 = L_mac(L_var3, var1, var2);
	printf("@mac_r L_var3 = L_mac(L_var3, var1, var2)=%d\n",L_var3);
    L_var3 = L_add(L_var3, (int)0x00008000L);
	printf("@mac_r L_var3 = L_add(L_var3, (int)0x00008000L)=%d\n",L_var3);
    var_out = extract_h(L_var3);

    return (var_out);
}

short lshr( short var1, short var2) {
   short var_out;
   int Overflow2 = 0;

   if( var2 < 0) {
      var2 = -var2;
      var_out = lshl( var1, var2);

   } else {
      if( var2 == 0 || var1 == 0) {
         var_out = var1;
      } else if( var2 >= 16) {
         var_out = 0;

         Overflow2 = 1;

      } else {
         var_out = var1 >> 1;
         printf("\n var_out = var1 >> 1=%d\n",var_out);
         var_out = var_out & 0x7fff;
		 printf("var_out = var_out & 0x7fff=%d\n",var_out);
         var_out =  var_out >> ( var2-1);

         if ( var2 >= 16 ) {
           Overflow2 = 1;
         }

      }
   }

   return( var_out);
}

short lshl( short var1, short var2) {
   short var_out=0;
   int Overflow2 = 0;

   if( var2 < 0) {
      var2 = -var2;
      var_out = lshr( var1, var2);

   } else {
      if( var2 == 0 || var1 == 0) {
         var_out = var1;
      } else if( var2 >= 16) {
         var_out = 0;

         Overflow2 = 1;

      } else {
         var_out = var1 << var2;
      }
   }

   return( var_out);
}