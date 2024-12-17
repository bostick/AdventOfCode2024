

xPrint = xxPrint


makeMove[move: ( "<"  | "^" | ">" | "v" )] :=
 Catch[
  Module[{s, s2, boxesInContact, frontier, clear, rowOfBoxes},
   xPrint["move: ", move//InputForm];
   s = slice[move];
   Switch[move,
    "<" | ">",
    s2 = Replace[s, {
       
       {"@", o : PatternSequence["]", "["] ..., "#", rest___} :> {"@", o, "#", rest},
       {"@", o : PatternSequence["[", "]"] ..., "#", rest___} :> {"@", o, "#", rest},
       
       {"@", o : PatternSequence["]", "["] ..., ".", rest___} :> {".", "@", o, rest},
       {"@", o : PatternSequence["[", "]"] ..., ".", rest___} :> {".", "@", o, rest},
       {a___} :> Throw[{a}, "unhandled slice"]
       }];
    replace[move, s2]
    ,
    "^" | "v",
    s2 = Replace[s, {
       {"@", "#", rest___} :> {"@", "#", rest},
       {"@", ".", rest___} :> {".", "@", rest},
       {a___} :> actualPhysics
       }];
    
    If[s2 =!= actualPhysics,
      replace[move, s2];
      Throw[Null]
    ];


    xPrint[actualPhysics];

     Switch[move,
      "^",
      boxesInContact = CreateDataStructure["DynamicArray"];
      frontier = Cases[boxPoss, {pos[[1]] - 1, (pos[[2]] - 1) | (pos[[2]]) (*| (pos[[2]] + 1)*)}];
      While[frontier != {},
       boxesInContact["Append", frontier];
       frontier = Flatten[Cases[boxPoss, {#[[1]] - 1, (#[[2]] - 1) | (#[[2]]) | (#[[2]] + 1)}] & /@ frontier, 1]
       ];
      boxesInContact = Reverse[Normal[boxesInContact]];
      xPrint["boxesInContact: ", boxesInContact];
      
      clear = True;
      Do[
       Do[
        If[(map[[b[[1]] - 1]][[b[[2]]]] != "#" && map[[b[[1]] - 1]][[b[[2]] + 1]] != "#") == False,
         clear = False;
         Break[]
         ]
        ,
        {b, rowOfBoxes}
        ];
       If[! clear,
        Break[]
        ];
       ,
       {rowOfBoxes, boxesInContact}
       ];
      
      xPrint["clear: ", clear];
      If[! clear, Throw[Null]];
      
      (*now move*)
      (*clear*)
      map[[pos[[1]]]][[pos[[2]]]] = ".";
      Do[
       Do[
        map[[b[[1]]]][[b[[2]]]] = ".";
        map[[b[[1]]]][[b[[2]] + 1]] = ".";
        ,
        {b, rowOfBoxes}
        ]
       ,
       {rowOfBoxes, boxesInContact}
       ];
      boxPoss = DeleteCases[boxPoss, Flatten[boxesInContact, 1]];
      Do[
       Do[
        map[[b[[1]] - 1]][[b[[2]]]] = "[";
        map[[b[[1]] - 1]][[b[[2]] + 1]] = "]";
        ,
        {b, rowOfBoxes}
        ]
       ,
       {rowOfBoxes, boxesInContact}
       ];
      map[[pos[[1]] - 1]][[pos[[2]]]] = "@";
      mapTranspose = Transpose[map];
      boxPoss = Position[map, "["];
      pos = Position[map, "@"][[1]];
      ,
      "v",
      boxesInContact = CreateDataStructure["DynamicArray"];
      frontier = Cases[boxPoss, {pos[[1]] + 1, (pos[[2]] - 1) | (pos[[2]]) (*| (pos[[2]] + 1)*)}];
      While[frontier != {},
       boxesInContact["Append", frontier];
       frontier = Flatten[Cases[boxPoss, {#[[1]] + 1, (#[[2]] - 1) | (#[[2]]) | (#[[2]] + 1)}] & /@ frontier, 1]
       ];
      boxesInContact = Reverse[Normal[boxesInContact]];
      xPrint["boxesInContact: ", boxesInContact];
      
      clear = True;
      Do[
       Do[
        If[(map[[b[[1]] + 1]][[b[[2]]]] != "#" && map[[b[[1]] + 1]][[b[[2]] + 1]] != "#") == False,
         clear = False;
         Break[]
         ]
        ,
        {b, rowOfBoxes}
        ];
       If[! clear,
        Break[]
        ];
       ,
       {rowOfBoxes, boxesInContact}
       ];
      
      xPrint["clear: ", clear];
      If[! clear, Throw[Null]];
      
      (*now move*)
      (*clear*)
      map[[pos[[1]]]][[pos[[2]]]] = ".";
      Do[
       Do[
        map[[b[[1]]]][[b[[2]]]] = ".";
        map[[b[[1]]]][[b[[2]] + 1]] = ".";
        ,
        {b, rowOfBoxes}
        ]
       ,
       {rowOfBoxes, boxesInContact}
       ];
      boxPoss = DeleteCases[boxPoss, Flatten[boxesInContact, 1]];
      Do[
       Do[
        map[[b[[1]] + 1]][[b[[2]]]] = "[";
        map[[b[[1]] + 1]][[b[[2]] + 1]] = "]";
        ,
        {b, rowOfBoxes}
        ]
       ,
       {rowOfBoxes, boxesInContact}
       ];
      map[[pos[[1]] + 1]][[pos[[2]]]] = "@";
      mapTranspose = Transpose[map];
      boxPoss = Position[map, "["];
      pos = Position[map, "@"][[1]];
      ]
     ]
    ]
   ]


makeMove[a___] := Throw[{a},"unhandled args to makeMove"]













