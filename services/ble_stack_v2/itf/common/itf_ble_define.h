/**
 ****************************************************************************************
 *
 * Copyright 2015-2021 BES.
 * All rights reserved. All unpublished rights reserved.
 *
 * No part of this work may be used or reproduced in any form or by any
 * means, or stored in a database or retrieval system, without prior written
 * permission of BES.
 *
 * Use of this work is governed by a license granted by BES.
 * This work contains confidential and proprietary information of
 * BES. which is protected by copyright, trade secret,
 * trademark and other intellectual property rights.
 *
 *
 ****************************************************************************************
 */

#ifndef ITF_BLE_DEFINE_H_
#define ITF_BLE_DEFINE_H_


/*
 * INCLUDE FILES
 ****************************************************************************************
 */
#include <stdbool.h>       // standard boolean definitions
#include <stddef.h>        // standard definitions
#include <stdint.h>        // standard integer definitions
#include "arch.h"
#include "compiler.h"
#include "itf_co_utils.h"
#include "itf_co_buf.h"
#include "itf_co_endian.h"
#include "itf_co_math.h"
#include "itf_co_version.h"

#if 0
#ifdef __cplusplus
extern "C" {
#endif

extern int rand (void);

/*
 * DEFINES
 ****************************************************************************************
 */

/*
 * ASSERTION CHECK
 ****************************************************************************************
 */
    /// Assertions showing a critical error that could require a full system reset
#define ASSERT_ERR(cond)                             { if (!(cond)) { TRACE(2,"line is %d file is %s", __LINE__, __FILE__); } }
    
    /// Assertions showing a critical error that could require a full system reset
#define ASSERT_INFO(cond, param0, param1)            { if (!(cond)) { TRACE(4,"line is %d file is %s, %d, %d", __LINE__, __FILE__, param0, param1); } }
    
    /// Assertions showing a non-critical problem that has to be fixed by the SW
#define ASSERT_WARN(cond, param0, param1)            { if (!(cond)) { TRACE(4,"line is %d file is %s, %d, %d", __LINE__, __FILE__, param0, param1); } }
    
#define DUMP_DATA(data, length) \
       // dump_data((uint8_t*)data, length)

#ifndef __ARRAY_EMPTY
#define __ARRAY_EMPTY 1
#endif

#ifndef __INLINE
#define __INLINE         inline
#endif
#ifndef __STATIC
#define __STATIC         static
#endif

/// size of meta-data variables 32 bytes
#define CO_BUF_META_DATA_SIZE (32 >> 2)

/// Get the number of elements within an array, give also number of rows in a 2-D array
#define ARRAY_LEN(array)   (sizeof((array))/sizeof((array)[0]))

/// macro to extract a field from a value containing several fields
/// @param[in] __r bit field value
/// @param[in] __f field name
/// @return the value of the register masked and shifted
#define GETF(__r, __f)                                                           \
    (( (__r) & (__f##_MASK) ) >> (__f##_LSB))

/// macro to set a field value into a value  containing several fields.
/// @param[in] __r bit field value
/// @param[in] __f field name
/// @param[in] __v value to put in field
#define SETF(__r, __f, __v)                                                      \
    do {                                                                         \
        ASSERT_INFO( ( ( ( (__v) << (__f##_LSB) ) & ( ~(__f##_MASK) ) ) ) == 0 ,(__f##_MASK), (__v)); \
        __r = (((__r) & ~(__f##_MASK)) | (__v) << (__f##_LSB));                  \
    } while (0)

/*
 * ENUMERATIONS DEFINITIONS
 ****************************************************************************************
 */
/// Buffer Error status codes
enum co_buf_err
{
    /// No Error
    CO_BUF_ERR_NO_ERROR             = 0x00,
    /// Invalid parameter(s)
    CO_BUF_ERR_INVALID_PARAM        = 0x01,
    /// Not enough resources
    CO_BUF_ERR_INSUFFICIENT_RESOURCE = 0x02,
    /// Resource is busy, operation cannot be performed
    CO_BUF_ERR_RESOURCE_BUSY        = 0x03,
};

/// Buffer meta-data bit field
enum co_buf_metadata_bf
{
    /// Size of meta-data data frozen. This size has a step of 4 bytes
    CO_BUF_METADATA_FROZEN_SIZE_LSB                  = 0,
    CO_BUF_METADATA_FROZEN_SIZE_MASK                 = 0x0F,
    /// If equals 1, a callback is executed before freeing the buffer.
    CO_BUF_METADATA_FREE_CB_POS                      = 4,
    CO_BUF_METADATA_FREE_CB_BIT                      = 0x10,
};

/// Status returned by generic packer-unpacker
enum CO_UTIL_PACK_STATUS
{
    CO_UTIL_PACK_OK,
    CO_UTIL_PACK_IN_BUF_OVFLW,
    CO_UTIL_PACK_OUT_BUF_OVFLW,
    CO_UTIL_PACK_WRONG_FORMAT,
    CO_UTIL_PACK_ERROR,
};

/*
 * TYPE DEFINITIONS
 ****************************************************************************************
 */
/// structure of a list
/*@TRACE*/
struct co_list_hdr
{
    /// Pointer to next co_list_hdr
    struct co_list_hdr *next;
};

/// simplify type name of list element header
typedef struct co_list_hdr co_list_hdr_t;

/// structure of a list
struct co_list
{
    /// pointer to first element of the list
    struct co_list_hdr *first;
    /// pointer to the last element
    struct co_list_hdr *last;

    #if (KE_PROFILING)
    /// number of element in the list
    uint32_t cnt;
    /// max number of element in the list
    uint32_t maxcnt;
    /// min number of element in the list
    uint32_t mincnt;
    #endif //KE_PROFILING
};

/// simplify type name of list
typedef struct co_list co_list_t;

/*
 * MACROS
 ****************************************************************************************
 */
/// pop a specific element from the list
#define CO_LIST_POP_ELT(list, elt) co_list_extract(&(list), &(elt->hdr));

/*
 * FUNCTION DECLARATIONS
 ****************************************************************************************
 */
/**
 ****************************************************************************************
 * @brief Initialize a list to defaults values.
 *
 * @param list           Pointer to the list structure.
 ****************************************************************************************
 */
void co_list_init(struct co_list *list);

/**
 ****************************************************************************************
 * @brief Construct a list of free elements representing a pool
 *
 * @param list           Pointer to the list structure
 * @param pool           Pointer to the pool to be initialized
 * @param elmt_size      Size of one element of the pool (in bytes)
 * @param elmt_cnt       Number of elements available in the pool
 ****************************************************************************************
 */
void co_list_pool_init(struct co_list *list,
                       void *pool,
                       size_t elmt_size,
                       uint32_t elmt_cnt);

/**
 ****************************************************************************************
 * @brief Add an element as last on the list.
 *
 * @param list           Pointer to the list structure
 * @param list_hdr       Pointer to the header to add at the end of the list
 *
 ****************************************************************************************
 */
void co_list_push_back(struct co_list *list, struct co_list_hdr *list_hdr);

/**
 ****************************************************************************************
 * @brief Append a sequence of elements at the end of a list.
 *
 * Note: the elements to append shall be linked together
 *
 * @param list           Pointer to the list structure
 * @param first_hdr      Pointer to the first element to append
 * @param last_hdr       Pointer to the last element to append
 ****************************************************************************************
 */
void co_list_push_back_sublist(struct co_list *list, struct co_list_hdr *first_hdr, struct co_list_hdr *last_hdr);

/**
 ****************************************************************************************
 * @brief Add an element as first on the list.
 *
 * @param list           Pointer to the list structure
 * @param list_hdr       Pointer to the header to add at the beginning of the list
 ****************************************************************************************
 */
void co_list_push_front(struct co_list *list, struct co_list_hdr *list_hdr);

/**
 ****************************************************************************************
 * @brief Extract the first element of the list.
 * @param list           Pointer to the list structure
 * @return The pointer to the element extracted, and NULL if the list is empty.
 ****************************************************************************************
 */
struct co_list_hdr *co_list_pop_front(struct co_list *list);

/**
 ****************************************************************************************
 * @brief Search for a given element in the list, and extract it if found.
 *
 * @param list           Pointer to the list structure
 * @param list_hdr       Element to extract
 *
 * @return true if the element is found in the list, false otherwise
 ****************************************************************************************
 */
bool co_list_extract(struct co_list *list, struct co_list_hdr *list_hdr);

/**
 ****************************************************************************************
 * @brief Extract an element when the previous element is known
 *
 * Note: the element to remove shall follow immediately the reference within the list
 *
 * @param list           Pointer to the list structure
 * @param elt_ref_hdr    Pointer to the referenced element (NULL if element to extract is the first in the list)
 * @param elt_to_rem_hdr Pointer to the element to be extracted
 ****************************************************************************************
 */
void co_list_extract_after(struct co_list *list, struct co_list_hdr *elt_ref_hdr, struct co_list_hdr *elt_to_rem_hdr);

/**
 ****************************************************************************************
 * @brief Extract a sub-list when the previous element is known
 *
 * Note: the elements to remove shall be linked together and  follow immediately the reference element
 *
 * @param[in]  list           Pointer to the list structure
 * @param[in]  ref_hdr        Pointer to the referenced element (NULL if first element to extract is first in the list)
 * @param[in]  last_hdr       Pointer to the last element to extract ()
 ****************************************************************************************
 */
void co_list_extract_sublist(struct co_list *list, struct co_list_hdr *ref_hdr, struct co_list_hdr *last_hdr);

/**
 ****************************************************************************************
 * @brief Searched a given element in the list.
 *
 * @param list           Pointer to the list structure
 * @param list_hdr       Pointer to the searched element
 *
 * @return true if the element is found in the list, false otherwise
 ****************************************************************************************
 */
bool co_list_find(struct co_list *list, struct co_list_hdr *list_hdr);

/**
 ****************************************************************************************
 * @brief Merge two lists in a single one.
 *
 * This function appends the list pointed by list2 to the list pointed by list1. Once the
 * merge is done, it empties list2.
 *
 * @param list1    Pointer to the destination list
 * @param list2    Pointer to the list to append to list1
 ****************************************************************************************
 */
void co_list_merge(struct co_list *list1, struct co_list *list2);

/**
 ****************************************************************************************
 * @brief Insert a given element in the list before the referenced element.
 *
 * @param list           Pointer to the list structure
 * @param elt_ref_hdr    Pointer to the referenced element
 * @param elt_to_add_hdr Pointer to the element to be inserted
 *
 * @return true if the element is found in the list, false otherwise
 ****************************************************************************************
 */
void co_list_insert_before(struct co_list *list,
                        struct co_list_hdr *elt_ref_hdr, struct co_list_hdr *elt_to_add_hdr);

/**
 ****************************************************************************************
 * @brief Insert a given element in the list after the referenced element.
 *
 * @param list           Pointer to the list structure
 * @param elt_ref_hdr    Pointer to the referenced element
 * @param elt_to_add_hdr Pointer to the element to be inserted
 *
 * @return true if the element is found in the list, false otherwise
 ****************************************************************************************
 */
void co_list_insert_after(struct co_list *list,
                        struct co_list_hdr *elt_ref_hdr, struct co_list_hdr *elt_to_add_hdr);


/**
 ****************************************************************************************
 * @brief Count number of elements present in the list
 *
 * @param list           Pointer to the list structure
 *
 * @return Number of elements present in the list
 ****************************************************************************************
 */
uint16_t co_list_size(struct co_list *list);

/**
 ****************************************************************************************
 * @brief Test if the list is empty.
 * @param list           Pointer to the list structure.
 * @return true if the list is empty, false else otherwise.
 ****************************************************************************************
 */
__INLINE bool co_list_is_empty(const struct co_list *const list)
{
    bool listempty;
    listempty = (list->first == NULL);
    return (listempty);
}

/**
 ****************************************************************************************
 * @brief Pick the first element from the list without removing it.
 *
 * @param list           Pointer to the list structure.
 *
 * @return First element address. Returns NULL pointer if the list is empty.
 ****************************************************************************************
 */
__INLINE struct co_list_hdr *co_list_pick(const struct co_list *const list)
{
    return(list->first);
}

/**
 ****************************************************************************************
 * @brief Pick last element from the list without removing it.
 *
 * @param list           Pointer to the list structure.
 *
 * @return Last element address. Returns NULL pointer if the list is empty.
 ****************************************************************************************
 */
__INLINE struct co_list_hdr *co_list_tail(const struct co_list *const list)
{
    return(list->last);
}


/**
 ****************************************************************************************
 * @brief Return following element of a list element.
 *
 * @param list_hdr     Pointer to the list element.
 *
 * @return The pointer to the next element.
 ****************************************************************************************
 */
__INLINE struct co_list_hdr *co_list_next(const struct co_list_hdr *const list_hdr)
{
    return(list_hdr->next);
}

/// MACRO to build a subversion field from the Minor and Release fields
#define CO_SUBVERSION_BUILD(minor, release)     (((minor) << 8) | (release))

/**
 ****************************************************************************************
 * Perform a division and ceil up the result
 *
 * @param[in] val Value to divide
 * @param[in] div Divide value
 * @return ceil(val/div)
 ****************************************************************************************
 */
#define CO_DIVIDE_CEIL(val, div) (((val) + ((div) - 1))/ (div))

/**
 ****************************************************************************************
 * Perform a division and round the result
 *
 * @param[in] val Value to divide
 * @param[in] div Divide value
 * @return round(val/div)
 ****************************************************************************************
 */
#define CO_DIVIDE_ROUND(val, div) (((val) + ((div) >> 1))/ (div))

/**
 ****************************************************************************************
 * Perform a modulo operation
 *
 * @param[in] val    Dividend
 * @param[in] div    Divisor
 * @return  val/div)
 ****************************************************************************************
 */
//#define CO_MOD(val, div) ((val) % (div))
static __INLINE uint32_t co_mod(uint32_t val, uint32_t div)
{
   ASSERT_ERR(div);
   return ((val) % (div));
}
#define CO_MOD(val, div) co_mod(val, div)


/// Buffer structure
typedef struct co_buf
{
    /// List header for chaining
    co_list_hdr_t hdr;
    /// Length of the data part
    uint16_t      data_len;
    /// Prefix length available
    uint16_t      head_len;
    /// Suffix length available
    uint16_t      tail_len;
    /// Pool identifier (@see enum co_buf_pool_id)
    uint8_t       pool_id;
    /// Acquisition counter
    uint8_t       acq_cnt;
    /// Meta-data variable that can be used for multiple purposes
    /// meta-data is always 32 bits aligned
    uint32_t      metadata[CO_BUF_META_DATA_SIZE];
    /// Pattern used to verify that meta-data didn�t overflow
    uint8_t       pattern;
    /// Meta-data bit field (@see enum co_buf_metadata_bf)
    uint8_t       metadata_bf;
    /// Padding
    uint16_t      padding;

    /// Variable buffer array that contains header, data and tail payload
    /// Length is buf_len = (head_len + data_len + tail_len)
    uint8_t       buf[__ARRAY_EMPTY];
} co_buf_t;

/**
 ****************************************************************************************
 * @brief This function is called when all software modules has release the buffer.
 *
 * @param[in] p_env   Pointer to environment that will be used as callback parameter.
 ****************************************************************************************
 */
typedef void (*co_buf_free_cb)(co_buf_t* p_buf, void* p_env);

__INLINE uint32_t co_rand_word(void)
{
    return (uint32_t)rand();
}

/**
 ****************************************************************************************
 * @brief Function to return the smallest of 2 unsigned 32 bits words.
 * @return The smallest value.
 ****************************************************************************************
 */
__INLINE uint32_t co_min(uint32_t a, uint32_t b)
{
    return a < b ? a : b;
}

/**
 ****************************************************************************************
 * @brief Function to return the greatest of 2 unsigned 32 bits words.
 * @return The greatest value.
 ****************************************************************************************
 */
__INLINE uint32_t co_max(uint32_t a, uint32_t b)
{
    return a > b ? a : b;
}

/// Number of '1' bits in values from 0 to 15, used to fasten bit counting
const unsigned char one_bits[] = {0,1,1,2,1,2,2,3,1,2,2,3,2,3,3,4};

const uint8_t co_null_key[KEY_LEN] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

/// Number of '1' bits in a byte
#define NB_ONE_BITS(byte)   (one_bits[byte & 0x0F] + one_bits[byte >> 4])

/**
 ****************************************************************************************
 * Count number of bit set to 1 in a value with variable length
 *
 * @param[in] p_val Pointer to value
 * @param[in] size  Number of Bytes
 * @return Number of bit counted
 ****************************************************************************************
 */
__INLINE uint8_t co_bit_cnt(const uint8_t* p_val, uint8_t size)
{
    uint8_t nb_bit = 0;
    while(size-- > 0)
    {
        nb_bit += NB_ONE_BITS(*p_val);
        p_val++;
    }
    return (nb_bit);
}

/**
 ****************************************************************************************
 * @brief Return value with one bit set.
 *
 * @param[in] pos Position of the bit to set.
 *
 * @return Value with one bit set.  There is no return type since this is a macro and this
 * will be resolved by the compiler upon assignment to an l-value.
 ****************************************************************************************
 */
#define CO_BIT(pos) (1UL<<(pos))

/**
 ****************************************************************************************
 * @brief Return value bit into a bit field.
 *
 * @param[in] bf  Bit Field
 * @param[in] pos Position of the bit
 *
 * @return value of a bit into a bit field
 ****************************************************************************************
 */
#define CO_BIT_GET(bf, pos) (((((uint8_t*)bf)[((pos) >> 3)])>>((pos) & 0x7)) & 0x1)

/**
 ****************************************************************************************
 * @brief Update value bit into a bit field.
 *
 * @param[in] bf  Bit Field
 * @param[in] pos Position of the bit
 * @param[in] val New value of the bit (0 or 1)
 ****************************************************************************************
 */
#define CO_BIT_SET(bf, pos, val) (((uint8_t*)bf)[((pos) >> 3)]) = ((((uint8_t*)bf)[((pos) >> 3)]) & ~CO_BIT(((pos) & 0x7))) \
                                                                | (((val) & 0x1) << ((pos) & 0x7))

/// count number of bit into a long field
#define CO_BIT_CNT(val) (co_bit_cnt((uint8_t*) &(val), sizeof(val)))


/// Increment value and make sure it's never greater or equals max (else wrap to 0)
#define CO_VAL_INC(_val, _max)      \
    (_val) = (_val) + 1;            \
    if((_val) >= (_max)) (_val) = 0


/// Add value and make sure it's never greater or equals max (else wrap)
/// _add must be less that _max
#define CO_VAL_ADD(_val, _add, _max)      \
    (_val) = (_val) + (_add);             \
    if((_val) >= (_max)) (_val) -= (_max)

/// sub value and make sure it's never greater or equals max (else wrap)
/// _sub must be less that _max
#define CO_VAL_SUB(_val, _sub, _max)      \
    if((_val) < (_sub)) (_val) += _max;   \
    (_val) = (_val) - (_sub)

/**
 ****************************************************************************************
 * @brief Align val on the multiple of 4 equal or nearest higher.
 * @param[in] val Value to align.
 * @return Value aligned.
 ****************************************************************************************
 */
#define CO_ALIGN4_HI(val) (((val)+3)&~3)


/**
 ****************************************************************************************
 * @brief Align val on the multiple of 4 equal or nearest lower.
 * @param[in] val Value to align.
 * @return Value aligned.
 ****************************************************************************************
 */
#define CO_ALIGN4_LO(val) ((val)&~3)

/**
 ****************************************************************************************
 * @brief Align val on the multiple of 2 equal or nearest higher.
 * @param[in] val Value to align.
 * @return Value aligned.
 ****************************************************************************************
 */
#define CO_ALIGN2_HI(val) (((val)+1)&~1)


/**
 ****************************************************************************************
 * @brief Align val on the multiple of 2 equal or nearest lower.
 * @param[in] val Value to align.
 * @return Value aligned.
 ****************************************************************************************
 */
#define CO_ALIGN2_LO(val) ((val)&~1)

/*
 * FUNCTION DEFINTIONS
 ****************************************************************************************
 */
/**
 ****************************************************************************************
 * @brief Count leading zeros.
 * @param[in] val Value to count the number of leading zeros on.
 * @return Number of leading zeros when value is written as 32 bits.
 ****************************************************************************************
 */
__INLINE uint32_t co_clz(uint32_t val)
{
    #if defined(__arm__)
    return __builtin_clz(val);
    #elif defined(__GNUC__)
    if (val == 0)
    {
        return 32;
    }
    return __builtin_clz(val);
    #else
    uint32_t i;
    for (i = 0; i < 32; i++)
    {
        if (val & CO_BIT(31 - i))
            break;
    }
    return i;
    #endif // defined(__arm__)
}

/**
 ****************************************************************************************
 * @brief Count trailing zeros.
 * @param[in] val Value to count the number of trailing zeros on.
 * @return Number of trailing zeros when value is written as 32 bits.
 ****************************************************************************************
 */
__INLINE uint32_t co_ctz(uint32_t val)
{
    #if defined(__arm__)
    return __builtin_ctz(val);
    #elif defined(__GNUC__)
    if (val == 0)
    {
        return 32;
    }
    return __builtin_ctz(val);
    #else
    uint32_t i;
    for (i = 0; i < 32; i++)
    {
        if (val & CO_BIT(i))
            break;
    }
    return i;
    #endif // defined(__arm__)
}

/**
 ****************************************************************************************
 * @brief Swap bytes of an array of bytes
 * .
 * The swap is done in every case. Should not be called directly.
 *
 * @param[in] p_val_out The output value.
 * @param[in] p_val_in  The input value.
 *
 * @param[in] len       number of bytes to swap
 ****************************************************************************************
 */
__INLINE void co_bswap(uint8_t* p_val_out, const uint8_t* p_val_in, uint16_t len)
{
    while (len > 0)
    {
        len--;
        *p_val_out = p_val_in[len];
        p_val_out++;
    }
}
/// @} CO_ENDIAN

/**
 ****************************************************************************************
 * @brief Swap bytes of a 32 bits value.
 * The swap is done in every case. Should not be called directly.
 * @param[in] val32 The 32 bits value to swap.
 * @return The 32 bit swapped value.
 ****************************************************************************************
 */
__INLINE uint32_t co_bswap32(uint32_t val32)
{
    return (val32<<24) | ((val32<<8)&0xFF0000) | ((val32>>8)&0xFF00) | ((val32>>24)&0xFF);
}

/**
 ****************************************************************************************
 * @brief Swap bytes of a 24 bits value.
 * The swap is done in every case. Should not be called directly.
 * @param[in] val24 The 24 bits value to swap.
 * @return The 24 bit swapped value.
 ****************************************************************************************
 */
__INLINE uint32_t co_bswap24(uint32_t val24)
{
    return ((val24<<16)&0xFF0000) | ((val24)&0xFF00) | ((val24>>16)&0xFF);
}

/**
 ****************************************************************************************
 * @brief Swap bytes of a 16 bits value.
 * The swap is done in every case. Should not be called directly.
 * @param[in] val16 The 16 bit value to swap.
 * @return The 16 bit swapped value.
 ****************************************************************************************
 */
__INLINE uint16_t co_bswap16(uint16_t val16)
{
    return ((val16<<8)&0xFF00) | ((val16>>8)&0xFF);
}

/**
 ****************************************************************************************
 * @brief Convert host to network long word.
 *
 * @param[in] hostlong    Long word value to convert.
 *
 * @return The converted long word.
 ****************************************************************************************
 */
__INLINE uint32_t co_htonl(uint32_t hostlong)
{
    #if (!CPU_LE)
        return hostlong;
    #else
        return co_bswap32(hostlong);
    #endif // CPU_LE
}

/**
 ****************************************************************************************
 * @brief Convert host to network long 24-bit value.
 *
 * @param[in] val24    24-bit value to convert.
 *
 * @return The converted 24-but value.
 ****************************************************************************************
 */
__INLINE uint32_t co_hton24(uint32_t host24)
{
    #if (!CPU_LE)
        return host24;
    #else
        return co_bswap24(host24);
    #endif // CPU_LE
}

/**
 ****************************************************************************************
 * @brief Convert host to network short word.
 *
 * @param[in] hostshort Short word value to convert.
 *
 * @return The converted short word.
 ****************************************************************************************
 */
__INLINE uint16_t co_htons(uint16_t hostshort)
{
    #if (!CPU_LE)
        return hostshort;
    #else
        return co_bswap16(hostshort);
    #endif // CPU_LE
}

/**
 ****************************************************************************************
 * @brief Convert host to Bluetooth long word.
 *
 * @param[in] hostlong Long word value to convert.
 *
 * @return The converted long word.
 ****************************************************************************************
 */
__INLINE uint32_t co_htobl(uint32_t hostlong)
{
    #if (CPU_LE)
        return hostlong;
    #else
        return co_bswap32(hostlong);
    #endif // CPU_LE
}

/**
 ****************************************************************************************
 * @brief Convert host to Bluetooth short word.
 *
 * @param[in] hostshort Short word value to convert.
 *
 * @return The converted short word.
 ****************************************************************************************
 */
static __INLINE uint16_t co_htobs(uint16_t hostshort)
{
    #if (CPU_LE)
        return hostshort;
    #else
        return co_bswap16(hostshort);
    #endif // CPU_LE
}

/**
 ****************************************************************************************
 * @brief Convert Bluetooth to host short word.
 *
 * @param[in] btshort Short word value to convert.
 *
 * @return The converted short word.
 ****************************************************************************************
 */
static __INLINE uint16_t co_btohs(uint16_t btshort)
{
    return co_htobs(btshort);
}

/**
 ****************************************************************************************
 * @brief Convert Bluetooth to host long word.
 *
 * @param[in] btlong Long word value to convert.
 *
 * @return The converted long word.
 ****************************************************************************************
 */
__INLINE uint32_t co_btohl(uint32_t btlong)
{
    return co_htobl(btlong);
}


/**
 ****************************************************************************************
 * @brief Read an aligned 32 bit word.
 * @param[in] ptr32 The address of the first byte of the 32 bit word.
 * @return The 32 bit value.
 ****************************************************************************************
 */
__STATIC __INLINE uint32_t co_read32(void const *ptr32)
{
    return *((uint32_t*)ptr32);
}

/**
 ****************************************************************************************
 * @brief Read an aligned 16 bits word.
 * @param[in] ptr16 The address of the first byte of the 16 bits word.
 * @return The 16 bits value.
 ****************************************************************************************
 */
__STATIC __INLINE uint16_t co_read16(void const *ptr16)
{
    return *((uint16_t*)ptr16);
}

/**
 ****************************************************************************************
 * @brief Write an aligned 32 bits word.
 * @param[in] ptr32 The address of the first byte of the 32 bits word.
 * @param[in] value The value to write.
 ****************************************************************************************
 */
__STATIC __INLINE void co_write32(void const *ptr32, uint32_t value)
{
    *(uint32_t*)ptr32 = value;
}

/**
 ****************************************************************************************
 * @brief Write an aligned 16 bits word.
 * @param[in] ptr16 The address of the first byte of the 16 bits word.
 * @param[in] value The value to write.
 ****************************************************************************************
 */
__STATIC __INLINE void co_write16(void const *ptr16, uint32_t value)
{
    *(uint16_t*)ptr16 = value;
}

/**
 ****************************************************************************************
 * @brief Write a 8 bits word.
 * @param[in] ptr8 The address of the first byte of the 8 bits word.
 * @param[in] value The value to write.
 ****************************************************************************************
 */
__STATIC __INLINE void co_write8(void const *ptr8, uint32_t value)
{
    *(uint8_t*)ptr8 = value;
}

/**
 ****************************************************************************************
 * @brief Read a packed 16 bits word.
 * @param[in] ptr16 The address of the first byte of the 16 bits word.
 * @return The 16 bits value.
 ****************************************************************************************
 */
__STATIC __INLINE uint16_t co_read16p(void const *ptr16)
{
    uint16_t value = ((uint8_t *)ptr16)[0] | ((uint8_t *)ptr16)[1] << 8;
    return value;
}

/**
 ****************************************************************************************
 * @brief Read a packed 24 bits word.
 * @param[in] ptr24 The address of the first byte of the 24 bits word.
 * @return The 24 bits value.
 ****************************************************************************************
 */
 
__STATIC __INLINE uint32_t co_read24p(void const *ptr24)
{
    uint16_t addr_l, addr_h;
    addr_l = co_read16p(ptr24);
    addr_h = *((uint8_t *)ptr24 + 2) & 0x00FF;
    return ((uint32_t)addr_l | (uint32_t)addr_h << 16);
}

/**
 ****************************************************************************************
 * @brief Write a packed 24 bits word.
 * @param[in] ptr24 The address of the first byte of the 24 bits word.
 * @param[in] value The value to write.
 ****************************************************************************************
 */
 
__STATIC __INLINE void co_write24p(void const *ptr24, uint32_t value)
{
    uint8_t *ptr=(uint8_t*)ptr24;

    *ptr++ = (uint8_t)(value&0xff);
    *ptr++ = (uint8_t)((value&0xff00)>>8);
    *ptr++ = (uint8_t)((value&0xff0000)>>16);
}

/**
 ****************************************************************************************
 * @brief Read a packed 32 bits word.
 * @param[in] ptr32 The address of the first byte of the 32 bits word.
 * @return The 32 bits value.
 ****************************************************************************************
 */
__STATIC __INLINE uint32_t co_read32p(void const *ptr32)
{
    uint16_t addr_l, addr_h;
    addr_l = co_read16p(ptr32);
    addr_h = co_read16p((uint8_t *)ptr32 + 2);
    return ((uint32_t)addr_l | (uint32_t)addr_h << 16);
}
/**
 ****************************************************************************************
 * @brief Write a packed 32 bits word.
 * @param[in] ptr32 The address of the first byte of the 32 bits word.
 * @param[in] value The value to write.
 ****************************************************************************************
 */
__STATIC __INLINE void co_write32p(void const *ptr32, uint32_t value)
{
    uint8_t *ptr=(uint8_t*)ptr32;

    *ptr++ = (uint8_t)(value&0xff);
    *ptr++ = (uint8_t)((value&0xff00)>>8);
    *ptr++ = (uint8_t)((value&0xff0000)>>16);
    *ptr = (uint8_t)((value&0xff000000)>>24);
}

/**
 ****************************************************************************************
 * @brief Write a packed 16 bits word.
 * @param[in] ptr16 The address of the first byte of the 16 bits word.
 * @param[in] value The value to write.
 ****************************************************************************************
 */
__STATIC __INLINE void co_write16p(void const *ptr16, uint16_t value)
{
    uint8_t *ptr=(uint8_t*)ptr16;

    *ptr++ = value&0xff;
    *ptr = (value&0xff00)>>8;
}

    /// macro to extract a field from a value containing several fields
    /// @param[in] __r bit field value
    /// @param[in] __f field name
    /// @return the value of the register masked and shifted
#define GETF(__r, __f)                                                           \
        (( (__r) & (__f##_MASK) ) >> (__f##_LSB))
    
    /// macro to set a field value into a value  containing several fields.
    /// @param[in] __r bit field value
    /// @param[in] __f field name
    /// @param[in] __v value to put in field
#define SETF(__r, __f, __v)                                                      \
        do {                                                                         \
            ASSERT_INFO( ( ( ( (__v) << (__f##_LSB) ) & ( ~(__f##_MASK) ) ) ) == 0 ,(__f##_MASK), (__v)); \
            __r = (((__r) & ~(__f##_MASK)) | (__v) << (__f##_LSB));                  \
        } while (0)
    
    
    
    /// macro to extract a bit field from a value containing several fields
    /// @param[in] __r bit field value
    /// @param[in] __b bit field name
    /// @return the value of the register masked and shifted
#define GETB(__r, __b)                                                           \
        (( (__r) & (__b##_BIT) ) >> (__b##_POS))
    
    /// macro to set a bit field value into a value containing several fields.
    /// @param[in] __r bit field value
    /// @param[in] __b bit field name
    /// @param[in] __v value to put in field
#define SETB(__r, __b, __v)                                                      \
        do {                                                                         \
            ASSERT_ERR( ( ( ( (__v ? 1 : 0) << (__b##_POS) ) & ( ~(__b##_BIT) ) ) ) == 0 ); \
            __r = (((__r) & ~(__b##_BIT)) | (__v ? 1 : 0) << (__b##_POS));                  \
        } while (0)
    
    /// macro to toggle a bit into a value containing several bits.
    /// @param[in] __r bit field value
    /// @param[in] __b bit field name
#define TOGB(__r, __b)                                                           \
        do {                                                                         \
            __r = ((__r) ^ (__b##_BIT));                                             \
        } while (0)


/**
 ****************************************************************************************
 * @brief Pack parameters from a C structure to a packed buffer
 *
 * This function packs parameters according to a specific format. It takes care of the
 * endianess, padding, required by the compiler.
 *
 * By default output format is LSB but it can be changed with first character of format string
 *     - <  : LSB output format
 *     - >  : MSB output format
 *
 * Format strings are the mechanism used to specify the expected layout when packing and unpacking data. They are built
 * up from Format Characters, which specify the type of data being packed/unpacked.
 *     - B  : byte - 8bits value
 *     - H  : word - 16bits value
 *     - L  : long - 32-bits value
 *     - D  : 24 bits value
 *     - XXB: table of several bytes, where XX is the byte number, in decimal
 *     - XXG: Number of several bytes, where XX is the byte number, in decimal - subject to be swapped according to endianess
 *     - nB : table size over 1 byte, followed by the table of bytes
 *     - NB : table size over 2 bytes, followed by the table of bytes
 *
 * Example:   "BBLH12BLnB" => 1 byte | 1 byte | 1 long | 1 short | 12-bytes table | 1 long | table size over 1 byte | n-bytes table
 *
 * Note: the function works in the same buffer
 *
 * @param[out]     out         Output Data Buffer
 * @param[in]      in          Input Data Buffer
 * @param[out]     out_len     Output size of packed data (in bytes)
 * @param[in]      in_len      Input buffer size (in bytes)
 * @param[in]      format      Parameters format
 *
 * @return  Status of the packing operation
 *****************************************************************************************
 */
uint8_t co_util_pack(uint8_t* out, uint8_t* in, uint16_t* out_len, uint16_t in_len, const char* format);

/**
 ****************************************************************************************
 * @brief Unpack parameters from an unpacked buffer to a C structure
 *
 * This function unpacks parameters according to a specific format. It takes care of the
 * endianess, padding, required by the compiler.
 *
 * By default input format is LSB but it can be changed with first character of format string
 *     - <  : LSB input format
 *     - >  : MSB input format
 *
 * Format strings are the mechanism used to specify the expected layout when packing and unpacking data. They are built
 * up from Format Characters, which specify the type of data being packed/unpacked.
 *     - B  : byte - 8bits value
 *     - H  : word - 16bits value
 *     - L  : long - 32-bits value
 *     - D  : 24 bits value
 *     - XXB: table of several bytes, where XX is the byte number, in decimal
 *     - XXG: Number of several bytes, where XX is the byte number, in decimal - subject to be swapped according to endianess
 *     - nB : table size over 1 byte, followed by the table of bytes
 *     - NB : table size over 2 bytes, followed by the table of bytes
 *
 * Example:   "BBLH12BLnB" => 1 byte | 1 byte | 1 long | 1 short | 12-bytes table | 1 long | table size over 1 byte | n-bytes table
 *
 * Note: the output buffer provided must be large enough to contain the unpacked data.
 * Note2: if a NULL output buffer is provided, the function does not copy the unpacked parameters. It still parses the
 *  format string and input buffer to return the number of unpacked bytes. Can be used to compute the expected unpacked
 *  buffer size.
 *
 * @param[out]    out         Unpacked parameters buffer
 * @param[in]     in          Packed parameters buffer
 * @param[inout]  out_len     Input: buffer size / Output: size of unpacked data (in bytes)
 * @param[in]     in_len      Size of the packed data (in bytes)
 * @param[in]     format      Parameters format
 *
 * @return  Status of the unpacking operation
 *****************************************************************************************
 */
uint8_t co_util_unpack(uint8_t* out, uint8_t* in, uint16_t* out_len, uint16_t in_len, const char* format);

#ifdef __cplusplus
}
#endif
#endif
#endif // ITF_BLE_DEFINE_H_
