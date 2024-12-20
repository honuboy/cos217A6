/*--------------------------------------------------------------------*/
/* miniassembler.c                                                    */
/* Author: Bob Dondero, Donna Gabai, Jonah Johnson, Jeffrey Xu        */
/*--------------------------------------------------------------------*/

#include "miniassembler.h"
#include <assert.h>
#include <stddef.h>

/*--------------------------------------------------------------------*/
/* Modify *puiDest in place,
   setting uiNumBits starting at uiDestStartBit (where 0 indicates
   the least significant bit) with bits taken from uiSrc,
   starting at uiSrcStartBit.
   uiSrcStartBit indicates the rightmost bit in the field.
   setField sets the appropriate bits in *puiDest to 1.
   setField never unsets any bits in *puiDest.                        */


   /* Use bitwise or, single pipe; get rid of stuff on the left, rid of stuff
   on the right; just work with the instruction */

const unsigned int INSTRLEN = 32;

static void setField(unsigned int uiSrc, unsigned int uiSrcStartBit,
                     unsigned int *puiDest, unsigned int uiDestStartBit,
                     unsigned int uiNumBits)
{
   /*uiSrc is shifted to the left to erase the unwanted digits that are 
   more significant than needed, and shifts to the right to wipe the 
   rightmost unwanted digits. Finally it shifts the uiSrc into the 
   correct position where it can be "added" to the instruction (puiDest)
   with the OR operand*/
  
   uiSrc = uiSrc << (INSTRLEN - uiNumBits - uiSrcStartBit);
   uiSrc = uiSrc >> (INSTRLEN - uiNumBits);

   uiSrc = uiSrc << uiDestStartBit;
   
   *puiDest = *puiDest | uiSrc;

}

/*--------------------------------------------------------------------*/

unsigned int MiniAssembler_mov(unsigned int uiReg, int iImmed)
{
   unsigned int uiInstr;

   /* Base Instruction Code */
   uiInstr = 0x52800000;

   /* register to be inserted in instruction */
   setField(uiReg, 0, &uiInstr, 0, 5);

   /* immediate value in bits 5 through 20 = iImmed */
   setField(iImmed, 0, &uiInstr, 5, 16);
   
   return uiInstr;

}

/*--------------------------------------------------------------------*/

unsigned int MiniAssembler_adr(unsigned int uiReg, unsigned long ulAddr,
   unsigned long ulAddrOfThisInstr)
{
   unsigned int uiInstr;
   unsigned int uiDisp;

   /* Base Instruction Code */
   uiInstr = 0x10000000;

   /* register to be inserted in instruction */
   setField(uiReg, 0, &uiInstr, 0, 5);

   /* displacement to be split into immlo and immhi and inserted */
   uiDisp = (unsigned int)(ulAddr - ulAddrOfThisInstr);

   setField(uiDisp, 0, &uiInstr, 29, 2);
   setField(uiDisp, 2, &uiInstr, 5, 19);

   return uiInstr;
}

/*--------------------------------------------------------------------*/

unsigned int MiniAssembler_strb(unsigned int uiFromReg,
   unsigned int uiToReg)
{
   unsigned int uiInstr;

   /* Base Instruction Code */
   uiInstr = 0x39000000;

   /* register to be inserted in instruction */
   setField(uiFromReg, 0, &uiInstr, 0, 5);

   /* register with destination */
   setField(uiToReg, 0, &uiInstr, 5, 5);

   return uiInstr;

}

/*--------------------------------------------------------------------*/

unsigned int MiniAssembler_b(unsigned long ulAddr,
   unsigned long ulAddrOfThisInstr)
{
   unsigned int uiInstr;
   unsigned int uiDisp;

   /* Base Instruction Code */
   uiInstr = 0x14000000;

   uiDisp = (unsigned int)(ulAddr - ulAddrOfThisInstr);

   /* register to be inserted in instruction */
   setField(uiDisp, 2, &uiInstr, 0, 26);

   return uiInstr;

}

/*--------------------------------------------------------------------*/

unsigned int MiniAssembler_bl(unsigned long ulAddr,
   unsigned long ulAddrOfThisInstr)
{
   unsigned int uiInstr;
   unsigned int uiDisp;

   /* Base Instruction Code */
   uiInstr = 0x94000000;

   uiDisp = (unsigned int)(ulAddr - ulAddrOfThisInstr);

   /* register to be inserted in instruction */
   setField(uiDisp, 2, &uiInstr, 0, 26);

   return uiInstr;

}