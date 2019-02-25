open BsReactNative;
module JerkCreated = [%graphql
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

module JerkCreatedSubscription = ReasonApollo.CreateSubscription(JerkCreated);

let component = ReasonReact.statelessComponent("Authenticated");

let make = (~token, ~displayName, ~channelId, _children) => {
  ...component,

  render: _self => {
    let jerkSubcription = JerkCreated.make(~channelId, ());

    <ReasonApollo.Provider client={Client.instance(token)}>
      <NewJerk displayName channelId />
      <JerkCreatedSubscription variables=jerkSubcription##variables>
        ...{({result}) => {
          Js.log(result);
          switch (result) {
          | Loading => <Text> {"loading" |> ReasonReact.string} </Text>
          | Error(_error) => <Text> {"error" |> ReasonReact.string} </Text>
          | Data(response) =>
            switch (response##inbox) {
            | None => <Text> {"No Message" |> ReasonReact.string} </Text>
            | Some(inboxResponse) =>
              <Text> {inboxResponse##from |> ReasonReact.string} </Text>
            }
          };
        }}
      </JerkCreatedSubscription>
    </ReasonApollo.Provider>;
  },
};