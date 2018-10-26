//
// Created by snorri on 26.10.2018.
//

#ifndef VULKAN_SPRITES_CURVESCALAR_H
#define VULKAN_SPRITES_CURVESCALAR_H

#include <vector>
#include <algorithm>
#include <cmath>

namespace CurveExtrapolation
{
    // Curve behavior when requesting value outside of the time
    // range defined for the curve
    enum Type
    {
        // Use first/last values
        CLAMP = 0,
        // Cycle the curve
        CYCLE = 1,
        // Cycle the curve with mirroring
        MIRROR = 2,
        // Linear extrapolation
        LINEAR = 3
    };
}

template <typename T>
struct CurveKey {
    float time;
    T value;

    bool operator<(const CurveKey& other) {
        return time < other.time;
    }
};

template <typename T>
class Curve {
public:
    T GetValue(float t) {
        if(m_keys.empty()) {
            return T();
        }

        if(m_keys.size() == 1) {
            return m_keys[0].value;
        }

        auto n = m_keys.size();

        float startTime = m_keys.front().time;
        float endTime = m_keys.back().time;
        float duration = endTime - startTime;

        double intPart;

        if(t < startTime) {
            auto fracPart = static_cast<float>(modf(-(t - startTime) / duration, &intPart));
            switch(m_extrapolationType) {
                case CurveExtrapolation::CLAMP:
                    return m_keys.front().value;
                case CurveExtrapolation::CYCLE:
                    t = fracPart * duration;
                    break;
                case CurveExtrapolation::MIRROR:
                    if(int(intPart) % 2 != 0) {
                        fracPart = 1.0f - fracPart;
                    }
                    t = fracPart * duration;
                    break;
                case CurveExtrapolation::LINEAR:
                    return GetSegmentValue(t, m_keys[0], m_keys[1]);
            }
        } else if(t > endTime) {
            auto fracPart = static_cast<float>(modf((t - startTime) / duration, &intPart));
            switch(m_extrapolationType) {
                case CurveExtrapolation::CLAMP:
                    return m_keys.back().value;
                case CurveExtrapolation::CYCLE:
                    t = fracPart * duration;
                    break;
                case CurveExtrapolation::MIRROR:
                    if(int(intPart) % 2 != 0) {
                        fracPart = 1.0f - fracPart;
                    }
                    t = fracPart * duration;
                    break;
                case CurveExtrapolation::LINEAR:
                    return GetSegmentValue(t, m_keys[n-2], m_keys[n-1]);
            }
        }

        for(int i = 0; i < n - 1; ++i) {
            auto& k0 = m_keys[i];
            auto& k1 = m_keys[i + 1];
            if(t >= k0.time && t <= k1.time) {
                return GetSegmentValue(t, k0, k1);
            }
        }

        return T();
    }

    int GetNumKeys() const {
        return static_cast<int>(m_keys.size());
    }

    void AddKey(float t, const T& v) {
        m_keys.push_back({t, v});

        std::sort(m_keys.begin(), m_keys.end());
    }

    CurveExtrapolation::Type GetExtrapolationType() const {
        return m_extrapolationType;
    }

    void SetExtrapolationType(CurveExtrapolation::Type et) {
        m_extrapolationType = et;
    }

protected:
    std::vector<CurveKey<T>> m_keys;

    T GetSegmentValue(float t, const CurveKey<T> &k0, const CurveKey<T> &k1) const {
        auto p = (t - k0.time) / (k1.time - k0.time);
        auto value = k0.value + p*(k1.value - k0.value);
        return value;
    }

    CurveExtrapolation::Type m_extrapolationType = CurveExtrapolation::CLAMP;
};



#endif //VULKAN_SPRITES_CURVESCALAR_H
