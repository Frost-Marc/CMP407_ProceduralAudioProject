#pragma once
#include <SFML/Audio.hpp>
#include <vector>
#include <random>>
#include <atomic>

class FireAudio : public sf::SoundStream
{
public:
	std::atomic<float> intensity{ 0.5f };
	std::atomic<float> crackle{ 0.4f };
	std::atomic<float> hiss{ 0.2f };

	FireAudio()
	{
		initialize(1, 44100, {sf::SoundChannel::Mono});
	}

private:
	std::mt19937 gen;
	std::uniform_real_distribution<float> dist{ -1.0f, 1.0f };
	float lastSample = 0.0f;

	bool onGetData(Chunk& data) override
	{
		static std::vector<std::int16_t> samples;
		samples.resize(2048);

		for (auto& sample : samples)
		{
			float white = dist(gen);

			float brown = (0.02f * white + 0.98f * lastSample);
			lastSample = brown;

			float crackle = 0.0f;
			if (std::uniform_real_distribution<float>(0, 10000)(gen) < crackle * 50.0f)
			{
				crackle = (white > 0 ? 1.0f : -1.0f) * 0.8f;
			}

			float combined = (brown * intensity) + (white * hiss * 0.1f) + crackle;

			if (combined > 1.0f)
			{
				combined = 1.0f;
			}
			
			if (combined < -1.0f)
			{
				combined = -1.0f;
			}

			sample = static_cast<std::int16_t>(combined * 32767);
		}

		data.samples = samples.data();
		data.sampleCount = samples.size();
		return true;
	}

	void onSeek(sf::Time time) override {}
};
