debugging 379A



shortest:

0  A v < < A > > ^ A v A ^ A < v A < A A > > ^ A A v A ^ < A > A A v A ^ A < v A ^ > A A < A > A v < < A > A ^ > A A A v A < ^ A > A
1  A       <       A   >   A     v   < <       A A   >     ^   A A   >   A     v     A A   ^   A       <   v     A A A   >     ^   A
2  A               ^       A                   < <             ^ ^       A           > >       A                 v v v             A
3  A                       3                                             7                     9                                   A






mine:

0 A v < < A > > ^ A v A ^ A v < < A > > ^ A A v < A < A > > ^ A A v A A ^ < A > A v < A > ^ A A < A > A v < A < A > > ^ A A A v A ^ < A > A
1 A       <       A   >   A       <       A A     v   <       A A   > >     ^   A     v     A A   ^   A     v   <       A A A   >     ^   A
2 A               ^       A               ^ ^                 < <               A           > >       A                 v v v             A
3 A                       3                                                     7                     9                                   A











move[A A] = A
move[A <] = v < < A
move[A >] = v A
move[A v] = < v A
move[A ^] = < A



move[< <] = A
move[< v] = > A
move[< A] = > > ^ A
move[< ^] = > ^ A



move[v <] = < A
move[v A] = ^ > A


move[> ^] = ^ < A
move[> A] = ^ A


move[^ A] = > A










