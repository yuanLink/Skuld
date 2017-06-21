#if 0
//
// Generated by Microsoft (R) HLSL Shader Compiler 6.3.9600.16384
//
//
//
// Input signature:
//
// Name                 Index   Mask Register SysValue  Format   Used
// -------------------- ----- ------ -------- -------- ------- ------
// SV_VERTEXID              0   x           0   VERTID    uint   x   
//
//
// Output signature:
//
// Name                 Index   Mask Register SysValue  Format   Used
// -------------------- ----- ------ -------- -------- ------- ------
// SV_POSITION              0   xyzw        0      POS   float   xyzw
// TEXCOORD                 0   xy          1     NONE   float   xy  
//
vs_5_0
dcl_globalFlags refactoringAllowed
dcl_input_sgv v0.x, vertex_id
dcl_output_siv o0.xyzw, position
dcl_output o1.xy
dcl_temps 1
dcl_indexableTemp x0[6], 4
mov x0[0].xy, l(-1.000000,-1.000000,0,0)
mov x0[1].xy, l(0,1.000000,0,0)
mov x0[2].xy, l(-1.000000,3.000000,0,0)
mov x0[3].xy, l(0,-1.000000,0,0)
mov x0[4].xy, l(3.000000,-1.000000,0,0)
mov x0[5].xy, l(2.000000,1.000000,0,0)
udiv null, r0.x, v0.x, l(3)
ishl r0.x, r0.x, l(1)
mov o0.xy, x0[r0.x + 0].xyxx
mov o1.xy, x0[r0.x + 1].xyxx
mov o0.zw, l(0,0,0,1.000000)
ret 
// Approximately 12 instruction slots used
#endif

const BYTE VS_FULLSCREEN_Data[] =
{
     68,  88,  66,  67,  83, 114, 
    117, 123, 177, 153,  51,  17, 
     50,  95,  99, 206,  42,  25, 
     78, 171,   1,   0,   0,   0, 
    160,   3,   0,   0,   5,   0, 
      0,   0,  52,   0,   0,   0, 
    172,   0,   0,   0, 224,   0, 
      0,   0,  56,   1,   0,   0, 
      4,   3,   0,   0,  82,  68, 
     69,  70, 112,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
     60,   0,   0,   0,   0,   5, 
    254, 255,   0, 129,   0,   0, 
     60,   0,   0,   0,  82,  68, 
     49,  49,  60,   0,   0,   0, 
     24,   0,   0,   0,  32,   0, 
      0,   0,  40,   0,   0,   0, 
     36,   0,   0,   0,  12,   0, 
      0,   0,   0,   0,   0,   0, 
     77, 105,  99, 114, 111, 115, 
    111, 102, 116,  32,  40,  82, 
     41,  32,  72,  76,  83,  76, 
     32,  83, 104,  97, 100, 101, 
    114,  32,  67, 111, 109, 112, 
    105, 108, 101, 114,  32,  54, 
     46,  51,  46,  57,  54,  48, 
     48,  46,  49,  54,  51,  56, 
     52,   0, 171, 171,  73,  83, 
     71,  78,  44,   0,   0,   0, 
      1,   0,   0,   0,   8,   0, 
      0,   0,  32,   0,   0,   0, 
      0,   0,   0,   0,   6,   0, 
      0,   0,   1,   0,   0,   0, 
      0,   0,   0,   0,   1,   1, 
      0,   0,  83,  86,  95,  86, 
     69,  82,  84,  69,  88,  73, 
     68,   0,  79,  83,  71,  78, 
     80,   0,   0,   0,   2,   0, 
      0,   0,   8,   0,   0,   0, 
     56,   0,   0,   0,   0,   0, 
      0,   0,   1,   0,   0,   0, 
      3,   0,   0,   0,   0,   0, 
      0,   0,  15,   0,   0,   0, 
     68,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      3,   0,   0,   0,   1,   0, 
      0,   0,   3,  12,   0,   0, 
     83,  86,  95,  80,  79,  83, 
     73,  84,  73,  79,  78,   0, 
     84,  69,  88,  67,  79,  79, 
     82,  68,   0, 171, 171, 171, 
     83,  72,  69,  88, 196,   1, 
      0,   0,  80,   0,   1,   0, 
    113,   0,   0,   0, 106,   8, 
      0,   1,  96,   0,   0,   4, 
     18,  16,  16,   0,   0,   0, 
      0,   0,   6,   0,   0,   0, 
    103,   0,   0,   4, 242,  32, 
     16,   0,   0,   0,   0,   0, 
      1,   0,   0,   0, 101,   0, 
      0,   3,  50,  32,  16,   0, 
      1,   0,   0,   0, 104,   0, 
      0,   2,   1,   0,   0,   0, 
    105,   0,   0,   4,   0,   0, 
      0,   0,   6,   0,   0,   0, 
      4,   0,   0,   0,  54,   0, 
      0,   9,  50,  48,  32,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   2,  64,   0,   0, 
      0,   0, 128, 191,   0,   0, 
    128, 191,   0,   0,   0,   0, 
      0,   0,   0,   0,  54,   0, 
      0,   9,  50,  48,  32,   0, 
      0,   0,   0,   0,   1,   0, 
      0,   0,   2,  64,   0,   0, 
      0,   0,   0,   0,   0,   0, 
    128,  63,   0,   0,   0,   0, 
      0,   0,   0,   0,  54,   0, 
      0,   9,  50,  48,  32,   0, 
      0,   0,   0,   0,   2,   0, 
      0,   0,   2,  64,   0,   0, 
      0,   0, 128, 191,   0,   0, 
     64,  64,   0,   0,   0,   0, 
      0,   0,   0,   0,  54,   0, 
      0,   9,  50,  48,  32,   0, 
      0,   0,   0,   0,   3,   0, 
      0,   0,   2,  64,   0,   0, 
      0,   0,   0,   0,   0,   0, 
    128, 191,   0,   0,   0,   0, 
      0,   0,   0,   0,  54,   0, 
      0,   9,  50,  48,  32,   0, 
      0,   0,   0,   0,   4,   0, 
      0,   0,   2,  64,   0,   0, 
      0,   0,  64,  64,   0,   0, 
    128, 191,   0,   0,   0,   0, 
      0,   0,   0,   0,  54,   0, 
      0,   9,  50,  48,  32,   0, 
      0,   0,   0,   0,   5,   0, 
      0,   0,   2,  64,   0,   0, 
      0,   0,   0,  64,   0,   0, 
    128,  63,   0,   0,   0,   0, 
      0,   0,   0,   0,  78,   0, 
      0,   8,   0, 208,   0,   0, 
     18,   0,  16,   0,   0,   0, 
      0,   0,  10,  16,  16,   0, 
      0,   0,   0,   0,   1,  64, 
      0,   0,   3,   0,   0,   0, 
     41,   0,   0,   7,  18,   0, 
     16,   0,   0,   0,   0,   0, 
     10,   0,  16,   0,   0,   0, 
      0,   0,   1,  64,   0,   0, 
      1,   0,   0,   0,  54,   0, 
      0,   7,  50,  32,  16,   0, 
      0,   0,   0,   0,  70,  48, 
     32,   4,   0,   0,   0,   0, 
     10,   0,  16,   0,   0,   0, 
      0,   0,  54,   0,   0,   8, 
     50,  32,  16,   0,   1,   0, 
      0,   0,  70,  48,  32,   6, 
      0,   0,   0,   0,   1,   0, 
      0,   0,  10,   0,  16,   0, 
      0,   0,   0,   0,  54,   0, 
      0,   8, 194,  32,  16,   0, 
      0,   0,   0,   0,   2,  64, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0, 128,  63, 
     62,   0,   0,   1,  83,  84, 
     65,  84, 148,   0,   0,   0, 
     12,   0,   0,   0,   1,   0, 
      0,   0,   0,   0,   0,   0, 
      3,   0,   0,   0,   0,   0, 
      0,   0,   1,   0,   0,   0, 
      1,   0,   0,   0,   1,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   6,   0, 
      0,   0,   8,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   1,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0
};