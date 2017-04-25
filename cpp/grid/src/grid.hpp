#ifndef __NGPT_GRID_HPP__
#define __NGPT_GRID_HPP__

#include <cmath>
#include <type_traits>
#include <tuple>

namespace ngpt
{

/// @class tick_axis
/// @brief A tick-axis is just an anotated axis.
///
/// A tick_axis is an anotated (every _step) axis, starting from _start and
/// ending at _stop.
/// The tick_axis is inlusive! Given e.g. the range [90, -90], both 90 and -90
/// will be valid nodes (i.e. ticks).
///
/// @tparam T the type of the axis, which can be any floating point type.
/// 
/// @warning  The class methods are designed not to validate (by default) if we
///           are indeed operating within the valid axis range (no checks are
///           performed on the input paramters). If the user wishes to check
///           the validity of the input values/parameters, the method
///           tick_axis::is_out_of_range could be used.
///
/// @example  test_tick_axis.cc
template<class T,
        typename = std::enable_if_t<std::is_floating_point<T>::value>
        >
    class tick_axis
{
public:

    /// Constructor. Set start, stop and step.
    ///
    /// @warning         This can never fail (i.e. throw) but the given 
    ///                  parameters could be wrong. Use the tick_axis::validate
    ///                  method to check if the tick_axis is constructed
    ///                  correctly.
    /// 
    /// @param[in] start The starting tick on the axis (inclusive).
    /// @param[in] stop  The ending tick on the axis (inclusive).
    /// @param[in] step  The step.
    ///
    /// @example         test_tick_axis.cc
    explicit
    tick_axis(T start, T stop, T step = T{1}) noexcept
    : _start{start},
      _stop{stop},
      _step{step}
    {}

    /// Validate that this tick_axis is actualy valid (i.e. its parameters are
    /// set correctly). The function will return false if something is wrong.
    ///
    /// @return True if the axis is setup correctly; false otherwise.
    ///
    /// @todo We should also check that (stop-start)%step == 0
    ///
    /// @example         test_tick_axis.cc
    bool
    validate() const noexcept
    { return static_cast<long>((_stop-_start)/_step) >= 0; }

    /// Check if the tick_axis is in ascending order.
    bool
    is_ascending() const noexcept { return _stop > _start; }

    /// Return the number of ticks on the axis. Valid indexes span the range:
    /// [0, num_pts()-1] or [0, num_pts()), i.e. the ticks are inclusive.
    std::size_t
    num_pts() const noexcept
    { return static_cast<std::size_t>((_stop - _start) / _step) + 1; }

    /// Given a value, return the index of the nearest **left** (axis) tick.
    /// The index may correspond to a tick greater than or less than the input
    /// value, depending on whether the axis is in ascending or descending
    /// order.
    ///
    /// @param[in] val An input value, to find the nearest tick, left of it
    ///                (i.e. on the tick-axis). This value should be in the
    ///                range [_start, _stop].
    /// @return        The index of the tick, left of the input value.
    ///
    /// @warning       Even if the input value falls outside the axis limits,
    ///                this function will compute and return a (maybe erroneus)
    ///                index. Check the value before feeding it to the function.
    ///
    /// @example       test_tick_axis.cc
    std::size_t
    index(T val) const noexcept 
    { return static_cast<std::size_t>((val-_start)/_step); }

    /// Given a value, return the index of the nearest axis tick.
    ///
    /// @param[in] val An input value for which to find the nearest tick on the
    ///                axis. This value should be in the range [_start, _stop].
    /// @return        The index of the tick nearest to the input value.
    ///
    /// @warning       Even if the input value falls outside the axis limits,
    ///                this function will compute and return a (maybe erroneus)
    ///                index. Check the value before feeding it to the function.
    ///
    /// @example       test_tick_axis.cc
    std::size_t
    nearest_neighbor(T val) const noexcept
    { return static_cast<std::size_t>(std::round((val-_start)/_step)); }
    
    /// Given an index of a tick, return it's value. A short example follows:
    /// @code{.cpp}
    ///   tick_axis<double> t {-90, 90, 5};
    ///   assert( t(0) == t.start() );
    ///   assert( t(t.num_pts()-1) == t.stop() );
    /// @endcode
    ///
    /// @param[in] idx The index of a tick on the axis, for which we want its
    ///                actual value. 
    /// @return        The value of the tick with index idx.
    /// 
    /// @warning       No check is performed on the validity of the given index
    ///                (meaning it could be out of range). Even if the given
    ///                index is out of range, a value will be returned.
    ///
    /// @example       test_tick_axis.cc
    T
    operator()(std::size_t idx) const noexcept
    { return _start + idx * _step; }

    /// Check if a value is out of range of the axis (i.e. out of range:
    /// [start, stop]).
    ///
    /// @param[in] val Value for which to check validity.
    /// @return        
    ///                -  0 if value is in valid range
    ///                - -1 if value is smaller than the minimum tick
    ///                - +1 if value is larger than the largest tick
    int
    is_out_of_range(T val) const noexcept
    {
        if ( val > this->max_val() ) return 1;
        if ( val < this->min_val() ) return -1;
        return 0;
    }

    /// Maximum value on the tick_axis (this may be **not** the rightmost value).
    T
    max_val() const noexcept
    { return this->is_ascending() ? _stop : _start; }

    /// Minimum value on the tick_axis (this may be **not** the leftmost value).
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
    T _start, ///< the leftmost value/tick of the axis.
      _stop,  ///< the rightmost value/tick of the axis.
      _step;  ///< the step size.
}; // class tick_axis

/// @class grid2d
/// @brief A two-dimensional grid (no data).
/// 
/// A grid2d is a two-dimensional grid skeleton, i.e. a grid with no data. It
/// consists of two axis (the x- and y-axis), which are tick_axis instances.
/// As with tick_axis, grid2d has inclusive ranges, meaning that, e.g.
/// constructing the grid: "grid2d g (-90, 90, 2.5, 0, 180, 5)", all of
/// -90, 90, 0 and 180 are valid ticks (for the x- and y-axis respectively).
/// @see tick_axis
///
/// @tparam T the type of the axis, which can be any floating point type.
/// 
/// @warning  The class methods are designed not to validate (by default) if we
///           are indeed operating within the valid grid range (no checks are
///           performed on the input paramters). If the user wishes to check
///           the validity of the input values/parameters, the method
///           tick_axis::is_out_of_range could be used.
///
/// @example  test_tick_axis.cc
template<typename T>
    class grid2d
{
public:
    typedef std::tuple<std::size_t, std::size_t> index_pair;
    typedef std::tuple<T, T>                     value_pair;

    /// Constructor. Set start, stop and step for both axis (x and y).
    ///
    /// @warning         This can never fail (i.e. throw) but the given 
    ///                  parameters could be wrong. Use the grid2d::validate
    ///                  method to check if the tick_axis are constructed
    ///                  correctly.
    /// 
    /// @param[in] xstart The starting tick on the x-axis (inclusive).
    /// @param[in] xstop  The ending tick on the x-axis (inclusive).
    /// @param[in] xstep  The step of the x-axis.
    /// @param[in] ystart The starting tick on the y-axis (inclusive).
    /// @param[in] ystop  The ending tick on the y-axis (inclusive).
    /// @param[in] ystep  The step of the y-axis.
    explicit
    grid2d(T xstart, T xstop, T xstep, T ystart, T ystop, T ystep) noexcept
    : _xaxis{xstart, xstop, xstep},
      _yaxis{ystart, ystop, ystep}
    {}

    /// Return the number of ticks on the x-axis. Valid indexes span the range:
    /// [0, num_pts()-1] or [0, num_pts()), i.e. the ticks are inclusive.
    std::size_t
    xpts() const noexcept { return _xaxis.num_pts(); }
    
    /// Return the number of ticks on the y-axis. Valid indexes span the range:
    /// [0, num_pts()-1] or [0, num_pts()), i.e. the ticks are inclusive.
    std::size_t
    ypts() const noexcept { return _yaxis.num_pts(); }

    /// Validate that this grid2d is actualy valid (i.e. its parameters are
    /// set correctly). The function will return false if something is wrong.
    ///
    /// @return True if the axis are setup correctly; false otherwise.
    bool
    validate() const noexcept
    { return _xaxis.validate() && _yaxis.validate(); }

    /// Check if a value pair lies in the valid range of the grid, i.e. within
    /// [x-axis-start, x-axis-stop] and [y-axis-start, y-axis-stop].
    ///
    /// @param[in] xval The value for the x-axis.
    /// @param[in] yval The value for the y-axis.
    /// @return         0 if the values fall in the valid range(s); if any other
    ///                 integer is returned, the one or both of the values are
    ///                 out of range.
    ///
    /// @see tick_axis::is_out_of_range
    int
    is_out_of_range(T&& xval, T&& yval) const noexcept
    { 
        return ( _xaxis.is_out_of_range(xval)
              || _yaxis.is_out_of_range(yval) );
    }
    /// Check if a value pair lies in the valid range of the grid, i.e. within
    /// [x-axis-start, x-axis-stop] and [y-axis-start, y-axis-stop].
    ///
    /// @param[in] xyvals A two-element tuple; first element is the x-axis
    ///                   value, second is the y-axis value.
    /// @return           0 if the values fall in the valid range(s); if any
    ///                   other integer is returned, the one or both of the
    ///                   values are out of range.
    ///
    /// @see tick_axis::is_out_of_range
    int is_out_of_range(value_pair&& xyvals) const noexcept
    {
        return this->is_out_of_range(std::get<0>(xyvals), std::get<1>(xyvals));
    }

    /// Transform an index_pair (i.e. a pair of x- and y-axis tick indexes) to
    /// their respective values (i.e a value_pair, meaning the values at the
    /// x- and y-axis tick indexes).
    ///
    /// @param[in] idx_pair The index pair; first element is the x-axis tick
    ///                     index, while the second element is the y-axis tick
    ///                     index.
    /// @return             A value_pair corresponding to the values at the
    ///                     tick indexes in idx_pair.
    /// @warning            No check is performed on the given indexes (i.e. if
    ///                     they indeed lie on the axis ranges).
    value_pair
    operator()(index_pair&& idx_pair) const noexcept
    {
        return value_pair{_xaxis(std::get<0>(idx_pair)),
                          _yaxis(std::get<1>(idx_pair))};
    }

    /// Find the node (i.e. x-axis & y-axis tick indexes) on the bottom-left of
    /// the given value-pair.
    ///
    /// @param[in] xval The input xval, should be in range:
    ///                 [xaxis.start, xaxis.stop]
    /// @param[in] yval The input xval, should be in range:
    ///                 [yaxis.start, yaxis.stop]
    /// @return         An index pair; the first value (of the pair) is the
    ///                 index of the x-axis tick, immidiately left of xval; the
    ///                 second value is the y-axis tick index 
    ///                 immidiately left (actually bottom) of yval.
    /// @warning        Even if the input values fall outside the axis limits,
    ///                 this function will compute and return (maybe erroneus)
    ///                 indexes. Check the values before feeding them to the
    ///                 function.
    /// @note           This function only returns indexes; should the user want
    ///                 the actual values on the ticks, then transform the
    ///                 result with the grid2d::idx_pair2val_pair function.
    index_pair
    bottom_left(T xval, T yval) const noexcept
    {
        return index_pair{_xaxis.index(xval), _yaxis.index(yval)};
    }

private:
    tick_axis<T> _xaxis,
                 _yaxis;
}; // class grid2d

/**
 * There are a few ways to load/save the grid (values) in memory. I can think
 * of four that can be of use. The first two are row-major:
 *
 * ROW-MAJOR START AT BOTTOM LEFT
 *
 *  +---+---+--...--+---+
 *  |   |   |       |   |
 *  +---+---+--...--+---+
 *  |   |   |       |   |
 *  .   .   .       .   .
 *  .   .   .       .   .
 *  |   |   |       |   |
 *  +---+---+--...--+---+
 *  |   |   |       |   |
 *  +---+---+--...--+---+
 */
enum class grid_alloc_type : char
{
    rm_tl, ///< Row-Major, starting on top left corner
    rm_bl  ///< Row-Major, starting on bottom left
};

/// @class data_grid2d
/// @brief A two-dimensional grid with data.
///
/// @tparam T The tick-axis type(s), can be any floating point type.
/// @tparam D The type of the (actual) data.
/// @tparam G The order the data is allocated in (any of grid_alloc_type).
template<typename T,
         typename D,
         grid_alloc_type G
         >
    class data_grid2d
{
public:
    /// Constructor ...
    explicit
    data_grid2d(T xstart, T xstop, T xstep, T ystart, T ystop, T ystep)
    noexcept
    :_grid{xstart, xstop, xstep, ystart, ystop, ystep},
     _xpts{_grid.xpts()},
     _ypts{_grid.ypts()},
     _data{nullptr}
     {}

    /// Convert a pair of x and y indexes to the corresponding data array index.
    /// @param[in] t A tuple of two elements; first is x-index, second is y-index.
    /// @return      The corresponding index of the data array.
    /// @warning     The function will not check the validity of either x or
    ///              y index. It will return a result even if they do not lie
    ///              within the valid ranges.
    std::size_t
    xy_idx2d_idx(std::tuple<std::size_t, std::size_t>&& t) const noexcept
    { return idx_pair2index_impl(t, __is_bl()); }

    /// Convert a pair of x and y indexes to the corresponding data array index.
    /// @param[in] xidx The x index value. 
    /// @param[in] yidx The y index value. 
    /// @return         The corresponding index of the data array.
    /// @warning        The function will not check the validity of either x or
    ///                 y index. It will return a result even if they do not lie
    ///                 within the valid ranges.
    std::size_t
    xy_idx2d_idx(std::size_t xidx, std::size_t yidx) const noexcept
    { return idx_pair2index_impl(xidx, yidx, __is_bl()); }

    D&
    at(std::tuple<std::size_t, std::size_t>&& t) noexcept
    { return _data[this->xy_idx2d_idx(t)]; }
    
    D&
    at(std::size_t xidx, std::size_t yidx) noexcept
    { return _data[this->xy_idx2d_idx(xidx, yidx)]; }

private:
    /// A static constant of type grid_alloc_type::rm_bl
    typedef std::integral_constant<grid_alloc_type,
                                   grid_alloc_type::rm_bl> __bl;
    /// A static constant of type grid_alloc_type::G (i.e. allocation type
    /// of instatiation).
    typedef std::integral_constant<grid_alloc_type,
                                   G> __gt;
    /// std::true_type if (this) allocation type is grid_alloc_type::rm_bl
    using __is_bl = std::is_same<__bl,
                                 __gt>;

    /// (Implementation) Convert a pair of x and y indexes to the corresponding
    /// data array index, when the allocation type is grid_alloc_type::rm_bl.
    /// This is a (partial) specialization for the method xy_idx2d_idx.
    ///
    /// @param[in] t A tuple of two elements; first is x-index, second is y-index.
    /// @return      The corresponding index of the data array.
    /// @warning     The function will not check the validity of either x or
    ///              y index. It will return a result even if they do not lie
    ///              within the valid ranges.
    /// @see xy_idx2d_idx
    std::size_t
    idx_pair2index_impl(std::tuple<std::size_t, std::size_t>&& t, std::true_type)
    const noexcept
    { return std::get<1>(t)*_xpts+std::get<0>(t); }
    
    /// (Implementation) Convert a pair of x and y indexes to the corresponding
    /// data array index, when the allocation type is grid_alloc_type::rm_bl.
    /// This is a (partial) specialization for the method xy_idx2d_idx.
    ///
    /// @param[in] xidx The x index value. 
    /// @param[in] yidx The y index value. 
    /// @return         The corresponding index of the data array.
    /// @warning        The function will not check the validity of either x or
    ///                 y index. It will return a result even if they do not lie
    ///                 within the valid ranges.
    /// @see xy_idx2d_idx
    std::size_t
    idx_pair2index_impl(std::size_t xidx, std::size_t yidx, std::true_type)
    const noexcept
    { return yidx*_xpts+xidx; }
    
    /// (Implementation) Convert a pair of x and y indexes to the corresponding
    /// data array index, when the allocation type is grid_alloc_type::rm_tl.
    /// This is a (partial) specialization for the method xy_idx2d_idx.
    ///
    /// @param[in] t A tuple of two elements; first is x-index, second is y-index.
    /// @return      The corresponding index of the data array.
    /// @warning     The function will not check the validity of either x or
    ///              y index. It will return a result even if they do not lie
    ///              within the valid ranges.
    /// @see xy_idx2d_idx
    std::size_t
    idx_pair2index_impl(std::tuple<std::size_t, std::size_t>&& t, std::false_type)
    const noexcept
    { return (_ypts-std::get<1>(t)-1)*_xpts+std::get<0>(t); }
    
    /// (Implementation) Convert a pair of x and y indexes to the corresponding
    /// data array index, when the allocation type is grid_alloc_type::rm_tl.
    /// This is a (partial) specialization for the method xy_idx2d_idx.
    ///
    /// @param[in] xidx The x index value. 
    /// @param[in] yidx The y index value. 
    /// @return         The corresponding index of the data array.
    /// @warning        The function will not check the validity of either x or
    ///                 y index. It will return a result even if they do not lie
    ///                 within the valid ranges.
    /// @see xy_idx2d_idx
    std::size_t
    idx_pair2index_impl(std::size_t xidx, std::size_t yidx, std::false_type)
    const noexcept
    { return (_ypts-yidx-1)*_xpts+xidx; }

    grid2d<T>   _grid; ///< The two-dimensional grid.
    std::size_t _xpts, ///< Number of ticks on x-axis.
                _ypts; ///< Number of ticks on y-axis.
    D*          _data; ///< The data array.
}; //class data_grid2d

} // namespace ngpt

#endif
