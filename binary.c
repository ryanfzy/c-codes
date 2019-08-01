#include <string.h>
#include "binary.h"
#include "mem_d.h"

#define _MALLOC(x) malloc_d(x, __FILE__, __LINE__)
#define _FREE free_d

#define BIN_LEN 256
#define CHAR_BITS 8
#define CHAR_NUM BIN_LEN/CHAR_BITS

typedef struct __num
{
    unsigned char bits[CHAR_NUM];
    struct __num *pnext;
} _num;

typedef struct __bin
{
    _num num;
    bool sig;
} _bin;

void bin_lshift(_bin*, int);
void bin_rshift(_bin*, int);
void _bin_add(_bin*, _bin*, _bin*);
void _bin_mul(_bin*, _bin*, _bin*);
void _bin_sub(_bin*, _bin*, _bin*);
void _bin_div(_bin*, _bin*, _bin*, _bin*);
void bin_printb(_bin*);
void bin_printx(_bin*);
void _bin_un2complement(_bin*);
bool _num_is_zero(_num *pn);
void _num_free(_num *pn);

bool bin_is_zero(Bin bin)
{
    _bin *pa = (_bin*)bin;
    if (pa != NULL)
        return _num_is_zero(&pa->num);
    return true;
}

void bin_set_sig(Bin bin, bool sig)
{
    _bin *pa = (_bin*)bin;
    if (pa != NULL)
        pa->sig = sig;
}

bool bin_get_sig(Bin a)
{
    _bin *pa = (_bin*)a;
    if (pa != NULL)
        return pa->sig;
    return true;
}

static char _x2ch(char ch)
{
    if (ch >= '0' && ch <= '9')
        return ch - '0';
    else if (ch >= 'a' && ch <= 'f')
        return ch - 'a' + 10;
    return ch;
}

static char _ch2x(char ch)
{
    if (ch >= 0 && ch <= 9)
        return ch + '0';
    else if (ch >= 10 && ch <= 16)
        return ch - 10 + 'a';
    return ch;
}

void _num_init(_num *pnum)
{
    if (pnum != NULL)
    {
        for (int i = 0; i < CHAR_NUM; i++)
            pnum->bits[i] = 0;
        pnum->pnext = NULL;
    }
}

void bin_init(_bin *pbin)
{
    if (pbin != NULL)
    {
        _num_init(&pbin->num);
        pbin->sig = true;
    }
}

_bin* _bin_create()
{
    _bin *pbin = _MALLOC(sizeof(_bin));
    bin_init(pbin);
    return pbin;
}

_num* _num_create()
{
    _num *pnum = _MALLOC(sizeof(_num));
    _num_init(pnum);
    return pnum;
}

void _bin_extend_if_null(_num **ppa, _num **ppb)
{
    if (*ppa == NULL)
    {
        (*ppb)->pnext = _num_create();
        *ppa = (*ppb)->pnext;
    }
}

_bin* _bin_create_x(const char *pstr, unsigned int isize)
{
    _bin *pa = _bin_create();
    _num *pn_tmp = &pa->num;
    _num *pn_next = pn_tmp;
    int inext = isize;
    while (inext > 0)
    {
        _bin_extend_if_null(&pn_next, &pn_tmp);
        for (int i = CHAR_NUM-1; i >= 0 && inext >= 0; i--)
        {
            char ch1 = _x2ch((--inext >= 0) ? pstr[inext] : 0);
            char ch2 = _x2ch((--inext >= 0) ? pstr[inext] : 0);
            pn_next->bits[i] = (ch2 << 4 | ch1);
        }
        pn_next = pn_next->pnext;
    }
    return pa;
}


Bin bin_create(const char *pstr)
{
    if (pstr != NULL)
    {
        int len = 0;
        for(; pstr[len] != '\0'; len++);
        return bin_create2(pstr, len);
    }
    return 0;
}

Bin bin_create2(const char *pstr, unsigned int isize)
{
    if (pstr != NULL)
    {
        bool sig = true;
        if (pstr[0] == '-')
        {
            sig = false;
            pstr++;
            isize--;
        }
        if (pstr[0] == 'x')
        {
            _bin *pa = _bin_create_x(pstr+1, isize-1);
            pa->sig = sig;
            return (Bin)pa;
        }
    }
    return 0;
}

_num* _num_create3(_num *pn)
{
    if (pn != NULL)
    {
        _num *pn2 = _num_create();
        memcpy(pn2->bits, pn->bits, sizeof(pn->bits));
        if (pn->pnext != NULL)
            pn2->pnext = _num_create3(pn->pnext);
        return pn2;
    }
    return NULL;
}

_bin* _bin_create3(_bin *pa)
{
    if (pa != NULL)
    {
        _bin *pa2 = _bin_create();
        memcpy(pa2->num.bits, pa->num.bits, sizeof(pa->num.bits));
        if (pa->num.pnext != NULL)
            pa2->num.pnext = _num_create3(pa->num.pnext);
        pa2->sig = pa->sig;
        return pa2;
    }
    return NULL;
}

Bin bin_create3(Bin a)
{
    _bin *pa = (_bin*)a;
    if (pa != NULL)
        return (Bin)_bin_create3(pa);
    return 0;
}

Bin bin_create_uint(unsigned int val)
{
    _bin *pa = _bin_create();
    for (int i = CHAR_NUM-1; i >= 0 && val > 0; i--)
    {
        char ch = val & 0xff;
        pa->num.bits[i] = ch;
        val >>= 8;
    }
    return (Bin)pa;
}

void _num_free(_num *pn)
{
    if (pn != NULL)
    {
        _num_free(pn->pnext);
        _FREE(pn);
    }
}

void _bin_destroy(_bin *pa)
{
    if (pa != NULL)
        _num_free(pa->num.pnext);
}

void _bin_free(_bin *pa)
{
    if (pa != NULL)
    {
        _bin_destroy(pa);
        _FREE(pa);
    }
}

void bin_free(Bin bin)
{
    _bin *pa = (_bin*)bin;
    if (pa != NULL)
        _bin_free(pa);
}

void _bin_init2(_bin *pa, _bin *pb)
{
    if (pa != NULL && pb != NULL)
    {
        memcpy(pa->num.bits, pb->num.bits, sizeof(pb->num.bits));
        if (pb->num.pnext != NULL)
        {
            _num_free(pa->num.pnext);
            pa->num.pnext = _num_create3(pb->num.pnext);
        }
        pa->sig = pb->sig;
    }
}

void _num_shift_next(_num **ppa, _num **ppb)
{
    *ppa = *ppb;
    *ppb = (*ppa)->pnext;
}

void _num_lshift1(_num *pn)
{
    if (pn != NULL)
    {
        int iBit = 0;
        _num *pn_tmp = pn;
        _num *pn_next = pn_tmp;
        while (iBit > 0 || pn_next != NULL)
        {
            _bin_extend_if_null(&pn_next, &pn_tmp);
            for (int i = CHAR_NUM-1; i >= 0; i--)
            {
                int inext = pn_next->bits[i] >> 7;
                pn_next->bits[i] = ((pn_next->bits[i] << 1) | iBit);
                iBit = inext;
            }
            _num_shift_next(&pn_tmp, &pn_next);
        }
    }
}

void bin_lshift(_bin *pbin, int iShift)
{
    if (pbin != NULL)
    {
        for (int i = 0; i < iShift; i++)
            _num_lshift1(&pbin->num);
    }
}

/*
// TODO
bool bin_init_istr(_bin *pbin, char *szStr, size_t iLen)
{
}
*/

bool _num_is_zero(_num *pn)
{
    if (pn != NULL)
    {
        for (int i = 0; i < CHAR_NUM; i++)
        {
            if (pn->bits[i] > 0)
                return false;
        }
    }
    return true;
}

void _num_clean(_num *pn)
{
    if (pn != NULL)
    {
        _num_clean(pn->pnext);
        if (pn->pnext != NULL && _num_is_zero(pn->pnext))
        {
            _num_free(pn->pnext);
            pn->pnext = NULL;
        }
    }
}

int _num_rshift1(_num *pn)
{
    int ibit = 0;
    if (pn != NULL)
    {
        ibit = _num_rshift1(pn->pnext);
        for (int i = 0; i < CHAR_NUM; i++)
        {
            int ibit_tmp = pn->bits[i] & 1;
            pn->bits[i] = (ibit << 7) | (pn->bits[i] >> 1);
            ibit = ibit_tmp;
        }
    }
    return ibit;
}

void bin_rshift(_bin *pa, int iShift)
{
    if (pa != NULL)
    {
        for (int i = 0; i < iShift; i++)
            _num_rshift1(&pa->num);
        _num_clean(&pa->num);
    }
}

Bin bin_add(Bin a, Bin b)
{
    Bin r = 0;
    bin_add2(a, b, &r);
    return r;
}

void bin_add2(Bin a, Bin b, Bin *ppr)
{
    _bin *pa = (_bin*)a;
    _bin *pb = (_bin*)b;
    _bin *pr = _bin_create();
    _bin_add(pa, pb, pr);
    bin_free(*ppr);
    *ppr = (Bin)pr;
}

void _bin_add(_bin *pa, _bin *pb, _bin *pr)
{
    if (pa != NULL && pb != NULL && pr != NULL)
    {
        _bin *px = _bin_create3(pa);
        _bin *py = _bin_create3(pb);
        if (px->sig && !py->sig)
        {
            py->sig = true;
            _bin_sub(px, py, pr);
        }
        else if (!px->sig && py->sig)
        {
            px->sig = true;
            _bin_sub(py, px, pr);
        }
        else if(!px->sig && !py->sig)
        {
            px->sig = true;
            py->sig = true;
            _bin_add(px, py, pr);
            pr->sig = false;
        }
        else
        {
            _num *pa_tmp = NULL, *pa_next = &px->num;
            _num *pb_tmp = NULL, *pb_next = &py->num;
            _num *pr_tmp = NULL, *pr_next = &pr->num;
            char chCarry = 0x00;
            while (chCarry > 0 || pa_next != NULL || pb_next != NULL)
            {
                _bin_extend_if_null(&pa_next, &pa_tmp);
                _bin_extend_if_null(&pb_next, &pb_tmp);
                _bin_extend_if_null(&pr_next, &pr_tmp);
    
                for (int i = CHAR_NUM-1; i >= 0; i--)
                {
                    for (int j = 0; j < 8; j++)
                    {
                        char chCur = 0x00;
                        int iMsk = 1 << j;
                        if ((pa_next->bits[i] & iMsk) == (pb_next->bits[i] & iMsk))
                        {
                            chCur = chCarry;
                            chCarry = (pa_next->bits[i] & iMsk) ? 0x01 : 0x00;
                        }
                        else
                            chCur = (chCarry == 0x01) ? 0x00 : 0x01;
                        if (chCur)
                            pr_next->bits[i] |= (chCur << j);
                        else
                            pr_next->bits[i] &= ~(1 << j);
                    }
                }
                _num_shift_next(&pa_tmp, &pa_next);
                _num_shift_next(&pb_tmp, &pb_next);
                _num_shift_next(&pr_tmp, &pr_next);
            }
        }
        _bin_free(px);
        _bin_free(py);
    }
}

Bin bin_mul(Bin a, Bin b)
{
    Bin r = 0;
    bin_mul2(a, b, &r);
    return r;
}

void bin_mul2(Bin a, Bin b, Bin *ppr)
{
    _bin *pa = (_bin*)a;
    _bin *pb = (_bin*)b;
    _bin *pr = _bin_create();
    _bin_mul(pa, pb, pr);
    bin_free(*ppr);
    *ppr = (Bin)pr;
}

void _bin_mul(_bin *pa, _bin *pb, _bin *pr)
{
    if (pa != NULL && pb != NULL && pr != NULL)
    {
        _bin *px = _bin_create3(pa);
        _bin *py = _bin_create3(pb);
        px->sig = true;
        py->sig = true;
        _num *pnext = &py->num;
        while (pnext != NULL)
        {
            for (int i = CHAR_NUM-1; i >= 0; i--)
            {
                for (int j = 0; j < 8; j++)
                {
                    //if (py->num.bits[i] & (1 << j))
                    if (pnext->bits[i] & (1 << j))
                        _bin_add(pr, px, pr);
                    bin_lshift(px, 1);
                }
            }
            pnext = pnext->pnext;
        }
        pr->sig = (pa->sig == pb->sig);
        _bin_free(px);
        _bin_free(py);
    }
}

int _num_eq(_num *pn, _num *pm)
{
    if (pn == NULL && pm == NULL)
        return 0;
    else if (pn != NULL && pm == NULL)
        return 1;
    else if (pn == NULL && pm != NULL)
        return -1;
    int ret = _num_eq(pn->pnext, pm->pnext);
    if (ret == 0)
    {
        for (int i = 0; i < CHAR_NUM; i++)
        {
            if (pn->bits[i] > pm->bits[i])
                return 1;
            else if (pn->bits[i] < pm->bits[i])
                return -1;
        }
    }
    return ret;
}

int _bin_eq(_bin *pa, _bin *pb)
{
    if (pa == NULL && pb == NULL)
        return 0;
    else if (pa != NULL && pb == NULL)
        return 1;
    else if (pa == NULL && pb != NULL)
        return -1;
    if (pa->sig && !pb->sig)
        return 1;
    else if (!pa->sig && pb->sig)
        return -1;
    int ret = _num_eq(&pa->num, &pb->num);
    if (!pa->sig && !pb->sig)
        ret = (ret == 1 ? -1 : (ret == -1 ? 1 : ret));
    return ret;
}

int bin_eq(Bin a, Bin b)
{
    _bin *pa = (_bin*)a;
    _bin *pb = (_bin*)b;
    if (pa != NULL && pb != NULL)
    {
        return _bin_eq(pa, pb);
    }
    return 0;
}

int _bin_get_startIndex(_bin *pa)
{
    if (pa != NULL)
    {
        _num *pn_next = &pa->num;
        for (; pn_next->pnext != NULL; pn_next = pn_next->pnext);

        for (int i = 0; i < CHAR_NUM; i++)
        {
            if (pn_next->bits[i] > 0)
            {
                for (int j = 7; j >= 0; j--)
                {
                    if (pn_next->bits[i] & (1 << j))
                        return i * CHAR_BITS + 8-j-1;
                }
            }
        }
        pn_next = pn_next->pnext;
    }
    return -1;
}

_bin* _bin_align(_bin *pa, _bin *pb)
{
    if (pa != NULL && pb != NULL)
    {
        int ia = _bin_get_startIndex(pa);
        int ib = _bin_get_startIndex(pb);
        _num *pn_next = &pa->num;
        int iext = -1;
        for (; pn_next != NULL; iext++, pn_next = pn_next->pnext);
        ib = ib + iext * BIN_LEN;
        _bin *pr = _bin_create3(pb);
        bin_lshift(pr, ib - ia);
        return pr;
    }
    return NULL;
}

Bin bin_mod(Bin a, Bin b)
{
    Bin r = 0;
    bin_mod2(a, b, &r);
    return r;
}

void bin_mod2(Bin a, Bin b, Bin *ppm)
{
    _bin *pa = (_bin*)a;
    _bin *pb = (_bin*)b;
    _bin r;
    bin_init(&r);
    _bin *pm = _bin_create();
    _bin_div(pa, pb, &r, pm);
    bin_free(*ppm);
    *ppm = (Bin)pm;
    _bin_destroy(&r);
}

Bin bin_div(Bin a, Bin b)
{
    Bin r = 0;
    bin_div2(a, b, &r);
    return r;
}

void bin_div2(Bin a, Bin b, Bin *ppr)
{
    Bin rem = 0;
    Bin ret = bin_div_rem(a, b, &rem);
    bin_free(*ppr);
    *ppr = (Bin)ret;
    bin_free(rem);
}

Bin bin_div_rem(Bin a, Bin b, Bin *pprem)
{
    _bin *pa = (_bin*)a;
    _bin *pb = (_bin*)b;
    _bin *pret = _bin_create();
    _bin *prem = _bin_create();
    _bin_div(pa, pb, pret, prem);
    bin_free(*pprem);
    *pprem = (Bin)prem;
    return (Bin)pret;
}

void _bin_un2complement(_bin *pa)
{
    if (pa != NULL)
    {
        _bin *pOne = (_bin*)bin_create("x1");
        _bin_sub(pa, pOne, pa);
        _num *pnext = &pa->num;
        while (pnext != NULL)
        {
            for (int i = 0; i < CHAR_NUM; i++)
                pnext->bits[i] = ~(pnext->bits[i]);
            pnext = pnext->pnext;
        }
        _bin_free(pOne);
    }
}

void _bin_div(_bin *pa, _bin *pb, _bin *pr, _bin *pm)
{
    if (pa != NULL && pb != NULL && pr != NULL)
    {
        _bin *px = _bin_create3(pa);
        _bin *pb2 = _bin_create3(pb);
        _bin *py = _bin_align(px, pb);
        px->sig = true;
        pb2->sig = true;
        py->sig = true;
        while (_bin_eq(py, pb2) >= 0)
        {
            bin_lshift(pr, 1);
            if (_bin_eq(px, py) >= 0)
            {
                _bin_sub(px, py, px);
                pr->num.bits[CHAR_NUM-1] |= 0x01;
            }
            bin_rshift(py, 1);
        }
        pr->sig = (pa->sig == pb->sig);
        if (pm != NULL)
        {
            if (!pa->sig)
            {
                // return positive reminder
                _bin_sub(px, pb, px);
                px->sig = true;
            }
            _bin_init2(pm, px);
        }
        _bin_free(py);
        _bin_free(pb2);
        _bin_free(px);
    }
}

Bin bin_sub(Bin a, Bin b)
{
    Bin r = 0;
    bin_sub2(a, b, &r);
    return r;
}

void bin_sub2(Bin a, Bin b, Bin *ppr)
{
    _bin *pa = (_bin*)a;
    _bin *pb = (_bin*)b;
    _bin *pr = _bin_create();
    _bin_sub(pa, pb, pr);
    bin_free(*ppr);
    *ppr = (Bin)pr;
}

void _bin_2complement(_bin *pa)
{
    if (pa != NULL)
    {
        _bin* pOne = (_bin*)bin_create("x1");
        _num *pn_next = &pa->num;
        while (pn_next != NULL)
        {
            for (int i = 0; i < CHAR_NUM; i++)
                pn_next->bits[i] = ~(pn_next->bits[i]);
            pn_next = pn_next->pnext;
        }
        _bin_add(pa, pOne, pa);
        _bin_free(pOne);
    }
}

bool _num_discard_first_digit(_num *pn)
{
    if (pn != NULL)
    {
        if (pn->pnext == NULL)
            return true;
        if (_num_discard_first_digit(pn->pnext))
        {
            _num_free(pn->pnext);
            pn->pnext = NULL;
        }
    }
    return false;
}

void _num_extend_if_required(_num *pn, _num *pm)
{
    if (pn == NULL)
        return;
    if (pn->pnext != NULL && pm->pnext == NULL)
        pm->pnext = _num_create();
    _num_extend_if_required(pn->pnext, pm->pnext);
}

void _bin_sub(_bin *pa, _bin *pb, _bin *pr)
{
    if (pa != NULL && pb != NULL && pr != NULL)
    {
        _bin *px =  _bin_create3(pa);
        _bin *py =  _bin_create3(pb);
        if (px->sig && !py->sig)
        {
            py->sig = true;
            _bin_add(px, py, pr);
        }
        else if (!px->sig && py->sig)
        {
            px->sig = true;
            py->sig = true;
            _bin_add(px, py, pr);
            pr->sig = false;
        }
        else if (!px->sig && !py->sig)
        {
            px->sig = true;
            py->sig = true;
            _bin_sub(py, px, pr);
        }
        else
        {
            _num_extend_if_required(&px->num, &py->num);
            bool sig = _bin_eq(px, py) >= 0;
            _bin_2complement(py);
            _bin_add(px, py, pr);
            // assume the first block is 1 so free it
            _num_discard_first_digit(&pr->num);
            if (!sig)
            {
                _bin_un2complement(pr);
                pr->sig = false;
            }
            _num_clean(&pr->num);
        }
        _bin_free(px);
        _bin_free(py);
    }
}

void _bin_reverse_str(char *szStr, size_t iLen)
{
    int iLastIndex = iLen - 1;
    for (int i = 0; i < iLen / 2; i++)
    {
        int iIndexToSwap = iLastIndex-i;
        if (iIndexToSwap > i)
        {
            char ch = szStr[i];
            szStr[i] = szStr[iIndexToSwap];
            szStr[iIndexToSwap] = ch;
        }
    }
}

void bin_printb(_bin *pbin)
{
    char szStr[257] = {0};
    bin2bstr((Bin)pbin, szStr, 256);
    int i = 0;
    for (; i < 257; i++)
    {
        if (szStr[i] == '1')
            break;
    }
    printf("%10s\n", szStr+i);
}

void bin_printx(_bin *pa)
{
    char szStr[258] = {0};
    bin2xstr2((Bin)pa, szStr, 257);
    printf("%s\n", szStr);
}

void bin2bstr(Bin bin, char *szStr, size_t iLen)
{
    _bin *pa = (_bin*)bin;
    if (pa != NULL && szStr != NULL && iLen > 0)
    {
        _num *pn = &pa->num;
        for (int i = BIN_LEN - 1, j = 0; i >= 0; i--, j++)
        {
            int iIndex = i / CHAR_BITS;
            int iBitOffset = CHAR_BITS - (i % CHAR_BITS) - 1;

            if (j < iLen)
                szStr[j] = (pn->bits[iIndex] & (1 << iBitOffset)) ? '1' : '0';
        }
    }
    _bin_reverse_str(szStr, iLen < BIN_LEN ? iLen : BIN_LEN);
}

void _remove_leading_zeros(char *str, size_t isize)
{
    if (str != NULL && isize > 0)
    {
        int istart = 0;
        for (; str[istart] == '0'; istart++);
        int iend = isize - istart;
        for (int i = 0; istart < isize; istart++, i++)
            str[i] = str[istart];
        str[iend] = '\0';
    }
}

void bin2xstr(Bin bin, char *szStr, size_t iLen)
{
    _bin *pa = (_bin*)bin;
    if (pa != NULL && szStr != NULL && iLen > 0)
    {
        _num *pn_next = &pa->num;
        //TODO: remove
        for (int i = 0; i < iLen; szStr[i++] = '0');

        int j = 0;
        while (pn_next != NULL && j < iLen) 
        {
            for (int i = CHAR_NUM-1; i >= 0 && j < iLen; i--, j+=2)
            {
                char ch1 = _ch2x(pn_next->bits[i] >> 4);
                char ch2 = _ch2x(pn_next->bits[i] & 0x0f);
                szStr[j] = ch2;
                if (j+1 < iLen)
                    szStr[j+1] = ch1;
            }
            pn_next = pn_next->pnext;
        }
        _bin_reverse_str(szStr, iLen);
        _remove_leading_zeros(szStr, iLen);
    }
}

void bin2xstr2(Bin bin, char *str, size_t len)
{
    _bin *pa = (_bin*)bin;
    if (pa != NULL && str != NULL && len > 0)
    {
        if (pa->sig)
            str[0] = '+';
        else
            str[0] = '-';
        str++;
        len--;
        bin2xstr(bin, str, len);
    }
}

/*
// float
void _fstr2bin(_bin *pbin, char *szStr, size_t iLen)
{
}

bool bin_init_fstr(_bin *pbin, char *szFloat, size_t iLen)
{
}

void _fstr_get_mantissa(_bin *pB1, _bin *pB2, _bin *pBRet, int *piExp)
{
}

void _fstr_add_parts(_bin *pB1, _bin *pB2, _bin *pBRet)
{
}
*/
