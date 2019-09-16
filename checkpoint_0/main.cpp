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
  
  // Read first data file
  std::ifstream f(data_files[0]);
  // Read the file and fill a vector with the numbers
  std::vector<double> v;
  double a;
  while (!f.eof()){
    f >> a;
    v.push_back(a);
  }
  f.close();

  // Compute mean
  double sum=0;
  for (int i=0;i<v.size();i++){
    sum+=v[i];
  }
  const double mean=sum/v.size();

  // Compute standard deviation
  sum=0;
  for (int i=0;i<v.size();i++){
    sum+=v[i]*v[i];
  }
  const double var = sum/v.size() - mean*mean;

  // Compute variance
  const double std = sqrt(var);

  auto t1 = std::chrono::high_resolution_clock::now();
  
  // Compute skewness
  sum=0;
  double one_over_sigma3 = 1./(std*std*std);
  for (int i=0;i<v.size();i++){
    sum+=(v[i]-mean)*(v[i]-mean)*(v[i]-mean)*one_over_sigma3;
  }
  const double skew = sum/v.size();

  // Compute median
  std::vector<double> v2{v};
  bubbleSort(v2);
  int half_way = v2.size()/2;
  const double median = v2[half_way];

  auto t2 = std::chrono::high_resolution_clock::now();
  std::chrono::duration<double, std::milli> ms = t2 - t1;
  std::cout << std::fixed << "sequential took " << ms.count() << " ms\n";

  std::cout << "mean:   " << mean << '\n';
  std::cout << "var:    " << var << '\n';
  std::cout << "skew:   " << skew << '\n';
  std::cout << "median: " << median << '\n';

  // Any of the elements > 50?
  // for (int i=0;i<v.size();i++){
  //   if(v[i]>50){
  //     std::cout << v[i] << " ";}
  // }
  std::cout << std::endl;

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
	  std::cout << "here" << std::endl;
	}
    }
  if(dist){
    std::cout << "Position " << dist << ", first " << first << " second: " << second << '\n';
  }
  else{
    std::cout << "Could not find any valid position" << std::endl;
  }
}
