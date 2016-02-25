	program derivatives
c--------------------------------------------------------------------
c This program drives function 'der_num' to calculate first and
c second order derivatives of the function y = exp(x)*sin(x)
c--------------------------------------------------------------------
c Statement functions: f1 = first derivative, f2 = second derivative
c
	f1(xinput) = exp(xinput)*(sin(xinput)+cos(xinput))
	f2(xinput) = 2.0*exp(xinput)*cos(xinput)
c--------------------------------------------------------------
c Declaration of variables
c
	real x, dx, diff1, diff2
	integer iorder
	character answer
c--------------------------------------------------------------
c Request input data from user
c
100	print*
	print*,"Calculation of derivatives"
	print*,"--------------------------"
	print*,"Enter a value of x:"
	read *,x
	print*,"Enter a value of dx:"
	read *,dx
c--------------------------------------------------------------
c First and second derivative
c
	diff1 = der_num(x,dx,1)
	diff2 = der_num(x,dx,2)
c--------------------------------------------------------------
c Print results
c
	print*
	print*,
     1  "==========================================================="
	print*, 
     1  "Numerical calculation of first and second order derivatives" 
	print*,
     1  "==========================================================="
        print*,"first derivative         = ",diff1
	print*,"actual first derivative  = ",f1(x)
	print*,"second derivative        = ",diff2
	print*,"actual second derivative = ",f2(x)
	print*,
     1  "==========================================================="
	print*
c--------------------------------------------------------------
c Check if more derivatives need to be calculated
c
200	print*,"Do you want to calculate more derivatives? (yes/no)"
	read *,answer
	if ((answer.eq."y").or.(answer.eq."Y"))go to 100
	if ((answer.eq."n").or.(answer.eq."N"))stop
	go to 200
c--------------------------------------------------------------
c End program
c
	end 
c===============================================================
	function der_num(x0,dx,order)
c--------------------------------------------------------------
c This Fortran function calculates the first or second order
c derivative of the function f(x) defined below. The argument 'order'
c can take values of 1 (1st order) or 2 (2nd order). x0 is the
c point about which the derivative is taken, and dx is the 
c increment in the independent variable to calculate the 
c derivative.
c--------------------------------------------------------------
c Statement function
c
  	f(x) = exp(x)*sin(x)
c--------------------------------------------------------------
c Declaration of variables
c
	real x0, dx
	integer order
c--------------------------------------------------------------
c If order is not 1 nor 2, return a zero and print warning
c
	if ((order.ne.1).and.(order.ne.2))then
	print*,"Function der_num - order must be either 1 or 2"
		der_num = 0
		return
	end if
c--------------------------------------------------------------
c Calculate derivative according to order = 1 or 2
c
	if (order.eq.1) then
		der_num = (f(x0+dx)-f(x0-dx))/(2*dx)
	else
		der_num = (f(x0+dx)-2*f(x0)+f(x0-dx))/dx**2
	end if
	return
	end
c===============================================================

