#include "TestBase.h"

TestSuite *TestSuite::sInst_ = nullptr;

TestSuite& TestSuite::create() {
    if (sInst_ == nullptr) {
        sInst_ = new TestSuite();
    }
    return *sInst_;
}
