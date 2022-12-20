#pragma once
#ifndef _SIMPLETEST_TESTRUNNER_HPP
#define _SIMPLETEST_TESTRUNNER_HPP

#include <vector>
#include <memory>
#include <type_traits>
#include <new>
#include <cassert>

#include "ITest.hpp"

namespace simpletest
{
    class TestRunner
    {
    private:
        using Self = simpletest::TestRunner;
    
        std::vector<std::unique_ptr<simpletest::ITest>> tests_;
    public:
    
        TestRunner() noexcept = default;
    
        TestRunner(const Self &other) = delete;
    
        TestRunner(Self &&other) noexcept = default;
    
        Self &operator=(const Self &other) = delete;
    
        inline Self &operator=(Self &&other) noexcept
        {
            if(this != std::addressof(other))
            {
                this->tests_ = std::move(other.tests_);
            }
            return *this;
        }
    
        ~TestRunner() noexcept = default;
    
        inline const Self &Const() const noexcept
        {
            return *this;
        }

        template<typename _Test,typename ..._Args,typename _Check = decltype(_Test{std::declval<_Args>()...},std::declval<simpletest::ITest*&>() = std::declval<_Test*>())>
        inline void Register(_Args &&...args)
        {
            simpletest::ITest *test{new (std::nothrow) _Test{std::forward<_Args>(args)...}};
            if(!test)
            {
                //bad alloc
                std::terminate();
            }
            std::unique_ptr<simpletest::ITest> testPtr{test};
            try
            {
                this->tests_.emplace_back(std::move(testPtr));
            }
            catch(const std::bad_alloc &fault)
            {
                (void)fault;
                std::terminate();
            }
        }

        inline int Run(FILE *fp) noexcept
        {
            for(std::size_t i = 0;i != this->tests_.size();++i)
            {
                simpletest::ITest *test{this->tests_[i].get()};
                assert(test != nullptr);
                std::fprintf(fp,"[%zu/%zu]Testing %s...",i + 1,this->tests_.size(),test->Name().c_str());
                std::fflush(fp);
                simpletest::TestResult result{test->Run()};
                if(result.Success())
                {
                    std::fputs("Passed\n",fp);
                }
                else
                {
                    std::fprintf(fp,"Failed - %s\n",result.Reason().c_str());
                    return -1;
                }
            }
            std::fprintf(fp,"All tests(%zu) passed\n",this->tests_.size());
            return 0;   
        }

        inline int Run() noexcept
        {
            return this->Run(stdout);
        }
    };   
}

#endif