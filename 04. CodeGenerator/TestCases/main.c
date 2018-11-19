
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

static void* pStaticPtr_0;
static void* pStaticPtr_1;
static void* pStaticPtr_2;
static void* pStaticPtr_3;
int32_t retNewFunc()
{
	print("In retNewFunc................");
	putc(10);

	int8_t ch = 'A';
	print("int8_t = ", ch);
	putc(10);
	
	int16_t iShort = 131071;
	print("int16_t = ", iShort);
	putc(10);

	int32_t j = 1;
	void* pNew = malloc(j * 9);
	print("pNew Allocated @", pNew);
	putc(10);
	
	@pNew = 1977;
	print("Value @ pNew = ", @pNew);
	putc(10);	
	
	pStaticPtr_0++;
	
	pStaticPtr_0 = malloc(j * 19);
	print("pNew Allocated @", @pStaticPtr_0);
	putc(10);
	
	@pStaticPtr_0 = 1982;
	print("Value of pStaticPtr_0 = ", @pStaticPtr_0);
	putc(10);
	
	@pStaticPtr_0 = @pNew;
	print("Assigned @pStaticPtr_0 = @pNew ==> ", @pStaticPtr_0);
	putc(10);
	
	int32_t iCount = @pStaticPtr_3;
	iCount++;
	@pStaticPtr_3 = iCount;
	print("@pStaticPtr_3 Count ==> ", @pStaticPtr_3);
	putc(10);
	
	print("In retNewFunc() start. (65535 & 255) = ", (65535 & 255), ", (170 | 85) = ", (170 | 85), ", (170 ^ 85) = ", (170 ^ 85), ", (~170) = ", (~170));
	putc(10);
	
	print("Bitwise: (15 << 4) = ", (15 << 4), ", (240 >> 4) = ", (240 >> 4));
	putc(10);	

	int32_t mmi = 11;
	switch(mmi)
	{
		case 10:
			print("In case 10");
			putc(10);			
		break;
		case 11:
			print("In case 11");
			putc(10);			
		case 12:
			print("In case 12");
			putc(10);					
		break;
		default:
			print("In default case");
			putc(10);						
		break;
	}
	
	int32_t j = 0;
	++j;
	
	int32_t m = 0;
	m = ++j + ++j + ++j;
	
	print("m = ", m);
	putc(10);
	
	for(int32_t i = 0, j = 1, int32_t iCount = 1; i < 10; ++i, ++iCount)
	{
		print("ForLoop i = ", i, " ,val of j = ", j, " ,iCount = ", iCount);
		putc(10);
	}
	
	free(pNew);
	free(pStaticPtr_0);
	
	int32_t iReturnValue = 1977;
	
	return iReturnValue;
}

int32_t retFunc()
{
	pStaticPtr_1 = malloc(14);
	
	print("In retFunc................");
	putc(10);
	
	free(pStaticPtr_1);
	
	return retNewFunc();
}

void tempFunc(int32_t arg0, int32_t arg1)
{
	pStaticPtr_2 = malloc(19);
	
	print("In tempFunc()");
	putc(10);
		
	int32_t iRet = 10;
	iRet = (10 + retFunc()) + 10;
	print("iRet = ", iRet);
	putc(10);
	
	if(((retFunc() + 10) - 1900) == 87)
	{
		print("retFunc returned 1977")
		putc(10);
	}
	
	int32_t iRet1 = 101 + retFunc();
	print("iRet1 = ", iRet1);
	putc(10);
	
	int32_t j = 111;
	j = j + arg0 + retFunc();
	print("tempFunc() args = ", j, ", ", arg1);
	putc(10);
	
	int32_t iCount = 1970;
	while(iCount <= retFunc())
	{
		iCount = iCount + 1;
		print("In While = ", iCount);
		putc(10);
	}
	
	if(j < 10)
	{
		j = 10;
		int32_t k = 2;
		k = 3;
	}
	else
	{
		int32_t k = 11;
		while(j == 11)
		{
			int32_t j = 1111;
			j = j + 1;
			
			arg0 = arg0 - 1;
		}
		
		k = j * 2;
	}
	
	free(pStaticPtr_2);
	
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
	int32_t count = 1;
	while (count < 10) {
		print("count is: ", count, "\n");
		putc(10);
		count = count + 1;
	}
}

void _100Doors()
{
	/* 100 Doors */
	int32_t i = 1;
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
	
	int32_t a = (-1 * ((1 * (5 * 15)) / 10));
	print(a);
	putc(10);
	
	int32_t b = -a + (3 * 4);
	print("b = ", b);
	putc(10);
	
	print("-b = ", -b);
	putc(10);
	
	print("-(1) = ", -(1));
	putc(10);
	
	int32_t i = a;
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
 
	int32_t a = 1071;
	int32_t b = 1029;
	print("gcd of ", a, " & ", b, " = ");
	
	while (b != 0) {
		int32_t new_a = b;
		b     = a % b;
		a     = new_a;
	}
	
	print(a);
	putc(10);
}

void factorial()
{
	/* 12 factorial is 479001600 */
 
	int32_t n = 12;
	int32_t result = 1;
	int32_t i = 1;
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
 
	int32_t n = 44;
	int32_t i = 1;
	int32_t a = 0;
	int32_t b = 1;
	int32_t w = 0;
	while (i < n) {
		int32_t w = a + b;
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
	int32_t i = 1;
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
	int32_t bottles = 99;
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
	int32_t count = 1;
	int32_t n = 1;
	int32_t limit = 100;
	while (n < limit) {
		int32_t k=3;
		int32_t p=1;
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
    int32_t left_edge   = -420;
    int32_t right_edge  =  300;
    int32_t top_edge    =  300;
    int32_t bottom_edge = -300;
    int32_t x_step      =    7;
    int32_t y_step      =   15;
 
    int32_t max_iter    =  200;
 
    int32_t y0 = top_edge;
    while (y0 > bottom_edge) {
        int32_t x0 = left_edge;
        while (x0 < right_edge) {
            int32_t y = 0;
            int32_t x = 0;
            int32_t the_char = ' ';
            int32_t i = 0;
            while (i < max_iter) {
                int32_t x_x = (x * x) / 200;
                int32_t y_y = (y * y) / 200;
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
	int32_t ii = 0;
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
	pStaticPtr_3 = malloc(4);
	@pStaticPtr_3 = 0;
	
	print("In main()");
	putc(10);
		
	int32_t a = (1 * ((1 * (5 * 15)) / 10));
		
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
	
	free(pStaticPtr_3);
	
	print("End of main()................");
	putc(10);
}
