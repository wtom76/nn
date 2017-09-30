#pragma once
#include <array>
#include <vector>
#include <spdlog/spdlog.h>
#include <spdlog/fmt/ostr.h>

using namespace std;

class Network
{
	static constexpr size_t input_size_ = 1;
	static constexpr size_t omega_size_ = 1;

	array<double, input_size_> input_out_;
	array<double, omega_size_> omega_out_;
	array<double, omega_size_> target_;
	array<double, omega_size_> error_;
	array<double, input_size_ * omega_size_> weights_;
	vector<double> input_series_;
	vector<double> target_series_;

	vector<double>::const_iterator next_input_;
	vector<double>::const_iterator next_target_;

	shared_ptr<spdlog::logger> log_;

	void _initInputAndTarget();
	void _initWeights();
	void _feedInput();
	void _feedTarget();
	void _feedOmega();
	double _feedError();
	void _correctWeights(double error);

	void _forward();

public:
	Network(shared_ptr<spdlog::logger> log_ptr);
	~Network();

	void init();
	void learn();

	std::ostream& write(std::ostream& s) const;
};

inline std::ostream& operator <<(std::ostream& s, const Network& src)
{
	return src.write(s);
}