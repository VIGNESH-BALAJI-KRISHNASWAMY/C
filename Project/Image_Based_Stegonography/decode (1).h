/*******************************************************DOCUMENTATION********************************************************************
*                                     
*   Name            : K.Vignesh Balaji
*   Date            : 21/11/2021. 
*   Description     : Project_1: Steganography. Decoding Header file. 
*                                                             
********************************************************START OF DECLARATIONS************************************************************/
#ifndef DECODE_H
#define DECODE_H

#include "types.h"

/* Structure to store information required for
 * decoding secret file from stego Image
 * Info about output and intermediate data is
 * also stored
 */

#define MAX_SECRET_BUF_SIZE 1
#define MAX_IMAGE_BUF_SIZE (MAX_SECRET_BUF_SIZE * 8)
#define MAX_FILE_SUFFIX 4

static char dec_mag_str[MAX_SECRET_BUF_SIZE];
static char dec_sec_file_ext[MAX_SECRET_BUF_SIZE];
static char decoded_data;
int op_file_mention;

typedef struct _DecodeInfo
{
    /* Source image file name */
    char *src_image_fname;
    FILE *fptr_src_image;
    char image_data[MAX_IMAGE_BUF_SIZE];

    /* Secret file info */
    char *secret_fname;
    FILE *fptr_secret;
    char extn_secret_file[MAX_FILE_SUFFIX];
    char secret_data[MAX_SECRET_BUF_SIZE];
    char dec_sec_file_ext_data[MAX_SECRET_BUF_SIZE];
    long dec_file_size[MAX_SECRET_BUF_SIZE];
    char dec_data[MAX_SECRET_BUF_SIZE];
    FILE *fptr_dec_data;

    /* Stego Image info */
    char *stego_image_fname;
    FILE *fptr_stego_image;
    FILE *fptr_dec_mag_str;
    char magic_data[MAX_SECRET_BUF_SIZE];
    long dec_mag_str[MAX_SECRET_BUF_SIZE];
    char dec_mag_data[MAX_SECRET_BUF_SIZE];
    char dec_sec_file_ext_size[MAX_SECRET_BUF_SIZE];

}DecodeInfo;


/* Decode function prototype */

/* check operation type */
OperationType check_operation_type(char *argv[]);

/* Read and validate args from argv */
Status read_and_validate_decode_args(char *argv[], DecodeInfo *decInfo);

/* Do decoding */
Status do_decoding(DecodeInfo *decInfo);

/* Open files */
Status open_file(DecodeInfo *decInfo);

/* Decode Magic string */
Status decode_magic_string(char *magic_string, DecodeInfo *decInfo);

/* Decode Secret file extension size */
Status decode_secret_file_ext_size(DecodeInfo *decInfo);

 /* Decode Secret file extension */
Status decode_secret_file_ext(DecodeInfo *decInfo);

 /* Open Secret file  */
Status open_secret_file(DecodeInfo *decInfo);

 /* Decode Secret file size */
Status decode_secret_file_size(DecodeInfo *decInfo);

 /* Decode Secret file */
Status decode_secret_file(DecodeInfo *decInfo);

/* Decode Magic string byte from image  */
Status decode_magic_str_bytes_from_image(const char *data, int size, FILE *fptr_stego_img, DecodeInfo *decInfo);

/* Decode Magic string data from LSB  */
Status decode_byte_fromlsb(char *stego_img_data, long *dec_magic_str,int position, DecodeInfo *decInfo);//

/* Decode Secret file extension size byte from LSB  */
Status decode_sec_ext_size_fromlsb(char *decoding,char *dec_sec_ext_size,DecodeInfo *decInfo);

/* Decode Secret file extension data from LSB  */
Status decode_byte_fromlsb_sec_extn(char *dec_sec_file_extn, char *dec_sec_ext_data,int position, DecodeInfo *decInfo);

/* Decode Secret file Size from LSB  */
Status decode_sec_file_size_fromlsb(char *dec_sec_file_size,long *dec_file_size,DecodeInfo *decInfo);

/* Decode Secret file data from LSB  */
Status decode_file_data_fromlsb(char *data,char *dec_data,int position,DecodeInfo *decInfo);

#endif

/********************************************************END OF DECLARATIONS*************************************************************/
