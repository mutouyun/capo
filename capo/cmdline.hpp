/*
    The Capo Library
    Code covered by the MIT License

    Author: mutouyun (http://orzz.org)
*/

#pragma once

#include "capo/output.hpp"

#include <string>       // std::string
#include <functional>   // std::function
#include <iostream>     // std::cout
#include <utility>      // std::forward
#include <vector>       // std::vector

namespace capo {
namespace cmdline {

class parser;
using handle_t = void(const parser&, const std::string&);

struct option
{
    const char* sname_;
    const char* lname_;
    const char* description_;
    bool        necessary_;
    std::string default_;
    std::function<handle_t> handle_;
};

using options = std::vector<option>;

class parser
{
    options necessary_, optional_;
    std::function<handle_t> usage_;
    std::string path_;

public:
    options&       necessary(void)       { return necessary_; }
    const options& necessary(void) const { return necessary_; }

    options&       optional(void)       { return optional_; }
    const options& optional(void) const { return optional_; }

    std::function<handle_t>&       usage(void)       { return usage_; }
    const std::function<handle_t>& usage(void) const { return usage_; }

    template <typename T>
    void print_usage(T&& out) const
    {
        auto flw = capo::output(std::forward<T>(out), "");
        if (path_.empty())
        {
            flw("Must has at least one argument (the path of current program).").ln();
            return;
        }
        size_t slash = path_.find_last_of('\\');
        if (slash == std::string::npos) slash = path_.find_last_of('/');
        std::string name = path_.substr(slash + 1);
        if (usage_)
        {
            usage_(*this, name);
        }
        else
        {
            flw("Usage: {} ", name);
            if (!necessary_.empty()) for (auto& o: necessary_)
            {
                flw(o.lname_);
                if (!o.default_.empty()) flw("={}", o.default_);
                flw(" ");
            }
            flw("[OPTIONS]...").ln()("Options: ").ln();
            auto print_opt = [&](auto& o)
            {
                flw("  {}, {}\t{}", o.sname_, o.lname_, o.description_);
                if (!o.default_.empty()) flw("[={}]", o.default_);
                flw.ln();
            };
            for (auto& o: necessary_) { print_opt(o); }
            for (auto& o: optional_ ) { print_opt(o); }
        }
    }

    void print_usage(void) const
    {
        this->print_usage(std::cout);
    }

    void push(options&& opts)
    {
        for (auto&& o: opts)
        {
            options* list = &(o.necessary_ ? necessary_ : optional_);
            list->push_back(std::move(o));
        }
    }

    void clear(void)
    {
        necessary_.clear();
        optional_.clear();
    }

    template <typename T>
    void exec(T&& out, int argc, const char * const argv[])
    {
        if (argc >= 1) path_ = argv[0];
        if (argc <= 1) this->print_usage(std::forward<T>(out));
        else
        {
            struct ST_opt
            {
                const std::function<handle_t>& hd_;
                std::string cm_;
            };
            std::vector<ST_opt> exec_list;
            size_t c_nec = 0;
            for (int i = 1; i < argc; ++i)
            {
                std::string a = argv[i];
                size_t c = a.find_first_of('=');
                std::string o = a.substr(0, c);
                auto foreach = [&](const auto& cc, auto fr)
                {
                    for (auto it = cc.begin(); it != cc.end(); ++it)
                    {
                        if (o == it->sname_ || o == it->lname_)
                        {
                            exec_list.push_back(ST_opt
                            {
                                it->handle_, 
                                c == std::string::npos ? it->default_ : a.substr(c + 1)
                            });
                            fr();
                        }
                    }
                };
                foreach(necessary_, [&c_nec]{ ++c_nec; });
                foreach(optional_ , []{});
            }
            if (c_nec != necessary_.size() || exec_list.empty()) this->print_usage(std::forward<T>(out));
            else for (auto& e: exec_list)
            {
                e.hd_(*this, e.cm_);
            }
        }
    }

    void exec(int argc, const char * const argv[])
    {
        this->exec(std::cout, argc, argv);
    }
};

} // namespace cmdline
} // namespace capo