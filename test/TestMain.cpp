#include "TestBase.h"

int main(int argc, char **argv) {
    TestSuite &suite = TestSuite::create();
    
    suite.startTest();

    printf("Total  test cases %d\n", suite.total());
    printf("Passed test cases %d\n", suite.passed());
    printf("Failed test cases %d\n", suite.failed());

    return 0;
}
