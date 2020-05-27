#pragma once

#include "BasicTypes/Intrinsics.hpp"
#include "File/FileAPI.hpp"

//  Order is from low-order byte to high-order byte of digest.
//   Each byte is printed with high-order hexadecimal digit first.
struct MD5Output
{
   uint32 dWord_0;   // [0][1][2][3]      // lowest byte
   uint32 dWord_1;
   uint32 dWord_2;
   uint32 dWord_3;   // [12][13][14][15]  // highest order byte
};

// take a raw buffer and find a hash number (16-bytes) see above)
FILE_API void MD5Buffer (uint8* buffer, uint32 buffLen, MD5Output &out);
