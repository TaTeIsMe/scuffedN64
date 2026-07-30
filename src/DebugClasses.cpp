#include "DebugClasses.h"

void InstructionTraceBuffer::record(uint32_t pc, uint32_t opcode, uint64_t rs_val, uint64_t rt_val)
{
    if (history.size() >= MAX_HISTORY)
        history.pop_front();

    history.push_back({
        pc,
        opcode,
        rs_val,
        rt_val
    });
}

void InstructionTraceBuffer::clear()
{
    history.clear();
}

void InstructionTraceBuffer::dump_to_file(const std::string &filename, const VR4300Interpreter &vr4300) const
{
    std::ofstream out(filename);

    if (!out.is_open())
    {
        std::cerr << "Failed to open " << filename << '\n';
        return;
    }

    for (const auto &inst : history)
    {
        out << symbol_table.resolve(inst.pc)
            << " PC: 0x"
            << std::hex << std::uppercase << std::setw(8) << std::setfill('0')
            << inst.pc

            << " rs:" << std::dec << vr4300.rs(inst.opcode)
            << " rt:" << vr4300.rt(inst.opcode)
            << " rd:" << vr4300.rd(inst.opcode)

            << " rs_val:0x"
            << std::hex << inst.rs_val

            << " rt_val:0x"
            << inst.rt_val

            << " imm:0x"
            << vr4300.imm(inst.opcode)

            << " "
            << vr4300.decode_op_name(inst.opcode)

            << '\n';
    }

    std::cout << "[InstructionTrace] Dumped "
              << history.size()
              << " instructions.\n";
}

inline uint32_t SymbolTable::normalize_addr(uint32_t addr)
{
    return addr & 0x1FFFFFFF;
}

SymbolTable::SymbolTable()
{
    load_map_file("oot-gc-eu-dbg.map");
}

void SymbolTable::set_history_size(size_t size)
{
    max_history_size = size;
}

void SymbolTable::add_symbol(uint32_t address, uint32_t size, const std::string &name)
{
    symbols[normalize_addr(address)] = { normalize_addr(address), size, name };
}

bool SymbolTable::load_map_file(const std::string &filename)
{
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "[SymbolTable] Failed to open map file: " << filename << "\n";
        return false;
    }

    std::string line;
    std::regex symbol_regex(R"(^\s+0x(?:00000000)?([0-9a-fA-F]{8})\s+([A-Za-z0-9_]+)$)");
    
    bool in_text_section = false;
    uint32_t loaded_count = 0;

    while (std::getline(file, line)) {
        if (line.find(".text") != std::string::npos) {
            in_text_section = true;
        } else if (line.find(".rodata") != std::string::npos || 
                    line.find(".data") != std::string::npos || 
                    line.find(".bss") != std::string::npos) {
            in_text_section = false;
        }

        if (!in_text_section) continue;

        std::smatch match;
        if (std::regex_match(line, match, symbol_regex)) {
            uint32_t addr = std::stoul(match[1].str(), nullptr, 16);
            std::string name = match[2].str();

            if (name.rfind("_", 0) == 0 || name.rfind("..", 0) == 0) continue;

            add_symbol(addr, 0, name);
            loaded_count++;
        }
    }

    std::cout << "[SymbolTable] Loaded " << loaded_count << " code symbols from " << filename << "\n";
    return true;
}

std::string SymbolTable::resolve(uint32_t pc) const
{
    if (symbols.empty()) return "Unknown";

    uint32_t norm_pc = normalize_addr(pc);
    auto it = symbols.upper_bound(norm_pc);
    
    if (it == symbols.begin()) return "Unknown";

    --it;
    const Symbol& sym = it->second;

    if (sym.size > 0 && norm_pc >= (sym.address + sym.size)) {
        return "Unknown";
    }

    uint32_t offset = norm_pc - sym.address;
    if (offset == 0) return sym.name;

    std::stringstream ss;
    ss << sym.name << " + 0x" << std::hex << offset;
    return ss.str();
}


void SymbolTable::record_call(uint32_t target_pc)
{
    std::string sym = resolve(target_pc);

    if (!history.empty()) {
        std::string last_sym = history.back().symbol;
        
        auto get_base_name = [](const std::string& name) {
            size_t pos = name.find(" + 0x");
            if (pos != std::string::npos) {
                return name.substr(0, pos);
            }
            return name;
        };

        if (get_base_name(sym) == get_base_name(last_sym)) {
            return;
        }
    }

    if (history.size() >= max_history_size) {
        history.pop_front();
    }

    history.push_back({ target_pc, sym });
}

void SymbolTable::print_trace_history(size_t limit, std::ostream &os) const
{
    if (history.empty()) {
        os << "\n[SymbolTable] Call history is empty.\n";
        return;
    }

    size_t total_entries = history.size();
    size_t entries_to_print = (limit == 0 || limit > total_entries) ? total_entries : limit;
    size_t start_index = total_entries - entries_to_print;

    os << "\n================================ CALL TRACE HISTORY ================================\n";
    os << " Showing last " << std::dec << entries_to_print << " recorded call(s):\n";
    os << "------------------------------------------------------------------------------------\n";

    for (size_t i = start_index; i < total_entries; ++i) {
        const auto& entry = history[i];
        size_t step_num = i - start_index + 1;

        os << " #" << std::dec << std::setw(3) << std::setfill('0') << step_num << " "
            << "[0x" << std::hex << std::uppercase << std::setw(8) << std::setfill('0') << entry.pc << "] "
            << entry.symbol << "\n";
    }

    os << "====================================================================================\n\n";
}

void SymbolTable::print_trace() const
{
    print_trace_history();
}