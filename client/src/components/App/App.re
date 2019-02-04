/* App is the actual entry point for the application. It matches on the route prop to determine which
   sub-component to render */
open BsReactNative;
open Twitch;

type state = {count: int};

type clicked = bool;

type action =
  | Click(clicked)
  | Reset;
let tempCounterStyle = Style.(style([color(String("lightblue"))]));

let styles =
  StyleSheet.create(
    Style.{
      "navBar":
        style([
          flexDirection(Row),
          backgroundColor(String("slateblue")),
          paddingVertical(Pt(8.)),
          paddingHorizontal(Pt(16.)),
        ]),
      "fooText": style([backgroundColor(String("orange"))]),
      "link":
        style([
          paddingVertical(Pt(4.)),
          paddingHorizontal(Pt(8.)),
          marginRight(Pt(8.)),
        ]),
      "baseText": style([fontSize(Float(95.))]),
    },
  );

let component = ReasonReact.reducerComponent("App");

let make = _children => {
  ...component,
  didMount: _ => onAuthorized(twitchAuth => Js.log(clientIdGet(twitchAuth))),

  initialState: () => {count: 0},

  reducer: (action, state) =>
    switch (action) {
    | Click(clicked) =>
      clicked ?
        ReasonReact.Update({count: state.count + 1}) : ReasonReact.NoUpdate
    | Reset => ReasonReact.Update({count: 0})
    },

  render: self =>
    <View>
      {self.state.count >= 5 ?
         <Wave onFinish={_event => self.send(Reset)} /> : ReasonReact.null}
      <TriggerContainer onPress={event => self.send(Click(event.clicked))} />
      <Text style=tempCounterStyle>
        {ReasonReact.string(string_of_int(self.state.count))}
      </Text>
    </View>,
};

let default = ReasonReact.wrapReasonForJs(~component, _ => make([||]));