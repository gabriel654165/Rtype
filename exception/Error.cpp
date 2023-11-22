/*
** EPITECH PROJECT, 2021
** B-CPP-500-PAR-5-1-Babel-killian.fleury
** File description:
** Error.cpp
*/
#include <utility>
#include "Error.hpp"

err::Rtype::Rtype(std::string what, std::string where) : _what(std::string("'") + std::move(what) + std::string("'")),
    _where(std::string("./") + std::move(where))
{
}

err::LoaderException::LoaderException(const std::string what, const std::string where) : Rtype(what, where)
{

}

const char *err::Rtype::where() const noexcept
{
    return (_where.c_str());
}

const char *err::Rtype::what() const noexcept
{
    return (_what.c_str());
}
