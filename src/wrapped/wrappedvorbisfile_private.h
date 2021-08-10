#if defined(GO) && defined(GOM) && defined(GO2) && defined(DATA)

#ifdef NOALIGN
GO(ov_bitrate,iFpi)
GO(ov_bitrate_instant,iFp)
GO(ov_clear,iFp)
GO(ov_comment,pFpi)
GO(ov_crosslap,iFpp)
GO(ov_fopen,iFpp)
GO(ov_halfrate,iFpi)
GO(ov_halfrate_p,iFp)
GO(ov_info,pFpi)
GO(ov_open,iFpppi)
GOM(ov_open_callbacks,iFEpppipppp)  // ov_callbaks are not "by ref", so all 4 values are just on the stack
GO(ov_pcm_seek,iFpI)
GO(ov_pcm_seek_lap,iFpi)
GO(ov_pcm_seek_page,iFpI)
GO(ov_pcm_seek_page_lap,iFpI)
GO(ov_pcm_tell,IFp)
GO(ov_pcm_total,IFpi)
GO(ov_raw_seek,iFpi)
GO(ov_raw_seek_lap,iFpi)
GO(ov_raw_tell,IFp)
GO(ov_raw_total,IFpi)
GO(ov_read,iFppiiiip)
//GO(ov_read_filter,iFppiiiipBp)
GO(ov_read_float,iFppip)
GO(ov_seekable,iFp)
GO(ov_serialnumber,iFpi)
GO(ov_streams,iFp)
GO(ov_test,iFpppi)
//GO(ov_test_callbacks,iFpppiS)
GO(ov_test_open,iFp)
GO(ov_time_seek,iFpd)
GO(ov_time_seek_lap,iFpd)
GO(ov_time_seek_page,iFpd)
GO(ov_time_seek_page_lap,iFpd)
GO(ov_time_tell,dFp)
GO(ov_time_total,dFpi)
#else
GOM(ov_bitrate,iFEpi)
GOM(ov_bitrate_instant,iFEp)
GOM(ov_clear,iFEp)
GOM(ov_comment,pFEpi)
GOM(ov_crosslap,iFEpp)
GOM(ov_fopen,iFEpp)
GOM(ov_halfrate,iFEpi)
GOM(ov_halfrate_p,iFEp)
GOM(ov_info,pFEpi)
GOM(ov_open,iFEpppi)
GOM(ov_open_callbacks,iFEppplpppp)  // ov_callbaks are not "by ref", so all 4 values are just on the stack
GOM(ov_pcm_seek,iFEpI)
GOM(ov_pcm_seek_lap,iFEpI)
GOM(ov_pcm_seek_page,iFEpI)
GOM(ov_pcm_seek_page_lap,iFEpI)
GOM(ov_pcm_tell,IFEp)
GOM(ov_pcm_total,IFEpi)
GOM(ov_raw_seek,iFEpi)
GOM(ov_raw_seek_lap,iFEpi)
GOM(ov_raw_tell,IFEp)
GOM(ov_raw_total,IFEpi)
GOM(ov_read,iFEppiiiip)
//GO(ov_read_filter,iFppiiiipBp)
GOM(ov_read_float,iFEppip)
GOM(ov_seekable,iFEp)
GOM(ov_serialnumber,iFEpi)
GOM(ov_streams,iFEp)
GOM(ov_test,iFEpppi)
//GO(ov_test_callbacks,iFpppiS)
GOM(ov_test_open,iFEp)
GOM(ov_time_seek,iFEpd)
GOM(ov_time_seek_lap,iFEpd)
GOM(ov_time_seek_page,iFEpd)
GOM(ov_time_seek_page_lap,iFEpd)
GOM(ov_time_tell,dFEp)
GOM(ov_time_total,dFEpi)
#endif //NOALIGN

#endif