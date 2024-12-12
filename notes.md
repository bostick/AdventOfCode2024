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




























