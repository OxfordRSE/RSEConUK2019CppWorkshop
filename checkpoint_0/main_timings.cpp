#include <chrono>
#include <cmath>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <iterator>
#include <vector>

int main()
{
  // Declare start and end points for timings with chrono
  std::chrono::high_resolution_clock::time_point t1;
  std::chrono::high_resolution_clock::time_point t2;
  
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// 1. Reading some data in from a file
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    // Find some pre-prepared data files
    std::vector<std::string> data_files;
    data_files.push_back("../data/0_rse_workshop.dat");
    data_files.push_back("../data/1_rse_workshop.dat");
    data_files.push_back("../data/2_rse_workshop.dat");

    // Do something with each of the files...
    for (int i = 0; i < data_files.size(); ++i)
    {
        std::cout << data_files[i] << std::endl;
    }


    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// 2. Calculate the mean and standard deviation of some data
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    // Read some data in
    std::ifstream f(data_files[0]);
    std::vector<double> v{std::istream_iterator<double>(f), std::istream_iterator<double>()};

    // Compute mean
    t1 = std::chrono::high_resolution_clock::now();
    double sum = 0.0;
    for (int i = 0; i < v.size(); i++)
    {
        sum += v[i];
    }
    const double mean = sum / v.size();
    t2 = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> reduce = t2 - t1;
    
    // Compute standard deviation
    t1 = std::chrono::high_resolution_clock::now();
    sum = 0.0;
    for (int i = 0; i < v.size(); i++)
    {
        sum += v[i] * v[i];
    }
    const double var = sum / v.size() - mean * mean;
    t2 = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> transform_reduce = t2 - t1;
    
    // Compute variance
    const double std = std::sqrt(var);

    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// 3. Calculate the skewness - there's no default algorithm for that!
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    // Compute skewness
    sum = 0.0;
    double one_over_sigma3 = 1.0 / (std * std * std);
    for (int i = 0; i < v.size(); i++)
    {
        sum += (v[i] - mean) * (v[i] - mean) * (v[i] - mean) * one_over_sigma3;
    }
    const double skew = sum / v.size();

    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// 4. Calculate the median (and bonus chrono)
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    

    // Take a copy of the vector so we can sort it
    std::vector<double> v2 = v;
    t1 = std::chrono::high_resolution_clock::now();
    std::sort(v2.begin(), v2.end());
    int half_way = v2.size() / 2;
    const double median = v2[half_way];

    t2 = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> nth_element = t2 - t1;
    
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// 5. What other algorithms are there?
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    // Are any of the elements > 50?
    t1 = std::chrono::high_resolution_clock::now();
    bool any_greater_than_50 = false;

    for (int i = 0; i < v.size(); i++)
    {
        if (v[i] > 50.0)
        {
            any_greater_than_50 = true;
        }
    }
    t2 = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> any_of = t2 - t1;

    // First position where consecutive elements differ by more than twice the standard deviation
    t1 = std::chrono::high_resolution_clock::now();
    int dist = 0;
    double first, second;
    for (int i = 0; i < v.size() - 1; i++)
    {
        if (std::fabs(v[i + 1] - v[i]) > 2 * std)
        {
            dist = i;
            first = v[i];
            second = v[i + 1];
            break;
        }
    }
    t2 = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> mismatch = t2 - t1;

    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// 7. Writing some data back out to a csv file - pulling several concepts together
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    std::string output_dir = "./output";
    std::string mkdir_command = "mkdir -p " + output_dir;
    // System takes a const char* as a parameter
    system(mkdir_command.c_str());

    // Create an output file
    t1 = std::chrono::high_resolution_clock::now();
    std::ofstream output_file(output_dir + "/results.dat");
    output_file << v[0];
    for (int i = 1; i < v.size(); i++)
    {
        output_file << ',' << v[i];
    }
    output_file.close();
    t2 = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> writeData = t2 - t1;

    ////////////////////////////////////////////////////////////////////////////////

        ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Writing timings to csv file checkpoint_xx/timings.csv
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    std::ofstream timings("timings.csv", std::ios::app);
    timings << std::fixed << readData.count() << ", "
	    << reduce.count() << ", "
	    << transform_reduce.count() << ", "
	    << nth_element.count() << ", "
	    << any_of.count() << ", "
	    << mismatch.count() << std::endl;
    timings.close();
}
