#include <iostream>
#include <fstream>
#include <sstream>
#include <unordered_map>
#include <limits>

struct Stats {
    double minVal, maxVal, sum;
    uint64_t count;
    Stats()
        : minVal(std::numeric_limits<double>::infinity()),
          maxVal(-std::numeric_limits<double>::infinity()),
          sum(0.0), count(0) {}
    void update(double val) {
        if (val < minVal) minVal = val;
        if (val > maxVal) maxVal = val;
        sum += val;
        count++;
    }
};

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <input_file>" << std::endl;
        return 1;
    }

    std::ifstream fin(argv[1]);
    if (!fin.is_open()) {
        std::cerr << "Error opening file.\n";
        return 1;
    }

    std::unordered_map<std::string, Stats> map;
    map.reserve(1'000'000); 

    std::string line;
    size_t lineCount = 0;
    while (std::getline(fin, line)) {
        auto sepPos = line.find(';');
        if (sepPos == std::string::npos) continue;

        std::string place = line.substr(0, sepPos);
        double measurement = std::stod(line.substr(sepPos + 1));

        auto& stats = map[place];
        stats.update(measurement);
        lineCount++;
    }

    std::cout.precision(6);
    std::cout << std::fixed;
    for (const auto& kv : map) {
        const auto& place = kv.first;
        const auto& stats = kv.second;
        double avg = (stats.count > 0) ? (stats.sum / stats.count) : 0.0;
        std::cout << "Place: " << place << "\n"
                  << "Min: " << stats.minVal << "\n"
                  << "Max: " << stats.maxVal << "\n"
                  << "Average: " << avg << "\n";
    }

    std::cerr << "Processed " << lineCount << " lines.\n";
    return 0;
}
