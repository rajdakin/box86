/*****************************************************************
 * File automatically generated by rebuild_wrappers.py (v1.2.0.09)
 *****************************************************************/
#ifndef __wrappedlibxcbshapeTYPES_H_
#define __wrappedlibxcbshapeTYPES_H_

#ifndef LIBNAME
#error You should only #include this file inside a wrapped*.c file
#endif
#ifndef ADDED_FUNCTIONS
#define ADDED_FUNCTIONS() 
#endif

typedef my_xcb_cookie_t (*XFpuuuwwu_t)(void*, uint32_t, uint32_t, uint32_t, int16_t, int16_t, uint32_t);

#define SUPER() ADDED_FUNCTIONS() \
	GO(xcb_shape_mask, XFpuuuwwu_t) \
	GO(xcb_shape_mask_checked, XFpuuuwwu_t)

#endif // __wrappedlibxcbshapeTYPES_H_
