#pragma once
#include <SFML/Audio.hpp>
#include <vector>
#include <cstdint>
#include "DSPHelper.h"

class FireAudio : public sf::SoundStream
{
public:
    // --- ImGui Accessible Parameters ---
    float crackleProbability = 0.0001f;
    float crackleFilterFreq = 3500.0f;

    float hissGain = 0.05f;
    float hissFilterFreq = 8000.0f;

    float roarGain = 0.3f;
    float roarFilterFreq = 100.0f;
    float lappingSpeed = 0.5f;

	FireAudio(unsigned int sampleRate = 44100) : m_sampleRate(sampleRate)
	{
		m_sampleRate = sampleRate;
		initialize(1, sampleRate, {sf::SoundChannel::Mono});

        setRelativeToListener(false);
        setMinDistance(100.f);
        setAttenuation(1.0f);

        m_processingBuffer.resize(4096);
	}

private:
	unsigned int m_sampleRate;
    std::vector<std::int16_t> m_processingBuffer;

	// DSP States
	float _hissState = 0, _hissLastIn = 0;
	float _roarState = 0;
	float _crackleState = 0, _crackleLastIn = 0;
	float _lfoPhase = 0;

	// Pink Noise State
	DSP::PinkNoise _pink;

	bool onGetData(Chunk& data) override
	{
        float dt = 1.0f / static_cast<float>(m_sampleRate);
		float sRateF = static_cast<float>(m_sampleRate);

		for (int i = 0; i < m_processingBuffer.size(); i++) {
            // 1. THE HISS (High-frequency gas sound)
            float hiss = DSP::ApplyHighPass(DSP::WhiteNoise(), hissFilterFreq, sRateF, _hissState, _hissLastIn);

            // 2. THE CRACKLE (Sharp wood snapping)
            float crackleImpulse = 0.0f;
            if (DSP::Chance(crackleProbability))
            {
                crackleImpulse = DSP::WhiteNoise() * 0.8f;
            }
            float crackle = DSP::ApplyHighPass(crackleImpulse, crackleFilterFreq, sRateF, _crackleState, _crackleLastIn);

            // 3. THE ROAR (The body of the flame with LFO "breathing")
            _lfoPhase += 2.0f * (float)M_PI * lappingSpeed * dt;
            if (_lfoPhase > 2.0f * (float)M_PI) _lfoPhase -= 2.0f * (float)M_PI;

            float lappingMod = (std::sin(_lfoPhase) * 0.5f) + 0.5f;
            float roar = DSP::ApplyLowPass(_pink.Next(), roarFilterFreq, sRateF, _roarState);
            roar *= (0.5f + (lappingMod * 0.5f));

            // FINAL MIX & CLAMP
            float finalOutput = (hiss * hissGain) + (crackle * 1.0f) + (roar * roarGain);

            if (finalOutput > 1.0f)  finalOutput = 1.0f;
            if (finalOutput < -1.0f) finalOutput = -1.0f;

            m_processingBuffer[i] = static_cast<std::int16_t>(finalOutput * 32767);
		}

		data.samples = m_processingBuffer.data();
		data.sampleCount = m_processingBuffer.size();
		return true;
	}

	void onSeek(sf::Time time) override {}
};
