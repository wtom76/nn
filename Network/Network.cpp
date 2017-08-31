#include "stdafx.h"
#include "Network.h"
#include <algorithm>
#include <numeric>

//---------------------------------------------------------------------------------------------------------
Network::Network()
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
	out_input_[0] = *next_input_++;
}
//---------------------------------------------------------------------------------------------------------
void Network::_feedTarget()
{
	out_target_[0] = *next_target_++;
}
//---------------------------------------------------------------------------------------------------------
void Network::_feedOmega()
{
	auto weight_i = cbegin(weights_);
	for (auto& omega_i : out_omega_)
	{
		omega_i = 0.;
		for (auto& input_i : out_input_)
		{
			omega_i += input_i * *weight_i++;
		}
	}
}
//---------------------------------------------------------------------------------------------------------
void Network::_feedError()
{
	auto omega_i = cbegin(out_omega_);
	auto target_i = cbegin(out_target_);
	auto error_i = begin(out_error_);
	while (error_i != end(out_error_))
	{
		*error_i++ = *omega_i++ - *target_i++;
	}
}
//---------------------------------------------------------------------------------------------------------
double Network::_error()
{
	return accumulate(cbegin(out_error_), cend(out_error_), 0.);
}
//---------------------------------------------------------------------------------------------------------
void Network::_correctWeights()
{
	auto error_i = begin(out_error_);
	while (error_i != end(out_error_))
	{
		const double correction = *error_i++ * 0.001;
		for (auto& weight_i : weights_)
		{
			weight_i -= correction;
		}
	}
}
//---------------------------------------------------------------------------------------------------------
void Network::init()
{
	_initInputAndTarget();
	_initWeights();
}
//---------------------------------------------------------------------------------------------------------
void Network::forward()
{
	_feedInput();
	_feedOmega();
}
//---------------------------------------------------------------------------------------------------------
void Network::backward()
{
	_feedTarget();
}
//---------------------------------------------------------------------------------------------------------
void Network::learn()
{
	static constexpr double min_error = 0.00001;
	double last_error = 0;
	do
	{
		next_input_ = cbegin(input_series_);
		next_target_ = cbegin(target_series_);
		while (next_input_ != end(input_series_))
		{
			forward();
			_feedError();
			last_error = _error();
			if (last_error >= min_error)
			{
				backward();
			}
		}
	}
	while (last_error >= min_error);
}
