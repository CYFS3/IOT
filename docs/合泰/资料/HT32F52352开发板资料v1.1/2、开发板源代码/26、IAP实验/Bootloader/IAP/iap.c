 #include "ht32.h"
 #include "iap.h"
 
 
 
__asm void IAP_GoCMD(u32 address)
{
  LDR R1, [R0]
  MOV SP, R1
  LDR R1, [R0, #4]
  BX R1
}




 
