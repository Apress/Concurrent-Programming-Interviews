# Test Build
g++ -std=c++20 -o test_build -DTEST_ENV 7-4.cc

# Prod Build
g++ -std=c++20 -o prod_build 7-4.cc
