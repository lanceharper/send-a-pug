open BsReactNative;

type item = {clickedInWave: bool};

type state = list(item);

type clicked = bool;

type action =
  | Click(clicked)
  | Reset;

let tempCounterStyle = Style.(style([color(String("lightblue"))]));

let component = ReasonReact.reducerComponent("NewJerk");

let containerStyle =
  Style.(
    style([
      position(Absolute),
      height(Pct(60.0)),
      width(Pct(30.0)),
      bottom(Pt(0.0)),
      left(Pt(0.0)),
      borderRadius(15.),
      justifyContent(FlexEnd),
    ])
  );

module CreateJerk = [%graphql
  {|
    mutation CreateJerk($displayName: String!, $channelId: String!) {
      createJerk(from: $displayName, message:"hey", to: $channelId) {
        from
        to
        message
        sentAt
      }
    }
  |}
];

module CreateJerkMutation = ReasonApollo.CreateMutation(CreateJerk);

let make = (~displayName, ~channelId, _children) => {
  ...component,

  initialState: () => [],

  reducer: (action, state) =>
    switch (action) {
    | Click(clicked) =>
      clicked ?
        ReasonReact.Update([{clickedInWave: true}, ...state]) :
        ReasonReact.NoUpdate
    | Reset => ReasonReact.Update([])
    },

  render: self => {
    <CreateJerkMutation>
      ...{(mutation, {result}) => {
        let createJerkResult = CreateJerk.make(~channelId, ~displayName, ());

        <View style=containerStyle>
          <TriggerContainer
            onPress={_event => {
              self.send(Click(true));
              mutation(~variables=createJerkResult##variables, ()) |> ignore;
            }}
          />
          {switch (result) {
           | _ => ReasonReact.null
           }}
          <Text style=tempCounterStyle>
            {ReasonReact.string(string_of_int(self.state |> List.length))}
          </Text>
        </View>;
      }}
    </CreateJerkMutation>;
  },
};