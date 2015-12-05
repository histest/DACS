#ifndef __EARTH_H
#define __EARTH_H

class CEarth
{
public:
	double		Rei[3][3];
	double		PrecessionZeta,PrecessionZi,PrecessionThet;
	double		TrueObliquityEcliptic,NutationInLongitude,MeanObliquityEcliptic;
	double		GreenwichAngle;
	double		JulianCentury;
public:
	void		Setup(const double MechanicsTime );
	void		Step(const double TimeStep);
	void		SetRei();
};

#endif
