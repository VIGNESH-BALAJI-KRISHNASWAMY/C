/*******************************************************DOCUMENTATION********************************************************************
*                                     
*   Name            : K.Vignesh Balaji
*   Date            : 21/11/2021. 
*   Description     : Project_1: Steganography. Main Test code file. 
*   Input           : ./a.out beautiful.bmp secret.txt 
*   Output          :   WARNING: Invalid Option
*                       KEY: Encoding: ./a.out -e beautiful.bmp secret.txt stego.bmp
*                       KEY: Decoding: ./a.out -d stego.bmp
*                                                             
********************************************************START OF CODE********************************************************************/
#include <stdio.h>
#include "encode.h"
#include "decode.h"
#include "types.h"
#include <string.h>

OperationType check_operation_type(char *argv[])
{
    if(strcmp(argv[1], "-e") == 0)
    {
        return e_encode;
    }
    else if(strcmp(argv[1], "-d") == 0)
    {
        return e_decode;
    }
    else
    {
        return e_unsupported;
    }
}


int main(int argc, char **argv)
{
    //check_operation_type
   
   if(argc > 1)
   { 
        if(check_operation_type(argv) == e_encode)
        {
            printf("INFO: Selected encoding\n");
            EncodeInfo encInfo;
            if(read_and_validate_encode_args(argv,&encInfo) == e_success)
            {
                printf("INFO: Read and Validation is successfull :)\n");
                if(do_encoding(&encInfo) == e_success)
                {
                    printf("INFO: >>>>Completed Encoding<<<<\n");
                }
                else
                {
                    printf("ERROR: Encoding not done :(\n");
                }
            }
            else
            {
                printf("ERROR: Read and validation is unsuccessfull :(\n");
            }
        }
        else if(check_operation_type(argv) == e_decode)
        {
            printf("INFO: Selected decoding\n");
            DecodeInfo decInfo;
            if(read_and_validate_decode_args(argv,&decInfo) == e_success)
            {
                printf("INFO: Read and Validation is successfull :)\n");
                if(do_decoding(&decInfo) == e_success)
                {
                    printf("INFO: >>>>Completed Decoding<<<<\n");
                }   
                else
                {
                    printf("ERROR: Decoding not done :(\n");
                }

            }   
            else
            {
                printf("ERROR: Read and Validation is unsuccessfull :(\n");
            }
        }
        else
        {
            printf("WARNING: Invalid Option\nKEY: Encoding: ./a.out -e beautiful.bmp secret.txt stego.bmp\n");
            printf("KEY: Decoding: ./a.out -d stego.bmp\n");
        }
   }
   else
   {
       printf("WARNING: Invalid Option\nKEY: Encoding: ./a.out -e beautiful.bmp secret.txt stego.bmp\n");
       printf("KEY: Decoding: ./a.out -d stego.bmp\n");
   }
        
    
return 0;
}
/********************************************************END OF CODE********************************************************************/

