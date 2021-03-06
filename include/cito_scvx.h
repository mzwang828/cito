/*! Successive Convexification */
/**
 *  \brief CitoSCvx class consists of functions to run the SCvx algorithm
 *
 *  This class defines functions that are used to roll-out the dynamics,
 *  evaluate the cost, and execute the SCvx algorithm.
 *
 *  \author Aykut Onol
 */


#ifndef CITO_SCVX_H
#define CITO_SCVX_H

#include <chrono>
#include "cito_numdiff.h"
#include "cito_sqopt.h"


class CitoSCvx
{
public:
    /// Constructor
    CitoSCvx(const mjModel* model);
    /// Destructor
    ~CitoSCvx() {}
    /// This function returns the nonlinear cost given control trajectory and final state
    double getCost(const stateVec XFinal, const ctrlTraj U);
    /// This function rolls-out and linearizes the dynamics given control trajectory
    trajectory runSimulation(const ctrlTraj U0, bool linearize, bool save);
    /// This function executes the successive convexification algorithm
    ctrlTraj solveSCvx(const ctrlTraj U);

private:
    /// MuJoCo model
    const mjModel* m;
    /// SCvx parameters
    int maxIter;                        // maximum number of iterations
    double *J, *JTemp, *JTilde,         // cost terms
           *r, *dJ, *dL, *rho,          // trust region radius, change, and similarity
           dLTol,                       // stopping criteria in terms of dL
           rho0, rho1, rho2,            // similarity thresholds
           beta_expand, beta_shrink,    // trust-region expand and shrink factors
           rMin, rMax;                  // trust-region radius limits
    bool *accept, dLTolMet = false, stop = false;
    /// Trajectories
    stateTraj XSucc, dX, XTilde;    ctrlTraj USucc, dU, UTemp;
    stateDerTraj Fx;                ctrlDerTraj Fu;
    trajectory traj, trajS, trajTemp;
    /// Cost function variables
    double weight[4];
    int controlJointDOF0;
    Eigen::Matrix<double,  6, 1> desiredPose, finalPose;
    Eigen::Matrix<double, NV, 1> desiredVelo, finalVelo;
    KConTraj KCon;
    double KConSN;          // total squared norm of virtual stiffness variables
    double Jf, Ji, Jt;      // final, integrated, and total cost values
    /// Control, numerical differentiation, and SQOPT objects
    CitoControl cc;
    CitoNumDiff nd;
    CitoSQOPT   sq;
};

#endif //CITO_SCVX_H
