#include "privacy.h"
#include <cassert>

bool compare_locations(const std::vector<location> &actual, const std::vector<location> &expected)
{
    for (size_t i = 0; i < actual.size(); ++i)
    {
        if (actual[i].connected_to != expected[i].connected_to)
        {
            std::cerr << "Mismatch in connected_to for location " << actual[i].id
                      << ": expected " << expected[i].connected_to
                      << ", got " << actual[i].connected_to << std::endl;
            return false;
        }
        if (std::abs(actual[i].capacity - expected[i].capacity) > 1e-3)
        {
            std::cerr << "Mismatch in capacity for location " << actual[i].id
                      << ": expected " << expected[i].capacity
                      << ", got " << actual[i].capacity << std::endl;
            return false;
        }
    }
    return true;
}

void test_case_1()
{
    std::vector<location> points = {
        {0, 2.0, 8.0, 1.0, 2.0, 0, 0, 2.0},
        {1, 2.0, 2.0, 1.0, 2.0, 0, 0, 2.0},
        {2, 7.0, 5.0, 1.0, 2.0, 0, 0, 2.0},
        {3, 2.0, 7.0, 1.5, 2.0, 0, 0, 2.0},
        {4, 2.0, 9.0, 1.5, 2.0, 0, 0, 2.0},
        {5, 1.0, 8.0, 1.5, 2.0, 0, 0, 2.0},
        {6, 3.0, 8.0, 1.5, 2.0, 0, 0, 2.0},
        {7, 7.0, 4.0, 1.5, 2.0, 0, 0, 2.0},
        {8, 7.0, 6.0, 1.5, 2.0, 0, 0, 2.0},
        {9, 6.0, 5.0, 1.5, 2.0, 0, 0, 2.0},
        {10, 8.0, 5.0, 1.5, 2.0, 0, 0, 2.0},
        {11, 2.0, 1.0, 1.5, 2.0, 0, 0, 2.0},
        {12, 2.0, 3.0, 1.5, 2.0, 0, 0, 2.0},
        {13, 1.0, 2.0, 1.5, 2.0, 0, 0, 2.0},
        {14, 3.0, 2.0, 1.5, 2.0, 0, 0, 2.0},
    };

    double eps = 1;
    double alpha = 0.1;
    double delta = 1;

    std::vector<location> expected_reconn = {
        {0, 2.0, 8.0, 1.0, 2.0, 35.50809, 0},
        {1, 2.0, 2.0, 1.0, 2.0, 35.50809, 1},
        {2, 7.0, 5.0, 1.0, 2.0, 35.50809, 2},
        {3, 2.0, 7.0, 1.5, 2.0, 0, 0},
        {4, 2.0, 9.0, 1.5, 2.0, 0, 0},
        {5, 1.0, 8.0, 1.5, 2.0, 0, 0},
        {6, 3.0, 8.0, 1.5, 2.0, 0, 0},
        {7, 7.0, 4.0, 1.5, 2.0, 0, 2},
        {8, 7.0, 6.0, 1.5, 2.0, 0, 2},
        {9, 6.0, 5.0, 1.5, 2.0, 0, 2},
        {10, 8.0, 5.0, 1.5, 2.0, 0, 2},
        {11, 2.0, 1.0, 1.5, 2.0, 0, 1},
        {12, 2.0, 3.0, 1.5, 2.0, 0, 1},
        {13, 1.0, 2.0, 1.5, 2.0, 0, 1},
        {14, 3.0, 2.0, 1.5, 2.0, 0, 1},
    };

    std::vector<location> expected_no_reconn = {
        {0, 2.0, 8.0, 1.0, 2.0, 13.40756, 0},
        {1, 2.0, 2.0, 1.0, 2.0, 13.40756, 1},
        {2, 7.0, 5.0, 1.0, 2.0, 13.40756, 2},
        {3, 2.0, 7.0, 1.5, 2.0, 13.40756, 3},
        {4, 2.0, 9.0, 1.5, 2.0, 13.40756, 4},
        {5, 1.0, 8.0, 1.5, 2.0, 13.40756, 5},
        {6, 3.0, 8.0, 1.5, 2.0, 13.40756, 6},
        {7, 7.0, 4.0, 1.5, 2.0, 13.40756, 7},
        {8, 7.0, 6.0, 1.5, 2.0, 13.40756, 8},
        {9, 6.0, 5.0, 1.5, 2.0, 13.40756, 9},
        {10, 8.0, 5.0, 1.5, 2.0, 13.40756, 10},
        {11, 2.0, 1.0, 1.5, 2.0, 13.40756, 11},
        {12, 2.0, 3.0, 1.5, 2.0, 13.40756, 12},
        {13, 1.0, 2.0, 1.5, 2.0, 13.40756, 13},
        {14, 3.0, 2.0, 1.5, 2.0, 13.40756, 14},
    };

    auto reconn_sol = private_reconnection_assignment(points, eps, alpha, delta);
    assert(compare_locations(reconn_sol, expected_reconn) && "Test case 1 failed for reconnected solution!");

    // std::string test_reconn_sol_filename = generate_timestamped_filename("test_out", "reconn_sol", ".out");
    // save_results_to_file(reconn_sol, test_reconn_sol_filename);

    auto no_reconn_sol = private_assignment(points, eps, alpha);
    assert(compare_locations(no_reconn_sol, expected_no_reconn) && "Test case 1 failed for no reconnected solution!");

    // std::string test_no_reconn_sol_filename = generate_timestamped_filename("test_out", "no_reconn_sol", ".out");
    // save_results_to_file(no_reconn_sol, test_no_reconn_sol_filename);

    std::cout << "Test case 1 passed!" << std::endl;
}

void test_case_2()
{
    std::vector<location> points = {
        {0, 2.0, 8.0, 1.0, 2.0, 0, 0, 2.0},
        {1, 2.0, 2.0, 1.0, 2.0, 0, 0, 2.0},
        {2, 7.0, 5.0, 1.0, 2.0, 0, 0, 2.0},
        {3, 2.0, 7.0, 4, 2.0, 0, 0, 2.0},
        {4, 2.0, 9.0, 4, 2.0, 0, 0, 2.0},
        {5, 1.0, 8.0, 4, 2.0, 0, 0, 2.0},
        {6, 3.0, 8.0, 4, 2.0, 0, 0, 2.0},
        {7, 7.0, 4.0, 4, 2.0, 0, 0, 2.0},
        {8, 7.0, 6.0, 4, 2.0, 0, 0, 2.0},
        {9, 6.0, 5.0, 4, 2.0, 0, 0, 2.0},
        {10, 8.0, 5.0, 4, 2.0, 0, 0, 2.0},
        {11, 2.0, 1.0, 4, 2.0, 0, 0, 2.0},
        {12, 2.0, 3.0, 4, 2.0, 0, 0, 2.0},
        {13, 1.0, 2.0, 4, 2.0, 0, 0, 2.0},
        {14, 3.0, 2.0, 4, 2.0, 0, 0, 2.0},
    };

    double eps = 1;
    double alpha = 0.1;
    double delta = 1;

    std::vector<location> expected = {
        {0, 2.0, 8.0, 1.0, 2.0, 35.50809, 0},
        {1, 2.0, 2.0, 1.0, 2.0, 35.50809, 1},
        {2, 7.0, 5.0, 1.0, 2.0, 35.50809, 2},
        {3, 2.0, 7.0, 1.5, 2.0, 0, 0},
        {4, 2.0, 9.0, 1.5, 2.0, 0, 0},
        {5, 1.0, 8.0, 1.5, 2.0, 0, 0},
        {6, 3.0, 8.0, 1.5, 2.0, 0, 0},
        {7, 7.0, 4.0, 1.5, 2.0, 0, 2},
        {8, 7.0, 6.0, 1.5, 2.0, 0, 2},
        {9, 6.0, 5.0, 1.5, 2.0, 0, 2},
        {10, 8.0, 5.0, 1.5, 2.0, 0, 2},
        {11, 2.0, 1.0, 1.5, 2.0, 0, 1},
        {12, 2.0, 3.0, 1.5, 2.0, 0, 1},
        {13, 1.0, 2.0, 1.5, 2.0, 0, 1},
        {14, 3.0, 2.0, 1.5, 2.0, 0, 1},
    };

    auto reconn_sol = private_reconnection_assignment(points, eps, alpha, delta);
    assert(compare_locations(reconn_sol, expected) && "Test case 2 failed for reconnected solution!");

    auto no_reconn_sol = private_assignment(points, eps, alpha);
    assert(compare_locations(no_reconn_sol, expected) && "Test case 2 failed for no reconnected solution!");

    std::cout << "Test case 2 passed!" << std::endl;
}

void test_case_3()
{
    std::vector<location> points = {
        {0, 2.0, 5.0, 1.0, 2.0, 0, 0, 2.0},
        {1, 4.0, 5.0, 1.5, 2.0, 0, 0, 2.0},
        {2, 6.0, 5.0, 1.0, 2.0,0, 0, 2.0},
        {3, 3.0, 5.0, 4, 2.0, 0, 0, 2.0},
        {4, 3.0, 5.0, 4, 2.0, 0, 0, 2.0},
        {5, 3.0, 5.0, 4, 2.0, 0, 0, 2.0},
        {6, 5.0, 5.0, 4, 2.0, 0, 0, 2.0},
        {7, 5.0, 5.0, 4, 2.0, 0, 0, 2.0},
        {8, 5.0, 5.0, 4, 2.0, 0, 0, 2.0},
        {9, 4.0, 4.0, 4, 2.0, 0, 0, 2.0}};

    double eps = 1;
    double alpha = 0.1;
    double delta = 1;

    std::vector<location> expected_reconn = {
        {0, 2.0, 5.0, 1.0, 2.0, 37.95635, 0},
        {1, 4.0, 5.0, 1.5, 2.0, 0, 0},
        {2, 6.0, 5.0, 1.0, 2.0, 29.19327, 2},
        {3, 3.0, 5.0, 4, 2.0, 0, 0},
        {4, 3.0, 5.0, 4, 2.0, 0, 0},
        {5, 3.0, 5.0, 4, 2.0, 0, 0},
        {6, 5.0, 5.0, 4, 2.0, 0, 2},
        {7, 5.0, 5.0, 4, 2.0, 0, 2},
        {8, 5.0, 5.0, 4, 2.0, 0, 2},
        {9, 4.0, 4.0, 4, 2.0, 0, 0}};

    std::vector<location> expected_no_reconn = {
        {0, 2.0, 5.0, 1.0, 2.0, 29.19327, 0},
        {1, 4.0, 5.0, 1.5, 2.0, 18.98590, 1},
        {2, 6.0, 5.0, 1.0, 2.0, 29.19327, 2},
        {3, 3.0, 5.0, 4, 2.0, 0, 0},
        {4, 3.0, 5.0, 4, 2.0, 0, 0},
        {5, 3.0, 5.0, 4, 2.0, 0, 0},
        {6, 5.0, 5.0, 4, 2.0, 0, 2},
        {7, 5.0, 5.0, 4, 2.0, 0, 2},
        {8, 5.0, 5.0, 4, 2.0, 0, 2},
        {9, 4.0, 4.0, 4, 2.0, 0, 1}};

    auto reconn_sol = private_reconnection_assignment(points, eps, alpha, delta);
    assert(compare_locations(reconn_sol, expected_reconn) && "Test case 3 failed for reconnected solution!");

    auto no_reconn_sol = private_assignment(points, eps, alpha);
    assert(compare_locations(no_reconn_sol, expected_no_reconn) && "Test case 3 failed for no reconnected solution!");

    std::cout << "Test case 3 passed!" << std::endl;
}

void test_case_4()
{
    std::vector<location> points = {
        {0, 2.0, 5.0, 1.5, 2.0, 0, 0, 2.0},
        {1, 4.0, 5.0, 1.0, 2.0, 0, 0, 2.0},
        {2, 6.0, 5.0, 1.5, 2.0, 0, 0, 2.0},
        {3, 3.0, 5.0, 4, 2.0, 0, 0, 2.0},
        {4, 3.0, 5.0, 4, 2.0, 0, 0, 2.0},
        {5, 3.0, 5.0, 4, 2.0, 0, 0, 2.0},
        {6, 5.0, 5.0, 4, 2.0, 0, 0, 2.0},
        {7, 5.0, 5.0, 4, 2.0, 0, 0, 2.0},
        {8, 5.0, 5.0, 4, 2.0, 0, 0, 2.0},
        {9, 4.0, 4.0, 4, 2.0, 0, 0, 2.0}};

    double eps = 1;
    double alpha = 0.1;
    double delta = 1;

    std::vector<location> expected_reconn = {
        {0, 2.0, 5.0, 1.5, 2.0, 0, 1},
        {1, 4.0, 5.0, 1.0, 2.0, 53.5095, 1},
        {2, 6.0, 5.0, 1.5, 2.0, 0, 1},
        {3, 3.0, 5.0, 4, 2.0, 0, 1},
        {4, 3.0, 5.0, 4, 2.0, 0, 1},
        {5, 3.0, 5.0, 4, 2.0, 0, 1},
        {6, 5.0, 5.0, 4, 2.0, 0, 1},
        {7, 5.0, 5.0, 4, 2.0, 0, 1},
        {8, 5.0, 5.0, 4, 2.0, 0, 1},
        {9, 4.0, 4.0, 4, 2.0, 0, 1}};

    std::vector<location> expected_no_reconn = {
        {0, 2.0, 5.0, 1.5, 2.0, 12.59663, 0},
        {1, 4.0, 5.0, 1.0, 2.0, 45.97181, 1},
        {2, 6.0, 5.0, 1.5, 2.0, 12.59663, 2},
        {3, 3.0, 5.0, 4, 2.0, 0, 1},
        {4, 3.0, 5.0, 4, 2.0, 0, 1},
        {5, 3.0, 5.0, 4, 2.0, 0, 1},
        {6, 5.0, 5.0, 4, 2.0, 0, 1},
        {7, 5.0, 5.0, 4, 2.0, 0, 1},
        {8, 5.0, 5.0, 4, 2.0, 0, 1},
        {9, 4.0, 4.0, 4, 2.0, 0, 1}};

    auto reconn_sol = private_reconnection_assignment(points, eps, alpha, delta);
    assert(compare_locations(reconn_sol, expected_reconn) && "Test case 4 failed for reconnected solution!");

    auto no_reconn_sol = private_assignment(points, eps, alpha);
    assert(compare_locations(no_reconn_sol, expected_no_reconn) && "Test case 4 failed for no reconnected solution!");

    std::cout << "Test case 4 passed!" << std::endl;
}

void test_reconnect_1()
{
    std::vector<location> points = {
        {0, 1, 3, 1, 1, 0, 0},
        {1, 2, 1, 1.5, 1, 0, 1},
        {2, 3, 3, 1.5, 1, 0, 2}};

    double delta = 2;
    std::unordered_map<int, std::unordered_set<int>> reverse_assignment = {
        {0, {0}},
        {1, {1}},
        {2, {2}}};

    std::vector<location> expected = {
        {0, 1, 3, 1, 1, 0, 0},
        {1, 2, 1, 1.5, 1, 0, 0},
        {2, 3, 3, 1.5, 1, 0, 0}};

    auto reconnect_result = reconnect(points, delta, reverse_assignment);
    assert(compare_locations(reconnect_result, expected) && "Reconnect Test 1 failed!");

    std::cout << "Reconnect Test 1 passed!" << std::endl;
}

void test_reconnect_2()
{
    std::vector<location> points = {
        {0, 1, 3, 1, 1, 0, 0},
        {1, 2, 1, 1.5, 1, 0, 1},
        {2, 3, 3, 1.5, 1, 0, 2}};

    double delta = 1;
    std::unordered_map<int, std::unordered_set<int>> reverse_assignment = {
        {0, {0}},
        {1, {1}},
        {2, {2}}};

    std::vector<location> expected = {
        {0, 1, 3, 1, 1, 0, 0},
        {1, 2, 1, 1.5, 1, 0, 1},
        {2, 3, 3, 1.5, 1, 0, 0}};

    auto reconnect_result = reconnect(points, delta, reverse_assignment);
    assert(compare_locations(reconnect_result, expected) && "Reconnect Test 2 failed!");

    std::cout << "Reconnect Test 2 passed!" << std::endl;
}

void test_reconnect_3()
{
    std::vector<location> points = {
        {0, 1, 2, 1, 1, 0, 0},
        {1, 2, 1, 1.1, 1, 0, 1},
        {2, 4, 3, 0.8, 1, 0, 2}};

    double delta = 1;
    std::unordered_map<int, std::unordered_set<int>> reverse_assignment = {
        {0, {0}},
        {1, {1}},
        {2, {2}}};

    std::vector<location> expected = {
        {0, 1, 2, 1, 1, 0, 0},
        {1, 2, 1, 1.1, 1, 0, 0},
        {2, 4, 3, 0.8, 1, 0, 2}};

    auto reconnect_result = reconnect(points, delta, reverse_assignment);
    assert(compare_locations(reconnect_result, expected) && "Reconnect Test 3 failed!");

    std::cout << "Reconnect Test 3 passed!" << std::endl;
}

int main()
{
    test_case_1();
    test_case_2();
    test_case_3();
    test_case_4();

    test_reconnect_1();
    test_reconnect_2();
    test_reconnect_3();

    return 0;
}