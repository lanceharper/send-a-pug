open Bs_fetch;

let endpoint = "https://ga99jrhfdc.execute-api.us-west-2.amazonaws.com/dev/users/twitch-auth";

let requestUserPoolToken = twitchToken => {
  let payload = Js.Dict.empty();
  Js.Dict.set(payload, "twitchToken", Js.Json.string(twitchToken));

  Js.Promise.(
    Fetch.fetchWithInit(
      endpoint,
      Fetch.RequestInit.make(
        ~method_=Post,
        ~body=
          Fetch.BodyInit.make(Js.Json.stringify(Js.Json.object_(payload))),
        ~headers=Fetch.HeadersInit.make({"Content-Type": "application/json"}),
        (),
      ),
    )
    |> then_(Fetch.Response.json)
  );
};