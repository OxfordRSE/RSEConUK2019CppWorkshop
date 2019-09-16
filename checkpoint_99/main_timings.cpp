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

void generate_data()
{
    int num_to_generate = 3;

    std::random_device rd;
    std::mt19937 gen{rd()};

    std::uniform_real_distribution real_dist_mean(10.0, 60.0);
    std::uniform_real_distribution real_dist_std(3.0, 8.0);

    for (int i = 0; i < num_to_generate; ++i)
    {
        std::normal_distribution<double> norm(real_dist_mean(gen), real_dist_std(gen));

        std::ofstream f("/home/fergus/GitRepos/OxfordRSE/OxfordRSEProjects/RSEConUK2019CppWorkshop/data/" +
                        std::to_string(i) + "_rse_workshop.dat");

        f << norm(gen);
        for (int j = 0; j < 1000; ++j)
        {
            f << ' ' << norm(gen);
        }
        f.close();
    }
}

int main()
{

    // Create a new directory under the build directory
    auto build_dir = fs::canonical(fs::path{"."});
    auto output_dir = build_dir / "output";
    fs::create_directories(output_dir);

    // Create an output file
    auto output_file = output_dir / "results.dat";
    std::ofstream(output_file) << "blahhhhhhh";

    //  generate_data();

    // Find all files like *_rse_workshop.dat under a specific directory
    std::vector<fs::path> data_files;
    for (auto &p : fs::recursive_directory_iterator(std::getenv("HOME")))
    {
        if (p.path().string().ends_with("_rse_workshop.dat"))
        {
            data_files.emplace_back(p.path());
        }
    }

    std::sort(data_files.begin(), data_files.end());

    // Which data files did we find? (range-for loop)
    for (auto &d : data_files)
    {
        std::cout << d << '\n';
    }
    
    auto t1 = std::chrono::high_resolution_clock::now();
    std::ifstream f(data_files[0]);
    std::vector<double> v{std::istream_iterator<double>(f), std::istream_iterator<double>()};
    auto t2 = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> readData = t2 - t1;

    auto t1 = std::chrono::high_resolution_clock::now();
    const double mean = std::reduce(v.begin(), v.end(), 0.0) / v.size();
    auto t2 = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> reduce = t2 - t1;

    auto t1 = std::chrono::high_resolution_clock::now();
    const double var = std::transform_reduce(v.begin(), v.end(), v.begin(), 0.0) / v.size() - mean * mean;
    auto t2 = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> transform_reduce = t2 - t1;
    const double std = std::sqrt(var);

    // Lambdas

    auto tt1 = std::chrono::high_resolution_clock::now();

    auto third_moment = [mean, std](const double x) { return std::pow((x - mean) / std, 3); };
    const double skew = std::transform_reduce(v.begin(), v.end(), 0.0, std::plus<>(), third_moment) / v.size();

    std::vector<double> v2{v};
    auto half_way = v2.size() / 2;

    auto t1 = std::chrono::high_resolution_clock::now();
    std::nth_element(v2.begin(), std::next(v2.begin(), half_way), v2.end());
    const double median = v2.at(half_way);
    auto t2 = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> nth_element = t2 - t1;
    
    auto tt2 = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> ms = tt2 - tt1;
    std::cout << std::fixed << "sequential took " << ms.count() << " ms\n";

    std::cout << "mean:   " << mean << '\n';
    std::cout << "var:    " << var << '\n';
    std::cout << "skew:   " << skew << '\n';
    std::cout << "median: " << median << '\n';

    // Any of the elements > 50?
    auto t1 = std::chrono::high_resolution_clock::now();
    std::cout << std::any_of(v.begin(), v.end(), [](const double x) { return x > 50.0; }) << '\n';
    auto t2 = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> any_of = t2 - t1;
    
    // First position where consecutive elements differ by more than twice the standard deviation
    auto t1 = std::chrono::high_resolution_clock::now();
    auto threshold = [std](const double x, const double y) { return std::fabs(x - y) < 2.0 * std; };
    auto answer = std::mismatch(v.begin(), std::prev(v.end()), std::next(v.begin()), threshold);
    auto dist = std::distance(v.begin(), answer.first);
    auto t2 = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> mismatch = t2 - t1;
    
    std::cout << "Position " << dist << ", first " << *answer.first << " second: " << *answer.second << '\n';


    std::ofstream timings("timings.csv", ios::app);
    timings << std::fixed << readData.count() << ", "
	    << reduce.count() << ", "
	    << transform_reduce.count() << ", "
	    << nth_element.count() << ", "
	    << any_of.count() << ", "
	    << mismatch.count() << ", "
	    << std::endl;
    return 0;
}
