/* App is the actual entry point for the application. It matches on the route prop to determine which
   sub-component to render */
open BsReactNative;
open Twitch;

type item = {clickedInWave: bool};

type state = list(item);

type clicked = bool;

type action =
  | Click(clicked)
  | Reset;
let tempCounterStyle = Style.(style([color(String("lightblue"))]));

let component = ReasonReact.reducerComponent("App");

let make = _children => {
  ...component,
  didMount: _ => onAuthorized(twitchAuth => Js.log(clientIdGet(twitchAuth))),

  initialState: () => [],

  reducer: (action, state) =>
    switch (action) {
    | Click(clicked) =>
      clicked ?
        ReasonReact.Update([{clickedInWave: true}, ...state]) :
        ReasonReact.NoUpdate
    | Reset => ReasonReact.Update([])
    },

  render: self =>
    <View>
      {self.state |> List.length >= 5 ?
         <Wave onFinish={_event => self.send(Reset)} /> : ReasonReact.null}
      <TriggerContainer onPress={event => self.send(Click(event.clicked))} />
      <Text style=tempCounterStyle>
        {ReasonReact.string(string_of_int(self.state |> List.length))}
      </Text>
    </View>,
};

let default = ReasonReact.wrapReasonForJs(~component, _ => make([||]));