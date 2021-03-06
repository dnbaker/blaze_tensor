//=================================================================================================
/*!
//  \file blaze/math/expressions/DTensEvalExpr.h
//  \brief Header file for the dense tensor evaluation expression
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

#ifndef _BLAZE_TENSOR_MATH_EXPRESSIONS_DTENSEVALEXPR_H_
#define _BLAZE_TENSOR_MATH_EXPRESSIONS_DTENSEVALEXPR_H_


//*************************************************************************************************
// Includes
//*************************************************************************************************

#include <blaze/math/expressions/DMatEvalExpr.h>

#include <blaze_tensor/math/constraints/DenseTensor.h>
#include <blaze_tensor/math/expressions/TensEvalExpr.h>

namespace blaze {

//=================================================================================================
//
//  CLASS DTENSEVALEXPR
//
//=================================================================================================

//*************************************************************************************************
/*!\brief Expression object for the forced evaluation of dense tensors.
// \ingroup dense_tensor_expression
//
// The DTensEvalExpr class represents the compile time expression for the forced evaluation
// of a dense tensor.
*/
template< typename MT > // Type of the dense tensor
class DTensEvalExpr
   : public TensEvalExpr< DenseTensor< DTensEvalExpr<MT> > >
   , private Computation
{
 public:
   //**Type definitions****************************************************************************
   using This          = DTensEvalExpr<MT>;  //!< Type of this DTensEvalExpr instance.
   using ResultType    = ResultType_t<MT>;     //!< Result type for expression template evaluations.
   using OppositeType  = OppositeType_t<MT>;   //!< Result type with opposite storage order for expression template evaluations.
   using TransposeType = TransposeType_t<MT>;  //!< Transpose type for expression template evaluations.
   using ElementType   = ElementType_t<MT>;    //!< Resulting element type.
   using ReturnType    = ReturnType_t<MT>;     //!< Return type for expression template evaluations.

   //! Data type for composite expression templates.
   using CompositeType = const ResultType;

   //! Composite data type of the dense tensor expression.
   using Operand = If_t< IsExpression_v<MT>, const MT, const MT& >;
   //**********************************************************************************************

   //**Compilation flags***************************************************************************
   //! Compilation switch for the expression template evaluation strategy.
   static constexpr bool simdEnabled = false;

   //! Compilation switch for the expression template assignment strategy.
   static constexpr bool smpAssignable = MT::smpAssignable;
   //**********************************************************************************************

   //**Constructor*********************************************************************************
   /*!\brief Constructor for the DTensEvalExpr class.
   //
   // \param dm The dense tensor operand of the evaluation expression.
   */
   explicit inline DTensEvalExpr( const MT& dm ) noexcept
      : dt_( dm )  // Dense tensor of the evaluation expression
   {}
   //**********************************************************************************************

   //**Access operator*****************************************************************************
   /*!\brief 2D-access to the tensor elements.
   //
   // \param i Access index for the row. The index has to be in the range \f$[0..M-1]\f$.
   // \param j Access index for the column. The index has to be in the range \f$[0..N-1]\f$.
   // \return The resulting value.
   */
   inline ReturnType operator()( size_t k, size_t i, size_t j ) const {
      BLAZE_INTERNAL_ASSERT( i < dt_.rows()   , "Invalid row access index"    );
      BLAZE_INTERNAL_ASSERT( j < dt_.columns(), "Invalid column access index" );
      BLAZE_INTERNAL_ASSERT( k < dt_.pages(),   "Invalid page access index"   );
      return dt_(k,i,j);
   }
   //**********************************************************************************************

   //**At function*********************************************************************************
   /*!\brief Checked access to the tensor elements.
   //
   // \param i Access index for the row. The index has to be in the range \f$[0..M-1]\f$.
   // \param j Access index for the column. The index has to be in the range \f$[0..N-1]\f$.
   // \return The resulting value.
   // \exception std::out_of_range Invalid tensor access index.
   */
   inline ReturnType at( size_t k, size_t i, size_t j ) const {
      if( i >= dt_.rows() ) {
         BLAZE_THROW_OUT_OF_RANGE( "Invalid row access index" );
      }
      if( j >= dt_.columns() ) {
         BLAZE_THROW_OUT_OF_RANGE( "Invalid column access index" );
      }
      if( k >= dt_.pages() ) {
         BLAZE_THROW_OUT_OF_RANGE( "Invalid page access index" );
      }
      return (*this)(k,i,j);
   }
   //**********************************************************************************************

   //**Rows function*******************************************************************************
   /*!\brief Returns the current number of rows of the tensor.
   //
   // \return The number of rows of the tensor.
   */
   inline size_t rows() const noexcept {
      return dt_.rows();
   }
   //**********************************************************************************************

   //**Columns function****************************************************************************
   /*!\brief Returns the current number of columns of the tensor.
   //
   // \return The number of columns of the tensor.
   */
   inline size_t columns() const noexcept {
      return dt_.columns();
   }
   //**********************************************************************************************

   //**Pages function****************************************************************************
   /*!\brief Returns the current number of pages of the tensor.
   //
   // \return The number of pages of the tensor.
   */
   inline size_t pages() const noexcept {
      return dt_.pages();
   }
   //**********************************************************************************************

   //**Operand access******************************************************************************
   /*!\brief Returns the dense tensor operand.
   //
   // \return The dense tensor operand.
   */
   inline Operand operand() const noexcept {
      return dt_;
   }
   //**********************************************************************************************

   //**********************************************************************************************
   /*!\brief Returns whether the expression can alias with the given address \a alias.
   //
   // \param alias The alias to be checked.
   // \return \a true in case the expression can alias, \a false otherwise.
   */
   template< typename T >
   inline bool canAlias( const T* alias ) const noexcept {
      return dt_.canAlias( alias );
   }
   //**********************************************************************************************

   //**********************************************************************************************
   /*!\brief Returns whether the expression is aliased with the given address \a alias.
   //
   // \param alias The alias to be checked.
   // \return \a true in case an alias effect is detected, \a false otherwise.
   */
   template< typename T >
   inline bool isAliased( const T* alias ) const noexcept {
      return dt_.isAliased( alias );
   }
   //**********************************************************************************************

   //**********************************************************************************************
   /*!\brief Returns whether the operands of the expression are properly aligned in memory.
   //
   // \return \a true in case the operands are aligned, \a false if not.
   */
   inline bool isAligned() const noexcept {
      return dt_.isAligned();
   }
   //**********************************************************************************************

   //**********************************************************************************************
   /*!\brief Returns whether the expression can be used in SMP assignments.
   //
   // \return \a true in case the expression can be used in SMP assignments, \a false if not.
   */
   inline bool canSMPAssign() const noexcept {
      return dt_.canSMPAssign();
   }
   //**********************************************************************************************

 private:
   //**Member variables****************************************************************************
   Operand dt_;  //!< Dense tensor of the evaluation expression.
   //**********************************************************************************************

   //**Assignment to dense tensors****************************************************************
   /*! \cond BLAZE_INTERNAL */
   /*!\brief Assignment of a dense tensor evaluation expression to a dense tensor.
   // \ingroup dense_tensor
   //
   // \param lhs The target left-hand side dense tensor.
   // \param rhs The right-hand side evaluation expression to be assigned.
   // \return void
   //
   // This function implements the performance optimized assignment of a dense tensor evaluation
   // expression to a dense tensor.
   */
   template< typename MT2 > // Type of the target dense tensor
   friend inline void assign( DenseTensor<MT2>& lhs, const DTensEvalExpr& rhs )
   {
      BLAZE_FUNCTION_TRACE;

      BLAZE_INTERNAL_ASSERT( (~lhs).rows()    == rhs.rows()   , "Invalid number of rows"    );
      BLAZE_INTERNAL_ASSERT( (~lhs).columns() == rhs.columns(), "Invalid number of columns" );
      BLAZE_INTERNAL_ASSERT( (~lhs).pages()   == rhs.pages(),   "Invalid number of pages"   );

      assign( ~lhs, rhs.dt_ );
   }
   /*! \endcond */
   //**********************************************************************************************

   //**Addition assignment to dense tensors*******************************************************
   /*! \cond BLAZE_INTERNAL */
   /*!\brief Addition assignment of a dense tensor evaluation expression to a dense tensor.
   // \ingroup dense_tensor
   //
   // \param lhs The target left-hand side dense tensor.
   // \param rhs The right-hand side evaluation expression to be added.
   // \return void
   //
   // This function implements the performance optimized addition assignment of a dense tensor
   // evaluation expression to a dense tensor.
   */
   template< typename MT2 > // Type of the target dense tensor
   friend inline void addAssign( DenseTensor<MT2>& lhs, const DTensEvalExpr& rhs )
   {
      BLAZE_FUNCTION_TRACE;

      BLAZE_INTERNAL_ASSERT( (~lhs).rows()    == rhs.rows()   , "Invalid number of rows"    );
      BLAZE_INTERNAL_ASSERT( (~lhs).columns() == rhs.columns(), "Invalid number of columns" );
      BLAZE_INTERNAL_ASSERT( (~lhs).pages()   == rhs.pages(),   "Invalid number of pages"   );

      addAssign( ~lhs, rhs.dt_ );
   }
   /*! \endcond */
   //**********************************************************************************************

   //**Subtraction assignment to dense tensors****************************************************
   /*! \cond BLAZE_INTERNAL */
   /*!\brief Subtraction assignment of a dense tensor evaluation expression to a dense tensor.
   // \ingroup dense_tensor
   //
   // \param lhs The target left-hand side dense tensor.
   // \param rhs The right-hand side evaluation expression to be subtracted.
   // \return void
   //
   // This function implements the performance optimized subtraction assignment of a dense tensor
   // evaluation expression to a dense tensor.
   */
   template< typename MT2 > // Type of the target dense tensor
   friend inline void subAssign( DenseTensor<MT2>& lhs, const DTensEvalExpr& rhs )
   {
      BLAZE_FUNCTION_TRACE;

      BLAZE_INTERNAL_ASSERT( (~lhs).rows()    == rhs.rows()   , "Invalid number of rows"    );
      BLAZE_INTERNAL_ASSERT( (~lhs).columns() == rhs.columns(), "Invalid number of columns" );
      BLAZE_INTERNAL_ASSERT( (~lhs).pages()   == rhs.pages(),   "Invalid number of pages"   );

      subAssign( ~lhs, rhs.dt_ );
   }
   /*! \endcond */
   //**********************************************************************************************

   //**Schur product assignment to dense tensors**************************************************
   /*! \cond BLAZE_INTERNAL */
   /*!\brief Schur product assignment of a dense tensor evaluation expression to a dense tensor.
   // \ingroup dense_tensor
   //
   // \param lhs The target left-hand side dense tensor.
   // \param rhs The right-hand side evaluation expression for the Schur product.
   // \return void
   //
   // This function implements the performance optimized Schur product assignment of a dense
   // tensor evaluation expression to a dense tensor.
   */
   template< typename MT2 > // Type of the target dense tensor
   friend inline void schurAssign( DenseTensor<MT2>& lhs, const DTensEvalExpr& rhs )
   {
      BLAZE_FUNCTION_TRACE;

      BLAZE_INTERNAL_ASSERT( (~lhs).rows()    == rhs.rows()   , "Invalid number of rows"    );
      BLAZE_INTERNAL_ASSERT( (~lhs).columns() == rhs.columns(), "Invalid number of columns" );
      BLAZE_INTERNAL_ASSERT( (~lhs).pages()   == rhs.pages(),   "Invalid number of pages"   );

      schurAssign( ~lhs, rhs.dt_ );
   }
   /*! \endcond */
   //**********************************************************************************************

   //**Multiplication assignment to dense tensors*************************************************
   /*! \cond BLAZE_INTERNAL */
   /*!\brief Multiplication assignment of a dense tensor evaluation expression to a dense tensor.
   // \ingroup dense_tensor
   //
   // \param lhs The target left-hand side dense tensor.
   // \param rhs The right-hand side evaluation expression to be multiplied.
   // \return void
   //
   // This function implements the performance optimized multiplication assignment of a dense
   // tensor evaluation expression to a dense tensor.
   */
   template< typename MT2 > // Type of the target dense tensor
   friend inline void multAssign( DenseTensor<MT2>& lhs, const DTensEvalExpr& rhs )
   {
      BLAZE_FUNCTION_TRACE;

      BLAZE_INTERNAL_ASSERT( (~lhs).rows()    == rhs.rows()   , "Invalid number of rows"    );
      BLAZE_INTERNAL_ASSERT( (~lhs).columns() == rhs.columns(), "Invalid number of columns" );
      BLAZE_INTERNAL_ASSERT( (~lhs).pages()   == rhs.pages(),   "Invalid number of pages"   );

      multAssign( ~lhs, rhs.dt_ );
   }
   /*! \endcond */
   //**********************************************************************************************

   //**SMP assignment to dense tensors************************************************************
   /*! \cond BLAZE_INTERNAL */
   /*!\brief SMP assignment of a dense tensor evaluation expression to a dense tensor.
   // \ingroup dense_tensor
   //
   // \param lhs The target left-hand side dense tensor.
   // \param rhs The right-hand side evaluation expression to be assigned.
   // \return void
   //
   // This function implements the performance optimized SMP assignment of a dense tensor
   // evaluation expression to a dense tensor.
   */
   template< typename MT2 > // Type of the target dense tensor
   friend inline void smpAssign( DenseTensor<MT2>& lhs, const DTensEvalExpr& rhs )
   {
      BLAZE_FUNCTION_TRACE;

      BLAZE_INTERNAL_ASSERT( (~lhs).rows()    == rhs.rows()   , "Invalid number of rows"    );
      BLAZE_INTERNAL_ASSERT( (~lhs).columns() == rhs.columns(), "Invalid number of columns" );
      BLAZE_INTERNAL_ASSERT( (~lhs).pages()   == rhs.pages(),   "Invalid number of pages"   );

      smpAssign( ~lhs, rhs.dt_ );
   }
   /*! \endcond */
   //**********************************************************************************************

   //**SMP addition assignment to dense tensors***************************************************
   /*! \cond BLAZE_INTERNAL */
   /*!\brief SMP addition assignment of a dense tensor evaluation expression to a dense tensor.
   // \ingroup dense_tensor
   //
   // \param lhs The target left-hand side dense tensor.
   // \param rhs The right-hand side evaluation expression to be added.
   // \return void
   //
   // This function implements the performance optimized SMP addition assignment of a dense tensor
   // evaluation expression to a dense tensor.
   */
   template< typename MT2 > // Type of the target dense tensor
   friend inline void smpAddAssign( DenseTensor<MT2>& lhs, const DTensEvalExpr& rhs )
   {
      BLAZE_FUNCTION_TRACE;

      BLAZE_INTERNAL_ASSERT( (~lhs).rows()    == rhs.rows()   , "Invalid number of rows"    );
      BLAZE_INTERNAL_ASSERT( (~lhs).columns() == rhs.columns(), "Invalid number of columns" );
      BLAZE_INTERNAL_ASSERT( (~lhs).pages()   == rhs.pages(),   "Invalid number of pages"   );

      smpAddAssign( ~lhs, rhs.dt_ );
   }
   /*! \endcond */
   //**********************************************************************************************

   //**SMP subtraction assignment to dense tensors************************************************
   /*! \cond BLAZE_INTERNAL */
   /*!\brief SMP subtraction assignment of a dense tensor evaluation expression to a dense tensor.
   // \ingroup dense_tensor
   //
   // \param lhs The target left-hand side dense tensor.
   // \param rhs The right-hand side evaluation expression to be subtracted.
   // \return void
   //
   // This function implements the performance optimized SMP subtraction assignment of a dense
   // tensor evaluation expression to a dense tensor.
   */
   template< typename MT2 > // Type of the target dense tensor
   friend inline void smpSubAssign( DenseTensor<MT2>& lhs, const DTensEvalExpr& rhs )
   {
      BLAZE_FUNCTION_TRACE;

      BLAZE_INTERNAL_ASSERT( (~lhs).rows()    == rhs.rows()   , "Invalid number of rows"    );
      BLAZE_INTERNAL_ASSERT( (~lhs).columns() == rhs.columns(), "Invalid number of columns" );
      BLAZE_INTERNAL_ASSERT( (~lhs).pages()   == rhs.pages(),   "Invalid number of pages"   );

      smpSubAssign( ~lhs, rhs.dt_ );
   }
   /*! \endcond */
   //**********************************************************************************************

   //**SMP Schur product assignment to dense tensors**********************************************
   /*! \cond BLAZE_INTERNAL */
   /*!\brief SMP Schur product assignment of a dense tensor evaluation expression to a dense tensor.
   // \ingroup dense_tensor
   //
   // \param lhs The target left-hand side dense tensor.
   // \param rhs The right-hand side evaluation expression for the Schur product.
   // \return void
   //
   // This function implements the performance optimized SMP Schur product assignment of a dense
   // tensor evaluation expression to a dense tensor.
   */
   template< typename MT2 > // Type of the target dense tensor
   friend inline void smpSchurAssign( DenseTensor<MT2>& lhs, const DTensEvalExpr& rhs )
   {
      BLAZE_FUNCTION_TRACE;

      BLAZE_INTERNAL_ASSERT( (~lhs).rows()    == rhs.rows()   , "Invalid number of rows"    );
      BLAZE_INTERNAL_ASSERT( (~lhs).columns() == rhs.columns(), "Invalid number of columns" );
      BLAZE_INTERNAL_ASSERT( (~lhs).pages()   == rhs.pages(),   "Invalid number of pages"   );

      smpSchurAssign( ~lhs, rhs.dt_ );
   }
   /*! \endcond */
   //**********************************************************************************************

   //**SMP multiplication assignment to dense tensors*********************************************
   /*! \cond BLAZE_INTERNAL */
   /*!\brief SMP multiplication assignment of a dense tensor evaluation expression to a dense
   //        tensor.
   // \ingroup dense_tensor
   //
   // \param lhs The target left-hand side dense tensor.
   // \param rhs The right-hand side evaluation expression to be multiplied.
   // \return void
   //
   // This function implements the performance optimized SMP multiplication assignment of a dense
   // tensor evaluation expression to a dense tensor.
   */
   template< typename MT2 > // Type of the target dense tensor
   friend inline void smpMultAssign( DenseTensor<MT2>& lhs, const DTensEvalExpr& rhs )
   {
      BLAZE_FUNCTION_TRACE;

      BLAZE_INTERNAL_ASSERT( (~lhs).rows()    == rhs.rows()   , "Invalid number of rows"    );
      BLAZE_INTERNAL_ASSERT( (~lhs).columns() == rhs.columns(), "Invalid number of columns" );
      BLAZE_INTERNAL_ASSERT( (~lhs).pages()   == rhs.pages(),   "Invalid number of pages"   );

      smpMultAssign( ~lhs, rhs.dt_ );
   }
   /*! \endcond */
   //**********************************************************************************************

   //**Compile time checks*************************************************************************
   /*! \cond BLAZE_INTERNAL */
   BLAZE_CONSTRAINT_MUST_BE_DENSE_TENSOR_TYPE( MT );
   /*! \endcond */
   //**********************************************************************************************
};
//*************************************************************************************************




//=================================================================================================
//
//  GLOBAL FUNCTIONS
//
//=================================================================================================

//*************************************************************************************************
/*!\brief Forces the evaluation of the given dense tensor expression \a dm.
// \ingroup dense_tensor
//
// \param dm The input tensor.
// \return The evaluated dense tensor.
//
// The \a eval function forces the evaluation of the given dense tensor expression \a dm.
// The function returns an expression representing the operation.\n
// The following example demonstrates the use of the \a eval function:

   \code
   blaze::DynamicMatrix<double> A, B;
   // ... Resizing and initialization
   B = eval( A );
   \endcode
*/
template< typename MT > // Type of the target dense tensor
inline decltype(auto) eval( const DenseTensor<MT>& dm )
{
   BLAZE_FUNCTION_TRACE;

   using ReturnType = const DTensEvalExpr<MT>;
   return ReturnType( ~dm );
}
//*************************************************************************************************




//=================================================================================================
//
//  GLOBAL RESTRUCTURING FUNCTIONS
//
//=================================================================================================

//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
/*!\brief Evaluation of the given dense tensor evaluation expression \a dm.
// \ingroup dense_tensor
//
// \param dm The input evaluation expression.
// \return The evaluated dense tensor.
//
// This function implements a performance optimized treatment of the evaluation of a dense tensor
// evaluation expression.
*/
template< typename MT > // Type of the target dense tensor
inline decltype(auto) eval( const DTensEvalExpr<MT>& dm )
{
   return dm;
}
/*! \endcond */
//*************************************************************************************************




//=================================================================================================
//
//  ISALIGNED SPECIALIZATIONS
//
//=================================================================================================

//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
template< typename MT >
struct IsAligned< DTensEvalExpr<MT> >
   : public IsAligned<MT>
{};
/*! \endcond */
//*************************************************************************************************




//=================================================================================================
//
//  ISSYMMETRIC SPECIALIZATIONS
//
//=================================================================================================

//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
template< typename MT >
struct IsSymmetric< DTensEvalExpr<MT> >
   : public IsSymmetric<MT>
{};
/*! \endcond */
//*************************************************************************************************




//=================================================================================================
//
//  ISHERMITIAN SPECIALIZATIONS
//
//=================================================================================================

//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
template< typename MT >
struct IsHermitian< DTensEvalExpr<MT> >
   : public IsHermitian<MT>
{};
/*! \endcond */
//*************************************************************************************************




//=================================================================================================
//
//  ISLOWER SPECIALIZATIONS
//
//=================================================================================================

//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
template< typename MT >
struct IsLower< DTensEvalExpr<MT> >
   : public IsLower<MT>
{};
/*! \endcond */
//*************************************************************************************************




//=================================================================================================
//
//  ISUNILOWER SPECIALIZATIONS
//
//=================================================================================================

//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
template< typename MT >
struct IsUniLower< DTensEvalExpr<MT> >
   : public IsUniLower<MT>
{};
/*! \endcond */
//*************************************************************************************************




//=================================================================================================
//
//  ISSTRICTLYLOWER SPECIALIZATIONS
//
//=================================================================================================

//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
template< typename MT >
struct IsStrictlyLower< DTensEvalExpr<MT> >
   : public IsStrictlyLower<MT>
{};
/*! \endcond */
//*************************************************************************************************




//=================================================================================================
//
//  ISUPPER SPECIALIZATIONS
//
//=================================================================================================

//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
template< typename MT >
struct IsUpper< DTensEvalExpr<MT> >
   : public IsUpper<MT>
{};
/*! \endcond */
//*************************************************************************************************




//=================================================================================================
//
//  ISUNIUPPER SPECIALIZATIONS
//
//=================================================================================================

//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
template< typename MT >
struct IsUniUpper< DTensEvalExpr<MT> >
   : public IsUniUpper<MT>
{};
/*! \endcond */
//*************************************************************************************************




//=================================================================================================
//
//  ISSTRICTLYUPPER SPECIALIZATIONS
//
//=================================================================================================

//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
template< typename MT >
struct IsStrictlyUpper< DTensEvalExpr<MT> >
   : public IsStrictlyUpper<MT>
{};
/*! \endcond */
//*************************************************************************************************

} // namespace blaze

#endif
