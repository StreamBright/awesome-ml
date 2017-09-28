type action =
  | Tick;

type state = {
  date: string,
  timerId: ref (option Js.Global.intervalId)
};

let component = ReasonReact.reducerComponent "Time";

let epoch_to_date epoch =>
  Js.Date.fromFloat epoch
  |> Js.Date.toISOString;

let make _children => {
  ...component,
  initialState: fun () => {date: epoch_to_date(Js.Date.now ()), timerId: ref None},
  reducer: fun action state =>
    switch action {
    | Tick => ReasonReact.Update {...state, date: epoch_to_date(Js.Date.now ())}
    },
  didMount: fun self => {
    self.state.timerId := Some (Js.Global.setInterval (self.reduce (fun _ => Tick)) 1000);
    ReasonReact.NoUpdate
  },
  render: fun {state: {date}} => {
    let out = {j|Current ISO date is: $date|j};
    <div> (ReasonReact.stringToElement out) </div>
  }
};
