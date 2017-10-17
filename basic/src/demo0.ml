(*
 *  This transpiles to console.log(message);
 *)

external pi: float = "Math.PI" [@@bs.val];

let () = Js.log "You are about to enter Mordor!"
