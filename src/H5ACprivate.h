/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Copyright by The HDF Group.                                               *
 * Copyright by the Board of Trustees of the University of Illinois.         *
 * All rights reserved.                                                      *
 *                                                                           *
 * This file is part of HDF5.  The full HDF5 copyright notice, including     *
 * terms governing use, modification, and redistribution, is contained in    *
 * the files COPYING and Copyright.html.  COPYING can be found at the root   *
 * of the source code distribution tree; Copyright.html can be found at the  *
 * root level of an installed copy of the electronic HDF5 document set and   *
 * is linked from the top-level documents page.  It can also be found at     *
 * http://hdfgroup.org/HDF5/doc/Copyright.html.  If you do not have          *
 * access to either file, you may request a copy from help@hdfgroup.org.     *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

/*-------------------------------------------------------------------------
 *
 * Created:		H5ACprivate.h
 *			Jul  9 1997
 *			Robb Matzke <matzke@llnl.gov>
 *
 * Purpose:		Constants and typedefs available to the rest of the
 *			library.
 *
 *-------------------------------------------------------------------------
 */

#ifndef _H5ACprivate_H
#define _H5ACprivate_H

#include "H5ACpublic.h"		/*public prototypes			*/

/* Pivate headers needed by this header */
#include "H5private.h"		/* Generic Functions			*/
#include "H5Cprivate.h"		/* Cache				*/
#include "H5Fprivate.h"		/* File access				*/

#ifdef H5_METADATA_TRACE_FILE
#define H5AC__TRACE_FILE_ENABLED	1
#else /* H5_METADATA_TRACE_FILE */
#define H5AC__TRACE_FILE_ENABLED	0
#endif /* H5_METADATA_TRACE_FILE */

/* Types of metadata objects cached */
typedef enum {
    H5AC_BT_ID = 0, 	/*B-tree nodes				     */
    H5AC_SNODE_ID,	/*symbol table nodes			     */
    H5AC_LHEAP_PRFX_ID, /*local heap prefix			     */
    H5AC_LHEAP_DBLK_ID, /*local heap data block			     */
    H5AC_GHEAP_ID,	/*global heap				     */
    H5AC_OHDR_ID,	/*object header				     */
    H5AC_OHDR_CHK_ID,	/*object header chunk			     */
    H5AC_BT2_HDR_ID,	/*v2 B-tree header			     */
    H5AC_BT2_INT_ID,	/*v2 B-tree internal node		     */
    H5AC_BT2_LEAF_ID,	/*v2 B-tree leaf node			     */
    H5AC_FHEAP_HDR_ID,	/*fractal heap header			     */
    H5AC_FHEAP_DBLOCK_ID, /*fractal heap direct block		     */
    H5AC_FHEAP_IBLOCK_ID, /*fractal heap indirect block		     */
    H5AC_FSPACE_HDR_ID,	/*free space header			     */
    H5AC_FSPACE_SINFO_ID,/*free space sections			     */
    H5AC_SOHM_TABLE_ID, /*shared object header message master table  */
    H5AC_SOHM_LIST_ID,  /*shared message index stored as a list      */
    H5AC_TEST_ID,	/*test entry -- not used for actual files    */
    H5AC_NTYPES		/* Number of types, must be last             */
} H5AC_type_t;

/* H5AC_DUMP_STATS_ON_CLOSE should always be FALSE when
 * H5C_COLLECT_CACHE_STATS is FALSE.
 *
 * When H5C_COLLECT_CACHE_STATS is TRUE, H5AC_DUMP_STATS_ON_CLOSE must
 * be FALSE for "make check" to succeed, but may be set to TRUE at other
 * times for debugging purposes.
 *
 * Hence the following, somewhat odd set of #defines.
 */
#if H5C_COLLECT_CACHE_STATS

#define H5AC_DUMP_STATS_ON_CLOSE	0

#else /* H5C_COLLECT_CACHE_STATS */

#define H5AC_DUMP_STATS_ON_CLOSE	0

#endif /* H5C_COLLECT_CACHE_STATS */

/* Default max metadata cache size and min clean size are give here.
 * At present, these are the same as those given in H5Cprivate.h.
 */

#define H5AC__DEFAULT_MAX_CACHE_SIZE	H5C__DEFAULT_MAX_CACHE_SIZE
#define H5AC__DEFAULT_MIN_CLEAN_SIZE	H5C__DEFAULT_MIN_CLEAN_SIZE


/*
 * Class methods pertaining to caching.	 Each type of cached object will
 * have a constant variable with permanent life-span that describes how
 * to cache the object.
 */

#define H5AC__SERIALIZE_RESIZED_FLAG	H5C__SERIALIZE_RESIZED_FLAG
#define H5AC__SERIALIZE_MOVED_FLAG	H5C__SERIALIZE_MOVED_FLAG

#define H5AC__CLASS_NO_FLAGS_SET 	H5C__CLASS_NO_FLAGS_SET
#define H5AC__CLASS_SPECULATIVE_LOAD_FLAG H5C__CLASS_SPECULATIVE_LOAD_FLAG
#define H5AC__CLASS_COMPRESSED_FLAG	 H5C__CLASS_COMPRESSED_FLAG

typedef H5C_get_load_size_func_t	H5AC_get_load_size_func_t;
typedef H5C_deserialize_func_t		H5AC_deserialize_func_t;
typedef H5C_image_len_func_t		H5AC_image_len_func_t;
typedef H5C_serialize_func_t		H5AC_serialize_func_t;
typedef H5C_free_icr_func_t		H5AC_free_icr_func_t;

typedef H5C_class_t			H5AC_class_t;

/* Cache entry info */
typedef H5C_cache_entry_t		H5AC_info_t;


/*===----------------------------------------------------------------------===
 *                             Protect Types
 *===----------------------------------------------------------------------===
 *
 * These are for the wrapper functions to H5AC_protect. They specify what
 * type of operation you're planning on doing to the metadata. The
 * Flexible Parallel HDF5 locking can then act accordingly.
 */

typedef enum H5AC_protect_t {
    H5AC_WRITE,                 /* Protect object for writing                */
    H5AC_READ                   /* Protect object for reading                */
} H5AC_protect_t;


/* Typedef for metadata cache (defined in H5Cpkg.h) */
typedef H5C_t	H5AC_t;

/* Metadata specific properties for FAPL */
/* (Only used for parallel I/O) */
#ifdef H5_HAVE_PARALLEL
/* Definitions for "block before metadata write" property */
#define H5AC_BLOCK_BEFORE_META_WRITE_NAME       "H5AC_block_before_meta_write"
#define H5AC_BLOCK_BEFORE_META_WRITE_SIZE       sizeof(unsigned)
#define H5AC_BLOCK_BEFORE_META_WRITE_DEF        0

/* Definitions for "library internal" property */
#define H5AC_LIBRARY_INTERNAL_NAME       "H5AC_library_internal"
#define H5AC_LIBRARY_INTERNAL_SIZE       sizeof(unsigned)
#define H5AC_LIBRARY_INTERNAL_DEF        0
#endif /* H5_HAVE_PARALLEL */

/* Dataset transfer property list for flush calls */
/* (Collective set, "block before metadata write" set and "library internal" set) */
/* (Global variable declaration, definition is in H5AC.c) */
extern hid_t H5AC_dxpl_id;

/* Dataset transfer property list for independent metadata I/O calls */
/* (just "library internal" set - i.e. independent transfer mode) */
/* (Global variable declaration, definition is in H5AC.c) */
extern hid_t H5AC_ind_dxpl_id;


/* Cache config field limit #defines */

#define H5AC__MIN_JBRB_BUF_SIZE        H5C__MIN_JBRB_BUF_SIZE
#define H5AC__MAX_JBRB_BUF_SIZE        H5C__MAX_JBRB_BUF_SIZE

#define H5AC__MIN_JBRB_NUM_BUFS        H5C__MIN_JBRB_NUM_BUFS
#define H5AC__MAX_JBRB_NUM_BUFS        H5C__MAX_JBRB_NUM_BUFS


/* Default cache configuration. */

#define H5AC__DEFAULT_CACHE_CONFIG                                            \
{                                                                             \
  /* int         version                = */ H5AC__CURR_CACHE_CONFIG_VERSION, \
  /* hbool_t     rpt_fcn_enabled        = */ FALSE,                           \
  /* hbool_t     open_trace_file        = */ FALSE,                           \
  /* hbool_t     close_trace_file       = */ FALSE,                           \
  /* char        trace_file_name[]      = */ "",                              \
  /* hbool_t     evictions_enabled      = */ TRUE,                            \
  /* hbool_t     set_initial_size       = */ TRUE,                            \
  /* size_t      initial_size           = */ (1 * 1024 * 1024),               \
  /* double      min_clean_fraction     = */ 0.5,                             \
  /* size_t      max_size               = */ (16 * 1024 * 1024),              \
  /* size_t      min_size               = */ (1 * 1024 * 1024),               \
  /* long int    epoch_length           = */ 50000,                           \
  /* enum H5C_cache_incr_mode incr_mode = */ H5C_incr__threshold,             \
  /* double      lower_hr_threshold     = */ 0.9,                             \
  /* double      increment              = */ 2.0,                             \
  /* hbool_t     apply_max_increment    = */ TRUE,                            \
  /* size_t      max_increment          = */ (4 * 1024 * 1024),               \
  /* enum H5C_cache_flash_incr_mode       */                                  \
  /*                    flash_incr_mode = */ H5C_flash_incr__add_space,       \
  /* double      flash_multiple         = */ 1.0,                             \
  /* double      flash_threshold        = */ 0.25,                            \
  /* enum H5C_cache_decr_mode decr_mode = */ H5C_decr__age_out_with_threshold, \
  /* double      upper_hr_threshold     = */ 0.999,                           \
  /* double      decrement              = */ 0.9,                             \
  /* hbool_t     apply_max_decrement    = */ TRUE,                            \
  /* size_t      max_decrement          = */ (1 * 1024 * 1024),               \
  /* int         epochs_before_eviction = */ 3,                               \
  /* hbool_t     apply_empty_reserve    = */ TRUE,                            \
  /* double      empty_reserve          = */ 0.1,                             \
  /* int	 dirty_bytes_threshold  = */ (256 * 1024)                     \
}


/* Default journal configuration. */

#define H5AC__DEFAULT_JNL_CONFIG                                             \
{                                                                             \
  /* int         version                 = */ H5AC__CURR_JNL_CONFIG_VER,     \
  /* hbool_t     enable_journaling       = */ FALSE,                          \
  /* char        journal_file_path[]     = */ "",                             \
  /* hbool_t     journal_recovered       = */ FALSE,                          \
  /* size_t      jbrb_buf_size           = */ (8 * 1024),                     \
  /* int         jbrb_num_bufs           = */ 2,                              \
  /* hbool_t     jbrb_use_aio            = */ FALSE,                          \
  /* hbool_t     jbrb_human_readable     = */ TRUE                            \
}


/*
 * Library prototypes.
 */

/* #defines of flags used in the flags parameters in some of the
 * following function calls.  Note that they are just copies of
 * the equivalent flags from H5Cprivate.h.
 */

#define H5AC__NO_FLAGS_SET		  H5C__NO_FLAGS_SET
#define H5AC__SET_FLUSH_MARKER_FLAG	  H5C__SET_FLUSH_MARKER_FLAG
#define H5AC__DELETED_FLAG		  H5C__DELETED_FLAG
#define H5AC__DIRTIED_FLAG		  H5C__DIRTIED_FLAG
#define H5AC__PIN_ENTRY_FLAG		  H5C__PIN_ENTRY_FLAG
#define H5AC__UNPIN_ENTRY_FLAG		  H5C__UNPIN_ENTRY_FLAG
#define H5AC__FLUSH_INVALIDATE_FLAG	  H5C__FLUSH_INVALIDATE_FLAG
#define H5AC__FLUSH_CLEAR_ONLY_FLAG	  H5C__FLUSH_CLEAR_ONLY_FLAG
#define H5AC__FLUSH_MARKED_ENTRIES_FLAG   H5C__FLUSH_MARKED_ENTRIES_FLAG
#define H5AC__FLUSH_IGNORE_PROTECTED_FLAG H5C__FLUSH_IGNORE_PROTECTED_FLAG


/* #defines of flags used to report entry status in the
 * H5AC_get_entry_status() call.
 */

#define H5AC_ES__IN_CACHE	0x0001
#define H5AC_ES__IS_DIRTY	0x0002
#define H5AC_ES__IS_PROTECTED	0x0004
#define H5AC_ES__IS_PINNED	0x0008


/* Forward declaration of structs used below */
struct H5O_loc_t;               /* Defined in H5Oprivate.h */

/* external function declarations: */

H5_DLL herr_t H5AC_init(void);

H5_DLL herr_t H5AC_check_for_journaling(H5F_t * f,
                                        hid_t dxpl_id,
                                        H5C_t * cache_ptr,
                                        hbool_t journal_recovered);

H5_DLL herr_t H5AC_deregister_mdjsc_callback(H5F_t * file_ptr,
                                             int32_t idx);

H5_DLL herr_t H5AC_create(const H5F_t *f, H5AC_cache_config_t *config_ptr);

H5_DLL herr_t H5AC_begin_transaction(hid_t id,
                                     hbool_t * do_transaction_ptr,
                                     struct H5O_loc_t * id_oloc_ptr,
                                     hbool_t * id_oloc_open_ptr,
                                     hbool_t * transaction_begun_ptr,
                                     uint64_t * trans_num_ptr,
                                     const char * api_call_name);

H5_DLL herr_t H5AC_end_transaction(hbool_t do_transaction,
                                   struct H5O_loc_t * id_oloc_ptr,
                                   hbool_t id_oloc_open,
                                   hbool_t transaction_begun,
				   hid_t dxpl_id,
                                   uint64_t trans_num,
                                   const char * api_call_name);

H5_DLL herr_t H5AC_get_entry_status(const H5F_t *f, haddr_t addr,
				    unsigned * status_ptr);
H5_DLL herr_t H5AC_insert_entry(H5F_t *f, hid_t dxpl_id, const H5AC_class_t *type,
                       haddr_t addr, void *thing, unsigned int flags);
H5_DLL herr_t H5AC_pin_protected_entry(void *thing);
H5_DLL void * H5AC_protect(H5F_t *f, hid_t dxpl_id, const H5AC_class_t *type,
                           haddr_t addr, void *udata, H5AC_protect_t rw);
H5_DLL herr_t H5AC_resize_entry(void *thing, size_t new_size);
H5_DLL herr_t H5AC_unpin_entry(void *thing);
H5_DLL herr_t H5AC_unprotect(H5F_t *f, hid_t dxpl_id,
                             const H5AC_class_t *type, haddr_t addr,
			     void *thing, unsigned flags);
H5_DLL herr_t H5AC_flush(H5F_t *f, hid_t dxpl_id, unsigned flags);
H5_DLL herr_t H5AC_mark_entry_dirty(void *thing);
H5_DLL herr_t H5AC_move_entry(H5F_t *f, const H5AC_class_t *type,
			   haddr_t old_addr, haddr_t new_addr);

H5_DLL herr_t H5AC_dest(H5F_t *f, hid_t dxpl_id);

H5_DLL herr_t H5AC_expunge_entry(H5F_t *f, hid_t dxpl_id,
                                 const H5AC_class_t *type, haddr_t addr);

H5_DLL herr_t H5AC_set_write_done_callback(H5C_t * cache_ptr,
                                           void (* write_done)(void));
H5_DLL herr_t H5AC_stats(const H5F_t *f);

H5_DLL herr_t H5AC_get_cache_auto_resize_config(const H5AC_t * cache_ptr,
                                               H5AC_cache_config_t *config_ptr);

H5_DLL herr_t H5AC_get_cache_size(H5AC_t * cache_ptr,
                                  size_t * max_size_ptr,
                                  size_t * min_clean_size_ptr,
                                  size_t * cur_size_ptr,
                                  int32_t * cur_num_entries_ptr);

H5_DLL herr_t H5AC_get_cache_hit_rate(H5AC_t * cache_ptr,
                                      double * hit_rate_ptr);

H5_DLL herr_t H5AC_get_jnl_config(H5AC_t * cache_ptr,
                                   H5AC_jnl_config_t * config_ptr);

H5_DLL herr_t H5AC_register_mdjsc_callback(const H5F_t * file_ptr,
                                         H5C_mdj_status_change_func_t fcn_ptr,
                                         void * data_ptr,
                                         int32_t * idx_ptr,
                                         H5C_mdj_config_t * config_ptr);

H5_DLL herr_t H5AC_reset_cache_hit_rate_stats(H5AC_t * cache_ptr);

H5_DLL herr_t H5AC_set_cache_auto_resize_config(H5AC_t *cache_ptr,
                                               H5AC_cache_config_t *config_ptr);

H5_DLL herr_t H5AC_set_jnl_config(H5F_t * f,
                                  hid_t dxpl_id,
                                  const H5AC_jnl_config_t *config_ptr,
				  hbool_t initializing);

H5_DLL herr_t H5AC_validate_config(H5AC_cache_config_t * config_ptr);

H5_DLL herr_t H5AC_validate_jnl_config(const H5AC_jnl_config_t *config_ptr);

H5_DLL hbool_t H5AC_validate_jnl_config_ver(int version_num);

H5_DLL hbool_t H5AC_validate_cache_config_ver(int version_num);

H5_DLL herr_t H5AC_close_trace_file(H5AC_t * cache_ptr);

H5_DLL herr_t H5AC_open_trace_file(H5AC_t * cache_ptr,
		                   const char * trace_file_name);

#endif /* !_H5ACprivate_H */

