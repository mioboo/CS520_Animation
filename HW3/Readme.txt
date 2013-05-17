CSCI 520, Assignment 3

<LI-FU LIN>

================

<Description of what you have accomplished>
Constrained Particle System can work very well. When the last constrain CRING does not implement, I will set the chain to a horizontal position, otherwise, the chain will be set at 3 o'clock position.

When I run my program, I found that the total length of the chain will over its original length. It will become stable if we set the beta value over 0; However, the chain will also unstable if we set the beta value too high.
Besides, you can check constraint equation and the error in the constraints which is in the 520HW3_Graph&Equations.pdf file.

When I run the program by Euler with 13 particles, beta = 1, CRING is enable, alpha can up to 300, it will explode if over this value.

<How to compile>
To compile the program, open the MAC OS terminal, just enter "make" to compile it. 

<How to run and control button>
./jello [Euler or RK4] [number of particles] [alpha value] [beta value] [Able CRING?]

Ex.  ./jello Euler 13 0 1 1

p: pause
space: screen-saver
1: decrease 1 force for x
2: decrease 1 force for y
3: increase 1 force for y
4: increase 1 force for x
5: decrease 1 value for alpha
6: increase 1 value for alpha
7: decrease 0.1 value for beta
8: increase 0.1 value for beta
r: reset the force to original gravity (0,-1,0), alpha to 0, beta to 0, also disable Perlin Noise.
n: enable/disable Perlin Noise

<explain any extra credit that you have implemented.>
There is a time-varying random force field which can able/disable by press n. You can also use number key to change the force field, alpha and beta.
When you press r, everything will be reset.
You can also do the screen shot by press space.
When we run the program, you can choose run it by Euler or by RK4.
The ground of program is kind of checkerboard.
I tried to print the values of force, alpha and beta on screen but I didn't finish it so it will show on terminal, you can watch it when you change the force or other value.




