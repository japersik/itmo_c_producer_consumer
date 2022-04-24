#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest.h>
#include <producer_consumer.h>

// just test debug
TEST_CASE("normal_test") {
  std::stringstream is;
  is << "1 2 3 4 5 6 7 8 9";
  CHECK(run_threads(3, 200, true, is) == 45);
  is.str("");
  is.clear();
}

// 10 threads 0 ms sleep
TEST_CASE("test_10_0") {
  std::stringstream is;
  is << "1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 "
        "1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 "
        "1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 ";
  CHECK(run_threads(10, 0, false, is) == 100);
  is.str("");
  is.clear();
}

// 1 thread
TEST_CASE("test_1_10") {
  std::stringstream is;
  is << "1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 "
        "1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 "
        "1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 ";
  CHECK(run_threads(1, 10, false, is) == 100);
  is.str("");
  is.clear();
}

// 2 threads
TEST_CASE("test_2_30") {
  std::stringstream is;
  is << "1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 "
        "1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 "
        "1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 ";
  CHECK(run_threads(2, 30, false, is) == 100);
  is.str("");
  is.clear();
}

// 200 thread, 100 values
TEST_CASE("test_200_200") {
  std::stringstream is;
  is << "1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 "
        "1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 "
        "1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 ";
  CHECK(run_threads(200, 200, false, is) == 100);
  is.str("");
  is.clear();
}

// 1 thread, 1 ms sleep
TEST_CASE("test_1_1") {
  std::stringstream is;
  is << "1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 "
        "1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 "
        "1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 ";
  CHECK(run_threads(1, 1, false, is) == 100);
  is.str("");
  is.clear();
}

// test from CI/CD testcases
TEST_CASE("from_CICD_test") {
  std::stringstream is;
  is << "7139 26849 8176 19486 18575 24844 1367 19349 11695 31758 7608 21238 "
        "21012 9029 12432 29335 7490 31962 26242 3658 15985 10978 6534 22673 "
        "19395 9802 31152 12718 6777 10550 17217 1513 6114 4313 5919 12624 "
        "5644 2280 14309 21081 24615 11069 12788 4609 13868 14590 17941 18280 "
        "5882 30726 31059 25669 8940 19292 3445 8290 2267 4116 17827 31242 "
        "24614 28885 24157 11481 3950 3778 7437 28073 12009 31366 11535 20062 "
        "9776 10127 18058 14407 17954 3818 24241 26903 27810 14719 29100 397 "
        "22049 19189 9150 9948 29196 6381 30814 29748 5001 9815 12122 28370 "
        "16139 9438 277 11188 19799";
  CHECK(run_threads(5, 1000, false, is) == 1556618);
  is.str("");
  is.clear();
}