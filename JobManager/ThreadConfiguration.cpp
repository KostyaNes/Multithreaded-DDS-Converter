#include "ThreadConfiguration.h"

ThreadConfiguration::ThreadConfiguration(ThreadAffinity affinity, bool bStrict)
    : m_affinity(affinity)
    , m_bStrict(bStrict)
{
}

bool ThreadConfiguration::IsStrict() const
{
    return m_bStrict;
}

ThreadAffinity ThreadConfiguration::GetAffinity() const
{
    return m_affinity;
}
