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

    /// Constructor. Set start, stop and step. This can never fail (i.e. throw)
    /// but the given parameters could be wrong. Use the tick_axis::validate
    /// method to check if the tick_axis is constructed correctly.
    explicit
    tick_axis(T start, T stop, T step = T{1}) noexcept
    : _start{start},
      _stop{stop},
      _step{step}
    {}

    /// Validate that this tick_axis is actualy valid (i.e. its parameters are
    /// set correctly). The function will return false if something is wrong.
    bool
    validate() const noexcept
    { return static_cast<long>((_stop-_start)/_step) >= 0; }

    /// Check if the tick_axis is in ascending order.
    bool
    is_ascending() const noexcept
    { return _stop > _start; }

    /// Return the number of ticks on the axis. Valid indexes span the range:
    /// [0, num_pts()-1] or [0, num_pts()).
    std::size_t
    num_pts() const noexcept
    { return static_cast<std::size_t>((_stop - _start) / _step) + 1; }

    /// Given a value, return the index of the nearest **left** axis tick.
    std::size_t
    index(T val) const noexcept
    { return static_cast<std::size_t>((val-_start)/_step); }

    /// Given a value, return the index of the nearest axis tick.
    std::size_t
    nearest_neighbor(T val) const noexcept
    {
        std::size_t idx { this->index(val) };
        // Note: the division should always be with a float/double 2; just
        //       imagine the case where the template parameter T is int!
        return  std::abs(val-this->operator()(idx)) > std::abs(_step)/2e0
                ? ( (idx == num_pts()-1) ? idx : ++idx )
                : idx;
    }
    
    /// Given an index of a tick, return it's value. No check is performed on
    /// on the validity of the given index (meaning it could be out of range).
    T
    operator()(std::size_t idx) const noexcept
    { return _start + idx * _step; }

    /// Maximum value on the tick_axis
    T
    max_val() const noexcept
    { return this->is_ascending() ? _stop : _start; }

    /// Minimum value on the tick_axis
    T
    min_val() const noexcept
    { return this->is_ascending() ? _start : _stop; }

    /// The (value) of the starting (i.e. leftmost) tick.
    T
    start() const noexcept { return _start; }

    /// The (value) of the ending (i.e. rightmost) tick.
    T
    stop() const noexcept { return _stop; }

    /// The step value.
    T
    step() const noexcept { return _step; }

private:
    T _start,
      _stop,
      _step;
}; // class tick_axis

enum class storage_order : char { col_wise, row_wise };

// forward decleration
template<typename T, storage_order S> class gnode;

template<typename T, storage_order S>
    class two_dim_grid
{
public:

    /// Constructor given start, stop and step for each of the axis (i.e. x
    /// and y). No check is performed on the validity of the given input
    /// arguments. To check if the constructed grid is valid, use the validate()
    /// method.
    explicit
    two_dim_grid(T xstart, T xstop, T xstep, T ystart, T ystop, T ystep)
    noexcept
    : _xaxis{xstart, xstop, xstep},
      _yaxis{ystart, ystop, ystep}
    {}

    /// Check if the calling grid is valid.
    bool
    validate() const noexcept
    { return _xaxis.validate() && _yaxis.validate(); }

    /// Number of points (or grid nodes) on the gird.
    std::size_t
    num_pts() const noexcept
    { return _xaxis.num_pts() * _yaxis.num_pts(); }

    /// Number of nodes on the x-axis.
    std::size_t
    x_pts() const noexcept { return _xaxis.num_pts(); }
    
    /// Number of nodes on the y-axis.
    std::size_t
    y_pts() const noexcept { return _yaxis.num_pts(); }

    /// Given two indexes (i.e. on the x and y-axis respectively), return
    /// the values (x and y) on the node.
    auto
    index2node_values(T xidx, T yidx) const noexcept
    {
        return std::pair<T,T>{_xaxis.operator()(xidx), _yaxis.operator()(yidx)};
    }
    
    /// Given a value, return the index of the nearest node.
    gnode<T,S>
    nearest_neighbor(T x, T y) const noexcept
    {
        std::size_t x_idx { _xaxis.nearest_neighbor(x) };
        std::size_t y_idx { _yaxis.nearest_neighbor(y) };
        return gnode<T,S>{x_idx, y_idx, this};
    }

    gnode<T,S>
    node_on_left(T x, T y) const noexcept
    {
        std::size_t x_idx { _xaxis.index(x) };
        std::size_t y_idx { _yaxis.index(y) };
        return gnode<T,S>{x_idx, y_idx, this};
    }

    /*
    grid_value
    val_at_index(const grid_node& node) const
    {
        return grid_value{_xaxis.val_at_index(node.first),
                          _yaxis.val_at_index(node.second)};
    }

    grid_value
    operator()(const grid_node& node) const noexcept
    {
        return grid_value{_xaxis(node.first), _yaxis(node.second)};
    }

    grid_value
    val_at_index(std::size_t xi, std::size_t yi) const
    { return val_at_index(grid_node{xi, yi}); }
    
    grid_value
    operator()(std::size_t xi, std::size_t yi) const noexcept
    { return this->operator()(grid_node{xi, yi}); }

    grid_node
    nearest_neighbor(T x, T y) const noexcept
    {
        auto n = grid_node{_xaxis.nearest_neighbor(x),
                           _yaxis.nearest_neighbor(y)};
        return n;
    }
    */

    /*
    gnode<T,S>
    begin() const noexcept
    {
        return gnode<T,S>{0, 0, this};
    }

    gnode<T,S>
    end() const noexcept
    {
        return gnode<T,S>{_xaxis.num_pts(), _yaxis.num_pts(), this};
    }

    /// Past-the-end (invalid) node on row (i.e. y-axis row) with index yi.
    gnode<T,S>
    end_of_row(std::size_t yi) const noexcept
    { return gnode<T,S>{_xaxis.num_pts(), yi, this}; }
    
    /// first (valid) node on row (i.e. y-axis row) with index yi.
    gnode<T,S>
    start_of_row(std::size_t yi) const noexcept
    { return gnode<T,S>{0, yi, this}; }

    /// Prior-to-first (invalid) node on row (i.e. y-axis row) with index yi.
    gnode<T,S>
    before_start_of_row(std::size_t yi) const noexcept
    { return gnode<T,S>{std::numeric_limits<std::size_t>::max(), yi, this}; }
    
    /// Past-the-end (invalid) node on column (i.e. x-axis row) with index xi.
    gnode<T,S>
    end_of_col(std::size_t xi) const noexcept
    { return gnode<T,S>{xi, _yaxis.num_pts(), this}; }
    
    /// first (valid) node on column (i.e. x-axis row) with index xi.
    gnode<T,S>
    start_of_col(std::size_t xi) const noexcept
    { return gnode<T,S>{xi, 0, this}; }
    
    /// Prior-to-first (invalid) node on column (i.e. x-axis row) with index xi.
    gnode<T,S>
    before_start_of_col(std::size_t xi) const noexcept
    { return gnode<T,S>{xi, std::numeric_limits<std::size_t>::max(), this}; }
    */

private:
    tick_axis<T> _xaxis,
                 _yaxis;

    /*
    std::size_t
    node2index_cw(const grid_node& node) const noexcept
    {
        auto x = node.first;
        auto y = node.second;
        return _yaxis.num_pts()*x + y;
    }

    std::size_t
    node2index_rw(const grid_node& node) const noexcept
    {
        auto x = node.first;
        auto y = node.second;
        return _xaxis.num_pts()*y + x;
    }
    */
};// class two_dim_grid

/// A grid-node class
template<typename T, storage_order S>
    class gnode
{
public:

    /// Default Constructor; the created gnode is hanging(!), i.e. it does not
    /// have an underlying grid.
    gnode() noexcept
    : _x{0},
      _y{0},
      _grid{nullptr}
    {};
    
    /// Constructor using a valid grid; the gnode will point to the first node
    /// of the grid.
    explicit
    gnode(const two_dim_grid<T,S>* t) noexcept
    : _x{0},
      _y{0},
      _grid{t}
    {};
    
    /// Constructor using a valid grid and axis indexes.
    explicit
    gnode(std::size_t xi, std::size_t yi, const two_dim_grid<T,S>* t) noexcept
    : _x{xi},
      _y{yi},
      _grid{t}
    {};

    /// Equality operator; two gnodes are the same if they are in the same grid
    /// and point to the same node.
    bool
    operator==(const gnode& node) const noexcept
    {
        return _grid == node._grid && (_x == node._x && _y == node._y);
    }

    /// Inequality operator. See gnode::operator==()
    bool
    operator!=(const gnode& node) const noexcept
    { return !(this->operator==(node)); }

    /*
    /// Return the next node; there are a number of different options here,
    /// depending on the current (i.e. calling) node:
    /// if the calling node is the last on the grid, then grid::end() is
    /// returned else, the next node is returned (which may be in a different
    /// column or/and row).
    gnode
    next() const noexcept
    {
        if ( _x == _grid->x_pts() ) {
            if ( _y == _grid->y_pts() ) {
                return _grid->end();
            }
            return _grid->start_of_row(this->_y + 1);
        }
        return gnode {_x+1, _y, _grid};
    }

    /// Return the node on the right (i.e. of the current calling node). if the
    /// calling node is the last on the grid node, then grid::end() is returned;
    /// if the calling node is already the one-past the row, then
    /// _grid::end_of_row() is returned.
    gnode
    next_right() const noexcept
    {
        if ( this->is_end_of_row() ) {
            if ( this->is_end_of_col() ) {
                return _grid->end();
            }
            return _grid->end_of_row(this->_y);
        }
        return gnode{_x+1, _y, _grid};
    }

    gnode
    next_up() const noexcept
    {
        if ( _y == 0 || _y == std::numeric_limits<std::size_t>::max() ) {
            return _grid->before_start_of_col(this->_x);
        }
        return gnode{_x, _y+1, this};
    }
    */
#ifdef DEBUG
    void
    dump() const noexcept
    {
        std::pair<T,T> vals = _grid->index2node_values(_x, _y);
        std::cout<< "(" << vals.first << ", " << vals.second << ")";
    }
#endif

private:
    std::size_t              _x,    ///< index of x axis
                             _y;    ///< index of y axis
    const two_dim_grid<T,S>* _grid; ///< ptr to the actual grid

    /*
    bool
    is_end_of_row() const noexcept { return _x >= _grid->x_pts(); }

    bool
    is_end_of_col() const noexcept { return _y >= _grid->y_pts(); }

    gnode
    next_on_right_colwise() const noexcept
    {
        gnode r {*this};
        ++r._x;
        return r;
    }
    
    gnode
    on_right_rowwise() const noexcept
    {
        gnode r {*this};
        r._x += 1;
        return r;
    }
    */
};// class gnode

}

using ngpt::tick_axis;
using ngpt::two_dim_grid;

int main()
{
    std::random_device rd; // obtain a random number from hardware
    std::mt19937 eng(rd()); // seed the generator
    std::uniform_real_distribution<double> distr; // define the distribution
    std::cout << std::fixed << std::setfill('0') << std::setprecision(3);

    std::cout<<"\nGrid:";
    std::cout<<"\nX range -90 :   90 :  2.5";
    std::cout<<"\nY range 180 : -180 : -5.0";
    two_dim_grid<double, ngpt::storage_order::row_wise> g{-90, 90, 2.5, 180, -180, -5};
    std::uniform_real_distribution<double>::param_type new_range1 {-90, 90};
    std::uniform_real_distribution<double>::param_type new_range2 {-180, 180};
    ngpt::gnode<double,ngpt::storage_order::row_wise> n;
    double xi,yi;
    for (int i=0; i<20; i++) {
        distr.param(new_range1);
        xi = distr(eng);
        distr.param(new_range2);
        yi = distr(eng);
        n = g.nearest_neighbor(xi, yi);
        std::cout<<"\nPoint ("<<xi<<", "<<yi<<") nearest neghr = ";
        n.dump();
        n = g.node_on_left(xi, yi);
        std::cout<<" left node = ";
        n.dump();
    }
    
    /*
    std::cout<<"\nGrid from -90 to 90, with step = -5. (INVALID!)";
    tick_axis<double> te1(-90, 90, -5);
    std::cout<<"\n\tGrid is valid? "<< std::boolalpha << te1.validate();
    std::cout<<"\nGrid from 90 to -90, with step = 5. (INVALID!)";
    tick_axis<double> te2(90, -90, 5);
    std::cout<<"\n\tGrid is valid? "<< std::boolalpha << te2.validate();
    std::cout<<"\nGrid from 90 to 180, with step = -5. (INVALID!)";
    tick_axis<double> te3(90, 180, -5);
    std::cout<<"\n\tGrid is valid? "<< std::boolalpha << te3.validate();

    std::cout<<"\nGrid from -90 to 90, with step = 5.";
    tick_axis<double> t1(-90, 90, 5);
    std::cout<<"\n\tAxis is ascending?"<< std::boolalpha << t1.is_ascending();
    std::cout<<"\n\tNum of points:    "<<t1.num_pts();
    for (auto i = 0; i < t1.num_pts(); i++) {
        std::cout<<"\n\tt1["<<i<<"] = " <<t1(i);
    }
    // set the range for the random generator
    std::uniform_real_distribution<double>::param_type new_range1 {-90, 90};
    distr.param(new_range1);
    for (int i=0; i<10; ++i) {
        t = distr(eng);
        std::cout<<"\n\tRanom Number "<<t<<" has index: "<<t1.index(t)
        <<" which is node: " <<t1.val_at_index(t1.index(t))
        <<" distance from node=" <<std::abs(t1.val_at_index(t1.index(t))-t)
        <<" nearest node: "<<t1.nearest_neighbor(t)<<" ("<<t1.val_at_index(t1.nearest_neighbor(t))<<")";
    }
    t = -90e0;
    std::cout<<"\n\tRanom Number "<<t<<" has index: "<<t1.index(t)
    <<" which is node: " <<t1.val_at_index(t1.index(t))
    <<" distance from node=" <<std::abs(t1.val_at_index(t1.index(t))-t)
    <<" nearest node: "<<t1.nearest_neighbor(t)<<" ("<<t1.val_at_index(t1.nearest_neighbor(t))<<")";
    t = 90e0;
    std::cout<<"\n\tRanom Number "<<t<<" has index: "<<t1.index(t)
    <<" which is node: " <<t1.val_at_index(t1.index(t))
    <<" distance from node=" <<std::abs(t1.val_at_index(t1.index(t))-t)
    <<" nearest node: "<<t1.nearest_neighbor(t)<<" ("<<t1.val_at_index(t1.nearest_neighbor(t))<<")";

    std::cout<<"\nGrid from 0 to 180, with step = 5.";
    tick_axis<double> t2(0, 180, 5);
    std::cout<<"\n\tAxis is ascending?"<< std::boolalpha << t2.is_ascending();
    std::cout<<"\n\tNum of points: "<<t2.num_pts();
    // set the range for the random generator
    std::uniform_real_distribution<double>::param_type new_range2 {0, 180};
    distr.param(new_range2);
    for (int i=0; i<10; ++i) {
        t = distr(eng);
        std::cout<<"\n\tRanom Number "<<t<<" has index: "<<t2.index(t)
        <<" which is node: " <<t2.val_at_index(t2.index(t))
        <<" distance from node=" <<std::abs(t2.val_at_index(t2.index(t))-t)
        <<" nearest node: "<<t2.nearest_neighbor(t)<<" ("<<t2.val_at_index(t2.nearest_neighbor(t))<<")";
    }
    t = 0e0;
    std::cout<<"\n\tRanom Number "<<t<<" has index: "<<t2.index(t)
    <<" which is node: " <<t2.val_at_index(t2.index(t))
    <<" distance from node=" <<std::abs(t2.val_at_index(t2.index(t))-t)
    <<" nearest node: "<<t2.nearest_neighbor(t)<<" ("<<t2.val_at_index(t2.nearest_neighbor(t))<<")";
    t = 180e0;
    std::cout<<"\n\tRanom Number "<<t<<" has index: "<<t2.index(t)
    <<" which is node: " <<t2.val_at_index(t2.index(t))
    <<" distance from node=" <<std::abs(t2.val_at_index(t2.index(t))-t)
    <<" nearest node: "<<t2.nearest_neighbor(t)<<" ("<<t2.val_at_index(t2.nearest_neighbor(t))<<")";
    
    std::cout<<"\nGrid from 180 to 0, with step = 5.";
    tick_axis<double> t3(180, 0, -5);
    std::cout<<"\n\tAxis is ascending?"<< std::boolalpha << t3.is_ascending();
    std::cout<<"\n\tNum of points: "<<t3.num_pts();
    // set the range for the random generator
    std::uniform_real_distribution<double>::param_type new_range3 {0, 180};
    distr.param(new_range3);
    for (int i=0; i<10; ++i) {
        t = distr(eng);
        std::cout<<"\n\tRanom Number "<<t<<" has index: "<<t3.index(t)
        <<" which is node: " <<t3.val_at_index(t3.index(t))
        <<" distance from node=" <<std::abs(t3.val_at_index(t3.index(t))-t)
        <<" nearest node: "<<t3.nearest_neighbor(t)<<" ("<<t3.val_at_index(t3.nearest_neighbor(t))<<")";
    }
    t = 180e0;
    std::cout<<"\n\tRanom Number "<<t<<" has index: "<<t3.index(t)
    <<" which is node: " <<t3.val_at_index(t3.index(t))
    <<" distance from node=" <<std::abs(t3.val_at_index(t3.index(t))-t)
    <<" nearest node: "<<t3.nearest_neighbor(t)<<" ("<<t3.val_at_index(t3.nearest_neighbor(t))<<")";
    t = 0e0;
    std::cout<<"\n\tRanom Number "<<t<<" has index: "<<t3.index(t)
    <<" which is node: " <<t3.val_at_index(t3.index(t))
    <<" distance from node=" <<std::abs(t3.val_at_index(t3.index(t))-t)
    <<" nearest node: "<<t3.nearest_neighbor(t)<<" ("<<t3.val_at_index(t3.nearest_neighbor(t))<<")";
    
    std::cout<<"\nGrid from 90 to -90, with step = 5.";
    tick_axis<double> t4(90, -90, -5);
    std::cout<<"\n\tAxis is ascending?"<< std::boolalpha << t4.is_ascending();
    std::cout<<"\n\tNum of points: "<<t4.num_pts();
    // set the range for the random generator
    std::uniform_real_distribution<double>::param_type new_range4 {90, -90};
    distr.param(new_range4);
    for (int i=0; i<10; ++i) {
        t = distr(eng);
        std::cout<<"\n\tRanom Number "<<t<<" has index: "<<t4.index(t)
        <<" which is node: " <<t4.val_at_index(t4.index(t))
        <<" distance from node=" <<std::abs(t4.val_at_index(t4.index(t))-t)
        <<" nearest node: "<<t4.nearest_neighbor(t)<<" ("<<t4.val_at_index(t4.nearest_neighbor(t))<<")";
    }
    t = 90e0;
    std::cout<<"\n\tRanom Number "<<t<<" has index: "<<t4.index(t)
    <<" which is node: " <<t4.val_at_index(t4.index(t))
    <<" distance from node=" <<std::abs(t4.val_at_index(t4.index(t))-t)
    <<" nearest node: "<<t4.nearest_neighbor(t)<<" ("<<t4.val_at_index(t4.nearest_neighbor(t))<<")";
    t = -90e0;
    std::cout<<"\n\tRanom Number "<<t<<" has index: "<<t4.index(t)
    <<" which is node: " <<t4.val_at_index(t4.index(t))
    <<" distance from node=" <<std::abs(t4.val_at_index(t4.index(t))-t)
    <<" nearest node: "<<t4.nearest_neighbor(t)<<" ("<<t4.val_at_index(t4.nearest_neighbor(t))<<")";

    float j = 90;
    std::cout<<"\n";
    for (int i=0; i<t4.num_pts(); i++) {
        std::cout<<" " << j+i*t4.step();
    }
    */

    std::cout<<"\n";
    return 0;
}

#endif
