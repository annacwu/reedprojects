(* E -> fun $id -> E
      | E E
      | $id
      | ( E ) *)

(* This is ambigious because of the E E term. 
   I'm still struggling with disambiguating grammars
   so I might come to office hours for extra focus on that. 
*)

exception LexError of string

type tok =
  | Fun             (* keyword "fun" *)
  | Arrow           (* -> *)
  | LPar            (* ( *)
  | RPar            (* ) *)
  | Id of string    (* identifier *)

let lex (src : string) : tok list =
  let is_id_char c =
    ('a' <= c && c <= 'z') ||
    ('A' <= c && c <= 'z') ||
    ('0' <= c && c <= '9') || c = '_' in
  let consume_id i =
    let rec loop j =
      if j < String.length src && is_id_char src.[j] then loop (j + 1)
      else (String.sub src i (j - i), j) in
    loop i in
  let rec loop i =
    if i >= String.length src then []
    else match src.[i] with
      | '(' -> LPar :: loop (i + 1)
      | ')' -> RPar :: loop (i + 1)
      | '-' ->
        if src.[i+1] = '>' then Arrow :: loop (i + 2)
        else raise (LexError "Expected >")
      | c when List.mem c ['\n'; '\r'; '\t'; ' '] -> loop (i + 1)
      | c ->
        let (v, j) = consume_id i in
        if v = "fun" then Fun :: loop j
        else Id v :: loop j in
  loop 0

type fun_ast =
  | Function of string * fun_ast
  | Application of fun_ast * fun_ast
  | Var of string

let parse (source : tok list) : fun_ast =
  failwith "undefined"