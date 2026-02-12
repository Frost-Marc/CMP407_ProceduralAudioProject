#pragma once
#include <SFML/Audio.hpp>
#include <vector>
#include <random>
#include <cstdint>

class FireSoundGen : public sf::SoundStream
{
public:
	FireSoundGen();

	void setIntensity(float value);
	void setCrackleRate(float value);
	void setBrightness(float value);
	void setFlickerSpeed(float value);

private:
	virtual bool onGetData(Chunk& data) override;
	virtual void onSeek(sf::Time timeOffset) override;

	float generateSample();
	float whiteNoise();
	void triggerCrackle();

private:
	std::vector<std::int16_t> m_samples;

	const unsigned int m_sampleRate = 44100;
	const unsigned int m_channelCount = 2;
	const unsigned int m_chunkSize = 512;

	//parameters
	float m_intensity = 0.7f;
	float m_crackleRate = 20.0f;
	float m_brightness = 0.5f;
	float m_flickerSpeed = 0.5f;

	//state
	double m_time = 0.0;

	//filter state
	float m_prevFilteredL = 0.0f;
	float m_prevFilteredR = 0.0f;

	struct Crackle
	{
		float value = 0.0f;
		float decay = 0.95f;
		bool active = false;
	};

	static constexpr int MAX_CRACKLES = 16;
	Crackle m_crackles[MAX_CRACKLES];

	std::mt19937 m_rng;
	std::uniform_real_distribution<float> m_dist;
};

