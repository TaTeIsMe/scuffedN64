//AI slop for logging instructions.
#include <iostream>
#include <map>
#include <string>
#include <cstdint>
#include <sstream>

#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <map>
#include <deque>
#include <string>
#include <cstdint>
#include <regex>
#include "VR4300Interpreter.h"

struct Symbol {
    uint32_t address;
    uint32_t size;
    std::string name;
};

struct CallEntry {
    uint32_t pc;
    std::string symbol;
};

class SymbolTable {
private:
    std::map<uint32_t, Symbol> symbols;
    
    std::deque<CallEntry> history;
    size_t max_history_size = 999;

    static inline uint32_t normalize_addr(uint32_t addr);

public:
    SymbolTable();

    void set_history_size(size_t size);

    void add_symbol(uint32_t address, uint32_t size, const std::string& name);

    bool load_map_file(const std::string& filename);

    std::string resolve(uint32_t pc) const;
    void record_call(uint32_t target_pc);
    void print_trace_history(size_t limit = 0, std::ostream& os = std::cout) const;

    void print_trace() const;
};

inline SymbolTable symbol_table;

#include <deque>
#include <fstream>
#include <sstream>

struct InstructionTraceEntry
{
    uint32_t pc;
    uint32_t opcode;
    uint64_t rs_val;
    uint64_t rt_val;
};

class InstructionTraceBuffer
{
public:
    static constexpr size_t MAX_HISTORY = 5000000;

    void record(uint32_t pc,
                uint32_t opcode,
                uint64_t rs_val,
                uint64_t rt_val);

    void clear();

    void dump_to_file(const std::string& filename, const VR4300Interpreter& vr4300) const;
  
    std::deque<InstructionTraceEntry> history;
};

inline InstructionTraceBuffer instruction_trace;
