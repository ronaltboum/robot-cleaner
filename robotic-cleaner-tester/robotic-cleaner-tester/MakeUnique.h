#ifndef _MAKE_UNIQUE__
#define _MAKE_UNIQUE__

#include <memory>
#ifdef __cpp_lib_make_unique
using std::make_unique;
#else
template<typename T, typename... Args>
inline unique_ptr<T>
make_unique(Args&&... args)
{
  return unique_ptr<T>(new T(std::forward<Args>(args)...));
}
#endif // __cpp_lib_make_unique

#endif // _MAKE_UNIQUE__
