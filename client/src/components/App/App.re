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

module GetAllJerks = [%graphql {|
  {
    getJerks
  }
|}];

module GetAllJerksQuery = ReasonApollo.CreateQuery(GetAllJerks);

let styles =
  StyleSheet.create(
    Style.{
      "explodeHeart":
        style([left(Pt(0.)), top(Pt(0.)), position(Absolute)]),
    },
  );

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
    <ReasonApollo.Provider client=Client.instance>
      <GetAllJerksQuery>
        ...{({result, _}) =>
          switch (result) {
          | Loading => <Text> {"Loading" |> ReasonReact.string} </Text>
          | Error(_e) => <Text> {"Error" |> ReasonReact.string} </Text>
          | Data(_) =>
            <View>
              <HeartContainer>
                <Heart filled=true style={styles##explodeHeart} />
                <Heart filled=true style={styles##explodeHeart} />
              </HeartContainer>
              {self.state |> List.length >= 5 ?
                 <Wave onFinish={_event => self.send(Reset)} /> :
                 ReasonReact.null}
              <TriggerContainer onPress={_event => self.send(Click(true))} />
              <Text style=tempCounterStyle>
                {ReasonReact.string(string_of_int(self.state |> List.length))}
              </Text>
            </View>
          }
        }
      </GetAllJerksQuery>
    </ReasonApollo.Provider>,
};

let default = ReasonReact.wrapReasonForJs(~component, _ => make([||]));