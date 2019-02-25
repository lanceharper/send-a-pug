open BsReactNative;

type item = {
  clickedBy: string,
  animation: Animated.Value.t,
  startPosition: int,
};

type state = list(item);

type action =
  | Click(item)
  | Reset;

let component = ReasonReact.reducerComponent("JerkList");

let make = (~inboxResponse, _children) => {
  ...component,

  initialState: () => [],

  willReceiveProps: self => {
    [
      {
        clickedBy: inboxResponse##from,
        animation: Animated.Value.create(0.0),
        startPosition: Js.Math.random_int(100, 100),
      },
      ...self.state,
    ];
  },

  // //if (oldSelf.state |> lengt != newSelf.state.delay) {
  //   stop();
  //   newSelf.handle(start, ());
  // //},

  reducer: ((), _state) => ReasonReact.NoUpdate,

  render: self => {
    <View>
      <Text>
        {self.state |> List.length |> string_of_int |> ReasonReact.string}
      </Text>
    </View>;
  },
};