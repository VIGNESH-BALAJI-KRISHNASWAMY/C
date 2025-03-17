/*******************************************************DOCUMENTATION********************************************************************
*                                     
*   Name            : K.Vignesh Balaji
*   Date            : 21/11/2021. 
*   Description     : Project_1: Steganography. Encoding code file. 
*   Input           : ./a.out -e beautiful.bmp secret.txt
*   Output          :   INFO: Selected encoding
*                       INFO: Read and Validation is successfull :)
*                       INFO: Started Encoding.....
*                       INFO: opened beautiful.bmp successfully
*                       INFO: opened secret.txt successfully
*                       INFO: opened stego.bmp successfully
*                       INFO: Open File is success..
*                       INFO: width of given image  = 1024
*                       INFO: height of given image = 768
*                       INFO: beautiful.bmp is capable of storing secret data....
*                       INFO: Copied the header successfully
*                       INFO: MAGIC STRING is encoded successfully....
*                       INFO: Encoded secret file extension size success....
*                       INFO: Encoded secret file extension success...
*                       INFO: Encode secret file size success...
*                       INFO: Encoded secret file data successfully... :)
*                       INFO: Encoded remaining image data successfully..... :)
*                       INFO: >>>>Completed Encoding<<<<
*                                                             
********************************************************START OF CODE********************************************************************/

#include <stdio.h>                                                          //header file declarations.
#include <string.h>
#include "common.h"
#include "encode.h"
#include "types.h"

/* Function Definitions */
Status read_and_validate_encode_args(char *argv[], EncodeInfo *encInfo)
{
    if(strcmp(strstr(argv[2],"."),".bmp") == 0)
    {
        encInfo -> src_image_fname = argv[2];
    }
    else
    {
        return e_failure;
    }
    if(strcmp(strstr(argv[3],"."),".txt") == 0)
    {
        encInfo -> secret_fname = argv[3];
    }
    else
    {
        return e_failure;
    }
    if(argv[4] != NULL)
    {
        encInfo -> stego_image_fname = argv[4];
    }
    else
    {
        encInfo -> stego_image_fname = "stego.bmp";
    }

    return e_success;
}

/* Get image size
 * Input: Image file ptr
 * Output: width * height * bytes per pixel (3 in our case)
 * Description: In BMP Image, width is stored in offset 18,
 * and height after that. size is 4 bytes
 */

uint get_image_size_for_bmp(FILE *fptr_image)
{
    uint width, height;                        
    fseek(fptr_image, 18, SEEK_SET);                        // Seek to 18th byte

    fread(&width, sizeof(int), 1, fptr_image);              // Read the width (an int)
    printf("INFO: width of given image  = %u\n", width);

    fread(&height, sizeof(int), 1, fptr_image);             // Read the height (an int)
    printf("INFO: height of given image = %u\n", height);

    // Return image capacity
    return width * height * 3; //TODO use bpp
}

/* Get File pointers for i/p and o/p files
 * Inputs: Src Image file, Secret file and Stego Image file
 * Output: FILE pointer for above files
 * Return Value: e_success or e_failure, on file errors
 */

Status open_files(EncodeInfo *encInfo)
{
    // Src Image file
    encInfo->fptr_src_image = fopen(encInfo->src_image_fname, "r");
    
    //Do Error handling
    if (encInfo->fptr_src_image == NULL)
    {
    	perror("fopen");
    	fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->src_image_fname);
    	return e_failure;
    }
    else
    {
        printf("INFO: opened %s successfully\n",encInfo -> src_image_fname);
    }

    // Secret file
    encInfo->fptr_secret = fopen(encInfo->secret_fname, "r");
    
    //Do Error handling
    if (encInfo->fptr_secret == NULL)
    {
    	perror("fopen");
    	fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->secret_fname);
    	return e_failure;
    }
    else
    {
        printf("INFO: opened %s successfully\n",encInfo -> secret_fname);
    }

    // Stego Image file
    encInfo->fptr_stego_image = fopen(encInfo->stego_image_fname, "w");
    
    //Do Error handling
    if (encInfo->fptr_stego_image == NULL)
    {
    	perror("fopen");
    	fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->stego_image_fname);
    	return e_failure;
    }
    else
    {
        printf("INFO: opened %s successfully\n",encInfo -> stego_image_fname);
    }

    // No failure return e_success
    return e_success;

}

uint get_file_size(FILE *fptr)
{
    fseek(fptr,0,SEEK_END);
    return ftell(fptr);
}

Status check_capacity(EncodeInfo *encInfo)
{
    //get the beautiful.bmp size
    encInfo -> image_capacity = get_image_size_for_bmp(encInfo -> fptr_src_image);
    encInfo -> size_secret_file = get_file_size(encInfo -> fptr_secret);
    if(encInfo -> image_capacity > (54+(2+4+4+4+encInfo -> size_secret_file) * 8))  //declaring encoding bytes necessary for calculation. 
    {
        return e_success;                   //required space is available in image.
    }
    else
    {
        return e_failure;                   //space is not available.
    }

}

Status copy_bmp_header(FILE *fptr_src_image, FILE *fptr_dest_image)         //copying 54 bytes of given bmp header to stego.bmp
{
    fseek(fptr_src_image, 0, SEEK_SET);
    char str[54];
    fread(str, sizeof(char), 54, fptr_src_image);
    fwrite(str, sizeof(char), 54, fptr_dest_image);
    return e_success;
}

Status encode_byte_tolsb(char data, char *image_buffer)   //encoding the last bit of each byte in given bmp, with secret data's last bit.
{
    unsigned int mask = 1 << 7;
    for(int i = 0; i < 8; i++)
    {
        image_buffer[i] = (image_buffer[i] & 0xFE) | ((data & mask) >> (7 - i));
        mask = mask >> 1;
    }
}

Status encode_data_to_image(const char *data, int size, FILE *fptr_src_img, FILE *fptr_stego_img, EncodeInfo *encInfo)
{
    for(int i = 0; i < size; i++)
    {
        fread(encInfo -> image_data, sizeof(char), 8, fptr_src_img);
        encode_byte_tolsb(data[i], encInfo -> image_data);                  //function call to perform bitwise encoding.
        fwrite(encInfo -> image_data, sizeof(char), 8, fptr_stego_img);
    }
    return e_success;
}

Status encode_size_to_lsb(char *buffer, int size)               //encoding the size to lsb of image.
{
    unsigned int mask = 1 << 7;
    for(int i = 0; i < 32; i++)
    {
        buffer[i] = (buffer[i] & 0xFE) | ((size & mask) >> (7 - i));
        mask = mask >> 1;
    }

}

Status encode_magic_string(char *magic_string, EncodeInfo *encInfo)     //encoding magic string of 2 bytes in 16 bytes of image.
{
    encode_data_to_image(magic_string, strlen(magic_string), encInfo -> fptr_src_image, encInfo -> fptr_stego_image, encInfo);
    return e_success;
}

Status encode_size(int size, FILE *fptr_src_image, FILE *fptr_stego_image)  //encoding size of 4 bytes with 32 bytes of given bmp.
{
    char str[32];
    fread(str, sizeof(char), 32, fptr_src_image);
    encode_size_to_lsb(str,size);
    fwrite(str, sizeof(char), 32, fptr_stego_image);
    return e_success;
}

Status encode_secret_file_extn(const char *file_extn, EncodeInfo *encInfo) //encoding file extension of 4 bytes(fixed)...if needed more..
{                                                                            //... modify in check_capacity function above.           
    file_extn = ".txt";
    encode_data_to_image(file_extn, strlen(file_extn),encInfo -> fptr_src_image, encInfo -> fptr_stego_image, encInfo);
    return e_success;
}

Status encode_secret_file_size(int size, EncodeInfo *encInfo) //encoding secret file size, modifiable from the file contents in secret.txt
{
    char str[32];
    fread(str, sizeof(char), 32, encInfo -> fptr_src_image);
    encode_size_to_lsb(str,size);
    fwrite(str, sizeof(char), 32, encInfo -> fptr_stego_image);
    return e_success;
}

Status encode_secret_file_data(EncodeInfo *encInfo)       //function to encode secret data from secret.txt with beautiful.bmp
{
    char ch;
    fseek(encInfo -> fptr_secret, 0, SEEK_SET);

    for(int i = 0; i < encInfo -> size_secret_file; i++)
    {
        fread(encInfo -> image_data, sizeof(char), 8, encInfo -> fptr_src_image);
        fread(&ch, sizeof(char), 1, encInfo -> fptr_secret);
        encode_byte_tolsb(ch, encInfo -> image_data);
        fwrite(encInfo -> image_data, sizeof(char), 8,encInfo -> fptr_stego_image);
    }
    return e_success;
}

Status copy_remaining_img_data(FILE *fptr_src_image, FILE *fptr_stego_image)   //copy rem data from given bmp to stego.bmp
{
    char ch;
    while(fread(&ch,1,1,fptr_src_image) > 0)
    {
        fwrite(&ch, 1, 1, fptr_stego_image);
    }
    return e_success;
}

Status do_encoding(EncodeInfo *encInfo)
{
    printf("INFO: Started Encoding.....\n");
    if(open_files(encInfo) == e_success)
    {
        printf("INFO: Open File is success..\n");
        if(check_capacity(encInfo) == e_success)
        {
            printf("INFO: %s is capable of storing secret data....\n",encInfo -> src_image_fname);
            if(copy_bmp_header(encInfo -> fptr_src_image, encInfo -> fptr_stego_image) == e_success)
            {
                printf("INFO: Copied the header successfully\n");
                if(encode_magic_string(MAGIC_STRING, encInfo) == e_success)
                {
                    printf("INFO: MAGIC STRING is encoded successfully....\n");
                    if(encode_size(strlen(".txt"),encInfo -> fptr_src_image,encInfo -> fptr_stego_image) == e_success)
                    {
                        printf("INFO: Encoded secret file extension size success....\n");
                        if(encode_secret_file_extn(encInfo -> extn_secret_file, encInfo) == e_success)
                        {
                            printf("INFO: Encoded secret file extension success...\n");
                            if(encode_secret_file_size(encInfo -> size_secret_file, encInfo) == e_success)
                            {
                                printf("INFO: Encode secret file size success...\n");
                                if(encode_secret_file_data(encInfo) == e_success)
                                {
                                    printf("INFO: Encoded secret file data successfully... :)\n");
                                    if(copy_remaining_img_data(encInfo -> fptr_src_image,encInfo -> fptr_stego_image) == e_success)
                                    {
                                        printf("INFO: Encoded remaining image data successfully..... :)\n");
                                    }
                                    else
                                    {
                                        printf("ERROR: Encoding remaining image data unsuccessfull ... :(\n");
                                        return e_failure;
                                    }
                                }
                                else
                                {
                                    printf("ERROR: Encoding secret file data unsuccessful... :(\n");
                                    return e_failure;
                                }
                            }
                            else
                            {
                                printf("ERROR: Encode secret file size unsuccess...\n");
                                return e_failure;
                            }
                        }
                        else
                        {
                            printf("ERROR: Encoding secret file extension unsuccess...\n");
                            return e_failure;
                        }
                    }
                    else
                    {
                        printf("ERROR: Encoding secret file extension size unsuccessfull....\n");
                        return e_failure;
                    }

                }
                else
                {
                    printf("ERROR: Encoding MAGIC STRIG is unsuccessful....\n");
                    return e_failure;
                }
            }
            else
            {
                printf("ERROR: Failed copying header...  :(\n");
                return e_failure;
            }
        }
        else
        {
            printf("ERROR: Beautiful.bmp is not having enough memory to do the encoding :(\n");
            return e_failure;
        }
    }
    else
    {
        printf("ERROR: Open file is unsuccessfull :(\n");
        return e_failure;
    }

    return e_success;
}

/********************************************************END OF CODE********************************************************************/
