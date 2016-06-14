#ifndef __WILLIS_TIMER_H
#define __WILLIS_TIMER_H

#ifdef OPENCV
#include <sys/time.h>
typedef unsigned long long nsecs_t;
#define MY_LOG(...) \
    printf(__VA_ARGS__)
#elif ANDROID

#include <cutils/log.h>
#include <utils/Timers.h>
#include <utils/threads.h>
#define LOG_TAG "Timer"
#define MY_LOG(...) \
    ALOGD(__VA_ARGS__)

#endif
namespace willis {
class DurationTimer {
public:
    DurationTimer()
    {
        memset(&m_startTime, 0x00, sizeof(struct timeval));
        memset(&m_stopTime, 0x00, sizeof(struct timeval));
    }
    ~DurationTimer() {}

    void start()
    {
        gettimeofday(&m_startTime, NULL);
    };

    void stop()
    {
        gettimeofday(&m_stopTime, NULL);
    };

    uint64_t durationMsecs() const
    {
        nsecs_t stop  = ((nsecs_t)m_stopTime.tv_sec) * 1000LL + ((nsecs_t)m_stopTime.tv_usec) / 1000LL;
        nsecs_t start = ((nsecs_t)m_startTime.tv_sec) * 1000LL + ((nsecs_t)m_startTime.tv_usec) / 1000LL;

        return stop - start;
    };

    uint64_t durationUsecs() const
    {
        nsecs_t stop  = ((nsecs_t)m_stopTime.tv_sec) * 1000000LL + ((nsecs_t)m_stopTime.tv_usec);
        nsecs_t start = ((nsecs_t)m_startTime.tv_sec) * 1000000LL + ((nsecs_t)m_startTime.tv_usec);

        return stop - start;
    };

    uint64_t intervalMsecs()
    {
        gettimeofday(&m_curTime, NULL);
        nsecs_t cur  = ((nsecs_t)m_curTime.tv_sec) * 1000LL + ((nsecs_t)m_curTime.tv_usec) / 1000LL;
        nsecs_t start = ((nsecs_t)m_startTime.tv_sec) * 1000LL + ((nsecs_t)m_startTime.tv_usec) / 1000LL;

        return cur - start;
    }

    uint64_t intervalUsecs()
    {
        gettimeofday(&m_curTime, NULL);
        nsecs_t cur  = ((nsecs_t)m_curTime.tv_sec) * 1000000LL + ((nsecs_t)m_curTime.tv_usec);
        nsecs_t start = ((nsecs_t)m_startTime.tv_sec) * 1000000LL + ((nsecs_t)m_startTime.tv_usec);

        return cur - start;
    }
private:
    struct timeval  m_startTime;
    struct timeval  m_stopTime;
    struct timeval  m_curTime;
};

class AutoTimer {
    private:
        AutoTimer(void)
        {}

    public:
        inline AutoTimer(char *strLog)
        {
            if (m_create(strLog) == false)
                MY_LOG("ERR(%s):m_create() fail \n", __func__);
        }

        inline AutoTimer(const char *strLog)
        {
            char *strTemp = (char*)strLog;

            if (m_create(strTemp) == false)
                MY_LOG("ERR(%s):m_create() fail, \n", __func__);
        }

        inline virtual ~AutoTimer()
        {
            uint64_t durationTime;

            m_timer.stop();

            durationTime = m_timer.durationMsecs();

            if (m_logStr) {
                MY_LOG("DEBUG:duration time(%5d msec):(%s)\n",
                        (int)durationTime, m_logStr);
            } else {
                MY_LOG("DEBUG:duration time(%5d msec):(NULL)\n",
                        (int)durationTime);
            }
        }

    private:
        bool m_create(char *strLog)
        {
            m_logStr = strLog;

            m_timer.start();

            return true;
        }

    private:
        DurationTimer m_timer;
        char         *m_logStr;
};


};
#endif // __WILLIS_TIMER_H
