## Conversion of lambda to SKIBCS'B'C' combinator implemented in antlr4

### SKIBCS'B'C'Y combinator

```
Ix = x
Kfx = f  
Sfgx = (fx)(gx)    (=fx(gx))
Bfgx = f(gx)
Cfgx = (fx)g       (=fxg)
S'ufgx = u(fx)(gx)
B'ufgx = uf(gx) 
C'ufgx = u(fx)g

Yf = f(Yf) = f(f(Yf)) = f(f(f(Yf))) = ...
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
[Y] is Y fixed-point combinator

[Y] |:= \f.((\x.f(xx))(\x.f(xx)));
[Y] |:= (B(SII)(\f.(\x.f(xx))));             (by 6)
[Y] |:= (B(SII)(\f.((B(f)(\x.(xx))))));      (by 4)
[Y] |:= (B(SII)((C(B)(\x.(xx))));            (by 8)
[Y] |:= (B(SII)((C(B)((S(\x.x)(\x.x))))));   (by 7)
[Y] |:= (B(SII)(CB(SII)));                   (by 1)
```

### Running the tests 

unzip [lambda2c-win64.zip](https://github.com/AkiraHakuta/Lambda2Combinator/releases)  


Open `Dos Command Prompt`  

```
/* test1.txt */  

[Y] |:= \f.((\x.f(xx))(\x.f(xx)));
[fact_fx] |=\f.\x.(([iszero]x)[1]([mult]x(f([pred]x))));
[pick] |:= \n.\s.(([iszero]n)([head]s)([pick]([pred]n)([tail]s)));
```

 command line options  

-t&nbsp;&nbsp;&nbsp;&nbsp;trace  
```
> lambda2c.exe test1.txt -t 

   0: [Y]|:=\f.((\x.f(xx))(\x.f(xx)));
   1: [Y] |:= (B(SII)(\f.((B(f)(\x.(xx))))));
   2: [Y] |:= (B(SII)((C(B)((S(\x.x)(\x.x))))));
   3: [Y] |:= (B(SII)(CB(SII)));
   4: [Y] |:= (B(SII)(CB(SII)));

   0: [fact_fx]|=\f.\x.(([iszero]x)[1]([mult]x(f([pred]x))));
   1: [fact_fx] |= (\f.(S(\x.(([iszero]x))([1]))(\x.([mult]x(f([pred]x))))));
   2: [fact_fx] |= ((B'(S)(((C([iszero])[1])))(\f.((S([mult])(\x.(f([pred]x))))))));
   3: [fact_fx] |= (B'S((C[iszero][1]))((B'(S)([mult])(\f.((B(f)(\x.([pred]x))))))));
   4: [fact_fx] |= (B'S(C[iszero][1])(B'S[mult]((C(B)(([pred]))))));
   5: [fact_fx] |= (B'S(C[iszero][1])(B'S[mult](CB[pred])));

   0: [pick]|:=\n.\s.(([iszero]n)([head]s)([pick]([pred]n)([tail]s)));
   1: [pick] |:= (\n.(S'(([iszero]n))(\s.(([head]s)))(\s.([pick]([pred]n)([tail]s)))));
   2: [pick] |:= ((S(\n.(S'([iszero]n))((([head]))))(\n.((B'([pick])(([pred]n))(\s.([tail]s)))))));
   3: [pick] |:= (S((C'(S')(\n.(([iszero]n)))[head]))((C'(B'[pick])(\n.(([pred]n)))(([tail])))));
   4: [pick] |:= (S(C'S'(([iszero]))[head])(C'(B'[pick])(([pred]))[tail]));
   5: [pick] |:= (S(C'S'[iszero][head])(C'(B'[pick])[pred][tail]));
```

```
/* test2.txt */  
putStr("/* header.h */");

NL;putStr("//fixedpoint combinators");
[Y] |:= \f.((\x.f(xx))(\x.f(xx)));
[Z] |:= \f.((\x.(f(\y.(xxy))))(\x.(f(\y.(xxy)))));
[T] |:= (\x.\y.(y(xxy)))(\x.\y.(y(xxy)));
putStr("//The simplest fixed-point combinator, found by John Tromp, is");
[Y''] |:= (\x.\y.(xyx))(\y.\x.(y(xyx)));

NL;putStr("//booleans");
[tru] |= \x.\y.x;
[fls] |= \x.\y.y;
[and] |= \x.\y.(xy[fls]);
[or]  |= \x.\y.(y[tru]x);
[not] |= \x.(x[fls][tru]);

NL;putStr("//church numerals");
[0] |= [fls];
[succ] |= \n.\s.\z.(s(nsz));
[1] |= [succ][0];
[2] |= [succ][1];
[3] |= [succ][2];
[4] |= [succ][3];
[5] |= [succ][4];
[6] |= [succ][5];
[7] |= [succ][6];
[8] |= [succ][7];
[9] |= [succ][8];
[10] |= [succ][9];
[pred] |= \n.(n(\g.(g[1])[succ])(\v.[0])[0]);
[iszero] |= \n.(n(\x.[fls])[tru]);
[add] |= \m.\n.\f.\x.(mf(nfx));
[mult] |= \m.\n.\f.\x.(m(nf)x);
[sub] |= \m.\n.(n[pred]m); 
putStr("//[power]xy=y^x");
[power] |= \x.\y.(xy);//[power]xy = y^x

NL;putStr("//pairs");
[pair] |= \x.\y.\f.(fxy); 
[fst] |= \p.(p([tru]));
[snd] |= \p.(p([fls]));

NL;putStr("//lists");
[cons] |= [pair];
[head] |= [fst];
[tail] |= [snd];
[nil] |= \x.[tru];
[isnil] |= \p.(p(\x.\y.[fls]));
[nth] |= \x.\i.([head](i[tail]x));

NL;putStr("//etc");
[fact_x] |:= \x.(([iszero]x)[1]([mult]x([fact_x]([pred]x))));
[fact_fx] |= \f.\x.(([iszero]x)[1]([mult]x(f([pred]x))));
[sum_list] |:= \x.(([isnil]x)[0]([add]([head]x)([sum_list]([tail]x))));
[map] |:= \f.\l.(([isnil] l)[nil]([cons](f([head]l))([map]f([tail]l))));
[fold_left] |:= \g.\a.\l.(([isnil]l)a([fold_left]g(ga([head]l))([tail]l)));
[max_x_y] |= \x.\y.(([iszero]([sub]xy))yx);
[pick] |:= \n.\s.(([iszero]n)([head]s)([pick]([pred]n)([tail]s)));

NL;putStr("//list example");
putStr("/* [list2431] := [cons][2]([cons][4]([cons][3]([cons][1][nil]))) */");
```

``` 
> lambda2c.exe test2.txt 

/* header.h */

//fixedpoint combinators
[Y] |:= (B(SII)(CB(SII)));
[Z] |:= (B(SII)(CB(SII)));
[T] |:= (B'SI(SII))(B'SI(SII));
//The simplest fixed-point combinator, found by John Tromp, is
[Y''] |:= (SCI)(SB(C'S(CI)I));

//booleans
[tru] |= K;
[fls] |= (KI);
[and] |= (CC[fls]);
[or] |= (C(CI[tru]));
[not] |= (C(CI[fls])[tru]);

//church numerals
[0] |= [fls];
[succ] |= (SB);
[1] |= [succ][0];
[2] |= [succ][1];
[3] |= [succ][2];
[4] |= [succ][3];
[5] |= [succ][4];
[6] |= [succ][5];
[7] |= [succ][6];
[8] |= [succ][7];
[9] |= [succ][8];
[10] |= [succ][9];
[pred] |= (C(C(CI(C(CI[1])[succ]))(K[0]))[0]);
[iszero] |= (C(CI(K[fls]))[tru]);
[add] |= (BSB');
[mult] |= B;
[sub] |= (C(CI[pred]));
//[power]xy=y^x
[power] |= I;

//pairs
[pair] |= (BC(CI));
[fst] |= (CI[tru]);
[snd] |= (CI[fls]);

//lists
[cons] |= [pair];
[head] |= [fst];
[tail] |= [snd];
[nil] |= (K[tru]);
[isnil] |= (CI(K(K[fls])));
[nth] |= (B'B[head](C(CI[tail])));

//etc
[fact_x] |:= (S(C[iszero][1])(S[mult](B[fact_x][pred])));
[fact_fx] |= (B'S(C[iszero][1])(B'S[mult](CB[pred])));
[sum_list] |:= (S(C[isnil][0])(S'[add][head](B[sum_list][tail])));
[map] |:= (B'S(C[isnil][nil])(S'(S'[cons])(CB[head])(C(B'[map])[tail])));
[fold_left] |:= (B'(S'S)(C[isnil])(C(S'C'(BS'[fold_left])(C'CB'[head]))[tail]));
[max_x_y] |= (S'C(C'(S'[iszero])[sub]I)I);
[pick] |:= (S(C'S'[iszero][head])(C'(B'[pick])[pred][tail]));

//list example
/* [list2431] := [cons][2]([cons][4]([cons][3]([cons][1][nil]))) */
``` 



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
-  D.A. Turner, A new implementation technique for applicative languages (1979)
-  Wikipedia [Combinatory logic](https://en.wikipedia.org/wiki/Combinatory_logic)
