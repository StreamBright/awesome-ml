(* map & reduce,
 * the |> operator represents reverse function application,  f |> g = g f
 * just like Unix pipes
 *)

let square_sum l =
  List.map (fun x -> x * x) l
  |> List.fold_left (+) 0

let () =
  Js.log (square_sum [1;2;3])
