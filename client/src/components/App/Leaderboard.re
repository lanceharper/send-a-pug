open BsReactNative;
open Js.Global;

module GetLeaderboard = [%graphql
  {|
    query listLeaders($streamerId: String!) {
      listLeaders(streamer_id: $streamerId) {
        items {
          player,
          score
        }
      }
    }
  |}
];

module GetLeaderboardQuery = ReasonApollo.CreateQuery(GetLeaderboard);

let scrollStyle = Style.(style([flex(1.0)]));
let containerStyle =
  Style.(
    style([
      backgroundColor(String("rgba(122,103,238, 0.1)")),
      position(Absolute),
      height(Pt(425.0)),
      width(Pt(265.0)),
      bottom(Pt(0.0)),
      right(Pt(0.0)),
      borderRadius(15.),
      borderTopWidth(1.),
      borderColor(String("#aaaaaa")),
      justifyContent(FlexStart),
    ])
  );

let leaderBoardRowStyle =
  Style.(
    style([
      flexDirection(Row),
      height(Pt(60.)),
      alignItems(Center),
      borderBottomWidth(1.0),
      borderBottomColor(String("rgba(255, 255, 255, 0.2)")),
    ])
  );

type action =
  | Increment
  | AssignIntervalId(intervalId);

type state = {
  count: int,
  intervalId: option(intervalId),
};

let component = ReasonReact.reducerComponent("Leaderboard");

let make = _children => {
  ...component,
  initialState: () => {count: 0, intervalId: None},

  didMount: self => {
    let intervalId = Js.Global.setInterval(() => self.send(Increment), 1000);
    self.send(AssignIntervalId(intervalId));
  },

  reducer: (action, state) =>
    switch (action) {
    | AssignIntervalId(intervalId) =>
      ReasonReact.Update({...state, intervalId: Some(intervalId)})
    | Increment => ReasonReact.Update({...state, count: state.count + 1})
    },

  render: _self => {
    let listLeadersQuery = GetLeaderboard.make(~streamerId="233205905", ());

    <GetLeaderboardQuery
      variables=listLeadersQuery##variables pollInterval=30000>
      ...{({result}) =>
        <ScrollView style=scrollStyle contentContainerStyle=containerStyle>
          {switch (result) {
           | Loading => <Text> {ReasonReact.string("Loading")} </Text>
           | Error(error) =>
             <Text> {ReasonReact.string(error##message)} </Text>
           | Data(response) =>
             switch (response##listLeaders) {
             | None => <Text> {"No Leaders" |> ReasonReact.string} </Text>
             | Some(data) =>
               Array.mapi(
                 (i, leaderboardEntry) =>
                   <View
                     key={leaderboardEntry##player} style=leaderBoardRowStyle>
                     <Text
                       style=Style.(
                         style([
                           fontFamily("Josefin Sans"),
                           color(String("#dcee67")),
                           fontSize(Float(20.0)),
                           fontWeight(`_100),
                           paddingLeft(Pt(16.)),
                           width(Pt(100.)),
                         ])
                       )>
                       {i + 1 |> string_of_int |> ReasonReact.string}
                     </Text>
                     <Text
                       style=Style.(
                         style([
                           fontFamily("Josefin Sans"),
                           fontSize(Float(20.0)),
                           color(String("rgba(255, 255, 255, 0.9)")),
                           textShadowColor(String("rgba(50, 50, 50, 0.8)")),
                           textShadowOffset(~height=1.0, ~width=1.0),
                           width(Pt(150.)),
                         ])
                       )>
                       {leaderboardEntry##player |> ReasonReact.string}
                     </Text>
                     <Text
                       style=Style.(
                         style([
                           color(String("rgba(238, 103, 220, 1.0)")),
                           fontFamily("Josefin Sans"),
                           fontWeight(`_400),
                           fontSize(Float(20.0)),
                           paddingRight(Pt(16.)),
                           textShadowColor(String("rgba(50, 50, 50, 0.8)")),
                           textShadowOffset(~height=1.0, ~width=1.0),
                           textAlign(Right),
                           width(Pct(100.)),
                         ])
                       )>
                       {leaderboardEntry##score
                        |> string_of_int
                        |> ReasonReact.string}
                     </Text>
                   </View>,
                 data##items,
               )
               |> ReasonReact.array
             }
           }}
        </ScrollView>
      }
    </GetLeaderboardQuery>;
  },
};