#pragma once
#include<cstdint>
#include<limits>
enum class EventType:uint8_t 
{
    VI_DONE=0,
    SP_DMA_DONE=1,
    SP_TASK_DONE=2,
    PI_DMA_DONE=3,
    SI_DMA_DONE=4,
    COUNT
};

//this used to be a priority queue, hence the name
//I might have to turn it back into one in the future
//there wasn't really a reason to redisign it in the first place...
class EventQ
{
public:
    EventQ();
    ~EventQ();
    class RCP* rcp;

    uint64_t curr_min = 1500000;
    uint64_t event_times[5]{
        1500000,
        std::numeric_limits<uint64_t>::max(),
        std::numeric_limits<uint64_t>::max(),
        std::numeric_limits<uint64_t>::max(),
        std::numeric_limits<uint64_t>::max()
    };
    void enqueue(uint64_t time, EventType event_type);
    void process_queue();
};
