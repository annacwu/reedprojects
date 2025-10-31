let parse = Ocaml_lite.Parser.parse
let interpret = Ocaml_lite.Interpret.interpret
let compile = Ocaml_lite.Compile.compile

let () =
  if Array.length Sys.argv <> 3 then
    failwith "Usage: ocaml_lite <interpret | compile> <source-file>"
  else
    let ch = In_channel.open_text Sys.argv.(2) in
    let text = In_channel.input_all ch in
    let () = In_channel.close ch in
    let ast = parse text in
    if Sys.argv.(1) = "interpret" then interpret ast []
    else if Sys.argv.(1) = "compile" then
      let bytecode = compile ast in
      let pos = String.rindex Sys.argv.(2) '.' in
      let name =
        String.concat "" [ String.sub Sys.argv.(2) 0 (pos + 1); "bc" ]
      in
      let ch = Out_channel.open_bin name in
      Out_channel.output_bytes ch bytecode
    else
      Printf.printf
        "Unrecognized option: %s. Options are \"interpret\" or \"compile\""
        Sys.argv.(1)
