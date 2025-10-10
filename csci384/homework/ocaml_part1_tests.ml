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

let coprime_tests (_ : unit) : bool =
  List.for_all id [
    run_test
      false
      (fun _ -> coprime 2 10)
      string_of_bool;
    run_test
      true
      (fun _ -> coprime 15 4)
      string_of_bool;
    run_test
      false
      (fun _ -> coprime 9 3)
      string_of_bool;
    run_test
      true
      (fun _ -> coprime 7 100)
      string_of_bool;
  ]

let test_coprime (_ : unit) : unit =
  if coprime_tests () then print_endline "All tests passed."

let range_tests (_ : unit) : bool =
  List.for_all id [
      run_test
        [1]
        (fun _ -> range 1 1)
        (print_list string_of_int);
      run_test
        [-2; -1; 0; 1; 2]
        (fun _ -> range (-2) 2)
        (print_list string_of_int);
      run_test
        [0; 1; 2; 3; 4]
        (fun _ -> range 0 4)
        (print_list string_of_int);
    ]

let test_range (_ : unit) : unit =
  if range_tests () then print_endline "All tests passed"

let remove_duplicates_tests (_ : unit) : bool =
  let v = [
    run_test
      []
      (fun _ -> remove_duplicates [])
      (print_list string_of_int);
    run_test
      [1; 3; 2]
      (fun _ -> remove_duplicates [1; 1; 3; 1; 1; 2; 2; 3; 1])
      (print_list string_of_int);
    run_test
      ["a"; "c"; "b"; "d"]
      (fun _ -> remove_duplicates ["a"; "c"; "c"; "b"; "d"; "b"])
      (print_list id);
    run_test
      [1; 2; 3]
      (fun _ -> remove_duplicates [1; 2; 3])
      (print_list string_of_int)
  ] in List.for_all id v

let test_remove_duplicates (_ : unit) : unit =
  if remove_duplicates_tests () then print_endline "All tests passed"

let prefix_sum_tests _ =
  let v = [
    run_test
      []
      (fun _ -> prefix_sum [])
      (print_list string_of_int);
    run_test
      [2]
      (fun _ -> prefix_sum [2])
      (print_list string_of_int);
    run_test
      [1; 3; 6; 10; 15]
      (fun _ -> prefix_sum [1; 2; 3; 4; 5])
      (print_list string_of_int);
    run_test
      [1; 4; 10; 20; 35]
      (fun _ -> prefix_sum [1; 3; 6; 10; 15])
      (print_list string_of_int);
  ] in List.for_all id v

let test_prefix_sum (_ : unit) : unit =
  if prefix_sum_tests () then print_endline "All tests passed"

let suffix_sum_tests _ =
  let v = [
    run_test
      []
      (fun _ -> suffix_sum [])
      (print_list string_of_int);
    run_test
      [2]
      (fun _ -> suffix_sum [2])
      (print_list string_of_int);
    run_test
      [15; 14; 12; 9; 5]
      (fun _ -> suffix_sum [1; 2; 3; 4; 5])
      (print_list string_of_int);
    run_test
      [55; 40; 26; 14; 5]
      (fun _ -> suffix_sum [15; 14; 12; 9; 5])
      (print_list string_of_int);
  ] in List.for_all id v

let test_suffix_sum (_ : unit) : unit =
  if suffix_sum_tests () then print_endline "All tests passed"

let table_to_str : (int * int * int) list list -> string =
  print_list
    (print_list
       (fun (x, y, fxy) -> "(" ^ string_of_int x ^ ", " ^ string_of_int y ^
                           ", " ^ string_of_int fxy ^ ")"))

let table_tests _ =
  let v = [
    run_test
      []
      (fun _ -> table ( + ) [] [1; 2; 3])
      table_to_str;
    run_test
      [[]; []; []]
      (fun _ -> table ( + ) [1; 2; 3] [])
      table_to_str;
    run_test
      [[(1, 5, 15); (1, 6, 16); (1, 7, 17); (1, 8, 18)];
       [(2, 5, 25); (2, 6, 26); (2, 7, 27); (2, 8, 28)];
       [(3, 5, 35); (3, 6, 36); (3, 7, 37); (3, 8, 38)]]
      (fun _ -> table (fun x y -> 10 * x + y) [1; 2; 3] [5; 6; 7; 8])
      table_to_str;
  ] in List.for_all id v

let test_table (_ : unit) : unit =
  if table_tests () then print_endline "All tests passed"

let binaries_tests _ =
  let v = [
    run_test
      [""]
      (fun _ -> binaries 0)
      (print_list id);
    run_test
      ["0"; "1"]
      (fun _ -> binaries 1)
      (print_list id);
    run_test
      ["000"; "001"; "010"; "011"; "100"; "101"; "110"; "111"]
      (fun _ -> binaries 3)
      (print_list id);
    run_test
      ["00000"; "00001"; "00010"; "00011"; "00100"; "00101"; "00110"; "00111";
       "01000"; "01001"; "01010"; "01011"; "01100"; "01101"; "01110"; "01111";
       "10000"; "10001"; "10010"; "10011"; "10100"; "10101"; "10110"; "10111";
       "11000"; "11001"; "11010"; "11011"; "11100"; "11101"; "11110"; "11111"]
      (fun _ -> binaries 5)
      (print_list id);
  ] in List.for_all id v

let test_binaries (_ : unit) : unit =
  if binaries_tests () then print_endline "All tests passed"

let combinations_tests (_ : unit) : bool =
  List.for_all id [
      run_test
        [[]]
        (fun _ -> combinations 0 [1; 2; 3])
        (print_list (print_list string_of_int));
      run_test
        [[1]; [2]; [3]]
        (fun _ -> combinations 1 [1; 2; 3])
        (print_list (print_list string_of_int))
        ~eq_test:compare_combs;
      run_test
        [[1; 2; 3]; [1; 2; 4]; [1; 2; 5]; [1; 3; 4]; [1; 3; 5];
         [1; 4; 5]; [2; 3; 4]; [2; 3; 5]; [2; 4; 5]; [3; 4; 5]]
        (fun _ -> combinations 3 [1; 2; 3; 4; 5])
        (print_list (print_list string_of_int))
        ~eq_test:compare_combs;
      run_test
        [[1; 2; 3; 4]]
        (fun _ -> combinations 4 [1; 2; 3; 4])
        (print_list (print_list string_of_int))
        ~eq_test:compare_combs;
    ]

let test_combinations (_ : unit) : unit =
  if combinations_tests () then print_endline "All tests passed"
