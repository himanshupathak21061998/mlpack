/**
 * @file radial_basis_function_impl.hpp
 * @author Himanshu Pathak
 *
 *
 * mlpack is free software; you may redistribute it and/or modify it under the
 * terms of the 3-clause BSD license.  You should have received a copy of the
 * 3-clause BSD license along with mlpack.  If not, see
 * http://www.opensource.org/licenses/BSD-3-Clause for more information.
 */
#ifndef MLPACK_METHODS_ANN_LAYER_RBF_IMPL_HPP
#define MLPACK_METHODS_ANN_LAYER_RBF_IMPL_HPP

// In case it hasn't yet been included.
#include "radial_basis_function.hpp"

namespace mlpack {
namespace ann /** Artificial Neural Network. */ {

template<typename InputDataType, typename OutputDataType,
         typename Activation>
RBF<InputDataType, OutputDataType, Activation>::RBF() :
    inSize(0),
    outSize(0)
{
  // Nothing to do here.
}

template<typename InputDataType, typename OutputDataType,
         typename Activation>
RBF<InputDataType, OutputDataType, Activation>::RBF(
    const size_t inSize,
    const size_t outSize,
    arma::mat& centres) :
    inSize(inSize),
    outSize(outSize),
    centres(centres)
{
  sigmas = arma::ones(1, 1);
  arma::mat dis = arma::mat(outSize, centres.n_cols);
  for (size_t i = 0; i < centres.n_cols; i++)
  {
    arma::mat temp = centres.each_col() - centres.col(i);
    dis.col(i) = arma::pow(arma::sum(
                                 arma::pow((temp),
                                 2), 0), 0.5).t();
  }
  sigmas = arma::max(dis);
}

template<typename InputDataType, typename OutputDataType,
         typename Activation>
template<typename eT>
void RBF<InputDataType, OutputDataType, Activation>::Forward(
    const arma::Mat<eT>& input,
    arma::Mat<eT>& output)
{
  distances = arma::mat(outSize, input.n_cols);

  for (size_t i = 0; i < input.n_cols; i++)
  {
    arma::mat temp = centres.each_col() - input.col(i);
    distances.col(i) = arma::pow(arma::sum(
                                 arma::pow((temp),
                                 2), 0), 0.5).t();
  }
  double betas = arma::accu(sigmas) / std::pow(2 * outSize, 0.5);

  Activation::Fn(std::pow(betas, 0.5) * distances, output);
}


template<typename InputDataType, typename OutputDataType,
         typename Activation>
template<typename eT>
void RBF<InputDataType, OutputDataType, Activation>::Backward(
    const arma::Mat<eT>& /* input */,
    const arma::Mat<eT>& gy,
    arma::Mat<eT>& g)
{
}

template<typename InputDataType, typename OutputDataType,
         typename Activation>
template<typename Archive>
void RBF<InputDataType, OutputDataType, Activation>::serialize(
    Archive& ar,
    const unsigned int /* version */)
{
  ar & BOOST_SERIALIZATION_NVP(distances);
  ar & BOOST_SERIALIZATION_NVP(centres);
}

} // namespace ann
} // namespace mlpack
#endif