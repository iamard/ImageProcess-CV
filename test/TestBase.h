#ifndef __TEST_BASE_H__
#define __TEST_BASE_H__

#include "Common.h"

class TestCase {
public:
    enum Status {
        TEST_PASSED,
        TEST_FAILED
    };

    TestCase(string testName, string descriptor)
        : testStatus_(TEST_PASSED),
          testName_(testName),
          descriptor_(descriptor),
          message_("") {
    }

    TestCase(const TestCase &other)
        : testStatus_(other.testStatus_),
          testName_(other.testName_),
          descriptor_(other.descriptor_),
          message_(other.message_) {
    }

    Status status() {
        return testStatus_;        
    }
    
    string message() {
        return message_;
    }
    
    virtual void start() = 0;

    TestCase& operator=(const TestCase &other) {
        testStatus_ = other.testStatus_;
        testName_   = other.testName_;
        descriptor_ = other.descriptor_;
        message_    = other.message_;
        return *this;
    }

protected:
    void verify(bool state, const string &message) {
        if (state == false) {
            message_    = message;
            testStatus_ = TEST_FAILED;
        }
    }

private:
    Status testStatus_;
    string testName_;
    string descriptor_;
    string message_;
};

class TestSuite {
public:
    static TestSuite& create();

    void addTest(TestCase &test) {
        tests_.push_back(&test);
    }

    void startTest() {
        for (int32_t index = 0; index < tests_.size(); index++) {
            tests_[index]->start();
            if (tests_[index]->status() != TestCase::TEST_PASSED) {
                printf("%s\n", tests_[index]->message());
                failed_++;
            } else {
                passed_++;
            }
        }
    }
    
    int32_t total() const {
        return tests_.size();
    }

    int32_t passed() const {
        return passed_;
    }

    int32_t failed() const {
        return failed_;
    }
    
private:
    TestSuite()
        : passed_(0),
          failed_(0) {
    }

    static TestSuite *sInst_;

    vector<TestCase*> tests_; 
    int32_t           passed_;
    int32_t           failed_;
};

#define TEST_CASE(name, desc)                  \
class Test##name: public TestCase              \
{                                              \
    static Test##name sInst;                   \
                                               \
    Test##name();                              \
                                               \
    void start();                              \
};                                             \
                                               \
Test##name Test##name::sInst;                  \
                                               \
Test##name::Test##name()                       \
    : TestCase(#name, desc) {                  \
    TestSuite::create().addTest(*this);        \
}                                              \
                                               \
void Test##name::start()

#endif  // __TEST_BASE_H__
