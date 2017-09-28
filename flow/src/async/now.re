type action =
  | Tick;

type state = {
  now: float,
  timerId: ref (option Js.Global.intervalId)
};

let module Date = {
  external now : unit => float = "Date.now" [@@bs.val];
};

let component = ReasonReact.reducerComponent "Now";

let make _children => {
  ...component,
  initialState: fun () => {now: Date.now (), timerId: ref None},
  reducer: fun action state =>
    switch action {
    | Tick => ReasonReact.Update {...state, now: Date.now ()}
    },
  didMount: fun self => {
    self.state.timerId := Some (Js.Global.setInterval (self.reduce (fun _ => Tick)) 1000);
    ReasonReact.NoUpdate
  },
  render: fun {state: {now}} => {
    let now = {j|Current epoch nanoseconds: $now|j};
    <div> (ReasonReact.stringToElement now) </div>
  }
};
