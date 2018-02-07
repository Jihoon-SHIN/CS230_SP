/* 
 * CS:APP Data Lab 
 * 
 * <Please put your name and userid here>
 * 
 * bits.c - Source file with your solutions to the Lab.
 *          This is the file you will hand in to your instructor.
 *
 * WARNING: Do not include the <stdio.h> header; it confuses the dlc
 * compiler. You can still use printf for debugging without including
 * <stdio.h>, although you might get a compiler warning. In general,
 * it's not good practice to ignore compiler warnings, but in this
 * case it's OK.  
 */

#if 0
/*
 * Instructions to Students:
 *
 * STEP 1: Read the following instructions carefully.
 */

You will provide your solution to the Data Lab by
editing the collection of functions in this source file.

INTEGER CODING RULES:
 
  Replace the "return" statement in each function with one
  or more lines of C code that implements the function. Your code 
  must conform to the following style:
 
  int Funct(arg1, arg2, ...) {
      /* brief description of how your implementation works */
      int var1 = Expr1;
      ...
      int varM = ExprM;

      varJ = ExprJ;
      ...
      varN = ExprN;
      return ExprR;
  }

  Each "Expr" is an expression using ONLY the following:
  1. Integer constants 0 through 255 (0xFF), inclusive. You are
      not allowed to use big constants such as 0xffffffff.
  2. Function arguments and local variables (no global variables).
  3. Unary integer operations ! ~
  4. Binary integer operations & ^ | + << >>
    
  Some of the problems restrict the set of allowed operators even further.
  Each "Expr" may consist of multiple operators. You are not restricted to
  one operator per line.

  You are expressly forbidden to:
  1. Use any control constructs such as if, do, while, for, switch, etc.
  2. Define or use any macros.
  3. Define any additional functions in this file.
  4. Call any functions.
  5. Use any other operations, such as &&, ||, -, or ?:
  6. Use any form of casting.
  7. Use any data type other than int.  This implies that you
     cannot use arrays, structs, or unions.

 
  You may assume that your machine:
  1. Uses 2s complement, 32-bit representations of integers.
  2. Performs right shifts arithmetically.
  3. Has unpredictable behavior when shifting an integer by more
     than the word size.

EXAMPLES OF ACCEPTABLE CODING STYLE:
  /*
   * pow2plus1 - returns 2^x + 1, where 0 <= x <= 31
   */
  int pow2plus1(int x) {
     /* exploit ability of shifts to compute powers of 2 */
     return (1 << x) + 1;
  }

  /*
   * pow2plus4 - returns 2^x + 4, where 0 <= x <= 31
   */
  int pow2plus4(int x) {
     /* exploit ability of shifts to compute powers of 2 */
     int result = (1 << x);
     result += 4;
     return result;
  }

FLOATING POINT CODING RULES

For the problems that require you to implent floating-point operations,
the coding rules are less strict.  You are allowed to use looping and
conditional control.  You are allowed to use both ints and unsigneds.
You can use arbitrary integer and unsigned constants.

You are expressly forbidden to:
  1. Define or use any macros.
  2. Define any additional functions in this file.
  3. Call any functions.
  4. Use any form of casting.
  5. Use any data type other than int or unsigned.  This means that you
     cannot use arrays, structs, or unions.
  6. Use any floating point data types, operations, or constants.


NOTES:
  1. Use the dlc (data lab checker) compiler (described in the handout) to 
     check the legality of your solutions.
  2. Each function has a maximum number of operators (! ~ & ^ | + << >>)
     that you are allowed to use for your implementation of the function. 
     The max operator count is checked by dlc. Note that '=' is not 
     counted; you may use as many of these as you want without penalty.
  3. Use the btest test harness to check your functions for correctness.
  4. Use the BDD checker to formally verify your functions
  5. The maximum number of ops for each function is given in the
     header comment for each function. If there are any inconsistencies 
     between the maximum ops in the writeup and in this file, consider
     this file the authoritative source.

/*
 * STEP 2: Modify the following functions according the coding rules.
 * 
 *   IMPORTANT. TO AVOID GRADING SURPRISES:
 *   1. Use the dlc compiler to check that your solutions conform
 *      to the coding rules.
 *   2. Use the BDD checker to formally verify that your solutions produce 
 *      the correct answers.
 */


#endif
/* Copyright (C) 1991-2016 Free Software Foundation, Inc.
   This file is part of the GNU C Library.

   The GNU C Library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) any later version.

   The GNU C Library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with the GNU C Library; if not, see
   <http://www.gnu.org/licenses/>.  */
/* This header is separate from features.h so that the compiler can
   include it implicitly at the start of every compilation.  It must
   not itself include <features.h> or any other header that includes
   <features.h> because the implicit include comes before any feature
   test macros that may be defined in a source file before it first
   explicitly includes a system header.  GCC knows the name of this
   header in order to preinclude it.  */
/* glibc's intent is to support the IEC 559 math functionality, real
   and complex.  If the GCC (4.9 and later) predefined macros
   specifying compiler intent are available, use them to determine
   whether the overall intent is to support these features; otherwise,
   presume an older compiler has intent to support these features and
   define these macros by default.  */
/* wchar_t uses Unicode 8.0.0.  Version 8.0 of the Unicode Standard is
   synchronized with ISO/IEC 10646:2014, plus Amendment 1 (published
   2015-05-15).  */
/* We do not support C11 <threads.h>.  */
/* 
 * fp_func4 - Return bit-level equivalent of expression 2*f for
 *   floating point argument f.
 *   Both the argument and result are passed as unsigned int's, but
 *   they are to be interpreted as the bit-level representation of
 *   single-precision floating point values.
 *   When argument is NaN, return argument
 *   Operations allowed to use: Any integer/unsigned operations incl. ||, &&. also if, while
 *   Max # of operations: 30
 *   Points: 4
 */
unsigned fp_func4(unsigned uf) {
  int s, exp, frac;
  s = uf>>31;            //sign bit
  exp = (uf>>23)&0xff;   //exp bit
  frac = ((uf<<9)>>9);   //frac bit
  if(exp==0){
	if(frac !=0){ 
		return (s<<31)|(frac<<1);  //case : exp bit ==0 , frac bit!= 0
	}else if(frac==0){
		return (s<<31)|uf;         //case : exp bit ==0, frac bit!=0 => return uf;
	}
  }else if(exp==0xff){
 	return uf;               //case : NaN => return uf;
  }else{
 	return (s<<31)| (exp+1)<<23 | frac;  //case : 2^E => 2^(E+1) so, exp+1
  }
}
/* 
 * is_addition_no_problem - Will it be okay(no overflow) when we add x and y?
 *   If there is an overflow, return 0. Otherwise, return 1.
 *   Example: is_addition_no_problem(0ix80000000,0x80000000) = 0,
 *            is_addition_no_problem(0x80000000,0x70000000) = 1, 
 *   Operations allowed to use: ! ~ & ^ | + << >>
 *   Max # of operations: 20
 *   Points: 3
 */
int is_addition_no_problem(int x, int y) {
  int a,b,c,d,e;
  a= (x>>31)+1; //if x>=0, a=1. else, a=0.
  b= (y>>31)+1; // if y>=0, b=1. else, b=0.
  c= ((x+y)>>31)+1; // if x+y>=0, c=1. else c=0.
  d = (a^b);       // if x'sign bit and y'sign bit is equal,then  d=0. else, d=1.
  e = (a&b);       // & operation,
  return d| ((!d)&e&c)| ((!d)&!e&!c);   /*If x'sign and y' sign are diffent, then there is not overflow.
				               However, When they are equal, their sign must be keeping. */
}
/* 
 * is_x_fits_in_16_bit - Returns 1 if x fits in 16-bit, 2's complement.
 *   Examples: is_x_fits_in_16_bit(34000) = 0, is_x_fits_in_16_bit(-32768) = 1
 *   Operations allowed to use: ! ~ & ^ | + << >>
 *   Max # of operations: 8
 *   Points: 1
 */
int is_x_fits_in_16_bit(int x) {
   return !(x^((x<<16)>>16));     //It is same as is_x_fits_in_n_bit. If x fits in 16 bit, then the x^((x<<16)>>16) must be 0.
}
/* 
 * fp_func1 - Return bit-level equivalent of expression 0.5*f for
 *   floating point argument f.
 *   Both the argument and result are passed as unsigned int's, but
 *   they are to be interpreted as the bit-level representation of
 *   single-precision floating point values.
 *   When argument is NaN, return argument
 *   Operations allowed to use: Any integer/unsigned operations incl. ||, &&. also if, while
 *   Max # of operations: 30
 *   Points: 4
 */
unsigned fp_func1(unsigned uf) {
  int s, exp, frac,newfrac,sign,fr,k,a,ans;
  s = uf>>31;			//sign
  exp = (uf>>23)&0xff;		//exp field
  frac = ((uf<<9)>>9);		//frac field
  sign = s<<31;			//sign field
  newfrac = frac>>1;		//0.5f on frac
  k = 0;			//check the rounding. when k=1,we  need to round.
  a = 0;			//check the ex) 0x800000=> 0x400000. when a=0, we need to change the method.
  if(exp==0){
	if(frac==1){
		 return sign|0;  //check the exception
	}else if(frac!=0){
		k=1;
		a=1;
	}
  }else if(exp==0xff){
	return uf;		//check the exception
  }else if(exp==1){
	exp = exp<<22;           //If exp==1, we need to shift only 22.
	k=1;
	a=1;	
  }else{
	exp = (exp-1)<<23;         //the method 0.5f when exp!0 && exp != 0xff.
  }
  fr = frac&0xf;
  ans = sign|exp;
  if(k && ((fr==0xb)|(fr==0x3)||(fr==0x7)||(fr==0xf))){		//condition that rounding.
	return ans|(newfrac+1);				//+1 is rounding.
  }else{
	if(a) return ans|newfrac;		//0.5f when exp==0 and exp==1.
	return ans|frac;			//0.5f when exp !=0.
  }
}
/* 
 * fp_func3 - Return bit-level equivalent of absolute value of f for
 *   floating point argument f.
 *   Both the argument and result are passed as unsigned int's, but
 *   they are to be interpreted as the bit-level representations of
 *   single-precision floating point values.
 *   When argument is NaN, return argument..
 *   Operations allowed to use: Any integer/unsigned operations incl. ||, &&. also if, while
 *   Max # of operations: 10
 *   Points: 2
 */
unsigned fp_func3(unsigned uf) {
  int exp, frac;
  exp = (uf>>23)&0xff;        //exp field.
  frac = ((uf<<9)>>9);       //frac field.
  if(exp == 0xff && frac !=0){   //check the exception.
	return uf;
  }else{
    return (uf<<1)>>1;  //uf is unsigned so, (<<1)>>1 makes uf positive.
  }
}
/* 
 * fp_func2 - Return bit-level equivalent of expression (int) f
 *   for floating point argument f.
 *   Argument is passed as unsigned int, but
 *   it is to be interpreted as the bit-level representation of a
 *   single-precision floating point value.
 *   Anything out of range (including NaN and infinity) should return
 *   0x80000000u.
 *   Operations allowed to use: Any integer/unsigned operations incl. ||, &&. also if, while
 *   Max # of operations: 30
 *   Points: 4
 */
int fp_func2(unsigned uf) {
  int s, exp, frac,bias,e,k,answer;
  s = uf>>31;    		//sign,uf is positive -> s= 0// uf is negative -> s=1.
  exp = (uf>>23)&0xff;		//exp field
  frac = ((uf<<9)>>9);		//frac field
  bias = 127;			//In 32 bit, bias =127 = 2^7-1
  if(exp==0){			//CHeck the exceoption
	return 0;
  }else if(exp==0xff){		
	return (1<<31);		//Out of range(infinity)
  }else{
	e = exp-bias;		//Calculate the exponent
	k = (1<<23)|frac;	//Convert the frac into 1.xxxxxxxx~~
	if(e<0){
		return 0;	//Denormalized Values return 0
	}else if(e>23){
		return (1<<31);	//Out of range
	}else{
		answer = k>>(23-e);	//Convert the value 1.xxxxx~ * 2^e =  1xxxxxxxx
	}
  }	
  if(s==1){
  	return (~answer)+1;  //Return negative value
  }else{
	return answer; //Return positive value
 }
}
/* 
 * extract_nth_byte - Extract the nth byte from a word x and return it.
 *   The index of the byte at LSB is 0.
 *   The index of the byte at MSB is 3.
 *   Examples: extract_nth_byte(0xFF34AB78,1) = 0xAB
 *   Operations allowed to use: ! ~ & ^ | + << >>
 *   Max # of operations: 6
 *   Points: 2
 */
int extract_nth_byte(int x, int n) {
  int a,b;
  b = n<<3;        //n=0->b=0, n=1->b=8, n=2->b=16 , n=3->b=24
  a = (x>>b)&(0xFF); //extract nth byte by shifting.
  return a;
}
/* 
 * substitute_byte(x,n,c) - In a word x, replace the nth byte with a new byte c.
 *   The index of the byte at LSB is 0.
 *   The index of the byte at MSB is 3.
 *   Examples: substitute_byte(0xffffffff,1,0xab) = 0xffffabff
 *   Assume that 0 <= n <= 3 and 0 <= c <= 255.
 *   Operations allowed to use: ! ~ & ^ | + << >>
 *   Max # of operations: 10
 *   Points: 3
 */
int substitute_byte(int x, int n, int c) {
  int a,b,d;
  a = n<<3;		 //n=0->b=0, n=1->b=8, n=2->b=16 , n=3->b=24
  b = 0xFF << a;	
  d = (x|b)^b;           //Making nth byte is 0.
  return d | (c<<a);     // Substituting c on nth byte because nth byte is 0.
}
/*
 * floor_log_2 - return floor(log base 2 of x), where x > 0
 *   Example: floor_log_2(16) = 4
 *   Operations allowed to use: ! ~ & ^ | + << >>
 *   Max # of operations: 90
 *   Points: 4
 */
int floor_log_2(int x) {
  int ans,a;
  a = 0;
  /*
  Suppose, we Caculate 0x01000000.
  Then ans may be ans = 16+8+4+2 = 30.
  log_2(0x01000000) = 30
  */
  ans = ( (!!( (x>>16)^a))<<4);  //Check the 16 bits of front of x.
  ans = ans + ( (!!( ((x>>ans)>>8)^a))<<3); //Check the 8 bits.
  ans = ans + ( (!!( ((x>>ans)>>4)^a))<<2); //Check the 4 bits.
  ans = ans + ( (!!( ((x>>ans)>>2)^a))<<1); //Check the 2 bits.
  ans = ans + ( (!!( ((x>>ans)>>1)^a)));  //Check the 1 bit of the last of x.
  return ans;
}
/* 
 * is_fits_in_bits - Can x be represented in 2's complement integer with n-bit?
 *   1 <= n <= 32
 *   Examples: is_fits_in_bits(5,3) = 0, is_fits_in_bits(-4,3) = 1
 *   Operations allowed to use: ! ~ & ^ | + << >>
 *   Max # of operations: 15
 *   Points: 2
 */
int is_fits_in_bits(int x, int n) {
  return !(x^((x<<(32+(~n+1)))>>(32+(~n+1))));  /*It same as is_x_fits_in_16bits.
						If x is fits in n bit, then x^((x<<32+(n-1))<<(32+(n-1)) must be zero.
						IF x is not fits in n bit, then sign bit of (x<<32+(n-1)) must be 1. 
						Then >>shift makes 1 instead of zero.		 */

}

/* hmb - How many bits are required to represent x in 2's complement?
 *  Examples: hmb(12) = 5
 *            hmb(298) = 10
 *            hmb(-5) = 4
 *            hmb(0)  = 1
 *            hmb(-1) = 1
 *            hmb(0x80000000) = 32
 *  Operations allowed to use: ! ~ & ^ | + << >>
 *  Max # of operations: 90
 *  Points: 4
 */
int hmb(int x) {
  int a,b,d,e,notb,answer,k;
  b = (x>>31);											//negative : -1 positive:0
  notb = ~b+1;			//negative : 1  positive : 0

  /*Check the case that 0xffffffff
   We need to caculate -2. K is -2 when the input is 0xffffffff.
  */
  d = !(x+1)+!(x+1);    
  e = ~d+1;					
  k = !(b+1);			//positive = 0, negative = 1;
  
  /*Check the how many bits are required. It is same method in log_2*/
  a =  ( (!!( (x>>16)^b )) <<4);						
  a += ( (!!( ((x>>a)>>8)^b )) <<3);
  a += ( (!!( ((x>>a)>>4)^b )) <<2);
  a += ( (!!( ((x>>a)>>2)^b )) <<1);	
  a += (1<< (!! (((x>>a)>>1)^b) )); //In log_2, because of input>0, MSB is always zero. But in this case, We need to consider MSB. So, we need to calculate 1<<.
  
  /*(notb+x) is need to +1 when x is positive.*/
  answer = a + ((((notb+x)>>31)+1)&!!x)+k+e; 
  return answer;
}
/* 
 * rr - For given x, rotate it to the right by n.
 
*   Can assume that 0 <= n <= 31
 *   Examples: rr(0xF7654321,4) = 0x7654321F
 *   Operations allowed to use: ~ & ^ | + << >> !
 *   Max # of operations: 25
 *   Points: 3 
 */
int rr(int x, int n) {
  int a,b,c,d,e;
  a = 32+(~n+1);  //Calculate the number of shifts we need
  b = x<<a;	//Shifting the bit that right bit.
  c = x>>n;	//Shifiting the bit that left bit.
  d = (~0)<<a;	//It will use erasing c's sign bit. ex) 0xF0000000>>4 -> 0xFF000000 erasing first F.
  e = (c|d)^d;  //Erasing
  return e|b;   //Return the answer.
}
/*
 * is_power_of_2 - Returns 1 if x is a power of 2. Returns 0 otherwise.
 *   Examples: is_power_of_2(5) = 0, is_power_of_2(8) = 1, is_power_of_2(0) = 0
 *   Hint: Negative numbers are not power of two.
 *   Operations allowed to use: ! ~ & ^ | + << >>
 *   Max # of operations: 20
 *   Points: 4
 */
int is_power_of_2(int x) {

  int sign,ans;
  sign = (x>>31)+1; //x>=0 -> sign =1, x<0 -> sign =0
  ans =(!!x) & sign & ( !(x&( (~(x+~0)) <<1 )) ); /*if x=0, then x=0. And ans must be positive, &sign. If x is the power of 2, then
  													x+~0 is the form like 00..1111111. then ~(x+~0) -> 11..00000. Then x&((~(x+~0))<<1) may be zero.
													If x is not not power of 2, x&((~(x+~0))<<1) may be some value not zero. */
  return ans;
}
/* 
 * aeb - If even-numbered bit in a word is set at least one, return 1.
 *   Examples aeb(0xA) = 0, aeb(0xE) = 1
 *   Operations allowed to use: ! ~ & ^ | + << >>
 *   Max # of operations: 12
 *   Points: 2
 */
int aeb(int x) {
  int a;
  a = (0x55) | (0x55<<8) | (0x55<<16) | (0x55<<24); // 01010101010101010101010101010101
  return !!(a&x);                             //If there is number on odd-numbered bit, then a&x is not zero. !!a&x is zero.
					      //Else, a&x is zero. !!a&x is 1.
}
/* 
 * is_ge - Return 1 when x >= y, else return 0 
 *   Example: is_ge(4,5) = 1.
 *   Operations allowed to use: ! ~ & ^ | + << >>
 *   Max # of operations: 24
 *   Points: 3
 */
int is_ge(int x, int y) {
  int a,b,c,d,e;
  a = (x>>31)+1;			//x>=0 -> a=1. x<0 -> a=0.
  b = (y>>31)+1;			//y>=0 -> b=1. y<0 -> b=0.
  c = a^b;				//x and y are same sign, then c is 0. x and y are different sign, then c is 1.
  d = x + (~y+1);			// d = x-y.
  e = d & (1<<31);  			// e is check the d is positive or negative.
  return (c&a)|(!c&!e);			// c is 1(different sign) -> check x is positive.
					// c is 0(same sign) -> check x-y is positive.
}
