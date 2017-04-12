#include "grid.hpp"
#ifdef DEBUG
#include <iostream>
#include <iomanip>
#include <random>
#include <cassert>
#endif

using ngpt::tick_axis;

int main()
{
    std::random_device rd; // obtain a random number from hardware
    std::mt19937 eng(rd()); // seed the generator
    std::uniform_real_distribution<double> distr; // define the distribution
    std::cout << std::fixed << std::setfill('0') << std::setprecision(3);

    // construct a vector of tick axis to be tested
    std::vector<tick_axis<double>> vecta;
    vecta.emplace_back(-90, 90, 2.5);
    vecta.emplace_back(-90, 90, -2.5);
    vecta.emplace_back(180, -180, 5.0);
    vecta.emplace_back(180, -180, -5.0);

    for (auto& i : vecta) {
        std::cout<<"\nChecking tick-axis from "<<i.start()<<" to "<<i.stop()
            <<" with step "<<i.step();
        if (i.validate()) {
            std::cout<<"\n\tNumber of points (ticks) is: "<<i.num_pts();
            // basic checking of indexes
            assert( i(0) == i.start() );
            assert( i(i.num_pts()-1) == i.stop() );
            // check random numbers within the valid axis range
            std::uniform_real_distribution<double>::param_type range {i.start(), i.stop()};
            distr.param(range);
            for (int j=0; j<10; ++j) {
                // a random number
                auto rand = distr(eng);
                // check the nearest left tick & value
                std::cout<<"\n\tNearest left tick index from "<<rand<<" is "
                    <<i.index(rand)<<" with value "<<i.operator()(i.index(rand));
                assert( std::abs(i.operator()(i.index(rand))-rand)
                    <std::abs(i.step()) );
                // check the nearest neighbor
                std::cout<<"\n\tNearest tick index from value "<<rand<<" is "
                    <<i.nearest_neighbor(rand)<<" with value "<<i.operator()(i.nearest_neighbor(rand));
                assert( std::abs(i.operator()(i.nearest_neighbor(rand))-rand)
                    <std::abs(i.step()/2) );
            }
            // check limits
            std::vector<double> axis_limits;
            axis_limits.push_back(i.start());
            axis_limits.push_back(i.stop());
            for (auto ll : axis_limits) {
                std::cout<<"\n\tNearest left tick index from "<<ll<<" is "
                    <<i.index(ll)<<" with value "<<i.operator()(i.index(ll));
                std::cout<<"\n\tNearest tick index from value "<<ll<<" is "
                    <<i.nearest_neighbor(ll)<<" with value "<<i.operator()(i.nearest_neighbor(ll));
            }
            // check outside limits
            if (i.is_ascending()) {
                axis_limits[0] -= i.step();
                axis_limits[1] += i.step();
            } else {
                axis_limits[0] += i.step()*-1.0;
                axis_limits[1] -= i.step()*-1.0;
            }
            for (auto ll : axis_limits) {
                std::cout<<"\n\tNearest left tick index from "<<ll<<" is "
                    <<i.index(ll)<<" with value "<<i.operator()(i.index(ll));
                std::cout<<"\n\tNearest tick index from value "<<ll<<" is "
                    <<i.nearest_neighbor(ll)<<" with value "<<i.operator()(i.nearest_neighbor(ll));
            }
        } else {
            std::cout<<"\n\tTick-axis is **NOT** valid!";
        }
    }
    
    std::cout<<"\n";
    return 0;
}
