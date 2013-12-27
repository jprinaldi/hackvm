Hack VM
=======

My C implementation of the Hack VM from Hacker.org

Example programs
----------------

Compute a % n, where a and n are in memory cells 0 and 1 respectively.
    
    0<0<1</1<*-p

Compute a xor b, where a and b are in memory cells 0 and 1 respectively.
    
    13>04>0<0<2/2*-1<1<2/2*-:0^*3<*4<+4>3<2*3>0<2/0>1<2/1>0<1<+2?6c4<p
    
Compute the length of a null-terminated string, starting from memory cell 0.

    00^<4?1+1cp
    
Compute max(a,b), where a and b are in memory cells 0 and 1 respectively. To make things interesting, it doesn't use the ':' instruction.

    0<1</4?0<p!1<p
    
Compute max(a,b), where a and b are in memory cells 0 and 1 respectively. To make things interesting, it doesn't use the ':' or '?' instructions.
    
    0<1</1+11v/<p
    
Compute max(a,b), where a and b are in memory cells 0 and 1 respectively. To make things interesting, it doesn't use the ':', '?' or '/' instructions.
    
    60<2+>00>1<2+<2*6+1<2+>01>11+0^<g066+-g1+0^<g066+-g2-p
  
Compute max(a,b), where a and b are in memory cells 0 and 1 respectively. To make things interesting, it doesn't use the ':', '?', '/' or '-' instructions.
    
    60<  >00>1<  <2*6+1<  >01>11+0^<g39*  c1+0^<g66*3+cp
    
Print 'Hello, Small World!'. To make things interesting, it uses only 77 instructions.

    048*1+54*5*0^8+0^6+0^3-99*6+48*4^0^7^3-1^1+5^4-5^29+4*9^6^0^0^7-98*0^5?P09-g!
