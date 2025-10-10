type prop =
  | And of prop * prop
  | Not of prop
  | True
  | False
  | Sym of string
  | Let of string * prop * prop

(* 

  AND: 

  e1 ==> v1 e2 ==> v2 v1 && v2
  -----------------------------
  And(e1, e2) ==> True

  e1 ==> false
  ------------
  And(e1, e2) ==> false

  e2 ==> false
  ------------
  And(e1, e2) ==> false

  NOT: 

  e1 ==> false
  ------------
  Not(e1) ==> true

  e1 ==> true
  -----------
  Not(e1) ==> false

  TRUE: 

  ----------
  True ==> true

  FALSE:

  -----------
  False ==> false

  LET: 

  e1 ==> v1 e2[v1/x] ==> v2
  -------------------------
  let x = e1 in e2 ==> v2

*)


let rec interpret (program: prop) : bool =
  match program with
  | And(e1, e2) ->
    let v1 = interpret e1 in
    let v2 = interpret e2 in
    v1 && v2
  | Not(e1) -> 
    let v1 = interpret e1 in 
    (match v1 with
      | true -> false
      | false -> true)
  | True -> true
  | False -> false
