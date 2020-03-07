#include "base64.h"

#define NOT_NULL(p) ((p)!=NULL)

static const char pTable[] =
{
    'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P',
    'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', 'a', 'b', 'c', 'd', 'e', 'f',
    'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v',
    'w', 'x', 'y', 'z', '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '+', '/',
};

bool base64_encode(const char *pin, const size_t insize, char *pout, const size_t outsize)
{
    if (NOT_NULL(pin) && insize > 0 && NOT_NULL(pout) && outsize > 0)
    {
        size_t in1, in2, in3, out1, out2, out3, out4;
        for (size_t inpos = 0, outpos = 0; inpos < insize && outpos < outsize; inpos+=3, outpos+=4)
        {
            in1 = pin[inpos];
            in2 = (inpos+1) < insize ? pin[inpos+1] : 0;
            in3 = (inpos+2) < insize ? pin[inpos+2] : 0;
            out1 = in1 >> 2;
            out2 = in2 >> 4 | ((in1 & 0x03) << 4);
            out3 = in3 >> 6 | ((in2 & 0x0f) << 2);
            out4 = in3 & 0x3f;

            if (outpos+3 < outsize)
                pout[outpos+3] = inpos+2 < insize ? pTable[out4] : '=';
            if (outpos+2 < outsize)
                pout[outpos+2] = inpos+1 < insize ? pTable[out3] : '=';
            if (outpos+1 < outsize)
                pout[outpos+1] = pTable[out2];
            pout[outpos] = pTable[out1];
        }
        return true;
    }
    return false;
}

bool base64_decode(const char *pin, size_t insize, char *pout, size_t outsize)
{
    return true;
}

