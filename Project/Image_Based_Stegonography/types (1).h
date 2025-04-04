/*******************************************************DOCUMENTATION********************************************************************
*                                     
*   Name            : K.Vignesh Balaji
*   Date            : 21/11/2021. 
*   Description     : Project_1: Steganography. Typedef definitions.
*                                                             
********************************************************START OF CODE********************************************************************/

#ifndef TYPES_H
#define TYPES_H

#define LP64

/* User defined types */
typedef unsigned int uint;


/* Status will be used in fn. return type */
typedef enum
{
    e_success,
    e_failure
} Status;

typedef enum
{
    e_encode,
    e_decode,
    e_unsupported
} OperationType;

#endif

/********************************************************END OF CODE********************************************************************/
