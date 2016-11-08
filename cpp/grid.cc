#ifndef __NGPT_GRID_HPP__
#define __NGPT_GRID_HPP__

#include <cstddef>
#include <utility>
#ifdef RANGE_CHECK
#include <cassert>
#include <limits>
#endif
#ifdef DEBUG
#include <iostream>
#include <iomanip>
#include <random>
#endif

namespace ngpt
{

/// The tick_axis is inlusive! Given e.g. the range [90, -90], both 90 and -90
/// will be valid nodes.
template<class T>
    class tick_axis
{
public:
    explicit
    tick_axis(T start, T stop, T step = T{1}) noexcept
    : _start{start},
      _stop{stop},
      _step{step}
    {}

    bool
    is_ascending() const noexcept
    { return _stop > _start; }

    std::size_t
    num_pts() const noexcept
    { return static_cast<std::size_t>((_stop - _start) / _step) + 1; }

    // left
    std::size_t
    index(T val) const noexcept
    {
#ifdef RANGE_CHECK
        std::size_t idx = static_cast<std::size_t>((val-_start)/_step);
        assert( idx < std::numeric_limits<int>::max() && idx < this->num_pts() );
#endif
        return static_cast<std::size_t>((val-_start)/_step);
    }

    std::size_t
    nearest_neighbor(T val) const noexcept
    {
        std::size_t idx { this->index(val) };
        return  std::abs(val-this->val_at_index(idx)) > std::abs(_step)/2
                ? ++idx
                : idx;
    }

    T
    max_val() const noexcept
    { return this->is_ascending() ? _stop : _start; }

    T
    min_val() const noexcept
    { return this->is_ascending() ? _start : _stop; }

    T
    start() const noexcept { return _start; }

    T
    stop() const noexcept { return _stop; }

    T
    step() const noexcept { return _step; }

    T
    val_at_index(std::size_t idx) const noexcept
    { return _start + idx*_step; }

private:
    T _start,
      _stop,
      _step;
};

template<typename T>
    class two_dim_grid
{
public:
    using grid_node  = std::pair<std::size_t, std::size_t>;
    using grid_value = std::pair<T, T>;

    explicit
    two_dim_grid(T xstart, T xstop, T xstep, T ystart, T ystop, T ystep)
    noexcept
    : _xaxis{xstart, xstop, xstep},
      _yaxis{ystart, ystop, ystep}
    {}

    grid_value
    val_at_index(const grid_node& node) const noexcept
    {
        return std::pair<std::size_t, std::size_t>(_xaxis.val_at_index(node->first),
            _yaxis.val_at_index(node->second));
    }

private:
    tick_axis<T> _xaxis, _yaxis;
};

}

using ngpt::tick_axis;

int main()
{
    std::random_device rd; // obtain a random number from hardware
    std::mt19937 eng(rd()); // seed the generator
    std::uniform_real_distribution<double> distr; // define the distribution
    std::cout << std::fixed << std::setfill('0') << std::setprecision(3);

    std::cout<<"\nGrid from -90 to 90, with step = 5.";
    tick_axis<double> t1(-90, 90, 5);
    std::cout<<"\n\tAxis is ascending?"<< std::boolalpha << t1.is_ascending();
    std::cout<<"\n\tNum of points:    "<<t1.num_pts();
    // set the range for the random generator
    std::uniform_real_distribution<double>::param_type new_range {-90, 90};
    distr.param(new_range);
    for (int i=0; i<10; ++i) {
        double t = distr(eng);
        std::cout<<"\n\tRanom Number "<<t<<" has index: "<<t1.index(t)
        <<" which is node: " <<t1.val_at_index(t1.index(t))
        <<" distance from node=" <<std::abs(t1.val_at_index(t1.index(t))-t)
        <<" nearest node: "<<t1.nearest_neighbor(t);
    }
    
    std::cout<<"\nGrid from 0 to 180, with step = 5.";
    tick_axis<double> t2(0, 180, 5);
    std::cout<<"\n\tAxis is ascending?"<< std::boolalpha << t2.is_ascending();
    std::cout<<"\n\tNum of points: "<<t2.num_pts();
    
    std::cout<<"\nGrid from 180 to 0, with step = 5.";
    tick_axis<double> t3(180, 0, -5);
    std::cout<<"\n\tAxis is ascending?"<< std::boolalpha << t3.is_ascending();
    std::cout<<"\n\tNum of points: "<<t3.num_pts();
    
    std::cout<<"\nGrid from 90 to -90, with step = 5.";
    tick_axis<double> t4(90, -90, -5);
    std::cout<<"\n\tAxis is ascending?"<< std::boolalpha << t4.is_ascending();
    std::cout<<"\n\tNum of points: "<<t4.num_pts();

    float j = 90;
    for (int i=0; i<t4.num_pts(); i++) {
        std::cout<<" " << j+i*t4.step();
    }

    std::cout<<"\n";
    return 0;
}

#endif
