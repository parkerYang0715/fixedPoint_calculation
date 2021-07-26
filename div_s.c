#include<stdio.h>
#include<stdlib.h>
#define MAX_32 (Word32)0x7fffffffL
#define MIN_32 (Word32)0x80000000L
#define MAX_16 (Word16)0x7fff
#define MIN_16 (Word16)0x8000
typedef short Word16;
typedef int Word32;
Word16 div_s(Word16 var1, Word16 var2);
Word16 div_s_my(Word16 var1, Word16 var2);
Word32 L_sub(Word32 L_var1, Word32 L_var2);
Word32 L_deposit_l(Word16 var1);
Word16 add(Word16 var1, Word16 var2);
Word16 sub(Word16 var1, Word16 var2);
Word16 shr(Word16 var1, Word16 var2);
Word16 shl(Word16 var1, Word16 var2);
static Word16 Sqrt16_common(Word16 m, Word16 e);
Word16 mult_r(Word16 var1, Word16 var2);
Word16 mac_r(Word32 L_var3, Word16 var1, Word16 var2);
Word32 L_mac(Word32 L_var3, Word16 var1, Word16 var2);
Word32 L_mult(Word16 var1, Word16 var2);
Word16 norm_s(Word16 var1);

static Word16 saturate(Word32 L_var1);
Word16 extract_l(Word32 L_var1);
Word16 extract_h(Word32 L_var1);

const Word32 SqrtTable[32] = {
    /* Q31 */
    0x5A82D429, 0x5BEA10FE, 0x5D4BE6E5, 0x5EA89270, 0x60004BE2, 0x615347A1, 0x62A1B68C, 0x63EBC651,
    0x6531A1B5, 0x667370D4, 0x67B1595F, 0x68EB7EC8, 0x6A220277, 0x6B5503F0, 0x6C84A0F9, 0x6DB0F5BD,
    0x6EDA1CE9, 0x70002FC7, 0x7123465A, 0x72437773, 0x7360D8C5, 0x747B7EFA, 0x75937DC4, 0x76A8E7EB,
    0x77BBCF60, 0x78CC4545, 0x79DA5A00, 0x7AE61D3E, 0x7BEF9E07, 0x7CF6EAC2, 0x7DFC113F, 0x7EFF1EC0,
};

const Word16 SqrtDiffTable[32] = {
    /* Q21 */
    0x59CF, 0x5875, 0x572B, 0x55EE, 0x54BF, 0x539C, 0x5284, 0x5177, 0x5074, 0x4F7A, 0x4E89,
    0x4DA1, 0x4CC0, 0x4BE7, 0x4B15, 0x4A4A, 0x4985, 0x48C6, 0x480C, 0x4758, 0x46AA, 0x4600,
    0x455B, 0x44BA, 0x441D, 0x4385, 0x42F1, 0x4260, 0x41D3, 0x414A, 0x40C3, 0x4040,
};



void main(){
	Word16 tmp1 = (1<<4);
	Word16 tmp2 = (1<<8);
	Word16 ret = div_s(tmp1,tmp2);
	printf("ret = div_s(tmp1,tmp2)=%d\n",ret);
	ret = div_s_my(tmp1,tmp2);
	printf("ret = div_s_my(tmp1,tmp2)=%d\n\n",ret);
	
	tmp1 = -(1<<4);
	tmp2 = (1<<8);
	ret = div_s(tmp1,tmp2);
	printf("ret = div_s(tmp1,tmp2)=%d\n",ret);
	ret = div_s_my(tmp1,tmp2);
	printf("ret = div_s_my(tmp1,tmp2)=%d\n\n",ret);
	
	tmp1 = 859;
	tmp2 = 859;
	ret = div_s(tmp1,tmp2);
	printf("ret = div_s(tmp1,tmp2)=%d\n",ret);
	ret = div_s_my(tmp1,tmp2);
	printf("ret = div_s_my(tmp1,tmp2)=%d\n\n",ret);
	
	tmp1 = 52;
	tmp2 = 69;
	ret = div_s(tmp1,tmp2);
	printf("ret = div_s(tmp1,tmp2)=%d\n",ret);
	ret = div_s_my(tmp1,tmp2);
	printf("ret = div_s_my(tmp1,tmp2)=%d\n\n",ret);
	
	tmp1 = -599;
	tmp2 = 599;
	ret = div_s(tmp1,tmp2);
	printf("ret = div_s(tmp1,tmp2)=%d\n",ret);
	ret = div_s_my(tmp1,tmp2);
	printf("ret = div_s_my(tmp1,tmp2)=%d\n\n",ret);
}

Word16 div_s_my(Word16 var1, Word16 var2){
	if(var1 == var2)
		return MAX_16;
	if ( abs(var2) > abs(var1) ) {
		return (Word16)(((Word32)var1 << 15) / ((Word32)var2));
	}
	//return c;
}

Word16 div_s(Word16 var1, Word16 var2)
{
    Word16 var_out = 0;
    Word16 iteration;
    Word32 L_num;
    Word32 L_denom;
    if (var1 == 0)
    {
        var_out = 0;
    }
    else
    {
        if (var1 == var2)
        {
            var_out = MAX_16;
        }
        else
        {
            L_num = L_deposit_l(var1);
            L_denom = L_deposit_l(var2);



            for (iteration = 0; iteration < 15; iteration++)
            {
                var_out <<= 1;
                L_num <<= 1;

                if (L_num >= L_denom)
                {
                    L_num = L_sub(L_num, L_denom);
                    var_out = add(var_out, 1);
                }
            }
        }
    }

    return (var_out);
}
Word32 L_sub(Word32 L_var1, Word32 L_var2)
{
    Word32 L_var_out;
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
Word32 L_deposit_l(Word16 var1)
{
    Word32 L_var_out;

    L_var_out = (Word32)var1;

    return (L_var_out);
}
Word16 add(Word16 var1, Word16 var2)
{
    Word16 var_out;
    Word32 L_sum;

    L_sum = (Word32)var1 + var2;
    var_out = saturate(L_sum);
    return (var_out);
}

static Word16 saturate(Word32 L_var1)
{
    Word16 var_out;
    int Overflow = 0;

    if (L_var1 > 0X00007fffL)
    {
        Overflow = 1;
        var_out = MAX_16;
    }
    else if (L_var1 < (Word32)0xffff8000L)
    {
        Overflow = 1;
        var_out = MIN_16;
    }
    else
    {
        var_out = extract_l(L_var1);

    return (var_out);
}
}

Word16 extract_l(Word32 L_var1)
{
    Word16 var_out;
    var_out = (Word16)L_var1;

    return (var_out);
}

Word16 extract_h(Word32 L_var1)
{
    Word16 var_out;
    var_out = (Word16)(L_var1 >> 16);
    return (var_out);
}

Word16 sub(Word16 var1, Word16 var2)
{
    Word16 var_out;
    Word32 L_diff;

    L_diff = (Word32)var1 - var2;
    var_out = saturate(L_diff);

    return (var_out);
}

Word16 shr(Word16 var1, Word16 var2)
{
    Word16 var_out;
    int Overflow2 = 0;

    if (var2 < 0)
    {
        if (var2 < -16)
            var2 = -16;
        var2 = -var2;
        var_out = shl(var1, var2);

    }
    else
    {
        if (var2 >= 15)
        {
            var_out = (var1 < 0) ? -1 : 0;
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

Word16 shl(Word16 var1, Word16 var2)
{
    Word16 var_out;
    Word32 result;
    int Overflow = 0;

    if (var2 < 0)
    {
        if (var2 < -16)
            var2 = -16;
        var2 = -var2;
        var_out = shr(var1, var2);
    }
    else
    {
        result = (Word32)var1 * ((Word32)1 << var2);

        if ((var2 > 15 && var1 != 0) || (result != (Word32)((Word16)result)))
        {
            Overflow = 1;
            var_out = (var1 > 0) ? MAX_16 : MIN_16;
        }
        else
        {
            var_out = extract_l(result);
        }
    }
    return (var_out);
}
Word16 Sqrt16(                  /*!< output mantissa */
              Word16  mantissa, /*!< input mantissa */
              Word16 *exponent  /*!< pointer to exponent */
)
{
    Word16 preShift, e;

    //assert(mantissa >= 0);

    /* normalize */
    preShift = norm_s(mantissa);

    e        = sub(*exponent, preShift);
    mantissa = shl(mantissa, preShift);

    /* calc mantissa */
    mantissa = Sqrt16_common(mantissa, e);

    /* e = (e + 1) >> 1 */
    *exponent = mult_r(e, 1 << 14); 

    return mantissa;
}

static Word16 Sqrt16_common(Word16 m, Word16 e)
{
    Word16 index, frac;
    /* get table index (upper 6 bits minus 32) */
    /* index = (m >> 9) - 32; */
    index = mac_r(-32768 - (32 << 16), m, 1 << 6);

    /* get fractional part for interpolation (lower 9 bits) */
    //frac = s_and(m, 0x1FF); /* Q9 */
    frac = m & 0x1FF;

    /* interpolate */
    if (m != 0)
    {
        m = mac_r(SqrtTable[index], SqrtDiffTable[index], frac);
    }

    /* handle odd exponents */
    if ( (e & 1) != 0)
        m = mult_r(m, 0x5a82);

    return m;
}
Word16 mult_r(Word16 var1, Word16 var2)
{
    Word16 var_out;
    Word32 L_product_arr;

    L_product_arr = (Word32)var1 * (Word32)var2;       /* product */
    L_product_arr += (Word32)0x00004000L;      /* round */
    L_product_arr &= (Word32)0xffff8000L;
    L_product_arr >>= 15;       /* shift */

    if (L_product_arr & (Word32)0x00010000L)   /* sign extend when necessary */
    {
        L_product_arr |= (Word32)0xffff0000L ;
    }
    var_out = saturate(L_product_arr);

    return (var_out);
}

Word16 mac_r(Word32 L_var3, Word16 var1, Word16 var2)
{
    Word16 var_out;

    L_var3 = L_mac(L_var3, var1, var2);
    L_var3 += (Word32)0x00008000L;
    var_out = extract_h(L_var3);

    return (var_out);
}

Word32 L_mac(Word32 L_var3, Word16 var1, Word16 var2)
{
    Word32 L_var_out;
    Word32 L_product;

    L_product = L_mult(var1, var2);
    L_var_out = L_var3 + L_product;

    return (L_var_out);
}

Word32 L_mult(Word16 var1, Word16 var2)
{
    Word32 L_var_out;
    int Overflow = 0;

    L_var_out = (Word32)var1 * (Word32)var2;

    if (L_var_out != (Word32)0x40000000L)
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

Word16 norm_s(Word16 var1)
{
    if (var1 == (Word16)0xffff)
    {
        return 15;
    }
    Word16 var_out = 0;
    if (var1 < 0)
    {
        var1 = ~var1;
    }
    if (var1)
    {
        while (!(0x4000 & var1))
        {
            var_out++;
            if ((0x2000 & var1))
            {
                break;
            }
            var_out++;
            if ((0x1000 & var1))
            {
                break;
            }
            var_out++;
            if ((0x0800 & var1))
            {
                break;
            }
            var_out++;
            var1 <<= 4;
        }
    }
    return (var_out);
}