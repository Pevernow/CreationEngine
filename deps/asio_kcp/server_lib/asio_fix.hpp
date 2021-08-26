#include "asio/basic_waitable_timer.hpp"
#include "asio/detail/config.hpp"

#include <chrono>

namespace asio
{
#if defined(GENERATING_DOCUMENTATION)
/// Typedef for a timer based on the high resolution clock.
/**
 * This typedef uses the C++11 @c &lt;chrono&gt; standard library facility,
 * if available. Otherwise, it may use the Boost.Chrono library. To
 * explicitly utilise Boost.Chrono, use the basic_waitable_timer template
 * directly:
 * @code
 * typedef basic_waitable_timer<boost::chrono::high_resolution_clock> timer;
 * @endcode
 */
typedef basic_waitable_timer<chrono::high_resolution_clock>
    high_resolution_timer;
#elif defined(ASIO_HAS_STD_CHRONO)
typedef basic_waitable_timer<std::chrono::high_resolution_clock>
    high_resolution_timer;
#elif defined(ASIO_HAS_BOOST_CHRONO)
typedef basic_waitable_timer<boost::chrono::high_resolution_clock>
    high_resolution_timer;
#endif

} // namespace asio