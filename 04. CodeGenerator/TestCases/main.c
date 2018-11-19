/////////////////////////////////////////////////////////////////////////////////////////////////
/* fibonacci of 44 is 701408733 */
/*
n = 44;
i = 1;
a = 0;
b = 1;
while (i < n) {
    w = a + b;
    a = b;
    b = w;
    i = i + 1;
}
print("[fibonacci of 44 is ", w, "]");
//*/
/////////////////////////////////////////////////////////////////////////////////////////////////
/* 12 factorial is 479001600 */
/*
n = 12;
result = 1;
i = 1;
while (i <= n) {
    result = result * i;
    i = i + 1;
}
print("[factorial of 12 is ", result, "]");
//*/
/*
n = 12;
result = 1;
i = 1;
while(i, n, <=) {
        result = result, i, *;
        i = i, 1, +;
}

print([factorial of 12 is , result, ], );
//*/
/////////////////////////////////////////////////////////////////////////////////////////////////
/*
 Simple prime number generator
 */
/*
count = 1;
n = 1;
limit = 100;
while (n < limit) {
    k=3;
    p=1;
    n=n+2;
    while ((k*k<=n) && (p)) {
        p=n/k*k!=n;
        k=k+2;
    }
    if (p) {
        print(n, " is prime\n");
        count = count + 1;
    }
}
print("Total primes found: ", count, "\n");
//*/
/////////////////////////////////////////////////////////////////////////////////////////////////
/* 99 bottles */
/*
bottles = 99;
while (bottles > 0) {
    print(bottles, " bottles of beer on the wall\n");
    print(bottles, " bottles of beer\n");
    print("Take one down, pass it around\n");
    bottles = bottles - 1;
    print(bottles, " bottles of beer on the wall\n\n");
}
*/
/////////////////////////////////////////////////////////////////////////////////////////////////
/* FizzBuzz */
/*
i = 1;
while (i <= 100) {
    if (!(i % 15)) {
        print("FizzBuzz");
	}
    else {
		if (!(i % 3)) {
			print("Fizz");
		}
		else {
			if (!(i % 5)) {
				print("Buzz");
			}
			else {
				print(i);
			}
		}
	}
 
    print("\n");
    i = i + 1;
}
//*/
/////////////////////////////////////////////////////////////////////////////////////////////////
/*
 This is an integer ascii Mandelbrot generator
*/
//*
    left_edge   = -420;
    right_edge  =  300;
    top_edge    =  300;
    bottom_edge = -300;
    x_step      =    7;
    y_step      =   15;
 
    max_iter    =  200;
 
    y0 = top_edge;
    while (y0 > bottom_edge) {
        x0 = left_edge;
        while (x0 < right_edge) {
            y = 0;
            x = 0;
            the_char = ' ';
            i = 0;
            while (i < max_iter) {
                x_x = (x * x) / 200;
                y_y = (y * y) / 200;
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
        putc(10);//ascii value for '\n' char. since the StringTokenizer is unable to read characters starting with a slash('\').
        y0 = y0 - y_step;
    }
	
	putc('A');
//*/
/////////////////////////////////////////////////////////////////////////////////////////////////
/*
j = 111;
print(j, " is prime\n");
count = j * 2 + j;

print("Total primes found: ", count, "\n", 'c');

if(count < 10) { // iJZHole[38] 81
	k = count + 10;
	print(count, " is prime\n");
	
	if(count > 100) // iJZHole[58] 63
	{
		j = 98;
	}// iJMPElseEndOffset [64] = 65
	
	while(count >= 100) // iJZHole[71] 81
	{
		count = count - 1;
		
		if(count >= 999)
		{
			count = 1999;
		}
	}// iLoopHole[80] 65
	
}// iJMPElseEndOffset [82] = 121
else {
	m = k + 11;
	xx = j;
	
	if(k == 1) // iJZHole[100] 108
	{
		count = count + 10;
	}// iJMPElseEndOffset [109] = 117
	else{
		m = 123 * 40;
	}

	//117.
	m = 123;
}

//121. 
j = 101;
*/