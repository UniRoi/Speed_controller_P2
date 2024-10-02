
# Part 1
![[images/Pasted image 20240925151227.png]]


# Part 2

Fault detection: entering stop-state after reboot
With each reboot of our system, it immediately transitioned to the stop state. This fault was visualized using an oscilloscope (see image xyz). The issue arises from a temporary loss of current, as the system requires a significant amount of electricity during startup. Consequently, an error is triggered. The state machine detects this fault and transitions directly to the stop state, even if the fault lasts only for a brief moment. The solution was to implement input debouncing, which prevents minor fluctuations from impacting our system.

# Part 3
The PI Controller gets 5 arguments: Kp, Ti, dt, maxOutput and minOutput. Kp and Ti are needed for the tuning of the controller, dt is needed to calculate the integral correctly and maxOutput and minOutput are there for Anti-Windup.

For testing the PI Controller we have 4 Tests: 
- test_PI_control_normal: For testing the normal functionality of the Controller.  
- test_PI_control_saturation_max: For testing the upper limit when the controller has to go above saturation mathematically.  
- test_PI_control_saturation_min: Same for the lower limit.  
- test_PI_control_windup_protection: Testing the upper limit of saturation in the special case when the integral is exceeding it.  

All tests work and pass.