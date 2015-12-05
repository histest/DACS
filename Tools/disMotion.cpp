#include "disMotion.h"
#include "UserMechanics.h"

void CMotionAgent::Step(void)
{
	this->llProcessTime += this->ulSynchronousTime;
	AddTimeSpan(&this->stcGMT,this->ulSynchronousTime*1000);
	this->usCurrentNo ++;
	if ( this->usCurrentNo >= this->usOutputTotalNo )
	{
		this->bOut = true;
		this->usCurrentNo = 0;
	}
	else
		this->bOut = false;
}
unsigned long CMotionAgent::ShiftTimeStep(signed char scRatio)
{
	if ( scRatio > 0 )
	{
		this->ulTimeStep *= scRatio;
		this->ulSynchronousTime *= scRatio;
	}
	else
	{
		this->ulTimeStep /= -scRatio;
		this->ulSynchronousTime /= -scRatio; 
	}
	return this->ulSynchronousTime;
}