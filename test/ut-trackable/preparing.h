#pragma once

#include "capo/trackable.hpp"
#include "capo/output.hpp"

namespace ut_trackable_ {

class XXX : public capo::trackable<XXX>
{
public:
    const char* name_;

    XXX(const char* name) : name_(name)
    {
        printf("XXX create: %s\n", name_);
    }
    XXX(const XXX&)
    {
        printf("XXX copy\n");
    }
    XXX(XXX&&)
    {
        printf("XXX move\n");
    }
    ~XXX(void)
    {
        printf("XXX destory: %s\n", name_);
    }
};

} // namespace ut_trackable_
