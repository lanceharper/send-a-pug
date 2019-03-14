open BsReactNative;

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
      minHeight(Pt(100.)),
      flex(1.0),
      backgroundColor(String("transparent")),
      paddingLeft(Pt(20.)),
      justifyContent(Center),
    ])
  );

let component = ReasonReact.statelessComponent("Leaderboard");

let make = _children => {
  ...component,
  render: _self => {
    let listLeadersQuery = GetLeaderboard.make(~streamerId="233205905", ());
    <GetLeaderboardQuery variables=listLeadersQuery##variables>
      ...{({result}) =>
        switch (result) {
        | Loading => <div> {ReasonReact.string("Loading")} </div>
        | Error(error) => <div> {ReasonReact.string(error##message)} </div>
        | Data(response) =>
          switch (response##listLeaders) {
          | None => <Text> {"No Message" |> ReasonReact.string} </Text>
          | Some(data) =>
            switch (data##items) {
            | None => <Text> {"No Message" |> ReasonReact.string} </Text>
            | Some(items) =>
              Array.map(
                leaderboardEntry =>
                  switch (leaderboardEntry) {
                  | Some(leaderboardEntry) =>
                    <Text>
                      {leaderboardEntry##player |> ReasonReact.string}
                    </Text>
                  | None => <Text> {"No Message" |> ReasonReact.string} </Text>
                  },
                items,
              )
              |> ReasonReact.array
            }
          }
        }
      }
    </GetLeaderboardQuery>;
  },
};