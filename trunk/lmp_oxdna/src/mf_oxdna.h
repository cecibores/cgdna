/* ----------------------------------------------------------------------
   LAMMPS - Large-scale Atomic/Molecular Massively Parallel Simulator
   http://lammps.sandia.gov, Sandia National Laboratories
   Steve Plimpton, sjplimp@sandia.gov

   Copyright (2003) Sandia Corporation.  Under the terms of Contract
   DE-AC04-94AL85000 with Sandia Corporation, the U.S. Government retains
   certain rights in this software.  This software is distributed under
   the GNU General Public License.

   See the README file in the top-level LAMMPS directory.
------------------------------------------------------------------------- */
/* ----------------------------------------------------------------------
   Contributing author: Oliver Henrich (EPCC, University of Edinburgh)
------------------------------------------------------------------------- */

#ifndef MF_OXDNA_H
#define MF_OXDNA_H

#include <math.h>
#include <stdio.h>
#include <string.h>
#include "error.h"

namespace MFOxdna {

  inline double F1(double, double, double, double, double, double, double, double, double, double, double);
  inline double DF1(double, double, double, double, double, double, double, double, double, double);
  inline double F3(double, double, double, double, double, double, double, double &);
  inline double F4(double, double, double, double, double, double);
  inline double DF4(double, double, double, double, double, double);
  inline double F5(double, double, double, double, double);
  inline double DF5(double, double, double, double, double);

}

/* ----------------------------------------------------------------------
   f1 modulation factor 
------------------------------------------------------------------------- */
inline double MFOxdna::F1(double r, double eps, double a, double cut_0,
	double cut_lc, double cut_hc, double cut_lo, double cut_hi, 
	double b_lo, double b_hi, double shift) 
{

  if (r > cut_hc) {
    return 0.0;
  }
  else if (r > cut_hi) {
    return eps * b_hi * (r-cut_hc) * (r-cut_hc);
  }
  else if (r > cut_lo) {
    double tmp = 1 - exp(-(r-cut_0) * a);
    return eps * tmp * tmp - shift;
  }
  else if (r > cut_lc) {
    return eps * b_lo * (r-cut_lc) * (r-cut_lc);
  }
  else {
    return 0.0;
  }

}

/* ----------------------------------------------------------------------
   derivative of f1 modulation factor 
------------------------------------------------------------------------- */
inline double MFOxdna::DF1(double r, double eps, double a, double cut_0,
	double cut_lc, double cut_hc, double cut_lo, double cut_hi, 
	double b_lo, double b_hi) 
{

  if (r > cut_hc) {
    return 0.0;
  }
  else if (r > cut_hi) {
    return 2 * eps * b_hi * (1 - cut_hc / r);
  }
  else if (r > cut_lo) {
    double tmp = exp(-(r-cut_0) * a);
    return 2 * eps * (1 - tmp) * tmp * a / r;
  }
  else if (r > cut_lc) {
    return 2 * eps * b_lo * (1 - cut_lc / r);
  }
  else {
    return 0.0;
  }

}

/* ----------------------------------------------------------------------
   f3 modulation factor, force and energy calculation
------------------------------------------------------------------------- */
inline double MFOxdna::F3(double rsq, double cutsq_ast, double cut_c, 
	double lj1, double lj2, double eps, double b, double & fpair) 
{
  double evdwl = 0.0;

  if (rsq < cutsq_ast) {
    double r2inv = 1.0/rsq;
    double r6inv = r2inv*r2inv*r2inv;
    fpair = r2inv*r6inv*(12*lj1*r6inv - 6*lj2);
    evdwl = r6inv*(lj1*r6inv-lj2);
  }
  else {
    double r = sqrt(rsq);
    double rinv = 1.0/r;
    fpair = 2*eps*b*(cut_c*rinv - 1);
    evdwl = eps*b*(cut_c-r)*(cut_c-r);
  }

  return evdwl;
}

/* ----------------------------------------------------------------------
   f4 modulation factor 
------------------------------------------------------------------------- */
inline double MFOxdna::F4(double theta, double a, double theta_0, 
	double dtheta_ast, double b, double dtheta_c) 
{
  double dtheta = theta-theta_0;

  if (fabs(dtheta) > dtheta_c) {
    return 0.0;
  }
  else if (dtheta > dtheta_ast) {
    return b * (dtheta-dtheta_c)*(dtheta-dtheta_c);
  }
  else if(dtheta > -dtheta_ast) {
    return 1 - a * dtheta*dtheta;
  }
  else {
    return b * (dtheta+dtheta_c)*(dtheta+dtheta_c);
  }

}

/* ----------------------------------------------------------------------
   derivative of f4 modulation factor 
------------------------------------------------------------------------- */
inline double MFOxdna::DF4(double theta, double a, double theta_0,
        double dtheta_ast, double b, double dtheta_c) 
{
  double dtheta = theta-theta_0;

  if (fabs(dtheta) > dtheta_c) {
    return 0.0;
  }
  else if (dtheta > dtheta_ast) {
    return 2*b * (dtheta-dtheta_c) / sin(theta);
  }
  else if (dtheta > -dtheta_ast) {
    return -2*a * dtheta / sin(theta);
  }
  else {
    return 2*b * (dtheta+dtheta_c) / sin(theta);
  } 

}

/* ----------------------------------------------------------------------
   f5 modulation factor 
------------------------------------------------------------------------- */
inline double MFOxdna::F5(double x, double a, double x_ast, 
	double b, double x_c) 
{

  if (x >= 0) {
    return 1.0;
  }
  else if (x > x_ast) {
    return 1 - a * x * x;
  }
  else if (x > x_c) {
    return b * (x-x_c) * (x-x_c);
  }
  else {
    return 0.0;
  }
}

/* ----------------------------------------------------------------------
   derivative of f5 modulation factor 
------------------------------------------------------------------------- */
inline double MFOxdna::DF5(double x, double a, double x_ast, 
	double b, double x_c) 
{
  if(x >= 0) {
    return 0.0;
  }
  else if (x > x_ast) {
    return -2 * a * x;
  }
  else if(x > x_c) {
    return 2 * b * (x-x_c);
  }
  else {
    return 0.0;
  }

return 0;
}
#endif