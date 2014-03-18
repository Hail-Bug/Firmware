/****************************************************************************
 *
 *   Copyright (c) 2013 PX4 Development Team. All rights reserved.
 *   Author: 	@author Thomas Gubler <thomasgubler@gmail.com>
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 * 3. Neither the name PX4 nor the names of its contributors may be
 *    used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 ****************************************************************************/


/**
 * @file mTecs.h
 *
 * @author Thomas Gubler <thomasgubler@gmail.com>
 */


#ifndef MTECS_H_
#define MTECS_H_

#include "mTecs_blocks.h"

#include <controllib/block/BlockParam.hpp>
#include <drivers/drv_hrt.h>

namespace fwPosctrl
{

/* Main class of the mTecs */
class mTecs : public control::SuperBlock
{
public:
	mTecs();
	virtual ~mTecs();

	/*
	 * Control in altitude setpoint and speed mode
	 */
	void updateAltitudeSpeed(float flightPathAngle, float altitude, float altitudeSp, float airspeed, float airspeedSp);

	/*
	 * Control in flightPathAngle setpoint (flollow a slope etc.) and speed mode
	 */
	void updateFlightPathAngleSpeed(float flightPathAngle, float flightPathAngleSp, float airspeed, float airspeedSp);

	/*
	 * Control in flightPathAngle setpoint (flollow a slope etc.) and acceleration mode (base case)
	 */
	void updateFlightPathAngleAcceleration(float flightPathAngle, float flightPathAngleSp, float airspeed, float accelerationLongitudinalSp);

	/*
	 * Reset all integrators
	 */
	void resetIntegrators();


	/* Accessors */
	bool getEnabled() {return _mTecsEnabled.get() > 0;}
	float getThrottleSetpoint() {return _throttleSp;}
	float getPitchSetpoint() {return _pitchSp;}

protected:
	/* parameters */
	control::BlockParamInt _mTecsEnabled;		/**< 1 if mTecs is enabled */

	/* control blocks */
	BlockFFPILimited _controlTotalEnergy;		/**< FFPI controller for total energy control: output is throttle */
	BlockFFPILimited _controlEnergyDistribution;	/**< FFPI controller for energy distribution control: output is pitch */
	BlockPLimited	_controlAltitude;		/**< P controller for altitude: output is the flight path angle setpoint */
	BlockPLimited	_controlAirSpeed;			/**< P controller for airspeed: output is acceleration setpoint */

	/* Other calculation Blocks */
	control::BlockDerivative _airspeedDerivative;

	/* Output setpoints */
	float _throttleSp;				/**< Throttle Setpoint from 0 to 1 */
	float _pitchSp;					/**< Pitch Setpoint from -pi to pi */

	/* Time measurements */
	hrt_abstime timestampLastIteration;		/**< Saves the result of hrt_absolute_time() of the last iteration */

	bool _firstIterationAfterReset;		/**< True during the first iteration after a reset */

};

} /* namespace fwPosctrl */

#endif /* MTECS_H_ */
