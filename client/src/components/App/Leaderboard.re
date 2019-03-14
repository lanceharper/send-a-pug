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

let containerStyle =
  Style.(
    style([
      flex(1.0),
      backgroundColor(String("rgba(255, 255, 255, 0.1)")),
      position(Absolute),
      top(Pt(0.0)),
      bottom(Pt(0.0)),
      left(Pt(0.0)),
      right(Pt(0.0)),
      paddingLeft(Pt(20.)),
      justifyContent(Center),
    ])
  );

let leaderBoardRowStyle =
  Style.(
    style([flexDirection(Row), flex(0.5), justifyContent(SpaceAround)])
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
      variables=listLeadersQuery##variables pollInterval=15000>
      ...{({result}) =>
        <View style=containerStyle>
          {switch (result) {
           | Loading => <Text> {ReasonReact.string("Loading")} </Text>
           | Error(error) =>
             <Text> {ReasonReact.string(error##message)} </Text>
           | Data(response) =>
             switch (response##listLeaders) {
             | None => <Text> {"No Leaders" |> ReasonReact.string} </Text>
             | Some(data) =>
               Array.map(
                 leaderboardEntry =>
                   <View
                     key={leaderboardEntry##player} style=leaderBoardRowStyle>
                     <Text>
                       {leaderboardEntry##player |> ReasonReact.string}
                     </Text>
                     <Text>
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
        </View>
      }
    </GetLeaderboardQuery>;
  },
};