#pragma once
#include <SFML/Audio.hpp>
#include <vector>
#include <cstdint>
#include "DSPHelper.h"

class FireAudio : public sf::SoundStream
{
public:
    // ImGui Accessible Parameters
    float crackleProbability = 0.0001f;
    float crackleFilterFreq = 3500.0f;
    float hissGain = 0.05f;
    float hissFilterFreq = 8000.0f;
    float roarGain = 0.3f;
    float roarFilterFreq = 100.0f;
    float lappingSpeed = 0.5f;

	FireAudio(unsigned int sampleRate = 44100) : m_sampleRate(sampleRate)
	{
		initialize(1, sampleRate, {sf::SoundChannel::Mono});

        setRelativeToListener(false);
        setPosition({ 0.f, 0.f, 0.f });
        setMinDistance(100.f);
        setAttenuation(10.0f);

        m_processingBuffer.resize(4096);

        // Initialize smoothed values to match targets immediately at start
        sCrackleFreq.setImmediately(crackleFilterFreq);
        sHissGain.setImmediately(hissGain);
        sHissFreq.setImmediately(hissFilterFreq);
        sRoarGain.setImmediately(roarGain);
        sRoarFreq.setImmediately(roarFilterFreq);
        sLappingSpeed.setImmediately(lappingSpeed);
        sOcclusionFreq.setImmediately(20000.0f);
	}

    void setOcclusionTraget(float freq)
    {
        sOcclusionFreq.target = freq;
    }

private:
	unsigned int m_sampleRate;
    std::vector<std::int16_t> m_processingBuffer;

    // DSP STATES
    DSP::HighPass  m_hissFilter;
    DSP::HighPass  m_crackleFilter;
    DSP::LowPass   m_roarFilter;
    DSP::LFO       m_lappingLFO;
    DSP::PinkNoise m_pink;
    DSP::LowPass   m_occlusionFilter;

    // SMOOTHING STATES
    DSP::Smoother sCrackleFreq;
    DSP::Smoother sHissGain;
    DSP::Smoother sHissFreq;
    DSP::Smoother sRoarGain;
    DSP::Smoother sRoarFreq;
    DSP::Smoother sLappingSpeed;
    DSP::Smoother sOcclusionFreq;

	bool onGetData(Chunk& data) override
	{
        float dt = 1.0f / static_cast<float>(m_sampleRate);
		float sRateF = static_cast<float>(m_sampleRate);

        // PARAMETER SMOOTHING
        sCrackleFreq.target = crackleFilterFreq;
        sHissGain.target = hissGain;
        sHissFreq.target = hissFilterFreq;
        sRoarGain.target = roarGain;
        sRoarFreq.target = roarFilterFreq;
        sLappingSpeed.target = lappingSpeed;

		for (int i = 0; i < m_processingBuffer.size(); i++) 
        {
            // THE HISS (High-frequency gas sound)
            float hiss = m_hissFilter.process(DSP::WhiteNoise(), sHissFreq.next(), sRateF);

            // THE CRACKLE (Sharp wood snapping)
            float crackleImpulse = DSP::Chance(crackleProbability) ? (DSP::WhiteNoise() * 0.8f) : 0.0f;
            float crackle = m_crackleFilter.process(crackleImpulse, sCrackleFreq.next(), sRateF);

            // THE ROAR (The body of the flame with LFO "breathing")
            float lfoValue = m_lappingLFO.process(sLappingSpeed.next(), dt);
            float roar = m_roarFilter.process(m_pink.Next(), sRoarFreq.next(), sRateF);
            roar *= (0.5f + (lfoValue * 0.5f));

            // FINAL MIX & CLAMP
            float finalOutput = (hiss * sHissGain.next()) + (crackle * 1.0f) + (roar * sRoarGain.next());

            // APPLY OCCLUSION
            finalOutput = m_occlusionFilter.process(finalOutput, sOcclusionFreq.next(), sRateF);

            m_processingBuffer[i] = static_cast<std::int16_t>(std::clamp(finalOutput, -1.0f, 1.0f) * 32767);
		}

		data.samples = m_processingBuffer.data();
		data.sampleCount = m_processingBuffer.size();
		return true;
	}

	void onSeek(sf::Time time) override {}
};
