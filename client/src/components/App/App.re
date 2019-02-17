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

module CreateJerk = [%graphql
  {|
    mutation CreateJerk {
      createJerk(message:"hey", to: "marge") {
        from
        to
        message
        sentAt
      }
    }
  |}
];

module JerkCreated = [%graphql
  {|
    subscription JerkCreatedSubscription {
      inbox(to: "marge") {
        from
        to
        message
        sentAt
      }
    }
  |}
];

module CreateJerkMutation = ReasonApollo.CreateMutation(CreateJerk);

module JerkCreatedSubscription = ReasonApollo.CreateSubscription(JerkCreated);

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
      <CreateJerkMutation>
        ...{(mutation, {result}) => {
          let createJerkResult = CreateJerk.make();

          <View>
            <HeartContainer>
              <Heart filled=true style={styles##explodeHeart} />
              <Heart filled=true style={styles##explodeHeart} />
            </HeartContainer>
            {self.state |> List.length >= 5 ?
               <Wave onFinish={_event => self.send(Reset)} /> :
               ReasonReact.null}
            <TriggerContainer
              onPress={_event =>
                mutation(~variables=createJerkResult##variables, ()) |> ignore
              }
            />
            {switch (result) {
             | Loading => <Text> {ReasonReact.string("Searching")} </Text>
             | NotCalled => ReasonReact.null
             | Error(error) =>
               <Text> {ReasonReact.string(error##message)} </Text>
             | Data(response) =>
               Js.log(response);

               <Text>
                 {response##createJerk##message |> ReasonReact.string}
               </Text>;
             }}
            <Text style=tempCounterStyle>
              {ReasonReact.string(string_of_int(self.state |> List.length))}
            </Text>
          </View>;
        }}
      </CreateJerkMutation>
      <JerkCreatedSubscription>
        ...{({result}) =>
          switch (result) {
          | Loading => <div />
          | Error(_error) => <div />
          | Data(response) =>
            switch (response##inbox) {
            | None => <Text> {"No Message" |> ReasonReact.string} </Text>
            | Some(inboxResponse) =>
              <Text> {inboxResponse##sentAt |> ReasonReact.string} </Text>
            }
          }
        }
      </JerkCreatedSubscription>
    </ReasonApollo.Provider>,
};

let default = ReasonReact.wrapReasonForJs(~component, _ => make([||]));