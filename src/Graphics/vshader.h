#if 0
//
// Generated by Microsoft (R) HLSL Shader Compiler 9.29.952.3111
//
//
//   fxc /Tvs_4_0 /EVShader /Fhvshader.h shader.hlsl
//
//
// Buffer Definitions: 
//
// cbuffer cbPerObject
// {
//
//   float4x4 WVP;                      // Offset:    0 Size:    64
//   float4 FillColor;                  // Offset:   64 Size:    16
//   float4 isTexture;                  // Offset:   80 Size:    16 [unused]
//
// }
//
//
// Resource Bindings:
//
// Name                                 Type  Format         Dim Slot Elements
// ------------------------------ ---------- ------- ----------- ---- --------
// cbPerObject                       cbuffer      NA          NA    0        1
//
//
//
// Input signature:
//
// Name                 Index   Mask Register SysValue Format   Used
// -------------------- ----- ------ -------- -------- ------ ------
// POSITION                 0   xyzw        0     NONE  float   xyzw
// TEXCOORD                 0   xy          1     NONE  float   xy  
// DIFFUSE                  0   xyzw        2     NONE  float   xyzw
//
//
// Output signature:
//
// Name                 Index   Mask Register SysValue Format   Used
// -------------------- ----- ------ -------- -------- ------ ------
// SV_POSITION              0   xyzw        0      POS  float   xyzw
// DIFFUSE                  0   xyzw        1     NONE  float   xyzw
// TEXCOORD                 0   xy          2     NONE  float   xy  
//
vs_4_0
dcl_constantbuffer cb0[5], immediateIndexed
dcl_input v0.xyzw
dcl_input v1.xy
dcl_input v2.xyzw
dcl_output_siv o0.xyzw, position
dcl_output o1.xyzw
dcl_output o2.xy
dp4 o0.x, v0.xyzw, cb0[0].xyzw
dp4 o0.y, v0.xyzw, cb0[1].xyzw
dp4 o0.z, v0.xyzw, cb0[2].xyzw
dp4 o0.w, v0.xyzw, cb0[3].xyzw
mul o1.xyzw, v2.xyzw, cb0[4].xyzw
mov o2.xy, v1.xyxx
ret 
// Approximately 7 instruction slots used
#endif

const BYTE g_VShader[] =
{
     68,  88,  66,  67,  86, 184, 
    241, 115,  41, 197, 166,  47, 
    112, 139, 171,  15,  89, 208, 
    155,  23,   1,   0,   0,   0, 
    220,   3,   0,   0,   5,   0, 
      0,   0,  52,   0,   0,   0, 
     80,   1,   0,   0, 196,   1, 
      0,   0,  60,   2,   0,   0, 
     96,   3,   0,   0,  82,  68, 
     69,  70,  20,   1,   0,   0, 
      1,   0,   0,   0,  72,   0, 
      0,   0,   1,   0,   0,   0, 
     28,   0,   0,   0,   0,   4, 
    254, 255,   0,   1,   0,   0, 
    226,   0,   0,   0,  60,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   1,   0, 
      0,   0,   0,   0,   0,   0, 
     99,  98,  80, 101, 114,  79, 
     98, 106, 101,  99, 116,   0, 
     60,   0,   0,   0,   3,   0, 
      0,   0,  96,   0,   0,   0, 
     96,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
    168,   0,   0,   0,   0,   0, 
      0,   0,  64,   0,   0,   0, 
      2,   0,   0,   0, 172,   0, 
      0,   0,   0,   0,   0,   0, 
    188,   0,   0,   0,  64,   0, 
      0,   0,  16,   0,   0,   0, 
      2,   0,   0,   0, 200,   0, 
      0,   0,   0,   0,   0,   0, 
    216,   0,   0,   0,  80,   0, 
      0,   0,  16,   0,   0,   0, 
      0,   0,   0,   0, 200,   0, 
      0,   0,   0,   0,   0,   0, 
     87,  86,  80,   0,   3,   0, 
      3,   0,   4,   0,   4,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,  70, 105, 108, 108, 
     67, 111, 108, 111, 114,   0, 
    171, 171,   1,   0,   3,   0, 
      1,   0,   4,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
    105, 115,  84, 101, 120, 116, 
    117, 114, 101,   0,  77, 105, 
     99, 114, 111, 115, 111, 102, 
    116,  32,  40,  82,  41,  32, 
     72,  76,  83,  76,  32,  83, 
    104,  97, 100, 101, 114,  32, 
     67, 111, 109, 112, 105, 108, 
    101, 114,  32,  57,  46,  50, 
     57,  46,  57,  53,  50,  46, 
     51,  49,  49,  49,   0, 171, 
     73,  83,  71,  78, 108,   0, 
      0,   0,   3,   0,   0,   0, 
      8,   0,   0,   0,  80,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   3,   0, 
      0,   0,   0,   0,   0,   0, 
     15,  15,   0,   0,  89,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   3,   0, 
      0,   0,   1,   0,   0,   0, 
      3,   3,   0,   0,  98,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   3,   0, 
      0,   0,   2,   0,   0,   0, 
     15,  15,   0,   0,  80,  79, 
     83,  73,  84,  73,  79,  78, 
      0,  84,  69,  88,  67,  79, 
     79,  82,  68,   0,  68,  73, 
     70,  70,  85,  83,  69,   0, 
    171, 171,  79,  83,  71,  78, 
    112,   0,   0,   0,   3,   0, 
      0,   0,   8,   0,   0,   0, 
     80,   0,   0,   0,   0,   0, 
      0,   0,   1,   0,   0,   0, 
      3,   0,   0,   0,   0,   0, 
      0,   0,  15,   0,   0,   0, 
     92,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      3,   0,   0,   0,   1,   0, 
      0,   0,  15,   0,   0,   0, 
    100,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      3,   0,   0,   0,   2,   0, 
      0,   0,   3,  12,   0,   0, 
     83,  86,  95,  80,  79,  83, 
     73,  84,  73,  79,  78,   0, 
     68,  73,  70,  70,  85,  83, 
     69,   0,  84,  69,  88,  67, 
     79,  79,  82,  68,   0, 171, 
    171, 171,  83,  72,  68,  82, 
     28,   1,   0,   0,  64,   0, 
      1,   0,  71,   0,   0,   0, 
     89,   0,   0,   4,  70, 142, 
     32,   0,   0,   0,   0,   0, 
      5,   0,   0,   0,  95,   0, 
      0,   3, 242,  16,  16,   0, 
      0,   0,   0,   0,  95,   0, 
      0,   3,  50,  16,  16,   0, 
      1,   0,   0,   0,  95,   0, 
      0,   3, 242,  16,  16,   0, 
      2,   0,   0,   0, 103,   0, 
      0,   4, 242,  32,  16,   0, 
      0,   0,   0,   0,   1,   0, 
      0,   0, 101,   0,   0,   3, 
    242,  32,  16,   0,   1,   0, 
      0,   0, 101,   0,   0,   3, 
     50,  32,  16,   0,   2,   0, 
      0,   0,  17,   0,   0,   8, 
     18,  32,  16,   0,   0,   0, 
      0,   0,  70,  30,  16,   0, 
      0,   0,   0,   0,  70, 142, 
     32,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,  17,   0, 
      0,   8,  34,  32,  16,   0, 
      0,   0,   0,   0,  70,  30, 
     16,   0,   0,   0,   0,   0, 
     70, 142,  32,   0,   0,   0, 
      0,   0,   1,   0,   0,   0, 
     17,   0,   0,   8,  66,  32, 
     16,   0,   0,   0,   0,   0, 
     70,  30,  16,   0,   0,   0, 
      0,   0,  70, 142,  32,   0, 
      0,   0,   0,   0,   2,   0, 
      0,   0,  17,   0,   0,   8, 
    130,  32,  16,   0,   0,   0, 
      0,   0,  70,  30,  16,   0, 
      0,   0,   0,   0,  70, 142, 
     32,   0,   0,   0,   0,   0, 
      3,   0,   0,   0,  56,   0, 
      0,   8, 242,  32,  16,   0, 
      1,   0,   0,   0,  70,  30, 
     16,   0,   2,   0,   0,   0, 
     70, 142,  32,   0,   0,   0, 
      0,   0,   4,   0,   0,   0, 
     54,   0,   0,   5,  50,  32, 
     16,   0,   2,   0,   0,   0, 
     70,  16,  16,   0,   1,   0, 
      0,   0,  62,   0,   0,   1, 
     83,  84,  65,  84, 116,   0, 
      0,   0,   7,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   6,   0,   0,   0, 
      5,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      1,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      1,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0
};