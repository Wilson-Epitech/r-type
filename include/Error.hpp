/*
** R_TYPE PROJECT, 2024
** r-type
** File description:
** Error
*/


#ifndef ERROR_HPP_
    #define ERROR_HPP_

    #include <string>

class Error : public std::exception
{
    public:
        Error(const std::string& error_message): _errorMessage(error_message) {}
        const char *what() const noexcept
        {
            return this->_errorMessage.c_str();
        }
    private:
        std::string _errorMessage;
};

#endif /* !ERROR_HPP_ */