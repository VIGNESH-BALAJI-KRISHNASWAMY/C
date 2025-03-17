/*******************************************************DOCUMENTATION********************************************************************
*                                     
*   Name            : K.Vignesh Balaji
*   Date            : 21/11/2021. 
*   Description     : Project_1: Steganography. Decoding code file. 
*   Input           : ./a.out -d stego.bmp 
*   output          :   INFO: Selected decoding
*                       INFO: Read and Validation is successfull :)
*                       INFO: Started Decoding Procedure...
*                       INFO: Opened Encoded stego.bmp File successfully...
*                       INFO: MAGIC STRING #* is decoded successfully... :)
*                       INFO: Secret file extension size of 4 is successfully decoded... :)
*                       INFO: Secret file extension is .txt ,successfully decoded... :)
*                       INFO: Output File not mentioned.>>>>Creating decoded_data.txt as default
*                       INFO: Opened File to store decoded content.Successfull... :)
*                       INFO: Opened All Files required for decoding Successfully... :)
*                       INFO: Cryptig secret file of size 29 BYTES is going on... :)
*                       INFO: <<<|>>>Decoded Successfully<<<|>>> :)
*                       INFO: >>>>Completed Decoding<<<<
*                       emertxe@ubuntu:~/Desktop/c_module/project/LSB-Steganography/Sources/SkeletonLSB-ImageSteg$ ls
*   a.out  beautiful.bmp  common.h  decode.c  decoded_data.txt  decode.h  encode.c  encode.h  secret.txt  stego.bmp  Test_Code.c  types.h
*                       emertxe@ubuntu:~/Desktop/c_module/project/LSB-Steganography/Sources/SkeletonLSB-ImageSteg$ cat decoded_data.txt 
*                       Today is my B!rthd@y!.
*                       emertxe@ubuntu:~/Desktop/c_module/project/LSB-Steganography/Sources/SkeletonLSB-ImageSteg$ cat secret.txt 
*                       Today is my B!rthd@y!.
*                                       
********************************************************START OF CODE********************************************************************/

#include <stdio.h>                                                          //header file declarations.
#include <string.h>
#include <stdlib.h>
#include "common.h"
#include "decode.h"
#include "types.h"

/* Function Definitions */

Status read_and_validate_decode_args(char *argv[], DecodeInfo *decInfo)
{
    if(strcmp(strstr(argv[2],"."),".bmp") == 0)
    {
        decInfo -> stego_image_fname = argv[2];
    }
    else
    {
        return e_failure;
    }
    if(argv[3] != NULL)
    {
        decInfo -> secret_fname = argv[3];
        op_file_mention = 1;
    }
    else
    {
        decInfo -> secret_fname = "decoded_data";
        op_file_mention = 2;
    }
    return e_success;
}


Status open_file(DecodeInfo *decInfo)
{
    // Stego Image file
    decInfo->fptr_stego_image = fopen(decInfo->stego_image_fname, "r");
    //Do Error handling
    if (decInfo->fptr_stego_image == NULL)
    {
    	perror("fopen");
    	fprintf(stderr, "ERROR: Unable to open file %s\n", decInfo->stego_image_fname);
    	return e_failure;
    }

    // No failure return e_success
    return e_success;

}

Status open_secret_file(DecodeInfo *decInfo)
{
    if(op_file_mention == 2)
    {
        printf("INFO: Output File not mentioned.");
    }
    char *ext = malloc(50);                                     //generating name for output file with file format.
    char *out = malloc(50);
    char *name = malloc(50);
    strcpy(name,decInfo->secret_fname);
    strcpy(ext,dec_sec_file_ext);
    out = strcat(name,ext);
    decInfo->secret_fname = out;
    decInfo->fptr_secret = fopen(decInfo->secret_fname, "w");
                        
    //Do Error handling
    if (decInfo->fptr_secret == NULL)
    {
        perror("fopen");
        fprintf(stderr, "ERROR: Unable to open file %s\n", out);
    	return e_failure;
    }
    else
    {
        if(op_file_mention == 2)
            printf(">>>>Creating %s as default\n",out);
        else
            printf("INFO: Opened file %s.\n",out);
    }
    free(ext);free(name);free(out);
    return e_success;
}

Status decode_file_data_fromlsb(char *data,char *dec_data,int position,DecodeInfo *decInfo) //decoding file data from lsb.
{
    int mask = 0x1;
    *dec_data = 0;
    for(int i = 0; i < 8; i++)
    {
      *dec_data = *dec_data | ((data[i] & mask)  << (7-i));
    }
    decoded_data = *dec_data;
    return e_success;
}

Status decode_secret_file(DecodeInfo *decInfo)          //decoding secret file data from stego image byte.
{
    int size,i;
    size = decInfo -> dec_file_size[0];
    char data[8];
    for(i = 0; i < size; i++)
    {
        fread(data, sizeof(char), 8, decInfo -> fptr_stego_image);
        decode_file_data_fromlsb(data,decInfo -> dec_data,i,decInfo);
        fwrite(&decoded_data, sizeof(char), 1,decInfo->fptr_secret);
    }
    return e_success;
}

Status decode_sec_file_size_fromlsb(char *dec_sec_file_size,long *dec_file_size,DecodeInfo *decInfo) //decoding secret file size from lsb.
{
    int mask = 0x1;
    *dec_file_size = 0;
    for(int i = 0; i < 32; i++)
    {
      *dec_file_size = *dec_file_size | ((dec_sec_file_size[i] & mask)  << (7-i));
    }
}

Status decode_secret_file_size(DecodeInfo *decInfo)     //decoding secret file size from stego image.
{
    char dec_sec_file_size[32];

        fread(dec_sec_file_size, sizeof(char), 32, decInfo -> fptr_stego_image);
        decode_sec_file_size_fromlsb(dec_sec_file_size,decInfo -> dec_file_size,decInfo);
            return e_success;
}

/* decoding secret file extension name from lsb */
Status decode_byte_fromlsb_sec_extn(char *dec_sec_file_extn, char *dec_sec_ext_data,int position, DecodeInfo *decInfo)
{
    int mask = 0x1;
    *dec_sec_ext_data = 0;
    for(int i = 0; i < 8; i++)
    {
      *dec_sec_ext_data = *dec_sec_ext_data | ((dec_sec_file_extn[i] & mask)  << (7-i));
    }
    dec_sec_file_ext[position] = *dec_sec_ext_data;
}

Status decode_secret_file_ext(DecodeInfo *decInfo)  //decoding secret file extension name from stego image.
{
    int size,i;
    size = decInfo -> dec_sec_file_ext_size[0];
    char data[8];    
    for(i = 0; i < size; i++)
    {
        fread(data, sizeof(char), 8, decInfo -> fptr_stego_image);
        decode_byte_fromlsb_sec_extn(data,decInfo -> dec_sec_file_ext_data,i,decInfo);
    }
    return e_success;
}
/*decoding secret file extension size from lsb.*/
Status decode_sec_ext_size_fromlsb(char *decoding,char *dec_sec_ext_size,DecodeInfo *decInfo)
{
    int mask = 0x1;
    *dec_sec_ext_size = 0;
    for(int i = 0; i < 32; i++)
    {
      *dec_sec_ext_size = *dec_sec_ext_size | ((decoding[i] & mask)  << (7-i));
    }
}

/*decoding secret file extension size from stego image.*/
Status decode_secret_file_ext_size(DecodeInfo *decInfo)
{
    char dec_sec_ext_size[32];

        fread(dec_sec_ext_size, sizeof(char), 32, decInfo -> fptr_stego_image);
        decode_sec_ext_size_fromlsb(dec_sec_ext_size,decInfo -> dec_sec_file_ext_size,decInfo);
        if ((decInfo -> dec_sec_file_ext_size[0]) == 4)
            return e_success;
}

/*  decoding magic string  from lsb  */
Status decode_byte_fromlsb(char *stego_img_data, long *dec_magic_str,int position, DecodeInfo *decInfo)//char to int
{
    int mask = 0x1;
    *dec_magic_str = 0;
    for(int i = 0; i < 8; i++)
    {
      *dec_magic_str = *dec_magic_str | ((stego_img_data[i] & mask)  << (7-i));
    }
    decInfo -> dec_mag_data[position] = *dec_magic_str;
    dec_mag_str[position] = *dec_magic_str;
}

/*decoding magic string bytes from lsb.*/
Status decode_magic_str_bytes_from_image(const char *data, int size, FILE *fptr_stego_img, DecodeInfo *decInfo)
{
    fseek(decInfo -> fptr_stego_image, 54, SEEK_SET);
    for(int i = 0; i < size; i++)
    {
        fread(decInfo -> magic_data, sizeof(char), 8, fptr_stego_img);
        decode_byte_fromlsb(decInfo -> magic_data,decInfo -> dec_mag_str,i,decInfo);
    }
}

/*decoding magic string bytes from stego image */
Status decode_magic_string(char *magic_string, DecodeInfo *decInfo)
{
    decode_magic_str_bytes_from_image(magic_string, strlen(magic_string), decInfo -> fptr_stego_image, decInfo);
    if(strcmp(dec_mag_str,MAGIC_STRING) == 0)
        return e_success;
}


Status do_decoding(DecodeInfo *decInfo)     //main function to call all sub functions.
{
    printf("INFO: Started Decoding Procedure...\n");
    if(open_file(decInfo) == e_success)
    {
        printf("INFO: Opened Encoded %s File successfully...\n",decInfo->stego_image_fname);
        if(decode_magic_string(MAGIC_STRING,decInfo) == e_success )
        {
            printf("INFO: MAGIC STRING %s is decoded successfully... :)\n",dec_mag_str);
            if(decode_secret_file_ext_size(decInfo) == e_success )
            {
                printf("INFO: Secret file extension size of %d is successfully decoded... :)\n",decInfo -> dec_sec_file_ext_size[0]);
                if(decode_secret_file_ext(decInfo) == e_success )
                {
                    printf("INFO: Secret file extension is %s ,successfully decoded... :)\n",dec_sec_file_ext);
                    if(open_secret_file(decInfo) == e_success)
                    {
                        printf("INFO: Opened File to store decoded content.Successfull... :)\n");
                        printf("INFO: Opened All Files required for decoding Successfully... :)\n");
                        if(decode_secret_file_size(decInfo) == e_success)
                        {
                            printf("INFO: Cryptig secret file of size %ld BYTES is going on... :)\n",decInfo -> dec_file_size[0]);
                            if(decode_secret_file(decInfo) == e_success)
                            {
                                printf("INFO: <<<|>>>Decoded Successfully<<<|>>> :)\n");
                            }
                            else
                            {
                                printf("ERROR: Cannot Decode encrypted Data :(");
                                return e_failure;
                            }
                        }
                        else
                        {
                            printf("ERROR: Decoding secret file size is unsuccessfull... :(\n");
                            return e_failure;
                        }
                    }
                    else
                    {
                        printf("ERROR: Cannot open File to store the Decode data.. :(\n");
                        return e_failure;
                    }
                }
                else
                {
                    printf("ERROR: Decoding secret file extension is unsuccessfull... :(\n");
                    return e_failure;
                }
            }
            else
            {
                printf("ERROR: Failed to Decode secret file extension size... :(\n");
                return e_failure;
            }
        }
        else
        {
            printf("ERROR: MAGIC STRING is NOT Found... :(\n");
            return e_failure;
        }
    }
    else
    {
        printf("ERROR: Cannot Open %s file :(\n",decInfo->stego_image_fname);
        return e_failure;
    }

return e_success;

}

/********************************************************END OF CODE********************************************************************/
