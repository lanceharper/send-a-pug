open BsReactNative;

type item = {clickedInWave: bool};

type state = list(item);

type clicked = bool;

type action =
  | Click(clicked)
  | Reset;

let styles =
  StyleSheet.create(
    Style.{
      "explodeHeart":
        style([left(Pt(0.)), top(Pt(0.)), position(Absolute)]),
    },
  );

let tempCounterStyle = Style.(style([color(String("lightblue"))]));

let component = ReasonReact.reducerComponent("NewJerk");

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

        <View>
          <HeartContainer>
            <Heart filled=true style={styles##explodeHeart} />
            <Heart filled=true style={styles##explodeHeart} />
          </HeartContainer>
          {self.state |> List.length >= 5 ?
             <Wave onFinish={_event => self.send(Reset)} /> : ReasonReact.null}
          <TriggerContainer
            onPress={_event => {
              self.send(Click(true));
              mutation(~variables=createJerkResult##variables, ()) |> ignore;
            }}
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
    </CreateJerkMutation>;
  },
};