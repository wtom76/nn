#pragma once
#include <array>
#include <vector>

using namespace std;

class Network
{
	static constexpr size_t input_size_ = 1;
	static constexpr size_t omega_size_ = 1;

	array<double, input_size_> out_input_;
	array<double, omega_size_> out_omega_;
	array<double, omega_size_> out_target_;
	array<double, omega_size_> out_error_;
	array<double, input_size_ * omega_size_> weights_;
	vector<double> input_series_;
	vector<double> target_series_;

	vector<double>::const_iterator next_input_;
	vector<double>::const_iterator next_target_;

	void _initInputAndTarget();
	void _initWeights();
	void _feedInput();
	void _feedTarget();
	void _feedOmega();
	void _feedError();
	double _error();
	void _correctWeights();
public:
	Network();
	~Network();

	void init();
	void forward();
	void backward();
	void learn();
};

