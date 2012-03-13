#ifndef _TYPES_H
#define _TYPES_H

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \typedef bool
 * \brief Use this instead of chars
 */
typedef enum {false, true} bool;

/**
 * \typedef string
 * \brief The standard C type string
 */
typedef char* string;

#ifdef __cplusplus
}
#endif

#endif
