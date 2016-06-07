:Begin:
:Function:	test
:Pattern:	myTest[i_Integer]
:Arguments:	{i}
:ArgumentTypes:	{Integer}
:ReturnType:	Manual
:End:

:Begin:
:Function: 	simulation
:Pattern:	simulation[Omega_Real,DeltaOmega_Real,ModelString_String,MeasurementString_String,AutocorrString_String,MagneticField_Real,Tmin_Real,Duration_Real,Timestep_Real,SpinNumber_Integer]
:Arguments:	{Omega,DeltaOmega,ModelString,MeasurementString,AutocorrString,MagneticField,Tmin,Duration,Timestep,SpinNumber}
:ArgumentTypes:	{Real,Real,String,String,String,Real,Real,Real,Real,Integer}
:ReturnType:	Manual
:End:
