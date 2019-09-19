#include <algorithm>
#include <chrono>
#include <cmath>
#include <cstdlib>
#include <execution>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <iterator>
#include <numeric>
#include <vector>

namespace fs = std::filesystem;

int main()
{
  // Declare start and end points for timings with chrono
  std::chrono::high_resolution_clock::time_point t1;
  std::chrono::high_resolution_clock::time_point t2;
  
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

    // This is just to make sure the same file is first, so the numbers later are the same on each version.
    // The sort isn't necessary!
    std::sort(data_files.begin(), data_files.end());

    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// 2. Calculate the mean and standard deviation of some data
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    // Read some data in
    std::ifstream f(data_files[0]);
    std::vector<double> v{std::istream_iterator<double>(f), std::istream_iterator<double>()};
    
    t1 = std::chrono::high_resolution_clock::now();
    const double mean = std::reduce(std::execution::par, v.begin(), v.end(), 0.0) / v.size();
    t2 = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> reduce = t2 - t1;

    t1 = std::chrono::high_resolution_clock::now();
    const double var =
        std::transform_reduce(std::execution::par_unseq, v.begin(), v.end(), v.begin(), 0.0) / v.size() - mean * mean;
    t2 = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> transform_reduce = t2 - t1;
    
    const double std = std::sqrt(var);

    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// 3. Calculate the skewness - there's no default algorithm for that!
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    // Compute skewness
    auto third_moment = [mean, std](const double x) { return std::pow((x - mean) / std, 3); };
    const double skew =
        std::transform_reduce(std::execution::par, v.begin(), v.end(), 0.0, std::plus<>(), third_moment) / v.size();

    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// 4. Calculate the median (and bonus chrono)
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    // Take a copy of the vector so we can sort it
    std::vector<double> v2 = v;

    t1 = std::chrono::high_resolution_clock::now();
    auto half_way = v2.size() / 2;
    std::nth_element(std::execution::par, v2.begin(), std::next(v2.begin(), half_way), v2.end());
    const double median = v2.at(half_way);
    t2 = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> nth_element = t2 - t1;

    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// 5. What other algorithms are there?
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    // Any of the elements > 50?
    t1 = std::chrono::high_resolution_clock::now();
    const bool any_greater_than_50 =
        std::any_of(std::execution::unseq, v.begin(), v.end(), [](const double x) { return x > 50.0; });
    t2 = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> any_of = t2 - t1;

    // First position where consecutive elements differ by more than twice the standard deviation
    t1 = std::chrono::high_resolution_clock::now();
    auto comparison = [std](const double x, const double y) { return std::fabs(x - y) < 2.0 * std; };
    const auto answer = std::mismatch(v.begin(), std::prev(v.end()), std::next(v.begin()), comparison);

    const auto dist = std::distance(v.begin(), answer.first);
    t2 = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> mismatch = t2 - t1;

    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// 7. Writing some data back out to a csv file - pulling several concepts together
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    // Create a new directory under the build directory
    auto build_dir = fs::canonical(fs::path{"."});
    auto output_dir = build_dir / "output";
    fs::create_directories(output_dir);

    // Create an output file
    t1 = std::chrono::high_resolution_clock::now();
    std::ofstream output_file(output_dir / "results.dat");
    output_file << std::accumulate(v.begin(), v.end(), std::to_string(v[0]),
        [](std::string a, double b) { return std::move(a) + ',' + std::to_string(b); });
    output_file.close();
    t2 = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> writeData = t2 - t1;

        ////////////////////////////////////////////////////////////////////////////////

        ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Writing timings to csv file checkpoint_xx/timings.csv
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    std::ofstream timings("timings_7.csv", std::ios::app);
    timings << std::fixed << writeData.count() << ", "
	    << reduce.count() << ", "
	    << transform_reduce.count() << ", "
	    << nth_element.count() << ", "
	    << any_of.count() << ", "
	    << mismatch.count() << std::endl;
    timings.close();
}
