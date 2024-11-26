## Conversion of lambda to SKIBCS'B'C' combinator implemented in antlr4

### SKIBCS'B'C' combinator

```
Ix = x
Kfx = f  
Sfgx = (fx)(gx)    (=fx(gx))
Bfgx = f(gx)
Cfgx = (fx)g       (=fxg)
S'ufgx = u(fx)(gx)
B'ufgx = uf(gx) 
C'ufgx = u(fx)g
```

### Conversion rules

```
    lambda -> combinator  (repeat until all terms are combinators)

(1) \x.x   -> I

(2) \x.M   -> (KM)                (if x is not in M)


\x.(MN) -> ...
 
 if x is not in (MN), \x.(MN) -> (K(MN)) from (2)

 (3) \x.(Mx) -> M                  (if x is not in M)

 (4) \x.(MN) -> (BM(\x.N))         (if x is not in M)

 (5) \x.(MN) -> (C(\x.M)N)         (if x is not in N)

 (6) \x.(MM) -> (B(SII)(\x.M))     (if x is in M)
       because (\x.(MM))y      -> (MM)[x:=y]
                               -> (M[x:=y])(M[x:=y])
               (B(SII)(\x.M))y -> (SII)((\x.M)y)
                               -> (SII)(M[x:=y])
                               -> I(M[x:=y])(I(M[x:=y]))
                               -> (M[x:=y])(M[x:=y])

 (7) \x.(MN) -> (S(\x.M)(\x.N))


\x.(LMN) -> ...

 if x is in L,
   if x is not N, \x.(LMN) -> (C(\x.LM)N)        from (3)
             else \x.(LMN) -> (S(\x.LM)(\x.N))   from (5)

 if x is not L,

 (8)  \x.(LxN) -> CLN            (if x is not in N)

 (9)  \x.(LxN) -> SL(\x.N) 

 (10) \x.(LMx) -> LM             (if x is not in M) (same as (6))

 (11) \x.(LMN) -> B'LM(\x.N)     
   or \x.(LMN) -> B(LM)(\x.N)    (if x is not in M) 

 (12) \x.(LMN) -> C'L(\x.M)N     (if x is not in N)  

 (13) \x.(LMN) -> S'L(\x.M)(\x.N)  
```

exmaple 

```
 Y fixed-point combinator

 \f.((\x.f(xx))(\x.f(xx)));
 (B(SII)(\f.(\x.f(xx))));             (by 6)
 (B(SII)(\f.((B(f)(\x.(xx))))));      (by 4)
 (B(SII)((C(B)(\x.(xx))));            (by 8)
 (B(SII)((C(B)((S(\x.x)(\x.x))))));   (by 7)
 (B(SII)(CB(SII)));                   (by 1)
```

### Running the tests 

unzip [lambda2c-win64.zip](https://github.com/AkiraHakuta/Lambda2Combinator/releases)  


Open `Dos Command Prompt`  

```
/* test1.txt (Scott encoding) */  

putStr("//Scott encoding");

NL;putStr("//Booleans");
[tru] = \x.\y.x;
[fls] = \x.\y.y;

NL;putStr("//Numbers");
[0] =  \z.\y.z;
[succ] = \n.\x.\f.(fn);
[1] = [succ][0];
[2] = [succ][1];
[3] = [succ][2];
[iszero] = \n.(n[tru](\x.[fls]));
[pred] = \n.(n[0](\x.x));

NL;putStr("//addtion (pattern matching)");

/*
    Scott encoding pattern matching

    \n.(nmg)  n is Scott Number, g is function

    nmg          = ?
    [0]mg        = (\z.\y.z)mg = (\y.m)g = m
    ([succ]n')mg = (\x.\f.(fn'))mg = (\f.(fn'))g = gn'
  
    N       ->  (\n.(nmg))N
   [0]      ->  m
   [succ]n' ->  gn'
*/

[add] := \n.\m.(nm(\t.[succ]([add]tm)));


```

 command line options  

-t&nbsp;&nbsp;&nbsp;&nbsp;trace  
```
> lambda2c.exe test1.txt -t 


   0: putStr("//Scott encoding");
   1: //Scott encoding

   0: NL;
   1: 

   0: putStr("//Booleans");
   1: //Booleans

   0: [tru]=\x.\y.x;
   1: [tru] = (\x.(Kx));
   2: [tru] = K;
   3: [tru] = K;

   0: [fls]=\x.\y.y;
   1: [fls] = (KI);
   2: [fls] = (KI);

   0: NL;
   1: 

   0: putStr("//Numbers");
   1: //Numbers

   0: [0]=\z.\y.z;
   1: [0] = (\z.(Kz));
   2: [0] = K;
   3: [0] = K;

   0: [succ]=\n.\x.\f.(fn);
   1: [succ] = (\n.(K(C(\f.f)(n))));
   2: [succ] = ((B(K)(\n.(CIn))));
   3: [succ] = (BK(((C)(I))));
   4: [succ] = (BK(CI));

   0: [1]=[succ][0];
   1: [1] = [succ][0];

   0: [2]=[succ][1];
   1: [2] = [succ][1];

   0: [3]=[succ][2];
   1: [3] = [succ][2];

   0: [iszero]=\n.(n[tru](\x.[fls]));
   1: [iszero] = (C(\n.(n)([tru]))((K[fls])));
   2: [iszero] = (C((C(\n.n)([tru])))(K[fls]));
   3: [iszero] = (C(CI[tru])(K[fls]));
   4: [iszero] = (C(CI[tru])(K[fls]));

   0: [pred]=\n.(n[0](\x.x));
   1: [pred] = (C(\n.(n)([0]))I);
   2: [pred] = (C((C(\n.n)([0])))I);
   3: [pred] = (C(CI[0])I);
   4: [pred] = (C(CI[0])I);

   0: NL;
   1: 

   0: putStr("//addtion (pattern matching)");
   1: //addtion (pattern matching)

   0: [add]:=\n.\m.(nm(\t.[succ]([add]tm)));
   1: [add] := (\n.(S(n)(\m.((B([succ])(\t.([add]tm)))))));
   2: [add] := ((C(S)((B'(B)([succ])(\m.((C([add])m)))))));
   3: [add] := (CS(B'B[succ](((C)([add])))));
   4: [add] := (CS(B'B[succ](C[add])));
```

``` 
> lambda2c.exe test1.txt 

//Scott encoding

//booleans
[tru] = K;
[fls] = (KI);

//Numbers
[0] = K;
[succ] = (BK(CI));
[1] = [succ][0];
[2] = [succ][1];
[3] = [succ][2];
[iszero] = (C(CI[tru])(K[fls]));
[pred] = (C(CI[0])I);

//addtion (pattern matching)
[add] := (CS(B'B[succ](C[add])));
``` 

```  
lambda2c.exe test2.txt > header.h
```
For how to use header.h, see [SKIBC](https://github.com/AkiraHakuta/SKIBC/releases)

### Building (Windows only)   
<ol>
<li>

Install [mingw-w64](https://www.mingw-w64.org/downloads/)  

</li>
<li>

Install [cmake](https://cmake.org/download/)   
 
</li>
<li>

Install antlr4 of the Cpp target  
see [antlr4_Cpp_examples](https://github.com/AkiraHakuta/antlr4_Cpp_examples)  

</li>

Compile   
open Dos Command Prompt  
from `C:/.../lambda2c`  

```
> mkdir build
> cd build
> cmake -G "MinGW Makefiles" ..
> mingw32-make.exe  
```
</li>  
</ol>


### References  

-  D.A. Turner, Another algorithm for bracket abstraction (1979)
-  Gert Smolka, [Untyped Lambda Calculus Lecture Notes](https://www.ps.uni-saarland.de/courses/sem-ws15/ulc.pdf)
-  Wikipedia [Combinatory logic](https://en.wikipedia.org/wiki/Combinatory_logic)
