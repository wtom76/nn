#include "stdafx.h"
#include "Network.h"
#include <algorithm>
#include <numeric>
#include <iostream>

//---------------------------------------------------------------------------------------------------------
Network::Network(shared_ptr<spdlog::logger> log_ptr) : log_(log_ptr)
{
}
//---------------------------------------------------------------------------------------------------------
Network::~Network()
{
}
//---------------------------------------------------------------------------------------------------------
void Network::_initInputAndTarget()
{
	double value = 0.;
	input_series_.resize(1000);
	target_series_.resize(1000);
	generate(begin(input_series_), end(input_series_), [&value]() { value += 0.1; return value; });
	value = 0.;
	generate(begin(target_series_), end(target_series_), [&value]() { value += 0.1; return value; });
}
//---------------------------------------------------------------------------------------------------------
void Network::_initWeights()
{
	fill(begin(weights_), end(weights_), 0.5);
}
//---------------------------------------------------------------------------------------------------------
void Network::_feedInput()
{
	input_out_[0] = *next_input_++;
}
//---------------------------------------------------------------------------------------------------------
void Network::_feedTarget()
{
	target_[0] = *next_target_++;
}
//---------------------------------------------------------------------------------------------------------
void Network::_feedOmega()
{
	auto weight_i = cbegin(weights_);
	for (auto& omega_i : omega_out_)
	{
		omega_i = 0.;
		for (auto& input_i : input_out_)
		{
			omega_i += input_i * *weight_i++;
		}
	}
}
//---------------------------------------------------------------------------------------------------------
double Network::_feedError()
{
	double sample_error = 0.;
	auto omega_i = cbegin(omega_out_);
	auto target_i = cbegin(target_);
	auto error_i = begin(error_);
	while (error_i != end(error_))
	{
		*error_i = *omega_i++ - *target_i++;
		sample_error += *error_i / 2.;
		++error_i;
	}
	return sample_error;
}
//---------------------------------------------------------------------------------------------------------
void Network::_correctWeights(double error)
{
	auto input_out_i = begin(input_out_);
	for (auto& weight_i : weights_)
	{
		weight_i -= *input_out_i++ * 0.001 * error;
	}
}
//---------------------------------------------------------------------------------------------------------
void Network::init()
{
	_initInputAndTarget();
	_initWeights();
}
//---------------------------------------------------------------------------------------------------------
void Network::_forward()
{
	_feedInput();
	_feedOmega();
}
//---------------------------------------------------------------------------------------------------------
void Network::learn()
{
	static constexpr double min_error = 0.00001;
	while (true)
	{
		double error = 0.;
		next_input_ = cbegin(input_series_);
		next_target_ = cbegin(target_series_);

		while (next_input_ != end(input_series_))
		{
			_forward();
			_feedTarget();
			const double error_p = _feedError();
			_correctWeights(error_p);
			error += error_p;
			log_->info("{:}", *this);
		}

		if (abs(error) <= min_error)
		{
			break;
		}
	}
}

std::ostream& Network::write(std::ostream& s) const
{
	s << endl << "o(input):" << endl;
	for_each(begin(input_out_), end(input_out_), [this, &s](const auto& val) { s << '\t' << val; });
	s << endl << "o(omega):" << endl;
	for_each(begin(omega_out_), end(omega_out_), [this, &s](const auto& val) { s << '\t' << val; });
	s << endl << "t(omega):" << endl;
	for_each(begin(target_), end(target_), [this, &s](const auto& val) { s << '\t' << val; });
	s << endl << "e(omega):" << endl;
	for_each(begin(error_), end(error_), [this, &s](const auto& val) { s << '\t' << val; });
	s << endl << "weights:" << endl;
	for_each(begin(weights_), end(weights_), [this, &s](const auto& val) { s << '\t' << val; });
	return s;
}
