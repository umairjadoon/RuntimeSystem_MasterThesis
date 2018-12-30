
#ifndef LIB_INCLUDE_CONTEXT_H_
#define LIB_INCLUDE_CONTEXT_H_

typedef void*   fcontext_t;
typedef struct { void* regs[9]; }  fcontext_data_t;

#include <cstdint>
struct transfer_t
{
    fcontext_t  fctx;
    void    *   data;
};

extern "C"
void*  jump_fcontext_1( fcontext_t from,  fcontext_t to, void * vp);
extern "C"
void*  make_fcontext_1( void * sp, std::size_t size, void (* fn)( void * ) ); //changed it to transfer_t from void*





#endif /* LIB_INCLUDE_CONTEXT_H_ */
