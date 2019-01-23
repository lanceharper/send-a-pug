[@bs.deriving abstract]
type twitchAuth = {
  clientId: string,
  token: string,
};

[@bs.scope ("Twitch", "ext")] [@bs.val]
external onAuthorized : (twitchAuth => unit) => unit = "onAuthorized";