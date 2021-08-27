/*******************************************************************
 * File automatically generated by rebuild_wrappers.py (v2.0.0.10) *
 *******************************************************************/
#ifndef __wrappedlibxcbrandrTYPES_H_
#define __wrappedlibxcbrandrTYPES_H_

#ifndef LIBNAME
#error You should only #include this file inside a wrapped*.c file
#endif
#ifndef ADDED_FUNCTIONS
#define ADDED_FUNCTIONS() 
#endif

typedef my_xcb_XXX_iterator_t (*XFp_t)(void*);
typedef my_xcb_cookie_t (*xFpu_t)(void*, uint32_t);
typedef my_xcb_cookie_t (*xFpuW_t)(void*, uint32_t, uint16_t);
typedef my_xcb_cookie_t (*xFpuu_t)(void*, uint32_t, uint32_t);
typedef my_xcb_cookie_t (*xFppu_t)(void*, void*, uint32_t);

#define SUPER() ADDED_FUNCTIONS() \
	GO(xcb_randr_get_screen_resources_current_outputs_end, XFp_t) \
	GO(xcb_randr_get_output_primary, xFpu_t) \
	GO(xcb_randr_get_output_primary_unchecked, xFpu_t) \
	GO(xcb_randr_get_screen_resources, xFpu_t) \
	GO(xcb_randr_get_screen_resources_current, xFpu_t) \
	GO(xcb_randr_get_screen_resources_outputs, xFpu_t) \
	GO(xcb_randr_get_screen_resources_unchecked, xFpu_t) \
	GO(xcb_randr_select_input, xFpuW_t) \
	GO(xcb_randr_select_input_checked, xFpuW_t) \
	GO(xcb_randr_query_version, xFpuu_t) \
	GO(xcb_randr_query_version_unchecked, xFpuu_t) \
	GO(xcb_randr_get_crtc_info, xFppu_t) \
	GO(xcb_randr_get_crtc_info_unchecked, xFppu_t) \
	GO(xcb_randr_get_output_info, xFppu_t) \
	GO(xcb_randr_get_output_info_unchecked, xFppu_t)

#endif // __wrappedlibxcbrandrTYPES_H_
