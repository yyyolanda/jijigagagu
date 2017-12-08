typedef unsigned int   uint;
typedef unsigned short ushort;
typedef unsigned char  uchar;
typedef uint pde_t;

typedef struct{
            volatile uint locked;
        } sploc; // spinlock
#define MAXTHREADN 100

typedef struct{
            volatile uint is_available[MAXTHREADN];
        } arrloc;

typedef struct{
            volatile unsigned long seq;
            volatile uint locked;
        } seqloc;
