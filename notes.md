Advent of Code


logged in with GitHub


## day 6 part 2

first day with an incorrect submission


## day 7

first day that cannot just be brute forced


## day 12

first day needed to look up how to do it

calculate perimeter of weird shape on a grid

this helped:
https://medium.com/@wendellrodrigues89/island-perimeter-7a4c95049aed

sketch for day 12 part 2



# sides can be:

4
6
8
10
etc.




for each region:

bounding box



C.
CC

there is 1 . corner region

.1 has 4 sides

4 + (4 - 2) = 6

C region has 6 sides







CC
C.
CC

there is 1 . side region

.1 has 4 sides

4 + (4 - 0) = 8

C region has 8 sides





C..C
C..C
CC.C
CCCC

there is 1 . side region

.1 has 6 sides

4 + (6 - 0) = 10

C region has 10 sides






C..
C..
CC.
CCC

there is 1 . corner region

.1 has 6 sides

4 + (6 - 2) = 8

C region has 8 sides









C...
C...
CC.C
CCCC

there is 1 . corner region

.1 has 8 sides

4 + (8 - 2) = 10

C region has 10 sides











C.
CC
.C

there are 2 . corner regions

.1 has 4 sides
.2 has 4 sides

4 + (4 - 2) + (4 - 2) = 8

C region has 8 sides









...CC.
...CCC
..CC..
CCC...
.C....
.CC...
..C...

there are 4 . corner regions

4 + (6 - 2) + (4 - 2) + (10 - 2) + (6 - 2) = 22

C region has 22 sides






.I...
.III.
IIIII
III.I
..I..


there are 4 . corner regions

.1 has 4 sides
.2 has 6 sides
.3 has 6 sides
.4 has 4 sides

4 + (4 - 2) + (6 - 2) + (6 - 2) + (4 - 2) = 16

I has 16 sides







EEEEE
E....
EEEEE
E....
EEEEE

there are 2 . side regions

4 + (4) + (4) = 12

E has 12 sides








EEEEE
E....
EEEEE
E....
EEEE.

there are 1 . side region and 1 . corner region

4 + (4 - 0) + (6 - 2) = 12

E has 12 sides







AAAAAA
AAA..A
AAA..A
A..AAA
A..AAA
AAAAAA

4 + 4 + 4 = 12



comment on Reddit made clear:

"counting # sides" == "counting # corners"

could have probably put that to use...








## day 16

first day that did not finish on same day



cannot compute a hash for entire path and prevent making final step to end node

that just results in paths not being found




example:
we have lowest score: 7036
we have turn count: 7
we have move count: 36



example2:
we have lowest score: 11048
we have turn count: 11
we have move count: 48



input:
we have lowest score: 83444
we have turn count: 83
we have move count: 444





all best paths will have number of turns and same number of moves



cannot just a tuple of 83 turns to try

In[3]:= 2^83

Out[3]= 9671406556917033397649408






the first example for part 2 reminds me of day 10

maybe do this:

run dijkstra to fill in shortest distance to end for all nodes

this is like a "topographic map"

now run the same code as in day 10 part 2?











## day 17

opcode 0: adv
A = floor(A / pow(2, asCombo[]))

opcode 1: bxl
B = B ^ asLiteral[]

opcode 2: bst
B = asCombo[] % 8

opcode 3: jnz
if A != 0 then ip = asLiteral[]

opcode 4: bxc
B = B ^ C

opcode 5: out
output asCombo[] % 8

opcode 7: cdv
C = floor(A / pow(2, asCombo[]))





2,4
bst
B = A % 8

1,5
bxl
B = B ^ 5

7,5
cdv
C = floor(A / pow(2, B))

0,3
adv
A = floor(A / pow(2, 3))

1,6
bxl
B = B ^ 6

4,3
bxc
B = B ^ C

5,5
out
output B % 8

3,0
jnz
if A != 0 then ip = 0






A = 14
B = 6
B = 3
C = 1
A = 1
B = 5
B = 4
output 4


















106078322331393

is too high


105875099913985

is too high



new candidate to try to submit:

105875099912602








possible first bytes:

{0, 1, 2, 3, 6}



searching 0:
no candidates found

0.16062 seconds


searching 1:
best candidate found: 105875099913985

394.387 seconds


searching 2:
best candidate found: 105875099912602

162.907 seconds


searching 3:
no candidates found

5.4332 seconds


searching 6:
no candidates found

0.159166 seconds


















































































