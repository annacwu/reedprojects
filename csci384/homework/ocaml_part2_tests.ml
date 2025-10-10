let run_test ?eq_test:(eq_test : 'a -> 'a -> bool = ( = ))
             (exp : 'a) (f : unit -> 'a) (show : 'a -> string) : bool =
  let v = f () in
  if eq_test v exp then true
  else begin
      print_endline ("Unexpected result: expected " ^ show exp ^
                     " but got " ^ show v);
      false
    end

let id x = x

let print_list (show : 'a -> string) (lst : 'a list) : string =
  "[" ^ String.concat "; " (List.map show lst) ^ "]"

let compare_contents (a : 'a list) (b : 'a list) : bool =
  List.sort compare a = List.sort compare b

let compare_combs (a : 'a list list) (b : 'a list list) : bool =
  List.sort compare (List.map (List.sort compare) a) =
    List.sort compare (List.map (List.sort compare) b)

let rec tree_to_str : bstree -> string = function
  | Lf -> "Lf"
  | Br (i, l, r) -> "Br (" ^ string_of_int i ^ ", " ^ tree_to_str l ^ ", " ^
                    tree_to_str r ^ ")"

let without_tests _ =
  let v = [
    run_test
      Lf
      (fun _ -> without Lf 1)
      tree_to_str;
    run_test
      Lf
      (fun _ -> without (Br (1, Lf, Lf)) 1)
      tree_to_str;
    run_test
      (insert_all Lf [2; 1; 3])
      (fun _ -> without (insert_all Lf [2; 1; 3]) 5)
      tree_to_str;
    run_test
      (Br (3, Br (1, Lf, Br (2, Lf, Lf)), Br (4, Lf, Br (6, Lf, Lf))))
      (fun _ -> without (insert_all Lf [3; 1; 2; 5; 4; 6]) 5)
      tree_to_str;
  ] in List.for_all id v

let test_without (_ : unit) : unit =
  if without_tests () then print_endline "All tests passed"

let flatten_tests _ =
  let v = [
    run_test
      []
      (fun _ -> flatten Lf)
      (print_list string_of_int);
    run_test
      [1]
      (fun _ -> flatten (Br (1, Lf, Lf)))
      (print_list string_of_int);
    run_test
      [1; 2; 3; 4; 5; 6]
      (fun _ -> flatten (insert_all Lf [4; 3; 1; 6; 5; 2]))
      (print_list string_of_int);
    run_test
      [1; 2; 5; 6]
      (fun _ -> flatten (insert_all Lf [1; 6; 5; 2]))
      (print_list string_of_int);
    run_test
      [1; 2; 3; 4]
      (fun _ -> flatten (insert_all Lf [1; 2; 3; 4]))
      (print_list string_of_int);
  ] in List.for_all id v

let test_flatten (_ : unit) : unit =
  if flatten_tests () then print_endline "All tests passed"

let free_vars_tests _ =
  let v = [
    run_test
      []
      (fun _ -> free_vars (Plus (Num 1, Num 2)))
      (print_list id);
    run_test
      ["x"; "z"]
      (fun _ -> free_vars (Negate (Plus (Times (Var "x", Var "z"), Var "x"))))
      (print_list id)
      ~eq_test:compare_contents;
  ] in List.for_all id v

let test_free_vars (_ : unit) : unit =
  if free_vars_tests () then print_endline "All tests passed"

let rec arith_to_str : arith -> string = function
  | Plus (e1, e2) -> "Plus (" ^ arith_to_str e1 ^ ", " ^ arith_to_str e2 ^ ")"
  | Times (e1, e2) ->
    "Times (" ^ arith_to_str e1 ^ ", " ^ arith_to_str e2 ^ ")"
  | Negate e1 -> "Negate (" ^ arith_to_str e1 ^ ")"
  | Var s -> "Var (" ^ s ^ ")"
  | Num i -> "Num (" ^ string_of_int i ^ ")"

let subst_tests _ =
  let v = [
    run_test
      (Negate (Plus (Times (Num 4, Var "z"), Num 4)))
      (fun _ ->
        subst "x" (Num 4) (Negate (Plus (Times (Var "x", Var "z"), Var "x"))))
      arith_to_str;
    run_test
      (Negate (Plus (Times (Var "x", Var "z"), Var "x")))
      (fun _ ->
        subst "y" (Num 4) (Negate (Plus (Times (Var "x", Var "z"), Var "x"))))
      arith_to_str;
  ] in List.for_all id v

let test_subst (_ : unit) : unit =
  if subst_tests () then print_endline "All tests passed"

let multiway_tests (_ : unit) : bool =
  List.for_all id [
      run_test
        ["a"; "b"; "c"; "d"; "e"; "f"; "g"; "h"; "i"; "j"; "k"]
        (fun _ -> breadth_first (deserialize "(a (b e) (c f (g k)) (d h i j))"))
        (print_list id);
      run_test
        ["a"; "b"; "c"; "g"; "d"; "f"; "e"]
        (fun _ -> breadth_first (deserialize "(a b (c (d e) f) g)"))
        (print_list id);
      run_test
        "(a b (c (d e) f) g)"
        (fun _ -> serialize (deserialize "(a b (c (d e) f) g)"))
        id;
      run_test
        "(a b c d e f g h i)"
        (fun _ -> serialize (deserialize "(a b c d e f g h i)"))
        id;
      run_test
        "(a (b (c (d (e (f (g h)))))))"
        (fun _ -> serialize (deserialize "(a (b (c (d (e (f (g h)))))))"))
        id;
    ]

let test_multiway (_ : unit) : unit =
  if multiway_tests () then print_endline "All tests passed"
