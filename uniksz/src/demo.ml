open Unix

let getenv = environment ()

let () = print_endline (Array.get (environment ()) 1);
