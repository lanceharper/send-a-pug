let component = ReasonReact.statelessComponent("Authenticated");

let make = (~token, ~displayName, ~channelId, _children) => {
  ...component,

  render: _self => {
    <ReasonApollo.Provider client={Client.instance(token)}>
      <PugListContainer channelId />
      <Leaderboard />
      <NewPug displayName channelId />
    </ReasonApollo.Provider>;
  },
};