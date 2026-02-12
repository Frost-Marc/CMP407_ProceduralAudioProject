#include "FireSoundGen.h"
#include <cmath>
#include <algorithm>

// commented code works but needs looked at

static constexpr float PI = 3.14159265359f;

FireSoundGen::FireSoundGen() : m_rng(std::random_device{}()), m_dist(-1.0f, 1.0f)
{
	initialize(m_channelCount, m_sampleRate, sf::SoundChannel::Stereo);
}

void FireSoundGen::setIntensity(float value)
{
	m_intensity = std::clamp(value, 0.0f, 1.0f);
}

void FireSoundGen::setCrackleRate(float value)
{
	m_crackleRate = std::max(0.0f, value);
}

void FireSoundGen::setBrightness(float value)
{
	m_brightness = std::clamp(value, 0.0f, 1.0f);
}

void FireSoundGen::setFlickerSpeed(float value)
{
	m_flickerSpeed = std::max(0.0f, value);
}

bool FireSoundGen::onGetData(Chunk& data)
{
    /*m_samples.resize(ChunkSize * ChannelCount);

    for (unsigned int i = 0; i < ChunkSize; ++i)
    {
        float sample = generateSample();
        std::int16_t s = static_cast<std::int16_t>(sample * 30000);

        m_samples[i * 2] = s;
        m_samples[i * 2 + 1] = s;
    }

    data.samples = m_samples.data();
    data.sampleCount = m_samples.size();

    return true;*/
}

void FireSoundGen::onSeek(sf::Time timeOffset)
{
    /*m_time = timeOffset.asSeconds();*/
}

float FireSoundGen::generateSample()
{
    //float dt = 1.0f / SampleRate;
    //m_time += dt;

    //// Flicker modulation
    //float lfo = 0.6f + 0.4f * std::sin(2.0f * PI * m_flickerSpeed * m_time);

    //// Base noise
    //float noiseL = whiteNoise();
    //float noiseR = whiteNoise();

    //// Simple low-pass filter
    //float cutoff = 200.0f + m_brightness * 4000.0f;
    //float alpha = cutoff / (cutoff + SampleRate);

    //float filteredL = m_prevFilteredL + alpha * (noiseL - m_prevFilteredL);
    //float filteredR = m_prevFilteredR + alpha * (noiseR - m_prevFilteredR);

    //m_prevFilteredL = filteredL;
    //m_prevFilteredR = filteredR;

    //float baseL = filteredL * lfo * m_intensity;
    //float baseR = filteredR * lfo * m_intensity;

    //// Random crackle triggering
    //float triggerChance = m_crackleRate / SampleRate;
    //if (((whiteNoise() + 1.f) * 0.5f) < triggerChance)
    //    triggerCrackle();

    //float crackleSum = 0.0f;

    //for (auto& c : m_crackles)
    //{
    //    if (c.active)
    //    {
    //        crackleSum += c.value * whiteNoise();
    //        c.value *= c.decay;

    //        if (std::abs(c.value) < 0.001f)
    //            c.active = false;
    //    }
    //}

    //float finalSample = baseL + baseR + crackleSum;
    //return std::clamp(finalSample, -1.0f, 1.0f);
}

float FireSoundGen::whiteNoise()
{
	return m_dist(m_rng);
}

void FireSoundGen::triggerCrackle()
{
	/*for (auto& c : m_crackles)
	{
		if (!c.active)
		{
			c.active = true;
			c.value = 0.8f;
			c.decay = 0.90f + 0.05f * ((whiteNoise() + 1.f) * 0.5f);
			break;
		}
	}*/
}
