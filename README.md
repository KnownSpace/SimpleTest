# SimpleTest
a simple unit test library

## Usage:

```c++

#include <simpletest/TestRunner.hpp>

class MyTest:public simpletest::ITypenamedTest<MyTest>
{
public:
    MyTest() noexcept = default;

    ~MyTest() noexcept = default;

    virtual simpletest::TestResult Run() noexcept override
    {
        int i = 0;
        if(i != 0)
        {
            return this->Fail("i should equal with 0");
        }
        //or this->Assert(i == 0,"i should equal with 0");
        return this->Success();
    }
};

int main()
{
    simpletest::TestRunner runner;
    runner.Register<MyTest>();
    return runner.Run();
}

```