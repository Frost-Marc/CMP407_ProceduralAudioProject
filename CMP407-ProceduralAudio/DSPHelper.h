#pragma once
#include <random>
#include <cmath>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

namespace DSP
{
	inline float WhiteNoise()
	{
		static std::mt19937 gen{ std::random_device{}() };
		static std::uniform_real_distribution<float> dist{ -1.0f, 1.0f };
		return dist(gen);
	}

	inline bool Chance(float probability)
	{
		static std::mt19937 gen{ std::random_device{}() };
		static std::uniform_real_distribution<float> dist{ 0.0f, 1.0f };
		return dist(gen) < probability;
	}

	struct PinkNoise
	{
		float b0 = 0.0f, b1 = 0.0f, b2 = 0.0f;

		float Next()
		{
			float white = WhiteNoise();
			b0 = 0.99765f * b0 + white * 0.0990460f;
			b1 = 0.96300f * b1 + white * 0.2965164f;
			b2 = 0.57000f * b2 + white * 1.0526913f;
			return b0 + b1 + b2 + (white * 0.1848f);
		}
	};

	inline float ApplyLowPass(float input, float cutoff, float sampleRate, float& state) 
	{
		float rc = 1.0f / (cutoff * 2.0f * (float)M_PI);
		float dt = 1.0f / sampleRate;
		float alpha = dt / (rc + dt);
		state = state + alpha * (input - state);
		return state;
	}

	inline float ApplyHighPass(float input, float cutoff, float sampleRate, float& state, float& lastIn) 
	{
		float rc = 1.0f / (cutoff * 2.0f * (float)M_PI);
		float dt = 1.0f / sampleRate;
		float alpha = rc / (rc + dt);
		state = alpha * (state + input - lastIn);
		lastIn = input;
		return state;
	}
}