
//
//					  m_pASTCurrentNode
//						     |
//					  ASTNode_FUNCTIONDEF
//			    _____________|_____________
//			   |						   |
//  ASTNode_FUNCTIONRETURNTYPE		ASTNode_FUNCTIONARGLIST
//  			 |							 |
//  ASTNode_PRIMITIVETYPEINT				 |
//	    		OR							 |
//  ASTNode_PRIMITIVETYPESTRING				 |
//		    						_________|______________________...
//			    				   |					|		   |...
//				    ASTNode_PRIMITIVETYPEINT  ASTNode_PRIMITIVETYPESTRING
//

int retNewFunc()
{
	print("In retNewFunc................");
	putc(10);

	int j = 0;
	for(int i = 0, j = 1, int iCount = 1; i < 10; i = i+1, iCount = iCount + 1)
	{
		print("ForLoop i = ", i, " ,val of j = ", j, " ,iCount = ", iCount);
		putc(10);
	}
	
	return 1977;
}

int retFunc()
{
	print("In retFunc................");
	putc(10);
	
	return retNewFunc();
}

void tempFunc(int arg0, int arg1)
{
	print("In tempFunc()");
	putc(10);
		
	int iRet = 10;
	iRet = (10 + retFunc()) + 10;
	print("iRet = ", iRet);
	putc(10);
	
	if(((retFunc() + 10) - 1900) == 87)
	{
		print("retFunc returned 1977")
		putc(10);
	}
	
	int iRet1 = 101 + retFunc();
	print("iRet1 = ", iRet1);
	putc(10);
	
	int j = 111;
	j = j + arg0 + retFunc();
	print("tempFunc() args = ", j, ", ", arg1);
	putc(10);
	
	int iCount = 1970;
	while(iCount <= retFunc())
	{
		iCount = iCount + 1;
		print("In While = ", iCount);
		putc(10);
	}
	
	if(j < 10)
	{
		j = 10;
		int k = 2;
		k = 3;
	}
	else
	{
		int k = 11;
		while(j == 11)
		{
			int j = 1111;
			j = j + 1;
			
			arg0 = arg0 - 1;
		}
		
		k = j * 2;
	}
	
	print("End of tempFunc()");
	putc(10);
}

void testPrinting()
{
	/*** test printing, embedded \n and comments with lots of '*' ***/
	print(42);
	print("\nHello World\nGood Bye\nok\n");
	print("Print a slash n - \\n.\n");
}

void simpleCount()
{
	int count = 1;
	while (count < 10) {
		print("count is: ", count, "\n");
		putc(10);
		count = count + 1;
	}
}

void _100Doors()
{
	/* 100 Doors */
	int i = 1;
	while (i * i <= 100) {
		print("door ", i * i, " is open\n");
		putc(10);
		i = i + 1;
	}
}

void negativeTest()
{
	print("negativeTest : ");
	putc(10);
	
	int a = (-1 * ((1 * (5 * 15)) / 10));
	print(a);
	putc(10);
	
	int b = -a + (3 * 4);
	print("b = ", b);
	putc(10);
	
	print("-b = ", -b);
	putc(10);
	
	print("-(1) = ", -(1));
	putc(10);
	
	int i = a;
	print("i = a = ", a);
	putc(10);

	i = ++a + ++a + ++a;
	print("i = ", i, " , a = ", a);
	putc(10);
	
	i = --a + --a + --a;
	print("i = ", i, " , a = ", a);
	putc(10);
	
	i = a++ + a++ + a++;
	print("i = ", i, " , a = ", a);
	putc(10);
    
	i = a-- + a-- + a--;
	print("i = ", i, " , a = ", a);
	putc(10);
}

void deep()
{
	print("In deep");
	putc(10);
	
//	print(---------------------------------+++5, "\n");
	print("Deep enough = ", ((((((((3 + 2) * ((((((2))))))))))))));
	putc(10);
 
	if (1) 
	{ 
		if (1) 
		{ 
			if (1) 
			{ 
				if (1) 
				{ 
					if (1) 
					{ 
						print(15); 
						putc(10);
					} 
				} 
			} 
		} 
	}
	
	print("End deep");
	putc(10);
}

void gcd()
{
	/* Compute the gcd of 1071, 1029:  21 */
 
	int a = 1071;
	int b = 1029;
	print("gcd of ", a, " & ", b, " = ");
	
	while (b != 0) {
		int new_a = b;
		b     = a % b;
		a     = new_a;
	}
	
	print(a);
	putc(10);
}

void factorial()
{
	/* 12 factorial is 479001600 */
 
	int n = 12;
	int result = 1;
	int i = 1;
	while (i <= n) {
		result = result * i;
		i = i + 1;
	}
	
	print("Factorial of ", n, " = ", result);
	putc(10);
}

void fibonacci()
{
	/* fibonacci of 44 is 701408733 */
 
	int n = 44;
	int i = 1;
	int a = 0;
	int b = 1;
	int w = 0;
	while (i < n) {
		int w = a + b;
		a = b;
		b = w;
		i = i + 1;
	}
	print(w, "\n");
	putc(10);
}

void fizzBuzz()
{
	/* FizzBuzz */
	int i = 1;
	while (i <= 100) {
		if (!(i % 15))
		{
			print("FizzBuzz");
			putc(10);
		}
		else 
		{
			if (!(i % 3))
			{
				print("Fizz");
				putc(10);
			}
			else 
			{
				if (!(i % 5)) 
				{
					print("Buzz");
					putc(10);
				}
				else 
				{
					print(i);
					putc(10);
				}
			}
		}
	
		print("\n");
		putc(10);
		i = i + 1;
	}
}

void _99BottlesOfBeer()
{
	/* 99 bottles */
	int bottles = 99;
	while (bottles > 0) {
		print(bottles, " bottles of beer on the wall\n");
		putc(10);
		print(bottles, " bottles of beer\n");
		putc(10);
		print("Take one down, pass it around\n");
		putc(10);
		bottles = bottles - 1;
		print(bottles, " bottles of beer on the wall\n\n");
		putc(10);
	}
}

void primes()
{
	/*
	Simple prime number generator
	*/
	int count = 1;
	int n = 1;
	int limit = 100;
	while (n < limit) {
		int k=3;
		int p=1;
		n=n+2;
		while ((k*k<=n) && (p)) {
			p=n/k*k!=n;
			k=k+2;
		}
		if (p) {
			print(n, " is prime\n");
			putc(10);
			count = count + 1;
		}
	}
	print("Total primes found: ", count, "\n");
	putc(10);
}

void AsciiMandlebrot()
{
/*
 This is an integer ascii Mandelbrot generator
 */
    int left_edge   = -420;
    int right_edge  =  300;
    int top_edge    =  300;
    int bottom_edge = -300;
    int x_step      =    7;
    int y_step      =   15;
 
    int max_iter    =  200;
 
    int y0 = top_edge;
    while (y0 > bottom_edge) {
        int x0 = left_edge;
        while (x0 < right_edge) {
            int y = 0;
            int x = 0;
            int the_char = ' ';
            int i = 0;
            while (i < max_iter) {
                int x_x = (x * x) / 200;
                int y_y = (y * y) / 200;
                if (x_x + y_y > 800 ) {
                    the_char = '0' + i;
                    if (i > 9) {
                        the_char = '@';
                    }
                    i = max_iter;
                }
                y = x * y / 100 + y0;
                x = x_x - y_y + x0;
                i = i + 1;
            }
            putc(the_char);
            x0 = x0 + x_step;
        }
        putc(10);
        y0 = y0 - y_step;
    }
}

void callMe()
{
	int ii = 0;
	while(ii < 3)
	{
		print("printing AsciiMandlebrot Before", ii);
		putc(10);
		
		// TODO:: Problem with the stack when we call a function inside a nested block.
		AsciiMandlebrot();
		
		ii = ii + 1;
		print("printing AsciiMandlebrot After", ii);
		putc(10);
		
		tempFunc(555, 1977);
		putc(10);
	}
}

void main()
{
	print("In main()");
	putc(10);
		
	int a = (1 * ((1 * (5 * 15)) / 10));
		
	tempFunc(55, 101);
	testPrinting();
	simpleCount();
	_100Doors();
	negativeTest();
	deep();
	gcd();
	factorial();
	fibonacci();
	fizzBuzz();
	_99BottlesOfBeer();
	primes();
	//AsciiMandlebrot();
	
	callMe();
	
	print("End of main()................");
	putc(10);
}
