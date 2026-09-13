#ifndef FLXSCALERS_EXCEPTIONS_NOT_FITTED_ERROR_HPP
#define FLXSCALERS_EXCEPTIONS_NOT_FITTED_ERROR_HPP

#include <stdexcept>

class NotFittedError : public std::logic_error {
public:
    using std::logic_error::logic_error;
};

#endif
