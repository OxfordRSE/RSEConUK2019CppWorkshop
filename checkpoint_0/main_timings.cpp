#include <iostream>
#include <cstdlib>
#include <fstream>
#include <vector>
#include <cmath>
#include <chrono>

// An optimized version of Bubble Sort 
void bubbleSort(std::vector<double> &v) 
{ 
   int i, j;
   double temp;
   bool swapped; 
   for (i = 0; i < v.size()-1; i++) 
   { 
     swapped = false; 
     for (j = 0; j < v.size()-i-1; j++) 
     { 
        if (v[j] > v[j+1]) 
        {
	  temp = v[j];
	  v[j]=v[j+1];
	  v[j+1]=temp;
           swapped = true; 
        } 
     } 
  
     // IF no two elements were swapped by inner loop, then break 
     if (swapped == false) 
        break; 
   } 
}

int main()
{
  
  // Create a new directory under the build directory
  std::string output_dir = "./output";
  std::string mkdir_command = "mkdir "+output_dir;
  // System takes a const char* as a parameter
  system(mkdir_command.c_str());

  // Create an output file
  std::ofstream output_file(output_dir+"/results.dat");
  output_file << "blahhhhhhh";

  std::string data_files[3];
  data_files[0] = "../data/0_rse_workshop.dat";
  data_files[1] = "../data/1_rse_workshop.dat";
  data_files[2] = "../data/2_rse_workshop.dat";

  std::chrono::high_resolution_clock::time_point t1;
  std::chrono::high_resolution_clock::time_point t2;
  
  // Read first data file
  t1 = std::chrono::high_resolution_clock::now();
  std::ifstream f(data_files[0]);
  // Read the file and fill a vector with the numbers
  std::vector<double> v;
  double a;
  while (!f.eof()){
    f >> a;
    v.push_back(a);
  }
  f.close();
  t2 = std::chrono::high_resolution_clock::now();
  std::chrono::duration<double, std::milli> readData = t2 - t1;
  
  // Compute mean
  t1 = std::chrono::high_resolution_clock::now();
  double sum=0;
  for (int i=0;i<v.size();i++){
    sum+=v[i];
  }
  const double mean=sum/v.size();
  t2 = std::chrono::high_resolution_clock::now();
  std::chrono::duration<double, std::milli> reduce = t2 - t1;
  
  // Compute standard deviation
  t1 = std::chrono::high_resolution_clock::now();
  sum=0;
  for (int i=0;i<v.size();i++){
    sum+=v[i]*v[i];
  }
  const double var = sum/v.size() - mean*mean;
  t2 = std::chrono::high_resolution_clock::now();
  std::chrono::duration<double, std::milli> transform_reduce = t2 - t1;
  
  // Compute variance
  const double std = sqrt(var);

  auto tt1 = std::chrono::high_resolution_clock::now();
  
  // Compute skewness
  sum=0;
  double one_over_sigma3 = 1./(std*std*std);
  for (int i=0;i<v.size();i++){
    sum+=(v[i]-mean)*(v[i]-mean)*(v[i]-mean)*one_over_sigma3;
  }
  const double skew = sum/v.size();
  
  // Compute median
  t1 = std::chrono::high_resolution_clock::now();
  std::vector<double> v2{v};
  bubbleSort(v2);
  int half_way = v2.size()/2;
  const double median = v2[half_way];
  t2 = std::chrono::high_resolution_clock::now();
  std::chrono::duration<double, std::milli> nth_element = t2 - t1;

  auto tt2 = std::chrono::high_resolution_clock::now();
  std::chrono::duration<double, std::milli> ms = tt2 - tt1;
  std::cout << std::fixed << "sequential took " << ms.count() << " ms\n";

  std::cout << "mean:   " << mean << '\n';
  std::cout << "var:    " << var << '\n';
  std::cout << "skew:   " << skew << '\n';
  std::cout << "median: " << median << '\n';

  // Any of the elements > 50?
  t1 = std::chrono::high_resolution_clock::now();
  // for (int i=0;i<v.size();i++){
  //   if(v[i]>50){
  //     std::cout << v[i] << " ";}
  // }
  std::cout << std::endl;
  t2 = std::chrono::high_resolution_clock::now();
  std::chrono::duration<double, std::milli> any_of = t2 - t1;

  t1 = std::chrono::high_resolution_clock::now();
  int dist=0;
  double first, second;
  for (int i=0;i<v.size()-1;i++)
    {
      if(abs(v[i+1]-v[i])>2*std)
	{
	  dist = i;
	  first = v[i];
	  second = v[i+1];
	  break;
	}
    }
  t2 = std::chrono::high_resolution_clock::now();
  std::chrono::duration<double, std::milli> mismatch = t2 - t1;
  if(dist){
    std::cout << "Position " << dist << ", first " << first << " second: " << second << '\n';
  }
  else{
    std::cout << "Could not find any valid position" << std::endl;
  }

  std::ofstream timings("timings.csv", std::ios::app);
  timings << std::fixed << readData.count() << ", "
	  << reduce.count() << ", "
	  << transform_reduce.count() << ", "
	  << nth_element.count() << ", "
	  << any_of.count() << ", "
	  << mismatch.count() << std::endl;
  timings.close();
  
}

