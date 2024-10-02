#include <speed_control.h>
#include "unity.h"


// Test for P_control class
void test_P_control_update() {
    Controller* p = new P_control(2.0);  // Kp = 2.0
    double ref = 10.0;
    double actual = 5.0;

    double output = p->update(ref, actual);
    // Expected output: (10 - 5) * 2 = 10
    TEST_ASSERT_EQUAL_FLOAT(10.0, output);

    delete p;  // Cleanup
}

// Test for PI_control normal operation
void test_PI_control_normal() {
    Controller* pi = new PI_control(1.0, 2.0, 0.1, 100.0, -100.0);  // Kp = 1.0, Ti = 2.0, dt = 0.1, max output = 100.0, min output = -100.0

    double ref = 10.0;
    double actual = 5.0;

    double output = pi->update(ref, actual);

    // Proportional part: Kp * (ref - actual) = 1.0 * (10 - 5) = 5.0
    // Integral part (first iteration): (1 / Ti) * (error * dt) = (1 / 2.0) * (5 * 0.1) = 0.25
    // Total output = 5.0 + 0.25 = 5.25
    TEST_ASSERT_EQUAL_FLOAT(5.25, output);

    delete pi;  // Cleanup
}

// Test for PI_control saturation at max output
void test_PI_control_saturation_max() {
    Controller* pi = new PI_control(1.0, 1.0, 0.1, 10.0, -10.0);  // Kp = 1.0, Ti = 1.0, dt = 0.1, max output = 10.0, min output = -10.0

    double ref = 20.0;
    double actual = 5.0;

    double output = pi->update(ref, actual);

    // Without saturation, the output would be larger than maxOutput
    TEST_ASSERT_EQUAL_FLOAT(10.0, output);  // Should saturate at maxOutput

    delete pi;  // Cleanup
}

// Test for PI_control saturation at min output
void test_PI_control_saturation_min() {
    Controller* pi = new PI_control(1.0, 1.0, 0.1, 10.0, -10.0);  // Kp = 1.0, Ti = 1.0, dt = 0.1, max output = 10.0, min output = -10.0

    double ref = -20.0;
    double actual = 5.0;

    double output = pi->update(ref, actual);

    // Without saturation, the output would be smaller than minOutput
    TEST_ASSERT_EQUAL_FLOAT(-10.0, output);  // Should saturate at minOutput

    delete pi;  // Cleanup
}

// Test for PI_control windup protection
void test_PI_control_windup_protection() {
    Controller* pi = new PI_control(1.0, 2.0, 0.1, 10.0, -10.0);  // Kp = 1.0, Ti = 2.0, dt = 0.1, max output = 10.0, min output = -10.0

    double ref = 100.0;
    double actual = 0.0;

    double output = pi->update(ref, actual);
    TEST_ASSERT_EQUAL_FLOAT(10.0, output);  // Saturated at maxOutput

    // Test if integral windup is prevented
    double output2 = pi->update(ref, actual);  // With windup protection, output should remain at maxOutput
    TEST_ASSERT_EQUAL_FLOAT(10.0, output2);

    delete pi;  // Cleanup
}

int main(int argc, char **argv) {
    UNITY_BEGIN();

    // Run all tests
    RUN_TEST(test_P_control_update);
    RUN_TEST(test_PI_control_normal);
    RUN_TEST(test_PI_control_saturation_max);
    RUN_TEST(test_PI_control_saturation_min);
    RUN_TEST(test_PI_control_windup_protection);

    UNITY_END();
    return 0;
}
