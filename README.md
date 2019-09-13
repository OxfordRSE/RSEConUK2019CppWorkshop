# Getting the most out of the modern C++ language and standard libraries
## RSECon 2019

## Instructions for testing

If possible, could these be tested on a new user account, via SSH and remote desktop?
I've been having problems getting system-wide environment variables working under remote desktop (but fine for ssh). 

- Check CMake version:
  ```
  cmake --version
  ```
  Should be `3.15.3`
- Check CXX environment variable:
  ```
  echo $CXX
  ```
  Should be `/usr/gcc-trunk/bin/g++`
- Copy data: `cp -r /usr/data/RSEConUK2019CppWorkshop/ ~`
- `cd ~/RSEConUK2019CppWorkshop`
- `mkdir build && cd build`
- Ensure CMake configures correctly
  ```
  cmake ..
  ```
  Should see `The CXX compiler identification is GNU 10.0.0`
- Ensure it compiles with no errors correctly
  ```
  make checkpoint_99
  ```
- Ensure it runs with no errors
  ```
  ./checkpoint_99
  ```
  Last line should be `Position 17, first 27.378900 second: 45.188000`

## Check other software

- Check that CLion is installed and opens
- Check that VS Code is installed and opens
- Check that emacs is installed and opens