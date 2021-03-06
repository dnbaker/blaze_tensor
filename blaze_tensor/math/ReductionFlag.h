//=================================================================================================
/*!
//  \file blaze_tensor/math/ReductionFlag.h
//  \brief Header file for the reduction flags
//
//  Copyright (C) 2012-2018 Klaus Iglberger - All Rights Reserved
//  Copyright (C) 2018 Hartmut Kaiser - All Rights Reserved
//
//  This file is part of the Blaze library. You can redistribute it and/or modify it under
//  the terms of the New (Revised) BSD License. Redistribution and use in source and binary
//  forms, with or without modification, are permitted provided that the following conditions
//  are met:
//
//  1. Redistributions of source code must retain the above copyright notice, this list of
//     conditions and the following disclaimer.
//  2. Redistributions in binary form must reproduce the above copyright notice, this list
//     of conditions and the following disclaimer in the documentation and/or other materials
//     provided with the distribution.
//  3. Neither the names of the Blaze development group nor the names of its contributors
//     may be used to endorse or promote products derived from this software without specific
//     prior written permission.
//
//  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY
//  EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
//  OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT
//  SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
//  INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED
//  TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
//  BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
//  CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
//  ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH
//  DAMAGE.
*/
//=================================================================================================

#ifndef _BLAZE_TENSOR_MATH_REDUCTIONFLAG_H_
#define _BLAZE_TENSOR_MATH_REDUCTIONFLAG_H_


//*************************************************************************************************
// Includes
//*************************************************************************************************

#include <blaze/util/Types.h>
#include <blaze/math/ReductionFlag.h>


namespace blaze {

//=================================================================================================
//
//  REDUCTION FLAGS
//
//=================================================================================================

//*************************************************************************************************
/*!\brief Reduction flag for page-wise reduction operations.
//
// This flag can be used to perform page-wise reduction operations on tensors. The following
// example shows the row-wise summation of a tensor:

   \code
   using blaze::rowMajor;
   using blaze::columnVector;

   blaze::DynamicTensor<int> A{ { { 4, 1, 2 }, { -2, 0, 3 } }, { { 4, 1, 2 }, { -2, 0, 3 } } };

   auto m = sum<pagewise>( A );  // Results in { { 8, 2, 4 }, { -4, 0, 6 } }
   \endcode
*/
constexpr size_t pagewise = 2UL;
//*************************************************************************************************

//*************************************************************************************************
/*!\brief Reduction flag for arbitrary reduction operations.
//
// This flag can be used to perform arbitrary reduction operations on arrays. The following
// example shows the row-wise summation of a tensor:

   \code
   blaze::DynamicArray<3, int> A{ { { 4, 1, 2 }, { -2, 0, 3 } }, { { 4, 1, 2 }, { -2, 0, 3 } } };

   auto m = sum<reduction<2>>( A );  // Results in { { 8, 2, 4 }, { -4, 0, 6 } }
   \endcode
*/
template< size_t N >
constexpr size_t reduction = N;
//*************************************************************************************************

} // namespace blaze

#endif
