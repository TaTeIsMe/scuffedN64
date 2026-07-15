#include "EventQ.h"
#include "RCP.h"
#include <algorithm>
#include<limits>
EventQ::EventQ(){}

EventQ::~EventQ()
{

}

void EventQ::enqueue(uint64_t time, EventType type)
{
    if(time < curr_min)curr_min = time;
    
    event_times[(uint8_t)type] = time;
}

void EventQ::process_queue()
{
    constexpr size_t num_events = static_cast<size_t>(EventType::COUNT);
    while (curr_min <= rcp->cycles)
    {
        uint64_t* min_ptr = std::min_element(event_times, event_times + 5);
        size_t min_idx = std::distance(event_times, min_ptr);
        event_times[min_idx] = std::numeric_limits<uint64_t>::max();
        
        switch ((EventType)min_idx)
        {
            case EventType::VI_DONE:
            rcp->mi.route_interrupt(InterruptSource::VI);
            enqueue(rcp->cycles + 1'500'000, EventType::VI_DONE); // this value shoudl be 1.5 mil for corect ratio
            break;
            
            case EventType::PI_DMA_DONE:
            rcp->pi.finish_dma();
            break;
            
            case EventType::SP_DMA_DONE:
            rcp->rsp.finish_dma();
            break;
            
            case EventType::SP_TASK_DONE:
            rcp->rsp.finish_task();
            break;
            
            case EventType::SI_DMA_DONE:
            rcp->si.finish_dma();
            break;
        }
        curr_min = *std::min_element(event_times, event_times + 5);
    }
    
}