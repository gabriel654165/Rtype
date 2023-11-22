/*
** EPITECH PROJECT, 2021
** B-CPP-500-PAR-5-1-Rtype-Maxime.Dodin
** File description:
** Error.hpp
*/
#pragma once

#include <iostream>
#include <exception>

namespace err {
    class Rtype : public std::exception {
        public:
        explicit Rtype(std::string what, std::string where = "./");

        ~Rtype() noexcept override = default;

        [[nodiscard]] const char *where() const noexcept;

        [[nodiscard]] const char *what() const noexcept final;

        protected:
        std::string _what;

        std::string _where;
    };

    class LoaderException : public Rtype {
        public:
        explicit LoaderException(const std::string, const std::string where = "./");

        ~LoaderException() noexcept override = default;

    };

}
