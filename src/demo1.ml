(* map & reduce,
 * the |> operator represents reverse function application,  f |> g = g f
 *)

let fff l =
  List.map (fun x -> x) l
  |> List.fold_left (+) 0

let () =
  Js.log (fff [1;2;3])
