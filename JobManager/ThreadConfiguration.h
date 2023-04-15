#pragma once

enum class ThreadAffinity
{
    Default = 0,
    Strict, // Put all affinity types that should be executed only in specialized thread after this
    IO,
    Count // Add new affinity types before this
};

class ThreadConfiguration
{
public:
    ThreadConfiguration() = default;
    ThreadConfiguration(ThreadAffinity affinity, bool bStrict = false);

    bool IsStrict() const;
    ThreadAffinity GetAffinity() const;

private:
    ThreadAffinity m_affinity { ThreadAffinity::Default };
    bool m_bStrict { false };
};

