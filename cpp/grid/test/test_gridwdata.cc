#include "grid.hpp"
#include <iostream>
#include <iomanip>
#include <random>
#include <cassert>
#include <chrono>

using ngpt::data_grid2d;
using ngpt::grid_storage_type;

int main()
{
    std::random_device rd; // obtain a random number from hardware
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dis(0, 100);
    std::chrono::steady_clock::time_point begin;// = std::chrono::steady_clock::now();
    std::chrono::steady_clock::time_point end;// = std::chrono::steady_clock::now();

    data_grid2d<double, double, grid_storage_type::rm_tl> g(-180, 180, 2.5, 90, -90, -5.0);
    std::cout <<"\n\tg(0,1)=" << g.xy_idx2d_idx(0,1);
    std::cout <<"\n\tg(1,0)=" << g.xy_idx2d_idx(1,0);
    std::cout <<"\n\tg(1,1)=" << g.xy_idx2d_idx(1,1);
    std::cout <<"\n\tg(0,0)=" << g.xy_idx2d_idx(0,0);
    begin = std::chrono::steady_clock::now();
    for (int i=0; i<1000; i++) {
        g.xy_idx2d_idx(dis(gen), dis(gen));
    }
    end = std::chrono::steady_clock::now();
    
    std::cout << "Time difference = " << std::chrono::duration_cast<std::chrono::nanoseconds> (end - begin).count();
    std::cout <<"\n\tg(0,1)=" << g.xy_idx2d_idx(std::tuple<std::size_t, std::size_t>{0,1});
    std::cout <<"\n\tg(1,0)=" << g.xy_idx2d_idx(std::tuple<std::size_t, std::size_t>{1,0});
    std::cout <<"\n\tg(1,1)=" << g.xy_idx2d_idx(std::tuple<std::size_t, std::size_t>{1,1});
    std::cout <<"\n\tg(0,0)=" << g.xy_idx2d_idx(std::tuple<std::size_t, std::size_t>{0,0});
    begin = std::chrono::steady_clock::now();
    for (int i=0; i<1000; i++) {
        g.xy_idx2d_idx(std::tuple<std::size_t, std::size_t>{dis(gen), dis(gen)});
    }
    end = std::chrono::steady_clock::now();
    std::cout << "Time difference = " << std::chrono::duration_cast<std::chrono::nanoseconds> (end - begin).count();

    std::cout<<"\n";
    return 0;
}
