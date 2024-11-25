//header.h

//Booleans
[tru] |= K;
[fls] |= (KI);
[and] |= (CC[fls]);
[or] |= (C(CI[tru]));
[not] |= (C(CI[fls])[tru]);

//Scott natural numbers
[0] |= [tru];
[succ] |= (BK(CI));
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
[iszero] |= (C(CI[tru])(K[fls]));
[pred] |= (C(CI[0])I);

//Arithmetic operations
[add] |:= (CS(B'B[succ](C[add])));
[mult] |:= (C(CB'[0])(S(B'[add])(C[mult])));
[sub] |:= (S'C(CI)(B[sub][pred]));
[power] |:= (B'C(CI[1])(S(B'[mult])[power]));

//List
[cons] |= (B'BK(BC(CI)));
[nil] |= K;
[isnil] |= (C(CI[tru])(K(K[fls])));
[head] |= (C(CIK)K);
[tail] |= (C(CI[nil])(KI));
[pick] |:= (C(CS'[head])(B'C[pick][tail]));

//Pair
[pair] |= (BC(CI));
[fst] |= (CIK);
[snd] |= (CI(KI));

//etc
[fact] |:= (S(CI[1])(C(B'[mult])[fact]));
[fib] |:= (S(C'[iszero][pred][1])(S'[add](B[fib][pred])(B[fib](B[pred][pred]))));
[isOdd] |:= (S(C[iszero][fls])(B[isEven][pred]));
[isEven] |:= (S(C[iszero][tru])(B[isOdd][pred]));

//[list2431] := [cons][2]([cons][4]([cons][3]([cons][1][nil])));
