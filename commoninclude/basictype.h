#if !defined(ZOSO_BASICTYPE_INCLUDED_)
#define ZOSO_BASICTYPE_INCLUDED_

#if !defined(_MSC_VER)
    #include <inttypes.h>
#endif

namespace zoso
{

#if !defined(NULL)
    #define NULL 0
#endif

#if !defined(byte)
    typedef unsigned char byte;
#endif

#if !defined(word)
    typedef unsigned short word;
#endif

#if !defined(dword)
    typedef unsigned long dword;
#endif

//#if !defined(_MSC_VER)
//    typedef uint64_t __int64;
//#endif

//#if !defined(qword)
//    typedef __int64 qword;
//#endif
/*
union easy_variant
{
    short   value_short;
    long    value_long;
//    __int64 value_int64;
    float   value_float;
    double  value_double;
};
*/
}

#endif
