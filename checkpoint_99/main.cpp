#include <chrono>
#include <cstdlib>
#include <execution>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <numeric>
#include <random>
#include <vector>

namespace fs = std::filesystem;

int main()
{
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// 1. Reading some data in from a file
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    // Find all files like *_rse_workshop.dat under a specific directory
    std::vector<fs::path> data_files;
    for (auto &p : fs::recursive_directory_iterator(std::getenv("HOME")))
    {
        if (p.path().string().ends_with("_rse_workshop.dat"))
        {
            data_files.emplace_back(p.path());
        }
    }

    // Which data files did we find? (range-for loop)
    for (const auto &d : data_files)
    {
        std::cout << d << std::endl;
    }

    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// 2. Calculate the mean and standard deviation of some data
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    // Read some data in
    std::ifstream f(data_files[0]);
    std::vector<double> v{std::istream_iterator<double>(f), std::istream_iterator<double>()};

    const double mean = std::reduce(v.begin(), v.end(), 0.0) / v.size();
    const double var = std::transform_reduce(v.begin(), v.end(), v.begin(), 0.0) / v.size() - mean * mean;
    const double std = std::sqrt(var);

    // Lambdas

    auto t1 = std::chrono::high_resolution_clock::now();

    auto third_moment = [mean, std](const double x) { return std::pow((x - mean) / std, 3); };
    const double skew = std::transform_reduce(v.begin(), v.end(), 0.0, std::plus<>(), third_moment) / v.size();

    std::vector<double> v2{v};
    auto half_way = v2.size() / 2;

    std::nth_element(v2.begin(), std::next(v2.begin(), half_way), v2.end());
    const double median = v2.at(half_way);

    auto t2 = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> ms = t2 - t1;
    std::cout << std::fixed << "sequential took " << ms.count() << " ms\n";

    std::cout << "mean:   " << mean << '\n';
    std::cout << "var:    " << var << '\n';
    std::cout << "skew:   " << skew << '\n';
    std::cout << "median: " << median << '\n';

    // Any of the elements > 50?
    const bool any_greater_than_50 = std::any_of(v.begin(), v.end(), [](const double x) { return x > 50.0; });
    std::cout << std::boolalpha << "Any greater than 50? " << any_greater_than_50 << '\n';

    // First position where consecutive elements differ by more than twice the standard deviation
    auto threshold = [std](const double x, const double y) { return std::fabs(x - y) < 2.0 * std; };
    const auto answer = std::mismatch(v.begin(), std::prev(v.end()), std::next(v.begin()), threshold);

    const auto dist = std::distance(v.begin(), answer.first);
    std::cout << "Position " << dist << ", first " << *answer.first << " second: " << *answer.second << '\n';


    // Create a new directory under the build directory
    auto build_dir = fs::canonical(fs::path{"."});
    auto output_dir = build_dir / "output";
    fs::create_directories(output_dir);

    // Create an output file
    auto output_file = output_dir / "results.dat";
    std::ofstream(output_file) << "blahhhhhhh";

    return 0;
}