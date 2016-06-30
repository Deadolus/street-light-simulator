#include "daysimulator.h"

#include <QDebug>
#include <random>
#include <ctime>

const int kDayCycle = 60;
const int kDayEventDuration = 60;
const int kDayRandom = 5;

DaySimulator::DaySimulator(QObject *parent) : EventSimulator(parent)
{
    this->setCycle(kDayCycle);
    this->setDuration(kDayEventDuration);
    setRandomTime(kDayRandom);
    setTiming( ((kDayCycle-kDayRandom) / 2.0) / kDayCycle);
    qDebug() << "Random time for day is " << getRandomTime() <<", timing is " << getTiming();

}

void DaySimulator::setRandomTime(const uint &randomTime)
{
    EventSimulator::setRandomTime(randomTime);
    setTiming( ((getCycle()-randomTime) / 2.0) / getCycle());
}

void DaySimulator::setCycle(const uint &cycle)
{
    EventSimulator::setCycle(cycle);
    setTiming( ((cycle-getRandomTime()) / 2.0) / cycle);

}
DaySimulator::Daytime DaySimulator::getDaytime() const
{
    return daytime_;
}

uint64_t DaySimulator::getNightStart() const
{
    return nightStart_;

}

void DaySimulator::setDaytime(const Daytime &daytime)
{
    if(daytime != daytime_) {
        daytime_ = daytime;
        emit daytimeChanged(daytime_);
    }
}

void DaySimulator::timeout()
{
    uint64_t now = time(0);
    qDebug() << "Now is: "<<now%60<<" Night start: "<<getEventStart(now)%60;
    setNightStart(getEventStart(now));
    if( (getEventStart(now) < now) && (getEventEnd(now) > now))
        setDaytime(DaySimulator::NIGHT);
    else
        setDaytime(DaySimulator::DAY);
}

void DaySimulator::setNightStart(uint64_t nightStart)
{
    if(nightStart != nightStart_) {
        nightStart_ = nightStart;
        emit nightStartChanged(nightStart_);
    }
}
