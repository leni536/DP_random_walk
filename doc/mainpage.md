\mainpage

Compilation
====

The program is mainly developed on a Linux Debian distribution. 
Compilation depends on the 
  *libarmadillo-dev*
  *libboost-program-options-dev*
  and *libboost-random-dev* packages.
Running a single *make* command compiles the program to the path *bin/main*.

Usage
===

The program can be operated with command line options,
  it allows scripting large number of simulations with different parameters.

```
$ bin/main --help
Allowed options:
  -h [ --help ]                      produce help message
  -v [ --version ]                   print version number
  --autocorr                         set autocorr measurement
  --spins arg (=10)                  set number of spins
  --duration arg (=300)              set simulation duration
  --timestep arg (=1)                set timestep
  --omega arg (=0.20000000000000001) set the absolute value of Larmor 
                                     precession
  --delta_omega arg (=0)             set the width of omega distribution
  --seed arg (=rand)                 set the seed for the random generator
  -o [ --output ] arg (=-)           output file path
  -m [ --model ] arg (=naiv)         name of the model
  --meas arg (=prep)                 name of measurement method
  -b [ --B_meas ] arg (=0)           measurement field
  --tmin arg (=0)                    starting time, B_meas turns in at t=0
```

As there are default arguments for all options (as seen in the help output), with no arguments the program outputs the result of a sample simulation.
The output contains a header which contains the simulation parameters so the simulation can be reproduced.
After the header it outputs the spin component of interest in the function of time.
The time interval and sampling interval is the same as specified in the command line.

```
$ bin/main | head -20
# Djakonov-Perel simulation
# t=0 Sz=1, no magnetic field
#  version: commit_17cc1d6e5275608347fa311f7f7e64735470c311
#  spins: 10
#  duration: 300
#  timestep: 1
#  omega: 0.2
#  seed: 2494990189
#  model: naiv
#  meas: prep
#  B_meas: 0
#  tmin: 0
#  autocorr: false
# t, Sz
0, 1
1, 0.990826
2, 0.97055
3, 0.946542
4, 0.925218
5, 0.910056
```

Command line options
===

We can see from the output of "bin/main --help" that there are many options regarding to a single run of the simulation.
Here I describe the details how these options work.

+ **help** Outputs the brief description of command line options.
+ **version** Outputs the version number.
  If the program is compiled in a clean state git repository then it is the commit hash of the given version.
+ **duration** Sets the simulation time window's length.
+ **timestep** Sets the interval between time samples.
+ **tmin** Sets the starting time of the measurement, it is useful for measurements with step function external magnetic field, where it is turned on at t=0. 
+ **omega** The primary parameter of the underlying SOC model.
  It is typically the amplitude of the Larmor angular frequency due to the SOC field.
  The exact meaning depends on the model in question.
+ **delta_omega** 
  The secondary paramter for the SOC.
  It typically describes the deviation of amplitude of the Larmor angular frequency.
  The exact meaning is model dependent.
+ **model**
  The name of the spin model selected.
  It also selects the initial condition and the spin direction of interest.
  The available models:
    - *naiv* 
      Fully isotropic 3D SOC model,
        we assume a spherical Fermi-surface.
      \f[
        H_\mathrm{SOC}
        = \hbar \Omega \frac{\bm{k} \bm{s}}{k_\mathrm{F}}
      \f]
    - *burkov_2d*
      2DEG model with Rashba SOC, z axis relaxation. The shape of the Fermi surface is a circle.
      \f[
        H_\mathrm{SOC}
        = \hbar \Omega \frac{(s_x k_y - s_y k_x)}{k_\mathrm{F}}
      \f]
    - *burkov_2d_Sx*
      2DEG model with Rashba SOC, x axis relaxationa.
    - *burkov_2d_angle*
      2DEG model with Rashba SOC, spins started polarized at a 45° angle to the z axis, z component gathered.
    - *burkov_2d_angle_sx*
      2DEG model with Rashba SOC, spins started polarized at a 45° angle to the z axis, x component gathered.
    - *rashba_3d*
      3D model with Rashba SOC.
      The SOC Hamiltonian is the same as in *burkov_2d*, but now we allow wave numbers in the z direction.
      We assume a spherical Fermi-surface.
    - *mixed_3d*
      3D model with both isotropic and Rashba SOC.
    - *mn_1d*
      1D model which exactly gives back the result of motional narrowing.
      The "Fermi-surface" is two points \f$ k=\pm k_\mathrm{F} \f$.
      We start the spin from the z axis, the SOC field is perpendicular to the z axis.
    - *dresselhaus*
      3D model with dresselhaus SOC, z axis relaxation.
    - *dresselhaus_xy*
      3D model with dresselhaus SOC, spins started with x direction polarization, y component gathered.
    - *rashba_dressel_2d_z*
      2DEG model with both rashba and dresselhaus SOC, z axis relaxation.
    - *rashba_dressel_2d_x*
      2DEG model with both rashba and dresselhaus SOC, x axis relaxation.
    - *rashba_dressel_2d_xy*
      2DEG model with both rashba and dresselhaus SOC, spins started with x direction polarization, y component gathered.

+ **autocorr**: If this option is set, then <s(t+tau)s(t)> time averaged autocorrelation is measured for a single spin.
  At this point the meaning of the options **spins** and **duration** changes.
  **duration** becomes the time window from where we take the tau values, **timestep** is the sampling interval for the **tau** values.
  The simulation is actually ran for **duration** × **spins** time, so the resulting graph has similar errors as an ensamble measurement.
+ **seed** The random seed of the random generator.
  Potentially useful for debugging as a fix value guarantees deterministic runs.
+ **output** The output file for the simulation, default is stdout.
# TODO
