open BsReactNative;

module PugCreated = [%graphql
  {|
    subscription JerkCreatedSubscription($channelId: String!) {
      inbox(to: $channelId) {
        from
        to
        message
        sentAt
      }
    }
  |}
];

module PugCreatedSubscription = ReasonApollo.CreateSubscription(PugCreated);

let component = ReasonReact.statelessComponent("PugListContainer");

let make = (~channelId, _children) => {
  ...component,
  render: _self => {
    let jerkSubcription = PugCreated.make(~channelId, ());

    <View
      style=Style.(
        style([
          position(Absolute),
          top(Pt(0.0)),
          left(Pt(0.0)),
          bottom(Pt(0.0)),
          right(Pt(0.0)),
        ])
      )>
      <PugCreatedSubscription variables=jerkSubcription##variables>
        ...{({result}) =>
          switch (result) {
          | Loading => <Text> {"loading" |> ReasonReact.string} </Text>
          | Error(_error) => <Text> {"error" |> ReasonReact.string} </Text>
          | Data(response) =>
            switch (response##inbox) {
            | None => <Text> {"No Message" |> ReasonReact.string} </Text>
            | Some(inboxResponse) => <PugList inboxResponse />
            }
          }
        }
      </PugCreatedSubscription>
    </View>;
  },
};