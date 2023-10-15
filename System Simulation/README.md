# System Simulation
## Goal
The goal of this program is to simulate a continuous system simulation by simulating a simple biological model including predators and prey.

## The Program
### General Information
This simulation model is uses this logistic growth equation:  

𝑑𝑁/𝑑𝑡 = 𝜆 ∙ 𝑁 ∙ (1 − 𝑁/𝑁𝑚𝑎𝑥)  

where 𝑁 is the mass of some biological species (e.g. biomass of a deer population) and 𝜆 is a constant
controlling the growth of the biomass. For example, if 𝑁max = 1000 and 𝜆 is 2.0, this equation says the
change of the biomass is determined by a growth factor 2𝑁 and a suppression term -(𝑁^2 / 500). These
terms roughly correspond to an average birth rate and death rate for a population.  

𝑑𝑁1/𝑑𝑡 = 𝜆1 ∙ 𝑁1 ∙ (1 − (𝛼 ∙ 𝑁2))  
𝑑𝑁2/𝑑𝑡 = -𝜆2 ∙ 𝑁2 ∙ (1 − (𝛽 ∙ 𝑁1))  

For this simulation, we use this equation multiple times for multiple populations (prey and predator, respectively). These equations are widely known as Lotka-Volterra equations which describe the dynamics between predator and prey populations. Examining the first equation, we see the change in the amount of prey (𝑁1) is controlled by a growth term (𝜆1 ∙ 𝑁1) that depends on the current amount of prey and a suppression term (𝜆1 ∙ 𝛼 ∙ 𝑁1 ∙ 𝑁2) that depends on the product of the amount of predator and prey. As the amount of predator increase, the rate of change in the amount of prey will become negative and the amount of prey is reduced. Similarly, the second equation is governed by two terms. The growth term (𝜆2 ∙ 𝛽 ∙ 𝑁1 ∙ 𝑁2) says that the rate of increase in the amount of predator grows as the amount of prey increases. The suppression term (−𝜆2 ∙ 𝑁2) adjusts the rate of change in the amount of predator in proportion to the current amount of predator.   

This system is in equilibrium when d𝑁1/dt = 0 and d𝑁2/dt = 0. If the system is not in equilibrium (e.g. d𝑁1/dt < 0 and d𝑁2/dt > 0), the biomass of one population will drop and the other will rise. At some point, one of the suppression terms will start to dominate its equation and the sign of the derivate will change causing the mass of the populations to move in the opposite direction. The net result is the system oscillates. 

Additionally, this system contains a third factor (named Pesticide) which is applied to the system once, decays at a constant rate, and kills some amount of both biomass populations over time. This killing effect is proportional to the amount of pesticide at a given time instant.

###Results
Below are the results of the simulation run with slightly different parameters. These parameters affect whether the system is stable or unstable.  

####Unstable Ecosystem
![Unstable Plot 01](https://github.com/AMoore86/Portfolio/blob/main/System%20Simulation/UnstablePlot01.png)
![Unstable Plot 01](https://github.com/AMoore86/Portfolio/blob/main/System%20Simulation/UnstablePlot02.png)
![Unstable Plot 01](https://github.com/AMoore86/Portfolio/blob/main/System%20Simulation/UnstablePlot03.png)

####Stable Ecosystem
![Unstable Plot 01](https://github.com/AMoore86/Portfolio/blob/main/System%20Simulation/StablePlot01.png)
![Unstable Plot 01](https://github.com/AMoore86/Portfolio/blob/main/System%20Simulation/StablePlot02.png)
![Unstable Plot 01](https://github.com/AMoore86/Portfolio/blob/main/System%20Simulation/StablePlot03.png)  

The result of this project are further discussed in the corresponding report: [Report Link](https://github.com/AMoore86/Portfolio/blob/main/System%20Simulation/ProjectReport.pdf) 