
# Part 1
![[images/Pasted image 20240925151227.png]]


# Part 2



# Part 3
The PI Controller gets 5 arguments: Kp, Ti, dt, maxOutput and minOutput. Kp and Ti are needed for the tuning of the controller, dt is needed to calculate the integral correctly and maxOutput and minOutput are there for Anti-Windup.

For testing the PI Controller we have 4 Tests: 
- test_PI_control_normal: For testing the normal functionality of the Controller.  
- test_PI_control_saturation_max: For testing the upper limit when the controller has to go above saturation mathematically.  
- test_PI_control_saturation_min: Same for the lower limit.  
- test_PI_control_windup_protection: Testing the upper limit of saturation in the special case when the integral is exceeding it.  

All tests work and pass.